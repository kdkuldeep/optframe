// OptFrame - Optimization Framework

// Copyright (C) 2009, 2010, 2011
// http://optframe.sourceforge.net/
//
// This file is part of the OptFrame optimization framework. This framework
// is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License v3 as published by the
// Free Software Foundation.

// This framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License v3 for more details.

// You should have received a copy of the GNU Lesser General Public License v3
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#ifndef OPTFRAME_EVALUATOR_HPP_
#define OPTFRAME_EVALUATOR_HPP_

#include "Solution.hpp"
#include "ADSManager.hpp"
#include "Evaluation.hpp"
#include "Move.hpp"
#include "MoveCost.hpp"

#include <iostream>

#include "Direction.hpp"
#include "Action.hpp"

//#define OPTFRAME_EPSILON 0.0001

using namespace std;
using namespace scannerpp;

namespace optframe
{


//! \english The Evaluator class is responsible for the attribution of objective values for each Solution \endenglish \portuguese A classe Evaluator é responsável pela atribuição de valores objetivo para cada Solution \endportuguese

/*!
 \english
 The Evaluator class is responsible for the attribution of objective values for each Solution
 This is done by the method evaluate().
 \endenglish

 \portuguese
 A classe Evaluator é responsável pela atribuição de valores objetivo para cada Solution.
 Isto é feito através do método evaluate().
 \endportuguese
 */

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class Evaluator: public Direction<DS>
{
protected:
	bool allowCosts; // move.cost() is enabled or disabled for this Evaluator

public:

	Evaluator(bool _allowCosts = true) :
			allowCosts(_allowCosts)
	{
	}

	virtual ~Evaluator()
	{
	}

	bool getAllowCosts()
	{
		return allowCosts;
	}

	Evaluation<DS>& evaluate(const Solution<R, ADS>& s)
	{
		return evaluate(s.getR(), s.getADS());
	}

//protected:
public: // because of MultiEvaluator... otherwise, make it 'friend'

	virtual Evaluation<DS>& evaluate(const R& r) = 0;

	virtual Evaluation<DS>& evaluate(const R& r, const ADS&)
	{
		return evaluate(r);
	}

public:
	void evaluate(Evaluation<DS>& e, const Solution<R, ADS>& s)
	{
		evaluate(e, s.getR(), s.getADS());
	}

//protected:
public: // because of MultiEvaluator... otherwise, make it 'friend'
	virtual void evaluate(Evaluation<DS>& e, const R& r, const ADS& ads)
	{
		Evaluation<DS>& e1 = evaluate(r, ads);
		e = e1;
		delete &e1;
	}

public:

	// Apply movement considering a previous evaluation => Faster.
	// Update evaluation 'e'
	Move<R, ADS, DS>& applyMove(Evaluation<DS>& e, Move<R, ADS, DS>& m, Solution<R, ADS>& s)
	{
		Move<R, ADS, DS>* rev = m.apply(e, s);
		if(!rev)
		{
			cout << "Evaluator error(1)! Expected reverse move, but it is NULL! TODO: FIX" << endl;
			exit(1);
		}
		evaluate(e, s);
		return *rev;
	}

	// Apply movement without considering a previous evaluation => Slower.
	// Return new evaluation 'e'
	pair<Move<R, ADS, DS>&, Evaluation<DS>&>& applyMove(Move<R, ADS, DS>& m, Solution<R, ADS>& s)
	{
		Move<R, ADS, DS>* rev = m.apply(s);
		if(!rev)
		{
			cout << "Evaluator error(2)! Expected reverse move, but it is NULL! TODO: FIX" << endl;
			exit(1);
		}
		return *new pair<Move<R, ADS, DS>&, Evaluation<DS>&>(*rev, evaluate(s));
	}

	// Movement cost based on reevaluation of 'e'
	MoveCost& moveCost(Evaluation<DS>& e, Move<R, ADS, DS>& m, Solution<R, ADS>& s)
	{
		MoveCost* p = NULL;
		if (allowCosts)
			p = m.cost(e, s.getR(), s.getADS());

		// do not update 's' => much faster (using updateDelta)
		if (p)
			return *p;
		else // need to update 's' together with reevaluation of 'e' => little faster (doesn't use updateDelta, but do reevaluation)
		{
			Move<R, ADS, DS>& rev = applyMove(e, m, s);
			pair<double, double> e_end = make_pair(e.getObjFunction(), e.getInfMeasure());

			vector<pair<double, double> > alternatives(e.getAlternativeCosts().size());

			for (unsigned i = 0; i < alternatives.size(); i++)
			{
				alternatives[i].first = e.getAlternativeCosts()[i].first;
				alternatives[i].second = e.getAlternativeCosts()[i].second;
			}

			Move<R, ADS, DS>& ini = applyMove(e, rev, s);
			pair<double, double> e_ini = make_pair(e.getObjFunction(), e.getInfMeasure());

			for (unsigned i = 0; i < alternatives.size(); i++)
			{
				alternatives[i].first -= e.getAlternativeCosts()[i].first;
				alternatives[i].second -= e.getAlternativeCosts()[i].second;
			}

			delete &rev;
			delete &ini;

			p = new MoveCost(e_end.first - e_ini.first, e_end.second - e_end.second);
			p->setAlternativeCosts(alternatives);

			return *p;
		}
	}

	// Movement cost based on complete evaluation
	// USE ONLY FOR VALIDATION OF CODE! OTHERWISE, USE moveCost(e, m, s)
	MoveCost& moveCost(Move<R, ADS, DS>& m, Solution<R, ADS>& s)
	{
		pair<Move<R, ADS, DS>&, Evaluation<DS>&>& rev = applyMove(m, s);

		pair<Move<R, ADS, DS>&, Evaluation<DS>&>& ini = applyMove(rev.first, s);

		// Difference: new - original

		double obj = rev.second.getObjFunction() - ini.second.getObjFunction();
		double inf = rev.second.getInfMeasure() - ini.second.getInfMeasure();

		vector<pair<double, double> > alternatives(rev.second.getAlternativeCosts().size());

		for (unsigned i = 0; i < alternatives.size(); i++)
		{
			alternatives[i].first = rev.second.getAlternativeCosts()[i].first - ini.second.getAlternativeCosts()[i].first;
			alternatives[i].second = rev.second.getAlternativeCosts()[i].second - ini.second.getAlternativeCosts()[i].second;
		}

		MoveCost* p = new MoveCost(obj, inf);
		p->setAlternativeCosts(alternatives);

		delete &rev.first;
		delete &rev.second;
		delete &ini.first;
		delete &ini.second;

		delete &rev;
		delete &ini;

		return *p;
	}


	// ============ betterThan ===========

	using Direction<DS>::betterThan;

	//! abstract method betterThan: true when a < b for minimization problems; and true when a > b for maximization problems.
	/*!
	 betterThan is the method in OptFrame used to guide the search methods into the solution space.
	 with betterThan the search methods are able to compare good and poor solutions, in one of the two directions: minimization and maximization.
	 It must be implemented by the final user in one of these ways:
	 - for minimization problems, returns a < b;
	 - for maximization problems, returns a > b.
	 */
	virtual bool betterThan(double a, double b) = 0;

	virtual bool betterThan(const Solution<R, ADS>& s1, const Solution<R, ADS>& s2)
	{
		Evaluation<DS>& e1 = evaluate(s1);
		Evaluation<DS>& e2 = evaluate(s2);
		bool r = betterThan(e1, e2);
		delete &e1;
		delete &e2;
		return r;
	}

	// ============= Component ===============

	virtual bool compatible(string s)
	{
		return (s == idComponent()) || (Direction<DS>::compatible(s));
	}

	static string idComponent()
	{
		stringstream ss;
		ss << Component::idComponent() << ":Evaluator";
		return ss.str();
	}

	virtual string id() const
	{
		return idComponent();
	}

};


template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class EvaluatorAction: public Action<R, ADS, DS>
{
public:

	virtual ~EvaluatorAction()
	{
	}

	virtual string usage()
	{
		return "OptFrame:Evaluator idx  evaluate   OptFrame:Solution idx  [output_variable] => OptFrame:Evaluation";
	}

	virtual bool handleComponent(string type)
	{
		return Component::compareBase(Evaluator<R, ADS, DS>::idComponent(), type);
	}

	virtual bool handleComponent(Component& component)
	{
		return component.compatible(Evaluator<R, ADS, DS>::idComponent());
	}

	virtual bool handleAction(string action)
	{
		return (action == "evaluate") || (action == "betterThan") || (action == "betterOrEquals");
	}

	virtual bool doCast(string component, int id, string type, string variable, HeuristicFactory<R, ADS, DS>& hf, map<string, string>& d)
	{
		if(!handleComponent(type))
		{
			cout << "EvaluatorAction::doCast error: can't handle component type '" << type << " " << id << "'" << endl;
			return false;
		}

		Component* comp = hf.components[component].at(id);

		if(!comp)
		{
			cout << "EvaluatorAction::doCast error: NULL component '" << component << " " << id << "'" << endl;
			return false;
		}

		if(!Component::compareBase(comp->id(), type))
		{
			cout << "EvaluatorAction::doCast error: component '" << comp->id() << " is not base of " << type << "'" << endl;
			return false;
		}

		// remove old component from factory
		hf.components[component].at(id) = NULL;

		// cast object to lower type
		Component* final = NULL;

		if(type == Evaluator<R, ADS, DS>::idComponent())
		{
			final = (Evaluator<R, ADS, DS>*) comp;
		}
		else
		{
			cout << "EvaluatorAction::doCast error: no cast for type '" << type << "'" << endl;
			return false;
		}

		// add new component
		Scanner scanner(variable);
		return ComponentAction<R, ADS, DS>::addAndRegister(scanner, *final, hf, d);
	}

	virtual bool doAction(string content, HeuristicFactory<R, ADS, DS>& hf, map<string, string>& dictionary, map<string, vector<string> >& ldictionary)
	{
		//cout << "Evaluator::doAction '" << content << "'" << endl;

		Scanner scanner(content);

		if (!scanner.hasNext())
			return false;

		Evaluator<R, ADS, DS>* ev;
		hf.assign(ev, scanner.nextInt(), scanner.next());

		if (!ev)
			return false;

		if (!scanner.hasNext())
			return false;

		string action = scanner.next();

		if (!handleAction(action))
			return false;

		if (action == "evaluate")
		{
			if (!scanner.hasNext())
				return false;

			Solution<R, ADS>* s;
			hf.assign(s, scanner.nextInt(), scanner.next());

			if (!s)
				return false;

			Evaluation<DS>& e = ev->evaluate(*s);

			return Action<R, ADS, DS>::addAndRegister(scanner, e, hf, dictionary);
		}

		// no action found!
		return false;
	}

};

}

#endif /*OPTFRAME_EVALUATOR_HPP_*/
