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

#ifndef OPTFRAME_SOLUTION_HPP_
#define OPTFRAME_SOLUTION_HPP_

#include <cstdlib>
#include <iostream>

using namespace std;

typedef int OPTFRAME_DEFAULT_ADS;

//! \english The Solution class is a container class for the Representation structure R. \endenglish \portuguese A classe Solution é uma classe contêiner para a Representação R. \endportuguese

/*!
  \english
  In the constructor, a copy of R is stored inside the Solution class.
  The getR() method returns a reference to the stored Representation R.
  \endenglish

  \portuguese
  No construtor, uma cópia de R é armazenada dentro da classe Solution.
  O método getR() retorna uma referência à Representação R armazenada.
  \endportuguese
*/

template<class R, class ADS = OPTFRAME_DEFAULT_ADS>
class Solution
{
protected:
	R& r;
	ADS& ads;

public:
	Solution(R& _r, ADS& _ads):r(*new R(_r)), ads(*new ADS(_ads)){};

	Solution(R& _r) :
      r(*new R(_r)), ads(*new OPTFRAME_DEFAULT_ADS)
   {
   }

	Solution(const Solution<R, ADS>& s):r(*new R(s.r)), ads(*new ADS(s.ads)){}

	virtual ~Solution() { delete &r; delete &ads;}

	void setR(const R& _r){ r = _r; }
	void setADS(const ADS& _ads){ ads = _ads; }

	const R& getR() const {	return r; }
	const ADS& getADS() const { return ads; }

	R& getR() { return r; }
	ADS& getADS() { return ads; }

	virtual bool check() const
	{
	   return true;
	}

	virtual void print() const
	{
		cout << "Solution: "<< r << endl;
      //cout << "ADS: "<< ads << endl;
	}

	virtual Solution<R, ADS>& operator= (const Solution<R, ADS>& s)
	{
		if(&s == this) // auto ref check
			return *this;

		r = s.r;
	   ads = s.ads;

		return *this;
	}

	virtual Solution<R, ADS>& clone() const
	{
		return * new Solution<R, ADS>(*this);
	}

};

#endif /* OPTFRAME_SOLUTION_HPP_ */
