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

#ifndef OPTFRAME_LOCAL_SEARCH_HPP_
#define OPTFRAME_LOCAL_SEARCH_HPP_

#include <iostream>
#include <vector>

using namespace std;

#include "OptFrameComponent.hpp"
#include "ComponentBuilder.h"

#include "Solution.hpp"
#include "Population.hpp"
#include "Evaluation.hpp"

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class LocalSearch: public OptFrameComponent
{
   typedef vector<Evaluation<DS>*> FitnessValues;
   typedef const vector<const Evaluation<DS>*> ConstFitnessValues;

public:

   LocalSearch()
   {
   }

   virtual ~LocalSearch()
   {
   }

   // core methods

   // no-optimization
   Solution<R, ADS>& search(const Solution<R, ADS>& s, double timelimit = 100000000, double target_f = 0)
   {
      Solution<R, ADS>& s2 = s.clone();
      exec(s2, timelimit, target_f);
      return s2;
   }

   // optimizated version
   pair<Solution<R, ADS>&, Evaluation<DS>&>& search(const Solution<R, ADS>& s, const Evaluation<DS>& e, double timelimit = 100000000, double target_f = 0)
   {
      Solution<R, ADS>& s2 = s.clone();
      Evaluation<DS>& e2 = e.clone();
      exec(s2, e2, timelimit, target_f);
      return *new pair<Solution<R, ADS>&, Evaluation<DS>&> (s2, e2);
   }


   // core methods

   // 1
   virtual void exec(Solution<R, ADS>& s, double timelimit, double target_f) = 0;

   // 2
   virtual void exec(Solution<R, ADS>& s, Evaluation<DS>& e, double timelimit, double target_f) = 0;

   virtual bool compatible(string s)
   {
	   return ( s == idComponent() ) || ( OptFrameComponent::compatible(s) );
   }

   static string idComponent()
   {
	   stringstream ss;
	   ss << OptFrameComponent::idComponent() << "LocalSearch:";
	   return ss.str();
   }

   virtual string id() const
   {
      return idComponent();
   }

};


template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class LocalSearchBuilder : public ComponentBuilder<R, ADS, DS>
{
public:
	virtual ~LocalSearchBuilder()
	{
	}

	virtual LocalSearch<R, ADS, DS>* build(Scanner& scanner, HeuristicFactory<R, ADS, DS>& hf, string family = "") = 0;

	virtual OptFrameComponent* buildComponent(Scanner& scanner, HeuristicFactory<R, ADS, DS>& hf, string family = "")
	{
		return build(scanner, hf, family);
	}

	virtual vector<pair<string, string> > parameters() = 0;

	virtual bool canBuild(string) = 0;

	static string idComponent()
	{
		stringstream ss;
		ss << ComponentBuilder<R, ADS, DS>::idComponent() << "LocalSearch:";
		return ss.str();
	}

	virtual string id() const
	{
		return idComponent();
	}
};


namespace optframe
{

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class LocalSearchAction: public Action<R, ADS, DS>
{
public:

	virtual ~LocalSearchAction()
	{
	}

	virtual string usage()
	{
		return "OptFrame:LocalSearch idx   search   OptFrame:Solution idx  timelimit  target_f  [output_variable] => OptFrame:Solution\nOptFrame:LocalSearch idx   search_e   OptFrame:Solution idx   OptFrame:Evaluation idx   timelimit  target_f   [output_variable] => OptFrame:Solution";
	}

	virtual bool handleComponent(OptFrameComponent& component)
	{
		return component.compatible(LocalSearch<R, ADS, DS>::idComponent());
	}

	virtual bool handleAction(string action)
	{
		return (action == "search") || (action == "search_e");
	}

	virtual bool doAction(string content, HeuristicFactory<R, ADS, DS>& hf, map<string, string>& dictionary, map<string, vector<string> >& ldictionary)
	{
		//cout << "LocalSearch::doAction '" << content << "'" << endl;

		Scanner scanner(content);

		if (!scanner.hasNext())
			return false;

		LocalSearch<R, ADS, DS>* ls;
		hf.assign(ls, scanner.nextInt(), scanner.next());

		if (!ls)
			return false;

		if (!scanner.hasNext())
			return false;

		string action = scanner.next();

		if (!handleAction(action))
			return false;

		if (action == "search")
		{
			if (!scanner.hasNext())
				return false;

			Solution<R, ADS>* s;
			hf.assign(s, scanner.nextInt(), scanner.next());

			if (!s)
				return false;

			if (!scanner.hasNext())
				return false;

			double timelimit = scanner.nextDouble();

			if (!scanner.hasNext())
				return false;

			double target_f = scanner.nextDouble();

			Solution<R, ADS>& s2 = ls->search(*s, timelimit, target_f);

			return Action<R, ADS, DS>::addAndRegister(scanner, s2, hf, dictionary);
		}

		if (action == "search_e")
		{
			if (!scanner.hasNext())
				return false;

			Solution<R, ADS>* s;
			hf.assign(s, scanner.nextInt(), scanner.next());

			if (!s)
				return false;

			if (!scanner.hasNext())
				return false;

			Evaluation<DS>* e;
			hf.assign(e, scanner.nextInt(), scanner.next());

			if (!e)
				return false;

			if (!scanner.hasNext())
				return false;

			double timelimit = scanner.nextDouble();

			if (!scanner.hasNext())
				return false;

			double target_f = scanner.nextDouble();

			pair<Solution<R, ADS>&, Evaluation<DS>&>& p = ls->search(*s, *e, timelimit, target_f);

			Solution<R, ADS>& s2 = p.first;

			delete& p.second;
			delete& p;

			return Action<R, ADS, DS>::addAndRegister(scanner, s2, hf, dictionary);
		}

		// no action found!
		return false;
	}

};

}


#endif /* OPTFRAME_LOCAL_SEARCH_HPP_ */
