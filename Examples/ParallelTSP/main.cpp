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

//Rename to mainTSP.cpp to run the example

// ===================================
// Main.cpp file generated by OptFrame
// Project Traveling Salesman Problem
// ===================================

#define MaPI

#include <stdlib.h>
#include <math.h>

#include <iostream>

using namespace std;

#include <set>

#include "../../OptFrame/OptFrame.hpp"
#include "../../OptFrame/Experimental/VShuffle.hpp"
#include "../TSP.h"

int main(int argc, char **argv)
{
	// Optimal value for berlin52 is 7542

	RandGen rg;

	Scanner scanner(new File("../TSP/tsplib/berlin52.txt"));

	TSPProblemInstance* p = new TSPProblemInstance(scanner);

	TSPEvaluator eval(p);

	// MapReduce declaration
	MaPI_MapReduce<RepTSP, RankAndStop, int, pair<RepTSP, double> , RepTSP> mapReduce;
	TSPSerializer serializer;
	MyMaPIMapper<RepTSP, OPTFRAME_DEFAULT_ADS, MemTSP> mapper(&mapReduce, &serializer, eval);
	MyMaPIReducer<RepTSP, OPTFRAME_DEFAULT_ADS, MemTSP> reducer(&mapReduce, &serializer, eval);

	srand(clock() + mapReduce.getMPIRank()); // Setting seed according to mpi rank

	RandomInitialSolutionTSP is(p, rg);

	SolutionTSP& s = is.generateSolution();
	s.print();
	EvaluationTSP * e = &eval.evaluate(s);
	e->print();
	cout << endl;

	OptFrame<RepTSP, OPTFRAME_DEFAULT_ADS, MemTSP> optframe(rg);
	optframe.factory.add_initsol(&is);
	optframe.factory.add_ev(&eval);
	optframe.factory.add_ns(new NSEnumSwap(p, rg));
	optframe.factory.add_method(new VShuffle<RepTSP, OPTFRAME_DEFAULT_ADS, MemTSP> );

	// Adding MapReduce to factory
	optframe.factory.setMapReduce(&serializer, &mapReduce, &mapper, &reducer, argc, argv);

	optframe.execute("read example.opt");

	cout << "Program ended successfully" << endl;

	return 0;
}
