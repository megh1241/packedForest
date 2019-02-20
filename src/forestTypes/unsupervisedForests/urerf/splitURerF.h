#ifndef splitURerF_h
#define splitURerF_h

#include "splitURerFInfo.h"
#include "../labeledData.h"
#include "../unLabeledData.h"
#include "../classTotals.h"
#include "../../../baseFunctions/pdqsort.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <assert.h>
#include <utility>
namespace fp{
	template<typename T>
		class splitURerF{
			protected:
				double overallImpurity;
				classTotals leftClasses;
				classTotals rightClasses;

				std::vector<int> labels;
				std::vector< labeledData<T> > combinedDataLabels;
				std::vector<T> featureValsVec;

				static T computeSampleVariance(const double mean, const std::vector<T>& v)
				{
					double accum = 0.0;
					std::for_each (std::begin(v), std::end(v), [&](const double d) {
    						accum += (d - mean) * (d - mean);
					});
    					/*auto const add_square = [mean](double sum, int i) {
        					auto d = i - mean;
        					return sum + d*d;
    					};
    					double total = std::accumulate(numbers.begin(), numbers.end(), 0.0, add_square);*/
    					//return total / (numbers.size() - 1);
    					return accum;
				}

				inline void setCombinedVecSize(){
					combinedDataLabels.resize(labels.size());
				}

				inline void createData(const std::vector<T> featureVals){
					std::cout<<"Inside createData\n";
					featureValsVec.clear();
					auto siz_vec = featureVals.size();
					std::cout<<"size: "<<siz_vec<<"\n";
					std::cout<<"**********************************"<<"\n";
					for(unsigned int i=0; i<siz_vec; ++i){
						featureValsVec.push_back(featureVals.at(i));
					}
					std::sort(featureValsVec.begin(), featureValsVec.end());
					for(unsigned int i=0; i<siz_vec; ++i){
						std::cout<<featureValsVec.at(i)<<", ";
					}
					std::cout<<"size 1: "<<featureValsVec.size()<<"\n";
					std::cout<<"**********************************"<<"\n";
					std::cout<<"\n";
				}


				inline T midVal(int leftIndex){
					return combinedDataLabels[leftIndex].midVal(combinedDataLabels[leftIndex+1]);
				}

				inline bool checkInequalityWithNext(int leftIndex){
					return combinedDataLabels[leftIndex].checkInequality(combinedDataLabels[leftIndex+1]);
				}

				inline void setupForNextRun(){
					//setup for next possible run
					leftClasses.incrementClass(combinedDataLabels.back().returnDataLabel());
					rightClasses.decrementClass(combinedDataLabels.back().returnDataLabel());

					assert(rightClasses.returnNumItems() == 0);
					std::swap(rightClasses, leftClasses);
					assert(leftClasses.returnNumItems() == 0);
				}

			public:
				splitURerF(const std::vector<int>& labelVector): labels(labelVector){
					rightClasses.findNumClasses(labels);
					leftClasses.setClassVecSize(rightClasses.returnClassVecSize());
					setCombinedVecSize();
					overallImpurity = rightClasses.calcAndReturnImpurity();
				}

				inline double returnImpurity(){
					return overallImpurity;
				}


				inline splitURerFInfo<T> giniSplit(const std::vector<T>& featureVal, const std::vector<int>& featureNums){
                                        // initialize return value
                                        splitURerFInfo<T> currSplitInfo;
					std::cout<<"size of featureVal vector before"<<featureVal.size()<<"\n";
                                        createData(featureVal);

                                        // sort feature Vals
					std::cout<<"*********************************************************************\n";
                                        std::vector<T> errVec;
                                        std::vector<T> errVecLeft;
                                        std::vector<T> errVecRight;
                                        auto pbegin = featureValsVec.begin();
					auto pend = featureValsVec.end();
                        		std::sort(pbegin, pend);
                                        pbegin = featureValsVec.begin();
					pend = featureValsVec.end();
					std::cout<<"size 2: "<<featureValsVec.size()<<"\n";
                                        int sizeX = featureValsVec.size();
					featureValsVec.erase(std::remove(pbegin, pend, 0), pend);
					std::cout<<"size 3: "<<featureValsVec.size()<<"\n";
                                        int sizeNNZ = featureValsVec.size();
                                        int sizeZ = sizeX - sizeNNZ;
                                        T meanRight, sumLeft=0, meanLeft, cutPoint=0;
                                        T minErr = std::numeric_limits<T>::infinity();
                                        T minErrLeft = std::numeric_limits<T>::infinity();
                                        T minErrRight = std::numeric_limits<T>::infinity();
					T sumRight = std::accumulate(featureValsVec.begin(), featureValsVec.end(), 0.0);
					std::cout<<"size 4: "<<featureValsVec.size()<<"\n";
					std::cout<<"initial sumRight: "<<sumRight<<"\n";
					pbegin = featureValsVec.begin();
					pend = featureValsVec.end();

					  if (sizeZ) {
                                                        meanRight = sumRight / sizeNNZ;
                                                        std::transform(featureValsVec.begin(), featureValsVec.end(), errVec.begin(), [meanRight](double x) { return (x - meanRight)*(x-meanRight); });
                                                        minErr = std::accumulate(errVec.begin(), errVec.end(),0);
                                                        cutPoint = featureValsVec.at(0) / 2;
                                          }


                                          if (sizeNNZ - 1) {
                                                        int index = 1;
							int sizeIt = featureValsVec.size()-1;
							std::cout<<"sizeIT: "<<sizeIt<<"\n";
                                                        for(int iter = 0; iter < sizeIt; ++iter) {
                                                                int leftSize = sizeZ + index;
                                                                int rightSize = sizeNNZ - index;
                                                                sumLeft = sumLeft + featureValsVec[iter];
                                                                sumRight = sumRight - featureValsVec[iter];
                                                                meanLeft = sumLeft / leftSize;
                                                                meanRight = sumRight / rightSize;
								auto last = pbegin;
								std::advance(last, index);
								std::vector<T> newVec(pbegin, last);
//auto errLeft = computeSampleVariance(meanLeft, newVec) + (sizeZ * meanLeft * meanLeft);
								auto errLeft = computeSampleVariance(meanLeft, newVec);
								std::vector<T> newVec2(last, pend);
								auto errRight = computeSampleVariance(meanRight, newVec2);
                                                        	auto errCurr = errLeft + errRight;

								if(index < 10){
									std::cout<<"**********************\n";
									std::cout<<"**********************\n";
									std::cout<<"leftArray: "<<"\n";
									for (auto iter2 = newVec.begin(); iter2 != newVec.end(); iter2++)
										std::cout<<*iter2<<" ";
									std::cout<<"\n";
									std::cout<<"rightArray: "<<"\n";
									for (auto iter3 = newVec2.begin(); iter3 != newVec2.end(); iter3++)
										std::cout<<*iter3<<" ";
									std::cout<<"\n";
									std::cout<<"leftsize: "<<leftSize<<"\n";
									std::cout<<"rightsize: "<<rightSize<<"\n";
									std::cout<<"sumLeft: "<<sumLeft<<"\n";
									std::cout<<"sumRight: "<<sumRight<<"\n";
									std::cout<<"meanLeft: "<<meanLeft<<"\n";
									std::cout<<"meanRight: "<<meanRight<<"\n";
									std::cout<<"errLeft: "<<errLeft<<"\n";
									std::cout<<"errRight: "<<errRight<<"\n";
									std::cout<<"**********************\n";
									std::cout<<"**********************\n";
								}

                                                                if (errCurr < minErr) {
									
                                                                        cutPoint = (featureValsVec[iter] + featureValsVec[iter+1]) / 2;
                                                                        minErrLeft = errLeft;
                                                                        minErrRight = errRight;
                                                                        minErr = errCurr;
                                                                }
                                                                ++index;
                                                        }
                                          }
					  currSplitInfo.setImpurity(minErr);
                                          currSplitInfo.setSplitValue(cutPoint);
                                          currSplitInfo.setLeftImpurity(minErrLeft);
                                          currSplitInfo.setRightImpurity(minErrRight);
                                          currSplitInfo.addFeatureNums(featureNums);	
					std::cout<<"*********************************************************************\n";

					 return currSplitInfo;
				}
		};

}//namespace fp
#endif //splitRerF_h
