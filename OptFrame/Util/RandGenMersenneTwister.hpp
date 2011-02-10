//Description and pseudocode in http://en.wikipedia.org/wiki/Mersenne_twister

#ifndef RANDGENMersenneTwister_HPP_
#define RANDGENMersenneTwister_HPP_

#include "../RandGen.hpp"

class RandGenMersenneTwister: public RandGen
{
private:
	static const unsigned int GEN_STATE_LENGTH = 624;
	static const unsigned int MASK_1 = 0xffffffff; //4294967295
	static const unsigned int MASK_2 = 0x80000000; //2147483648
	static const unsigned int MASK_3 = 0x7fffffff; //2147483647

	static const unsigned int CONSTANT_1 = 0x6c078965; //1812433253
	static const unsigned int CONSTANT_2 = 0x9d2c5680; //2636928640
	static const unsigned int CONSTANT_3 = 0xefc60000; //4022730752
	static const unsigned int CONSTANT_4 = 0x9908b0df; //2567483615
	unsigned int MT[GEN_STATE_LENGTH];
	int index;

public:

	RandGenMersenneTwister() :
		RandGen()
	{
		initialize();
	}

	RandGenMersenneTwister(unsigned int _seed)
	{
		RandGen::seed = _seed;
		initialize();
	}

	void initialize()
	{
		// initialize random number generation
		register int i;

		index = 0;
	    MT[0] = seed;
		for(i = 1; i < GEN_STATE_LENGTH; i++){
			MT[i] = ((CONSTANT_1 * (MT[i - 1] ^ (MT[i - 1] >> 30))) + i) & MASK_1;
		}
	}

	unsigned int rand()
	{// generate random number
		unsigned int y;

		if(index == 0){
			GenNum();
		}

		y = MT[index];
		y = y ^ (y >> 11);
		y = y ^ ((y << 7) & CONSTANT_2);
		y = y ^ ((y << 15) & CONSTANT_3);
		y = y ^ (y >> 18);

		index = (index + 1) % GEN_STATE_LENGTH;

		return y;
	}


	void GenNum()
	{
	    register int i;
	    unsigned int y;
	    for( i = 0; i < GEN_STATE_LENGTH; i++){
	        y = (MT[i] ^ MASK_2) + (MT[(i + 1) % GEN_STATE_LENGTH] ^ MASK_3);
	        MT[i] = MT[(i + 397) % GEN_STATE_LENGTH] ^ (y >> 1);
	        if((y % 2) == 1){
	            MT[i] = MT[i] ^ CONSTANT_4;
	        }
	    }
	}
};

#endif /* RANDGENMersenneTwister_HPP_ */
