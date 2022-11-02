/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HCLASSTRAINDATA
#define HCPP_HCLASSTRAINDATA

namespace HalconCpp
{

// Represents an instance of a training data management class.
class LIntExport HClassTrainData : public HHandle
{

public:

  // Create an uninitialized instance
  HClassTrainData():HHandle() {}

  // Copy constructor
  HClassTrainData(const HClassTrainData& source) : HHandle(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HClassTrainData& operator=(const HClassTrainData&) = default;
#endif

  // Copy constructor
  HClassTrainData(const HHandle& handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('class_train_data')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HClassTrainData Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_class_train_data: Read the training data for classifiers from a file.
  explicit HClassTrainData(const HString& FileName);

  // read_class_train_data: Read the training data for classifiers from a file.
  explicit HClassTrainData(const char* FileName);

#ifdef _WIN32
  // read_class_train_data: Read the training data for classifiers from a file.
  explicit HClassTrainData(const wchar_t* FileName);
#endif

  // create_class_train_data: Create a handle for training data for classifiers.
  explicit HClassTrainData(Hlong NumDim);




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Deserialize serialized training data for classifiers.
  void DeserializeClassTrainData(const HSerializedItem& SerializedItemHandle);

  // Serialize training data for classifiers.
  HSerializedItem SerializeClassTrainData() const;

  // Read the training data for classifiers from a file.
  void ReadClassTrainData(const HString& FileName);

  // Read the training data for classifiers from a file.
  void ReadClassTrainData(const char* FileName);

#ifdef _WIN32
  // Read the training data for classifiers from a file.
  void ReadClassTrainData(const wchar_t* FileName);
#endif

  // Save the training data for classifiers in a file.
  void WriteClassTrainData(const HString& FileName) const;

  // Save the training data for classifiers in a file.
  void WriteClassTrainData(const char* FileName) const;

#ifdef _WIN32
  // Save the training data for classifiers in a file.
  void WriteClassTrainData(const wchar_t* FileName) const;
#endif

  // Select certain features from training data to create training data containing less features.
  HClassTrainData SelectSubFeatureClassTrainData(const HTuple& SubFeatureIndices) const;

  // Define subfeatures in training data.
  void SetFeatureLengthsClassTrainData(const HTuple& SubFeatureLength, const HTuple& Names) const;

  // Get the training data of a Gaussian Mixture Model (GMM).
  void GetClassTrainDataGmm(const HClassGmm& GMMHandle);

  // Add training data to a Gaussian Mixture Model (GMM).
  void AddClassTrainDataGmm(const HClassGmm& GMMHandle) const;

  // Get the training data of a multilayer perceptron (MLP).
  void GetClassTrainDataMlp(const HClassMlp& MLPHandle);

  // Add training data to a multilayer perceptron (MLP).
  void AddClassTrainDataMlp(const HClassMlp& MLPHandle) const;

  // Get the training data of a k-nearest neighbors (k-NN) classifier.
  void GetClassTrainDataKnn(const HClassKnn& KNNHandle);

  // Add training data to a k-nearest neighbors (k-NN) classifier.
  void AddClassTrainDataKnn(const HClassKnn& KNNHandle) const;

  // Get the training data of a support vector machine (SVM).
  void GetClassTrainDataSvm(const HClassSvm& SVMHandle);

  // Add training data to a support vector machine (SVM).
  void AddClassTrainDataSvm(const HClassSvm& SVMHandle) const;

  // Return the number of training samples stored in the training data.
  Hlong GetSampleNumClassTrainData() const;

  // Return a training sample from training data.
  HTuple GetSampleClassTrainData(Hlong IndexSample, Hlong* ClassID) const;

  // Clears training data for classifiers.
  void ClearClassTrainData() const;

  // Add a training sample to training data.
  void AddSampleClassTrainData(const HString& Order, const HTuple& Features, const HTuple& ClassID) const;

  // Add a training sample to training data.
  void AddSampleClassTrainData(const char* Order, const HTuple& Features, const HTuple& ClassID) const;

#ifdef _WIN32
  // Add a training sample to training data.
  void AddSampleClassTrainData(const wchar_t* Order, const HTuple& Features, const HTuple& ClassID) const;
#endif

  // Create a handle for training data for classifiers.
  void CreateClassTrainData(Hlong NumDim);

  // Selects an optimal combination of features to classify the provided data.
  HClassMlp SelectFeatureSetMlp(const HString& SelectionMethod, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination of features to classify the provided data.
  HClassMlp SelectFeatureSetMlp(const HString& SelectionMethod, const HString& GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination of features to classify the provided data.
  HClassMlp SelectFeatureSetMlp(const char* SelectionMethod, const char* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

#ifdef _WIN32
  // Selects an optimal combination of features to classify the provided data.
  HClassMlp SelectFeatureSetMlp(const wchar_t* SelectionMethod, const wchar_t* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;
#endif

  // Selects an optimal combination of features to classify the provided data.
  HClassSvm SelectFeatureSetSvm(const HString& SelectionMethod, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination of features to classify the provided data.
  HClassSvm SelectFeatureSetSvm(const HString& SelectionMethod, const HString& GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination of features to classify the provided data.
  HClassSvm SelectFeatureSetSvm(const char* SelectionMethod, const char* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

#ifdef _WIN32
  // Selects an optimal combination of features to classify the provided data.
  HClassSvm SelectFeatureSetSvm(const wchar_t* SelectionMethod, const wchar_t* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;
#endif

  // Selects an optimal combination from a set of features to classify the provided data.
  HClassGmm SelectFeatureSetGmm(const HString& SelectionMethod, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination from a set of features to classify the provided data.
  HClassGmm SelectFeatureSetGmm(const HString& SelectionMethod, const HString& GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal combination from a set of features to classify the provided data.
  HClassGmm SelectFeatureSetGmm(const char* SelectionMethod, const char* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

#ifdef _WIN32
  // Selects an optimal combination from a set of features to classify the provided data.
  HClassGmm SelectFeatureSetGmm(const wchar_t* SelectionMethod, const wchar_t* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;
#endif

  // Selects an optimal subset from a set of features to solve a certain classification problem.
  HClassKnn SelectFeatureSetKnn(const HString& SelectionMethod, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal subset from a set of features to solve a certain classification problem.
  HClassKnn SelectFeatureSetKnn(const HString& SelectionMethod, const HString& GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

  // Selects an optimal subset from a set of features to solve a certain classification problem.
  HClassKnn SelectFeatureSetKnn(const char* SelectionMethod, const char* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;

#ifdef _WIN32
  // Selects an optimal subset from a set of features to solve a certain classification problem.
  HClassKnn SelectFeatureSetKnn(const wchar_t* SelectionMethod, const wchar_t* GenParamName, double GenParamValue, HTuple* SelectedFeatureIndices, HTuple* Score) const;
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HClassTrainData> HClassTrainDataArrayRef;
typedef HSmartPtr< HClassTrainDataArrayRef > HClassTrainDataArrayPtr;


// Represents multiple tool instances
class LIntExport HClassTrainDataArray : public HHandleBaseArray
{

public:

  // Create empty array
  HClassTrainDataArray();

  // Create array from native array of tool instances
  HClassTrainDataArray(HClassTrainData* classes, Hlong length);

  // Copy constructor
  HClassTrainDataArray(const HClassTrainDataArray &tool_array);

  // Destructor
  virtual ~HClassTrainDataArray();

  // Assignment operator
  HClassTrainDataArray &operator=(const HClassTrainDataArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HClassTrainData* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HClassTrainDataArrayPtr *mArrayPtr;
};

}

#endif
