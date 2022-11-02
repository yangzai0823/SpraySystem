﻿
/*
 * Copyright (c) 2017, Open Source Robotics Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VWS_ROBOTLIB_GLOBAL_H
#define VWS_ROBOTLIB_GLOBAL_H
// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROBOTLIB_EXPORT __attribute__ ((dllexport))
    #define ROBOTLIB_IMPORT __attribute__ ((dllimport))
  #else
    #define ROBOTLIB_EXPORT  __declspec(dllexport)
    #define ROBOTLIB_IMPORT __declspec(dllimport)
    //#define ROBOTLIB_IMPORT
  #endif
  #ifdef ROBOTLIB_BUILDING_LIBRARY
    #define ROBOTLIB_MODULE ROBOTLIB_EXPORT
    #pragma message("ROBOTLIB_EXPORT")
  #else
    #define ROBOTLIB_MODULE ROBOTLIB_IMPORT
    #pragma message("ROBOTLIB_IMPORT")
  #endif
  #define ROBOTLIB_PUBLIC_TYPE ROBOTLIB_MODULE
  #define ROBOTLIB_LOCAL

#else
  #define ROBOTLIB_EXPORT __attribute__ ((visibility("default")))
  #define ROBOTLIB_IMPORT
  #if __GNUC__ >= 4
    #define ROBOTLIB_PUBLIC __attribute__ ((visibility("default")))
    #define ROBOTLIB_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define ROBOTLIB_PUBLIC
    #define ROBOTLIB_LOCAL
  #endif
  // #define ROBOTLIB_PUBLIC_TYPE
  #ifdef ROBOTLIB_BUILDING_LIBRARY
    #define ROBOTLIB_MODULE ROBOTLIB_EXPORT
    #pragma message("ROBOTLIB_EXPORT")
  #else
    #define ROBOTLIB_MODULE ROBOTLIB_IMPORT
    #pragma message("ROBOTLIB_IMPORT")
  #endif
  #define ROBOTLIB_PUBLIC_TYPE ROBOTLIB_MODULE
  // #define ROBOTLIB_LOCAL

#endif

#endif // VWS_ROBOTLIB_GLOBAL_H
