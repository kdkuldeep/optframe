// OptFrame - Optimization Framework

// Copyright (C) 2009-2015
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

/*
 * MultObjectiveMetrics.hpp
 *
 * LGPLv3
 *
 */

#ifndef OPTFRAME_MOMETRICS_HPP_
#define OPTFRAME_MOMETRICS_HPP_

#include "../Solution.hpp"
#include "../Evaluation.hpp"
#include "../Evaluator.hpp"
#include "../Move.hpp"
#include "../Population.hpp"

#include <iostream>
#include <cmath>

using namespace std;

template<class R, class ADS = OPTFRAME_DEFAULT_ADS>
class UnionNDSets
{
protected:
	vector<Evaluator<R, ADS>*> v_e;

	bool addSolution(vector<vector<double> >& p, vector<double>& s)
	{
		bool added = true;
		for (int ind = 0; ind < p.size(); ind++)
		{

			if (weakDominates(p[ind], s))
				return false;

			if (dominates(s, p.at(ind)))
			{
				p.erase(p.begin() + ind); //Esta perdendo o individuo retornado,tem problema? todo
				ind--;
			}
		}

		if (added == true)
			p.push_back(s);

		return added;
	}

	// true if 's1' dominates 's2'
	virtual bool weakDominates(const vector<double>& s1, const vector<double>& s2)
	{

		int better = 0;
		int equals = 0;

		for (int e = 0; e < s1.size(); e++)
		{
			if (v_e[e]->betterThan(s1[e], s2[e]))
				better++;

			if (abs(s1[e] - s2[e]) < 0.0001)
				equals++;
		}

		return ((better + equals == v_e.size()));
	}

	// true if 's1' weak dominates 's2'
	virtual bool dominates(const vector<double>& s1, const vector<double>& s2)
	{

		int better = 0;
		int equals = 0;

		for (int e = 0; e < s1.size(); e++)
		{
			if (v_e[e]->betterThan(s1[e], s2[e]))
				better++;

			if (abs(s1[e] - s2[e]) < 0.0001)
				equals++;
		}

		return ((better + equals == v_e.size()) && (better > 0));
	}

public:

	UnionNDSets(vector<Evaluator<R, ADS>*> _v_e) :
			v_e(_v_e)
	{

	}

	UnionNDSets()
	{

	}

	virtual ~UnionNDSets()
	{
	}

	vector<vector<double> > readPF(string caminho, int nTests, int nOF)
	{

		vector<vector<double> > D;

		for (int e = 0; e < nTests; e++)
		{
			stringstream ss;
			ss << caminho;

			Scanner scanner(new File(ss.str()));
			while (scanner.hasNext())
			{
				vector<double> ind;
				for (int o = 0; o < nOF; o++)
					ind.push_back(scanner.nextDouble());


				addSolution(D, ind);
			}

		}

		return D;
	}

	vector<vector<double> > unionSets(vector<vector<double> > D1, vector<vector<double> > D2)
	{
		vector<vector<double> > ref = D1;

		for (int ind = 0; ind < D2.size(); ind++)
			addSolution(ref, D2[ind]);

		return ref;
	}

	int cardinalite(vector<vector<double> > D, vector<vector<double> > ref)
	{
		int card = 0;
		for (int i = 0; i < D.size(); i++)
		{
			for (int j = 0; j < ref.size(); j++)
			{
				if (weakDominates(D[i], ref[j]))
				{
					card++;
					j = ref.size();
				}
			}
		}

		return card;
	}

	double setCoverage(vector<vector<double> > a, vector<vector<double> > b)
	{
		double cover = 0;

		for (int i = 0; i < b.size(); i++)
		{
			for (int j = 0; j < a.size(); j++)
			{
				bool wD = weakDominates(a[j], b[i]);
				if (wD)
				{
					j = a.size();
					cover += 1;
					//cout<<" i = "<<i<<" j = "<<j<<endl;
					//getchar();
				}
			}
		}

		//cout<<" cover = "<<cover<<endl;

		double sCover = cover / b.size();

		return sCover;
	}

	double deltaMetric(vector<vector<double> > pareto, vector<double> utopicEval)
	{

		vector<double> vDist;

		int nObj = utopicEval.size();
		int nSol = pareto.size();
		vector<double> minEval(nObj, 1000000);
		double dMean = 0;
		for (int nS = 0; nS < nSol; nS++)
		{
			//vector with the evaluation of solution nS
			vector<double> eval = pareto[nS];
			for (int e = 0; e < nObj; e++)
			{
				if (eval[e] < minEval[e])
					minEval[e] = eval[e];
			}

			double minDist = 10000000;
			for (int nS2 = 0; nS2 < pareto.size(); nS2++)
			{
				double dist = 0;
				if (nS2 != nS)
				{
					vector<double> eval2 = pareto[nS2];
					for (int e = 0; e < nObj; e++)
						dist += pow(eval[e] - eval2[e], 2);
					dist = sqrt(dist);
					if (dist < minDist)
						minDist = dist;
				}

			}

			vDist.push_back(minDist);
			dMean += minDist;

		}
		dMean /= nSol;

		double dUtopic = 0;
		for (int e = 0; e < nObj; e++)
		{
			dUtopic += pow(minEval[e] - utopicEval[e], 2);
			//cout<<minEval[e]<<endl;
			//cout<<utopicEval[e]<<endl;
		}

		dUtopic = sqrt(dUtopic);
		//cout<<dUtopic<<endl;
		//getchar();

		double sumDist = 0;
		for (int nS = 0; nS < nSol; nS++)
		{
			sumDist += vDist[nS] - dMean;
		}

		double delta = (dUtopic + sumDist) / (nSol * dMean + dUtopic);
		return delta;
		//cout << "delta = " << delta << endl;
		//getchar();

	}

	double spacing(vector<vector<double> > a)
	{
		double ss = 0;
		vector<double> distMin;
		int N = a.size();
		int m = a[0].size();
		for (int i = 0; i < a.size(); i++)
		{
			double di = 100000000;

			for (int j = 0; j < a.size(); j++)
			{
				if (i != j)
				{
					double diMin = 0;
					for (int eval = 0; eval < m; eval++)
						diMin += abs(a[i][eval] - a[j][eval]);

					if (diMin < di)
						di = diMin;
				}
			}
			distMin.push_back(di);
		}

		double dMean = 0;

		for (int i = 0; i < N; i++)
			dMean += distMin[i];

		dMean = dMean / N;

		for (int i = 0; i < N; i++)
			ss += pow(distMin[i] - dMean, 2);

		ss = ss / N;
		ss = sqrt(ss);

		return ss;
	}

	vector<double> spacing2(vector<vector<vector<double> > > D)
	{
		vector<double> spacings;
		for (int frente = 0; frente < D.size(); frente++)
		{
			vector<vector<double> > a = D[frente];
			double ss = 0;
			vector<double> distMin;
			int N = a.size();
			int m = a[0].size();
			for (int i = 0; i < a.size(); i++)
			{
				double di = 100000000;

				for (int j = 0; j < a.size(); j++)
				{
					if (i != j)
					{
						double diMin = 0;
						for (int eval = 0; eval < m; eval++)
							diMin += abs(a[i][eval] - a[j][eval]);

						if (diMin < di)
							di = diMin;
					}
				}
				distMin.push_back(di);
			}

			double dMean = 0;

			for (int i = 0; i < N; i++)
				dMean += distMin[i];

			dMean = dMean / N;

			for (int i = 0; i < N; i++)
				ss += pow(distMin[i] - dMean, 2);

			ss = ss / N;
			ss = sqrt(ss);
			spacings.push_back(ss);
		}
		return spacings;
	}

	char* execCommand(const char* command)
	{

		FILE* fp;
		char* line = NULL;
		// Following initialization is equivalent to char* result = ""; and just
		// initializes result to an empty string, only it works with
		// -Werror=write-strings and is so much less clear.
		char* result = (char*) calloc(1, 1);
		size_t len = 0;

		fflush(NULL);
		fp = popen(command, "r");
		if (fp == NULL)
		{
			printf("Cannot execute command:\n%s\n", command);
			return NULL;
		}

		while (getline(&line, &len, fp) != -1)
		{
			// +1 below to allow room for null terminator.
			result = (char*) realloc(result, strlen(result) + strlen(line) + 1);
			// +1 below so we copy the final null terminator.
			strncpy(result + strlen(result), line, strlen(line) + 1);
			free(line);
			line = NULL;
		}

		fflush(fp);
		if (pclose(fp) != 0)
		{
			perror("Cannot close stream.\n");
		}

		return result;
	}

	double hipervolumeWithExecRequested(vector<vector<double> > v, vector<double> refPoints)
	{
		int nSol = v.size();
		int nObj = v[0].size();
		string tempFile = "tempFileHipervolueFunc";
		FILE* fTempHV = fopen(tempFile.c_str(), "w");

		for (int s = 0; s < nSol; s++)
		{
			for (int o = 0; o < nObj; o++)
			{
				fprintf(fTempHV, "%.7f\t", v[s][o]);
			}
			fprintf(fTempHV, "\n");
		}

		fclose(fTempHV);
		stringstream ss;
		ss << "./hv\t -r \"";
		for (int o = 0; o < nObj; o++)
			ss << refPoints[o] << " ";
		ss << "\" \t" << tempFile.c_str();
		string hvValueString = execCommand(ss.str().c_str());
		double hvValue = atof(hvValueString.c_str());
		return hvValue;
	}

};

#endif /*OPTFRAME_MOMETRICS_HPP_*/

