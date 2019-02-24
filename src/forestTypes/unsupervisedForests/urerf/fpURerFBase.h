#ifndef fpRerF_h
#define fpRerf_h

#include "../../../baseFunctions/fpForestBase.h"
#include <vector>
#include <map>
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include "urerfTree.h"

namespace fp {

        bool sortbysec(const std::pair<int,int> &a, const std::pair<int,int> &b)
                {
                        return (a.second > b.second);
                }

	template <typename T>
		class fpURerFBase : public fpForestBase<T>
	{
		protected:
			std::vector<urerfTree<T> > trees;
			std::map<std::pair<int, int>, int > simMat;
		public:

			~fpURerFBase(){}

			fpDisplayProgress printProgress;
			inline void printForestType(){
				std::cout << "This is a urerf forest.\n";
			}

			inline void changeForestSize(){
				trees.resize(fpSingleton::getSingleton().returnNumTrees());
			}

			inline void growTrees(){
#pragma omp parallel for num_threads(fpSingleton::getSingleton().returnNumThreads())
				for(int i = 0; i < (int)trees.size(); ++i){
					printProgress.displayProgress(i);
					trees[i].growTree();
					trees[i].updateSimMat(simMat);
				}
				std::cout << " done growing forest.\n"<< std::flush;
			}

			inline void checkParameters(){
				//TODO: check parameters to make sure they make sense for this forest type.
				;
			}


                        inline double precisionPerRow(int row, int k) {
                                // show content:
                                std::vector<std::pair<int, int> > sim;
                                for (auto it=simMat.begin(); it!=simMat.end(); ++it)
                                {
                                        if ((it->first).first == row)
                                        {
                                                sim.push_back(std::make_pair((it->first).second, it->second));
                                        }
                                }
                                std::sort(sim.begin(), sim.end(), sortbysec);
                                double numCorrect = 0;
                                int labelCurr = fpSingleton::getSingleton().returnLabel(row), labelNeighbor=0;
                                for(int i = 0; i< k; i++)
                                {
                                        labelNeighbor = fpSingleton::getSingleton().returnLabel(sim[i].first);
                                        if (labelCurr == labelNeighbor)
                                                numCorrect++;
                                }
                                return numCorrect/(double)k;

                        }

                        inline double computePrecision(int k) {
                        // show content:
                                // show content:
                                double prec = 0;
                                auto numObs = fpSingleton::getSingleton().returnNumObservations();
                                for(int i = 0; i < numObs; ++i)
                                {
                                        prec += precisionPerRow(i, k);
                                }
                                return prec/ numObs;
                        }



			inline void treeStats(){
				int maxDepth=0;
				int totalLeafNodes=0;
				int totalLeafDepth=0;

				int tempMaxDepth;
				for(int i = 0; i < fpSingleton::getSingleton().returnNumTrees(); ++i){
					tempMaxDepth = trees[i].returnMaxDepth();
					maxDepth = ((maxDepth < tempMaxDepth) ? tempMaxDepth : maxDepth);

					totalLeafNodes += trees[i].returnNumLeafNodes();
					totalLeafDepth += trees[i].returnLeafDepthSum();
				}

				std::cout << "max depth: " << maxDepth << "\n";
				std::cout << "avg leaf depth: " << float(totalLeafDepth)/float(totalLeafNodes) << "\n";
				std::cout << "num leaf nodes: " << totalLeafNodes << "\n";

				for(int i = 5; i<55; i+=5)
						std::cout << "precision: " << computePrecision(i) << "\n";
			}

			void printTree0(){
				trees[0].printTree();
			}

			void growForest(){
				changeForestSize();
				growTrees();
				treeStats();
			}

			inline int predictClass(int observationNumber){
				std::vector<int> classTally(fpSingleton::getSingleton().returnNumClasses(),0);
				for(int i = 0; i < fpSingleton::getSingleton().returnNumTrees(); ++i){
					++classTally[trees[i].predictObservation(observationNumber)];
				}
				int bestClass = 0;
				for(int j = 1; j < fpSingleton::getSingleton().returnNumClasses(); ++j){
					if(classTally[bestClass] < classTally[j]){
						bestClass = j;
					}
				}
				return bestClass;
			}

			inline int predictClass(std::vector<T>& observation){
				std::vector<int> classTally(fpSingleton::getSingleton().returnNumClasses(),0);
				for(int i = 0; i < fpSingleton::getSingleton().returnNumTrees(); ++i){
					++classTally[trees[i].predictObservation(observation)];
				}
				int bestClass = 0;
				for(int j = 1; j < fpSingleton::getSingleton().returnNumClasses(); ++j){
					if(classTally[bestClass] < classTally[j]){
						bestClass = j;
					}
				}
				return bestClass;
			}


inline int predictClass(const T* observation){
	/*
				std::vector<int> classTally(fpSingleton::getSingleton().returnNumClasses(),0);
				for(int i = 0; i < fpSingleton::getSingleton().returnNumTrees(); ++i){
					++classTally[trees[i].predictObservation(observation)];
				}
				int bestClass = 0;
				for(int j = 1; j < fpSingleton::getSingleton().returnNumClasses(); ++j){
					if(classTally[bestClass] < classTally[j]){
						bestClass = j;
					}
				}
				return bestClass;
				*/
	return 0;
			}


			inline float testForest(){
				int numTried = 0;
				int numWrong = 0;

				for (int i = 0; i <fpSingleton::getSingleton().returnNumObservations();i++){
					++numTried;
					int predClass = predictClass(i);

					if(predClass != fpSingleton::getSingleton().returnTestLabel(i)){
						++numWrong;
					}
				}
				std::cout << "\nnumWrong= " << numWrong << "\n";

				return (float)numWrong/(float)numTried;
			}
	};

}// namespace fp
#endif //fpForestClassification_h
