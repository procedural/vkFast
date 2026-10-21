// ======================================================================== //
// Copyright 2009-2012 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_CONSTANTS_H__
#define __EMBREE_CONSTANTS_H__

#ifndef NULL
#define NULL 0
#endif

#include <limits>

#undef True
#undef False

namespace embree
{
  struct NullTy {
  } null;

  struct TrueTy {
    __forceinline operator bool( ) const { return true; }
  } True;

  struct FalseTy {
    __forceinline operator bool( ) const { return false; }
  } False;

  struct ZeroTy
  {
    __forceinline operator          double   ( ) const { return 0; }
    __forceinline operator          float    ( ) const { return 0; }
    __forceinline operator          long long( ) const { return 0; }
    __forceinline operator unsigned long long( ) const { return 0; }
    __forceinline operator          long     ( ) const { return 0; }
    __forceinline operator unsigned long     ( ) const { return 0; }
    __forceinline operator          int      ( ) const { return 0; }
    __forceinline operator unsigned int      ( ) const { return 0; }
    __forceinline operator          short    ( ) const { return 0; }
    __forceinline operator unsigned short    ( ) const { return 0; }
    __forceinline operator          char     ( ) const { return 0; }
    __forceinline operator unsigned char     ( ) const { return 0; }
  } zero;

  struct OneTy
  {
    __forceinline operator          double   ( ) const { return 1; }
    __forceinline operator          float    ( ) const { return 1; }
    __forceinline operator          long long( ) const { return 1; }
    __forceinline operator unsigned long long( ) const { return 1; }
    __forceinline operator          long     ( ) const { return 1; }
    __forceinline operator unsigned long     ( ) const { return 1; }
    __forceinline operator          int      ( ) const { return 1; }
    __forceinline operator unsigned int      ( ) const { return 1; }
    __forceinline operator          short    ( ) const { return 1; }
    __forceinline operator unsigned short    ( ) const { return 1; }
    __forceinline operator          char     ( ) const { return 1; }
    __forceinline operator unsigned char     ( ) const { return 1; }
  } one;

  struct NegInfTy
  {
    __forceinline operator          double   ( ) const { return -std::numeric_limits<double>::infinity(); }
    __forceinline operator          float    ( ) const { return -std::numeric_limits<float>::infinity(); }
    __forceinline operator          long long( ) const { return std::numeric_limits<long long>::min(); }
    __forceinline operator unsigned long long( ) const { return std::numeric_limits<unsigned long long>::min(); }
    __forceinline operator          long     ( ) const { return std::numeric_limits<long>::min(); }
    __forceinline operator unsigned long     ( ) const { return std::numeric_limits<unsigned long>::min(); }
    __forceinline operator          int      ( ) const { return std::numeric_limits<int>::min(); }
    __forceinline operator unsigned int      ( ) const { return std::numeric_limits<unsigned int>::min(); }
    __forceinline operator          short    ( ) const { return std::numeric_limits<short>::min(); }
    __forceinline operator unsigned short    ( ) const { return std::numeric_limits<unsigned short>::min(); }
    __forceinline operator          char     ( ) const { return std::numeric_limits<char>::min(); }
    __forceinline operator unsigned char     ( ) const { return std::numeric_limits<unsigned char>::min(); }

  } neg_inf;

  struct PosInfTy
  {
    __forceinline operator          double   ( ) const { return std::numeric_limits<double>::infinity(); }
    __forceinline operator          float    ( ) const { return std::numeric_limits<float>::infinity(); }
    __forceinline operator          long long( ) const { return std::numeric_limits<long long>::max(); }
    __forceinline operator unsigned long long( ) const { return std::numeric_limits<unsigned long long>::max(); }
    __forceinline operator          long     ( ) const { return std::numeric_limits<long>::max(); }
    __forceinline operator unsigned long     ( ) const { return std::numeric_limits<unsigned long>::max(); }
    __forceinline operator          int      ( ) const { return std::numeric_limits<int>::max(); }
    __forceinline operator unsigned int      ( ) const { return std::numeric_limits<unsigned int>::max(); }
    __forceinline operator          short    ( ) const { return std::numeric_limits<short>::max(); }
    __forceinline operator unsigned short    ( ) const { return std::numeric_limits<unsigned short>::max(); }
    __forceinline operator          char     ( ) const { return std::numeric_limits<char>::max(); }
    __forceinline operator unsigned char     ( ) const { return std::numeric_limits<unsigned char>::max(); }
  } inf MAYBE_UNUSED, pos_inf;

  struct NaNTy
  {
    __forceinline operator double( ) const { return std::numeric_limits<double>::quiet_NaN(); }
    __forceinline operator float ( ) const { return std::numeric_limits<float>::quiet_NaN(); }
  } nan;

  struct UlpTy
  {
    __forceinline operator double( ) const { return std::numeric_limits<double>::epsilon(); }
    __forceinline operator float ( ) const { return std::numeric_limits<float>::epsilon(); }
  } ulp;

  struct PiTy
  {
    __forceinline operator double( ) const { return 3.14159265358979323846; }
    __forceinline operator float ( ) const { return 3.14159265358979323846f; }
  } pi;

  struct OneOverPiTy
  {
    __forceinline operator double( ) const { return 0.31830988618379069122; }
    __forceinline operator float ( ) const { return 0.31830988618379069122f; }
  } one_over_pi;

  struct TwoPiTy
  {
    __forceinline operator double( ) const { return 6.283185307179586232; }
    __forceinline operator float ( ) const { return 6.283185307179586232f; }
  } two_pi;

  struct OneOverTwoPiTy
  {
    __forceinline operator double( ) const { return 0.15915494309189534561; }
    __forceinline operator float ( ) const { return 0.15915494309189534561f; }
  } one_over_two_pi;

  struct FourPiTy
  {
    __forceinline operator double( ) const { return 12.566370614359172464; } 
    __forceinline operator float ( ) const { return 12.566370614359172464f; }
  } four_pi;

  struct OneOverFourPiTy
  {
    __forceinline operator double( ) const { return 0.079577471545947672804; }
    __forceinline operator float ( ) const { return 0.079577471545947672804f; }
  } one_over_four_pi;

  struct StepTy {
  } step;

  struct EmptyTy {
  } empty;

  struct FullTy {
  } full;
}

#endif
