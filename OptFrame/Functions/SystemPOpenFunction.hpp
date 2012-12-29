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

#ifndef OPTFRAME_POPEN_FUNCTION_HPP_
#define OPTFRAME_POPEN_FUNCTION_HPP_

#include <iostream>
#include <ostream>

#include <vector>
#include <map>

#include "../Scanner++/Scanner.h"

#include "../OptFrameFunction.hpp"

#include "../OptFrameList.hpp"

#include <algorithm>

class SystemPOpenFunction : public OptFrameFunction
{
public:

	virtual ~SystemPOpenFunction()
	{
	}

	virtual string id()
	{
		return "system.popen";
	}

	virtual string usage()
	{
		return "system.popen( \"command\" ) : return output";
	}

	virtual string* run(vector<OptFrameFunction*>&, map< string, string >&, map< string,vector<string> >&, string body)
	{
		//cout << "popen function  POPEN: '" << Scanner::trim(body) << "'" << endl;
		Scanner scanner(Scanner::trim(body));
		scanner.useSeparators("\"");

		string command = scanner.next();
		scanner.useDefaultSeparators();
		scanner.next(); // drop '"'
		scanner.next(); // drop ')'

		//cout << "popen function COMMAND: '" << command << "'" << endl;

		FILE* pPipe = popen(command.c_str(), "r"); //popen("echo -e \"x <- c(1,2,3,4,5,6) \n shapiro.test(x)\" | R --no-save", "rt");
		if (pPipe == NULL)
		{
			cout << "popen function: PIPE NOT OPEN!" << endl;
			return NULL;
		}

		char line[100];

		string output = "";

		while(fgets(line, 100, pPipe) != NULL)
		{
			string sline = line;
			output.append(sline);
		}

		pclose(pPipe);

		return new string(output);
	}
};

#endif /* OPTFRAME_POPEN_FUNCTION_HPP_ */