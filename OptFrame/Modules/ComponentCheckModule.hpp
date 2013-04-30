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

#ifndef CHECKMODULE_HPP_
#define CHECKMODULE_HPP_

#include "../OptFrameModule.hpp"
#include "../Util/Timer.hpp"

#include "SystemRequireModule.hpp"

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class ComponentCheckModule: public OptFrameModule<R, ADS, DS>
{
private:
	bool verbose;

public:

	virtual ~ComponentCheckModule()
	{
	}

	string id()
	{
		return "component.check";
	}

	string usage()
	{
		return "component.check iterMax nSolNSSeq verbose=false [ OptFrame:Constructive[] OptFrame:Evaluator[] OptFrame:Move[] OptFrame:NS[] OptFrame:NS:NSSeq[] OptFrame:NS:NSSeq:NSEnum[] OptFrame:UpdateADS[] ]";
	}

	void message(string component, int iter, string text)
	{
		if (verbose)
			cout << "module " << id() << " iter: " << iter << " testing component '" << component << "' => " << text << endl;
	}

	void error(string text)
	{
		cout << "module " << id() << " error: " << text << endl;
	}

	bool parseBool(string b)
	{
		return b == "true";
	}

	bool run(vector<OptFrameModule<R, ADS, DS>*>& allModules, vector<OptFrameFunction*>& allFunctions, HeuristicFactory<R, ADS, DS>& factory, map<string, string>& dictionary, map<string, vector<string> >& ldictionary, string input)
	{
		cout << "check: " << input << endl;
		Scanner scanner(input);

		// -------------------
		//     iterMax
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}

		int iterMax;
		string sIterMax = scanner.next();
		try
		{
			iterMax = Scanner::parseInt(sIterMax);
		} catch (ConversionError& e)
		{
			cout << "component.check module error: no such integer 'iterMax' (number of solutions to be build and moves to be tested per solution)" << endl;
			return false;
		}

		// -----------------------------------------
		//  number of solutions to apply NSSeq tests
		// -----------------------------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		int nSolNSSeq;
		string sNSolNSSeq = scanner.next();
		try
		{
			nSolNSSeq = Scanner::parseInt(sNSolNSSeq);
		} catch (ConversionError& e)
		{
			cout << "component.check module error: no such integer 'number of solutions to test NSSeq exploration'" << endl;
			return false;
		}

		// -------------------
		//     verbose
		// -------------------

		if (!scanner.hasNext())
			verbose = false;
		else
		{
			string sverbose = scanner.next();
			verbose = parseBool(sverbose);
		}

		// optional parameters taken automatically from the system
		if (!scanner.hasNext())
		{
			// check dependency on 'module.rename' module
			if (!OptFrameModule<R, ADS, DS>::run_module("system.require", allModules, allFunctions, factory, dictionary, ldictionary, "component.list"))
			{
				cout << "error: system.use module depends on 'component.list' module, which is not loaded!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lconstructive", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:Constructive _aux_check_lconstructive"))
			{
				cout << "error: reading list of OptFrame:Constructive!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_levaluator", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:Evaluator _aux_check_levaluator"))
			{
				cout << "error: reading list of OptFrame:Evaluator!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lmove", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:Move _aux_check_lmove"))
			{
				cout << "error: reading list of OptFrame:Move!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lns", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:NS _aux_check_lns"))
			{
				cout << "error: reading list of OptFrame:NS!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lnsseq", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:NS:NSSeq _aux_check_lnsseq"))
			{
				cout << "error: reading list of OptFrame:NS:NSSeq!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lnsenum", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:NS:NSSeq:NSEnum _aux_check_lnsenum"))
			{
				cout << "error: reading list of OptFrame:NS:NSSeq:NSEnum!" << endl;
				return false;
			}

			OptFrameModule<R, ADS, DS>::undefine("_aux_check_lupdateads", dictionary, ldictionary);
			if (!OptFrameModule<R, ADS, DS>::run_module("component.list", allModules, allFunctions, factory, dictionary, ldictionary, "OptFrame:UpdateADS _aux_check_lupdateads"))
			{
				cout << "error: reading list of OptFrame:UpdateADS!" << endl;
				return false;
			}


			scanner = Scanner("_aux_check_lconstructive  _aux_check_levaluator  _aux_check_lmove  _aux_check_lns  _aux_check_lnsseq  _aux_check_lnsenum   _aux_check_lupdateads");
		}

		//string rest = scanner.rest();
		//cout << "REST: '" << rest << "'" << endl;
		//scanner = Scanner(rest);

		// -------------------
		//    Constructive
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lConstructive;
		vector<string>* p_lConstructive = OptFrameList::readList(ldictionary, scanner);
		if (p_lConstructive)
		{
			lConstructive = vector<string>(*p_lConstructive);
			delete p_lConstructive;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:Constructive!" << endl;
			return false;
		}

		// -------------------
		//     Evaluator
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lEvaluator;
		vector<string>* p_lEvaluator = OptFrameList::readList(ldictionary, scanner);
		if (p_lEvaluator)
		{
			lEvaluator = vector<string>(*p_lEvaluator);
			delete p_lEvaluator;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:Evaluator!" << endl;
			return false;
		}

		// -------------------
		//        Move
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lMove;
		vector<string>* p_lMove = OptFrameList::readList(ldictionary, scanner);
		if (p_lMove)
		{
			lMove = vector<string>(*p_lMove);
			delete p_lMove;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:Move!" << endl;
			return false;
		}

		// -------------------
		//        NS
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lNS;
		vector<string>* p_lNS = OptFrameList::readList(ldictionary, scanner);
		if (p_lNS)
		{
			lNS = vector<string>(*p_lNS);
			delete p_lNS;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:NS!" << endl;
			return false;
		}

		// -------------------
		//     NSSeq
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lNSSeq;
		vector<string>* p_lNSSeq = OptFrameList::readList(ldictionary, scanner);
		if (p_lNSSeq)
		{
			lNSSeq = vector<string>(*p_lNSSeq);
			delete p_lNSSeq;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:NS:NSSeq!" << endl;
			return false;
		}

		// -------------------
		//     NSEnum
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lNSEnum;
		vector<string>* p_lNSEnum = OptFrameList::readList(ldictionary, scanner);
		if (p_lNSEnum)
		{
			lNSEnum = vector<string>(*p_lNSEnum);
			delete p_lNSEnum;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:NS:NSSeq:NSEnum!" << endl;
			return false;
		}

		// -------------------
		//     UpdateADS
		// -------------------

		if (!scanner.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}
		vector<string> lUpdateADS;
		vector<string>* p_lUpdateADS = OptFrameList::readList(ldictionary, scanner);
		if (p_lUpdateADS)
		{
			lUpdateADS = vector<string>(*p_lUpdateADS);
			delete p_lUpdateADS;
		}
		else
		{
			cout << "module " << id() << " error: couldn't read list of OptFrame:UpdateADS!" << endl;
			return false;
		}

		// cleanup auxiliar list definitions
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lconstructive", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_levaluator", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lmove", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lns", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lnsseq", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lnsenum", dictionary, ldictionary);
		OptFrameModule<R, ADS, DS>::undefine("_aux_check_lupdateads", dictionary, ldictionary);

		// ======================================
		//           BEGIN TESTS
		// ======================================

		UpdateADS<R, ADS>* updateADS;
		if (lUpdateADS.size() > 0)
		{
			Scanner scan(lUpdateADS.at(0));
			factory.assign(updateADS, scan.nextInt(), scan.next()); // reversed!

			if (lUpdateADS.size() > 1)
				cout << id() << " module warning: more than 1 UpdateADS (" << lUpdateADS.size() << ")" << endl;
		}

		vector<pair<int, double> > timeInitializeADS(1, make_pair(0, 0.0));

		// ----------------
		// read evaluators
		// ----------------

		vector<Evaluator<R, ADS, DS>*> evaluators;
		for (unsigned ev = 0; ev < lEvaluator.size(); ev++)
		{
			Scanner scan(lEvaluator.at(ev));
			Evaluator<R, ADS, DS>* evaluator;
			factory.assign(evaluator, scan.nextInt(), scan.next()); // reversed!

			if (!evaluator)
			{
				cout << "module " << id() << " error: NULL evaluator!" << endl;
				return false;
			}

			evaluators.push_back(evaluator);
		}

		vector<pair<int, double> > fullTimeEval(evaluators.size(), make_pair(0, 0.0));
		vector<pair<int, double> > timeReeval(evaluators.size(), make_pair(0, 0.0));

		// ----------------------------------------------------------------------------------------
		// generate 'iterMax' OptFrame:Solution for each OptFrame:Constructive and store evaluation
		// ----------------------------------------------------------------------------------------

		vector<Solution<R, ADS>*> solutions;
		vector<vector<Evaluation<DS>*> > evaluations(evaluators.size());

		vector<pair<int, double> > timeConstructive(lConstructive.size(), make_pair(0, 0.0));

		cout << "module " << id() << " will test constructive components (iterMax=" << iterMax << ")" << endl;
		for (unsigned c = 0; c < lConstructive.size(); c++)
		{
			Scanner scan(lConstructive.at(c));
			Constructive<R, ADS>* constructive;
			factory.assign(constructive, scan.nextInt(), scan.next()); // reversed!

			if (!constructive)
			{
				cout << "module " << id() << " error: NULL constructive!" << endl;
				return false;
			}

			for (int iter = 1; iter <= iterMax; iter++)
			{
				message(lConstructive.at(c), iter, "generating solution.");

				Timer ts;
				Solution<R, ADS>& s = constructive->generateSolution();
				timeConstructive[c].second += ts.inMilliSecs();
				timeConstructive[c].first++;

				if (updateADS)
				{
					Timer ts2;
					updateADS->initializeADS(s.getR(), s.getADS());
					timeInitializeADS[0].second += ts2.inMilliSecs();
					timeInitializeADS[0].first++;
				}

				solutions.push_back(&s);

				for (unsigned ev = 0; ev < evaluators.size(); ev++)
				{
					message(lEvaluator.at(ev), iter, "evaluating solution.");
					Timer te;
					Evaluation<DS>& e = evaluators.at(ev)->evaluate(s);
					fullTimeEval[ev].second += te.inMilliSecs();
					fullTimeEval[ev].first++;

					evaluations.at(ev).push_back(&e);
				}
			}

			cout << "component.check: " << lConstructive.at(c) << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		if (verbose)
			cout << endl << endl;

		// ====================================================================
		// testing Move
		// ====================================================================

		cout << "module " << id() << " will test given Move components (|Move|=" << lMove.size() << "; numSolutions=" << solutions.size() << ")" << endl;

		for (unsigned id_move = 0; id_move < lMove.size(); id_move++)
		{
			Scanner scan(lMove.at(id_move));
			Move<R, ADS, DS>* pmove;
			factory.assign(pmove, scan.nextInt(), scan.next()); // reversed!

			if (!pmove)
			{
				cout << "module " << id() << " error: NULL OptFrame:Move!" << endl;
				return false;
			}

			for (unsigned id_s = 0; id_s < solutions.size(); id_s++)
			{
				if (verbose)
					cout << endl;
				message(lMove.at(id_move), -1, "working on move.");

				Solution<R, ADS>& s = *solutions.at(id_s);

				Move<R, ADS, DS>& move = *pmove;

				if (!move.canBeApplied(s))
				{
					if (verbose)
					{
						cout << "move cannot be applied: ";
						move.print();
					}
					continue;
				}

				for (unsigned ev = 0; ev < evaluators.size(); ev++)
				{
					message(lEvaluator.at(ev), -1, "evaluating move (apply, revert and moveCost).");

					string moveFrom = "Move ";
					moveFrom.append(move.id());

					if (verbose)
						move.print();

					message(moveFrom, -1, "testing reverse.");

					Move<R, ADS, DS>& rev = move.apply(s);
					Solution<R, ADS>& sNeighbor = s.clone(); // remove if not verbose

					Evaluation<DS>& e_rev = evaluators.at(ev)->evaluate(s);

					Move<R, ADS, DS>& ini = rev.apply(s);

					Evaluation<DS>& e_ini = evaluators.at(ev)->evaluate(s);

					if (ini != move)
					{
						error("reverse of reverse is not the original move!");
						move.print();
						cout << "move: ";
						move.print();
						cout << "rev: ";
						rev.print();
						cout << "ini (reverse of rev): ";
						ini.print();

						return false;
					}

					message(lEvaluator.at(ev), -1, "testing reverse value.");
					Evaluation<DS>& e = *evaluations.at(ev).at(id_s);

					if (abs(e_ini.evaluation() - e.evaluation()) > 0.0001)
					{
						error("reverse of reverse has a different evaluation value!");
						move.print();
						cout << "move: ";
						move.print();
						cout << "original: ";
						e.print();
						cout << "reverse of reverse:";
						e_ini.print();

						return false;
					}

					// ===================== tests with UpdateADS ======================

					if (updateADS)
					{
						message(lEvaluator.at(ev), -1, "testing ADS.");

						if (!updateADS->compareADS(s.getADS(), sNeighbor.getADS()))
						{
							cout << id() << " module error: ADS not updated correctly! Compared apply and reverse for move => ";
							move.print();
							return false;
						}

						ADS ads(sNeighbor.getADS()); // copy
						Timer ts_ds;
						updateADS->initializeADS(sNeighbor.getR(), sNeighbor.getADS());
						timeInitializeADS[0].second += ts_ds.inMilliSecs();
						timeInitializeADS[0].first++;

						if (!updateADS->compareADS(ads, sNeighbor.getADS()))
						{
							cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from move => ";
							move.print();
							return false;
						}

						ads = ADS(s.getADS()); // copy
						Timer ts_ds2;
						updateADS->initializeADS(s.getR(), s.getADS());
						timeInitializeADS[0].second += ts_ds2.inMilliSecs();
						timeInitializeADS[0].first++;

						if (!updateADS->compareADS(ads, s.getADS()))
						{
							cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from reverse move => ";
							rev.print();
							return false;
						}
					}

					// =============================================================


					message(lEvaluator.at(ev), -1, "testing move cost.");

					double revCost = e_rev.evaluation() - e.evaluation();

					double simpleCost = evaluators[ev]->moveCost(move, s);

					if (abs(revCost - simpleCost) > 0.0001)
					{
						error("difference between revCost and simpleCost");
						move.print();
						printf("revCost = %.4f\n", revCost);
						printf("simpleCost = %.4f\n", simpleCost);
						return false;
					}

					// fasterCost
					Move<R, ADS, DS>& rev1 = evaluators[ev]->applyMove(e, move, s);
					double e_end1 = e.evaluation();
					Move<R, ADS, DS>& ini1 = evaluators[ev]->applyMove(e, rev1, s);
					double e_ini1 = e.evaluation();

					delete &rev1;
					delete &ini1;

					double fasterCost = e_end1 - e_ini1;

					if (abs(revCost - fasterCost) > 0.0001)
					{
						error("difference between revCost and fasterCost");
						move.print();
						printf("revCost = %.4f\n", revCost);
						printf("fasterCost = %.4f\n", fasterCost);
						printf("e = %.4f\n", e.evaluation());
						printf("e_rev = %.4f\n", e_rev.evaluation());
						return false;
					}

					pair<double, double>* cost = NULL;

					if (evaluators[ev]->getAllowCosts())
						cost = move.cost(e, s.getR(), s.getADS());

					if (cost)
					{
						double cValue = cost->first + cost->second;
						if (abs(revCost - cValue) > 0.0001)
						{
							error("difference between expected cost and cost()");
							move.print();
							printf("expected =\t %.4f\n", revCost);
							printf("cost() =\t %.4f\n", cValue);
							printf("==============\n");
							printf("CORRECT VALUES \n");
							printf("==============\n");
							printf("e: \t obj:%.4f \t inf:%.4f \t total:%.4f\n", e.getObjFunction(), e.getInfMeasure(), e.evaluation());
							printf("e':\t obj:%.4f \t inf:%.4f \t total:%.4f\n", e_rev.getObjFunction(), e_rev.getInfMeasure(), e_rev.evaluation());
							cout << "s: ";
							s.print();
							cout << "s': ";
							sNeighbor.print();
							cout << "move: ";
							move.print();
							printf("==============\n");
							printf("  GOOD LUCK!  \n");
							printf("==============\n");
							return false;
						}

						delete cost;
					}

					message(lEvaluator.at(ev), -1, "all move costs okay!");

					delete &rev;
					delete &sNeighbor;
					delete &e_rev;
					delete &ini;
					delete &e_ini;
				}

				/////delete& move; // NEVER DESTROY THIS OptFrame:Move!
			}

		}

		if (verbose)
			cout << endl << endl;

		// ====================================================================
		// testing NS
		// ====================================================================

		cout << "module " << id() << " will test NS components (iterMax=" << iterMax << "; numSolutions=" << solutions.size() << ")" << endl;

		vector<pair<int, double> > timeNSApply(lNS.size(), make_pair(0, 0.0));
		vector<pair<int, double> > timeNSCostApply(lNS.size(), make_pair(0, 0.0));
		vector<pair<int, double> > timeNSCostApplyDelta(lNS.size(), make_pair(0, 0.0));
		vector<pair<int, double> > timeNSCost(lNS.size(), make_pair(0, 0.0));
		vector<pair<int, double> > timeNSEstimatedCost(lNS.size(), make_pair(0, 0.0));
		vector<pair<int, double> > errorNSEstimatedCost(lNS.size(), make_pair(0, 0.0));

		for (unsigned id_ns = 0; id_ns < lNS.size(); id_ns++)
		{
			Scanner scan(lNS.at(id_ns));
			NS<R, ADS, DS>* ns;
			factory.assign(ns, scan.nextInt(), scan.next()); // reversed!

			if (!ns)
			{
				cout << "module " << id() << " error: NULL OptFrame:NS!" << endl;
				return false;
			}

			for (int iter = 1; iter <= iterMax; iter++)
			{
				message(lNS.at(id_ns), iter, "starting tests!");

				for (unsigned id_s = 0; id_s < solutions.size(); id_s++)
				{
					if (verbose)
						cout << endl;

					stringstream ss_msg1;
					ss_msg1 << "generating random move for solution id=" << id_s;
					message(lNS.at(id_ns), iter, ss_msg1.str());

					Solution<R, ADS>& s = *solutions.at(id_s);

					Move<R, ADS, DS>& move = ns->move(s);

					if (!move.canBeApplied(s))
					{
						if (verbose)
						{
							cout << "move cannot be applied: ";
							move.print();
						}
						continue;
					}

					for (unsigned ev = 0; ev < evaluators.size(); ev++)
					{
						message(lEvaluator.at(ev), iter, "evaluating random move (apply, revert and moveCost).");

						string moveFrom = "Move ";
						moveFrom.append(move.id());
						moveFrom.append(" from: ");
						moveFrom.append(ns->id());
						moveFrom.append(" toString: ");
						moveFrom.append(ns->toString());

						if (verbose)
							move.print();

						message(moveFrom, iter, "testing reverse.");

						Timer tMovApply;
						Move<R, ADS, DS>& rev = move.apply(s);
						timeNSApply[id_ns].second += tMovApply.inMilliSecs();
						timeNSApply[id_ns].first++;

						Solution<R, ADS>& sNeighbor = s.clone(); // remove if not verbose

						Timer te;
						Evaluation<DS>& e_rev = evaluators.at(ev)->evaluate(s);
						fullTimeEval[ev].second += te.inMilliSecs();
						fullTimeEval[ev].first++;

						Timer tMovRevApply;
						Move<R, ADS, DS>& ini = rev.apply(s);
						timeNSApply[id_ns].second += tMovRevApply.inMilliSecs();
						timeNSApply[id_ns].first++;

						Timer te2;
						Evaluation<DS>& e_ini = evaluators.at(ev)->evaluate(s);
						fullTimeEval[ev].second += te2.inMilliSecs();
						fullTimeEval[ev].first++;

						if (ini != move)
						{
							error("reverse of reverse is not the original move!");
							move.print();
							cout << "move: ";
							move.print();
							cout << "rev: ";
							rev.print();
							cout << "ini (reverse of rev): ";
							ini.print();

							return false;
						}

						message(lEvaluator.at(ev), iter, "testing reverse value.");
						Evaluation<DS>& e = *evaluations.at(ev).at(id_s);

						if (abs(e_ini.evaluation() - e.evaluation()) > 0.0001)
						{
							error("reverse of reverse has a different evaluation value!");
							move.print();
							cout << "move: ";
							move.print();
							cout << "original: ";
							e.print();
							cout << "reverse of reverse:";
							e_ini.print();

							return false;
						}


						// ===================== tests with UpdateADS ======================

						if (updateADS)
						{
							message(lEvaluator.at(ev), -1, "testing ADS.");

							if (!updateADS->compareADS(s.getADS(), sNeighbor.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared apply and reverse for move => ";
								move.print();
								return false;
							}

							ADS ads(sNeighbor.getADS()); // copy
							Timer ts_ds;
							updateADS->initializeADS(sNeighbor.getR(), sNeighbor.getADS());
							timeInitializeADS[0].second += ts_ds.inMilliSecs();
							timeInitializeADS[0].first++;

							if (!updateADS->compareADS(ads, sNeighbor.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from move => ";
								move.print();
								return false;
							}

							ads = ADS(s.getADS()); // copy
							Timer ts_ds2;
							updateADS->initializeADS(s.getR(), s.getADS());
							timeInitializeADS[0].second += ts_ds2.inMilliSecs();
							timeInitializeADS[0].first++;

							if (!updateADS->compareADS(ads, s.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from reverse move => ";
								rev.print();
								return false;
							}
						}

						// =============================================================


						message(lEvaluator.at(ev), iter, "testing move cost.");

						double revCost = e_rev.evaluation() - e.evaluation();
						message(lEvaluator.at(ev), iter, "revCost calculated!");

						Timer tMoveCostApply;
						double simpleCost = evaluators[ev]->moveCost(move, s);
						message(lEvaluator.at(ev), iter, "simpleCost calculated!");
						timeNSCostApply[id_ns].second += tMoveCostApply.inMilliSecs();
						timeNSCostApply[id_ns].first++;

						if (abs(revCost - simpleCost) > 0.0001)
						{
							error("difference between revCost and simpleCost");
							move.print();
							printf("revCost = %.4f\n", revCost);
							printf("simpleCost = %.4f\n", simpleCost);
							return false;
						}

						// fasterCost
						Timer tMoveCostApplyDelta;
						Move<R, ADS, DS>& rev1 = evaluators[ev]->applyMove(e, move, s);
						double e_end1 = e.evaluation();
						Move<R, ADS, DS>& ini1 = evaluators[ev]->applyMove(e, rev1, s);
						double e_ini1 = e.evaluation();
						timeNSCostApplyDelta[id_ns].second += tMoveCostApplyDelta.inMilliSecs();
						timeNSCostApplyDelta[id_ns].first++;

						delete &rev1;
						delete &ini1;

						double fasterCost = e_end1 - e_ini1;
						message(lEvaluator.at(ev), iter, "fasterCost calculated!");

						if (abs(revCost - fasterCost) > 0.0001)
						{
							error("difference between revCost and fasterCost");
							move.print();
							printf("revCost = %.4f\n", revCost);
							printf("fasterCost = %.4f\n", fasterCost);
							printf("e = %.4f\n", e.evaluation());
							printf("e_rev = %.4f\n", e_rev.evaluation());
							return false;
						}

						Timer tMoveEstimatedCost;
						pair<double, double>* estimatedCost = NULL;

						if (evaluators[ev]->getAllowCosts())
							estimatedCost = move.estimatedCost(e, s.getR(), s.getADS());
						message(lEvaluator.at(ev), iter, "estimatedCost calculated!");

						if (estimatedCost)
						{
							timeNSEstimatedCost[id_ns].second += tMoveEstimatedCost.inMilliSecs();
							timeNSEstimatedCost[id_ns].first++;

							double cValue = estimatedCost->first + estimatedCost->second;
							double diff = abs(revCost - cValue);
							double ref = abs(max(cValue, revCost)); // reference can be max or min! Whatever...
							if (ref < 0.0001)
								ref = 0.0001;

							errorNSEstimatedCost[id_ns].second += (diff / ref) * 100; // in percentage
							errorNSEstimatedCost[id_ns].first++;

							delete estimatedCost;
						}


						Timer tMoveCost;
						pair<double, double>* cost = NULL;

						if (evaluators[ev]->getAllowCosts())
							cost = move.cost(e, s.getR(), s.getADS());

						message(lEvaluator.at(ev), iter, "cost() calculated!");

						if (cost)
						{
							timeNSCost[id_ns].second += tMoveCost.inMilliSecs();
							timeNSCost[id_ns].first++;
						}

						if (cost)
						{
							double cValue = cost->first + cost->second;
							if (abs(revCost - cValue) > 0.0001)
							{
								error("difference between expected cost and cost()");
								move.print();
								printf("expected =\t %.4f\n", revCost);
								printf("cost() =\t %.4f\n", cValue);
								printf("==============\n");
								printf("CORRECT VALUES \n");
								printf("==============\n");
								printf("e: \t obj:%.4f \t inf:%.4f \t total:%.4f\n", e.getObjFunction(), e.getInfMeasure(), e.evaluation());
								printf("e':\t obj:%.4f \t inf:%.4f \t total:%.4f\n", e_rev.getObjFunction(), e_rev.getInfMeasure(), e_rev.evaluation());
								cout << "s: ";
								s.print();
								cout << "s': ";
								sNeighbor.print();
								cout << "move: ";
								move.print();
								printf("==============\n");
								printf("  GOOD LUCK!  \n");
								printf("==============\n");
								return false;
							}

							delete cost;
						}

						message(lEvaluator.at(ev), iter, "all move costs okay!");

						delete &rev;
						delete &sNeighbor;
						delete &e_rev;
						delete &ini;
						delete &e_ini;
					}

					delete &move;
				}
			}

			cout << "component.check: " << lNS.at(id_ns) << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		// ====================================================================
		// testing NSSeq
		// ====================================================================

		cout << "module " << id() << " will test NSSeq components (nSolNSSeq=" << nSolNSSeq << " of numSolutions=" << solutions.size() << ")" << endl;

		vector<int> vCountMoves(lNSSeq.size());
		vector<int> vCountValidMoves(lNSSeq.size());

		for (unsigned id_nsseq = 0; id_nsseq < lNSSeq.size(); id_nsseq++)
		{
			Scanner scan(lNSSeq.at(id_nsseq));
			NSSeq<R, ADS, DS>* nsseq;
			factory.assign(nsseq, scan.nextInt(), scan.next()); // reversed!

			if (!nsseq)
			{
				cout << "module " << id() << " error: NULL OptFrame:NS:NSSeq!" << endl;
				return false;
			}

			int countMoves = 0;
			int countValidMoves = 0;

			for (int nqs = 1; nqs <= nSolNSSeq; nqs++)
			{
				message(lNSSeq.at(id_nsseq), nqs, "starting tests!");

				int randomIndex = rand() % solutions.size();
				Solution<R, ADS>& s = *solutions.at(randomIndex);

				// ===================
				// apply a single move
				// ===================

				bool moveApplied = false;
				for(int i=0; i<iterMax; i++)
				{
					Move<R, ADS, DS>& move = nsseq->move(s);

					if(move.canBeApplied(s))
					{
						delete &move.apply(s);
						delete &move;
						moveApplied = true;
						break;
					}
					else
						delete& move;
				}

				if(!moveApplied)
					message(lNSSeq.at(id_nsseq), nqs, "Warning. Couldn't apply a move before iterator tests (NSSeq tests).");

				NSIterator<R, ADS, DS>& it = nsseq->getIterator(s);

				for (it.first(); !it.isDone(); it.next())
				{
					if (verbose)
						cout << endl;
					message(lNSSeq.at(id_nsseq), nqs, "getting current move (NSSeq tests).");

					Move<R, ADS, DS>& move = it.current();
					countMoves++;

					if (!move.canBeApplied(s))
					{
						if (verbose)
						{
							cout << "move cannot be applied (NSSeq tests): ";
							move.print();
						}
						continue;
					}

					countValidMoves++;

					for (unsigned ev = 0; ev < evaluators.size(); ev++)
					{
						message(lEvaluator.at(ev), nqs, "evaluating random move (apply, revert and moveCost) in NSSeq tests.");

						Evaluation<DS>& e = evaluators[ev]->evaluate(s);

						string moveFrom = "Move ";
						moveFrom.append(move.id());
						moveFrom.append(" from: ");
						moveFrom.append(nsseq->id());
						moveFrom.append(" toString: ");
						moveFrom.append(nsseq->toString());

						if (verbose)
							move.print();

						message(moveFrom, nqs, "testing reverse.");

						Timer tMovApply;
						Move<R, ADS, DS>& rev = move.apply(s);
						timeNSApply[id_nsseq].second += tMovApply.inMilliSecs();
						timeNSApply[id_nsseq].first++;

						Solution<R, ADS>& sNeighbor = s.clone(); // remove if not verbose

						Timer te;
						Evaluation<DS>& e_rev = evaluators.at(ev)->evaluate(s);
						fullTimeEval[ev].second += te.inMilliSecs();
						fullTimeEval[ev].first++;

						Timer tMovRevApply;
						Move<R, ADS, DS>& ini = rev.apply(s);
						timeNSApply[id_nsseq].second += tMovRevApply.inMilliSecs();
						timeNSApply[id_nsseq].first++;

						Timer te2;
						Evaluation<DS>& e_ini = evaluators.at(ev)->evaluate(s);
						fullTimeEval[ev].second += te2.inMilliSecs();
						fullTimeEval[ev].first++;

						if (ini != move)
						{
							error("reverse of reverse is not the original move!");
							move.print();
							cout << "move: ";
							move.print();
							cout << "rev: ";
							rev.print();
							cout << "ini (reverse of rev): ";
							ini.print();

							return false;
						}

						message(lEvaluator.at(ev), nqs, "testing reverse value (NSSeq tests).");

						if (abs(e_ini.evaluation() - e.evaluation()) > 0.0001)
						{
							error("reverse of reverse has a different evaluation value!");
							move.print();
							cout << "move: ";
							move.print();
							cout << "original: ";
							e.print();
							cout << "reverse of reverse:";
							e_ini.print();

							return false;
						}


						// ===================== tests with UpdateADS ======================

						if (updateADS)
						{
							message(lEvaluator.at(ev), -1, "testing ADS (NSSeq tests).");

							if (!updateADS->compareADS(s.getADS(), sNeighbor.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared apply and reverse for move => ";
								move.print();
								return false;
							}

							ADS ads(sNeighbor.getADS()); // copy
							Timer ts_ds;
							updateADS->initializeADS(sNeighbor.getR(), sNeighbor.getADS());
							timeInitializeADS[0].second += ts_ds.inMilliSecs();
							timeInitializeADS[0].first++;

							if (!updateADS->compareADS(ads, sNeighbor.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from move => ";
								move.print();
								return false;
							}

							ads = ADS(s.getADS()); // copy
							Timer ts_ds2;
							updateADS->initializeADS(s.getR(), s.getADS());
							timeInitializeADS[0].second += ts_ds2.inMilliSecs();
							timeInitializeADS[0].first++;

							if (!updateADS->compareADS(ads, s.getADS()))
							{
								cout << id() << " module error: ADS not updated correctly! Compared brand new initializeADS with update from reverse move => ";
								rev.print();
								return false;
							}
						}

						// =============================================================


						message(lEvaluator.at(ev), nqs, "testing move cost (NSSeq tests).");

						double revCost = e_rev.evaluation() - e.evaluation();

						Timer tMoveCostApply;
						double simpleCost = evaluators[ev]->moveCost(move, s);
						timeNSCostApply[id_nsseq].second += tMoveCostApply.inMilliSecs();
						timeNSCostApply[id_nsseq].first++;

						if (abs(revCost - simpleCost) > 0.0001)
						{
							error("difference between revCost and simpleCost");
							move.print();
							printf("revCost = %.4f\n", revCost);
							printf("simpleCost = %.4f\n", simpleCost);
							return false;
						}

						// fasterCost
						Timer tMoveCostApplyDelta;
						Move<R, ADS, DS>& rev1 = evaluators[ev]->applyMove(e, move, s);
						double e_end1 = e.evaluation();
						Move<R, ADS, DS>& ini1 = evaluators[ev]->applyMove(e, rev1, s);
						double e_ini1 = e.evaluation();
						timeNSCostApplyDelta[id_nsseq].second += tMoveCostApplyDelta.inMilliSecs();
						timeNSCostApplyDelta[id_nsseq].first++;

						delete &rev1;
						delete &ini1;

						double fasterCost = e_end1 - e_ini1;

						if (abs(revCost - fasterCost) > 0.0001)
						{
							error("difference between revCost and fasterCost");
							move.print();
							printf("revCost = %.4f\n", revCost);
							printf("fasterCost = %.4f\n", fasterCost);
							printf("e = %.4f\n", e.evaluation());
							printf("e_rev = %.4f\n", e_rev.evaluation());
							return false;
						}

						Timer tMoveCost;
						pair<double, double>* cost = NULL;

						if (evaluators[ev]->getAllowCosts())
							cost = move.cost(e, s.getR(), s.getADS());

						if (cost)
						{
							timeNSCost[id_nsseq].second += tMoveCost.inMilliSecs();
							timeNSCost[id_nsseq].first++;
						}

						if (cost)
						{
							double cValue = cost->first + cost->second;
							if (abs(revCost - cValue) > 0.0001)
							{
								error("difference between expected cost and cost()");
								move.print();
								printf("expected =\t %.4f\n", revCost);
								printf("cost() =\t %.4f\n", cValue);
								printf("==============\n");
								printf("CORRECT VALUES \n");
								printf("==============\n");
								printf("e: \t obj:%.4f \t inf:%.4f \t total:%.4f\n", e.getObjFunction(), e.getInfMeasure(), e.evaluation());
								printf("e':\t obj:%.4f \t inf:%.4f \t total:%.4f\n", e_rev.getObjFunction(), e_rev.getInfMeasure(), e_rev.evaluation());
								cout << "s: ";
								s.print();
								cout << "s': ";
								sNeighbor.print();
								cout << "move: ";
								move.print();
								printf("==============\n");
								printf("  GOOD LUCK!  \n");
								printf("==============\n");
								return false;
							}

							delete cost;
						}

						message(lEvaluator.at(ev), nqs, "all move costs okay (NSSeq tests)!");

						delete &e;

						delete &rev;
						delete &sNeighbor;
						delete &e_rev;
						delete &ini;
						delete &e_ini;
					}

					delete &move;
				}

				delete &it;
			}

			vCountMoves[id_nsseq] += countMoves;
			vCountValidMoves[id_nsseq] += countValidMoves;

			cout << "component.check: " << lNSSeq.at(id_nsseq) << " finished." << endl;
			if (verbose)
				cout << endl << endl;
		}

		for (unsigned i = 0; i < solutions.size(); i++)
			delete solutions[i];

		for (unsigned i = 0; i < evaluations.size(); i++)
			for (unsigned j = 0; j < evaluations[i].size(); j++)
				delete evaluations[i][j];

		cout << "===============================" << endl;
		cout << "           SUMMARY             " << endl;
		cout << "===============================" << endl << endl;

		printSummary(timeConstructive, "Constructive", "testing construction of initial solution");

		if(updateADS)
			printSummary(timeInitializeADS, "UpdateADS::initializeADS()", "testing lazy initializeADS in solutions");
		else
			cout << endl << "No UpdateADS was tested." << endl << endl;

		printSummary(fullTimeEval, "Evaluators", "testing full evaluate(s) of a solution");

		printSummary(timeNSApply, "NS", "testing time of move apply(s)");

		printSummary(timeNSCostApply, "NS", "testing time of cost based on move apply(s)");

		printSummary(timeNSCostApplyDelta, "NS", "testing time of cost based on move apply(e, s)");

		printSummary(timeNSCost, "NS", "testing time of move cost()");

		printSummary(timeNSEstimatedCost, "NS", "testing time of move estimatedCost()");

		printSummary(errorNSEstimatedCost, "NS", "testing error (%) of move estimatedCost()");

		printSummarySimple(vCountMoves, nSolNSSeq, "NSSeq", "counting moves of NSSeq iterator");

		printSummarySimple(vCountValidMoves, nSolNSSeq, "NSSeq", "counting valid moves of NSSeq iterator");

		cout << "component.check module: tests finished successfully!" << endl;
		return true;
	}

	void printSummary(const vector<pair<int, double> >& values, string type, string title)
	{
		printf("---------------------------------\n");
		cout << "|" << type << "|=" << values.size() << "\t" << title << endl;
		printf("---------------------------------\n");
		printf("#id\t#tests\tavg(ms)\tsum(ms)\n");
		double avg = 0;
		int validValues = 0;
		for (unsigned id = 0; id < values.size(); id++)
		{
			if (values[id].first > 0)
			{
				printf("#%d\t%d\t%.4f\t%.4f\n", ((int) id), values[id].first, (values[id].second / values[id].first), values[id].second);
				avg += (values[id].second / values[id].first);
				validValues++;
			}
			else
				printf("#%d\t%d\tUNTESTED OR UNIMPLEMENTED\n", ((int) id), 0);
		}
		printf("---------------------------------\n");
		printf("all\t-\t%.4f\t-\n", (avg / validValues));
		cout << endl;
	}

	void printSummarySimple(const vector<int>& values, int numTests, string type, string title)
	{
		printf("---------------------------------\n");
		cout << "|" << type << "|=" << values.size() << "\t" << title << endl;
		printf("---------------------------------\n");
		printf("#id\t#tests\tavg of %d tests\n", numTests);
		double avg = 0;
		int validValues = 0;
		for (unsigned id = 0; id < values.size(); id++)
		{
			if (values[id] > 0)
			{
				printf("#%d\t%d\t%d\n", ((int) id), values[id], (values[id] / numTests));
				avg += (values[id] / numTests);
				validValues++;
			}
			else
				printf("#%d\t%d\tUNTESTED OR UNIMPLEMENTED\n", ((int) id), 0);
		}
		printf("---------------------------------\n");
		printf("all\t-\t%.4f\t-\n", (avg / validValues));
		cout << endl;
	}

};

#endif /* CHECKMODULE_HPP_ */

