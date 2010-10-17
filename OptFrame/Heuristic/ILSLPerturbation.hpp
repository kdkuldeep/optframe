#ifndef OPTFRAME_ILSLPerturbation_HPP_
#define OPTFRAME_ILSLPerturbation_HPP_

#include <math.h>
#include <vector>

#include "../NS.hpp"

template<class R, class M = OPTFRAME_DEFAULT_MEMORY>
class ILSLPerturbation
{
public:
	virtual void perturb(Solution<R>& s, Evaluation<M>& e, double timelimit, double target_f, int level) = 0;
};

template<class R, class M = OPTFRAME_DEFAULT_MEMORY>
class ILSLPerturbationLPlus2: public ILSLPerturbation<R, M>
{
private:
	vector<NS<R, M>*> ns;
	Evaluator<R, M>& evaluator;
	int pMax;

public:
	ILSLPerturbationLPlus2(Evaluator<R, M>& e, int _pMax, NS<R, M>& _ns) :
		evaluator(e), pMax(_pMax)
	{
		ns.push_back(&_ns);
	}

	void add_ns(NS<R, M>& _ns)
	{
		ns.push_back(&_ns);
	}

	void perturb(Solution<R>& s, Evaluation<M>& e, double timelimit, double target_f, int level)
	{
		int f = 0; // number of failures
		int a = 0; // number of appliable moves

		level += 2; // level 0 applies 2 moves

		while ((a < level) && (f < pMax))
		{
			int x = rand() % ns.size();

			Move<R, M>& m = ns[x]->move(s);

			if (m.canBeApplied(e, s))
			{
				a++;
				delete &m.apply(e, s);
			}
			else
				f++;

			delete &m;
		}

		if (f == pMax)
			cout << "ILS Warning: perturbation had no effect in " << pMax << " tries!" << endl;

		evaluator.evaluate(e, s); // updates 'e'
	}
};

#endif /*OPTFRAME_ILSLPerturbation_HPP_*/