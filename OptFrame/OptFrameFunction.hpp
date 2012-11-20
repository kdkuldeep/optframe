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

/*
 * OptFrameFunction.hpp
 *
 * LGPLv3
 *
 */

#ifndef OPTFRAME_FUNCTION_HPP_
#define OPTFRAME_FUNCTION_HPP_

#include <iostream>
#include <ostream>

#include <vector>
#include <map>

#include "Scanner++/Scanner.h"

#include <algorithm>

class OptFrameFunction
{
public:

	static pair<string, string> run_function(string func, vector<OptFrameFunction*>& allFunctions, string input)
	{
		for(unsigned int i=0;i<allFunctions.size();i++)
			if(func==allFunctions[i]->id())
			{
				string iprep = allFunctions[i]->preprocess(allFunctions, input);
				pair<string, string> p = allFunctions[i]->run(allFunctions, iprep);
				return p;
			}

		cout << "Function '" << func << "' not found." << endl;

		return make_pair("", input);
	}

	static bool functionExists(string func, vector<OptFrameFunction*>& allFunctions)
	{
		for(unsigned int i=0;i<allFunctions.size();i++)
			if(func==allFunctions[i]->id())
				return true;
		return false;
	}

	virtual ~OptFrameFunction()
	{
	}

	virtual string id() = 0;
	virtual string usage() = 0;

	virtual pair<string, string> run(vector<OptFrameFunction*>& allFunctions, string body) = 0;

	virtual string preprocess(vector<OptFrameFunction*>& allFunctions, string input)
	{
		return defaultPreprocess(allFunctions, input);
	}

	static string defaultPreprocess(vector<OptFrameFunction*>& allFunctions, string input)
	{
		Scanner scanner(input);

		// Locate functions

		if(!scanner.hasNext())
			return input; // empty

		int bcount = 0;
		stringstream ssfunc;
		// add space before and after brackets '(' ')'
		for(unsigned i=0; i<input.length(); i++)
			if(input.at(i)=='(')
			{
				ssfunc << " " << input.at(i) << " ";
				bcount++;
			}
			else if(input.at(i)==')')
			{
				ssfunc << " " << input.at(i) << " ";
			}
			else
				ssfunc << input.at(i);

		if(bcount==0)
			return input; // no functions

		Scanner scanFunc(ssfunc.str());

		string input5 = "";

		string last       = scanFunc.next();
		string ldiscarded = scanFunc.getDiscarded();

		while(scanFunc.hasNext())
		{
			string current    = scanFunc.next();
			string cdiscarded = scanFunc.getDiscarded();

			if((current == "(") && OptFrameFunction::functionExists(last, allFunctions)) // FUNCTION
			{
				pair<string, string> p = OptFrameFunction::run_function(last, allFunctions, scanFunc.rest());

				input5.append(" ");
				input5.append(p.first);
				input5.append(" ");

				scanFunc = Scanner(p.second);

				if(!scanFunc.hasNext())
				{
					last = "";
					ldiscarded = "";
					break;
				}
				else
				{
					last    = scanFunc.next();
					ldiscarded = scanFunc.getDiscarded();
					continue;
				}
			}
			else
			{
				input5.append(ldiscarded);
				input5.append(last);
			}

			// update last
			ldiscarded = cdiscarded;
			last       = current;
		}

		input5.append(ldiscarded);
		input5.append(last);

		string input6 = Scanner::trim(input5);

		return input6;
	}
};

#endif /* OPTFRAME_FUNCTION_HPP_ */
