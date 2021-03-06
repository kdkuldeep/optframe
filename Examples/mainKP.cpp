// ===================================
// Main.cpp file generated by OptFrame
// Project KnapsackProblem
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "../OptFrame/Util/printable.h"
#include "../OptFrame/Util/CheckCommand.hpp"
#include "../OptFrame/Util/RandGenMersenneTwister.hpp"
#include "../OptFrame/Heuristics/LocalSearches/BestImprovement.hpp"
#include "../OptFrame/Heuristics/LocalSearches/FirstImprovement.hpp"
#include "../OptFrame/Heuristics/LocalSearches/HillClimbing.hpp"
#include "../OptFrame/Heuristics/LocalSearches/RandomDescentMethod.hpp"
#include "../OptFrame/Heuristics/EvolutionaryAlgorithms/BRKGA.hpp"
#include "../OptFrame/Heuristics/SA/BasicSimulatedAnnealing.hpp"
#include "KP.h"
using namespace std;
using namespace optframe;
using namespace KP;

int main(int argc, char **argv)
{
   RandGenMersenneTwister rg; // declara um bom gerador de números aleatórios  
   Scanner scanner(new File("instance-4.txt")); // carrega arquivo no disco
   ProblemInstance p(scanner);  // cria um problema-teste usando o arquivo carregado
   MyEvaluator ev(p);   ConstructiveRandom c1(p); NSSeqBitFlip ns1(p, rg);
   cout << "will generate solution" << endl;
   Solution<RepKP>& s = c1.generateSolution(); s.print();
   Evaluation& e = ev.evaluate(s.getR()); e.print();
   cout << "GUD" << endl;



   CheckCommand<RepKP> check;                    // cria o módulo de testes (opcional)
   check.add(ev);                                // carrega a avaliação para testes
   check.add(c1);                                // carrega o construtivo para testes
   check.add(ns1);                               // carrega a vizinhança para testes
   check.run(100,10);                             // executa testes com 10 iterações

   BasicSimulatedAnnealing<RepKP, MY_ADS> sa(ev, c1, &ns1, 0.98, 100, 900.0, rg);
   pair<Solution<RepKP, MY_ADS>&, Evaluation&>* r = sa.search();
   r->first.print();
   r->second.print();
   delete &r->first;
   delete &r->second;
   delete r;

   BestImprovement<RepKP, MY_ADS> bi(ev, ns1);  FirstImprovement<RepKP, MY_ADS> fi(ev, ns1);
   HillClimbing<RepKP, MY_ADS> sd(ev, bi);      HillClimbing<RepKP, MY_ADS> pm(ev, fi);
   RandomDescentMethod<RepKP, MY_ADS> rdm(ev, ns1, 10);
   sd.search(s,e).second.print();  // executa e imprime HC + BI
   pm.search(s,e).second.print();  // executa e imprime HC + FI
   rdm.search(s,e).second.print(); // executa e imprime RDM com 10 iterações
   
   delete &s;
   delete &e;

   EvaluatorSubsetRandomKeys eprk(ev, 0, p.N-1);
   BRKGA<RepKP> brkga(eprk, p.N, 1000, 30, 0.4, 0.3, 0.6);

   pair<Solution<random_keys>&, Evaluation&>* r2 = brkga.search();
   r2->first.print();
   r2->second.print();
   delete &r2->first;
   delete &r2->second;
   delete r2;

   cout << "Program ended successfully" << endl;
   return 0;
}
