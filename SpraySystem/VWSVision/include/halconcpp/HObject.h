/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HOBJECT
#define HCPP_HOBJECT

namespace HalconCpp
{

// Represents an instance of an iconic object(-array). Base class for images, regions and XLDs
class LIntExport HObject : public HObjectBase
{

public:

  // Create an uninitialized instance
  HObject():HObjectBase() {}

  // Copy constructor
  HObject(const HObject& source) : HObjectBase(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HObject& operator=(const HObject&) = default;
#endif

  // Create HObject from object id. For copy=false takes
  // over management of input key. Type of key must match!
  explicit HObject(Hkey key, bool copy=true);

  // Access of object tuple element
  const HObject operator [] (Hlong index) const;

  // Deep copy of all data represented by this object instance
  HObject Clone() const;




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Calculate the difference of two object tuples.
  HObject ObjDiff(const HObject& ObjectsSub) const;

  // Convert an "integer number" into an iconic object.
  void IntegerToObj(const HTuple& SurrogateTuple);

  // Convert an "integer number" into an iconic object.
  void IntegerToObj(void* SurrogateTuple);

  // Convert an iconic object into an "integer number."
  HTuple ObjToInteger(Hlong Index, Hlong Number) const;

  // Copy an iconic object in the HALCON database.
  HObject CopyObj(Hlong Index, Hlong NumObj) const;

  // Concatenate two iconic object tuples.
  HObject ConcatObj(const HObject& Objects2) const;

  // Select objects from an object tuple.
  HObject SelectObj(const HTuple& Index) const;

  // Select objects from an object tuple.
  HObject SelectObj(Hlong Index) const;

  // Compare iconic objects regarding equality.
  Hlong CompareObj(const HObject& Objects2, const HTuple& Epsilon) const;

  // Compare iconic objects regarding equality.
  Hlong CompareObj(const HObject& Objects2, double Epsilon) const;

  // Compare image objects regarding equality.
  Hlong TestEqualObj(const HObject& Objects2) const;

  // Number of objects in a tuple.
  Hlong CountObj() const;

  // Information about the components of an image object.
  HTuple GetChannelInfo(const HString& Request, const HTuple& Channel) const;

  // Information about the components of an image object.
  HString GetChannelInfo(const HString& Request, Hlong Channel) const;

  // Information about the components of an image object.
  HString GetChannelInfo(const char* Request, Hlong Channel) const;

#ifdef _WIN32
  // Information about the components of an image object.
  HString GetChannelInfo(const wchar_t* Request, Hlong Channel) const;
#endif

  // Name of the class of an image object.
  HTuple GetObjClass() const;

  // Create an empty object tuple.
  void GenEmptyObj();

  // Displays image objects (image, region, XLD).
  void DispObj(const HWindow& WindowHandle) const;

  // Read an iconic object.
  void ReadObject(const HString& FileName);

  // Read an iconic object.
  void ReadObject(const char* FileName);

#ifdef _WIN32
  // Read an iconic object.
  void ReadObject(const wchar_t* FileName);
#endif

  // Write an iconic object.
  void WriteObject(const HString& FileName) const;

  // Write an iconic object.
  void WriteObject(const char* FileName) const;

#ifdef _WIN32
  // Write an iconic object.
  void WriteObject(const wchar_t* FileName) const;
#endif

  // Deserialize a serialized iconic object.
  void DeserializeObject(const HSerializedItem& SerializedItemHandle);

  // Serialize an iconic object.
  HSerializedItem SerializeObject() const;

  // Insert objects into an iconic object tuple.
  HObject InsertObj(const HObject& ObjectsInsert, Hlong Index) const;

  // Remove objects from an iconic object tuple.
  HObject RemoveObj(const HTuple& Index) const;

  // Remove objects from an iconic object tuple.
  HObject RemoveObj(Hlong Index) const;

  // Replaces one or more elements of an iconic object tuple.
  HObject ReplaceObj(const HObject& ObjectsReplace, const HTuple& Index) const;

  // Replaces one or more elements of an iconic object tuple.
  HObject ReplaceObj(const HObject& ObjectsReplace, Hlong Index) const;

private:

  // Verify matching semantic type ('object')!
  void AssertObjectClass();

};

}

#endif
