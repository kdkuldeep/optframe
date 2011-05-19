// OptFrame - Optimization Framework

// Copyright (C) 2009, 2010, 2011
// Igor Machado - Mario Henrique Perche - Pablo Munhoz - Sabir Ribas
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

#ifndef OPTFRAME_ITERATORNSSEQKROUTE_HPP_
#define OPTFRAME_ITERATORNSSEQKROUTE_HPP_

// Framework includes
#include "../../NSIterator.hpp"

#include "../Moves/MovekRoute.hpp"


using namespace std;

template<class T, class M = OPTFRAME_DEFAULT_MEMORY, class MOVE = MovekRoute<T, M> >
class IteratorNSSeqkRoute: public NSIterator<vector<vector<T> > , M>
{
	typedef vector<T> Route;
	typedef vector<vector<T> > MultiRoute;

private:
	int k;
	NSIterator<Route, M>& iterator;

public:

	IteratorNSSeqkRoute(int _k, NSIterator<Route, M>& it) :
		k(_k), iterator(it)
	{
	}

	virtual ~IteratorNSSeqkRoute()
	{
		delete &iterator;
	}

	void first()
	{
		iterator.first();
	}

	void next()
	{
		iterator.next();
	}

	bool isDone()
	{
		return iterator.isDone();
	}

	Move<MultiRoute, M>& current()
	{
		return *new MOVE(k, iterator.current());
	}
};

#endif /*OPTFRAME_ITERATORNSSEQKROUTE_HPP_*/