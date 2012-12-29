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

#ifndef OPTFRAME_ELEMENT_FUNCTION_HPP_
#define OPTFRAME_ELEMENT_FUNCTION_HPP_

#include <iostream>
#include <ostream>

#include <vector>
#include <map>

#include "../Scanner++/Scanner.h"

#include "../OptFrameFunction.hpp"

#include "../OptFrameList.hpp"

#include <algorithm>

class ListElementFunction : public OptFrameFunction
{
public:

	virtual ~ListElementFunction()
	{
	}

	string id()
	{
		return "list.element";
	}
	string usage()
	{
		return "list.element( N list ) : return element at 'N'th position of 'list'";
	}

	virtual string* run(vector<OptFrameFunction*>& allFunctions, map< string, string >&, map< string,vector<string> >& ldictionary, string body)
	{
		Scanner scan(body);

		if (!scan.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return NULL;
		}

		int n = scan.nextInt();

		if (n < 0)
		{
			cout << "N must be a positive value!" << endl;
			return NULL;
		}

		if (n == 0)
		{
			cout << "sorry, this is not C language :)" << endl;
			cout << "0 not included, try a number from 1 to the size of the list" << endl;
			return NULL;
		}

		n--;

		if (!scan.hasNext())
		{
			cout << "Usage: " << usage() << endl;
			return NULL;
		}

		vector<string>* plist = OptFrameList::readList(ldictionary, scan);
		vector<string>  list;
		if(plist)
		{
			list = vector<string>(*plist);
			delete plist;
		}
		//else
		//	return NULL;

		if (n >= ((int) list.size()))
		{
			cout << "N is too big! " << (n + 1) << " > " << list.size() << endl;
			return NULL;
		}

		string element = list.at(n);

		return new string(element);
	}
};

#endif /* OPTFRAME_ELEMENT_FUNCTION_HPP_ */