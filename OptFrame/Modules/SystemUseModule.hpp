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

#ifndef OPTFRAME_SYSTEM_USE_MODULE_HPP_
#define OPTFRAME_SYSTEM_USE_MODULE_HPP_

#include "../OptFrameModule.hpp"

#include "SystemRequireModule.hpp"

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class M = OPTFRAME_DEFAULT_EMEMORY>
class SystemUseModule : public OptFrameModule<R, ADS, M>
{
public:

	virtual ~SystemUseModule()
	{
	}

	string id()
	{
		return "system.use";
	}

	string usage()
	{
		return "system.use prefix";
	}

	bool moduleExists(string moduleName, vector<OptFrameModule<R, ADS, M>*>& allModules)
	{
		for(unsigned i=0; i<allModules.size(); i++)
			if(allModules[i]->id() == moduleName)
				return true;
		return false;
	}

	bool functionExists(string functionName, vector<OptFrameFunction*>& allFunctions)
	{
		for(unsigned i=0; i<allFunctions.size(); i++)
			if(allFunctions[i]->id() == functionName)
				return true;
		return false;
	}

	bool run(vector<OptFrameModule<R, ADS, M>*>& allModules, vector<OptFrameFunction*>& allFunctions, HeuristicFactory<R, ADS, M>& factory, map<string,string>& dictionary, map< string,vector<string> >& ldictionary, string input)
	{
		// check dependency on 'module.rename' module
		if(!OptFrameModule<R, ADS, M>::run_module("system.require", allModules, allFunctions, factory, dictionary, ldictionary, "module.rename"))
		{
			cout << "error: system.use module depends on 'module.rename' module, which is not loaded!" << endl;
			return false;
		}

		// check dependency on 'function.rename' module
		if(!OptFrameModule<R, ADS, M>::run_module("system.require", allModules, allFunctions, factory, dictionary, ldictionary, "function.rename"))
		{
			cout << "error: system.use module depends on 'function.rename' module, which is not loaded!" << endl;
			return false;
		}

		Scanner scanner(input);

		if(!scanner.hasNext()) // no module
		{
			cout << "Usage: " << usage() << endl;
			return false;
		}

		string prefix = scanner.next();

		for(unsigned i=0; i<allModules.size(); i++)
		{
			Scanner scanPrefix(allModules[i]->id());
			scanPrefix.useSeparators(".");

			if(scanPrefix.next() == prefix)
			{
				string smallName = scanPrefix.next();

				if(moduleExists(smallName, allModules))
				{
					// WARNING!
					cout << "system.use module warning: couldn't rename '" << allModules[i]->id() << "' because module '" << smallName << "' is already registered!" << endl;
					continue;
				}

				stringstream ss;
				ss << allModules[i]->id() << " " << smallName;

				if(!OptFrameModule<R, ADS, M>::run_module("module.rename", allModules, allFunctions, factory, dictionary, ldictionary, ss.str()))
				{
					cout << "system.use module error: failed to do a module.rename with parameters '" << ss.str() << "'" << endl;
					return false;
				}
			}
		}

		for(unsigned i=0; i<allFunctions.size(); i++)
		{
			Scanner scanPrefix(allFunctions[i]->id());
			scanPrefix.useSeparators(".");

			if(scanPrefix.next() == prefix)
			{
				string smallName = scanPrefix.next();

				if(functionExists(smallName, allFunctions))
				{
					// WARNING!
					cout << "system.use module warning: couldn't rename '" << allFunctions[i]->id() << "' because function '" << smallName << "' is already registered!" << endl;
					continue;
				}

				stringstream ss;
				ss << allFunctions[i]->id() << " " << smallName;

				if(!OptFrameModule<R, ADS, M>::run_module("function.rename", allModules, allFunctions, factory, dictionary, ldictionary, ss.str()))
				{
					cout << "system.use module error: failed to do a function.rename with parameters '" << ss.str() << "'" << endl;
					return false;
				}
			}
		}


		return true;
	}

	// disable preprocess, only need module prefix
	virtual string* preprocess(vector<OptFrameFunction*>&, map<string, string>&,  map< string,vector<string> >&, string input)
	{
		// disable preprocess!!
		return new string(input);
	}

};

#endif /* OPTFRAME_SYSTEM_USE_MODULE_HPP_ */
