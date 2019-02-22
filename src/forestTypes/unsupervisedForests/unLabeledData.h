#ifndef unLabeledData_h
#define unLabeledData_h

#include <assert.h>
#include <iomanip>
#include <cmath>

#define FLOAT_EPSILON  .0000001
#define DOUBLE_EPSILON .0000001


namespace fp{
	template<typename T>
		class alignas(16)unLabeledData
		{
			protected:
				T dataElement;

			public:

				inline bool operator < (const unLabeledData<T>& otherData) const
				{
					return dataElement < otherData.dataElement;
				}


				inline T returnDataElement(){
					return dataElement;
				}

				inline T midVal (const unLabeledData<T>& otherData)
				{
					assert(dataElement != otherData.dataElement);
					assert(dataElement != (dataElement+otherData.dataElement)/2.0);
					assert(otherData.dataElement != (dataElement+otherData.dataElement)/2.0);
					return (dataElement + otherData.dataElement)/2.0;
				}

				inline bool checkInequality(const unLabeledData<double>& otherData){
					return std::abs(dataElement - otherData.dataElement) > DOUBLE_EPSILON;
				}

				inline bool checkInequality(const unLabeledData<float>& otherData){
					return std::abs(dataElement - otherData.dataElement) > FLOAT_EPSILON;
				}

				inline bool checkInequality(const unLabeledData<int>& otherData){
					return dataElement != otherData.dataElement;
				}

				inline void setElem(const T dElement){
					dataElement = dElement;
				}
		};

}//namespace fp
#endif //labeledData_h
