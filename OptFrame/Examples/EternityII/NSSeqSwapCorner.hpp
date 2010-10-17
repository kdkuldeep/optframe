#ifndef EtII_NSSEQSwapCorner_HPP_
#define EtII_NSSEQSwapCorner_HPP_

// Framework includes
#include "../../../OptFrame/NSSeq.hpp"

// Own includes
#include "ProblemInstance.hpp"
#include "Memory.h"
#include "Solution.h"

using namespace std;

class MoveSwapCorner: public Move<RepEtII, MemEtII>
{
private:
	int x1, y1, x2, y2;

public:

	using Move<RepEtII, MemEtII>::apply; // prevents name hiding
	using Move<RepEtII, MemEtII>::canBeApplied; // prevents name hiding

	MoveSwapCorner(int _x1, int _y1, int _x2, int _y2) :
		x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	{
	}

	virtual ~MoveSwapCorner()
	{
	}

	bool canBeApplied(const RepEtII& rep)
	{
		return true;
	}

	Move<RepEtII, MemEtII>& apply(RepEtII& rep)
	{
		Piece p = rep(x1, y1);
		rep(x1, y1) = rep(x2, y2);
		rep(x2, y2) = p;

		while ((rep(0, 0).left != 0) || (rep(0, 0).up != 0))
			rep(0, 0).rotate();

		while ((rep(0, rep.getCols() - 1).right != 0) || (rep(0, rep.getCols() - 1).up != 0))
			rep(0, rep.getCols() - 1).rotate();

		while ((rep(rep.getRows() - 1, 0).left != 0) || (rep(rep.getRows() - 1, 0).down != 0))
			rep(rep.getRows() - 1, 0).rotate();

		while ((rep(rep.getRows() - 1, rep.getCols() - 1).right != 0) || (rep(rep.getRows() - 1, rep.getCols() - 1).down != 0))
			rep(rep.getRows() - 1, rep.getCols() - 1).rotate();

		return *new MoveSwapCorner(x2, y2, x1, y1);
	}

	Move<RepEtII, MemEtII>& apply(MemEtII& mem, RepEtII& rep)
	{
		int f = 0;

		if (rep(0, 0).right == rep(0, 1).left)
			f++;
		if (rep(0, 0).down == rep(1, 0).up)
			f++;

		if (rep(0, rep.getCols() - 1).left == rep(0, rep.getCols() - 2).right)
			f++;
		if (rep(0, rep.getCols() - 1).down == rep(1, rep.getCols() - 1).up)
			f++;

		if (rep(rep.getRows() - 1, 0).right == rep(rep.getRows() - 1, 1).left)
			f++;
		if (rep(rep.getRows() - 1, 0).up == rep(rep.getRows() - 2, 0).down)
			f++;

		if (rep(rep.getRows() - 1, rep.getCols() - 1).left == rep(rep.getRows() - 1, rep.getCols() - 2).right)
			f++;
		if (rep(rep.getRows() - 1, rep.getCols() - 1).up == rep(rep.getRows() - 2, rep.getCols() - 1).down)
			f++;

		Move<RepEtII, MemEtII>& rev = apply(rep);

		int f2 = 0;

		if (rep(0, 0).right == rep(0, 1).left)
			f2++;
		if (rep(0, 0).down == rep(1, 0).up)
			f2++;

		if (rep(0, rep.getCols() - 1).left == rep(0, rep.getCols() - 2).right)
			f2++;
		if (rep(0, rep.getCols() - 1).down == rep(1, rep.getCols() - 1).up)
			f2++;

		if (rep(rep.getRows() - 1, 0).right == rep(rep.getRows() - 1, 1).left)
			f2++;
		if (rep(rep.getRows() - 1, 0).up == rep(rep.getRows() - 2, 0).down)
			f2++;

		if (rep(rep.getRows() - 1, rep.getCols() - 1).left == rep(rep.getRows() - 1, rep.getCols() - 2).right)
			f2++;
		if (rep(rep.getRows() - 1, rep.getCols() - 1).up == rep(rep.getRows() - 2, rep.getCols() - 1).down)
			f2++;

		mem += (f2 - f);

		return rev;
	}

	virtual bool operator==(const Move<RepEtII, MemEtII>& _m) const
	{
		const MoveSwapCorner& m = (const MoveSwapCorner&) _m;
		return (m.x1 == x1) && (m.y1 == y1) && (m.x2 == x2) && (m.y2 == y2);
	}

	void print()
	{
		cout << "MoveSwapCorner: (" << x1 << "," << y1 << ") <=> (" << x2 << "," << y2 << ")" << endl;
	}
};

class NSIteratorSwapCorner: public NSIterator<RepEtII, MemEtII>
{
private:
	int x1, y1, x2, y2;
	int nRows, nCols;

public:
	NSIteratorSwapCorner(int _nRows, int _nCols) :
		nRows(_nRows), nCols(_nCols)
	{
	}

	virtual ~NSIteratorSwapCorner()
	{
	}

	virtual void first()
	{
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 1;
	}

	virtual void next()
	{
		y2++;
		if (y2 > 1)
		{
			y2 = 0;
			x2++;
			if (x2 > 1)
			{
				y1++;
				if (y1 > 1)
				{
					y1 = 0;
					x1++;

					x2 = x1;
					y2 = y1 + 1;
					if (y2 > 1)
					{
						y2 = 0;
						x2++;
					}
				}
				else
				{
					x2 = x1;
					y2 = y1 + 1;
					if (y2 > 1)
					{
						y2 = 0;
						x2++;
					}
				}
			}
		}
	}

	virtual bool isDone()
	{
		return x2 > 1;
	}

	virtual Move<RepEtII, MemEtII>& current()
	{
		return *new MoveSwapCorner(x1 * (nRows - 1), y1 * (nCols - 1), x2 * (nRows - 1), y2 * (nCols - 1));
	}
};

class NSSeqSwapCorner: public NSSeq<RepEtII, MemEtII>
{
public:

	using NSSeq<RepEtII, MemEtII>::move; // prevents name hiding

	NSSeqSwapCorner()
	{
	}

	virtual ~NSSeqSwapCorner()
	{
	}

	virtual Move<RepEtII, MemEtII>& move(const RepEtII& rep)
	{
		int x1 = (rand() % 2) * (rep.getRows() - 1);
		int y1 = (rand() % 2) * (rep.getCols() - 1);

		int x2 = x1;
		int y2 = y1;

		while ((x2 == x1) && (y2 == y1))
		{
			x2 = (rand() % 2) * (rep.getRows() - 1);
			y2 = (rand() % 2) * (rep.getCols() - 1);
		}

		return *new MoveSwapCorner(x1, y1, x2, y2);
	}

	virtual NSIterator<RepEtII, MemEtII>& getIterator(const RepEtII& rep)
	{
		// return an iterator to the neighbors of 'rep'
		return *new NSIteratorSwapCorner(rep.getRows(), rep.getCols());
	}

	virtual void print()
	{
		cout << "NSSeqSwapCorner" << endl;
	}
};

#endif /*EtII_NSSEQSwapCorner_HPP_*/
