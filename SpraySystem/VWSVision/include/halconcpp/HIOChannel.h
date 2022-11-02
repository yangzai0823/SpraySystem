/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HIOCHANNEL
#define HCPP_HIOCHANNEL

namespace HalconCpp
{

// Represents an instance of a channel of an I/O device.
class LIntExport HIOChannel : public HHandle
{

public:

  // Create an uninitialized instance
  HIOChannel():HHandle() {}

  // Copy constructor
  HIOChannel(const HIOChannel& source) : HHandle(source) {}

#if __cplusplus >= 201103
  // C++11 deprecates declaration of an implicit default assignment operator if
  // the class has a custom copy constructor.
  HIOChannel& operator=(const HIOChannel&) = default;
#endif

  // Copy constructor
  HIOChannel(const HHandle& handle);

  // Create HIOChannel from handle, taking ownership
  explicit HIOChannel(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('io_channel')!
  virtual void AssertType(Hphandle handle) const;

public:



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // open_io_channel: Open and configure I/O channels.
  explicit HIOChannel(const HIODevice& IODeviceHandle, const HString& IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);

  // open_io_channel: Open and configure I/O channels.
  explicit HIOChannel(const HIODevice& IODeviceHandle, const char* IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);

#ifdef _WIN32
  // open_io_channel: Open and configure I/O channels.
  explicit HIOChannel(const HIODevice& IODeviceHandle, const wchar_t* IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);
#endif




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Perform an action on I/O channels.
  static HTuple ControlIoChannel(const HIOChannelArray& IOChannelHandle, const HString& ParamAction, const HTuple& ParamArgument);

  // Perform an action on I/O channels.
  HTuple ControlIoChannel(const HString& ParamAction, const HTuple& ParamArgument) const;

  // Perform an action on I/O channels.
  HTuple ControlIoChannel(const char* ParamAction, const HTuple& ParamArgument) const;

#ifdef _WIN32
  // Perform an action on I/O channels.
  HTuple ControlIoChannel(const wchar_t* ParamAction, const HTuple& ParamArgument) const;
#endif

  // Write a value to the specified I/O channels.
  static HTuple WriteIoChannel(const HIOChannelArray& IOChannelHandle, const HTuple& Value);

  // Write a value to the specified I/O channels.
  HTuple WriteIoChannel(const HTuple& Value) const;

  // Read a value from the specified I/O channels.
  static HTuple ReadIoChannel(const HIOChannelArray& IOChannelHandle, HTuple* Status);

  // Read a value from the specified I/O channels.
  HTuple ReadIoChannel(HTuple* Status) const;

  // Set specific parameters of I/O channels.
  static void SetIoChannelParam(const HIOChannelArray& IOChannelHandle, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Set specific parameters of I/O channels.
  void SetIoChannelParam(const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Query specific parameters of I/O channels.
  static HTuple GetIoChannelParam(const HIOChannelArray& IOChannelHandle, const HTuple& GenParamName);

  // Query specific parameters of I/O channels.
  HTuple GetIoChannelParam(const HString& GenParamName) const;

  // Query specific parameters of I/O channels.
  HTuple GetIoChannelParam(const char* GenParamName) const;

#ifdef _WIN32
  // Query specific parameters of I/O channels.
  HTuple GetIoChannelParam(const wchar_t* GenParamName) const;
#endif

  // Close I/O channels.
  static void CloseIoChannel(const HIOChannelArray& IOChannelHandle);

  // Close I/O channels.
  void CloseIoChannel() const;

  // Open and configure I/O channels.
  static HIOChannelArray OpenIoChannel(const HIODevice& IODeviceHandle, const HTuple& IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Open and configure I/O channels.
  void OpenIoChannel(const HIODevice& IODeviceHandle, const HString& IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Open and configure I/O channels.
  void OpenIoChannel(const HIODevice& IODeviceHandle, const char* IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);

#ifdef _WIN32
  // Open and configure I/O channels.
  void OpenIoChannel(const HIODevice& IODeviceHandle, const wchar_t* IOChannelName, const HTuple& GenParamName, const HTuple& GenParamValue);
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HIOChannel> HIOChannelArrayRef;
typedef HSmartPtr< HIOChannelArrayRef > HIOChannelArrayPtr;


// Represents multiple tool instances
class LIntExport HIOChannelArray : public HHandleBaseArray
{

public:

  // Create empty array
  HIOChannelArray();

  // Create array from native array of tool instances
  HIOChannelArray(HIOChannel* classes, Hlong length);

  // Copy constructor
  HIOChannelArray(const HIOChannelArray &tool_array);

  // Destructor
  virtual ~HIOChannelArray();

  // Assignment operator
  HIOChannelArray &operator=(const HIOChannelArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HIOChannel* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HIOChannelArrayPtr *mArrayPtr;
};

}

#endif
