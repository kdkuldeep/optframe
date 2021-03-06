// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;

int rainMain(int argc, char **argv)
{

	cout << "Welcome to rain forecast place" << endl;
	RandGenMersenneTwister rg;
	//long
	long seed = time(nullptr); //CalibrationMode
	seed = 1;
	cout << "Seed = " << seed << endl;
	srand(seed);
	rg.setSeed(seed);

	if (argc != 10)
	{
		cout << "Parametros incorretos!" << endl;
		cout << "Os parametros esperados sao: nome nomeValidationSet saida parameters options precision" << endl;
		exit(1);
	}

	const char* caminho = argv[1];
	const char* caminhoValidation = argv[2];
	const char* caminhoOutput = argv[3];
	const char* caminhoParameters = argv[4];
	int instN = atoi(argv[5]);
	int stepsAhead = atoi(argv[6]);
	int mu = atoi(argv[9]);

	string nome = caminho;
	string nomeValidationSet = caminhoValidation;
	string nomeOutput = caminhoOutput;
	string parametersFile = caminhoParameters;

	//===================================
	cout << "Parametros:" << endl;
	cout << "nome=" << nome << endl;
	cout << "nomeValidationSet=" << nomeValidationSet << endl;
	cout << "nomeOutput=" << nomeOutput << endl;
	cout << "nomeParameters=" << parametersFile << endl;
	cout << "instN=" << instN << endl;
	cout << "stepsAhead=" << stepsAhead << endl;
	cout << "mu=" << mu << endl;
	//======================================

	vector<string> vParametersFiles;
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_124Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_7Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_2Points");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_BasedOnAutoCor");
	vParametersFiles.push_back("./MyProjects/ConfigParameters/JTEuropean/1Column_BasedOnAutoCor2");

	//parametersFile = "./MyProjects/configParametersBestPrice3ColunaseForward";

	vector<string> explanatoryVariables;

	string TriUm = "./MyProjects/EFP/Instance/rain/vitoriaMaxMensal";

	explanatoryVariables.push_back(TriUm);

	treatForecasts rF(explanatoryVariables);

	//vector<vector<double> > batchOfBlindResults; //vector with the blind results of each batch

	/*int beginValidationSet = 0;
	 int nTrainningRoundsValidation = 50;
	 int nValidationSamples = problemParam.getNotUsedForTest() + nTrainningRoundsValidation * stepsAhead;
	 cout << "nValidationSamples = " << nValidationSamples << endl;
	 int nTotalForecastingsValidationSet = nValidationSamples;

	 vector<vector<double> > validationSet; //validation set for calibration
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginValidationSet, nTotalForecastingsValidationSet));
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(1, beginValidationSet, nTotalForecastingsValidationSet + stepsAhead));
	 validationSet.push_back(rF.getPartsForecastsEndToBegin(2, beginValidationSet, nTotalForecastingsValidationSet + stepsAhead));
	 */

	int maxPrecision = 80;



	int lambda = mu * 6;
	double initialDesv;
	double mutationDesv;

	int maxMu = 100;
	int maxInitialDesv = 10;
	int maxMutationDesv = 30;


	int timeES = 120;
	int timeVND = 0;
	int timeILS = 0;
	int timeGRASP = 0;

	vector<vector<double> > vfoIndicatorCalibration; //vector with the FO of each batch

	vector<SolutionEFP> vSolutionsBatches; //vector with the solution of each batch
	int nBatches = 500;
	vector<vector<double> > batchOfResults;
	int stepsAheadR = 12;
	for (int n = 0; n < nBatches; n++)
	{
		int randomPrecision = rg.rand(maxPrecision) + 20;
		int randomParametersFiles = rg.rand(vParametersFiles.size());
		int evalFOMinimizer = rg.rand(EVALUTORS_NMETRICS_ENUM_COUNT); //tree is the number of possible objetive function index minimizers
		int evalAprox = rg.rand(2); //Enayatifar aproximation using previous values
		int construtive = rg.rand(2);
		initialDesv = rg.rand(maxInitialDesv) + 1;
		mutationDesv = rg.rand(maxMutationDesv) + 1;
		mu = rg.rand(maxMu) + 1;
		lambda = mu * 6;
		double alphaACF = rg.rand01(); //limit ACF for construtive ACF
		int alphaSign = rg.rand(2);
		if (alphaSign == 0)
			alphaACF = alphaACF * -1;

		// ============ FORCES ======================
		//mu = 100;
		//lambda = mu * 6;
		//initialDesv = 10;
		//mutationDesv = 20;
		//randomPrecision = 20;
		randomParametersFiles = 1;
		evalFOMinimizer = MAPE_INDEX;
		evalAprox = 0;
		construtive = 2;
		alphaACF = -1;
		cout<<"mu = "<<mu<<endl;
		cout<<"initialDesv = "<<initialDesv<<endl;
		cout<<"mutationDesv = "<<mutationDesv<<endl;
		cout<<"randomPrecision = "<<randomPrecision<<endl;
		// ============ END FORCES ======================

		// ============= METHOD PARAMETERS=================
		HFMParams methodParam;

		//seting up ES params
		methodParam.setESMU(mu);
		methodParam.setESLambda(lambda);
		methodParam.setESInitialDesv(initialDesv);
		methodParam.setESMutationDesv(mutationDesv);
		methodParam.setESMaxG(100000);
		//seting up Construtive params
		methodParam.setConstrutiveMethod(construtive);
		methodParam.setConstrutivePrecision(randomPrecision);
		vector<double> vAlphaACFlimits;
		vAlphaACFlimits.push_back(alphaACF);
		methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);
		//seting up Eval params
		methodParam.setEvalAprox(evalAprox);
		methodParam.setEvalFOMinimizer(evalFOMinimizer);
		// ==========================================

		// ================== READ FILE ============== CONSTRUTIVE 0 AND 1
//		ProblemParameters problemParam(vParametersFiles[randomParametersFiles]); //DEPRECATED
		ProblemParameters problemParam;
		problemParam.setStepsAhead(stepsAheadR);
		stepsAhead = problemParam.getStepsAhead();
		//stepsAhead = 1;
		// =========================================== CONSTRUTIVE 0 AND 1
		//========ADAPTATION FOR CONSTRUTIVE 2 ===============
		int maxNotUsedMAX = 200;
		int maxNotUsedR = rg.rand(maxNotUsedMAX) + 2;
		if (construtive == 2) //ACF construtive
			problemParam.setMaxLag(maxNotUsedR);

		int maxNotUsedForTest = problemParam.getMaxLag(0);

		cout << "maxNotUsedForTest: " << maxNotUsedForTest << endl;

		//getchar();
		//int randomObjFunc = rg.rand(2);
		//cout<<"randomObjFunc = "<<randomObjFunc<<endl;
		//problemParam.setFunction(randomObjFunc);

		//validationBlindForecastings.clear();
		int maxTrainningRounds = 20;
		int nTrainningRounds = rg.rand(maxTrainningRounds) + 1;
		//nTrainningRounds = 10;
		int nTotalForecastingsTrainningSet = maxNotUsedForTest + nTrainningRounds * stepsAhead;
		cout << "nTrainningRounds: " << nTrainningRounds << endl;
		cout << "nTotalForecastingsTrainningSet: " << nTotalForecastingsTrainningSet << endl;

		vector<vector<double> > trainningSet; // trainningSetVector
		int beginTrainingSet = stepsAhead;
		trainningSet.push_back(rF.getPartsForecastsEndToBegin(0, beginTrainingSet, nTotalForecastingsTrainningSet));
		//cout<<trainningSet<<endl;
		//cout<< rF.getForecastsDataSize()<<endl;
		//getchar();

		ForecastClass forecastObject(trainningSet, problemParam, rg, methodParam);

		pair<Solution<RepHFM>, Evaluation>* sol;

		int optMethod = rg.rand(2);
		optMethod = 0;
		if (optMethod == 0)
			sol = forecastObject.run(timeES, timeVND, timeILS);
		else
			sol = forecastObject.runGILS(timeGRASP, timeILS); // GRASP + ILS

		//int needInputs = sol->first.getR().earliestInput;
		int needInputs = problemParam.getMaxLag(0);

		vector<vector<double> > validationSet; //validation set for calibration
		validationSet.push_back(rF.getPartsForecastsEndToBegin(0, 0, needInputs + stepsAhead));
		//validationSet.push_back(validationSetPure);

		double intervalOfBeginTrainningSet = double(beginTrainingSet) / double(rF.getForecastsDataSize());

		batchOfResults.push_back(*forecastObject.returnForecasts(sol, validationSet));

		vector<double> foIndicatorCalibration;
		foIndicatorCalibration = *forecastObject.returnErrors(sol->first.getR(), validationSet);
		foIndicatorCalibration.push_back(randomPrecision);
		foIndicatorCalibration.push_back(randomParametersFiles);
		foIndicatorCalibration.push_back(nTrainningRounds);
		foIndicatorCalibration.push_back(beginTrainingSet);
		foIndicatorCalibration.push_back(intervalOfBeginTrainningSet);
		foIndicatorCalibration.push_back(nTotalForecastingsTrainningSet);
		foIndicatorCalibration.push_back(mu);
		foIndicatorCalibration.push_back(lambda);
		foIndicatorCalibration.push_back(initialDesv);
		foIndicatorCalibration.push_back(mutationDesv);
		foIndicatorCalibration.push_back(timeES);
		foIndicatorCalibration.push_back(timeVND);
		foIndicatorCalibration.push_back(timeILS);
		foIndicatorCalibration.push_back(timeGRASP);
		foIndicatorCalibration.push_back(evalFOMinimizer);
		foIndicatorCalibration.push_back(evalAprox);
		foIndicatorCalibration.push_back(construtive);
		foIndicatorCalibration.push_back(alphaACF);
		foIndicatorCalibration.push_back(optMethod);
		foIndicatorCalibration.push_back(seed);
		//getchar();
		//cout << foIndicatorCalibration << endl;
		vfoIndicatorCalibration.push_back(foIndicatorCalibration);
		vSolutionsBatches.push_back(sol->first);

	}

	vector<vector<double> > finalResultQuantis;

	for (int sa = 0; sa < stepsAheadR; sa++)
	{
		vector<double> distribution;
		for (int n = 0; n < nBatches; n++)
		{
			distribution.push_back(batchOfResults[n][sa]);
		}
		vector<double> quantis;
		quantis = rF.generateQuantis(distribution, 1, 99);
		finalResultQuantis.push_back(quantis);
	}

	rF.exportQuantisVector(finalResultQuantis, "./Quantis12AFrente");

	cout << vfoIndicatorCalibration << endl;
	for (int n = 0; n < nBatches; n++)
	{

		for (int i = 0; i <  (int) vfoIndicatorCalibration[n].size(); i++)
			cout << vfoIndicatorCalibration[n][i] << "\t";

		cout << endl;
	}

	string calibrationFile = "./rainLe";

	FILE* fResults = fopen(calibrationFile.c_str(), "a");
	for (int n = 0; n < nBatches; n++)
	{

		for (int i = 0; i <  (int) vfoIndicatorCalibration[n].size(); i++)
			fprintf(fResults, "%.7f\t", vfoIndicatorCalibration[n][i]);
		fprintf(fResults, "\n");
	}

	fclose(fResults);

	return 0;
}
