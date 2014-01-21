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

#ifndef OPTFRAME_DECODER_HPP_
#define OPTFRAME_DECODER_HPP_

#include <algorithm>

#include "Component.hpp"
#include "Solution.hpp"
#include "MultiEvaluation.hpp"


namespace optframe
{

template<class R, class X, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class Decoder: public Component
{
public:

	virtual ~Decoder()
	{
	}

	virtual pair<vector<Solution<X, ADS>*>, vector<MultiEvaluation<DS>*> > decode(const Solution<R>& solution) = 0;
};

}

#endif /*OPTFRAME_DECODER_HPP_*/