/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HMETROLOGYMODEL
#define HCPP_HMETROLOGYMODEL

namespace HalconCpp
{

// Represents an instance of a metrology model.
class LIntExport HMetrologyModel : public HHandle
{

public:

  // Copy constructor
  HMetrologyModel(const HMetrologyModel& source) : HHandle(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HMetrologyModel& operator=(const HMetrologyModel&) = default;
#endif

  // Copy constructor
  HMetrologyModel(const HHandle& handle);

  // Create HMetrologyModel from handle, taking ownership
  explicit HMetrologyModel(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('metrology_model')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HMetrologyModel Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_metrology_model: Read a metrology model from a file.
  explicit HMetrologyModel(const HString& FileName);

  // read_metrology_model: Read a metrology model from a file.
  explicit HMetrologyModel(const char* FileName);

#ifdef _WIN32
  // read_metrology_model: Read a metrology model from a file.
  explicit HMetrologyModel(const wchar_t* FileName);
#endif

  // create_metrology_model: Create the data structure that is needed to measure geometric shapes.
  explicit HMetrologyModel();




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Query the model contour of a metrology object in image coordinates. 
  HXLDCont GetMetrologyObjectModelContour(const HTuple& Index, double Resolution) const;

  // Query the model contour of a metrology object in image coordinates. 
  HXLDCont GetMetrologyObjectModelContour(Hlong Index, double Resolution) const;

  // Query the result contour of a metrology object. 
  HXLDCont GetMetrologyObjectResultContour(const HTuple& Index, const HTuple& Instance, double Resolution) const;

  // Query the result contour of a metrology object. 
  HXLDCont GetMetrologyObjectResultContour(Hlong Index, const HString& Instance, double Resolution) const;

  // Query the result contour of a metrology object. 
  HXLDCont GetMetrologyObjectResultContour(Hlong Index, const char* Instance, double Resolution) const;

#ifdef _WIN32
  // Query the result contour of a metrology object. 
  HXLDCont GetMetrologyObjectResultContour(Hlong Index, const wchar_t* Instance, double Resolution) const;
#endif

  // Alignment of a metrology model.
  void AlignMetrologyModel(const HTuple& Row, const HTuple& Column, const HTuple& Angle) const;

  // Alignment of a metrology model.
  void AlignMetrologyModel(double Row, double Column, double Angle) const;

  // Add a metrology object to a metrology model.
  Hlong AddMetrologyObjectGeneric(const HTuple& Shape, const HTuple& ShapeParam, const HTuple& MeasureLength1, const HTuple& MeasureLength2, const HTuple& MeasureSigma, const HTuple& MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a metrology object to a metrology model.
  Hlong AddMetrologyObjectGeneric(const HString& Shape, const HTuple& ShapeParam, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a metrology object to a metrology model.
  Hlong AddMetrologyObjectGeneric(const char* Shape, const HTuple& ShapeParam, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

#ifdef _WIN32
  // Add a metrology object to a metrology model.
  Hlong AddMetrologyObjectGeneric(const wchar_t* Shape, const HTuple& ShapeParam, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;
#endif

  // Get parameters that are valid for the entire metrology model.
  HTuple GetMetrologyModelParam(const HString& GenParamName) const;

  // Get parameters that are valid for the entire metrology model.
  HTuple GetMetrologyModelParam(const char* GenParamName) const;

#ifdef _WIN32
  // Get parameters that are valid for the entire metrology model.
  HTuple GetMetrologyModelParam(const wchar_t* GenParamName) const;
#endif

  // Set parameters that are valid for the entire metrology model.
  void SetMetrologyModelParam(const HString& GenParamName, const HTuple& GenParamValue) const;

  // Set parameters that are valid for the entire metrology model.
  void SetMetrologyModelParam(const HString& GenParamName, const HString& GenParamValue) const;

  // Set parameters that are valid for the entire metrology model.
  void SetMetrologyModelParam(const char* GenParamName, const char* GenParamValue) const;

#ifdef _WIN32
  // Set parameters that are valid for the entire metrology model.
  void SetMetrologyModelParam(const wchar_t* GenParamName, const wchar_t* GenParamValue) const;
#endif

  // Deserialize a serialized metrology model.
  void DeserializeMetrologyModel(const HSerializedItem& SerializedItemHandle);

  // Serialize a metrology model.
  HSerializedItem SerializeMetrologyModel() const;

  // Transform metrology objects of a metrology model, e.g., for alignment.
  void TransformMetrologyObject(const HTuple& Index, const HTuple& Row, const HTuple& Column, const HTuple& Phi, const HTuple& Mode) const;

  // Transform metrology objects of a metrology model, e.g., for alignment.
  void TransformMetrologyObject(const HString& Index, double Row, double Column, double Phi, const HString& Mode) const;

  // Transform metrology objects of a metrology model, e.g., for alignment.
  void TransformMetrologyObject(const char* Index, double Row, double Column, double Phi, const char* Mode) const;

#ifdef _WIN32
  // Transform metrology objects of a metrology model, e.g., for alignment.
  void TransformMetrologyObject(const wchar_t* Index, double Row, double Column, double Phi, const wchar_t* Mode) const;
#endif

  // Write a metrology model to a file.
  void WriteMetrologyModel(const HString& FileName) const;

  // Write a metrology model to a file.
  void WriteMetrologyModel(const char* FileName) const;

#ifdef _WIN32
  // Write a metrology model to a file.
  void WriteMetrologyModel(const wchar_t* FileName) const;
#endif

  // Read a metrology model from a file.
  void ReadMetrologyModel(const HString& FileName);

  // Read a metrology model from a file.
  void ReadMetrologyModel(const char* FileName);

#ifdef _WIN32
  // Read a metrology model from a file.
  void ReadMetrologyModel(const wchar_t* FileName);
#endif

  // Copy a metrology model.
  Hlong CopyMetrologyModel(const HTuple& Index) const;

  // Copy a metrology model.
  Hlong CopyMetrologyModel(const HString& Index) const;

  // Copy a metrology model.
  Hlong CopyMetrologyModel(const char* Index) const;

#ifdef _WIN32
  // Copy a metrology model.
  Hlong CopyMetrologyModel(const wchar_t* Index) const;
#endif

  // Copy metrology metrology objects of a metrology model.
  HTuple CopyMetrologyObject(const HTuple& Index) const;

  // Copy metrology metrology objects of a metrology model.
  Hlong CopyMetrologyObject(const HString& Index) const;

  // Copy metrology metrology objects of a metrology model.
  Hlong CopyMetrologyObject(const char* Index) const;

#ifdef _WIN32
  // Copy metrology metrology objects of a metrology model.
  Hlong CopyMetrologyObject(const wchar_t* Index) const;
#endif

  // Get the number of instances of the metrology objects of a metrology model.
  HTuple GetMetrologyObjectNumInstances(const HTuple& Index) const;

  // Get the number of instances of the metrology objects of a metrology model.
  double GetMetrologyObjectNumInstances(Hlong Index) const;

  // Get the results of the measurement of a metrology model.
  HTuple GetMetrologyObjectResult(const HTuple& Index, const HTuple& Instance, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Get the results of the measurement of a metrology model.
  HTuple GetMetrologyObjectResult(Hlong Index, const HString& Instance, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Get the results of the measurement of a metrology model.
  HTuple GetMetrologyObjectResult(Hlong Index, const char* Instance, const HTuple& GenParamName, const HTuple& GenParamValue) const;

#ifdef _WIN32
  // Get the results of the measurement of a metrology model.
  HTuple GetMetrologyObjectResult(Hlong Index, const wchar_t* Instance, const HTuple& GenParamName, const HTuple& GenParamValue) const;
#endif

  // Get the measure regions and the results of the edge location for the metrology objects of a metrology model.
  HXLDCont GetMetrologyObjectMeasures(const HTuple& Index, const HString& Transition, HTuple* Row, HTuple* Column) const;

  // Get the measure regions and the results of the edge location for the metrology objects of a metrology model.
  HXLDCont GetMetrologyObjectMeasures(const HString& Index, const HString& Transition, HTuple* Row, HTuple* Column) const;

  // Get the measure regions and the results of the edge location for the metrology objects of a metrology model.
  HXLDCont GetMetrologyObjectMeasures(const char* Index, const char* Transition, HTuple* Row, HTuple* Column) const;

#ifdef _WIN32
  // Get the measure regions and the results of the edge location for the metrology objects of a metrology model.
  HXLDCont GetMetrologyObjectMeasures(const wchar_t* Index, const wchar_t* Transition, HTuple* Row, HTuple* Column) const;
#endif

  // Measure and fit the geometric shapes of all metrology objects of a metrology model.
  void ApplyMetrologyModel(const HImage& Image) const;

  // Get the indices of the metrology objects of a metrology model.
  HTuple GetMetrologyObjectIndices() const;

  // Reset all fuzzy parameters and fuzzy functions of a metrology model.
  void ResetMetrologyObjectFuzzyParam(const HTuple& Index) const;

  // Reset all fuzzy parameters and fuzzy functions of a metrology model.
  void ResetMetrologyObjectFuzzyParam(const HString& Index) const;

  // Reset all fuzzy parameters and fuzzy functions of a metrology model.
  void ResetMetrologyObjectFuzzyParam(const char* Index) const;

#ifdef _WIN32
  // Reset all fuzzy parameters and fuzzy functions of a metrology model.
  void ResetMetrologyObjectFuzzyParam(const wchar_t* Index) const;
#endif

  // Reset all parameters of a metrology model.
  void ResetMetrologyObjectParam(const HTuple& Index) const;

  // Reset all parameters of a metrology model.
  void ResetMetrologyObjectParam(const HString& Index) const;

  // Reset all parameters of a metrology model.
  void ResetMetrologyObjectParam(const char* Index) const;

#ifdef _WIN32
  // Reset all parameters of a metrology model.
  void ResetMetrologyObjectParam(const wchar_t* Index) const;
#endif

  // Get a fuzzy parameter of a metroloy model.
  HTuple GetMetrologyObjectFuzzyParam(const HTuple& Index, const HTuple& GenParamName) const;

  // Get a fuzzy parameter of a metroloy model.
  HTuple GetMetrologyObjectFuzzyParam(const HString& Index, const HTuple& GenParamName) const;

  // Get a fuzzy parameter of a metroloy model.
  HTuple GetMetrologyObjectFuzzyParam(const char* Index, const HTuple& GenParamName) const;

#ifdef _WIN32
  // Get a fuzzy parameter of a metroloy model.
  HTuple GetMetrologyObjectFuzzyParam(const wchar_t* Index, const HTuple& GenParamName) const;
#endif

  // Get one or several parameters of a metrology model.
  HTuple GetMetrologyObjectParam(const HTuple& Index, const HTuple& GenParamName) const;

  // Get one or several parameters of a metrology model.
  HTuple GetMetrologyObjectParam(const HString& Index, const HTuple& GenParamName) const;

  // Get one or several parameters of a metrology model.
  HTuple GetMetrologyObjectParam(const char* Index, const HTuple& GenParamName) const;

#ifdef _WIN32
  // Get one or several parameters of a metrology model.
  HTuple GetMetrologyObjectParam(const wchar_t* Index, const HTuple& GenParamName) const;
#endif

  // Set fuzzy parameters or fuzzy functions for a metrology model.
  void SetMetrologyObjectFuzzyParam(const HTuple& Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Set fuzzy parameters or fuzzy functions for a metrology model.
  void SetMetrologyObjectFuzzyParam(const HString& Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Set fuzzy parameters or fuzzy functions for a metrology model.
  void SetMetrologyObjectFuzzyParam(const char* Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

#ifdef _WIN32
  // Set fuzzy parameters or fuzzy functions for a metrology model.
  void SetMetrologyObjectFuzzyParam(const wchar_t* Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;
#endif

  // Set parameters for the metrology objects of a metrology model.
  void SetMetrologyObjectParam(const HTuple& Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Set parameters for the metrology objects of a metrology model.
  void SetMetrologyObjectParam(const HString& Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Set parameters for the metrology objects of a metrology model.
  void SetMetrologyObjectParam(const char* Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;

#ifdef _WIN32
  // Set parameters for the metrology objects of a metrology model.
  void SetMetrologyObjectParam(const wchar_t* Index, const HTuple& GenParamName, const HTuple& GenParamValue) const;
#endif

  // Add a rectangle to a metrology model.
  Hlong AddMetrologyObjectRectangle2Measure(const HTuple& Row, const HTuple& Column, const HTuple& Phi, const HTuple& Length1, const HTuple& Length2, const HTuple& MeasureLength1, const HTuple& MeasureLength2, const HTuple& MeasureSigma, const HTuple& MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a rectangle to a metrology model.
  Hlong AddMetrologyObjectRectangle2Measure(double Row, double Column, double Phi, double Length1, double Length2, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a line to a metrology model.
  Hlong AddMetrologyObjectLineMeasure(const HTuple& RowBegin, const HTuple& ColumnBegin, const HTuple& RowEnd, const HTuple& ColumnEnd, const HTuple& MeasureLength1, const HTuple& MeasureLength2, const HTuple& MeasureSigma, const HTuple& MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a line to a metrology model.
  Hlong AddMetrologyObjectLineMeasure(double RowBegin, double ColumnBegin, double RowEnd, double ColumnEnd, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add an ellipse or an elliptic arc to a metrology model.
  Hlong AddMetrologyObjectEllipseMeasure(const HTuple& Row, const HTuple& Column, const HTuple& Phi, const HTuple& Radius1, const HTuple& Radius2, const HTuple& MeasureLength1, const HTuple& MeasureLength2, const HTuple& MeasureSigma, const HTuple& MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add an ellipse or an elliptic arc to a metrology model.
  Hlong AddMetrologyObjectEllipseMeasure(double Row, double Column, double Phi, double Radius1, double Radius2, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a circle or a circular arc to a metrology model.
  Hlong AddMetrologyObjectCircleMeasure(const HTuple& Row, const HTuple& Column, const HTuple& Radius, const HTuple& MeasureLength1, const HTuple& MeasureLength2, const HTuple& MeasureSigma, const HTuple& MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Add a circle or a circular arc to a metrology model.
  Hlong AddMetrologyObjectCircleMeasure(double Row, double Column, double Radius, double MeasureLength1, double MeasureLength2, double MeasureSigma, double MeasureThreshold, const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Delete a metrology model and free the allocated memory.
  void ClearMetrologyModel() const;

  // Delete metrology objects and free the allocated memory.
  void ClearMetrologyObject(const HTuple& Index) const;

  // Delete metrology objects and free the allocated memory.
  void ClearMetrologyObject(const HString& Index) const;

  // Delete metrology objects and free the allocated memory.
  void ClearMetrologyObject(const char* Index) const;

#ifdef _WIN32
  // Delete metrology objects and free the allocated memory.
  void ClearMetrologyObject(const wchar_t* Index) const;
#endif

  // Set the size of the image of metrology objects.
  void SetMetrologyModelImageSize(Hlong Width, Hlong Height) const;

  // Create the data structure that is needed to measure geometric shapes.
  void CreateMetrologyModel();

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HMetrologyModel> HMetrologyModelArrayRef;
typedef HSmartPtr< HMetrologyModelArrayRef > HMetrologyModelArrayPtr;


// Represents multiple tool instances
class LIntExport HMetrologyModelArray : public HHandleBaseArray
{

public:

  // Create empty array
  HMetrologyModelArray();

  // Create array from native array of tool instances
  HMetrologyModelArray(HMetrologyModel* classes, Hlong length);

  // Copy constructor
  HMetrologyModelArray(const HMetrologyModelArray &tool_array);

  // Destructor
  virtual ~HMetrologyModelArray();

  // Assignment operator
  HMetrologyModelArray &operator=(const HMetrologyModelArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HMetrologyModel* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HMetrologyModelArrayPtr *mArrayPtr;
};

}

#endif
