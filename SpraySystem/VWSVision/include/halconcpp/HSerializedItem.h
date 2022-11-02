/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HSERIALIZEDITEM
#define HCPP_HSERIALIZEDITEM

namespace HalconCpp
{

// Represents an instance of a serialized item.
#if defined(_WIN32)
class HSerializedItem : public HHandle
#else
class LIntExport HSerializedItem : public HHandle
#endif
{

public:

  // Create an uninitialized instance
  LIntExport inline HSerializedItem():HHandle() {}

  // Copy constructor
  LIntExport inline HSerializedItem(const HSerializedItem& source) : HHandle(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HSerializedItem& operator=(const HSerializedItem&) = default;
#endif

  // Destructor must be defined explicitly to force certain compilers to emit
  // the virtual table of a class in the object file and to be able to set
  // the appropriate visibility attribute
  LIntExport ~HSerializedItem();

  // Copy constructor
  LIntExport HSerializedItem(const HHandle& handle);

  // Create HSerializedItem from handle, taking ownership
  LIntExport explicit HSerializedItem(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('serialized_item')!
  LIntExport virtual void AssertType(Hphandle handle) const;

public:



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // create_serialized_item_ptr: Create a serialized item.
  LIntExport explicit HSerializedItem(void* Pointer, Hlong Size, const HString& Copy);

  // create_serialized_item_ptr: Create a serialized item.
  LIntExport explicit HSerializedItem(void* Pointer, Hlong Size, const char* Copy);

#ifdef _WIN32
  // create_serialized_item_ptr: Create a serialized item.
  LIntExport explicit HSerializedItem(void* Pointer, Hlong Size, const wchar_t* Copy);
#endif




/***************************************************************************
 * Serialization                                                           *
 ***************************************************************************/
#define H_SERIAL_HEADER_SIZE 16

static LIntExport char*
                                  DeserializeInitFromHeader(char* header, bool streamIsOk, uint64_t* size);
static LIntExport HSerializedItem DeserializeBufferToItem(uint64_t size,
                                                          bool     streamIsOk,
                                                          char*    serialized);
#ifndef HCPP_NO_USE_IOSTREAM
void inline Serialize(std::ostream& stream);
static HSerializedItem inline Deserialize(std::istream& stream);
#endif


  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Receive a serialized item over a socket connection.
  LIntExport void ReceiveSerializedItem(const HSocket& Socket);

  // Send a serialized item over a socket connection.
  LIntExport void SendSerializedItem(const HSocket& Socket) const;

  // Write a serialized item to a file.
  LIntExport void FwriteSerializedItem(const HFile& FileHandle) const;

  // Read a serialized item from a file.
  LIntExport void FreadSerializedItem(const HFile& FileHandle);

  // Delete a serialized item.
  static LIntExport void ClearSerializedItem(const HSerializedItemArray& SerializedItemHandle);

  // Delete a serialized item.
  LIntExport void ClearSerializedItem() const;

  // Access the data pointer of a serialized item.
  LIntExport void* GetSerializedItemPtr(Hlong* Size) const;

  // Create a serialized item.
  LIntExport void CreateSerializedItemPtr(void* Pointer, Hlong Size, const HString& Copy);

  // Create a serialized item.
  LIntExport void CreateSerializedItemPtr(void* Pointer, Hlong Size, const char* Copy);

#ifdef _WIN32
  // Create a serialized item.
  LIntExport void CreateSerializedItemPtr(void* Pointer, Hlong Size, const wchar_t* Copy);
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HSerializedItem> HSerializedItemArrayRef;
typedef HSmartPtr< HSerializedItemArrayRef > HSerializedItemArrayPtr;


// Represents multiple tool instances
class LIntExport HSerializedItemArray : public HHandleBaseArray
{

public:

  // Create empty array
  HSerializedItemArray();

  // Create array from native array of tool instances
  HSerializedItemArray(HSerializedItem* classes, Hlong length);

  // Copy constructor
  HSerializedItemArray(const HSerializedItemArray &tool_array);

  // Destructor
  virtual ~HSerializedItemArray();

  // Assignment operator
  HSerializedItemArray &operator=(const HSerializedItemArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HSerializedItem* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HSerializedItemArrayPtr *mArrayPtr;
};

}

#endif
