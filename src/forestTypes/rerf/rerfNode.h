#ifndef rerfNode_h
#define rerfNode_h

#include "../../baseFunctions/fpBaseNode.h"
#include <stdio.h>
#include <vector>

template <typename T>
class rerfNode : public fpBaseNode<T>
{
	protected:
		std::vector<int> feature;

	public:
		rerfNode(){}

		inline void addFeatureValue(int fVal){
			feature.push_back(fVal);
		}

		inline void setFeatureValue(std::vector<int> fVal){
			feature = fVal;
		}

		inline std::vector<int>& returnFeatureNumber(){
			return feature;
		}

    inline int nextNode(std::vector<T>& observation){
			//TODO
T featureVal = 0;
			for(unsigned int j = 0; j < feature.size(); ++j){
							featureVal += observation[feature[j]];
						}
			return (featureVal < fpBaseNode<T>::cutValue) ? fpBaseNode<T>::left : fpBaseNode<T>::right;
		}

    inline int nextNode(double featureVal){
			return (featureVal < fpBaseNode<T>::cutValue) ? fpBaseNode<T>::left : fpBaseNode<T>::right;
		}
};
#endif //padNode_h
