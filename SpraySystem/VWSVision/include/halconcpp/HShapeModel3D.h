/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HSHAPEMODEL3D
#define HCPP_HSHAPEMODEL3D

namespace HalconCpp
{

// Represents an instance of a 3D shape model for 3D matching.
class LIntExport HShapeModel3D : public HHandle
{

public:

  // Create an uninitialized instance
  HShapeModel3D():HHandle() {}

  // Copy constructor
  HShapeModel3D(const HShapeModel3D& source) : HHandle(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HShapeModel3D& operator=(const HShapeModel3D&) = default;
#endif

  // Copy constructor
  HShapeModel3D(const HHandle& handle);

  // Create HShapeModel3D from handle, taking ownership
  explicit HShapeModel3D(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('shape_model_3d')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HShapeModel3D Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_shape_model_3d: Read a 3D shape model from a file.
  explicit HShapeModel3D(const HString& FileName);

  // read_shape_model_3d: Read a 3D shape model from a file.
  explicit HShapeModel3D(const char* FileName);

#ifdef _WIN32
  // read_shape_model_3d: Read a 3D shape model from a file.
  explicit HShapeModel3D(const wchar_t* FileName);
#endif

  // create_shape_model_3d: Prepare a 3D object model for matching.
  explicit HShapeModel3D(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const HString& OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const HTuple& GenParamName, const HTuple& GenParamValue);

  // create_shape_model_3d: Prepare a 3D object model for matching.
  explicit HShapeModel3D(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const HString& OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const HString& GenParamName, Hlong GenParamValue);

  // create_shape_model_3d: Prepare a 3D object model for matching.
  explicit HShapeModel3D(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const char* OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const char* GenParamName, Hlong GenParamValue);

#ifdef _WIN32
  // create_shape_model_3d: Prepare a 3D object model for matching.
  explicit HShapeModel3D(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const wchar_t* OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const wchar_t* GenParamName, Hlong GenParamValue);
#endif




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Free the memory of a 3D shape model.
  static void ClearShapeModel3d(const HShapeModel3DArray& ShapeModel3DID);

  // Free the memory of a 3D shape model.
  void ClearShapeModel3d() const;

  // Deserialize a serialized 3D shape model.
  void DeserializeShapeModel3d(const HSerializedItem& SerializedItemHandle);

  // Serialize a 3D shape model.
  HSerializedItem SerializeShapeModel3d() const;

  // Read a 3D shape model from a file.
  void ReadShapeModel3d(const HString& FileName);

  // Read a 3D shape model from a file.
  void ReadShapeModel3d(const char* FileName);

#ifdef _WIN32
  // Read a 3D shape model from a file.
  void ReadShapeModel3d(const wchar_t* FileName);
#endif

  // Write a 3D shape model to a file.
  void WriteShapeModel3d(const HString& FileName) const;

  // Write a 3D shape model to a file.
  void WriteShapeModel3d(const char* FileName) const;

#ifdef _WIN32
  // Write a 3D shape model to a file.
  void WriteShapeModel3d(const wchar_t* FileName) const;
#endif

  // Transform a pose that refers to the coordinate system of a 3D object model to a pose that refers to the reference coordinate system of a 3D shape model and vice versa.
  HPose TransPoseShapeModel3d(const HPose& PoseIn, const HString& Transformation) const;

  // Transform a pose that refers to the coordinate system of a 3D object model to a pose that refers to the reference coordinate system of a 3D shape model and vice versa.
  HPose TransPoseShapeModel3d(const HPose& PoseIn, const char* Transformation) const;

#ifdef _WIN32
  // Transform a pose that refers to the coordinate system of a 3D object model to a pose that refers to the reference coordinate system of a 3D shape model and vice versa.
  HPose TransPoseShapeModel3d(const HPose& PoseIn, const wchar_t* Transformation) const;
#endif

  // Project the edges of a 3D shape model into image coordinates.
  HXLDCont ProjectShapeModel3d(const HCamPar& CamParam, const HPose& Pose, const HString& HiddenSurfaceRemoval, const HTuple& MinFaceAngle) const;

  // Project the edges of a 3D shape model into image coordinates.
  HXLDCont ProjectShapeModel3d(const HCamPar& CamParam, const HPose& Pose, const HString& HiddenSurfaceRemoval, double MinFaceAngle) const;

  // Project the edges of a 3D shape model into image coordinates.
  HXLDCont ProjectShapeModel3d(const HCamPar& CamParam, const HPose& Pose, const char* HiddenSurfaceRemoval, double MinFaceAngle) const;

#ifdef _WIN32
  // Project the edges of a 3D shape model into image coordinates.
  HXLDCont ProjectShapeModel3d(const HCamPar& CamParam, const HPose& Pose, const wchar_t* HiddenSurfaceRemoval, double MinFaceAngle) const;
#endif

  // Return the contour representation of a 3D shape model view.
  HXLDCont GetShapeModel3dContours(Hlong Level, Hlong View, HPose* ViewPose) const;

  // Return the parameters of a 3D shape model.
  HTuple GetShapeModel3dParams(const HTuple& GenParamName) const;

  // Return the parameters of a 3D shape model.
  HTuple GetShapeModel3dParams(const HString& GenParamName) const;

  // Return the parameters of a 3D shape model.
  HTuple GetShapeModel3dParams(const char* GenParamName) const;

#ifdef _WIN32
  // Return the parameters of a 3D shape model.
  HTuple GetShapeModel3dParams(const wchar_t* GenParamName) const;
#endif

  // Find the best matches of a 3D shape model in an image.
  HPoseArray FindShapeModel3d(const HImage& Image, double MinScore, double Greediness, const HTuple& NumLevels, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* CovPose, HTuple* Score) const;

  // Prepare a 3D object model for matching.
  void CreateShapeModel3d(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const HString& OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Prepare a 3D object model for matching.
  void CreateShapeModel3d(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const HString& OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const HString& GenParamName, Hlong GenParamValue);

  // Prepare a 3D object model for matching.
  void CreateShapeModel3d(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const char* OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const char* GenParamName, Hlong GenParamValue);

#ifdef _WIN32
  // Prepare a 3D object model for matching.
  void CreateShapeModel3d(const HObjectModel3D& ObjectModel3D, const HCamPar& CamParam, double RefRotX, double RefRotY, double RefRotZ, const wchar_t* OrderOfRotation, double LongitudeMin, double LongitudeMax, double LatitudeMin, double LatitudeMax, double CamRollMin, double CamRollMax, double DistMin, double DistMax, Hlong MinContrast, const wchar_t* GenParamName, Hlong GenParamValue);
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HShapeModel3D> HShapeModel3DArrayRef;
typedef HSmartPtr< HShapeModel3DArrayRef > HShapeModel3DArrayPtr;


// Represents multiple tool instances
class LIntExport HShapeModel3DArray : public HHandleBaseArray
{

public:

  // Create empty array
  HShapeModel3DArray();

  // Create array from native array of tool instances
  HShapeModel3DArray(HShapeModel3D* classes, Hlong length);

  // Copy constructor
  HShapeModel3DArray(const HShapeModel3DArray &tool_array);

  // Destructor
  virtual ~HShapeModel3DArray();

  // Assignment operator
  HShapeModel3DArray &operator=(const HShapeModel3DArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HShapeModel3D* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HShapeModel3DArrayPtr *mArrayPtr;
};

}

#endif
