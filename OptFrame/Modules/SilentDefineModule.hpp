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

#ifndef SILENT_DEFINE_MODULE_HPP_
#define SILENT_DEFINE_MODULE_HPP_

#include "../OptFrameModule.hpp"

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class M = OPTFRAME_DEFAULT_EMEMORY>
class SilentDefineModule : public OptFrameModule<R, ADS, M>
{
public:

	virtual ~SilentDefineModule()
	{
	}

	string id()
	{
		return "silent_define";
	}
	string usage()
	{
		return "silent_define new_name string_to_be_substituted_from_the_new_name";
	}

	void run(vector<OptFrameModule<R, ADS, M>*>&, HeuristicFactory<R, ADS, M>*, map<string,string>* dictionary, string rest)
	{
		Scanner scanner(rest);

		string new_name = scanner.next();

		if(new_name != "")
		{
			string second_word = scanner.rest();

			Scanner s2(second_word);

			while(s2.hasNext())
				if(new_name==s2.next())
				{
					cout << "Recursive definitions are not allowed!" << endl;
					return;
				}

			(*dictionary)[new_name] = scanner.trim(second_word);
		}
		else
			cout << "Usage: "<<usage()<<endl;
	}

	virtual string preprocess(map<string,string>*, string input)
	{
		Scanner scanner(input);

		// First, remove the comments '%'

		string input2 = "";

		while(scanner.hasNextChar())
		{
			char c = scanner.nextChar();
			if(c != '%')
				input2 += c;
			else
				break;
		}

		return input2;
	}

};

#endif /* SILENT_DEFINE_MODULE_HPP_ */
