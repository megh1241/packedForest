#ifndef fpInfo_h
#define fpInfo_h

#include <string>
#include <memory>
#include <math.h>
#include <iostream>
#include "../baseFunctions/MWC.h"

namespace fp {

	/**
	 * fpInfo holds all parameters for the forests.  These parameters should be
	 * set before calling fpForest to run forest creation.
	 */

	class fpInfo{

		protected:
			int numTreesInForest;
			int minParent;
			int numClasses;
			int numObservations;
			int numFeatures;
			int mtry;
			int columnWithY;

			int numberOfNodes;
			int maxDepth;
			int sumLeafNodeDepths;

			double fractionOfFeaturesToTest;

			int binSize;
			int numCores;

			double double_epsilon;
			float float_epsilon;

			std::string forestType;
			std::string CSVFileName;

			int seed;
			randomNumberRerFMWC randNum;

			int numTreeBins;


		public:
			inline int returnNumTreeBins(){
				return numTreeBins;
			}

			inline void setNumTreeBins(int numTB){
				numTreeBins = numTB;
			}

			inline float returnFloatEpsilon(){
				return float_epsilon;
			}

			inline double returnDoubleEpsilon(){
				return double_epsilon;
			}

			inline bool returnUseBinning(){
				return binSize;
			}

			inline int returnBinSize(){
				return binSize;
			}

			inline std::string& returnCSVFileName(){
				return CSVFileName;
			}

			inline std::string& returnForestType(){
				return forestType;
			}

			inline int returnColumnWithY() const{
				return columnWithY;
			}

			inline int returnNumClasses() const{
				return numClasses;
			}

			inline int returnMinParent() const{
				return minParent;
			}

			inline int returnNumFeatures() const{
				return numFeatures;
			}
			inline int returnNumObservations() const{
				return numObservations;
			}

			inline void setNumClasses(const int& numC){
				numClasses = numC;
			}

			inline void setNumFeatures(const int& numF){
				numFeatures = numF;
			}

			inline void setNumObservations(const int& numO){
				numObservations = numO;
			}

			inline int returnNumTrees() const{
				return numTreesInForest;
			}

			inline int returnNumThreads() const{
				return numCores;
			}

			inline bool loadDataFromCSV(){
				if(!CSVFileName.empty() || columnWithY != -1){
					return true;
				}
				return false;
			}

			inline int returnMtry(){
				return mtry;
			}
			inline bool useDefaultMTRY(){
				return fractionOfFeaturesToTest < 0;
			}

			inline void setMTRY(){
				if(mtry == -1){
					if(useDefaultMTRY()){
						mtry = sqrt(numFeatures);
					}else{
						mtry = fractionOfFeaturesToTest * numFeatures;
					}
				}
			}

			////////////////////////////////////////
			//Random Number Generator
			///////////////////////////////////////
			inline void initRandom(){
				std::random_device rd;
				if(seed == -1){
					seed = rd();
				}
				randNum.initialize(seed);
			}

			inline int genRandom(int range){
				return randNum.gen(range);
			}

			///////////////////////////////////////
			//
			//////////////////////////////////////





			//////
			//This is from CPP.
			/////////
			fpInfo(): numTreesInForest(100),
			minParent(1),	numClasses(-1), numObservations(-1), numFeatures(-1),
			mtry(-1), columnWithY(-1), 
			numberOfNodes(0), maxDepth(0),sumLeafNodeDepths(0), fractionOfFeaturesToTest(-1.0), binSize(0), numCores(1), double_epsilon(0.0000001), float_epsilon(0.0000001),seed(-1),numTreeBins(-1){}



			void setParameter(const std::string& parameterName, const std::string& parameterValue){
				if(parameterName == "forestType"){
					forestType = parameterValue;
				}else if(parameterName == "CSVFileName"){
					CSVFileName = parameterValue;
				}else{
					throw std::runtime_error("Unknown parameter type.(string)");
				}
			}


			void setParameter(const std::string& parameterName, const double parameterValue){
				if(parameterName == "numTreesInForest"){
					numTreesInForest = (int)parameterValue;
				}else if(parameterName == "minParent"){
					minParent = (int)parameterValue;
				}else if(parameterName == "numClasses"){
					numClasses = (int)parameterValue;
				}else if(parameterName == "mtry"){
					mtry = (int)parameterValue;
				}else if(parameterName == "fractionOfFeaturesToTest"){
					fractionOfFeaturesToTest = parameterValue;
				}else if(parameterName == "columnWithY"){
					columnWithY = (int)parameterValue;
				}else if(parameterName == "useBinning"){
					binSize = (int)parameterValue;
				}else if(parameterName == "numCores"){
					numCores = (int)parameterValue;
				}else if(parameterName == "seed"){
					seed = (int)parameterValue;
				}else if(parameterName == "numTreeBins"){
					numTreeBins = (int)parameterValue;
				}else {
					throw std::runtime_error("Unknown parameter type.(double)");
				}
			}


			void setParameter(const std::string& parameterName, const int parameterValue){
				if(parameterName == "numTreesInForest"){
					numTreesInForest = parameterValue;
				}else if(parameterName == "minParent"){
					minParent = parameterValue;
				}else if(parameterName == "numClasses"){
					numClasses = parameterValue;
				}else if(parameterName == "mtry"){
					mtry = parameterValue;
				}else if(parameterName == "fractionOfFeaturesToTest"){
					fractionOfFeaturesToTest = (double)parameterValue;
				}else if(parameterName == "columnWithY"){
					columnWithY = parameterValue;
				}else if(parameterName == "useBinning"){
					binSize = parameterValue;
				}else if(parameterName == "numCores"){
					numCores = parameterValue;
				}else if(parameterName == "seed"){
					seed = parameterValue;
				}else if(parameterName == "numTreeBins"){
					numTreeBins = parameterValue;
				}else {
					throw std::runtime_error("Unknown parameter type.(int)");
				}
			}

			void printAllParameters(){
				std::cout << "numTreesInForest -> " << numTreesInForest << "\n";
				std::cout << "minParent -> " << minParent << "\n";
				std::cout << "numClasses -> " << numClasses << "\n";
				std::cout << "numObservations -> " << numObservations << "\n";
				std::cout << "numFeatures -> " << numFeatures << "\n";
				std::cout << "mtry -> " << mtry << "\n";
				std::cout << "fractionOfFeaturesToTest -> " << fractionOfFeaturesToTest << "\n";
				std::cout << "CSV file name -> " <<  CSVFileName << "\n";
				std::cout << "columnWithY -> " << columnWithY << "\n";
				std::cout << "Type of Forest -> " << forestType << "\n";
				std::cout << "binSize -> " << binSize << "\n";
				std::cout << "numCores -> " << numCores << "\n";
				std::cout << "seed -> " << seed << "\n";
				std::cout << "numTreeBins -> " << numTreeBins << "\n";
			}


			void printForestType(){
				std::cout << forestType << "\n";
			}

	}; // class fpInfo
} //namespace fp
#endif //fpInfo.h
