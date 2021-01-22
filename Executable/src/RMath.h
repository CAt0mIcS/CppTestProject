//-------------------------------------------------------------------------------------
// DirectXMath.h -- SIMD C++ Math library
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkID=615560
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 							   The MIT License (MIT)
//
// Copyright (c) 2011-2020 Microsoft Corp
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// NOTICE
// This file has been modified to assure platform compatibility, correct namespacing
// and easier usage.
//-------------------------------------------------------------------------------------

#pragma once

#ifndef __cplusplus
#error DirectX Math requires C++
#endif


#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(RAY_NO_INTRINSICS) && !defined(RAYMATH_VECTORCALL)
#define RAYMATH_VECTORCALL 1
#endif

#if defined(__linux__) && !defined(RAY_NO_INTRINSICS)
#define RAY_NO_INTRINSICS
#endif

#if RAYMATH_VECTORCALL
#define RAYMATH_CALLCONV __vectorcall
#else
#define RAYMATH_CALLCONV
#endif

#if defined(_MSC_VER) && (_MSC_FULL_VER < 190023506)
#define RAYMATH_CONST const
#define RAYMATH_CONSTEXPR
#else
#define RAYMATH_CONST constexpr
#define RAYMATH_CONSTEXPR constexpr
#endif

#if defined(_MSC_VER)
#define RAY_ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define RAY_ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

#if !defined(RAY_AVX2_INTRINSICS) && defined(__AVX2__) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVX2_INTRINSICS
#endif

#if !defined(RAY_FMA3_INTRINSICS) && defined(RAY_AVX2_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_FMA3_INTRINSICS
#endif

#if !defined(RAY_F16C_INTRINSICS) && defined(RAY_AVX2_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#define RAY_F16C_INTRINSICS
#endif

#if defined(RAY_FMA3_INTRINSICS) && !defined(RAY_AVC_INTRINSICS)
#define RAY_AVC_INTRINSICS
#endif

#if defined(RAY_F16C_INTRINSICS) && !defined(RAY_AVC_INTRINSICS)
#define RAY_AVC_INTRINSICS
#endif

#if !defined(RAY_AVC_INTRINSICS) && defined(__AVX__) && !defined(RAY_NO_INTRINSICS)
#define RAY_AVC_INTRINSICS
#endif

#if defined(RAY_AVC_INTRINSICS) && !defined(RAY_SSE4_INTRINSICS)
#define RAY_SSE4_INTRINSICS
#endif

#if defined(RAY_SSE4_INTRINSICS) && !defined(RAY_SSE3_INTRINSICS)
#define RAY_SSE3_INTRINSICS
#endif

#if defined(RAY_SSE3_INTRINSICS) && !defined(RAY_SSE_INTRINSICS)
#define RAY_SSE_INTRINSICS
#endif

#if !defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
#if (defined(_M_IX86) || defined(_M_X64)) && !defined(_M_HYBRID_X86_ARM64)
#define RAY_SSE_INTRINSICS
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#define RAY_ARM_NEON_INTRINSICS
#endif
#endif // !RAY_ARM_NEON_INTRINSICS && !RAY_SSE_INTRINSICS && !RAY_NO_INTRINSICS

// RAY_TODO: Test clang with overloads
#if !defined(RAYMATH_NO_VECTOR_OVERLOADS) && defined(__clang__)
#define RAYMATH_NO_VECTOR_OVERLOADS
#endif

#pragma warning(push)
#pragma warning(disable:4514 4820)
// C4514/4820: Off by default noise
#include <math.h>
#include <float.h>
#include <malloc.h>
#pragma warning(pop)

#ifndef RAY_NO_INTRINSICS
#pragma warning(push)
#pragma warning(disable : 4987)
// C4987: Off by default noise
#pragma warning(pop)

#ifdef _WIN32
#include <intrin.h>
#endif

#ifdef RAY_SSE_INTRINSICS
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef RAY_SSE3_INTRINSICS
#include <pmmintrin.h>
#endif

#ifdef RAY_SSE4_INTRINSICS
#include <smmintrin.h>
#endif

#ifdef RAY_AVC_INTRINSICS
#include <immintrin.h>
#endif

#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif
#endif // !RAY_NO_INTRINSICS

#include <assert.h>

#pragma warning(push)
#pragma warning(disable : 4005 4668)
// C4005/4668: Old header issue
#include <stdint.h>
#pragma warning(pop)

/****************************************************************************
 *
 * Conditional intrinsics
 *
 ****************************************************************************/

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)

#if defined(RAYMATH_NO_MOVNT)
#define RAYMATH_STREAM_PS( p, a ) _mm_store_ps( p, a )
#define RAYMATH_SFENCE()
#else
#define RAYMATH_STREAM_PS( p, a ) _mm_stream_ps( p, a )
#define RAYMATH_SFENCE() _mm_sfence()
#endif

#if defined(RAY_AVC_INTRINSICS)
#define RAYMATH_PERMUTE_PS( v, c ) _mm_permute_ps( v, c )
#else
#define RAYMATH_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )
#endif

#endif // RAY_SSE_INTRINSICS && !RAY_NO_INTRINSICS

namespace At0
{
	namespace Ray
	{


		/****************************************************************************
		 *
		 * Constant definitions
		 *
		 ****************************************************************************/

		namespace Constants
		{
			RAYMATH_CONST float PI = 3.141592654f;
			RAYMATH_CONST float RAY_2PI = 6.283185307f;
			RAYMATH_CONST float RAY_1DIVPI = 0.318309886f;
			RAYMATH_CONST float RAY_1DIV2PI = 0.159154943f;
			RAYMATH_CONST float PIDIV2 = 1.570796327f;
			RAYMATH_CONST float PIDIV4 = 0.785398163f;
		}


		RAYMATH_CONST uint32_t RAYMATH_SELECT_0 = 0x00000000;
		RAYMATH_CONST uint32_t RAYMATH_SELECT_1 = 0xFFFFFFFF;

		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_0X = 0;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_0Y = 1;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_0Z = 2;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_0W = 3;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_1X = 4;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_1Y = 5;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_1Z = 6;
		RAYMATH_CONST uint32_t RAYMATH_PERMUTE_1W = 7;

		RAYMATH_CONST uint32_t RAYMATH_SWIZZLE_X = 0;
		RAYMATH_CONST uint32_t RAYMATH_SWIZZLE_Y = 1;
		RAYMATH_CONST uint32_t RAYMATH_SWIZZLE_Z = 2;
		RAYMATH_CONST uint32_t RAYMATH_SWIZZLE_W = 3;

		RAYMATH_CONST uint32_t RAYMATH_CRMASK_CR6 = 0x000000F0;
		RAYMATH_CONST uint32_t RAYMATH_CRMASK_CR6TRUE = 0x00000080;
		RAYMATH_CONST uint32_t RAYMATH_CRMASK_CR6FALSE = 0x00000020;
		RAYMATH_CONST uint32_t RAYMATH_CRMASK_CR6BOUNDS = RAYMATH_CRMASK_CR6FALSE;

		RAYMATH_CONST size_t RAYMATH_CACHE_LINE_SIZE = 64;


		/****************************************************************************
		 *
		 * Macros
		 *
		 ****************************************************************************/

#if defined(__XNAMATH_H__) && defined(ComparisonAllTrue)
#undef ComparisonAllTrue
#undef ComparisonAnyTrue
#undef ComparisonAllFalse
#undef ComparisonAnyFalse
#undef ComparisonMixed
#undef ComparisonAllInBounds
#undef ComparisonAnyOutOfBounds
#endif

		 // Unit conversion

		inline RAYMATH_CONSTEXPR float ConvertToRadians(float fDegrees) { return fDegrees * (Constants::PI / 180.0f); }
		inline RAYMATH_CONSTEXPR float ConvertToDegrees(float fRadians) { return fRadians * (180.0f / Constants::PI); }

		// Condition register evaluation proceeding a recording (R) comparison

		inline bool ComparisonAllTrue(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6TRUE) == RAYMATH_CRMASK_CR6TRUE); }
		inline bool ComparisonAnyTrue(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6FALSE) != RAYMATH_CRMASK_CR6FALSE); }
		inline bool ComparisonAllFalse(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6FALSE) == RAYMATH_CRMASK_CR6FALSE); }
		inline bool ComparisonAnyFalse(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6TRUE) != RAYMATH_CRMASK_CR6TRUE); }
		inline bool ComparisonMixed(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6) == 0); }
		inline bool ComparisonAllInBounds(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6BOUNDS) == RAYMATH_CRMASK_CR6BOUNDS); }
		inline bool ComparisonAnyOutOfBounds(uint32_t CR) { return (((CR)&RAYMATH_CRMASK_CR6BOUNDS) != RAYMATH_CRMASK_CR6BOUNDS); }


		/****************************************************************************
		 *
		 * Data types
		 *
		 ****************************************************************************/

#pragma warning(push)
#pragma warning(disable:4068 4201 4365 4324 4820)
		 // C4068: ignore unknown pragmas
		 // C4201: nonstandard extension used : nameless struct/union
		 // C4365: Off by default noise
		 // C4324/4820: padding warnings

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 25000, "FVectorType is 16 bytes")
#endif

//------------------------------------------------------------------------------
#if defined(RAY_NO_INTRINSICS)
		struct __vector4
		{
			union
			{
				float       vector4_f32[4];
				uint32_t    vector4_u32[4];
			};
		};
#endif // RAY_NO_INTRINSICS


		struct Quaternion;
		struct Vector;

		//------------------------------------------------------------------------------
		// VectorType intrinsic: Four 32 bit floating point components aligned on a 16 byte
		// boundary and mapped to hardware vector registers
#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		typedef __m128 VectorType;
#elif defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		typedef float32x4_t VectorType;
#else
		typedef __vector4 VectorType;
#endif

		// Fix-up for (1st-3rd) VectorType parameters that are pass-in-register for x86, ARM, ARM64, and vector call; by reference otherwise
#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || RAYMATH_VECTORCALL ) && !defined(RAY_NO_INTRINSICS)
		typedef const VectorType FVectorType;
		typedef const Quaternion FQuaternion;
		typedef const Vector FVector;
#else
		typedef const VectorType& FVectorType;
		typedef const Quaternion& FQuaternion;
		typedef const Vector& FVector;
#endif

		// Fix-up for (4th) VectorType parameter to pass in-register for ARM, ARM64, and x64 vector call; by reference otherwise
#if ( defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || (RAYMATH_VECTORCALL && !defined(_M_IX86) ) ) && !defined(RAY_NO_INTRINSICS)
		typedef const VectorType GVectorType;
		typedef const Quaternion GQuaternion;
		typedef const Vector GVector;
#else
		typedef const VectorType& GVectorType;
		typedef const Quaternion& GQuaternion;
		typedef const Vector& GVector;
#endif

		// Fix-up for (5th & 6th) VectorType parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || RAYMATH_VECTORCALL ) && !defined(RAY_NO_INTRINSICS)
		typedef const VectorType HVectorType;
		typedef const Quaternion HQuaternion;
		typedef const Vector HVector;
#else
		typedef const VectorType& HVectorType;
		typedef const Quaternion& HQuaternion;
		typedef const Vector& HVector;
#endif

		// Fix-up for (7th+) VectorType parameters to pass by reference
		typedef const VectorType& CVectorType;
		typedef const Quaternion& CQuaternion;
		typedef const Vector& CVector;

		//------------------------------------------------------------------------------
		// Conversion types for constants
		RAY_ALIGNED(16) struct VectorF32
		{
			union
			{
				float f[4];
				VectorType v;
			};

			inline operator VectorType() const { return v; }
			inline operator const float* () const { return f; }
#if !defined(RAY_NO_INTRINSICS) && defined(RAY_SSE_INTRINSICS)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		RAY_ALIGNED(16) struct VectorI32
		{
			union
			{
				int32_t i[4];
				VectorType v;
			};

			inline operator VectorType() const { return v; }
#if !defined(RAY_NO_INTRINSICS) && defined(RAY_SSE_INTRINSICS)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		RAY_ALIGNED(16) struct VectorU8
		{
			union
			{
				uint8_t u[16];
				VectorType v;
			};

			inline operator VectorType() const { return v; }
#if !defined(RAY_NO_INTRINSICS) && defined(RAY_SSE_INTRINSICS)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		RAY_ALIGNED(16) struct VectorU32
		{
			union
			{
				uint32_t u[4];
				VectorType v;
			};

			inline operator VectorType() const { return v; }
#if !defined(RAY_NO_INTRINSICS) && defined(RAY_SSE_INTRINSICS)
			inline operator __m128i() const { return _mm_castps_si128(v); }
			inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
		};

		//------------------------------------------------------------------------------
		// Basic Vector
		struct Float3;
		VectorType    RAYMATH_CALLCONV     LoadFloat3(/*_In_ */const Float3* pSource);

		struct Vector
		{
			union
			{
				float f[4];
				struct
				{
					float x, y, z, w;
				};
				VectorType v;
			};

			Vector() = default;
			Vector(FVectorType v) : v(v) {}
			Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
			Vector(const VectorF32& vec) : v(vec.v) {}
			Vector(const VectorI32& vec) : v(vec.v) {}
			Vector(const VectorU8& vec) : v(vec.v) {}
			Vector(const VectorU32& vec) : v(vec.v) {}

			operator VectorType& () { return v; }
			operator const VectorType& () const { return v; }

			Vector(const Vector&) = default;
			Vector& RAYMATH_CALLCONV operator=(const Vector&) = default;

			Vector& RAYMATH_CALLCONV operator=(const Float3& float3) { *this = LoadFloat3(&float3); return *this; }

			Vector(Vector&&) = default;
			Vector& RAYMATH_CALLCONV operator=(Vector&&) = default;

			Vector  RAYMATH_CALLCONV operator-() const { return Negate(*this); }
			Vector  RAYMATH_CALLCONV operator+(FVector v1) const { return Add(*this, v1); }
			Vector  RAYMATH_CALLCONV operator+(float value) const { return Add(*this, { value, value, value, value }); }
			Vector& RAYMATH_CALLCONV operator+=(FVector v1) { v = Add(*this, v1); return *this; }
			Vector& RAYMATH_CALLCONV operator+=(float value) { v = Add(*this, { value, value, value, value }); return *this; }
			Vector& RAYMATH_CALLCONV operator++() { v = Add(*this, { 1.0f, 1.0f, 1.0f, 1.0f }); return *this; }
			Vector  RAYMATH_CALLCONV operator-(FVector v1) const { return Subtract(*this, v1); }
			Vector  RAYMATH_CALLCONV operator-(float value) const { return Subtract(*this, { value, value, value, value }); }
			Vector& RAYMATH_CALLCONV operator-=(FVector v1) { v = Subtract(*this, v1); return *this; }
			Vector& RAYMATH_CALLCONV operator-=(float value) { v = Subtract(*this, { value, value, value, value }); return *this; }
			Vector& RAYMATH_CALLCONV operator--() { v = Subtract(*this, { 1.0f, 1.0f, 1.0f, 1.0f }); return *this; }
			Vector  RAYMATH_CALLCONV operator*(FVector v1) const { return Multiply(*this, v1); }
			Vector  RAYMATH_CALLCONV operator*(float value) const { return Multiply(*this, { value, value, value, value }); }
			Vector& RAYMATH_CALLCONV operator*=(FVector v1) { v = Multiply(*this, v1); return *this; }
			Vector& RAYMATH_CALLCONV operator*=(float value) { v = Multiply(*this, { value, value, value, value }); return *this; }
			Vector  RAYMATH_CALLCONV operator/(FVector v1) const { return Divide(*this, v1); }
			Vector  RAYMATH_CALLCONV operator/(float value) const { return Divide(*this, { value, value, value, value }); }
			Vector& RAYMATH_CALLCONV operator/=(FVector v1) { v = Divide(*this, v1); return *this; }
			Vector& RAYMATH_CALLCONV operator/=(float value) { v = Divide(*this, { value, value, value, value }); return *this; }

			Vector RAYMATH_CALLCONV Equal(FVectorType vec) { return Equal(*this, vec); }
			Vector RAYMATH_CALLCONV EqualR(FVectorType vec, /*_Out_ */ uint32_t* pCR) { return EqualR(pCR, *this, vec); }
			Vector RAYMATH_CALLCONV EqualInt(FVectorType vec) { return EqualInt(*this, vec); }
			Vector RAYMATH_CALLCONV EqualIntR(FVectorType vec, /*_Out_ */ uint32_t* pCR) { return EqualIntR(pCR, *this, vec); }
			Vector RAYMATH_CALLCONV NearEqual(FVectorType V1, FVectorType Epsilon) { return NearEqual(*this, V1, Epsilon); }
			Vector RAYMATH_CALLCONV NotEqual(FVectorType V1) { return NotEqual(*this, V1); }
			Vector RAYMATH_CALLCONV NotEqualInt(FVectorType V1) { return NotEqualInt(*this, V1); }
			float  RAYMATH_CALLCONV GetByIndex(size_t i) const;
			float  RAYMATH_CALLCONV GetX() const;
			float  RAYMATH_CALLCONV GetY() const;
			float  RAYMATH_CALLCONV GetZ() const;
			float  RAYMATH_CALLCONV GetW() const;
			Vector RAYMATH_CALLCONV Round() const;
			Vector RAYMATH_CALLCONV Truncate() const;
			Vector RAYMATH_CALLCONV Floor() const;
			Vector RAYMATH_CALLCONV Ceiling() const;
			Vector RAYMATH_CALLCONV Clamp(FVectorType Min, FVectorType Max) const;
			Vector RAYMATH_CALLCONV Saturate() const;
			Vector RAYMATH_CALLCONV Min(FVectorType other) const { return Min(*this, other); }
			Vector RAYMATH_CALLCONV Max(FVectorType other) const { return Max(*this, other); }
			void   RAYMATH_CALLCONV SetByIndex(float f, size_t i);
			void   RAYMATH_CALLCONV SetX(float x);
			void   RAYMATH_CALLCONV SetY(float y);
			void   RAYMATH_CALLCONV SetZ(float z);
			void   RAYMATH_CALLCONV SetW(float w);
			Vector RAYMATH_CALLCONV Greater(FVectorType V2) const;
			Vector RAYMATH_CALLCONV GreaterR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVectorType V2) const;
			Vector RAYMATH_CALLCONV GreaterOrEqual(FVectorType V2) const;
			Vector RAYMATH_CALLCONV GreaterOrEqualR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVectorType V2) const;
			Vector RAYMATH_CALLCONV Less(FVectorType V2) const;
			Vector RAYMATH_CALLCONV LessOrEqual(FVectorType V2) const;
			Vector RAYMATH_CALLCONV InBounds(FVectorType Bounds) const;
			Vector RAYMATH_CALLCONV InBoundsR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVectorType Bounds) const;
			Vector RAYMATH_CALLCONV IsNaN() const;
			Vector RAYMATH_CALLCONV IsInfinite() const;

			Vector RAYMATH_CALLCONV AddAngles(FVectorType V2) const;
			Vector RAYMATH_CALLCONV Sum() const;
			Vector RAYMATH_CALLCONV SubtractAngles(FVectorType V2) const;
			Vector RAYMATH_CALLCONV Scale(float ScaleFactor) const;
			Vector RAYMATH_CALLCONV ReciprocalEst() const;
			Vector RAYMATH_CALLCONV Reciprocal() const;
			Vector RAYMATH_CALLCONV SqrtEst() const;
			Vector RAYMATH_CALLCONV Sqrt() const;
			Vector RAYMATH_CALLCONV ReciprocalSqrtEst() const;
			Vector RAYMATH_CALLCONV ReciprocalSqrt() const;

			static Vector RAYMATH_CALLCONV Equal(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV EqualR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVectorType V1, /*_In_ */FVectorType V2);
			static Vector RAYMATH_CALLCONV EqualInt(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV EqualIntR(/*_Out_ */ uint32_t* pCR, /*_In_ */FVectorType V, /*_In_ */FVectorType V2);
			static Vector RAYMATH_CALLCONV NearEqual(FVectorType V1, FVectorType V2, FVectorType Epsilon);
			static Vector RAYMATH_CALLCONV NotEqual(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV NotEqualInt(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Negate(FVectorType V);
			static Vector RAYMATH_CALLCONV Add(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Subtract(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Multiply(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Divide(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV ShiftLeft(FVectorType V1, FVectorType V2, uint32_t Elements);
			static Vector RAYMATH_CALLCONV RotateLeft(FVectorType V, uint32_t Elements);
			static Vector RAYMATH_CALLCONV RotateRight(FVectorType V, uint32_t Elements);
			static Vector RAYMATH_CALLCONV Insert(FVectorType VD, FVectorType VS, uint32_t VSLeftRotateElements,
				uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3);
			static Vector RAYMATH_CALLCONV Swizzle(FVectorType V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3);
			static Vector RAYMATH_CALLCONV Permute(FVectorType V1, FVectorType V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW);
			static Vector RAYMATH_CALLCONV SelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3);
			static Vector RAYMATH_CALLCONV Select(FVectorType V1, FVectorType V2, FVectorType Control);
			static Vector RAYMATH_CALLCONV MergeXY(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV MergeZW(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Min(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Max(FVectorType V1, FVectorType V2);
			static Vector RAYMATH_CALLCONV Zero();
			static Vector RAYMATH_CALLCONV Set(float x, float y, float z, float w);
			static Vector RAYMATH_CALLCONV SetInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
			static Vector RAYMATH_CALLCONV Replicate(float Value);
			static Vector RAYMATH_CALLCONV ReplicatePtr(/*_In_ */const float* pValue);
			static Vector RAYMATH_CALLCONV ReplicateInt(uint32_t Value);
			static Vector RAYMATH_CALLCONV ReplicateIntPtr(/*_In_ */const uint32_t* pValue);
			static Vector RAYMATH_CALLCONV TrueInt();
			static Vector RAYMATH_CALLCONV FalseInt();
			static Vector RAYMATH_CALLCONV SplatX(FVectorType V);
			static Vector RAYMATH_CALLCONV SplatY(FVectorType V);
			static Vector RAYMATH_CALLCONV SplatZ(FVectorType V);
			static Vector RAYMATH_CALLCONV SplatW(FVectorType V);
			static Vector RAYMATH_CALLCONV SplatOne();
			static Vector RAYMATH_CALLCONV SplatInfinity();
			static Vector RAYMATH_CALLCONV SplatQNaN();
			static Vector RAYMATH_CALLCONV SplatEpsilon();
			static Vector RAYMATH_CALLCONV SplatSignMask();
		};

		//------------------------------------------------------------------------------
		// VectorType operators

#ifndef RAYMATH_NO_VECTOR_OVERLOADS

		VectorType    RAYMATH_CALLCONV     operator+ (FVectorType V);
		VectorType    RAYMATH_CALLCONV     operator- (FVectorType V);

		VectorType& RAYMATH_CALLCONV     operator+= (VectorType& V1, FVectorType V2);
		VectorType& RAYMATH_CALLCONV     operator-= (VectorType& V1, FVectorType V2);
		VectorType& RAYMATH_CALLCONV     operator*= (VectorType& V1, FVectorType V2);
		VectorType& RAYMATH_CALLCONV     operator/= (VectorType& V1, FVectorType V2);

		VectorType& operator*= (VectorType& V, float S);
		VectorType& operator/= (VectorType& V, float S);

		VectorType    RAYMATH_CALLCONV     operator+ (FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     operator- (FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     operator* (FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     operator/ (FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     operator* (FVectorType V, float S);
		VectorType    RAYMATH_CALLCONV     operator* (float S, FVectorType V);
		VectorType    RAYMATH_CALLCONV     operator/ (FVectorType V, float S);

#endif

		//------------------------------------------------------------------------------
		// Matrix type: Sixteen 32 bit floating point components aligned on a
		// 16 byte boundary and mapped to four hardware vector registers

		struct Matrix;
		struct Float3;

		// Fix-up for (1st) Matrix parameter to pass in-register for ARM64 and vector call; by reference otherwise
#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || RAYMATH_VECTORCALL ) && !defined(RAY_NO_INTRINSICS)
		typedef const Matrix FMatrix;
#else
		typedef const Matrix& FMatrix;
#endif

		// Fix-up for (2nd+) Matrix parameters to pass by reference
		typedef const Matrix& CMatrix;

#ifdef RAY_NO_INTRINSICS
		struct Matrix
#else
		RAY_ALIGNED(16) struct Matrix
#endif
		{
#ifdef RAY_NO_INTRINSICS
			union
			{
				VectorType r[4];
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};
#else
			VectorType r[4];
#endif

			Matrix() = default;

			Matrix(const Matrix&) = default;

#if defined(_MSC_VER) && (_MSC_FULL_VER < 191426431)
			Matrix& operator= (const Matrix& M) noexcept { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }
#else
			Matrix& operator=(const Matrix&) = default;

			Matrix(Matrix&&) = default;
			Matrix& operator=(Matrix&&) = default;
#endif

			RAYMATH_CONSTEXPR Matrix(FVectorType R0, FVectorType R1, FVectorType R2, CVectorType R3) : r{ R0,R1,R2,R3 } {}
			Matrix(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
			explicit Matrix(/*_In_reads_(16) */const float* pArray);

#ifdef RAY_NO_INTRINSICS
			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
#endif

			Matrix RAYMATH_CALLCONV operator+() const { return *this; }
			Matrix RAYMATH_CALLCONV operator-() const;

			Matrix& RAYMATH_CALLCONV operator+= (FMatrix M);
			Matrix& RAYMATH_CALLCONV operator-= (FMatrix M);
			Matrix& RAYMATH_CALLCONV operator*= (FMatrix M);
			Matrix& RAYMATH_CALLCONV operator*= (float S);
			Matrix& RAYMATH_CALLCONV operator/= (float S);

			Matrix RAYMATH_CALLCONV operator+ (FMatrix M) const;
			Matrix RAYMATH_CALLCONV operator- (FMatrix M) const;
			Matrix RAYMATH_CALLCONV operator* (FMatrix M) const;
			Matrix RAYMATH_CALLCONV operator* (float S) const;
			Matrix RAYMATH_CALLCONV operator/ (float S) const;

			friend Matrix     RAYMATH_CALLCONV     operator* (float S, FMatrix M);


			bool RAYMATH_CALLCONV IsNaN() const;
			bool RAYMATH_CALLCONV IsInfinite() const;
			bool RAYMATH_CALLCONV IsIdentity() const;
			Matrix RAYMATH_CALLCONV Transpose() const;
			Matrix RAYMATH_CALLCONV Inverse(/*_Out_opt_ */VectorType* pDeterminant = nullptr) const;
			Vector RAYMATH_CALLCONV Determinant() const;
			bool RAYMATH_CALLCONV Decompose(/*_Out_ */ VectorType* outScale, /*_Out_ */ VectorType* outRotQuat, /*_Out_ */ VectorType* outTrans) const;

			static Matrix RAYMATH_CALLCONV Identity();
			static Matrix RAYMATH_CALLCONV PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV PerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV PerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV PerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV PerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV LookAtLH(FVectorType EyePosition, FVectorType FocusPosition, FVectorType UpDirection);
			static Matrix RAYMATH_CALLCONV LookAtRH(FVectorType EyePosition, FVectorType FocusPosition, FVectorType UpDirection);
			static Matrix RAYMATH_CALLCONV LookToLH(FVectorType EyePosition, FVectorType EyeDirection, FVectorType UpDirection);
			static Matrix RAYMATH_CALLCONV LookToRH(FVectorType EyePosition, FVectorType EyeDirection, FVectorType UpDirection);
			static Matrix RAYMATH_CALLCONV OrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV OrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV OrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
			static Matrix RAYMATH_CALLCONV RotationX(float Angle);
			static Matrix RAYMATH_CALLCONV RotationY(float Angle);
			static Matrix RAYMATH_CALLCONV RotationZ(float Angle);
			static Matrix RAYMATH_CALLCONV RotationRollPitchYaw(float Pitch, float Yaw, float Roll);
			static Matrix RAYMATH_CALLCONV RotationRollPitchYawFromVector(FVectorType Angles);
			static Matrix RAYMATH_CALLCONV Translation(float OffsetX, float OffsetY, float OffsetZ);
			static Matrix RAYMATH_CALLCONV Translation(const Float3& d);
			static Matrix RAYMATH_CALLCONV Translation(FVectorType Offset);
			static Matrix RAYMATH_CALLCONV Scaling(float ScaleX, float ScaleY, float ScaleZ);
			static Matrix RAYMATH_CALLCONV Scaling(const Float3& d);
			static Matrix RAYMATH_CALLCONV Scaling(FVectorType Scale);
			static Matrix RAYMATH_CALLCONV AffineTransformation2D(FVectorType Scaling, FVectorType RotationOrigin, float Rotation, FVectorType Translation);
			static Matrix RAYMATH_CALLCONV AffineTransformation(FVectorType Scaling, FVectorType RotationOrigin, FVectorType RotationFromQuaternion, GVectorType Translation);
			static Matrix RAYMATH_CALLCONV Reflect(FVectorType ReflectionPlane);
			static Matrix RAYMATH_CALLCONV Shadow(FVectorType ShadowPlane, FVectorType LightPosition);

			static Matrix RAYMATH_CALLCONV Multiply(FMatrix M1, CMatrix M2);
			static Matrix RAYMATH_CALLCONV MultiplyTranspose(FMatrix M1, CMatrix M2);

			static Matrix RAYMATH_CALLCONV Set(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
			static Matrix RAYMATH_CALLCONV RotationNormal(FVectorType NormalAxis, float Angle);
			static Matrix RAYMATH_CALLCONV RotationAxis(FVectorType Axis, float Angle);
			static Matrix RAYMATH_CALLCONV RotationFromQuaternion(const Quaternion& Quaternion);
			static Matrix RAYMATH_CALLCONV Transformation2D(FVectorType ScalingOrigin, float ScalingOrientation, FVectorType Scaling,
				FVectorType RotationOrigin, float Rotation, GVectorType Translation);
			static Matrix RAYMATH_CALLCONV Transformation(FVectorType ScalingOrigin, FVectorType ScalingOrientationQuaternion, FVectorType Scaling,
				GVectorType RotationOrigin, HVectorType RotationFromQuaternion, HVectorType Translation);
		};

		//------------------------------------------------------------------------------
		// Quaternion, can be used interchangeably with VectorType
		struct Quaternion
		{
			union
			{
				VectorType v;
				struct
				{
					float x, y, z, w;
				};
			};

			Quaternion() = default;

			Quaternion(const Quaternion&) = default;
			Quaternion& operator=(const Quaternion&) = default;

			Quaternion(Quaternion&&) = default;
			Quaternion& operator=(Quaternion&&) = default;

			RAYMATH_CONSTEXPR Quaternion(FVectorType v) : v(v) {}
			RAYMATH_CONSTEXPR Quaternion(FVector v) : v(v.v) {}
			RAYMATH_CONSTEXPR Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

			operator FVectorType() const { return v; }

			bool RAYMATH_CALLCONV operator==(FQuaternion other) const;
			bool RAYMATH_CALLCONV operator!=(FQuaternion other) const;

			Quaternion RAYMATH_CALLCONV operator*(FQuaternion other) const;
			Quaternion& RAYMATH_CALLCONV operator*=(FQuaternion other);

			bool RAYMATH_CALLCONV IsNaN() const;
			bool RAYMATH_CALLCONV IsInfinite() const;
			bool RAYMATH_CALLCONV IsIdentity() const;
			Quaternion RAYMATH_CALLCONV LengthSq() const;
			Quaternion RAYMATH_CALLCONV ReciprocalLength() const;
			Quaternion RAYMATH_CALLCONV Length() const;
			Quaternion RAYMATH_CALLCONV NormalizeEst() const;
			Quaternion RAYMATH_CALLCONV Normalize() const;
			Quaternion RAYMATH_CALLCONV Conjugate() const;
			Quaternion RAYMATH_CALLCONV Inverse() const;
			Quaternion RAYMATH_CALLCONV Ln() const;
			Quaternion RAYMATH_CALLCONV Exp() const;
			void RAYMATH_CALLCONV ToAxisAngle(/*_Out_ */ VectorType* pAxis, /*_Out_ */ float* pAngle) const;


			static Quaternion RAYMATH_CALLCONV Dot(FQuaternion Q1, FQuaternion Q2);
			static Quaternion RAYMATH_CALLCONV Multiply(FQuaternion Q1, FQuaternion Q2);
			static Quaternion RAYMATH_CALLCONV Slerp(FQuaternion Q0, FQuaternion Q1, float t);
			static Quaternion RAYMATH_CALLCONV SlerpV(FQuaternion Q0, FQuaternion Q1, FQuaternion T);
			static Quaternion RAYMATH_CALLCONV Squad(FQuaternion Q0, FQuaternion Q1, FQuaternion Q2, GQuaternion Q3, float t);
			static Quaternion RAYMATH_CALLCONV SquadV(FQuaternion Q0, FQuaternion Q1, FQuaternion Q2, GQuaternion Q3, HQuaternion T);
			static void RAYMATH_CALLCONV SquadSetup(/*_Out_ */ VectorType* pA, /*_Out_ */ VectorType* pB, /*_Out_ */ VectorType* pC, /*_In_ */FQuaternion Q0, /*_In_ */FQuaternion Q1, /*_In_ */FQuaternion Q2, /*_In_ */GQuaternion Q3);
			static Quaternion RAYMATH_CALLCONV BaryCentric(FQuaternion Q0, FQuaternion Q1, FQuaternion Q2, float f, float g);
			static Quaternion RAYMATH_CALLCONV BaryCentricV(FQuaternion Q0, FQuaternion Q1, FQuaternion Q2, GQuaternion F, HQuaternion G);

			static Quaternion RAYMATH_CALLCONV Identity();
			static Quaternion RAYMATH_CALLCONV RotationRollPitchYaw(float Pitch, float Yaw, float Roll);
			static Quaternion RAYMATH_CALLCONV RotationRollPitchYawFromVector(FVectorType Angles);
			static Quaternion RAYMATH_CALLCONV RotationNormal(FVectorType NormalAxis, float Angle);
			static Quaternion RAYMATH_CALLCONV RotationAxis(FVectorType Axis, float Angle);
			static Quaternion RAYMATH_CALLCONV RotationMatrix(FMatrix M);
		};

		//------------------------------------------------------------------------------
		// 2D VectorType; 32 bit floating point components
		struct Float2
		{
			float x;
			float y;

			Float2() = default;

			Float2(const Float2&) = default;
			Float2& operator=(const Float2&) = default;

			Float2(Float2&&) = default;
			Float2& operator=(Float2&&) = default;

			RAYMATH_CONSTEXPR Float2(float _x, float _y) : x(_x), y(_y) {}
			explicit Float2(/*_In_reads_(2) */const float* pArray) : x(pArray[0]), y(pArray[1]) {}


			Float2  RAYMATH_CALLCONV operator-() const { return { -x, -y }; }
			Float2  RAYMATH_CALLCONV operator+(const Float2& v1) const { return { x + v1.x, y + v1.y }; }
			Float2  RAYMATH_CALLCONV operator+(float value) const { return { x + value, y + value }; }
			Float2& RAYMATH_CALLCONV operator+=(const Float2& v1) { *this = *this + v1; return *this; }
			Float2& RAYMATH_CALLCONV operator+=(float value) { *this = *this + value; return *this; }
			Float2& RAYMATH_CALLCONV operator++() { *this = *this + 1.0f; return *this; }
			Float2  RAYMATH_CALLCONV operator-(const Float2& v1) const { return { x - v1.x, y - v1.y }; }
			Float2  RAYMATH_CALLCONV operator-(float value) const { return { x - value, y - value }; }
			Float2& RAYMATH_CALLCONV operator-=(const Float2& v1) { *this = *this - v1; return *this; }
			Float2& RAYMATH_CALLCONV operator-=(float value) { *this = *this - value; return *this; }
			Float2& RAYMATH_CALLCONV operator--() { *this = *this - 1.0f; return *this; }
			Float2  RAYMATH_CALLCONV operator*(const Float2& v1) const { return { x * v1.x, y * v1.y }; }
			Float2  RAYMATH_CALLCONV operator*(float value) const { return { x * value, y * value }; }
			Float2& RAYMATH_CALLCONV operator*=(const Float2& v1) { *this = *this * v1; return *this; }
			Float2& RAYMATH_CALLCONV operator*=(float value) { *this = *this * value; return *this; }
			Float2  RAYMATH_CALLCONV operator/(const Float2& v1) const { return { x / v1.x, y / v1.y }; }
			Float2  RAYMATH_CALLCONV operator/(float value) const { return { x / value, y / value }; }
			Float2& RAYMATH_CALLCONV operator/=(const Float2& v1) { *this = *this / v1; return *this; }
			Float2& RAYMATH_CALLCONV operator/=(float value) { *this = *this / value; return *this; }
		};

		// 2D VectorType; 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float2A : public Float2
		{
			Float2A() = default;

			Float2A(const Float2A&) = default;
			Float2A& operator=(const Float2A&) = default;

			Float2A(Float2A&&) = default;
			Float2A& operator=(Float2A&&) = default;

			RAYMATH_CONSTEXPR Float2A(float _x, float _y) : Float2(_x, _y) {}
			explicit Float2A(/*_In_reads_(2) */const float* pArray) : Float2(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 2D VectorType; 32 bit signed integer components
		struct Int2
		{
			int32_t x;
			int32_t y;

			Int2() = default;

			Int2(const Int2&) = default;
			Int2& operator=(const Int2&) = default;

			Int2(Int2&&) = default;
			Int2& operator=(Int2&&) = default;

			RAYMATH_CONSTEXPR Int2(int32_t _x, int32_t _y) : x(_x), y(_y) {}
			explicit Int2(/*_In_reads_(2) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]) {}
		};

		// 2D VectorType; 32 bit unsigned integer components
		struct UInt2
		{
			uint32_t x;
			uint32_t y;

			UInt2() = default;

			UInt2(const UInt2&) = default;
			UInt2& operator=(const UInt2&) = default;

			UInt2(UInt2&&) = default;
			UInt2& operator=(UInt2&&) = default;

			RAYMATH_CONSTEXPR UInt2(uint32_t _x, uint32_t _y) : x(_x), y(_y) {}
			explicit UInt2(/*_In_reads_(2) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]) {}
		};

		//------------------------------------------------------------------------------
		// 3D VectorType; 32 bit floating point components
		struct Float3
		{
			float x;
			float y;
			float z;

			Float3() = default;
			Float3(float xyz) : x(xyz), y(xyz), z(xyz) { }

			Float3(const Float3&) = default;
			Float3& operator=(const Float3&) = default;

			Float3(Float3&&) = default;
			Float3& operator=(Float3&&) = default;

			RAYMATH_CONSTEXPR Float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
			explicit Float3(/*_In_reads_(3) */const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

			Float3  RAYMATH_CALLCONV operator-() const { return { -x, -y, -z }; }
			Float3  RAYMATH_CALLCONV operator+(const Float3& v1) const { return { x + v1.x, y + v1.y, z + v1.z }; }
			Float3  RAYMATH_CALLCONV operator+(float value) const { return { x + value, y + value, z + value }; }
			Float3& RAYMATH_CALLCONV operator+=(const Float3& v1) { *this = *this + v1; return *this; }
			Float3& RAYMATH_CALLCONV operator+=(float value) { *this = *this + value; return *this; }
			Float3& RAYMATH_CALLCONV operator++() { *this = *this + 1.0f; return *this; }
			Float3  RAYMATH_CALLCONV operator-(const Float3& v1) const { return { x - v1.x, y - v1.y, z - v1.z }; }
			Float3  RAYMATH_CALLCONV operator-(float value) const { return { x - value, y - value, z - value }; }
			Float3& RAYMATH_CALLCONV operator-=(const Float3& v1) { *this = *this - v1; return *this; }
			Float3& RAYMATH_CALLCONV operator-=(float value) { *this = *this - value; return *this; }
			Float3& RAYMATH_CALLCONV operator--() { *this = *this - 1.0f; return *this; }
			Float3  RAYMATH_CALLCONV operator*(const Float3& v1) const { return { x * v1.x, y * v1.y, z * v1.z }; }
			Float3  RAYMATH_CALLCONV operator*(float value) const { return { x * value, y * value, z * value }; }
			Float3& RAYMATH_CALLCONV operator*=(const Float3& v1) { *this = *this * v1; return *this; }
			Float3& RAYMATH_CALLCONV operator*=(float value) { *this = *this * value; return *this; }
			Float3  RAYMATH_CALLCONV operator/(const Float3& v1) const { return { x / v1.x, y / v1.y, z / v1.z }; }
			Float3  RAYMATH_CALLCONV operator/(float value) const { return { x / value, y / value, z / value }; }
			Float3& RAYMATH_CALLCONV operator/=(const Float3& v1) { *this = *this / v1; return *this; }
			Float3& RAYMATH_CALLCONV operator/=(float value) { *this = *this / value; return *this; }
		};

		// 3D VectorType; 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float3A : public Float3
		{
			Float3A() = default;

			Float3A(const Float3A&) = default;
			Float3A& operator=(const Float3A&) = default;

			Float3A(Float3A&&) = default;
			Float3A& operator=(Float3A&&) = default;

			RAYMATH_CONSTEXPR Float3A(float _x, float _y, float _z) : Float3(_x, _y, _z) {}
			explicit Float3A(/*_In_reads_(3) */const float* pArray) : Float3(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 3D VectorType; 32 bit signed integer components
		struct Int3
		{
			int32_t x;
			int32_t y;
			int32_t z;

			Int3() = default;

			Int3(const Int3&) = default;
			Int3& operator=(const Int3&) = default;

			Int3(Int3&&) = default;
			Int3& operator=(Int3&&) = default;

			RAYMATH_CONSTEXPR Int3(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}
			explicit Int3(/*_In_reads_(3) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
		};

		// 3D VectorType; 32 bit unsigned integer components
		struct UInt3
		{
			uint32_t x;
			uint32_t y;
			uint32_t z;

			UInt3() = default;

			UInt3(const UInt3&) = default;
			UInt3& operator=(const UInt3&) = default;

			UInt3(UInt3&&) = default;
			UInt3& operator=(UInt3&&) = default;

			RAYMATH_CONSTEXPR UInt3(uint32_t _x, uint32_t _y, uint32_t _z) : x(_x), y(_y), z(_z) {}
			explicit UInt3(/*_In_reads_(3) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
		};

		//------------------------------------------------------------------------------
		// 4D VectorType; 32 bit floating point components
		struct Float4
		{
			float x;
			float y;
			float z;
			float w;

			Float4() = default;

			Float4(const Float4&) = default;
			Float4& operator=(const Float4&) = default;

			Float4(Float4&&) = default;
			Float4& operator=(Float4&&) = default;

			RAYMATH_CONSTEXPR Float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit Float4(/*_In_reads_(4) */const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		// 4D VectorType; 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float4A : public Float4
		{
			Float4A() = default;

			Float4A(const Float4A&) = default;
			Float4A& operator=(const Float4A&) = default;

			Float4A(Float4A&&) = default;
			Float4A& operator=(Float4A&&) = default;

			RAYMATH_CONSTEXPR Float4A(float _x, float _y, float _z, float _w) : Float4(_x, _y, _z, _w) {}
			explicit Float4A(/*_In_reads_(4) */const float* pArray) : Float4(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 4D VectorType; 32 bit signed integer components
		struct Int4
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;

			Int4() = default;

			Int4(const Int4&) = default;
			Int4& operator=(const Int4&) = default;

			Int4(Int4&&) = default;
			Int4& operator=(Int4&&) = default;

			RAYMATH_CONSTEXPR Int4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit Int4(/*_In_reads_(4) */const int32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		// 4D VectorType; 32 bit unsigned integer components
		struct UInt4
		{
			uint32_t x;
			uint32_t y;
			uint32_t z;
			uint32_t w;

			UInt4() = default;

			UInt4(const UInt4&) = default;
			UInt4& operator=(const UInt4&) = default;

			UInt4(UInt4&&) = default;
			UInt4& operator=(UInt4&&) = default;

			RAYMATH_CONSTEXPR UInt4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit UInt4(/*_In_reads_(4) */const uint32_t* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		//------------------------------------------------------------------------------
		// 3x3 Matrix: 32 bit floating point components
		struct Float3X3
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
				};
				float m[3][3];
			};

			Float3X3() = default;

			Float3X3(const Float3X3&) = default;
			Float3X3& operator=(const Float3X3&) = default;

			Float3X3(Float3X3&&) = default;
			Float3X3& operator=(Float3X3&&) = default;

			RAYMATH_CONSTEXPR Float3X3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22)
				: _11(m00), _12(m01), _13(m02),
				_21(m10), _22(m11), _23(m12),
				_31(m20), _32(m21), _33(m22) {}
			explicit Float3X3(/*_In_reads_(9) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		//------------------------------------------------------------------------------
		// 4x3 Row-major Matrix: 32 bit floating point components
		struct Float4X3
		{
			union
			{
				struct
				{
					float _11, _12, _13;
					float _21, _22, _23;
					float _31, _32, _33;
					float _41, _42, _43;
				};
				float m[4][3];
				float f[12];
			};

			Float4X3() = default;

			Float4X3(const Float4X3&) = default;
			Float4X3& operator=(const Float4X3&) = default;

			Float4X3(Float4X3&&) = default;
			Float4X3& operator=(Float4X3&&) = default;

			RAYMATH_CONSTEXPR Float4X3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22,
				float m30, float m31, float m32)
				: _11(m00), _12(m01), _13(m02),
				_21(m10), _22(m11), _23(m12),
				_31(m20), _32(m21), _33(m22),
				_41(m30), _42(m31), _43(m32) {}
			explicit Float4X3(/*_In_reads_(12) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 4x3 Row-major Matrix: 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float4X3A : public Float4X3
		{
			Float4X3A() = default;

			Float4X3A(const Float4X3A&) = default;
			Float4X3A& operator=(const Float4X3A&) = default;

			Float4X3A(Float4X3A&&) = default;
			Float4X3A& operator=(Float4X3A&&) = default;

			RAYMATH_CONSTEXPR Float4X3A(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22,
				float m30, float m31, float m32) :
				Float4X3(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
			explicit Float4X3A(/*_In_reads_(12) */const float* pArray) : Float4X3(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 3x4 Column-major Matrix: 32 bit floating point components
		struct Float3X4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
				};
				float m[3][4];
				float f[12];
			};

			Float3X4() = default;

			Float3X4(const Float3X4&) = default;
			Float3X4& operator=(const Float3X4&) = default;

			Float3X4(Float3X4&&) = default;
			Float3X4& operator=(Float3X4&&) = default;

			RAYMATH_CONSTEXPR Float3X4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23)
				: _11(m00), _12(m01), _13(m02), _14(m03),
				_21(m10), _22(m11), _23(m12), _24(m13),
				_31(m20), _32(m21), _33(m22), _34(m23) {}
			explicit Float3X4(/*_In_reads_(12) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 3x4 Column-major Matrix: 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float3X4A : public Float3X4
		{
			Float3X4A() = default;

			Float3X4A(const Float3X4A&) = default;
			Float3X4A& operator=(const Float3X4A&) = default;

			Float3X4A(Float3X4A&&) = default;
			Float3X4A& operator=(Float3X4A&&) = default;

			RAYMATH_CONSTEXPR Float3X4A(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23) :
				Float3X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23) {}
			explicit Float3X4A(/*_In_reads_(12) */const float* pArray) : Float3X4(pArray) {}
		};

		//------------------------------------------------------------------------------
		// 4x4 Matrix: 32 bit floating point components
		struct Float4X4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};

			Float4X4() = default;

			Float4X4(const Float4X4&) = default;
			Float4X4& operator=(const Float4X4&) = default;

			Float4X4(Float4X4&&) = default;
			Float4X4& operator=(Float4X4&&) = default;

			RAYMATH_CONSTEXPR Float4X4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: _11(m00), _12(m01), _13(m02), _14(m03),
				_21(m10), _22(m11), _23(m12), _24(m13),
				_31(m20), _32(m21), _33(m22), _34(m23),
				_41(m30), _42(m31), _43(m32), _44(m33) {}
			explicit Float4X4(/*_In_reads_(16) */const float* pArray);

			float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
			float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
		};

		// 4x4 Matrix: 32 bit floating point components aligned on a 16 byte boundary
		RAY_ALIGNED(16) struct Float4X4A : public Float4X4
		{
			Float4X4A() = default;

			Float4X4A(const Float4X4A&) = default;
			Float4X4A& operator=(const Float4X4A&) = default;

			Float4X4A(Float4X4A&&) = default;
			Float4X4A& operator=(Float4X4A&&) = default;

			RAYMATH_CONSTEXPR Float4X4A(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: Float4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
			explicit Float4X4A(/*_In_reads_(16) */const float* pArray) : Float4X4(pArray) {}
		};

		////////////////////////////////////////////////////////////////////////////////

		// Constants that require structs from aboce
		namespace Constants
		{
			inline const Vector DefaultForwardVector = { 0.0f, 0.0f, 1.0f, 0.0f };
			inline const Vector DefaultUpVector = { 0.0f, 1.0f, 0.0f, 0.0f };
			inline const Vector DefaultBackwardVector = { 0.0f, 0.0f, -1.0f, 0.0f };
			inline const Vector DefaultLeftVector = { -1.0f, 0.0f, 0.0f, 0.0f };
			inline const Vector DefaultRightVector = { 1.0f, 0.0f, 0.0f, 0.0f };
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

#pragma warning(pop)

		/****************************************************************************
		 *
		 * Data conversion operations
		 *
		 ****************************************************************************/

		VectorType    RAYMATH_CALLCONV     ConvertVectorIntToFloat(FVectorType VInt, uint32_t DivExponent);
		VectorType    RAYMATH_CALLCONV     ConvertVectorFloatToInt(FVectorType VFloat, uint32_t MulExponent);
		VectorType    RAYMATH_CALLCONV     ConvertVectorUIntToFloat(FVectorType VUInt, uint32_t DivExponent);
		VectorType    RAYMATH_CALLCONV     ConvertVectorFloatToUInt(FVectorType VFloat, uint32_t MulExponent);

#if defined(__XNAMATH_H__) && defined(VectorSetBinaryConstant)
#undef VectorSetBinaryConstant
#undef VectorSplatConstant
#undef VectorSplatConstantInt
#endif

		VectorType    RAYMATH_CALLCONV     VectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3);
		VectorType    RAYMATH_CALLCONV     VectorSplatConstant(int32_t IntConstant, uint32_t DivExponent);
		VectorType    RAYMATH_CALLCONV     VectorSplatConstantInt(int32_t IntConstant);

		/****************************************************************************
		 *
		 * Load operations
		 *
		 ****************************************************************************/

		VectorType    RAYMATH_CALLCONV     LoadInt(/*_In_ */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat(/*_In_ */const float* pSource);

		VectorType    RAYMATH_CALLCONV     LoadInt2(/*_In_reads_(2) */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadInt2A(/*_In_reads_(2) */const uint32_t* PSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat2(/*_In_ */const Float2* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat2A(/*_In_ */const Float2A* pSource);
		VectorType    RAYMATH_CALLCONV     LoadSInt2(/*_In_ */const Int2* pSource);
		VectorType    RAYMATH_CALLCONV     LoadUInt2(/*_In_ */const UInt2* pSource);

		VectorType    RAYMATH_CALLCONV     LoadInt3(/*_In_reads_(3) */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadInt3A(/*_In_reads_(3) */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat3(/*_In_ */const Float3* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat3A(/*_In_ */const Float3A* pSource);
		VectorType    RAYMATH_CALLCONV     LoadSInt3(/*_In_ */const Int3* pSource);
		VectorType    RAYMATH_CALLCONV     LoadUInt3(/*_In_ */const UInt3* pSource);

		VectorType    RAYMATH_CALLCONV     LoadInt4(/*_In_reads_(4) */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadInt4A(/*_In_reads_(4) */const uint32_t* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat4(/*_In_ */const Float4* pSource);
		VectorType    RAYMATH_CALLCONV     LoadFloat4A(/*_In_ */const Float4A* pSource);
		VectorType    RAYMATH_CALLCONV     LoadSInt4(/*_In_ */const Int4* pSource);
		VectorType    RAYMATH_CALLCONV     LoadUInt4(/*_In_ */const UInt4* pSource);

		Matrix    RAYMATH_CALLCONV     LoadFloat3x3(/*_In_ */const Float3X3* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat4x3(/*_In_ */const Float4X3* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat4x3A(/*_In_ */const Float4X3A* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat3x4(/*_In_ */const Float3X4* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat3x4A(/*_In_ */const Float3X4A* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat4x4(/*_In_ */const Float4X4* pSource);
		Matrix    RAYMATH_CALLCONV     LoadFloat4x4A(/*_In_ */const Float4X4A* pSource);

		/****************************************************************************
		 *
		 * Store operations
		 *
		 ****************************************************************************/

		void        RAYMATH_CALLCONV     StoreInt(/*_Out_ */ uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat(/*_Out_ */ float* pDestination, /*_In_ */FVectorType V);

		void        RAYMATH_CALLCONV     StoreInt2(/*_Out_writes_(2)*/ uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreInt2A(/*_Out_writes_(2)*/ uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat2(/*_Out_ */ Float2* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat2A(/*_Out_ */ Float2A* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreSInt2(/*_Out_ */ Int2* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreUInt2(/*_Out_ */ UInt2* pDestination, /*_In_ */FVectorType V);

		void        RAYMATH_CALLCONV     StoreInt3(/*_Out_writes_(3) */uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreInt3A(/*_Out_writes_(3)*/ uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat3(/*_Out_ */ Float3* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat3A(/*_Out_ */ Float3A* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreSInt3(/*_Out_ */ Int3* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreUInt3(/*_Out_ */ UInt3* pDestination, /*_In_ */FVectorType V);

		void        RAYMATH_CALLCONV     StoreInt4(/*_Out_writes_(4) */uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreInt4A(/*_Out_writes_(4) */uint32_t* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat4(/*_Out_ */ Float4* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreFloat4A(/*_Out_ */ Float4A* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreSInt4(/*_Out_ */ Int4* pDestination, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     StoreUInt4(/*_Out_ */ UInt4* pDestination, /*_In_ */FVectorType V);

		void        RAYMATH_CALLCONV     StoreFloat3x3(/*_Out_ */ Float3X3* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat4x3(/*_Out_ */ Float4X3* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat4x3A(/*_Out_ */ Float4X3A* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat3x4(/*_Out_ */ Float3X4* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat3x4A(/*_Out_ */ Float3X4A* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat4x4(/*_Out_ */ Float4X4* pDestination, /*_In_ */FMatrix M);
		void        RAYMATH_CALLCONV     StoreFloat4x4A(/*_Out_ */ Float4X4A* pDestination, /*_In_ */FMatrix M);

		/****************************************************************************
		 *
		 * General vector operations
		 *
		 ****************************************************************************/

		void        RAYMATH_CALLCONV     VectorGetByIndexPtr(/*_Out_ */ float* f, /*_In_ */FVectorType V, /*_In_ */size_t i);
		void        RAYMATH_CALLCONV     VectorGetXPtr(/*_Out_ */ float* x, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetYPtr(/*_Out_ */ float* y, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetZPtr(/*_Out_ */ float* z, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetWPtr(/*_Out_ */ float* w, /*_In_ */FVectorType V);

		uint32_t    RAYMATH_CALLCONV     VectorGetIntByIndex(FVectorType V, size_t i);
		uint32_t    RAYMATH_CALLCONV     VectorGetIntX(FVectorType V);
		uint32_t    RAYMATH_CALLCONV     VectorGetIntY(FVectorType V);
		uint32_t    RAYMATH_CALLCONV     VectorGetIntZ(FVectorType V);
		uint32_t    RAYMATH_CALLCONV     VectorGetIntW(FVectorType V);

		void        RAYMATH_CALLCONV     VectorGetIntByIndexPtr(/*_Out_ */ uint32_t* x, /*_In_ */FVectorType V, /*_In_ */size_t i);
		void        RAYMATH_CALLCONV     VectorGetIntXPtr(/*_Out_ */ uint32_t* x, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetIntYPtr(/*_Out_ */ uint32_t* y, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetIntZPtr(/*_Out_ */ uint32_t* z, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorGetIntWPtr(/*_Out_ */ uint32_t* w, /*_In_ */FVectorType V);

		VectorType    RAYMATH_CALLCONV     VectorSetByIndexPtr(/*_In_ */FVectorType V, /*_In_ */const float* f, /*_In_ */size_t i);
		VectorType    RAYMATH_CALLCONV     VectorSetXPtr(/*_In_ */FVectorType V, /*_In_ */const float* x);
		VectorType    RAYMATH_CALLCONV     VectorSetYPtr(/*_In_ */FVectorType V, /*_In_ */const float* y);
		VectorType    RAYMATH_CALLCONV     VectorSetZPtr(/*_In_ */FVectorType V, /*_In_ */const float* z);
		VectorType    RAYMATH_CALLCONV     VectorSetWPtr(/*_In_ */FVectorType V, /*_In_ */const float* w);

		VectorType    RAYMATH_CALLCONV     VectorSetIntByIndex(FVectorType V, uint32_t x, size_t i);
		VectorType    RAYMATH_CALLCONV     VectorSetIntX(FVectorType V, uint32_t x);
		VectorType    RAYMATH_CALLCONV     VectorSetIntY(FVectorType V, uint32_t y);
		VectorType    RAYMATH_CALLCONV     VectorSetIntZ(FVectorType V, uint32_t z);
		VectorType    RAYMATH_CALLCONV     VectorSetIntW(FVectorType V, uint32_t w);

		VectorType    RAYMATH_CALLCONV     VectorSetIntByIndexPtr(/*_In_ */FVectorType V, /*_In_ */const uint32_t* x, /*_In_ */size_t i);
		VectorType    RAYMATH_CALLCONV     VectorSetIntXPtr(/*_In_ */FVectorType V, /*_In_ */const uint32_t* x);
		VectorType    RAYMATH_CALLCONV     VectorSetIntYPtr(/*_In_ */FVectorType V, /*_In_ */const uint32_t* y);
		VectorType    RAYMATH_CALLCONV     VectorSetIntZPtr(/*_In_ */FVectorType V, /*_In_ */const uint32_t* z);
		VectorType    RAYMATH_CALLCONV     VectorSetIntWPtr(/*_In_ */FVectorType V, /*_In_ */const uint32_t* w);

		VectorType    RAYMATH_CALLCONV     VectorAndInt(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorAndCInt(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorOrInt(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorNorInt(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorXorInt(FVectorType V1, FVectorType V2);

		VectorType    RAYMATH_CALLCONV     VectorMultiplyAdd(FVectorType V1, FVectorType V2, FVectorType V3);
		VectorType    RAYMATH_CALLCONV     VectorNegativeMultiplySubtract(FVectorType V1, FVectorType V2, FVectorType V3);
		VectorType    RAYMATH_CALLCONV     VectorExp2(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorExpE(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorExp(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorLog2(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorLogE(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorLog(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorPow(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorAbs(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorMod(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     VectorModAngles(FVectorType Angles);
		VectorType    RAYMATH_CALLCONV     VectorSin(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorSinEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorCos(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorCosEst(FVectorType V);
		void        RAYMATH_CALLCONV     VectorSinCos(/*_Out_ */ VectorType* pSin, /*_Out_ */ VectorType* pCos, /*_In_ */FVectorType V);
		void        RAYMATH_CALLCONV     VectorSinCosEst(/*_Out_ */ VectorType* pSin, /*_Out_ */ VectorType* pCos, /*_In_ */FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorTan(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorTanEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorSinH(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorCosH(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorTanH(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorASin(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorASinEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorACos(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorACosEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorATan(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorATanEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     VectorATan2(FVectorType Y, FVectorType X);
		VectorType    RAYMATH_CALLCONV     VectorATan2Est(FVectorType Y, FVectorType X);
		VectorType    RAYMATH_CALLCONV     VectorLerp(FVectorType V0, FVectorType V1, float t);
		VectorType    RAYMATH_CALLCONV     VectorLerpV(FVectorType V0, FVectorType V1, FVectorType T);
		VectorType    RAYMATH_CALLCONV     VectorHermite(FVectorType Position0, FVectorType Tangent0, FVectorType Position1, GVectorType Tangent1, float t);
		VectorType    RAYMATH_CALLCONV     VectorHermiteV(FVectorType Position0, FVectorType Tangent0, FVectorType Position1, GVectorType Tangent1, HVectorType T);
		VectorType    RAYMATH_CALLCONV     VectorCatmullRom(FVectorType Position0, FVectorType Position1, FVectorType Position2, GVectorType Position3, float t);
		VectorType    RAYMATH_CALLCONV     VectorCatmullRomV(FVectorType Position0, FVectorType Position1, FVectorType Position2, GVectorType Position3, HVectorType T);
		VectorType    RAYMATH_CALLCONV     VectorBaryCentric(FVectorType Position0, FVectorType Position1, FVectorType Position2, float f, float g);
		VectorType    RAYMATH_CALLCONV     VectorBaryCentricV(FVectorType Position0, FVectorType Position1, FVectorType Position2, GVectorType F, HVectorType G);

		/****************************************************************************
		 *
		 * 2D vector operations
		 *
		 ****************************************************************************/

		bool        RAYMATH_CALLCONV     Vector2Equal(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector2EqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2EqualInt(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector2EqualIntR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2NearEqual(FVectorType V1, FVectorType V2, FVectorType Epsilon);
		bool        RAYMATH_CALLCONV     Vector2NotEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2NotEqualInt(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2Greater(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector2GreaterR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2GreaterOrEqual(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector2GreaterOrEqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2Less(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2LessOrEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector2InBounds(FVectorType V, FVectorType Bounds);

		bool        RAYMATH_CALLCONV     Vector2IsNaN(FVectorType V);
		bool        RAYMATH_CALLCONV     Vector2IsInfinite(FVectorType V);

		VectorType    RAYMATH_CALLCONV     Vector2Dot(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector2Cross(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector2LengthSq(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2ReciprocalLengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2ReciprocalLength(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2LengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2Length(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2NormalizeEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2Normalize(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2ClampLength(FVectorType V, float LengthMin, float LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector2ClampLengthV(FVectorType V, FVectorType LengthMin, FVectorType LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector2Reflect(FVectorType Incident, FVectorType Normal);
		VectorType    RAYMATH_CALLCONV     Vector2Refract(FVectorType Incident, FVectorType Normal, float RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector2RefractV(FVectorType Incident, FVectorType Normal, FVectorType RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector2Orthogonal(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector2AngleBetweenNormalsEst(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector2AngleBetweenNormals(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector2AngleBetweenVectors(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector2LinePointDistance(FVectorType LinePoint1, FVectorType LinePoint2, FVectorType Point);
		VectorType    RAYMATH_CALLCONV     Vector2IntersectLine(FVectorType Line1Point1, FVectorType Line1Point2, FVectorType Line2Point1, GVectorType Line2Point2);
		VectorType    RAYMATH_CALLCONV     Vector2Transform(FVectorType V, FMatrix M);
		Float4* RAYMATH_CALLCONV     Vector2TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		VectorType    RAYMATH_CALLCONV     Vector2TransformCoord(FVectorType V, FMatrix M);
		Float2* RAYMATH_CALLCONV     Vector2TransformCoordStream(/*_Out_writes_bytes_(sizeof(Float2) + OutputStride * (VectorCount - 1)) */Float2* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		VectorType    RAYMATH_CALLCONV     Vector2TransformNormal(FVectorType V, FMatrix M);
		Float2* RAYMATH_CALLCONV     Vector2TransformNormalStream(/*_Out_writes_bytes_(sizeof(Float2) + OutputStride * (VectorCount - 1)) */Float2* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float2) + InputStride * (VectorCount - 1)) */const Float2* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);

		/****************************************************************************
		 *
		 * 3D vector operations
		 *
		 ****************************************************************************/

		bool        RAYMATH_CALLCONV     Vector3Equal(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector3EqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3EqualInt(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector3EqualIntR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3NearEqual(FVectorType V1, FVectorType V2, FVectorType Epsilon);
		bool        RAYMATH_CALLCONV     Vector3NotEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3NotEqualInt(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3Greater(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector3GreaterR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3GreaterOrEqual(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector3GreaterOrEqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3Less(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3LessOrEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector3InBounds(FVectorType V, FVectorType Bounds);

		bool        RAYMATH_CALLCONV     Vector3IsNaN(FVectorType V);
		bool        RAYMATH_CALLCONV     Vector3IsInfinite(FVectorType V);

		VectorType    RAYMATH_CALLCONV     Vector3Dot(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector3Cross(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector3LengthSq(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3ReciprocalLengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3ReciprocalLength(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3LengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3Length(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3NormalizeEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3Normalize(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3ClampLength(FVectorType V, float LengthMin, float LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector3ClampLengthV(FVectorType V, FVectorType LengthMin, FVectorType LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector3Reflect(FVectorType Incident, FVectorType Normal);
		VectorType    RAYMATH_CALLCONV     Vector3Refract(FVectorType Incident, FVectorType Normal, float RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector3RefractV(FVectorType Incident, FVectorType Normal, FVectorType RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector3Orthogonal(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector3AngleBetweenNormalsEst(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector3AngleBetweenNormals(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector3AngleBetweenVectors(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector3LinePointDistance(FVectorType LinePoint1, FVectorType LinePoint2, FVectorType Point);
		void        RAYMATH_CALLCONV     Vector3ComponentsFromNormal(/*_Out_ */ VectorType* pParallel, /*_Out_ */ VectorType* pPerpendicular, /*_In_ */FVectorType V, /*_In_ */FVectorType Normal);
		VectorType    RAYMATH_CALLCONV     Vector3Rotate(FVectorType V, FQuaternion RotationFromQuaternion);
		VectorType    RAYMATH_CALLCONV     Vector3InverseRotate(FVectorType V, FQuaternion RotationFromQuaternion);
		VectorType    RAYMATH_CALLCONV     Vector3Transform(FVectorType V, FMatrix M);
		Float4* RAYMATH_CALLCONV     Vector3TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		VectorType    RAYMATH_CALLCONV     Vector3TransformCoord(FVectorType V, FMatrix M);
		Float3* RAYMATH_CALLCONV     Vector3TransformCoordStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		VectorType    RAYMATH_CALLCONV     Vector3TransformNormal(FVectorType V, FMatrix M);
		Float3* RAYMATH_CALLCONV     Vector3TransformNormalStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);
		VectorType    RAYMATH_CALLCONV     Vector3Project(FVectorType V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
			FMatrix Projection, CMatrix View, CMatrix World);
		Float3* RAYMATH_CALLCONV     Vector3ProjectStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount,
			/*_In_ */float ViewportX, /*_In_ */float ViewportY, /*_In_ */float ViewportWidth, /*_In_ */float ViewportHeight, /*_In_ */float ViewportMinZ, /*_In_ */float ViewportMaxZ,
			/*_In_ */FMatrix Projection, /*_In_ */CMatrix View, /*_In_ */CMatrix World);
		VectorType    RAYMATH_CALLCONV     Vector3Unproject(FVectorType V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ,
			FMatrix Projection, CMatrix View, CMatrix World);
		Float3* RAYMATH_CALLCONV     Vector3UnprojectStream(/*_Out_writes_bytes_(sizeof(Float3) + OutputStride * (VectorCount - 1)) */Float3* pOutputStream,
			/*_In_ */size_t OutputStride,
			/* _In_reads_bytes_(sizeof(Float3) + InputStride * (VectorCount - 1)) */const Float3* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount,
			/*_In_ */float ViewportX, /*_In_ */float ViewportY, /*_In_ */float ViewportWidth, /*_In_ */float ViewportHeight, /*_In_ */float ViewportMinZ, /*_In_ */float ViewportMaxZ,
			/*_In_ */FMatrix Projection, /*_In_ */CMatrix View, /*_In_ */CMatrix World);

		/****************************************************************************
		 *
		 * 4D vector operations
		 *
		 ****************************************************************************/

		bool        RAYMATH_CALLCONV     Vector4Equal(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector4EqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4EqualInt(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector4EqualIntR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4NearEqual(FVectorType V1, FVectorType V2, FVectorType Epsilon);
		bool        RAYMATH_CALLCONV     Vector4NotEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4NotEqualInt(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4Greater(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector4GreaterR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4GreaterOrEqual(FVectorType V1, FVectorType V2);
		uint32_t    RAYMATH_CALLCONV     Vector4GreaterOrEqualR(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4Less(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4LessOrEqual(FVectorType V1, FVectorType V2);
		bool        RAYMATH_CALLCONV     Vector4InBounds(FVectorType V, FVectorType Bounds);

		bool        RAYMATH_CALLCONV     Vector4IsNaN(FVectorType V);
		bool        RAYMATH_CALLCONV     Vector4IsInfinite(FVectorType V);

		VectorType    RAYMATH_CALLCONV     Vector4Dot(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector4Cross(FVectorType V1, FVectorType V2, FVectorType V3);
		VectorType    RAYMATH_CALLCONV     Vector4LengthSq(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4ReciprocalLengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4ReciprocalLength(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4LengthEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4Length(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4NormalizeEst(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4Normalize(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4ClampLength(FVectorType V, float LengthMin, float LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector4ClampLengthV(FVectorType V, FVectorType LengthMin, FVectorType LengthMax);
		VectorType    RAYMATH_CALLCONV     Vector4Reflect(FVectorType Incident, FVectorType Normal);
		VectorType    RAYMATH_CALLCONV     Vector4Refract(FVectorType Incident, FVectorType Normal, float RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector4RefractV(FVectorType Incident, FVectorType Normal, FVectorType RefractionIndex);
		VectorType    RAYMATH_CALLCONV     Vector4Orthogonal(FVectorType V);
		VectorType    RAYMATH_CALLCONV     Vector4AngleBetweenNormalsEst(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector4AngleBetweenNormals(FVectorType N1, FVectorType N2);
		VectorType    RAYMATH_CALLCONV     Vector4AngleBetweenVectors(FVectorType V1, FVectorType V2);
		VectorType    RAYMATH_CALLCONV     Vector4Transform(FVectorType V, FMatrix M);
		Float4* RAYMATH_CALLCONV     Vector4TransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (VectorCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float4) + InputStride * (VectorCount - 1)) */const Float4* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t VectorCount, /*_In_ */FMatrix M);


		/****************************************************************************
		 *
		 * Plane operations
		 *
		 ****************************************************************************/

		bool        RAYMATH_CALLCONV     PlaneEqual(FVectorType P1, FVectorType P2);
		bool        RAYMATH_CALLCONV     PlaneNearEqual(FVectorType P1, FVectorType P2, FVectorType Epsilon);
		bool        RAYMATH_CALLCONV     PlaneNotEqual(FVectorType P1, FVectorType P2);

		bool        RAYMATH_CALLCONV     PlaneIsNaN(FVectorType P);
		bool        RAYMATH_CALLCONV     PlaneIsInfinite(FVectorType P);

		VectorType    RAYMATH_CALLCONV     PlaneDot(FVectorType P, FVectorType V);
		VectorType    RAYMATH_CALLCONV     PlaneDotCoord(FVectorType P, FVectorType V);
		VectorType    RAYMATH_CALLCONV     PlaneDotNormal(FVectorType P, FVectorType V);
		VectorType    RAYMATH_CALLCONV     PlaneNormalizeEst(FVectorType P);
		VectorType    RAYMATH_CALLCONV     PlaneNormalize(FVectorType P);
		VectorType    RAYMATH_CALLCONV     PlaneIntersectLine(FVectorType P, FVectorType LinePoint1, FVectorType LinePoint2);
		void        RAYMATH_CALLCONV     PlaneIntersectPlane(/*_Out_ */ VectorType* pLinePoint1, /*_Out_ */ VectorType* pLinePoint2, /*_In_ */FVectorType P1, /*_In_ */FVectorType P2);
		VectorType    RAYMATH_CALLCONV     PlaneTransform(FVectorType P, FMatrix M);
		Float4* RAYMATH_CALLCONV     PlaneTransformStream(/*_Out_writes_bytes_(sizeof(Float4) + OutputStride * (PlaneCount - 1)) */Float4* pOutputStream,
			/*_In_ */size_t OutputStride,
			/*_In_reads_bytes_(sizeof(Float4) + InputStride * (PlaneCount - 1)) */const Float4* pInputStream,
			/*_In_ */size_t InputStride, /*_In_ */size_t PlaneCount, /*_In_ */FMatrix M);

		VectorType    RAYMATH_CALLCONV     PlaneFromPointNormal(FVectorType Point, FVectorType Normal);
		VectorType    RAYMATH_CALLCONV     PlaneFromPoints(FVectorType Point1, FVectorType Point2, FVectorType Point3);

		/****************************************************************************
		 *
		 * Color operations
		 *
		 ****************************************************************************/

		bool        RAYMATH_CALLCONV     ColorEqual(FVectorType C1, FVectorType C2);
		bool        RAYMATH_CALLCONV     ColorNotEqual(FVectorType C1, FVectorType C2);
		bool        RAYMATH_CALLCONV     ColorGreater(FVectorType C1, FVectorType C2);
		bool        RAYMATH_CALLCONV     ColorGreaterOrEqual(FVectorType C1, FVectorType C2);
		bool        RAYMATH_CALLCONV     ColorLess(FVectorType C1, FVectorType C2);
		bool        RAYMATH_CALLCONV     ColorLessOrEqual(FVectorType C1, FVectorType C2);

		bool        RAYMATH_CALLCONV     ColorIsNaN(FVectorType C);
		bool        RAYMATH_CALLCONV     ColorIsInfinite(FVectorType C);

		VectorType    RAYMATH_CALLCONV     ColorNegative(FVectorType C);
		VectorType    RAYMATH_CALLCONV     ColorModulate(FVectorType C1, FVectorType C2);
		VectorType    RAYMATH_CALLCONV     ColorAdjustSaturation(FVectorType C, float Saturation);
		VectorType    RAYMATH_CALLCONV     ColorAdjustContrast(FVectorType C, float Contrast);

		VectorType    RAYMATH_CALLCONV     ColorRGBToHSL(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorHSLToRGB(FVectorType hsl);

		VectorType    RAYMATH_CALLCONV     ColorRGBToHSV(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorHSVToRGB(FVectorType hsv);

		VectorType    RAYMATH_CALLCONV     ColorRGBToYUV(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorYUVToRGB(FVectorType yuv);

		VectorType    RAYMATH_CALLCONV     ColorRGBToYUV_HD(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorYUVToRGB_HD(FVectorType yuv);

		VectorType    RAYMATH_CALLCONV     ColorRGBToXYZ(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorXYZToRGB(FVectorType xyz);

		VectorType    RAYMATH_CALLCONV     ColorXYZToSRGB(FVectorType xyz);
		VectorType    RAYMATH_CALLCONV     ColorSRGBToXYZ(FVectorType srgb);

		VectorType    RAYMATH_CALLCONV     ColorRGBToSRGB(FVectorType rgb);
		VectorType    RAYMATH_CALLCONV     ColorSRGBToRGB(FVectorType srgb);


		/****************************************************************************
		 *
		 * Miscellaneous operations
		 *
		 ****************************************************************************/

		bool            VerifyCPUSupport();

		VectorType    RAYMATH_CALLCONV     FresnelTerm(FVectorType CosIncidentAngle, FVectorType RefractionIndex);

		bool            ScalarNearEqual(float S1, float S2, float Epsilon);
		float           ScalarModAngle(float Value);

		float           ScalarSin(float Value);
		float           ScalarSinEst(float Value);

		float           ScalarCos(float Value);
		float           ScalarCosEst(float Value);

		void            ScalarSinCos(/*_Out_ */ float* pSin, /*_Out_ */ float* pCos, float Value);
		void            ScalarSinCosEst(/*_Out_ */ float* pSin, /*_Out_ */ float* pCos, float Value);

		float           ScalarASin(float Value);
		float           ScalarASinEst(float Value);

		float           ScalarACos(float Value);
		float           ScalarACosEst(float Value);

		/****************************************************************************
		 *
		 * Templates
		 *
		 ****************************************************************************/

		template<class T> inline T RayMathMin(T a, T b) { return (a < b) ? a : b; }
		template<class T> inline T RayMathMax(T a, T b) { return (a > b) ? a : b; }

		//------------------------------------------------------------------------------

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)

// PermuteHelper internal template (SSE only)
		namespace Internal
		{
			// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
			template<uint32_t Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
			{
				static VectorType     RAYMATH_CALLCONV     Permute(FVectorType v1, FVectorType v2)
				{
					static const VectorU32 selectMask =
					{ { {
							WhichX ? 0xFFFFFFFF : 0,
							WhichY ? 0xFFFFFFFF : 0,
							WhichZ ? 0xFFFFFFFF : 0,
							WhichW ? 0xFFFFFFFF : 0,
					} } };

					VectorType shuffled1 = RAYMATH_PERMUTE_PS(v1, Shuffle);
					VectorType shuffled2 = RAYMATH_PERMUTE_PS(v2, Shuffle);

					VectorType masked1 = _mm_andnot_ps(selectMask, shuffled1);
					VectorType masked2 = _mm_and_ps(selectMask, shuffled2);

					return _mm_or_ps(masked1, masked2);
				}
			};

			// Fast path for permutes that only read from the first vector.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, false, false>
			{
				static VectorType     RAYMATH_CALLCONV     Permute(FVectorType v1, FVectorType) { return RAYMATH_PERMUTE_PS(v1, Shuffle); }
			};

			// Fast path for permutes that only read from the second vector.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, true, true>
			{
				static VectorType     RAYMATH_CALLCONV     Permute(FVectorType, FVectorType v2) { return RAYMATH_PERMUTE_PS(v2, Shuffle); }
			};

			// Fast path for permutes that read XY from the first vector, ZW from the second.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, true, true>
			{
				static VectorType     RAYMATH_CALLCONV     Permute(FVectorType v1, FVectorType v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
			};

			// Fast path for permutes that read XY from the second vector, ZW from the first.
			template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
			{
				static VectorType     RAYMATH_CALLCONV     Permute(FVectorType v1, FVectorType v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
			};
		}

#endif // RAY_SSE_INTRINSICS && !RAY_NO_INTRINSICS

		// General permute template
		template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW>
		inline VectorType     RAYMATH_CALLCONV     VectorPermute(FVectorType V1, FVectorType V2)
		{
			static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
			static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
			static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
			static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			const uint32_t Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

			const bool WhichX = PermuteX > 3;
			const bool WhichY = PermuteY > 3;
			const bool WhichZ = PermuteZ > 3;
			const bool WhichW = PermuteW > 3;

			return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
#else

			return Vector::Permute(V1, V2, PermuteX, PermuteY, PermuteZ, PermuteW);

#endif
		}

		// Special-case permute templates
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 2, 3>(FVectorType V1, FVectorType) { return V1; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 5, 6, 7>(FVectorType, FVectorType V2) { return V2; }

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 4, 5>(FVectorType V1, FVectorType V2) { return _mm_movelh_ps(V1, V2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<6, 7, 2, 3>(FVectorType V1, FVectorType V2) { return _mm_movehl_ps(V1, V2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 4, 1, 5>(FVectorType V1, FVectorType V2) { return _mm_unpacklo_ps(V1, V2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 6, 3, 7>(FVectorType V1, FVectorType V2) { return _mm_unpackhi_ps(V1, V2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 3, 6, 7>(FVectorType V1, FVectorType V2) { return _mm_castpd_ps(_mm_unpackhi_pd(_mm_castps_pd(V1), _mm_castps_pd(V2))); }
#endif

#if defined(RAY_SSE4_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 1, 2, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x1); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 5, 2, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 5, 2, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x3); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 6, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x4); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 1, 6, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x5); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 5, 6, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x6); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 5, 6, 3>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x7); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 2, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x8); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 1, 2, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0x9); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 5, 2, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0xA); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 5, 2, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0xB); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 6, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0xC); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<4, 1, 6, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0xD); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 5, 6, 7>(FVectorType V1, FVectorType V2) { return _mm_blend_ps(V1, V2, 0xE); }
#endif

#if defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)

		// If the indices are all in the range 0-3 or 4-7, then use Vector::Swizzle instead
		// The mirror cases are not spelled out here as the programmer can always swap the arguments
		// (i.e. prefer permutes where the X element comes from the V1 vector instead of the V2 vector)

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 4, 5>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_low_f32(V1), vget_low_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 0, 4, 5>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_low_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 5, 4>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 0, 5, 4>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 3, 6, 7>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_high_f32(V1), vget_high_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<3, 2, 6, 7>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_high_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 3, 7, 6>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_high_f32(V2))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<3, 2, 7, 6>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 6, 7>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_low_f32(V1), vget_high_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 0, 6, 7>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_high_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 1, 7, 6>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_high_f32(V2))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 0, 7, 6>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<3, 2, 4, 5>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_low_f32(V2)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 3, 5, 4>(FVectorType V1, FVectorType V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_low_f32(V2))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<3, 2, 5, 4>(FVectorType V1, FVectorType V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 4, 2, 6>(FVectorType V1, FVectorType V2) { return vtrnq_f32(V1, V2).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 5, 3, 7>(FVectorType V1, FVectorType V2) { return vtrnq_f32(V1, V2).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 4, 1, 5>(FVectorType V1, FVectorType V2) { return vzipq_f32(V1, V2).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 6, 3, 7>(FVectorType V1, FVectorType V2) { return vzipq_f32(V1, V2).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<0, 2, 4, 6>(FVectorType V1, FVectorType V2) { return vuzpq_f32(V1, V2).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 3, 5, 7>(FVectorType V1, FVectorType V2) { return vuzpq_f32(V1, V2).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<1, 2, 3, 4>(FVectorType V1, FVectorType V2) { return vextq_f32(V1, V2, 1); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<2, 3, 4, 5>(FVectorType V1, FVectorType V2) { return vextq_f32(V1, V2, 2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorPermute<3, 4, 5, 6>(FVectorType V1, FVectorType V2) { return vextq_f32(V1, V2, 3); }

#endif // RAY_ARM_NEON_INTRINSICS && !RAY_NO_INTRINSICS

		//------------------------------------------------------------------------------

		// General swizzle template
		template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW>
		inline VectorType     RAYMATH_CALLCONV     VectorSwizzle(FVectorType V)
		{
			static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
			static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
			static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
			static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			return RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(SwizzleW, SwizzleZ, SwizzleY, SwizzleX));
#else

			return Vector::Swizzle(V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);

#endif
		}

		// Specialized swizzles
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 1, 2, 3>(FVectorType V) { return V; }

#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 1, 0, 1>(FVectorType V) { return _mm_movelh_ps(V, V); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 3, 2, 3>(FVectorType V) { return _mm_movehl_ps(V, V); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 1, 1>(FVectorType V) { return _mm_unpacklo_ps(V, V); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 2, 3, 3>(FVectorType V) { return _mm_unpackhi_ps(V, V); }
#endif

#if defined(RAY_SSE3_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 2, 2>(FVectorType V) { return _mm_moveldup_ps(V); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 1, 3, 3>(FVectorType V) { return _mm_movehdup_ps(V); }
#endif

#if defined(RAY_AVX2_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 0, 0>(FVectorType V) { return _mm_broadcastss_ps(V); }
#endif

#if defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 0, 0>(FVectorType V) { return vdupq_lane_f32(vget_low_f32(V), 0); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 1, 1, 1>(FVectorType V) { return vdupq_lane_f32(vget_low_f32(V), 1); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 2, 2, 2>(FVectorType V) { return vdupq_lane_f32(vget_high_f32(V), 0); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<3, 3, 3, 3>(FVectorType V) { return vdupq_lane_f32(vget_high_f32(V), 1); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 0, 3, 2>(FVectorType V) { return vrev64q_f32(V); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 1, 0, 1>(FVectorType V) { float32x2_t vt = vget_low_f32(V); return vcombine_f32(vt, vt); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 3, 2, 3>(FVectorType V) { float32x2_t vt = vget_high_f32(V); return vcombine_f32(vt, vt); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 0, 1, 0>(FVectorType V) { float32x2_t vt = vrev64_f32(vget_low_f32(V)); return vcombine_f32(vt, vt); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<3, 2, 3, 2>(FVectorType V) { float32x2_t vt = vrev64_f32(vget_high_f32(V)); return vcombine_f32(vt, vt); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 1, 3, 2>(FVectorType V) { return vcombine_f32(vget_low_f32(V), vrev64_f32(vget_high_f32(V))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 0, 2, 3>(FVectorType V) { return vcombine_f32(vrev64_f32(vget_low_f32(V)), vget_high_f32(V)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 3, 1, 0>(FVectorType V) { return vcombine_f32(vget_high_f32(V), vrev64_f32(vget_low_f32(V))); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<3, 2, 0, 1>(FVectorType V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vget_low_f32(V)); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<3, 2, 1, 0>(FVectorType V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vrev64_f32(vget_low_f32(V))); }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 2, 2>(FVectorType V) { return vtrnq_f32(V, V).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 1, 3, 3>(FVectorType V) { return vtrnq_f32(V, V).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 0, 1, 1>(FVectorType V) { return vzipq_f32(V, V).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 2, 3, 3>(FVectorType V) { return vzipq_f32(V, V).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<0, 2, 0, 2>(FVectorType V) { return vuzpq_f32(V, V).val[0]; }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 3, 1, 3>(FVectorType V) { return vuzpq_f32(V, V).val[1]; }

		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<1, 2, 3, 0>(FVectorType V) { return vextq_f32(V, V, 1); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<2, 3, 0, 1>(FVectorType V) { return vextq_f32(V, V, 2); }
		template<> inline VectorType      RAYMATH_CALLCONV     VectorSwizzle<3, 0, 1, 2>(FVectorType V) { return vextq_f32(V, V, 3); }

#endif // RAY_ARM_NEON_INTRINSICS && !RAY_NO_INTRINSICS

		//------------------------------------------------------------------------------

		template<uint32_t Elements>
		inline VectorType     RAYMATH_CALLCONV     VectorShiftLeft(FVectorType V1, FVectorType V2)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorPermute<Elements, (Elements + 1), (Elements + 2), (Elements + 3)>(V1, V2);
		}

		template<uint32_t Elements>
		inline VectorType     RAYMATH_CALLCONV     VectorRotateLeft(FVectorType V)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorSwizzle<Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3>(V);
		}

		template<uint32_t Elements>
		inline VectorType     RAYMATH_CALLCONV     VectorRotateRight(FVectorType V)
		{
			static_assert(Elements < 4, "Elements template parameter out of range");
			return VectorSwizzle<(4 - Elements) & 3, (5 - Elements) & 3, (6 - Elements) & 3, (7 - Elements) & 3>(V);
		}

		template<uint32_t VSLeftRotateElements, uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3>
		inline VectorType     RAYMATH_CALLCONV     VectorInsert(FVectorType VD, FVectorType VS)
		{
			VectorType Control = Vector::SelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
			return Vector::Select(VD, VectorRotateLeft<VSLeftRotateElements>(VS), Control);
		}

		/****************************************************************************
		 *
		 * Globals
		 *
		 ****************************************************************************/

		 // The purpose of the following global constants is to prevent redundant
		 // reloading of the constants when they are referenced by more than one
		 // separate inline math routine called within the same function.  Declaring
		 // a constant locally within a routine is sufficient to prevent redundant
		 // reloads of that constant when that single routine is called multiple
		 // times in a function, but if the constant is used (and declared) in a
		 // separate math routine it would be reloaded.

		namespace Constants
		{
#ifndef RAYMATH_GLOBALCONST
#ifdef _MSC_VER
#define RAYMATH_GLOBALCONST extern const __declspec(selectany)
#elif defined(__GNUC__)
#define RAYMATH_GLOBALCONST extern const __attribute__ ((selectany)) inline
#endif
#endif

			RAYMATH_GLOBALCONST VectorF32 SinCoefficients0 = { { { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f } } };
			RAYMATH_GLOBALCONST VectorF32 SinCoefficients1 = { { { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ } } };
			RAYMATH_GLOBALCONST VectorF32 CosCoefficients0 = { { { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f } } };
			RAYMATH_GLOBALCONST VectorF32 CosCoefficients1 = { { { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ } } };
			RAYMATH_GLOBALCONST VectorF32 TanCoefficients0 = { { { 1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f } } };
			RAYMATH_GLOBALCONST VectorF32 TanCoefficients1 = { { { 2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f } } };
			RAYMATH_GLOBALCONST VectorF32 TanCoefficients2 = { { { 5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f } } };
			RAYMATH_GLOBALCONST VectorF32 ArcCoefficients0 = { { { +1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f } } };
			RAYMATH_GLOBALCONST VectorF32 ArcCoefficients1 = { { { +0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f } } };
			RAYMATH_GLOBALCONST VectorF32 ATanCoefficients0 = { { { -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f } } };
			RAYMATH_GLOBALCONST VectorF32 ATanCoefficients1 = { { { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f } } };
			RAYMATH_GLOBALCONST VectorF32 ATanEstCoefficients0 = { { { +0.999866f, +0.999866f, +0.999866f, +0.999866f } } };
			RAYMATH_GLOBALCONST VectorF32 ATanEstCoefficients1 = { { { -0.3302995f, +0.180141f, -0.085133f, +0.0208351f } } };
			RAYMATH_GLOBALCONST VectorF32 TanEstCoefficients = { { { 2.484f, -1.954923183e-1f, 2.467401101f, Constants::RAY_1DIVPI } } };
			RAYMATH_GLOBALCONST VectorF32 ArcEstCoefficients = { { { +1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f } } };
			RAYMATH_GLOBALCONST VectorF32 PiConstants0 = { { { Constants::PI, Constants::RAY_2PI, Constants::RAY_1DIVPI, Constants::RAY_1DIV2PI } } };
			RAYMATH_GLOBALCONST VectorF32 IdentityR0 = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 IdentityR1 = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 IdentityR2 = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 IdentityR3 = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegIdentityR0 = { { { -1.0f, 0.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegIdentityR1 = { { { 0.0f, -1.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegIdentityR2 = { { { 0.0f, 0.0f, -1.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegIdentityR3 = { { { 0.0f, 0.0f, 0.0f, -1.0f } } };
			RAYMATH_GLOBALCONST VectorU32 NegativeZero = { { { 0x80000000, 0x80000000, 0x80000000, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorU32 Negate3 = { { { 0x80000000, 0x80000000, 0x80000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 MaskXY = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 Mask3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 MaskX = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 MaskY = { { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 MaskZ = { { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorU32 MaskW = { { { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF } } };
			RAYMATH_GLOBALCONST VectorF32 One = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 One3 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 Zero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 Two = { { { 2.f, 2.f, 2.f, 2.f } } };
			RAYMATH_GLOBALCONST VectorF32 Four = { { { 4.f, 4.f, 4.f, 4.f } } };
			RAYMATH_GLOBALCONST VectorF32 Six = { { { 6.f, 6.f, 6.f, 6.f } } };
			RAYMATH_GLOBALCONST VectorF32 NegativeOne = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 OneHalf = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
			RAYMATH_GLOBALCONST VectorF32 NegativeOneHalf = { { { -0.5f, -0.5f, -0.5f, -0.5f } } };
			RAYMATH_GLOBALCONST VectorF32 NegativeTwoPi = { { { -Constants::RAY_2PI, -Constants::RAY_2PI, -Constants::RAY_2PI, -Constants::RAY_2PI } } };
			RAYMATH_GLOBALCONST VectorF32 NegativePi = { { { -Constants::PI, -Constants::PI, -Constants::PI, -Constants::PI } } };
			RAYMATH_GLOBALCONST VectorF32 HalfPi = { { { Constants::PIDIV2, Constants::PIDIV2, Constants::PIDIV2, Constants::PIDIV2 } } };
			RAYMATH_GLOBALCONST VectorF32 Pi = { { { Constants::PI, Constants::PI, Constants::PI, Constants::PI } } };
			RAYMATH_GLOBALCONST VectorF32 ReciprocalPi = { { { Constants::RAY_1DIVPI, Constants::RAY_1DIVPI, Constants::RAY_1DIVPI, Constants::RAY_1DIVPI } } };
			RAYMATH_GLOBALCONST VectorF32 TwoPi = { { { Constants::RAY_2PI, Constants::RAY_2PI, Constants::RAY_2PI, Constants::RAY_2PI } } };
			RAYMATH_GLOBALCONST VectorF32 ReciprocalTwoPi = { { { Constants::RAY_1DIV2PI, Constants::RAY_1DIV2PI, Constants::RAY_1DIV2PI, Constants::RAY_1DIV2PI } } };
			RAYMATH_GLOBALCONST VectorF32 Epsilon = { { { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f } } };
			RAYMATH_GLOBALCONST VectorI32 Infinity = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
			RAYMATH_GLOBALCONST VectorI32 QNaN = { { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } } };
			RAYMATH_GLOBALCONST VectorI32 QNaNTest = { { { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF } } };
			RAYMATH_GLOBALCONST VectorI32 AbsMask = { { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } } };
			RAYMATH_GLOBALCONST VectorI32 FltMin = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
			RAYMATH_GLOBALCONST VectorI32 FltMax = { { { 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF } } };
			RAYMATH_GLOBALCONST VectorU32 NegOneMask = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } } };
			RAYMATH_GLOBALCONST VectorU32 MaskA8R8G8B8 = { { { 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipA8R8G8B8 = { { { 0x00000000, 0x00000000, 0x00000000, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorF32 FixAA8R8G8B8 = { { { 0.0f, 0.0f, 0.0f, float(0x80000000U) } } };
			RAYMATH_GLOBALCONST VectorF32 NormalizeA8R8G8B8 = { { { 1.0f / (255.0f * float(0x10000)), 1.0f / (255.0f * float(0x100)), 1.0f / 255.0f, 1.0f / (255.0f * float(0x1000000)) } } };
			RAYMATH_GLOBALCONST VectorU32 MaskA2B10G10R10 = { { { 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipA2B10G10R10 = { { { 0x00000200, 0x00080000, 0x20000000, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorF32 FixAA2B10G10R10 = { { { -512.0f, -512.0f * float(0x400), -512.0f * float(0x100000), float(0x80000000U) } } };
			RAYMATH_GLOBALCONST VectorF32 NormalizeA2B10G10R10 = { { { 1.0f / 511.0f, 1.0f / (511.0f * float(0x400)), 1.0f / (511.0f * float(0x100000)), 1.0f / (3.0f * float(0x40000000)) } } };
			RAYMATH_GLOBALCONST VectorU32 MaskX16Y16 = { { { 0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorI32 FlipX16Y16 = { { { 0x00008000, 0x00000000, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorF32 FixX16Y16 = { { { -32768.0f, 0.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NormalizeX16Y16 = { { { 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorU32 MaskX16Y16Z16W16 = { { { 0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000 } } };
			RAYMATH_GLOBALCONST VectorI32 FlipX16Y16Z16W16 = { { { 0x00008000, 0x00008000, 0x00000000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorF32 FixX16Y16Z16W16 = { { { -32768.0f, -32768.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NormalizeX16Y16Z16W16 = { { { 1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f) } } };
			RAYMATH_GLOBALCONST VectorF32 NoFraction = { { { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f } } };
			RAYMATH_GLOBALCONST VectorI32 MaskByte = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
			RAYMATH_GLOBALCONST VectorF32 NegateX = { { { -1.0f, 1.0f, 1.0f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegateY = { { { 1.0f, -1.0f, 1.0f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegateZ = { { { 1.0f, 1.0f, -1.0f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 NegateW = { { { 1.0f, 1.0f, 1.0f, -1.0f } } };
			RAYMATH_GLOBALCONST VectorU32 Select0101 = { { { RAYMATH_SELECT_0, RAYMATH_SELECT_1, RAYMATH_SELECT_0, RAYMATH_SELECT_1 } } };
			RAYMATH_GLOBALCONST VectorU32 Select1010 = { { { RAYMATH_SELECT_1, RAYMATH_SELECT_0, RAYMATH_SELECT_1, RAYMATH_SELECT_0 } } };
			RAYMATH_GLOBALCONST VectorI32 OneHalfMinusEpsilon = { { { 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD } } };
			RAYMATH_GLOBALCONST VectorU32 Select1000 = { { { RAYMATH_SELECT_1, RAYMATH_SELECT_0, RAYMATH_SELECT_0, RAYMATH_SELECT_0 } } };
			RAYMATH_GLOBALCONST VectorU32 Select1100 = { { { RAYMATH_SELECT_1, RAYMATH_SELECT_1, RAYMATH_SELECT_0, RAYMATH_SELECT_0 } } };
			RAYMATH_GLOBALCONST VectorU32 Select1110 = { { { RAYMATH_SELECT_1, RAYMATH_SELECT_1, RAYMATH_SELECT_1, RAYMATH_SELECT_0 } } };
			RAYMATH_GLOBALCONST VectorU32 Select1011 = { { { RAYMATH_SELECT_1, RAYMATH_SELECT_0, RAYMATH_SELECT_1, RAYMATH_SELECT_1 } } };
			RAYMATH_GLOBALCONST VectorF32 FixupY16 = { { { 1.0f, 1.0f / 65536.0f, 0.0f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 FixupY16W16 = { { { 1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f } } };
			RAYMATH_GLOBALCONST VectorU32 FlipY = { { { 0, 0x80000000, 0, 0 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipZ = { { { 0, 0, 0x80000000, 0 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipW = { { { 0, 0, 0, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipYZ = { { { 0, 0x80000000, 0x80000000, 0 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipZW = { { { 0, 0, 0x80000000, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorU32 FlipYW = { { { 0, 0x80000000, 0, 0x80000000 } } };
			RAYMATH_GLOBALCONST VectorI32 MaskDec4 = { { { 0x3FF, 0x3FF << 10, 0x3FF << 20, static_cast<int>(0xC0000000) } } };
			RAYMATH_GLOBALCONST VectorI32 XorDec4 = { { { 0x200, 0x200 << 10, 0x200 << 20, 0 } } };
			RAYMATH_GLOBALCONST VectorF32 AddUDec4 = { { { 0, 0, 0, 32768.0f * 65536.0f } } };
			RAYMATH_GLOBALCONST VectorF32 AddDec4 = { { { -512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0 } } };
			RAYMATH_GLOBALCONST VectorF32 MulDec4 = { { { 1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f) } } };
			RAYMATH_GLOBALCONST VectorU32 MaskByte4 = { { { 0xFF, 0xFF00, 0xFF0000, 0xFF000000 } } };
			RAYMATH_GLOBALCONST VectorI32 XorByte4 = { { { 0x80, 0x8000, 0x800000, 0x00000000 } } };
			RAYMATH_GLOBALCONST VectorF32 AddByte4 = { { { -128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0 } } };
			RAYMATH_GLOBALCONST VectorF32 FixUnsigned = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
			RAYMATH_GLOBALCONST VectorF32 MaxInt = { { { 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f } } };
			RAYMATH_GLOBALCONST VectorF32 MaxUInt = { { { 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f } } };
			RAYMATH_GLOBALCONST VectorF32 UnsignedFix = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
			RAYMATH_GLOBALCONST VectorF32 srgbScale = { { { 12.92f, 12.92f, 12.92f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorF32 srgbA = { { { 0.055f, 0.055f, 0.055f, 0.0f } } };
			RAYMATH_GLOBALCONST VectorF32 srgbA1 = { { { 1.055f, 1.055f, 1.055f, 1.0f } } };
			RAYMATH_GLOBALCONST VectorI32 ExponentBias = { { { 127, 127, 127, 127 } } };
			RAYMATH_GLOBALCONST VectorI32 SubnormalExponent = { { { -126, -126, -126, -126 } } };
			RAYMATH_GLOBALCONST VectorI32 NumTrailing = { { { 23, 23, 23, 23 } } };
			RAYMATH_GLOBALCONST VectorI32 MinNormal = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
			RAYMATH_GLOBALCONST VectorU32 NegInfinity = { { { 0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000 } } };
			RAYMATH_GLOBALCONST VectorU32 NegQNaN = { { { 0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000 } } };
			RAYMATH_GLOBALCONST VectorI32 Bin128 = { { { 0x43000000, 0x43000000, 0x43000000, 0x43000000 } } };
			RAYMATH_GLOBALCONST VectorU32 BinNeg150 = { { { 0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000 } } };
			RAYMATH_GLOBALCONST VectorI32 Vector253 = { { { 253, 253, 253, 253 } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst1 = { { { -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst2 = { { { +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst3 = { { { -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst4 = { { { +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst5 = { { { -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst6 = { { { +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f } } };
			RAYMATH_GLOBALCONST VectorF32 ExpEst7 = { { { -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst0 = { { { +1.442693f, +1.442693f, +1.442693f, +1.442693f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst1 = { { { -0.721242f, -0.721242f, -0.721242f, -0.721242f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst2 = { { { +0.479384f, +0.479384f, +0.479384f, +0.479384f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst3 = { { { -0.350295f, -0.350295f, -0.350295f, -0.350295f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst4 = { { { +0.248590f, +0.248590f, +0.248590f, +0.248590f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst5 = { { { -0.145700f, -0.145700f, -0.145700f, -0.145700f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst6 = { { { +0.057148f, +0.057148f, +0.057148f, +0.057148f } } };
			RAYMATH_GLOBALCONST VectorF32 LogEst7 = { { { -0.010578f, -0.010578f, -0.010578f, -0.010578f } } };
			RAYMATH_GLOBALCONST VectorF32 LgE = { { { +1.442695f, +1.442695f, +1.442695f, +1.442695f } } };
			RAYMATH_GLOBALCONST VectorF32 InvLgE = { { { +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f } } };
			RAYMATH_GLOBALCONST VectorF32 UByteMax = { { { 255.0f, 255.0f, 255.0f, 255.0f } } };
			RAYMATH_GLOBALCONST VectorF32 ByteMin = { { { -127.0f, -127.0f, -127.0f, -127.0f } } };
			RAYMATH_GLOBALCONST VectorF32 ByteMax = { { { 127.0f, 127.0f, 127.0f, 127.0f } } };
			RAYMATH_GLOBALCONST VectorF32 ShortMin = { { { -32767.0f, -32767.0f, -32767.0f, -32767.0f } } };
			RAYMATH_GLOBALCONST VectorF32 ShortMax = { { { 32767.0f, 32767.0f, 32767.0f, 32767.0f } } };
			RAYMATH_GLOBALCONST VectorF32 UShortMax = { { { 65535.0f, 65535.0f, 65535.0f, 65535.0f } } };

		}
		/****************************************************************************
		 *
		 * Implementation
		 *
		 ****************************************************************************/

#pragma warning(push)
#pragma warning(disable:4068 4214 4204 4365 4616 4640 6001 6101)
		 // C4068/4616: ignore unknown pragmas
		 // C4214/4204: nonstandard extension used
		 // C4365/4640: Off by default noise
		 // C6001/6101: False positives

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 25000, "FVectorType is 16 bytes")
#pragma prefast(disable : 26495, "Union initialization confuses /analyze")
#endif

//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSetBinaryConstant(uint32_t C0, uint32_t C1, uint32_t C2, uint32_t C3)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] = (0 - (C0 & 1)) & 0x3F800000;
			vResult.u[1] = (0 - (C1 & 1)) & 0x3F800000;
			vResult.u[2] = (0 - (C2 & 1)) & 0x3F800000;
			vResult.u[3] = (0 - (C3 & 1)) & 0x3F800000;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] = (0 - (C0 & 1)) & 0x3F800000;
			vResult.u[1] = (0 - (C1 & 1)) & 0x3F800000;
			vResult.u[2] = (0 - (C2 & 1)) & 0x3F800000;
			vResult.u[3] = (0 - (C3 & 1)) & 0x3F800000;
			return vResult.v;
#else // RAYMATH_SSE_INTRINSICS_
			static const VectorU32 vMask1 = { { { 1, 1, 1, 1 } } };
			// Move the parms to a vector
			__m128i vTemp = _mm_set_epi32(static_cast<int>(C3), static_cast<int>(C2), static_cast<int>(C1), static_cast<int>(C0));
			// Mask off the low bits
			vTemp = _mm_and_si128(vTemp, vMask1);
			// 0xFFFFFFFF on true bits
			vTemp = _mm_cmpeq_epi32(vTemp, vMask1);
			// 0xFFFFFFFF -> 1.0f, 0x00000000 -> 0.0f
			vTemp = _mm_and_si128(vTemp, Constants::One);
			return _mm_castsi128_ps(vTemp);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSplatConstant(int32_t IntConstant, uint32_t DivExponent)
		{
			assert(IntConstant >= -16 && IntConstant <= 15);
			assert(DivExponent < 32);
#if defined(RAY_NO_INTRINSICS)

			using At0::Ray::ConvertVectorIntToFloat;

			VectorI32 V = { { { IntConstant, IntConstant, IntConstant, IntConstant } } };
			return ConvertVectorIntToFloat(V.v, DivExponent);

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Splat the int
			int32x4_t vScale = vdupq_n_s32(IntConstant);
			// Convert to a float
			VectorType vResult = vcvtq_f32_s32(vScale);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value (It's really a float)
			vScale = vdupq_n_u32(uScale);
			// Multiply by the reciprocal (Perform a right shift by DivExponent)
			vResult = vmulq_f32(vResult, reinterpret_cast<const float32x4_t*>(&vScale)[0]);
			return vResult;
#else // RAYMATH_SSE_INTRINSICS_
			// Splat the int
			__m128i vScale = _mm_set1_epi32(IntConstant);
			// Convert to a float
			VectorType vResult = _mm_cvtepi32_ps(vScale);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value (It's really a float)
			vScale = _mm_set1_epi32(static_cast<int>(uScale));
			// Multiply by the reciprocal (Perform a right shift by DivExponent)
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSplatConstantInt(int32_t IntConstant)
		{
			assert(IntConstant >= -16 && IntConstant <= 15);
#if defined(RAY_NO_INTRINSICS)

			VectorI32 V = { { { IntConstant, IntConstant, IntConstant, IntConstant } } };
			return V.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t V = vdupq_n_s32(IntConstant);
			return reinterpret_cast<float32x4_t*>(&V)[0];
#else // RAYMATH_SSE_INTRINSICS_
			__m128i V = _mm_set1_epi32(IntConstant);
			return _mm_castsi128_ps(V);
#endif
		}


		//-------------------------------------------------------------------------------------
	// DirectXMathConvert.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------

	/****************************************************************************
	 *
	 * Data conversion
	 *
	 ****************************************************************************/

	 //------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4701)
// C4701: false positives

		inline VectorType RAYMATH_CALLCONV ConvertVectorIntToFloat
		(
			FVectorType    VInt,
			uint32_t     DivExponent
		)
		{
			assert(DivExponent < 32);
#if defined(RAY_NO_INTRINSICS)
			float fScale = 1.0f / static_cast<float>(1U << DivExponent);
			uint32_t ElementIndex = 0;
			VectorType Result;
			do {
				auto iTemp = static_cast<int32_t>(VInt.vector4_u32[ElementIndex]);
				Result.vector4_f32[ElementIndex] = static_cast<float>(iTemp) * fScale;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float fScale = 1.0f / (float)(1U << DivExponent);
			float32x4_t vResult = vcvtq_f32_s32(VInt);
			return vmulq_n_f32(vResult, fScale);
#else // RAY_SSE_INTRINSICS
			// Convert to floats
			VectorType vResult = _mm_cvtepi32_ps(_mm_castps_si128(VInt));
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat the scalar value
			__m128i vScale = _mm_set1_epi32(static_cast<int>(uScale));
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(vScale));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ConvertVectorFloatToInt
		(
			FVectorType    VFloat,
			uint32_t     MulExponent
		)
		{
			assert(MulExponent < 32);
#if defined(RAY_NO_INTRINSICS)
			// Get the scalar factor.
			auto fScale = static_cast<float>(1U << MulExponent);
			uint32_t ElementIndex = 0;
			VectorType Result;
			do {
				int32_t iResult;
				float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
				if (fTemp <= -(65536.0f * 32768.0f)) {
					iResult = (-0x7FFFFFFF) - 1;
				}
				else if (fTemp > (65536.0f * 32768.0f) - 128.0f) {
					iResult = 0x7FFFFFFF;
				}
				else {
					iResult = static_cast<int32_t>(fTemp);
				}
				Result.vector4_u32[ElementIndex] = static_cast<uint32_t>(iResult);
			} while (++ElementIndex < 4);
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
			// In case of positive overflow, detect it
			uint32x4_t vOverflow = vcgtq_f32(vResult, Constants::MaxInt);
			// Float to int conversion
			int32x4_t vResulti = vcvtq_s32_f32(vResult);
			// If there was positive overflow, set to 0x7FFFFFFF
			vResult = vandq_u32(vOverflow, Constants::AbsMask);
			vOverflow = vbicq_u32(vResulti, vOverflow);
			vOverflow = vorrq_u32(vOverflow, vResult);
			return vOverflow;
#else // RAY_SSE_INTRINSICS
			VectorType vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
			vResult = _mm_mul_ps(vResult, VFloat);
			// In case of positive overflow, detect it
			VectorType vOverflow = _mm_cmpgt_ps(vResult, Constants::MaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// If there was positive overflow, set to 0x7FFFFFFF
			vResult = _mm_and_ps(vOverflow, Constants::AbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			return vOverflow;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ConvertVectorUIntToFloat
		(
			FVectorType     VUInt,
			uint32_t      DivExponent
		)
		{
			assert(DivExponent < 32);
#if defined(RAY_NO_INTRINSICS)
			float fScale = 1.0f / static_cast<float>(1U << DivExponent);
			uint32_t ElementIndex = 0;
			VectorType Result;
			do {
				Result.vector4_f32[ElementIndex] = static_cast<float>(VUInt.vector4_u32[ElementIndex]) * fScale;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float fScale = 1.0f / (float)(1U << DivExponent);
			float32x4_t vResult = vcvtq_f32_u32(VUInt);
			return vmulq_n_f32(vResult, fScale);
#else // RAY_SSE_INTRINSICS
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			VectorType vMask = _mm_and_ps(VUInt, Constants::NegativeZero);
			// Force all values positive
			VectorType vResult = _mm_xor_ps(VUInt, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Constants::FixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			// Convert DivExponent into 1.0f/(1<<DivExponent)
			uint32_t uScale = 0x3F800000U - (DivExponent << 23);
			// Splat
			iMask = _mm_set1_epi32(static_cast<int>(uScale));
			vResult = _mm_mul_ps(vResult, _mm_castsi128_ps(iMask));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ConvertVectorFloatToUInt
		(
			FVectorType     VFloat,
			uint32_t      MulExponent
		)
		{
			assert(MulExponent < 32);
#if defined(RAY_NO_INTRINSICS)
			// Get the scalar factor.
			auto fScale = static_cast<float>(1U << MulExponent);
			uint32_t ElementIndex = 0;
			VectorType Result;
			do {
				uint32_t uResult;
				float fTemp = VFloat.vector4_f32[ElementIndex] * fScale;
				if (fTemp <= 0.0f) {
					uResult = 0;
				}
				else if (fTemp >= (65536.0f * 65536.0f)) {
					uResult = 0xFFFFFFFFU;
				}
				else {
					uResult = static_cast<uint32_t>(fTemp);
				}
				Result.vector4_u32[ElementIndex] = uResult;
			} while (++ElementIndex < 4);
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vResult = vmulq_n_f32(VFloat, (float)(1U << MulExponent));
			// In case of overflow, detect it
			uint32x4_t vOverflow = vcgtq_f32(vResult, Constants::MaxUInt);
			// Float to int conversion
			uint32x4_t vResulti = vcvtq_u32_f32(vResult);
			// If there was overflow, set to 0xFFFFFFFFU
			vResult = vbicq_u32(vResulti, vOverflow);
			vOverflow = vorrq_u32(vOverflow, vResult);
			return vOverflow;
#else // RAY_SSE_INTRINSICS
			VectorType vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
			vResult = _mm_mul_ps(vResult, VFloat);
			// Clamp to >=0
			vResult = _mm_max_ps(vResult, Constants::Zero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			VectorType vOverflow = _mm_cmpgt_ps(vResult, Constants::MaxUInt);
			VectorType vValue = Constants::UnsignedFix;
			// Too large for a signed integer?
			VectorType vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, Constants::NegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			return vResult;
#endif
		}

#pragma warning(pop)

		/****************************************************************************
		 *
		 * VectorType and matrix load operations
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		inline VectorType RAYMATH_CALLCONV LoadInt(const uint32_t* pSource)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = *pSource;
			V.vector4_u32[1] = 0;
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t zero = vdupq_n_u32(0);
			return vld1q_lane_u32(pSource, zero, 0);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_load_ss(reinterpret_cast<const float*>(pSource));
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat(const float* pSource)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = *pSource;
			V.vector4_f32[1] = 0.f;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t zero = vdupq_n_f32(0);
			return vld1q_lane_f32(pSource, zero, 0);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_load_ss(pSource);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt2
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t x = vld1_u32(pSource);
			uint32x2_t zero = vdup_n_u32(0);
			return vcombine_u32(x, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pSource));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pSource + 1));
			return _mm_unpacklo_ps(x, y);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt2A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = 0;
			V.vector4_u32[3] = 0;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t x = vld1_u32_ex(pSource, 64);
			uint32x2_t zero = vdup_n_u32(0);
			return vcombine_u32(x, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat2
		(
			const Float2* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t x = vld1_f32(reinterpret_cast<const float*>(pSource));
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(x, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(&pSource->x);
			__m128 y = _mm_load_ss(&pSource->y);
			return _mm_unpacklo_ps(x, y);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat2A
		(
			const Float2A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t x = vld1_f32_ex(reinterpret_cast<const float*>(pSource), 64);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(x, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadSInt2
		(
			const Int2* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x2_t x = vld1_s32(reinterpret_cast<const int32_t*>(pSource));
			float32x2_t v = vcvt_f32_s32(x);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(v, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 V = _mm_unpacklo_ps(x, y);
			return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadUInt2
		(
			const UInt2* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = 0.f;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t x = vld1_u32(reinterpret_cast<const uint32_t*>(pSource));
			float32x2_t v = vcvt_f32_u32(x);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(v, zero);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 V = _mm_unpacklo_ps(x, y);
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			VectorType vMask = _mm_and_ps(V, Constants::NegativeZero);
			// Force all values positive
			VectorType vResult = _mm_xor_ps(V, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Constants::FixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt3
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = 0;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t x = vld1_u32(pSource);
			uint32x2_t zero = vdup_n_u32(0);
			uint32x2_t y = vld1_lane_u32(pSource + 2, zero, 0);
			return vcombine_u32(x, y);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pSource));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pSource + 1));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
			__m128 xy = _mm_unpacklo_ps(x, y);
			return _mm_movelh_ps(xy, z);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt3A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = 0;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Reads an extra integer which is zero'd
			uint32x4_t V = vld1q_u32_ex(pSource, 128);
			return vsetq_lane_u32(0, V, 3);
#elif defined(RAY_SSE_INTRINSICS)
			// Reads an extra integer which is zero'd
			__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
			V = _mm_and_si128(V, Constants::Mask3);
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat3
		(
			const Float3* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t x = vld1_f32(reinterpret_cast<const float*>(pSource));
			float32x2_t zero = vdup_n_f32(0);
			float32x2_t y = vld1_lane_f32(reinterpret_cast<const float*>(pSource) + 2, zero, 0);
			return vcombine_f32(x, y);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(&pSource->x);
			__m128 y = _mm_load_ss(&pSource->y);
			__m128 z = _mm_load_ss(&pSource->z);
			__m128 xy = _mm_unpacklo_ps(x, y);
			return _mm_movelh_ps(xy, z);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat3A
		(
			const Float3A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Reads an extra float which is zero'd
			float32x4_t V = vld1q_f32_ex(reinterpret_cast<const float*>(pSource), 128);
			return vsetq_lane_f32(0, V, 3);
#elif defined(RAY_SSE_INTRINSICS)
			// Reads an extra float which is zero'd
			__m128 V = _mm_load_ps(&pSource->x);
			return _mm_and_ps(V, Constants::Mask3);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadSInt3
		(
			const Int3* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = 0.f;
			return V;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x2_t x = vld1_s32(reinterpret_cast<const int32_t*>(pSource));
			int32x2_t zero = vdup_n_s32(0);
			int32x2_t y = vld1_lane_s32(reinterpret_cast<const int32_t*>(pSource) + 2, zero, 0);
			int32x4_t v = vcombine_s32(x, y);
			return vcvtq_f32_s32(v);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(&pSource->z));
			__m128 xy = _mm_unpacklo_ps(x, y);
			__m128 V = _mm_movelh_ps(xy, z);
			return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadUInt3
		(
			const UInt3* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = 0.f;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t x = vld1_u32(reinterpret_cast<const uint32_t*>(pSource));
			uint32x2_t zero = vdup_n_u32(0);
			uint32x2_t y = vld1_lane_u32(reinterpret_cast<const uint32_t*>(pSource) + 2, zero, 0);
			uint32x4_t v = vcombine_u32(x, y);
			return vcvtq_f32_u32(v);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 x = _mm_load_ss(reinterpret_cast<const float*>(&pSource->x));
			__m128 y = _mm_load_ss(reinterpret_cast<const float*>(&pSource->y));
			__m128 z = _mm_load_ss(reinterpret_cast<const float*>(&pSource->z));
			__m128 xy = _mm_unpacklo_ps(x, y);
			__m128 V = _mm_movelh_ps(xy, z);
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			VectorType vMask = _mm_and_ps(V, Constants::NegativeZero);
			// Force all values positive
			VectorType vResult = _mm_xor_ps(V, vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Constants::FixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt4
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);

#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = pSource[3];
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_u32(pSource);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadInt4A
		(
			const uint32_t* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_u32[0] = pSource[0];
			V.vector4_u32[1] = pSource[1];
			V.vector4_u32[2] = pSource[2];
			V.vector4_u32[3] = pSource[3];
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_u32_ex(pSource, 128);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_load_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat4
		(
			const Float4* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = pSource->w;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_f32(reinterpret_cast<const float*>(pSource));
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_loadu_ps(&pSource->x);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadFloat4A
		(
			const Float4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = pSource->x;
			V.vector4_f32[1] = pSource->y;
			V.vector4_f32[2] = pSource->z;
			V.vector4_f32[3] = pSource->w;
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_f32_ex(reinterpret_cast<const float*>(pSource), 128);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_load_ps(&pSource->x);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadSInt4
		(
			const Int4* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = static_cast<float>(pSource->w);
			return V;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t v = vld1q_s32(reinterpret_cast<const int32_t*>(pSource));
			return vcvtq_f32_s32(v);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			return _mm_cvtepi32_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV LoadUInt4
		(
			const UInt4* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)
			VectorType V;
			V.vector4_f32[0] = static_cast<float>(pSource->x);
			V.vector4_f32[1] = static_cast<float>(pSource->y);
			V.vector4_f32[2] = static_cast<float>(pSource->z);
			V.vector4_f32[3] = static_cast<float>(pSource->w);
			return V;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t v = vld1q_u32(reinterpret_cast<const uint32_t*>(pSource));
			return vcvtq_f32_u32(v);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			VectorType vMask = _mm_and_ps(_mm_castsi128_ps(V), Constants::NegativeZero);
			// Force all values positive
			VectorType vResult = _mm_xor_ps(_mm_castsi128_ps(V), vMask);
			// Convert to floats
			vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
			// Convert 0x80000000 -> 0xFFFFFFFF
			__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
			// For only the ones that are too big, add the fixup
			vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Constants::FixUnsigned);
			vResult = _mm_add_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat3x3
		(
			const Float3X3* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;
			M.r[3].vector4_f32[0] = 0.0f;
			M.r[3].vector4_f32[1] = 0.0f;
			M.r[3].vector4_f32[2] = 0.0f;
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t v0 = vld1q_f32(&pSource->m[0][0]);
			float32x4_t v1 = vld1q_f32(&pSource->m[1][1]);
			float32x2_t v2 = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&pSource->m[2][2])));
			float32x4_t T = vextq_f32(v0, v1, 3);

			Matrix M;
			M.r[0] = vandq_u32(v0, Constants::Mask3);
			M.r[1] = vandq_u32(T, Constants::Mask3);
			M.r[2] = vcombine_f32(vget_high_f32(v1), v2);
			M.r[3] = Constants::IdentityR3;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 Z = _mm_setzero_ps();

			__m128 V1 = _mm_loadu_ps(&pSource->m[0][0]);
			__m128 V2 = _mm_loadu_ps(&pSource->m[1][1]);
			__m128 V3 = _mm_load_ss(&pSource->m[2][2]);

			__m128 T1 = _mm_unpackhi_ps(V1, Z);
			__m128 T2 = _mm_unpacklo_ps(V2, Z);
			__m128 T3 = _mm_shuffle_ps(V3, T2, _MM_SHUFFLE(0, 1, 0, 0));
			__m128 T4 = _mm_movehl_ps(T2, T3);
			__m128 T5 = _mm_movehl_ps(Z, T1);

			Matrix M;
			M.r[0] = _mm_movelh_ps(V1, T1);
			M.r[1] = _mm_add_ps(T4, T5);
			M.r[2] = _mm_shuffle_ps(V2, V3, _MM_SHUFFLE(1, 0, 3, 2));
			M.r[3] = Constants::IdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat4x3
		(
			const Float4X3* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t v0 = vld1q_f32(&pSource->m[0][0]);
			float32x4_t v1 = vld1q_f32(&pSource->m[1][1]);
			float32x4_t v2 = vld1q_f32(&pSource->m[2][2]);

			float32x4_t T1 = vextq_f32(v0, v1, 3);
			float32x4_t T2 = vcombine_f32(vget_high_f32(v1), vget_low_f32(v2));
			float32x4_t T3 = vextq_f32(v2, v2, 1);

			Matrix M;
			M.r[0] = vandq_u32(v0, Constants::Mask3);
			M.r[1] = vandq_u32(T1, Constants::Mask3);
			M.r[2] = vandq_u32(T2, Constants::Mask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			// Use unaligned load instructions to 
			// load the 12 floats
			// vTemp1 = x1,y1,z1,x2
			VectorType vTemp1 = _mm_loadu_ps(&pSource->m[0][0]);
			// vTemp2 = y2,z2,x3,y3
			VectorType vTemp2 = _mm_loadu_ps(&pSource->m[1][1]);
			// vTemp4 = z3,x4,y4,z4
			VectorType vTemp4 = _mm_loadu_ps(&pSource->m[2][2]);
			// vTemp3 = x3,y3,z3,z3
			VectorType vTemp3 = _mm_shuffle_ps(vTemp2, vTemp4, _MM_SHUFFLE(0, 0, 3, 2));
			// vTemp2 = y2,z2,x2,x2
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(3, 3, 1, 0));
			// vTemp2 = x2,y2,z2,z2
			vTemp2 = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 1, 0, 2));
			// vTemp1 = x1,y1,z1,0
			vTemp1 = _mm_and_ps(vTemp1, Constants::Mask3);
			// vTemp2 = x2,y2,z2,0
			vTemp2 = _mm_and_ps(vTemp2, Constants::Mask3);
			// vTemp3 = x3,y3,z3,0
			vTemp3 = _mm_and_ps(vTemp3, Constants::Mask3);
			// vTemp4i = x4,y4,z4,0
			__m128i vTemp4i = _mm_srli_si128(_mm_castps_si128(vTemp4), 32 / 8);
			// vTemp4i = x4,y4,z4,1.0f
			vTemp4i = _mm_or_si128(vTemp4i, Constants::IdentityR3);
			Matrix M(vTemp1,
				vTemp2,
				vTemp3,
				_mm_castsi128_ps(vTemp4i));
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat4x3A
		(
			const Float4X3A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t v0 = vld1q_f32_ex(&pSource->m[0][0], 128);
			float32x4_t v1 = vld1q_f32_ex(&pSource->m[1][1], 128);
			float32x4_t v2 = vld1q_f32_ex(&pSource->m[2][2], 128);

			float32x4_t T1 = vextq_f32(v0, v1, 3);
			float32x4_t T2 = vcombine_f32(vget_high_f32(v1), vget_low_f32(v2));
			float32x4_t T3 = vextq_f32(v2, v2, 1);

			Matrix M;
			M.r[0] = vandq_u32(v0, Constants::Mask3);
			M.r[1] = vandq_u32(T1, Constants::Mask3);
			M.r[2] = vandq_u32(T2, Constants::Mask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			// Use aligned load instructions to 
			// load the 12 floats
			// vTemp1 = x1,y1,z1,x2
			VectorType vTemp1 = _mm_load_ps(&pSource->m[0][0]);
			// vTemp2 = y2,z2,x3,y3
			VectorType vTemp2 = _mm_load_ps(&pSource->m[1][1]);
			// vTemp4 = z3,x4,y4,z4
			VectorType vTemp4 = _mm_load_ps(&pSource->m[2][2]);
			// vTemp3 = x3,y3,z3,z3
			VectorType vTemp3 = _mm_shuffle_ps(vTemp2, vTemp4, _MM_SHUFFLE(0, 0, 3, 2));
			// vTemp2 = y2,z2,x2,x2
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(3, 3, 1, 0));
			// vTemp2 = x2,y2,z2,z2
			vTemp2 = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 1, 0, 2));
			// vTemp1 = x1,y1,z1,0
			vTemp1 = _mm_and_ps(vTemp1, Constants::Mask3);
			// vTemp2 = x2,y2,z2,0
			vTemp2 = _mm_and_ps(vTemp2, Constants::Mask3);
			// vTemp3 = x3,y3,z3,0
			vTemp3 = _mm_and_ps(vTemp3, Constants::Mask3);
			// vTemp4i = x4,y4,z4,0
			__m128i vTemp4i = _mm_srli_si128(_mm_castps_si128(vTemp4), 32 / 8);
			// vTemp4i = x4,y4,z4,1.0f
			vTemp4i = _mm_or_si128(vTemp4i, Constants::IdentityR3);
			Matrix M(vTemp1,
				vTemp2,
				vTemp3,
				_mm_castsi128_ps(vTemp4i));
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat3x4
		(
			const Float3X4* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[1][0];
			M.r[0].vector4_f32[2] = pSource->m[2][0];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[0][1];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[2][1];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[0][2];
			M.r[2].vector4_f32[1] = pSource->m[1][2];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[0][3];
			M.r[3].vector4_f32[1] = pSource->m[1][3];
			M.r[3].vector4_f32[2] = pSource->m[2][3];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2x4_t vTemp0 = vld4_f32(&pSource->_11);
			float32x4_t vTemp1 = vld1q_f32(&pSource->_31);

			float32x2_t l = vget_low_f32(vTemp1);
			float32x4_t T0 = vcombine_f32(vTemp0.val[0], l);
			float32x2_t rl = vrev64_f32(l);
			float32x4_t T1 = vcombine_f32(vTemp0.val[1], rl);

			float32x2_t h = vget_high_f32(vTemp1);
			float32x4_t T2 = vcombine_f32(vTemp0.val[2], h);
			float32x2_t rh = vrev64_f32(h);
			float32x4_t T3 = vcombine_f32(vTemp0.val[3], rh);

			Matrix M = {};
			M.r[0] = vandq_u32(T0, Constants::Mask3);
			M.r[1] = vandq_u32(T1, Constants::Mask3);
			M.r[2] = vandq_u32(T2, Constants::Mask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_loadu_ps(&pSource->_11);
			M.r[1] = _mm_loadu_ps(&pSource->_21);
			M.r[2] = _mm_loadu_ps(&pSource->_31);
			M.r[3] = Constants::IdentityR3;

			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat3x4A
		(
			const Float3X4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[1][0];
			M.r[0].vector4_f32[2] = pSource->m[2][0];
			M.r[0].vector4_f32[3] = 0.0f;

			M.r[1].vector4_f32[0] = pSource->m[0][1];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[2][1];
			M.r[1].vector4_f32[3] = 0.0f;

			M.r[2].vector4_f32[0] = pSource->m[0][2];
			M.r[2].vector4_f32[1] = pSource->m[1][2];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = 0.0f;

			M.r[3].vector4_f32[0] = pSource->m[0][3];
			M.r[3].vector4_f32[1] = pSource->m[1][3];
			M.r[3].vector4_f32[2] = pSource->m[2][3];
			M.r[3].vector4_f32[3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2x4_t vTemp0 = vld4_f32_ex(&pSource->_11, 128);
			float32x4_t vTemp1 = vld1q_f32_ex(&pSource->_31, 128);

			float32x2_t l = vget_low_f32(vTemp1);
			float32x4_t T0 = vcombine_f32(vTemp0.val[0], l);
			float32x2_t rl = vrev64_f32(l);
			float32x4_t T1 = vcombine_f32(vTemp0.val[1], rl);

			float32x2_t h = vget_high_f32(vTemp1);
			float32x4_t T2 = vcombine_f32(vTemp0.val[2], h);
			float32x2_t rh = vrev64_f32(h);
			float32x4_t T3 = vcombine_f32(vTemp0.val[3], rh);

			Matrix M = {};
			M.r[0] = vandq_u32(T0, Constants::Mask3);
			M.r[1] = vandq_u32(T1, Constants::Mask3);
			M.r[2] = vandq_u32(T2, Constants::Mask3);
			M.r[3] = vsetq_lane_f32(1.f, T3, 3);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_load_ps(&pSource->_11);
			M.r[1] = _mm_load_ps(&pSource->_21);
			M.r[2] = _mm_load_ps(&pSource->_31);
			M.r[3] = Constants::IdentityR3;

			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat4x4
		(
			const Float4X4* pSource
		)
		{
			assert(pSource);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = pSource->m[0][3];

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = pSource->m[1][3];

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = pSource->m[2][3];

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = pSource->m[3][3];
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			Matrix M;
			M.r[0] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_11));
			M.r[1] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_21));
			M.r[2] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_31));
			M.r[3] = vld1q_f32(reinterpret_cast<const float*>(&pSource->_41));
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_loadu_ps(&pSource->_11);
			M.r[1] = _mm_loadu_ps(&pSource->_21);
			M.r[2] = _mm_loadu_ps(&pSource->_31);
			M.r[3] = _mm_loadu_ps(&pSource->_41);
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV LoadFloat4x4A
		(
			const Float4X4A* pSource
		)
		{
			assert(pSource);
			assert((reinterpret_cast<uintptr_t>(pSource) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.r[0].vector4_f32[0] = pSource->m[0][0];
			M.r[0].vector4_f32[1] = pSource->m[0][1];
			M.r[0].vector4_f32[2] = pSource->m[0][2];
			M.r[0].vector4_f32[3] = pSource->m[0][3];

			M.r[1].vector4_f32[0] = pSource->m[1][0];
			M.r[1].vector4_f32[1] = pSource->m[1][1];
			M.r[1].vector4_f32[2] = pSource->m[1][2];
			M.r[1].vector4_f32[3] = pSource->m[1][3];

			M.r[2].vector4_f32[0] = pSource->m[2][0];
			M.r[2].vector4_f32[1] = pSource->m[2][1];
			M.r[2].vector4_f32[2] = pSource->m[2][2];
			M.r[2].vector4_f32[3] = pSource->m[2][3];

			M.r[3].vector4_f32[0] = pSource->m[3][0];
			M.r[3].vector4_f32[1] = pSource->m[3][1];
			M.r[3].vector4_f32[2] = pSource->m[3][2];
			M.r[3].vector4_f32[3] = pSource->m[3][3];
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			Matrix M;
			M.r[0] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_11), 128);
			M.r[1] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_21), 128);
			M.r[2] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_31), 128);
			M.r[3] = vld1q_f32_ex(reinterpret_cast<const float*>(&pSource->_41), 128);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_load_ps(&pSource->_11);
			M.r[1] = _mm_load_ps(&pSource->_21);
			M.r[2] = _mm_load_ps(&pSource->_31);
			M.r[3] = _mm_load_ps(&pSource->_41);
			return M;
#endif
		}

		/****************************************************************************
		 *
		 * VectorType and matrix store operations
		 *
		 ****************************************************************************/

		inline void RAYMATH_CALLCONV StoreInt
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			*pDestination = VectorGetIntX(V);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_u32(pDestination, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ss(reinterpret_cast<float*>(pDestination), V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat
		(
			float* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			*pDestination = Vector(V).GetX();
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_f32(pDestination, V, 0);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ss(pDestination, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt2
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32(pDestination, VL);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[0]), V);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[1]), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt2A
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32_ex(pDestination, VL, 64);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat2
		(
			Float2* pDestination,
			FVectorType  V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32(reinterpret_cast<float*>(pDestination), VL);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(&pDestination->x, V);
			_mm_store_ss(&pDestination->y, T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat2A
		(
			Float2A* pDestination,
			FVectorType     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32_ex(reinterpret_cast<float*>(pDestination), VL, 64);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreSInt2
		(
			Int2* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x2_t v = vget_low_s32(V);
			v = vcvt_s32_f32(v);
			vst1_s32(reinterpret_cast<int32_t*>(pDestination), v);
#elif defined(RAY_SSE_INTRINSICS)
			// In case of positive overflow, detect it
			VectorType vOverflow = _mm_cmpgt_ps(V, Constants::MaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			VectorType vResult = _mm_and_ps(vOverflow, Constants::AbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			// Write two ints
			VectorType T = RAYMATH_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vOverflow);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreUInt2
		(
			UInt2* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t v = vget_low_f32(V);
			uint32x2_t iv = vcvt_u32_f32(v);
			vst1_u32(reinterpret_cast<uint32_t*>(pDestination), iv);
#elif defined(RAY_SSE_INTRINSICS)
			// Clamp to >=0
			VectorType vResult = _mm_max_ps(V, Constants::Zero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			VectorType vOverflow = _mm_cmpgt_ps(vResult, Constants::MaxUInt);
			VectorType vValue = Constants::UnsignedFix;
			// Too large for a signed integer?
			VectorType vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, Constants::NegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			// Write two uints
			VectorType T = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vResult);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt3
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32(pDestination, VL);
			vst1q_lane_u32(pDestination + 2, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T1 = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType T2 = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(pDestination), V);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[1]), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[2]), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt3A
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t VL = vget_low_u32(V);
			vst1_u32_ex(pDestination, VL, 64);
			vst1q_lane_u32(pDestination + 2, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination[2]), T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat3
		(
			Float3* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32(reinterpret_cast<float*>(pDestination), VL);
			vst1q_lane_f32(reinterpret_cast<float*>(pDestination) + 2, V, 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T1 = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType T2 = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(&pDestination->x, V);
			_mm_store_ss(&pDestination->y, T1);
			_mm_store_ss(&pDestination->z, T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat3A
		(
			Float3A* pDestination,
			FVectorType     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			vst1_f32_ex(reinterpret_cast<float*>(pDestination), VL, 64);
			vst1q_lane_f32(reinterpret_cast<float*>(pDestination) + 2, V, 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType T = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_storel_epi64(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
			_mm_store_ss(&pDestination->z, T);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreSInt3
		(
			Int3* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<int32_t>(V.vector4_f32[2]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t v = vcvtq_s32_f32(V);
			int32x2_t vL = vget_low_s32(v);
			vst1_s32(reinterpret_cast<int32_t*>(pDestination), vL);
			vst1q_lane_s32(reinterpret_cast<int32_t*>(pDestination) + 2, v, 2);
#elif defined(RAY_SSE_INTRINSICS)
			// In case of positive overflow, detect it
			VectorType vOverflow = _mm_cmpgt_ps(V, Constants::MaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			VectorType vResult = _mm_and_ps(vOverflow, Constants::AbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			// Write 3 uints
			VectorType T1 = RAYMATH_PERMUTE_PS(vOverflow, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType T2 = RAYMATH_PERMUTE_PS(vOverflow, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vOverflow);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->z), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreUInt3
		(
			UInt3* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<uint32_t>(V.vector4_f32[2]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t v = vcvtq_u32_f32(V);
			uint32x2_t vL = vget_low_u32(v);
			vst1_u32(reinterpret_cast<uint32_t*>(pDestination), vL);
			vst1q_lane_u32(reinterpret_cast<uint32_t*>(pDestination) + 2, v, 2);
#elif defined(RAY_SSE_INTRINSICS)
			// Clamp to >=0
			VectorType vResult = _mm_max_ps(V, Constants::Zero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			VectorType vOverflow = _mm_cmpgt_ps(vResult, Constants::MaxUInt);
			VectorType vValue = Constants::UnsignedFix;
			// Too large for a signed integer?
			VectorType vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, Constants::NegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			// Write 3 uints
			VectorType T1 = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType T2 = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->x), vResult);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->y), T1);
			_mm_store_ss(reinterpret_cast<float*>(&pDestination->z), T2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt4
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
			pDestination[3] = V.vector4_u32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_u32(pDestination, V);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreInt4A
		(
			uint32_t* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination[0] = V.vector4_u32[0];
			pDestination[1] = V.vector4_u32[1];
			pDestination[2] = V.vector4_u32[2];
			pDestination[3] = V.vector4_u32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_u32_ex(pDestination, V, 128);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4
		(
			Float4* pDestination,
			FVectorType  V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
			pDestination->w = V.vector4_f32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_f32(reinterpret_cast<float*>(pDestination), V);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_storeu_ps(&pDestination->x, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4A
		(
			Float4A* pDestination,
			FVectorType     V
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = V.vector4_f32[0];
			pDestination->y = V.vector4_f32[1];
			pDestination->z = V.vector4_f32[2];
			pDestination->w = V.vector4_f32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_f32_ex(reinterpret_cast<float*>(pDestination), V, 128);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ps(&pDestination->x, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreSInt4
		(
			Int4* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<int32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<int32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<int32_t>(V.vector4_f32[2]);
			pDestination->w = static_cast<int32_t>(V.vector4_f32[3]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t v = vcvtq_s32_f32(V);
			vst1q_s32(reinterpret_cast<int32_t*>(pDestination), v);
#elif defined(RAY_SSE_INTRINSICS)
			// In case of positive overflow, detect it
			VectorType vOverflow = _mm_cmpgt_ps(V, Constants::MaxInt);
			// Float to int conversion
			__m128i vResulti = _mm_cvttps_epi32(V);
			// If there was positive overflow, set to 0x7FFFFFFF
			VectorType vResult = _mm_and_ps(vOverflow, Constants::AbsMask);
			vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
			vOverflow = _mm_or_ps(vOverflow, vResult);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vOverflow));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreUInt4
		(
			UInt4* pDestination,
			FVectorType V
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)
			pDestination->x = static_cast<uint32_t>(V.vector4_f32[0]);
			pDestination->y = static_cast<uint32_t>(V.vector4_f32[1]);
			pDestination->z = static_cast<uint32_t>(V.vector4_f32[2]);
			pDestination->w = static_cast<uint32_t>(V.vector4_f32[3]);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t v = vcvtq_u32_f32(V);
			vst1q_u32(reinterpret_cast<uint32_t*>(pDestination), v);
#elif defined(RAY_SSE_INTRINSICS)
			// Clamp to >=0
			VectorType vResult = _mm_max_ps(V, Constants::Zero);
			// Any numbers that are too big, set to 0xFFFFFFFFU
			VectorType vOverflow = _mm_cmpgt_ps(vResult, Constants::MaxUInt);
			VectorType vValue = Constants::UnsignedFix;
			// Too large for a signed integer?
			VectorType vMask = _mm_cmpge_ps(vResult, vValue);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			vValue = _mm_and_ps(vValue, vMask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			vResult = _mm_sub_ps(vResult, vValue);
			__m128i vResulti = _mm_cvttps_epi32(vResult);
			// Convert from signed to unsigned pnly if greater than 0x80000000
			vMask = _mm_and_ps(vMask, Constants::NegativeZero);
			vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
			// On those that are too large, set to 0xFFFFFFFF
			vResult = _mm_or_ps(vResult, vOverflow);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vResult));
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat3x3
		(
			Float3X3* pDestination,
			FMatrix	M
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(Constants::Mask3, M.r[0], T1);
			vst1q_f32(&pDestination->m[0][0], T2);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32(&pDestination->m[1][1], T2);

			vst1q_lane_f32(&pDestination->m[2][2], M.r[2], 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp1 = M.r[0];
			VectorType vTemp2 = M.r[1];
			VectorType vTemp3 = M.r[2];
			VectorType vWork = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(0, 0, 2, 2));
			vTemp1 = _mm_shuffle_ps(vTemp1, vWork, _MM_SHUFFLE(2, 0, 1, 0));
			_mm_storeu_ps(&pDestination->m[0][0], vTemp1);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			_mm_storeu_ps(&pDestination->m[1][1], vTemp2);
			vTemp3 = RAYMATH_PERMUTE_PS(vTemp3, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4x3
		(
			Float4X3* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(Constants::Mask3, M.r[0], T1);
			vst1q_f32(&pDestination->m[0][0], T2);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32(&pDestination->m[1][1], T2);

			T1 = vdupq_lane_f32(vget_high_f32(M.r[2]), 0);
			T2 = vextq_f32(T1, M.r[3], 3);
			vst1q_f32(&pDestination->m[2][2], T2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp1 = M.r[0];
			VectorType vTemp2 = M.r[1];
			VectorType vTemp3 = M.r[2];
			VectorType vTemp4 = M.r[3];
			VectorType vTemp2x = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp1, _MM_SHUFFLE(2, 2, 0, 0));
			vTemp1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(0, 2, 1, 0));
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(0, 0, 2, 2));
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 1, 2, 0));
			_mm_storeu_ps(&pDestination->m[0][0], vTemp1);
			_mm_storeu_ps(&pDestination->m[1][1], vTemp2x);
			_mm_storeu_ps(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4x3A
		(
			Float4X3A* pDestination,
			FMatrix		M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t T1 = vextq_f32(M.r[0], M.r[1], 1);
			float32x4_t T2 = vbslq_f32(Constants::Mask3, M.r[0], T1);
			vst1q_f32_ex(&pDestination->m[0][0], T2, 128);

			T1 = vextq_f32(M.r[1], M.r[1], 1);
			T2 = vcombine_f32(vget_low_f32(T1), vget_low_f32(M.r[2]));
			vst1q_f32_ex(&pDestination->m[1][1], T2, 128);

			T1 = vdupq_lane_f32(vget_high_f32(M.r[2]), 0);
			T2 = vextq_f32(T1, M.r[3], 3);
			vst1q_f32_ex(&pDestination->m[2][2], T2, 128);
#elif defined(RAY_SSE_INTRINSICS)
			// x1,y1,z1,w1
			VectorType vTemp1 = M.r[0];
			// x2,y2,z2,w2
			VectorType vTemp2 = M.r[1];
			// x3,y3,z3,w3
			VectorType vTemp3 = M.r[2];
			// x4,y4,z4,w4
			VectorType vTemp4 = M.r[3];
			// z1,z1,x2,y2
			VectorType vTemp = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(1, 0, 2, 2));
			// y2,z2,x3,y3 (Final)
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp3, _MM_SHUFFLE(1, 0, 2, 1));
			// x1,y1,z1,x2 (Final)
			vTemp1 = _mm_shuffle_ps(vTemp1, vTemp, _MM_SHUFFLE(2, 0, 1, 0));
			// z3,z3,x4,x4
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(0, 0, 2, 2));
			// z3,x4,y4,z4 (Final)
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 1, 2, 0));
			// Store in 3 operations
			_mm_store_ps(&pDestination->m[0][0], vTemp1);
			_mm_store_ps(&pDestination->m[1][1], vTemp2);
			_mm_store_ps(&pDestination->m[2][2], vTemp3);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat3x4
		(
			Float3X4* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[1].vector4_f32[0];
			pDestination->m[0][2] = M.r[2].vector4_f32[0];
			pDestination->m[0][3] = M.r[3].vector4_f32[0];

			pDestination->m[1][0] = M.r[0].vector4_f32[1];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[2].vector4_f32[1];
			pDestination->m[1][3] = M.r[3].vector4_f32[1];

			pDestination->m[2][0] = M.r[0].vector4_f32[2];
			pDestination->m[2][1] = M.r[1].vector4_f32[2];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[3].vector4_f32[2];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
			float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			vst1q_f32(&pDestination->m[0][0], T0.val[0]);
			vst1q_f32(&pDestination->m[1][0], T0.val[1]);
			vst1q_f32(&pDestination->m[2][0], T1.val[0]);
#elif defined(RAY_SSE_INTRINSICS)
			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

			// x.x,y.x,z.x,w.x
			VectorType r0 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			VectorType r1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			VectorType r2 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));

			_mm_storeu_ps(&pDestination->m[0][0], r0);
			_mm_storeu_ps(&pDestination->m[1][0], r1);
			_mm_storeu_ps(&pDestination->m[2][0], r2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat3x4A
		(
			Float3X4A* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[1].vector4_f32[0];
			pDestination->m[0][2] = M.r[2].vector4_f32[0];
			pDestination->m[0][3] = M.r[3].vector4_f32[0];

			pDestination->m[1][0] = M.r[0].vector4_f32[1];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[2].vector4_f32[1];
			pDestination->m[1][3] = M.r[3].vector4_f32[1];

			pDestination->m[2][0] = M.r[0].vector4_f32[2];
			pDestination->m[2][1] = M.r[1].vector4_f32[2];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[3].vector4_f32[2];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
			float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			vst1q_f32_ex(&pDestination->m[0][0], T0.val[0], 128);
			vst1q_f32_ex(&pDestination->m[1][0], T0.val[1], 128);
			vst1q_f32_ex(&pDestination->m[2][0], T1.val[0], 128);
#elif defined(RAY_SSE_INTRINSICS)
			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

			// x.x,y.x,z.x,w.x
			VectorType r0 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			VectorType r1 = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			VectorType r2 = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));

			_mm_store_ps(&pDestination->m[0][0], r0);
			_mm_store_ps(&pDestination->m[1][0], r1);
			_mm_store_ps(&pDestination->m[2][0], r2);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4x4
		(
			Float4X4* pDestination,
			FMatrix M
		)
		{
			assert(pDestination);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];
			pDestination->m[0][3] = M.r[0].vector4_f32[3];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];
			pDestination->m[1][3] = M.r[1].vector4_f32[3];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[2].vector4_f32[3];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];
			pDestination->m[3][3] = M.r[3].vector4_f32[3];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_11), M.r[0]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_21), M.r[1]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_31), M.r[2]);
			vst1q_f32(reinterpret_cast<float*>(&pDestination->_41), M.r[3]);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_storeu_ps(&pDestination->_11, M.r[0]);
			_mm_storeu_ps(&pDestination->_21, M.r[1]);
			_mm_storeu_ps(&pDestination->_31, M.r[2]);
			_mm_storeu_ps(&pDestination->_41, M.r[3]);
#endif
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV StoreFloat4x4A
		(
			Float4X4A* pDestination,
			FMatrix		M
		)
		{
			assert(pDestination);
			assert((reinterpret_cast<uintptr_t>(pDestination) & 0xF) == 0);
#if defined(RAY_NO_INTRINSICS)

			pDestination->m[0][0] = M.r[0].vector4_f32[0];
			pDestination->m[0][1] = M.r[0].vector4_f32[1];
			pDestination->m[0][2] = M.r[0].vector4_f32[2];
			pDestination->m[0][3] = M.r[0].vector4_f32[3];

			pDestination->m[1][0] = M.r[1].vector4_f32[0];
			pDestination->m[1][1] = M.r[1].vector4_f32[1];
			pDestination->m[1][2] = M.r[1].vector4_f32[2];
			pDestination->m[1][3] = M.r[1].vector4_f32[3];

			pDestination->m[2][0] = M.r[2].vector4_f32[0];
			pDestination->m[2][1] = M.r[2].vector4_f32[1];
			pDestination->m[2][2] = M.r[2].vector4_f32[2];
			pDestination->m[2][3] = M.r[2].vector4_f32[3];

			pDestination->m[3][0] = M.r[3].vector4_f32[0];
			pDestination->m[3][1] = M.r[3].vector4_f32[1];
			pDestination->m[3][2] = M.r[3].vector4_f32[2];
			pDestination->m[3][3] = M.r[3].vector4_f32[3];

#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_11), M.r[0], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_21), M.r[1], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_31), M.r[2], 128);
			vst1q_f32_ex(reinterpret_cast<float*>(&pDestination->_41), M.r[3], 128);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ps(&pDestination->_11, M.r[0]);
			_mm_store_ps(&pDestination->_21, M.r[1]);
			_mm_store_ps(&pDestination->_31, M.r[2]);
			_mm_store_ps(&pDestination->_41, M.r[3]);
#endif
		}




		//-------------------------------------------------------------------------------------
	// DirectXMathVector.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------


#if defined(RAY_NO_INTRINSICS)
#define RAYMATH_ISNAN(x)  ((*(const uint32_t*)&(x) & 0x7F800000) == 0x7F800000 && (*(const uint32_t*)&(x) & 0x7FFFFF) != 0)
#define RAYMATH_ISINF(x)  ((*(const uint32_t*)&(x) & 0x7FFFFFFF) == 0x7F800000)
#endif

#if defined(RAY_SSE_INTRINSICS)

#define RAYMATH3_UNPACK3_INTO4(l1,l2,l3) \
    VectorType V3 = _mm_shuffle_ps(l2,l3,_MM_SHUFFLE(0,0,3,2));\
    VectorType V2 = _mm_shuffle_ps(l2,l1,_MM_SHUFFLE(3,3,1,0));\
    V2 = RAYMATH_PERMUTE_PS(V2,_MM_SHUFFLE(1,1,0,2));\
    VectorType V4 = _mm_castsi128_ps( _mm_srli_si128(_mm_castps_si128(L3),32/8) );

#define RAYMATH3_PACK4_INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2,V3,_MM_SHUFFLE(1,0,2,1));\
    V2 = _mm_shuffle_ps(V2,V1,_MM_SHUFFLE(2,2,0,0));\
    V1 = _mm_shuffle_ps(V1,V2,_MM_SHUFFLE(0,2,1,0));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(0,0,2,2));\
    V3 = _mm_shuffle_ps(V3,V4,_MM_SHUFFLE(2,1,2,0));\

#endif

/****************************************************************************
 *
 * General VectorType
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
 // Assignment operations
 //------------------------------------------------------------------------------

 //------------------------------------------------------------------------------
 // Return a vector with all elements equaling zero
		inline Vector RAYMATH_CALLCONV Vector::Zero()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_f32(0);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_setzero_ps();
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with four floating point values
		inline Vector RAYMATH_CALLCONV Vector::Set
		(
			float x,
			float y,
			float z,
			float w
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { { x, y, z, w } } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t V0 = vcreate_f32(
				static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&x))
				| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&y)) << 32));
			float32x2_t V1 = vcreate_f32(
				static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&z))
				| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&w)) << 32));
			return vcombine_f32(V0, V1);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_set_ps(w, z, y, x);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with four integer values
		inline Vector RAYMATH_CALLCONV Vector::SetInt
		(
			uint32_t x,
			uint32_t y,
			uint32_t z,
			uint32_t w
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult = { { { x, y, z, w } } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t V0 = vcreate_u32(static_cast<uint64_t>(x) | (static_cast<uint64_t>(y) << 32));
			uint32x2_t V1 = vcreate_u32(static_cast<uint64_t>(z) | (static_cast<uint64_t>(w) << 32));
			return vcombine_u32(V0, V1);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated floating point value
		inline Vector RAYMATH_CALLCONV Vector::Replicate
		(
			float Value
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = Value;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_f32(Value);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_set_ps1(Value);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated floating point value passed by pointer

		inline Vector RAYMATH_CALLCONV Vector::ReplicatePtr
		(
			const float* pValue
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float Value = pValue[0];
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = Value;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_dup_f32(pValue);
#elif defined(RAY_AVC_INTRINSICS)
			return _mm_broadcast_ss(pValue);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_load_ps1(pValue);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated integer value
		inline Vector RAYMATH_CALLCONV Vector::ReplicateInt
		(
			uint32_t Value
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = Value;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(Value);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_set1_epi32(static_cast<int>(Value));
			return _mm_castsi128_ps(vTemp);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with a replicated integer value passed by pointer

		inline Vector RAYMATH_CALLCONV Vector::ReplicateIntPtr
		(
			const uint32_t* pValue
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t Value = pValue[0];
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = Value;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_dup_u32(pValue);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_load_ps1(reinterpret_cast<const float*>(pValue));
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with all bits set (true mask)
		inline Vector RAYMATH_CALLCONV VectorTrueInt()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_s32(-1);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_set1_epi32(-1);
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Initialize a vector with all bits clear (false mask)
		inline Vector RAYMATH_CALLCONV Vector::FalseInt()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
			return vResult;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(0);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_setzero_ps();
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the x component of the vector
		inline Vector RAYMATH_CALLCONV Vector::SplatX
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[0];
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_lane_f32(vget_low_f32(V), 0);
#elif defined(RAY_AVX2_INTRINSICS)
			return _mm_broadcastss_ps(V);
#elif defined(RAY_SSE_INTRINSICS)
			return RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the y component of the vector
		inline Vector RAYMATH_CALLCONV Vector::SplatY
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[1];
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_lane_f32(vget_low_f32(V), 1);
#elif defined(RAY_SSE_INTRINSICS)
			return RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the z component of the vector
		inline Vector RAYMATH_CALLCONV Vector::SplatZ
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[2];
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_lane_f32(vget_high_f32(V), 0);
#elif defined(RAY_SSE_INTRINSICS)
			return RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
#endif
		}

		//------------------------------------------------------------------------------
		// Replicate the w component of the vector
		inline Vector RAYMATH_CALLCONV Vector::SplatW
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = V.vector4_f32[3];
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_lane_f32(vget_high_f32(V), 1);
#elif defined(RAY_SSE_INTRINSICS)
			return RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of 1.0f,1.0f,1.0f,1.0f
		inline Vector RAYMATH_CALLCONV Vector::SplatOne()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = 1.0f;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_f32(1.0f);
#elif defined(RAY_SSE_INTRINSICS)
			return Vector(Constants::One);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of INF,INF,INF,INF
		inline Vector RAYMATH_CALLCONV Vector::SplatInfinity()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x7F800000;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(0x7F800000);
#elif defined(RAY_SSE_INTRINSICS)
			return Vector(Constants::Infinity);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of Q_NAN,Q_NAN,Q_NAN,Q_NAN
		inline Vector RAYMATH_CALLCONV Vector::SplatQNaN()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x7FC00000;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(0x7FC00000);
#elif defined(RAY_SSE_INTRINSICS)
			return Vector(Constants::QNaN);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of 1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f
		inline Vector RAYMATH_CALLCONV Vector::SplatEpsilon()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x34000000;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(0x34000000);
#elif defined(RAY_SSE_INTRINSICS)
			return Vector(Constants::Epsilon);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a vector of -0.0f (0x80000000),-0.0f,-0.0f,-0.0f
		inline Vector RAYMATH_CALLCONV Vector::SplatSignMask()
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 vResult;
			vResult.u[0] =
				vResult.u[1] =
				vResult.u[2] =
				vResult.u[3] = 0x80000000U;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vdupq_n_u32(0x80000000U);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_set1_epi32(static_cast<int>(0x80000000));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Return a floating point value via an index. This is not a recommended
		// function to use due to performance loss.
		inline float RAYMATH_CALLCONV Vector::GetByIndex(size_t i) const
		{
			assert(i < 4);
#if defined(RAY_NO_INTRINSICS)
			return v.vector4_f32[i];
#else
			VectorF32 U;
			U.v = *this;
			return U.f[i];
#endif
		}

		//------------------------------------------------------------------------------
		// Return the X component in an FPU register. 
		inline float RAYMATH_CALLCONV Vector::GetX() const
		{
#if defined(RAY_NO_INTRINSICS)
			return v.vector4_f32[0];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_f32(v, 0);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cvtss_f32(v);
#endif
		}

		// Return the Y component in an FPU register. 
		inline float RAYMATH_CALLCONV Vector::GetY() const
		{
#if defined(RAY_NO_INTRINSICS)
			return v.vector4_f32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_f32(v, 1);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		// Return the Z component in an FPU register. 
		inline float RAYMATH_CALLCONV Vector::GetZ() const
		{
#if defined(RAY_NO_INTRINSICS)
			return v.vector4_f32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_f32(v, 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(2, 2, 2, 2));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		// Return the W component in an FPU register. 
		inline float RAYMATH_CALLCONV Vector::GetW() const
		{
#if defined(RAY_NO_INTRINSICS)
			return v.vector4_f32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_f32(v, 3);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(3, 3, 3, 3));
			return _mm_cvtss_f32(vTemp);
#endif
		}

		//------------------------------------------------------------------------------

		// Store a component indexed by i into a 32 bit float location in memory.

		inline void RAYMATH_CALLCONV VectorGetByIndexPtr(float* f, FVectorType V, size_t i)
		{
			assert(f != nullptr);
			assert(i < 4);
#if defined(RAY_NO_INTRINSICS)
			*f = V.vector4_f32[i];
#else
			VectorF32 U;
			U.v = V;
			*f = U.f[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Store the X component into a 32 bit float location in memory.

		inline void RAYMATH_CALLCONV VectorGetXPtr(float* x, FVectorType V)
		{
			assert(x != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*x = V.vector4_f32[0];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_f32(x, V, 0);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ss(x, V);
#endif
		}

		// Store the Y component into a 32 bit float location in memory.

		inline void RAYMATH_CALLCONV VectorGetYPtr(float* y, FVectorType V)
		{
			assert(y != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*y = V.vector4_f32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_f32(y, V, 1);
#elif defined(RAY_SSE4_INTRINSICS)
			* (reinterpret_cast<int*>(y)) = _mm_extract_ps(V, 1);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(y, vResult);
#endif
		}

		// Store the Z component into a 32 bit float location in memory.

		inline void RAYMATH_CALLCONV VectorGetZPtr(float* z, FVectorType V)
		{
			assert(z != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*z = V.vector4_f32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_f32(z, V, 2);
#elif defined(RAY_SSE4_INTRINSICS)
			* (reinterpret_cast<int*>(z)) = _mm_extract_ps(V, 2);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(z, vResult);
#endif
		}

		// Store the W component into a 32 bit float location in memory.

		inline void RAYMATH_CALLCONV VectorGetWPtr(float* w, FVectorType V)
		{
			assert(w != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*w = V.vector4_f32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_f32(w, V, 3);
#elif defined(RAY_SSE4_INTRINSICS)
			* (reinterpret_cast<int*>(w)) = _mm_extract_ps(V, 3);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			_mm_store_ss(w, vResult);
#endif
		}

		//------------------------------------------------------------------------------

		// Return an integer value via an index. This is not a recommended
		// function to use due to performance loss.
		inline uint32_t RAYMATH_CALLCONV VectorGetIntByIndex(FVectorType V, size_t i)
		{
			assert(i < 4);
#if defined(RAY_NO_INTRINSICS)
			return V.vector4_u32[i];
#else
			VectorU32 U;
			U.v = V;
			return U.u[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Return the X component in an integer register. 
		inline uint32_t RAYMATH_CALLCONV VectorGetIntX(FVectorType V)
		{
#if defined(RAY_NO_INTRINSICS)
			return V.vector4_u32[0];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_u32(V, 0);
#elif defined(RAY_SSE_INTRINSICS)
			return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(V)));
#endif
		}

		// Return the Y component in an integer register. 
		inline uint32_t RAYMATH_CALLCONV VectorGetIntY(FVectorType V)
		{
#if defined(RAY_NO_INTRINSICS)
			return V.vector4_u32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_u32(V, 1);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(1, 1, 1, 1));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		// Return the Z component in an integer register. 
		inline uint32_t RAYMATH_CALLCONV VectorGetIntZ(FVectorType V)
		{
#if defined(RAY_NO_INTRINSICS)
			return V.vector4_u32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_u32(V, 2);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(2, 2, 2, 2));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		// Return the W component in an integer register. 
		inline uint32_t RAYMATH_CALLCONV VectorGetIntW(FVectorType V)
		{
#if defined(RAY_NO_INTRINSICS)
			return V.vector4_u32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vgetq_lane_u32(V, 3);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			return static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vResulti = _mm_shuffle_epi32(_mm_castps_si128(V), _MM_SHUFFLE(3, 3, 3, 3));
			return static_cast<uint32_t>(_mm_cvtsi128_si32(vResulti));
#endif
		}

		//------------------------------------------------------------------------------

		// Store a component indexed by i into a 32 bit integer location in memory.

		inline void RAYMATH_CALLCONV VectorGetIntByIndexPtr(uint32_t* x, FVectorType V, size_t i)
		{
			assert(x != nullptr);
			assert(i < 4);
#if defined(RAY_NO_INTRINSICS)
			*x = V.vector4_u32[i];
#else
			VectorU32 U;
			U.v = V;
			*x = U.u[i];
#endif
		}

		//------------------------------------------------------------------------------

		// Store the X component into a 32 bit integer location in memory.

		inline void RAYMATH_CALLCONV VectorGetIntXPtr(uint32_t* x, FVectorType V)
		{
			assert(x != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*x = V.vector4_u32[0];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_u32(x, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(RAY_SSE_INTRINSICS)
			_mm_store_ss(reinterpret_cast<float*>(x), V);
#endif
		}

		// Store the Y component into a 32 bit integer location in memory.

		inline void RAYMATH_CALLCONV VectorGetIntYPtr(uint32_t* y, FVectorType V)
		{
			assert(y != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*y = V.vector4_u32[1];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_u32(y, *reinterpret_cast<const uint32x4_t*>(&V), 1);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			*y = static_cast<uint32_t>(_mm_extract_epi32(V1, 1));
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(reinterpret_cast<float*>(y), vResult);
#endif
		}

		// Store the Z component into a 32 bit integer locaCantion in memory.

		inline void RAYMATH_CALLCONV VectorGetIntZPtr(uint32_t* z, FVectorType V)
		{
			assert(z != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*z = V.vector4_u32[2];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_u32(z, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			*z = static_cast<uint32_t>(_mm_extract_epi32(V1, 2));
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(reinterpret_cast<float*>(z), vResult);
#endif
		}

		// Store the W component into a 32 bit integer location in memory.

		inline void RAYMATH_CALLCONV VectorGetIntWPtr(uint32_t* w, FVectorType V)
		{
			assert(w != nullptr);
#if defined(RAY_NO_INTRINSICS)
			*w = V.vector4_u32[3];
#elif defined(RAY_ARM_NEON_INTRINSICS)
			vst1q_lane_u32(w, *reinterpret_cast<const uint32x4_t*>(&V), 3);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i V1 = _mm_castps_si128(V);
			*w = static_cast<uint32_t>(_mm_extract_epi32(V1, 3));
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			_mm_store_ss(reinterpret_cast<float*>(w), vResult);
#endif
		}

		//------------------------------------------------------------------------------

		// Set a single indexed floating point component
		inline void RAYMATH_CALLCONV Vector::SetByIndex(float f, size_t i)
		{
			assert(i < 4);
			this->f[i] = f;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to a passed floating point value
		inline void RAYMATH_CALLCONV Vector::SetX(float x)
		{
#if defined(RAY_NO_INTRINSICS)
			this->x = x;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			*this = vsetq_lane_f32(x, v, 0);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_set_ss(x);
			*this = _mm_move_ss(v, vResult);
#endif
		}

		// Sets the Y component of a vector to a passed floating point value
		inline void RAYMATH_CALLCONV Vector::SetY(float y)
		{
#if defined(RAY_NO_INTRINSICS)
			this->y = y;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			*this = vsetq_lane_f32(y, v, 1);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vResult = _mm_set_ss(y);
			*this = _mm_insert_ps(v, vResult, 0x10);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap y and x
			VectorType vResult = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			VectorType vTemp = _mm_set_ss(y);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			*this = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
#endif
		}
		// Sets the Z component of a vector to a passed floating point value
		inline void RAYMATH_CALLCONV Vector::SetZ(float z)
		{
#if defined(RAY_NO_INTRINSICS)
			this->z = z;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			*this = vsetq_lane_f32(z, v, 2);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vResult = _mm_set_ss(z);
			*this = _mm_insert_ps(v, vResult, 0x20);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap z and x
			VectorType vResult = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			VectorType vTemp = _mm_set_ss(z);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			*this = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
#endif
		}

		// Sets the W component of a vector to a passed floating point value
		inline void RAYMATH_CALLCONV Vector::SetW(float w)
		{
#if defined(RAY_NO_INTRINSICS)
			this->w = w;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			*this = vsetq_lane_f32(w, v, 3);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vResult = _mm_set_ss(w);
			*this = _mm_insert_ps(v, vResult, 0x30);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap w and x
			VectorType vResult = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			VectorType vTemp = _mm_set_ss(w);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			*this = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to a floating point value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetByIndexPtr(FVectorType V, const float* f, size_t i)
		{
			assert(f != nullptr);
			assert(i < 4);
			VectorF32 U;
			U.v = V;
			U.f[i] = *f;
			return U.v;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to a floating point value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetXPtr(FVectorType V, const float* x)
		{
			assert(x != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorF32 U = { { {
					*x,
					V.vector4_f32[1],
					V.vector4_f32[2],
					V.vector4_f32[3]
				}
 }
			};
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_f32(x, V, 0);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_load_ss(x);
			vResult = _mm_move_ss(V, vResult);
			return vResult;
#endif
		}

		// Sets the Y component of a vector to a floating point value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetYPtr(FVectorType V, const float* y)
		{
			assert(y != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorF32 U = { { {
					V.vector4_f32[0],
					*y,
					V.vector4_f32[2],
					V.vector4_f32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_f32(y, V, 1);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap y and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(y);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to a floating point value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetZPtr(FVectorType V, const float* z)
		{
			assert(z != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					*z,
					V.vector4_f32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_f32(z, V, 2);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap z and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(z);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to a floating point value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetWPtr(FVectorType V, const float* w)
		{
			assert(w != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorF32 U = { { {
					V.vector4_f32[0],
					V.vector4_f32[1],
					V.vector4_f32[2],
					*w
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_f32(w, V, 3);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap w and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(w);
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to an integer passed by value
		inline VectorType RAYMATH_CALLCONV VectorSetIntByIndex(FVectorType V, uint32_t x, size_t i)
		{
			assert(i < 4);
			VectorU32 tmp;
			tmp.v = V;
			tmp.u[i] = x;
			return tmp;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to an integer passed by value
		inline VectorType RAYMATH_CALLCONV VectorSetIntX(FVectorType V, uint32_t x)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					x,
					V.vector4_u32[1],
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vsetq_lane_u32(x, V, 0);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(x));
			VectorType vResult = _mm_move_ss(V, _mm_castsi128_ps(vTemp));
			return vResult;
#endif
		}

		// Sets the Y component of a vector to an integer passed by value
		inline VectorType RAYMATH_CALLCONV VectorSetIntY(FVectorType V, uint32_t y)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					y,
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vsetq_lane_u32(y, V, 1);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(y), 1);
			return _mm_castsi128_ps(vResult);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap y and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(y));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap y and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to an integer passed by value
		inline VectorType RAYMATH_CALLCONV VectorSetIntZ(FVectorType V, uint32_t z)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					z,
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vsetq_lane_u32(z, V, 2);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(z), 2);
			return _mm_castsi128_ps(vResult);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap z and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(z));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap z and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to an integer passed by value
		inline VectorType RAYMATH_CALLCONV VectorSetIntW(FVectorType V, uint32_t w)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					V.vector4_u32[2],
					w
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vsetq_lane_u32(w, V, 3);
#elif defined(RAY_SSE4_INTRINSICS)
			__m128i vResult = _mm_castps_si128(V);
			vResult = _mm_insert_epi32(vResult, static_cast<int>(w), 3);
			return _mm_castsi128_ps(vResult);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap w and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			__m128i vTemp = _mm_cvtsi32_si128(static_cast<int>(w));
			// Replace the x component
			vResult = _mm_move_ss(vResult, _mm_castsi128_ps(vTemp));
			// Swap w and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		// Sets a component of a vector to an integer value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetIntByIndexPtr(FVectorType V, const uint32_t* x, size_t i)
		{
			assert(x != nullptr);
			assert(i < 4);
			VectorU32 tmp;
			tmp.v = V;
			tmp.u[i] = *x;
			return tmp;
		}

		//------------------------------------------------------------------------------

		// Sets the X component of a vector to an integer value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetIntXPtr(FVectorType V, const uint32_t* x)
		{
			assert(x != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					*x,
					V.vector4_u32[1],
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_u32(x, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_load_ss(reinterpret_cast<const float*>(x));
			VectorType vResult = _mm_move_ss(V, vTemp);
			return vResult;
#endif
		}

		// Sets the Y component of a vector to an integer value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetIntYPtr(FVectorType V, const uint32_t* y)
		{
			assert(y != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					*y,
					V.vector4_u32[2],
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_u32(y, *reinterpret_cast<const uint32x4_t*>(&V), 1);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap y and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(reinterpret_cast<const float*>(y));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap y and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
			return vResult;
#endif
		}

		// Sets the Z component of a vector to an integer value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetIntZPtr(FVectorType V, const uint32_t* z)
		{
			assert(z != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					*z,
					V.vector4_u32[3]
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_u32(z, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap z and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(reinterpret_cast<const float*>(z));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap z and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
			return vResult;
#endif
		}

		// Sets the W component of a vector to an integer value passed by pointer

		inline VectorType RAYMATH_CALLCONV VectorSetIntWPtr(FVectorType V, const uint32_t* w)
		{
			assert(w != nullptr);
#if defined(RAY_NO_INTRINSICS)
			VectorU32 U = { { {
					V.vector4_u32[0],
					V.vector4_u32[1],
					V.vector4_u32[2],
					*w
				} } };
			return U.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vld1q_lane_u32(w, *reinterpret_cast<const uint32x4_t*>(&V), 3);
#elif defined(RAY_SSE_INTRINSICS)
			// Swap w and x
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
			// Convert input to vector
			VectorType vTemp = _mm_load_ss(reinterpret_cast<const float*>(w));
			// Replace the x component
			vResult = _mm_move_ss(vResult, vTemp);
			// Swap w and x again
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Swizzle
		(
			FVectorType V,
			uint32_t E0,
			uint32_t E1,
			uint32_t E2,
			uint32_t E3
		)
		{
			assert((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					V.vector4_f32[E0],
					V.vector4_f32[E1],
					V.vector4_f32[E2],
					V.vector4_f32[E3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const uint32_t ControlElement[4] =
			{
				0x03020100, // RAYMATH_SWIZZLE_X
				0x07060504, // RAYMATH_SWIZZLE_Y
				0x0B0A0908, // RAYMATH_SWIZZLE_Z
				0x0F0E0D0C, // RAYMATH_SWIZZLE_W
			};

			int8x8x2_t tbl;
			tbl.val[0] = vget_low_f32(V);
			tbl.val[1] = vget_high_f32(V);

			uint32x2_t idx = vcreate_u32(static_cast<uint64_t>(ControlElement[E0]) | (static_cast<uint64_t>(ControlElement[E1]) << 32));
			const uint8x8_t rL = vtbl2_u8(tbl, idx);

			idx = vcreate_u32(static_cast<uint64_t>(ControlElement[E2]) | (static_cast<uint64_t>(ControlElement[E3]) << 32));
			const uint8x8_t rH = vtbl2_u8(tbl, idx);

			return vcombine_f32(rL, rH);
#elif defined(RAY_AVC_INTRINSICS)
			unsigned int elem[4] = { E0, E1, E2, E3 };
			__m128i vControl = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&elem[0]));
			return _mm_permutevar_ps(V, vControl);
#else
			auto aPtr = reinterpret_cast<const uint32_t*>(&V);

			VectorType Result;
			auto pWork = reinterpret_cast<uint32_t*>(&Result);

			pWork[0] = aPtr[E0];
			pWork[1] = aPtr[E1];
			pWork[2] = aPtr[E2];
			pWork[3] = aPtr[E3];

			return Result;
#endif
		}

		//------------------------------------------------------------------------------
		inline Vector RAYMATH_CALLCONV Vector::Permute
		(
			FVectorType V1,
			FVectorType V2,
			uint32_t PermuteX,
			uint32_t PermuteY,
			uint32_t PermuteZ,
			uint32_t PermuteW
		)
		{
			assert(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);

#if defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			static const uint32_t ControlElement[8] =
			{
				0x03020100, // RAYMATH_PERMUTE_0X
				0x07060504, // RAYMATH_PERMUTE_0Y
				0x0B0A0908, // RAYMATH_PERMUTE_0Z
				0x0F0E0D0C, // RAYMATH_PERMUTE_0W
				0x13121110, // RAYMATH_PERMUTE_1X
				0x17161514, // RAYMATH_PERMUTE_1Y
				0x1B1A1918, // RAYMATH_PERMUTE_1Z
				0x1F1E1D1C, // RAYMATH_PERMUTE_1W
			};

			int8x8x4_t tbl;
			tbl.val[0] = vget_low_f32(V1);
			tbl.val[1] = vget_high_f32(V1);
			tbl.val[2] = vget_low_f32(V2);
			tbl.val[3] = vget_high_f32(V2);

			uint32x2_t idx = vcreate_u32(static_cast<uint64_t>(ControlElement[PermuteX]) | (static_cast<uint64_t>(ControlElement[PermuteY]) << 32));
			const uint8x8_t rL = vtbl4_u8(tbl, idx);

			idx = vcreate_u32(static_cast<uint64_t>(ControlElement[PermuteZ]) | (static_cast<uint64_t>(ControlElement[PermuteW]) << 32));
			const uint8x8_t rH = vtbl4_u8(tbl, idx);

			return vcombine_f32(rL, rH);
#elif defined(RAY_AVC_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			static const VectorU32 three = { { { 3, 3, 3, 3 } } };

			RAY_ALIGNED(16) unsigned int elem[4] = { PermuteX, PermuteY, PermuteZ, PermuteW };
			__m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i*>(&elem[0]));

			__m128i vSelect = _mm_cmpgt_epi32(vControl, three);
			vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

			__m128 shuffled1 = _mm_permutevar_ps(V1, vControl);
			__m128 shuffled2 = _mm_permutevar_ps(V2, vControl);

			__m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
			__m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

			return _mm_or_ps(masked1, masked2);
#else

			const uint32_t* aPtr[2];
			aPtr[0] = reinterpret_cast<const uint32_t*>(&V1);
			aPtr[1] = reinterpret_cast<const uint32_t*>(&V2);

			VectorType Result;
			auto pWork = reinterpret_cast<uint32_t*>(&Result);

			const uint32_t i0 = PermuteX & 3;
			const uint32_t vi0 = PermuteX >> 2;
			pWork[0] = aPtr[vi0][i0];

			const uint32_t i1 = PermuteY & 3;
			const uint32_t vi1 = PermuteY >> 2;
			pWork[1] = aPtr[vi1][i1];

			const uint32_t i2 = PermuteZ & 3;
			const uint32_t vi2 = PermuteZ >> 2;
			pWork[2] = aPtr[vi2][i2];

			const uint32_t i3 = PermuteW & 3;
			const uint32_t vi3 = PermuteW >> 2;
			pWork[3] = aPtr[vi3][i3];

			return Result;
#endif
		}

		//------------------------------------------------------------------------------
		// Define a control vector to be used in Vector::Select 
		// operations.  The four integers specified in Vector::SelectControl
		// serve as indices to select between components in two vectors.
		// The first index controls selection for the first component of 
		// the vectors involved in a select operation, the second index 
		// controls selection for the second component etc.  A value of
		// zero for an index causes the corresponding component from the first 
		// vector to be selected whereas a one causes the component from the
		// second vector to be selected instead.

		inline Vector RAYMATH_CALLCONV Vector::SelectControl
		(
			uint32_t VectorIndex0,
			uint32_t VectorIndex1,
			uint32_t VectorIndex2,
			uint32_t VectorIndex3
		)
		{
#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			// x=Index0,y=Index1,z=Index2,w=Index3
			__m128i vTemp = _mm_set_epi32(static_cast<int>(VectorIndex3), static_cast<int>(VectorIndex2), static_cast<int>(VectorIndex1), static_cast<int>(VectorIndex0));
			// Any non-zero entries become 0xFFFFFFFF else 0
			vTemp = _mm_cmpgt_epi32(vTemp, Constants::Zero);
			return _mm_castsi128_ps(vTemp);
#elif defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			int32x2_t V0 = vcreate_s32(static_cast<uint64_t>(VectorIndex0) | (static_cast<uint64_t>(VectorIndex1) << 32));
			int32x2_t V1 = vcreate_s32(static_cast<uint64_t>(VectorIndex2) | (static_cast<uint64_t>(VectorIndex3) << 32));
			int32x4_t vTemp = vcombine_s32(V0, V1);
			// Any non-zero entries become 0xFFFFFFFF else 0
			return vcgtq_s32(vTemp, Constants::Zero);
#else
			VectorType    ControlVector;
			const uint32_t  ControlElement[] =
			{
				RAYMATH_SELECT_0,
				RAYMATH_SELECT_1
			};

			assert(VectorIndex0 < 2);
			assert(VectorIndex1 < 2);
			assert(VectorIndex2 < 2);
			assert(VectorIndex3 < 2);

			ControlVector.vector4_u32[0] = ControlElement[VectorIndex0];
			ControlVector.vector4_u32[1] = ControlElement[VectorIndex1];
			ControlVector.vector4_u32[2] = ControlElement[VectorIndex2];
			ControlVector.vector4_u32[3] = ControlElement[VectorIndex3];

			return ControlVector;

#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Select
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType Control
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					(V1.vector4_u32[0] & ~Control.vector4_u32[0]) | (V2.vector4_u32[0] & Control.vector4_u32[0]),
					(V1.vector4_u32[1] & ~Control.vector4_u32[1]) | (V2.vector4_u32[1] & Control.vector4_u32[1]),
					(V1.vector4_u32[2] & ~Control.vector4_u32[2]) | (V2.vector4_u32[2] & Control.vector4_u32[2]),
					(V1.vector4_u32[3] & ~Control.vector4_u32[3]) | (V2.vector4_u32[3] & Control.vector4_u32[3]),
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vbslq_f32(Control, V2, V1);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp1 = _mm_andnot_ps(Control, V1);
			VectorType vTemp2 = _mm_and_ps(V2, Control);
			return _mm_or_ps(vTemp1, vTemp2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::MergeXY
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[0],
					V2.vector4_u32[0],
					V1.vector4_u32[1],
					V2.vector4_u32[1],
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vzipq_f32(V1, V2).val[0];
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_unpacklo_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::MergeZW
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[2],
					V2.vector4_u32[2],
					V1.vector4_u32[3],
					V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vzipq_f32(V1, V2).val[1];
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_unpackhi_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::ShiftLeft(FVectorType V1, FVectorType V2, uint32_t Elements)
		{
			assert(Elements < 4);
			return Vector::Permute(V1, V2, Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3));
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::RotateLeft(FVectorType V, uint32_t Elements)
		{
			assert(Elements < 4);
			return Vector::Swizzle(V, Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3);
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::RotateRight(FVectorType V, uint32_t Elements)
		{
			assert(Elements < 4);
			return Vector::Swizzle(V, (4 - (Elements)) & 3, (5 - (Elements)) & 3, (6 - (Elements)) & 3, (7 - (Elements)) & 3);
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Insert(FVectorType VD, FVectorType VS, uint32_t VSLeftRotateElements,
			uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3)
		{
			VectorType Control = Vector::SelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
			return Vector::Select(VD, Vector::RotateLeft(VS, VSLeftRotateElements), Control);
		}

		//------------------------------------------------------------------------------
		// Comparison operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Equal
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFF : 0,
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vceqq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmpeq_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector RAYMATH_CALLCONV Vector::EqualR
		(
			uint32_t* pCR,
			FVectorType V1,
			FVectorType V2
		)
		{
			assert(pCR != nullptr);
#if defined(RAY_NO_INTRINSICS)
			uint32_t ux = (V1.vector4_f32[0] == V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (V1.vector4_f32[1] == V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (V1.vector4_f32[2] == V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (V1.vector4_f32[3] == V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are equal
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not equal
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------
		// Treat the components of the vectors as unsigned integers and
		// compare individual bits between the two.  This is useful for
		// comparing control vectors and result vectors returned from
		// other comparison operations.

		inline Vector RAYMATH_CALLCONV Vector::EqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(V1.vector4_u32[0] == V2.vector4_u32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[1] == V2.vector4_u32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[2] == V2.vector4_u32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_u32[3] == V2.vector4_u32[3]) ? 0xFFFFFFFF : 0,
				}
 }
			};
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vceqq_u32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector RAYMATH_CALLCONV Vector::EqualIntR
		(
			uint32_t* pCR,
			FVectorType V1,
			FVectorType V2
		)
		{
			assert(pCR != nullptr);
#if defined(RAY_NO_INTRINSICS)

			VectorType Control = Vector::EqualInt(V1, V2);

			*pCR = 0;
			if (Vector4EqualInt(Control, VectorTrueInt()))
			{
				// All elements are equal
				*pCR |= RAYMATH_CRMASK_CR6TRUE;
			}
			else if (Vector4EqualInt(Control, Vector::FalseInt()))
			{
				// All elements are not equal
				*pCR |= RAYMATH_CRMASK_CR6FALSE;
			}
			return Control;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are equal
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not equal
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTemp = _mm_movemask_ps(_mm_castsi128_ps(V));
			uint32_t CR = 0;
			if (iTemp == 0x0F)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTemp)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::NearEqual
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType Epsilon
		)
		{
#if defined(RAY_NO_INTRINSICS)

			float fDeltax = V1.vector4_f32[0] - V2.vector4_f32[0];
			float fDeltay = V1.vector4_f32[1] - V2.vector4_f32[1];
			float fDeltaz = V1.vector4_f32[2] - V2.vector4_f32[2];
			float fDeltaw = V1.vector4_f32[3] - V2.vector4_f32[3];

			fDeltax = fabsf(fDeltax);
			fDeltay = fabsf(fDeltay);
			fDeltaz = fabsf(fDeltaz);
			fDeltaw = fabsf(fDeltaw);

			VectorU32 Control = { { {
					(fDeltax <= Epsilon.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					(fDeltay <= Epsilon.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					(fDeltaz <= Epsilon.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					(fDeltaw <= Epsilon.vector4_f32[3]) ? 0xFFFFFFFFU : 0,
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vDelta = vsubq_f32(V1, V2);
			return vacleq_f32(vDelta, Epsilon);
#elif defined(RAY_SSE_INTRINSICS)
			// Get the difference
			VectorType vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			VectorType vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::NotEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(V1.vector4_f32[0] != V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[1] != V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[2] != V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(V1.vector4_f32[3] != V2.vector4_f32[3]) ? 0xFFFFFFFF : 0,
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vmvnq_u32(vceqq_f32(V1, V2));
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmpneq_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::NotEqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(V1.vector4_u32[0] != V2.vector4_u32[0]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[1] != V2.vector4_u32[1]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[2] != V2.vector4_u32[2]) ? 0xFFFFFFFFU : 0,
					(V1.vector4_u32[3] != V2.vector4_u32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vmvnq_u32(vceqq_u32(V1, V2));
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_xor_ps(_mm_castsi128_ps(V), Constants::NegOneMask);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Greater
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(v.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vcgtq_f32(v, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmpgt_ps(v, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector RAYMATH_CALLCONV Vector::GreaterR
		(
			uint32_t* pCR,
			FVectorType V2
		) const
		{
			assert(pCR != nullptr);
#if defined(RAY_NO_INTRINSICS)

			uint32_t ux = (v.vector4_f32[0] > V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (v.vector4_f32[1] > V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (v.vector4_f32[2] > V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (v.vector4_f32[3] > V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgtq_f32(v, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are greater
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(v, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::GreaterOrEqual
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(v.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} }
			};
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vcgeq_f32(v, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmpge_ps(v, V2);
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector RAYMATH_CALLCONV Vector::GreaterOrEqualR
		(
			uint32_t* pCR,
			FVectorType V2
		) const
		{
			assert(pCR != nullptr);
#if defined(RAY_NO_INTRINSICS)

			uint32_t ux = (v.vector4_f32[0] >= V2.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (v.vector4_f32[1] >= V2.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (v.vector4_f32[2] >= V2.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (v.vector4_f32[3] >= V2.vector4_f32[3]) ? 0xFFFFFFFFU : 0;
			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are greater
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!(ux | uy | uz | uw))
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgeq_f32(v, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are greater or equal
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				// All elements are not greater or equal
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(v, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				// All elements are not greater
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			*pCR = CR;
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Less
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(v.vector4_f32[0] < V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[1] < V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[2] < V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[3] < V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vcltq_f32(v, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmplt_ps(v, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::LessOrEqual
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(v.vector4_f32[0] <= V2.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[1] <= V2.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[2] <= V2.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[3] <= V2.vector4_f32[3]) ? 0xFFFFFFFF : 0
				}
 } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vcleq_f32(v, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_cmple_ps(v, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::InBounds
		(
			FVectorType Bounds
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					(v.vector4_f32[0] <= Bounds.vector4_f32[0] && v.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[1] <= Bounds.vector4_f32[1] && v.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[2] <= Bounds.vector4_f32[2] && v.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFF : 0,
					(v.vector4_f32[3] <= Bounds.vector4_f32[3] && v.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFF : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = vcleq_f32(v, Bounds);
			// Negate the bounds
			VectorType vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			vTemp2 = vcleq_f32(vTemp2, v);
			// Blend answers
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			return vTemp1;
#elif defined(RAY_SSE_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = _mm_cmple_ps(v, Bounds);
			// Negate the bounds
			VectorType vTemp2 = _mm_mul_ps(Bounds, Constants::NegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, v);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			return vTemp1;
#endif
		}

		//------------------------------------------------------------------------------


		inline Vector RAYMATH_CALLCONV Vector::InBoundsR
		(
			uint32_t* pCR,
			FVectorType Bounds
		) const
		{
			assert(pCR != nullptr);
#if defined(RAY_NO_INTRINSICS)

			uint32_t ux = (v.vector4_f32[0] <= Bounds.vector4_f32[0] && v.vector4_f32[0] >= -Bounds.vector4_f32[0]) ? 0xFFFFFFFFU : 0;
			uint32_t uy = (v.vector4_f32[1] <= Bounds.vector4_f32[1] && v.vector4_f32[1] >= -Bounds.vector4_f32[1]) ? 0xFFFFFFFFU : 0;
			uint32_t uz = (v.vector4_f32[2] <= Bounds.vector4_f32[2] && v.vector4_f32[2] >= -Bounds.vector4_f32[2]) ? 0xFFFFFFFFU : 0;
			uint32_t uw = (v.vector4_f32[3] <= Bounds.vector4_f32[3] && v.vector4_f32[3] >= -Bounds.vector4_f32[3]) ? 0xFFFFFFFFU : 0;

			uint32_t CR = 0;
			if (ux & uy & uz & uw)
			{
				// All elements are in bounds
				CR = RAYMATH_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;

			VectorU32 Control = { { { ux, uy, uz, uw } } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = vcleq_f32(v, Bounds);
			// Negate the bounds
			VectorType vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			vTemp2 = vcleq_f32(vTemp2, v);
			// Blend answers
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				// All elements are in bounds
				CR = RAYMATH_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;
			return vTemp1;
#elif defined(RAY_SSE_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = _mm_cmple_ps(v, Bounds);
			// Negate the bounds
			VectorType vTemp2 = _mm_mul_ps(Bounds, Constants::NegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, v);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);

			uint32_t CR = 0;
			if (_mm_movemask_ps(vTemp1) == 0xf) {
				// All elements are in bounds
				CR = RAYMATH_CRMASK_CR6BOUNDS;
			}
			*pCR = CR;
			return vTemp1;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::IsNaN() const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					RAYMATH_ISNAN(v.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISNAN(v.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISNAN(v.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISNAN(v.vector4_f32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(v, v);
			// Flip results
			return vmvnq_u32(vTempNan);
#elif defined(RAY_SSE_INTRINSICS)
			// Test against itself. NaN is always not equal
			return _mm_cmpneq_ps(v, v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::IsInfinite() const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Control = { { {
					RAYMATH_ISINF(v.vector4_f32[0]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISINF(v.vector4_f32[1]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISINF(v.vector4_f32[2]) ? 0xFFFFFFFFU : 0,
					RAYMATH_ISINF(v.vector4_f32[3]) ? 0xFFFFFFFFU : 0
				} } };
			return Control.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Mask off the sign bit
			uint32x4_t vTemp = vandq_u32(v, Constants::AbsMask);
			// Compare to infinity
			vTemp = vceqq_f32(vTemp, Constants::Infinity);
			// If any are infinity, the signs are true.
			return vTemp;
#elif defined(RAY_SSE_INTRINSICS)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(v, Constants::AbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, Constants::Infinity);
			// If any are infinity, the signs are true.
			return vTemp;
#endif
		}

		//------------------------------------------------------------------------------
		// Rounding and clamping operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Min
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					(V1.vector4_f32[0] < V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0],
					(V1.vector4_f32[1] < V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1],
					(V1.vector4_f32[2] < V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2],
					(V1.vector4_f32[3] < V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vminq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_min_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Max
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					(V1.vector4_f32[0] > V2.vector4_f32[0]) ? V1.vector4_f32[0] : V2.vector4_f32[0],
					(V1.vector4_f32[1] > V2.vector4_f32[1]) ? V1.vector4_f32[1] : V2.vector4_f32[1],
					(V1.vector4_f32[2] > V2.vector4_f32[2]) ? V1.vector4_f32[2] : V2.vector4_f32[2],
					(V1.vector4_f32[3] > V2.vector4_f32[3]) ? V1.vector4_f32[3] : V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vmaxq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_max_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		namespace Internal
		{
			// Round to nearest (even) a.k.a. banker's rounding
			inline float round_to_nearest(float x)
			{
				float i = floorf(x);
				x -= i;
				if (x < 0.5f)
					return i;
				if (x > 0.5f)
					return i + 1.f;

				float int_part;
				(void)modff(i / 2.f, &int_part);
				if ((2.f * int_part) == i)
				{
					return i;
				}

				return i + 1.f;
			}
		}

#if !defined(RAY_NO_INTRINSICS) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

		inline Vector RAYMATH_CALLCONV Vector::Round() const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					Internal::round_to_nearest(v.vector4_f32[0]),
					Internal::round_to_nearest(v.vector4_f32[1]),
					Internal::round_to_nearest(v.vector4_f32[2]),
					Internal::round_to_nearest(v.vector4_f32[3])
				}
 } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndnq_f32(v);
#else
			uint32x4_t sign = vandq_u32(v, Constants::NegativeZero);
			uint32x4_t sMagic = vorrq_u32(Constants::NoFraction, sign);
			float32x4_t R1 = vaddq_f32(v, sMagic);
			R1 = vsubq_f32(R1, sMagic);
			float32x4_t R2 = vabsq_f32(v);
			uint32x4_t mask = vcleq_f32(R2, Constants::NoFraction);
			VectorType vResult = vbslq_f32(mask, R1, v);
			return vResult;
#endif
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(RAY_SSE_INTRINSICS)
			__m128 sign = _mm_and_ps(v, Constants::NegativeZero);
			__m128 sMagic = _mm_or_ps(Constants::NoFraction, sign);
			__m128 R1 = _mm_add_ps(v, sMagic);
			R1 = _mm_sub_ps(R1, sMagic);
			__m128 R2 = _mm_and_ps(v, Constants::AbsMask);
			__m128 mask = _mm_cmple_ps(R2, Constants::NoFraction);
			R2 = _mm_andnot_ps(mask, v);
			R1 = _mm_and_ps(R1, mask);
			VectorType vResult = _mm_xor_ps(R1, R2);
			return vResult;
#endif
		}

#if !defined(RAY_NO_INTRINSICS) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Truncate() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorType Result;
			uint32_t     i;

			// Avoid C4701
			Result.vector4_f32[0] = 0.0f;

			for (i = 0; i < 4; i++)
			{
				if (RAYMATH_ISNAN(v.vector4_f32[i]))
				{
					Result.vector4_u32[i] = 0x7FC00000;
				}
				else if (fabsf(v.vector4_f32[i]) < 8388608.0f)
				{
					Result.vector4_f32[i] = static_cast<float>(static_cast<int32_t>(v.vector4_f32[i]));
				}
				else
				{
					Result.vector4_f32[i] = v.vector4_f32[i];
				}
			}
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndq_f32(v);
#else
			float32x4_t vTest = vabsq_f32(v);
			vTest = vcltq_f32(vTest, Constants::NoFraction);

			int32x4_t vInt = vcvtq_s32_f32(v);
			VectorType vResult = vcvtq_f32_s32(vInt);

			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, v);
#endif
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_round_ps(v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
#elif defined(RAY_SSE_INTRINSICS)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			// Get the abs value
			__m128i vTest = _mm_and_si128(_mm_castps_si128(v), Constants::AbsMask);
			// Test for greater than 8388608 (All floats with NO fractionals, NAN and INF
			vTest = _mm_cmplt_epi32(vTest, Constants::NoFraction);
			// Convert to int and back to float for rounding with truncation
			__m128i vInt = _mm_cvttps_epi32(v);
			// Convert back to floats
			VectorType vResult = _mm_cvtepi32_ps(vInt);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(v));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Floor() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					floorf(v.vector4_f32[0]),
					floorf(v.vector4_f32[1]),
					floorf(v.vector4_f32[2]),
					floorf(v.vector4_f32[3])
				}
 }
			};
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndmq_f32(v);
#else
			float32x4_t vTest = vabsq_f32(v);
			vTest = vcltq_f32(vTest, Constants::NoFraction);
			// Truncate
			int32x4_t vInt = vcvtq_s32_f32(v);
			VectorType vResult = vcvtq_f32_s32(vInt);
			VectorType vLarger = vcgtq_f32(vResult, v);
			// 0 -> 0, 0xffffffff -> -1.0f
			vLarger = vcvtq_f32_s32(vLarger);
			vResult = vaddq_f32(vResult, vLarger);
			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, v);
#endif
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_floor_ps(v);
#elif defined(RAY_SSE_INTRINSICS)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			__m128i vTest = _mm_and_si128(_mm_castps_si128(v), Constants::AbsMask);
			vTest = _mm_cmplt_epi32(vTest, Constants::NoFraction);
			// Truncate
			__m128i vInt = _mm_cvttps_epi32(v);
			VectorType vResult = _mm_cvtepi32_ps(vInt);
			__m128 vLarger = _mm_cmpgt_ps(vResult, v);
			// 0 -> 0, 0xffffffff -> -1.0f
			vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
			vResult = _mm_add_ps(vResult, vLarger);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(v));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Ceiling() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					ceilf(v.vector4_f32[0]),
					ceilf(v.vector4_f32[1]),
					ceilf(v.vector4_f32[2]),
					ceilf(v.vector4_f32[3])
				} }
			};
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vrndpq_f32(v);
#else
			float32x4_t vTest = vabsq_f32(v);
			vTest = vcltq_f32(vTest, Constants::NoFraction);
			// Truncate
			int32x4_t vInt = vcvtq_s32_f32(v);
			VectorType vResult = vcvtq_f32_s32(vInt);
			VectorType vSmaller = vcltq_f32(vResult, v);
			// 0 -> 0, 0xffffffff -> -1.0f
			vSmaller = vcvtq_f32_s32(vSmaller);
			vResult = vsubq_f32(vResult, vSmaller);
			// All numbers less than 8388608 will use the round to int
			// All others, use the ORIGINAL value
			return vbslq_f32(vTest, vResult, v);
#endif
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_ceil_ps(v);
#elif defined(RAY_SSE_INTRINSICS)
			// To handle NAN, INF and numbers greater than 8388608, use masking
			__m128i vTest = _mm_and_si128(_mm_castps_si128(v), Constants::AbsMask);
			vTest = _mm_cmplt_epi32(vTest, Constants::NoFraction);
			// Truncate
			__m128i vInt = _mm_cvttps_epi32(v);
			VectorType vResult = _mm_cvtepi32_ps(vInt);
			__m128 vSmaller = _mm_cmplt_ps(vResult, v);
			// 0 -> 0, 0xffffffff -> -1.0f
			vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
			vResult = _mm_sub_ps(vResult, vSmaller);
			// All numbers less than 8388608 will use the round to int
			vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
			// All others, use the ORIGINAL value
			vTest = _mm_andnot_si128(vTest, _mm_castps_si128(v));
			vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Clamp
		(
			FVectorType Min,
			FVectorType Max
		) const
		{
			assert(Vector4LessOrEqual(Min, Max));

#if defined(RAY_NO_INTRINSICS)

			VectorType Result;
			Result = Vector::Max(Min, v);
			Result = Vector::Min(Max, Result);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vResult;
			vResult = vmaxq_f32(Min, v);
			vResult = vminq_f32(Max, vResult);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult;
			vResult = _mm_max_ps(Min, v);
			vResult = _mm_min_ps(Max, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Saturate() const
		{
#if defined(RAY_NO_INTRINSICS)

			const VectorType Zero = Vector::Zero();

			return Clamp(Zero, Constants::One.v);

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Set <0 to 0
			VectorType vResult = vmaxq_f32(v, vdupq_n_f32(0));
			// Set>1 to 1
			return vminq_f32(vResult, vdupq_n_f32(1.0f));
#elif defined(RAY_SSE_INTRINSICS)
			// Set <0 to 0
			VectorType vResult = _mm_max_ps(v, Constants::Zero);
			// Set>1 to 1
			return _mm_min_ps(vResult, Constants::One);
#endif
		}

		//------------------------------------------------------------------------------
		// Bitwise logical operations
		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorAndInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[0] & V2.vector4_u32[0],
					V1.vector4_u32[1] & V2.vector4_u32[1],
					V1.vector4_u32[2] & V2.vector4_u32[2],
					V1.vector4_u32[3] & V2.vector4_u32[3]
				} } };
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vandq_u32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_and_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorAndCInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[0] & ~V2.vector4_u32[0],
					V1.vector4_u32[1] & ~V2.vector4_u32[1],
					V1.vector4_u32[2] & ~V2.vector4_u32[2],
					V1.vector4_u32[3] & ~V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vbicq_u32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_andnot_si128(_mm_castps_si128(V2), _mm_castps_si128(V1));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorOrInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[0] | V2.vector4_u32[0],
					V1.vector4_u32[1] | V2.vector4_u32[1],
					V1.vector4_u32[2] | V2.vector4_u32[2],
					V1.vector4_u32[3] | V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vorrq_u32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorNorInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					~(V1.vector4_u32[0] | V2.vector4_u32[0]),
					~(V1.vector4_u32[1] | V2.vector4_u32[1]),
					~(V1.vector4_u32[2] | V2.vector4_u32[2]),
					~(V1.vector4_u32[3] | V2.vector4_u32[3])
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t Result = vorrq_u32(V1, V2);
			return vbicq_u32(Constants::NegOneMask, Result);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i Result;
			Result = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			Result = _mm_andnot_si128(Result, Constants::NegOneMask);
			return _mm_castsi128_ps(Result);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorXorInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorU32 Result = { { {
					V1.vector4_u32[0] ^ V2.vector4_u32[0],
					V1.vector4_u32[1] ^ V2.vector4_u32[1],
					V1.vector4_u32[2] ^ V2.vector4_u32[2],
					V1.vector4_u32[3] ^ V2.vector4_u32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return veorq_u32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i V = _mm_xor_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return _mm_castsi128_ps(V);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Negate
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					-V.vector4_f32[0],
					-V.vector4_f32[1],
					-V.vector4_f32[2],
					-V.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vnegq_f32(V);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType Z;

			Z = _mm_setzero_ps();

			return _mm_sub_ps(Z, V);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Add
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					V1.vector4_f32[0] + V2.vector4_f32[0],
					V1.vector4_f32[1] + V2.vector4_f32[1],
					V1.vector4_f32[2] + V2.vector4_f32[2],
					V1.vector4_f32[3] + V2.vector4_f32[3]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vaddq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_add_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Sum() const
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = v.vector4_f32[0] + v.vector4_f32[1] + v.vector4_f32[2] + v.vector4_f32[3];
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			VectorType vTemp = vpaddq_f32(v, v);
			return vpaddq_f32(vTemp, vTemp);
#else
			float32x2_t v1 = vget_low_f32(v);
			float32x2_t v2 = vget_high_f32(v);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			return vcombine_f32(v1, v1);
#endif
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vTemp = _mm_hadd_ps(v, v);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = RAYMATH_PERMUTE_PS(v, _MM_SHUFFLE(2, 3, 0, 1));
			VectorType vTemp2 = _mm_add_ps(v, vTemp);
			vTemp = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 0, 3, 2));
			return _mm_add_ps(vTemp, vTemp2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::AddAngles
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			const VectorType Zero = Vector::Zero();

			// Add the given angles together.  If the range of V1 is such
			// that -Pi <= V1 < Pi and the range of V2 is such that
			// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
			// will be -Pi <= Result < Pi.
			Vector Result = Vector::Add(v, V2);

			VectorType Mask = Result.Less(Constants::NegativePi.v);
			VectorType Offset = Vector::Select(Zero, Constants::TwoPi.v, Mask);

			Mask = Result.GreaterOrEqual(Constants::Pi.v);
			Offset = Vector::Select(Offset, Constants::NegativeTwoPi.v, Mask);

			Result = Vector::Add(Result, Offset);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Adjust the angles
			VectorType vResult = vaddq_f32(v, V2);
			// Less than Pi?
			uint32x4_t vOffset = vcltq_f32(vResult, Constants::NegativePi);
			vOffset = vandq_u32(vOffset, Constants::TwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = vaddq_f32(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = vcgeq_f32(vResult, Constants::Pi);
			vOffset = vandq_u32(vOffset, Constants::TwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = vsubq_f32(vResult, vOffset);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Adjust the angles
			VectorType vResult = _mm_add_ps(v, V2);
			// Less than Pi?
			VectorType vOffset = _mm_cmplt_ps(vResult, Constants::NegativePi);
			vOffset = _mm_and_ps(vOffset, Constants::TwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = _mm_add_ps(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = _mm_cmpge_ps(vResult, Constants::Pi);
			vOffset = _mm_and_ps(vOffset, Constants::TwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = _mm_sub_ps(vResult, vOffset);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Subtract
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					V1.vector4_f32[0] - V2.vector4_f32[0],
					V1.vector4_f32[1] - V2.vector4_f32[1],
					V1.vector4_f32[2] - V2.vector4_f32[2],
					V1.vector4_f32[3] - V2.vector4_f32[3]
				}
 } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vsubq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_sub_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::SubtractAngles
		(
			FVectorType V2
		) const
		{
#if defined(RAY_NO_INTRINSICS)

			const VectorType Zero = Vector::Zero();

			// Subtract the given angles.  If the range of V1 is such
			// that -Pi <= V1 < Pi and the range of V2 is such that
			// -2Pi <= V2 <= 2Pi, then the range of the resulting angle
			// will be -Pi <= Result < Pi.
			Vector Result = Vector::Subtract(v, V2);

			VectorType Mask = Result.Less(Constants::NegativePi.v);
			VectorType Offset = Vector::Select(Zero, Constants::TwoPi.v, Mask);

			Mask = Result.GreaterOrEqual(Constants::Pi.v);
			Offset = Vector::Select(Offset, Constants::NegativeTwoPi.v, Mask);

			Result = Vector::Add(Result, Offset);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Adjust the angles
			VectorType vResult = vsubq_f32(v, V2);
			// Less than Pi?
			uint32x4_t vOffset = vcltq_f32(vResult, Constants::NegativePi);
			vOffset = vandq_u32(vOffset, Constants::TwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = vaddq_f32(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = vcgeq_f32(vResult, Constants::Pi);
			vOffset = vandq_u32(vOffset, Constants::TwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = vsubq_f32(vResult, vOffset);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Adjust the angles
			VectorType vResult = _mm_sub_ps(v, V2);
			// Less than Pi?
			VectorType vOffset = _mm_cmplt_ps(vResult, Constants::NegativePi);
			vOffset = _mm_and_ps(vOffset, Constants::TwoPi);
			// Add 2Pi to all entries less than -Pi
			vResult = _mm_add_ps(vResult, vOffset);
			// Greater than or equal to Pi?
			vOffset = _mm_cmpge_ps(vResult, Constants::Pi);
			vOffset = _mm_and_ps(vOffset, Constants::TwoPi);
			// Sub 2Pi to all entries greater than Pi
			vResult = _mm_sub_ps(vResult, vOffset);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Multiply
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					V1.vector4_f32[0] * V2.vector4_f32[0],
					V1.vector4_f32[1] * V2.vector4_f32[1],
					V1.vector4_f32[2] * V2.vector4_f32[2],
					V1.vector4_f32[3] * V2.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vmulq_f32(V1, V2);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_mul_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorMultiplyAdd
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType V3
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					V1.vector4_f32[0] * V2.vector4_f32[0] + V3.vector4_f32[0],
					V1.vector4_f32[1] * V2.vector4_f32[1] + V3.vector4_f32[1],
					V1.vector4_f32[2] * V2.vector4_f32[2] + V3.vector4_f32[2],
					V1.vector4_f32[3] * V2.vector4_f32[3] + V3.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vfmaq_f32(V3, V1, V2);
#else
			return vmlaq_f32(V3, V1, V2);
#endif
#elif defined(RAY_FMA3_INTRINSICS)
			return _mm_fmadd_ps(V1, V2, V3);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_mul_ps(V1, V2);
			return _mm_add_ps(vResult, V3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Divide
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					V1.vector4_f32[0] / V2.vector4_f32[0],
					V1.vector4_f32[1] / V2.vector4_f32[1],
					V1.vector4_f32[2] / V2.vector4_f32[2],
					V1.vector4_f32[3] / V2.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vdivq_f32(V1, V2);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x4_t Reciprocal = vrecpeq_f32(V2);
			float32x4_t S = vrecpsq_f32(Reciprocal, V2);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, V2);
			Reciprocal = vmulq_f32(S, Reciprocal);
			return vmulq_f32(V1, Reciprocal);
#endif
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_div_ps(V1, V2);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorNegativeMultiplySubtract
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType V3
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					V3.vector4_f32[0] - (V1.vector4_f32[0] * V2.vector4_f32[0]),
					V3.vector4_f32[1] - (V1.vector4_f32[1] * V2.vector4_f32[1]),
					V3.vector4_f32[2] - (V1.vector4_f32[2] * V2.vector4_f32[2]),
					V3.vector4_f32[3] - (V1.vector4_f32[3] * V2.vector4_f32[3])
				} } };
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			return vfmsq_f32(V3, V1, V2);
#else
			return vmlsq_f32(V3, V1, V2);
#endif
#elif defined(RAY_FMA3_INTRINSICS)
			return _mm_fnmadd_ps(V1, V2, V3);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType R = _mm_mul_ps(V1, V2);
			return _mm_sub_ps(V3, R);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Scale
		(
			float ScaleFactor
		) const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					v.vector4_f32[0] * ScaleFactor,
					v.vector4_f32[1] * ScaleFactor,
					v.vector4_f32[2] * ScaleFactor,
					v.vector4_f32[3] * ScaleFactor
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vmulq_n_f32(v, ScaleFactor);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_set_ps1(ScaleFactor);
			return _mm_mul_ps(vResult, v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::ReciprocalEst() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					1.f / v.vector4_f32[0],
					1.f / v.vector4_f32[1],
					1.f / v.vector4_f32[2],
					1.f / v.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vrecpeq_f32(v);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_rcp_ps(v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Reciprocal() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					1.f / v.vector4_f32[0],
					1.f / v.vector4_f32[1],
					1.f / v.vector4_f32[2],
					1.f / v.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t one = vdupq_n_f32(1.0f);
			return vdivq_f32(one, v);
#else
			// 2 iterations of Newton-Raphson refinement
			float32x4_t Reciprocal = vrecpeq_f32(v);
			float32x4_t S = vrecpsq_f32(Reciprocal, v);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, v);
			return vmulq_f32(S, Reciprocal);
#endif
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_div_ps(Constants::One, v);
#endif
		}

		//------------------------------------------------------------------------------
		// Return an estimated square root
		inline Vector RAYMATH_CALLCONV Vector::SqrtEst() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					sqrtf(v.vector4_f32[0]),
					sqrtf(v.vector4_f32[1]),
					sqrtf(v.vector4_f32[2]),
					sqrtf(v.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// 1 iteration of Newton-Raphson refinment of sqrt
			float32x4_t S0 = vrsqrteq_f32(v);
			float32x4_t P0 = vmulq_f32(v, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			float32x4_t S1 = vmulq_f32(S0, R0);

			VectorType VEqualsInfinity = Vector::EqualInt(v, Constants::Infinity.v);
			VectorType VEqualsZero = Vector::Equal(v, vdupq_n_f32(0));
			VectorType Result = vmulq_f32(v, S1);
			VectorType Select = Vector::EqualInt(VEqualsInfinity, VEqualsZero);
			return Vector::Select(v, Result, Select);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_sqrt_ps(v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::Sqrt() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					sqrtf(v.vector4_f32[0]),
					sqrtf(v.vector4_f32[1]),
					sqrtf(v.vector4_f32[2]),
					sqrtf(v.vector4_f32[3])
				}
 }
			};
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// 3 iterations of Newton-Raphson refinment of sqrt
			float32x4_t S0 = vrsqrteq_f32(v);
			float32x4_t P0 = vmulq_f32(v, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(v, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);
			float32x4_t S2 = vmulq_f32(S1, R1);
			float32x4_t P2 = vmulq_f32(v, S2);
			float32x4_t R2 = vrsqrtsq_f32(P2, S2);
			float32x4_t S3 = vmulq_f32(S2, R2);

			VectorType VEqualsInfinity = Vector::EqualInt(v, Constants::Infinity.v);
			VectorType VEqualsZero = Vector::Equal(v, vdupq_n_f32(0));
			VectorType Result = vmulq_f32(v, S3);
			VectorType Select = Vector::EqualInt(VEqualsInfinity, VEqualsZero);
			return Vector::Select(v, Result, Select);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_sqrt_ps(v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::ReciprocalSqrtEst() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					1.f / sqrtf(v.vector4_f32[0]),
					1.f / sqrtf(v.vector4_f32[1]),
					1.f / sqrtf(v.vector4_f32[2]),
					1.f / sqrtf(v.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vrsqrteq_f32(v);
#elif defined(RAY_SSE_INTRINSICS)
			return _mm_rsqrt_ps(v);
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Vector::ReciprocalSqrt() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					1.f / sqrtf(v.vector4_f32[0]),
					1.f / sqrtf(v.vector4_f32[1]),
					1.f / sqrtf(v.vector4_f32[2]),
					1.f / sqrtf(v.vector4_f32[3])
				} } };
			return Result;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x4_t S0 = vrsqrteq_f32(v);

			float32x4_t P0 = vmulq_f32(v, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);

			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(v, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);

			return vmulq_f32(S1, R1);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_sqrt_ps(v);
			vResult = _mm_div_ps(Constants::One, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorExp2
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					powf(2.0f, V.vector4_f32[0]),
					powf(2.0f, V.vector4_f32[1]),
					powf(2.0f, V.vector4_f32[2]),
					powf(2.0f, V.vector4_f32[3])
				} } };
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t itrunc = vcvtq_s32_f32(V);
			float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
			float32x4_t y = vsubq_f32(V, ftrunc);

			float32x4_t poly = vmlaq_f32(Constants::ExpEst6, Constants::ExpEst7, y);
			poly = vmlaq_f32(Constants::ExpEst5, poly, y);
			poly = vmlaq_f32(Constants::ExpEst4, poly, y);
			poly = vmlaq_f32(Constants::ExpEst3, poly, y);
			poly = vmlaq_f32(Constants::ExpEst2, poly, y);
			poly = vmlaq_f32(Constants::ExpEst1, poly, y);
			poly = vmlaq_f32(Constants::One, poly, y);

			int32x4_t biased = vaddq_s32(itrunc, Constants::ExponentBias);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result0 = Vector::Divide(biased, poly);

			biased = vaddq_s32(itrunc, Constants::Vector253);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result1 = Vector::Divide(biased, poly);
			result1 = vmulq_f32(Constants::MinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			int32x4_t comp = vcltq_s32(V, Constants::Bin128);
			float32x4_t result2 = vbslq_f32(comp, result0, Constants::Infinity);

			comp = vcltq_s32(itrunc, Constants::SubnormalExponent);
			float32x4_t result3 = vbslq_f32(comp, result1, result0);

			comp = vcltq_s32(V, Constants::BinNeg150);
			float32x4_t result4 = vbslq_f32(comp, result3, Constants::Zero);

			int32x4_t sign = vandq_s32(V, Constants::NegativeZero);
			comp = vceqq_s32(sign, Constants::NegativeZero);
			float32x4_t result5 = vbslq_f32(comp, result4, result2);

			int32x4_t t0 = vandq_s32(V, Constants::QNaNTest);
			int32x4_t t1 = vandq_s32(V, Constants::Infinity);
			t0 = vceqq_s32(t0, Constants::Zero);
			t1 = vceqq_s32(t1, Constants::Infinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t vResult = vbslq_f32(isNaN, Constants::QNaN, result5);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i itrunc = _mm_cvttps_epi32(V);
			__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
			__m128 y = _mm_sub_ps(V, ftrunc);
			__m128 poly = _mm_mul_ps(Constants::ExpEst7, y);
			poly = _mm_add_ps(Constants::ExpEst6, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst5, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst4, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst3, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst2, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst1, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::One, poly);

			__m128i biased = _mm_add_epi32(itrunc, Constants::ExponentBias);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

			biased = _mm_add_epi32(itrunc, Constants::Vector253);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
			result1 = _mm_mul_ps(Constants::MinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(V), Constants::Bin128);
			__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
			__m128i select1 = _mm_andnot_si128(comp, Constants::Infinity);
			__m128i result2 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(itrunc, Constants::SubnormalExponent);
			select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
			select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
			__m128i result3 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(_mm_castps_si128(V), Constants::BinNeg150);
			select0 = _mm_and_si128(comp, result3);
			select1 = _mm_andnot_si128(comp, Constants::Zero);
			__m128i result4 = _mm_or_si128(select0, select1);

			__m128i sign = _mm_and_si128(_mm_castps_si128(V), Constants::NegativeZero);
			comp = _mm_cmpeq_epi32(sign, Constants::NegativeZero);
			select0 = _mm_and_si128(comp, result4);
			select1 = _mm_andnot_si128(comp, result2);
			__m128i result5 = _mm_or_si128(select0, select1);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), Constants::QNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), Constants::Infinity);
			t0 = _mm_cmpeq_epi32(t0, Constants::Zero);
			t1 = _mm_cmpeq_epi32(t1, Constants::Infinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isNaN, Constants::QNaN);
			select1 = _mm_andnot_si128(isNaN, result5);
			__m128i vResult = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(vResult);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorExpE
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					expf(V.vector4_f32[0]),
					expf(V.vector4_f32[1]),
					expf(V.vector4_f32[2]),
					expf(V.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// expE(V) = exp2(vin*log2(e))
			float32x4_t Ve = vmulq_f32(Constants::LgE, V);

			int32x4_t itrunc = vcvtq_s32_f32(Ve);
			float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
			float32x4_t y = vsubq_f32(Ve, ftrunc);


			float32x4_t poly = vmlaq_f32(Constants::ExpEst6, Constants::ExpEst7, y);
			poly = vmlaq_f32(Constants::ExpEst5, poly, y);
			poly = vmlaq_f32(Constants::ExpEst4, poly, y);
			poly = vmlaq_f32(Constants::ExpEst3, poly, y);
			poly = vmlaq_f32(Constants::ExpEst2, poly, y);
			poly = vmlaq_f32(Constants::ExpEst1, poly, y);
			poly = vmlaq_f32(Constants::One, poly, y);

			int32x4_t biased = vaddq_s32(itrunc, Constants::ExponentBias);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result0 = Vector::Divide(biased, poly);

			biased = vaddq_s32(itrunc, Constants::Vector253);
			biased = vshlq_n_s32(biased, 23);
			float32x4_t result1 = Vector::Divide(biased, poly);
			result1 = vmulq_f32(Constants::MinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			int32x4_t comp = vcltq_s32(Ve, Constants::Bin128);
			float32x4_t result2 = vbslq_f32(comp, result0, Constants::Infinity);

			comp = vcltq_s32(itrunc, Constants::SubnormalExponent);
			float32x4_t result3 = vbslq_f32(comp, result1, result0);

			comp = vcltq_s32(Ve, Constants::BinNeg150);
			float32x4_t result4 = vbslq_f32(comp, result3, Constants::Zero);

			int32x4_t sign = vandq_s32(Ve, Constants::NegativeZero);
			comp = vceqq_s32(sign, Constants::NegativeZero);
			float32x4_t result5 = vbslq_f32(comp, result4, result2);

			int32x4_t t0 = vandq_s32(Ve, Constants::QNaNTest);
			int32x4_t t1 = vandq_s32(Ve, Constants::Infinity);
			t0 = vceqq_s32(t0, Constants::Zero);
			t1 = vceqq_s32(t1, Constants::Infinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t vResult = vbslq_f32(isNaN, Constants::QNaN, result5);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// expE(V) = exp2(vin*log2(e))
			__m128 Ve = _mm_mul_ps(Constants::LgE, V);

			__m128i itrunc = _mm_cvttps_epi32(Ve);
			__m128 ftrunc = _mm_cvtepi32_ps(itrunc);
			__m128 y = _mm_sub_ps(Ve, ftrunc);
			__m128 poly = _mm_mul_ps(Constants::ExpEst7, y);
			poly = _mm_add_ps(Constants::ExpEst6, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst5, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst4, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst3, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst2, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::ExpEst1, poly);
			poly = _mm_mul_ps(poly, y);
			poly = _mm_add_ps(Constants::One, poly);

			__m128i biased = _mm_add_epi32(itrunc, Constants::ExponentBias);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

			biased = _mm_add_epi32(itrunc, Constants::Vector253);
			biased = _mm_slli_epi32(biased, 23);
			__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
			result1 = _mm_mul_ps(Constants::MinNormal.v, result1);

			// Use selection to handle the cases
			//  if (V is NaN) -> QNaN;
			//  else if (V sign bit set)
			//      if (V > -150)
			//         if (V.exponent < -126) -> result1
			//         else -> result0
			//      else -> +0
			//  else
			//      if (V < 128) -> result0
			//      else -> +inf

			__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), Constants::Bin128);
			__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
			__m128i select1 = _mm_andnot_si128(comp, Constants::Infinity);
			__m128i result2 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(itrunc, Constants::SubnormalExponent);
			select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
			select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
			__m128i result3 = _mm_or_si128(select0, select1);

			comp = _mm_cmplt_epi32(_mm_castps_si128(Ve), Constants::BinNeg150);
			select0 = _mm_and_si128(comp, result3);
			select1 = _mm_andnot_si128(comp, Constants::Zero);
			__m128i result4 = _mm_or_si128(select0, select1);

			__m128i sign = _mm_and_si128(_mm_castps_si128(Ve), Constants::NegativeZero);
			comp = _mm_cmpeq_epi32(sign, Constants::NegativeZero);
			select0 = _mm_and_si128(comp, result4);
			select1 = _mm_andnot_si128(comp, result2);
			__m128i result5 = _mm_or_si128(select0, select1);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(Ve), Constants::QNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(Ve), Constants::Infinity);
			t0 = _mm_cmpeq_epi32(t0, Constants::Zero);
			t1 = _mm_cmpeq_epi32(t1, Constants::Infinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isNaN, Constants::QNaN);
			select1 = _mm_andnot_si128(isNaN, result5);
			__m128i vResult = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(vResult);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorExp
		(
			FVectorType V
		)
		{
			return VectorExp2(V);
		}

		//------------------------------------------------------------------------------

#if defined(RAY_SSE_INTRINSICS)

		namespace Internal
		{
			inline __m128i multi_sll_epi32(__m128i value, __m128i count)
			{
				__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
				__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r0 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r1 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r2 = _mm_sll_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r3 = _mm_sll_epi32(v, c);

				// (r0,r0,r1,r1)
				__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
				// (r2,r2,r3,r3)
				__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
				// (r0,r1,r2,r3)
				__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
				return _mm_castps_si128(result);
			}

			inline __m128i multi_srl_epi32(__m128i value, __m128i count)
			{
				__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
				__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r0 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r1 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r2 = _mm_srl_epi32(v, c);

				v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
				c = _mm_and_si128(c, Constants::MaskX);
				__m128i r3 = _mm_srl_epi32(v, c);

				// (r0,r0,r1,r1)
				__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
				// (r2,r2,r3,r3)
				__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
				// (r0,r1,r2,r3)
				__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
				return _mm_castps_si128(result);
			}

			inline __m128i GetLeadingBit(const __m128i value)
			{
				static const VectorI32 vec0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
				static const VectorI32 vec000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
				static const VectorI32 vec0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
				static const VectorI32 vec00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

				__m128i v = value, r, c, b, s;

				c = _mm_cmpgt_epi32(v, vec0000FFFF);   // c = (v > 0xFFFF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				r = _mm_slli_epi32(b, 4);               // r = (b << 4)
				v = multi_srl_epi32(v, r);              // v = (v >> r)

				c = _mm_cmpgt_epi32(v, vec000000FF);   // c = (v > 0xFF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 3);               // s = (b << 3)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				c = _mm_cmpgt_epi32(v, vec0000000F);   // c = (v > 0xF)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 2);               // s = (b << 2)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				c = _mm_cmpgt_epi32(v, vec00000003);   // c = (v > 0x3)
				b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
				s = _mm_slli_epi32(b, 1);               // s = (b << 1)
				v = multi_srl_epi32(v, s);              // v = (v >> s)
				r = _mm_or_si128(r, s);                 // r = (r | s)

				s = _mm_srli_epi32(v, 1);
				r = _mm_or_si128(r, s);
				return r;
			}
		} // namespace Internal

#endif // RAY_SSE_INTRINSICS

#if defined(RAY_ARM_NEON_INTRINSICS)

		namespace Internal
		{
			inline int32x4_t GetLeadingBit(const int32x4_t value)
			{
				static const VectorI32 vec0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
				static const VectorI32 vec000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
				static const VectorI32 vec0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
				static const VectorI32 vec00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

				int32x4_t v = value, r, c, b, s;

				c = vcgtq_s32(v, vec0000FFFF);     // c = (v > 0xFFFF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				r = vshlq_n_s32(b, 4);              // r = (b << 4)
				r = vnegq_s32(r);
				v = vshlq_u32(v, r);              // v = (v >> r)

				c = vcgtq_s32(v, vec000000FF);     // c = (v > 0xFF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 3);              // s = (b << 3)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				c = vcgtq_s32(v, vec0000000F);     // c = (v > 0xF)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 2);              // s = (b << 2)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				c = vcgtq_s32(v, vec00000003);     // c = (v > 0x3)
				b = vshrq_n_u32(c, 31);             // b = (c ? 1 : 0)
				s = vshlq_n_s32(b, 1);              // s = (b << 1)
				s = vnegq_s32(s);
				v = vshlq_u32(v, s);                // v = (v >> s)
				r = vorrq_s32(r, s);                // r = (r | s)

				s = vshrq_n_u32(v, 1);
				r = vorrq_s32(r, s);
				return r;
			}

		} // namespace Internal

#endif

//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorLog2
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			const float fScale = 1.4426950f; // (1.0f / logf(2.0f));

			VectorF32 Result = { { {
					logf(V.vector4_f32[0]) * fScale,
					logf(V.vector4_f32[1]) * fScale,
					logf(V.vector4_f32[2]) * fScale,
					logf(V.vector4_f32[3]) * fScale
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t rawBiased = vandq_s32(V, Constants::Infinity);
			int32x4_t trailing = vandq_s32(V, Constants::QNaNTest);
			int32x4_t isExponentZero = vceqq_s32(Constants::Zero, rawBiased);

			// Compute exponent and significand for normals.
			int32x4_t biased = vshrq_n_u32(rawBiased, 23);
			int32x4_t exponentNor = vsubq_s32(biased, Constants::ExponentBias);
			int32x4_t trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			int32x4_t leading = Internal::GetLeadingBit(trailing);
			int32x4_t shift = vsubq_s32(Constants::NumTrailing, leading);
			int32x4_t exponentSub = vsubq_s32(Constants::SubnormalExponent, shift);
			int32x4_t trailingSub = vshlq_u32(trailing, shift);
			trailingSub = vandq_s32(trailingSub, Constants::QNaNTest);
			int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
			int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

			// Compute the approximation.
			int32x4_t tmp = vorrq_s32(Constants::One, t);
			float32x4_t y = vsubq_f32(tmp, Constants::One);

			float32x4_t log2 = vmlaq_f32(Constants::LogEst6, Constants::LogEst7, y);
			log2 = vmlaq_f32(Constants::LogEst5, log2, y);
			log2 = vmlaq_f32(Constants::LogEst4, log2, y);
			log2 = vmlaq_f32(Constants::LogEst3, log2, y);
			log2 = vmlaq_f32(Constants::LogEst2, log2, y);
			log2 = vmlaq_f32(Constants::LogEst1, log2, y);
			log2 = vmlaq_f32(Constants::LogEst0, log2, y);
			log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			int32x4_t isInfinite = vandq_s32((V), Constants::AbsMask);
			isInfinite = vceqq_s32(isInfinite, Constants::Infinity);

			int32x4_t isGreaterZero = vcgtq_s32((V), Constants::Zero);
			int32x4_t isNotFinite = vcgtq_s32((V), Constants::Infinity);
			int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

			int32x4_t isZero = vandq_s32((V), Constants::AbsMask);
			isZero = vceqq_s32(isZero, Constants::Zero);

			int32x4_t t0 = vandq_s32((V), Constants::QNaNTest);
			int32x4_t t1 = vandq_s32((V), Constants::Infinity);
			t0 = vceqq_s32(t0, Constants::Zero);
			t1 = vceqq_s32(t1, Constants::Infinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t result = vbslq_f32(isInfinite, Constants::Infinity, log2);
			tmp = vbslq_f32(isZero, Constants::NegInfinity, Constants::NegQNaN);
			result = vbslq_f32(isPositive, result, tmp);
			result = vbslq_f32(isNaN, Constants::QNaN, result);
			return result;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), Constants::Infinity);
			__m128i trailing = _mm_and_si128(_mm_castps_si128(V), Constants::QNaNTest);
			__m128i isExponentZero = _mm_cmpeq_epi32(Constants::Zero, rawBiased);

			// Compute exponent and significand for normals.
			__m128i biased = _mm_srli_epi32(rawBiased, 23);
			__m128i exponentNor = _mm_sub_epi32(biased, Constants::ExponentBias);
			__m128i trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			__m128i leading = Internal::GetLeadingBit(trailing);
			__m128i shift = _mm_sub_epi32(Constants::NumTrailing, leading);
			__m128i exponentSub = _mm_sub_epi32(Constants::SubnormalExponent, shift);
			__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
			trailingSub = _mm_and_si128(trailingSub, Constants::QNaNTest);

			__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
			__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
			__m128i e = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isExponentZero, trailingSub);
			select1 = _mm_andnot_si128(isExponentZero, trailingNor);
			__m128i t = _mm_or_si128(select0, select1);

			// Compute the approximation.
			__m128i tmp = _mm_or_si128(Constants::One, t);
			__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), Constants::One);

			__m128 log2 = _mm_mul_ps(Constants::LogEst7, y);
			log2 = _mm_add_ps(Constants::LogEst6, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst5, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst4, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst3, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst2, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst1, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst0, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), Constants::AbsMask);
			isInfinite = _mm_cmpeq_epi32(isInfinite, Constants::Infinity);

			__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), Constants::Zero);
			__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), Constants::Infinity);
			__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

			__m128i isZero = _mm_and_si128(_mm_castps_si128(V), Constants::AbsMask);
			isZero = _mm_cmpeq_epi32(isZero, Constants::Zero);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), Constants::QNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), Constants::Infinity);
			t0 = _mm_cmpeq_epi32(t0, Constants::Zero);
			t1 = _mm_cmpeq_epi32(t1, Constants::Infinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isInfinite, Constants::Infinity);
			select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
			__m128i result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isZero, Constants::NegInfinity);
			select1 = _mm_andnot_si128(isZero, Constants::NegQNaN);
			tmp = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isPositive, result);
			select1 = _mm_andnot_si128(isPositive, tmp);
			result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isNaN, Constants::QNaN);
			select1 = _mm_andnot_si128(isNaN, result);
			result = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(result);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorLogE
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					logf(V.vector4_f32[0]),
					logf(V.vector4_f32[1]),
					logf(V.vector4_f32[2]),
					logf(V.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			int32x4_t rawBiased = vandq_s32(V, Constants::Infinity);
			int32x4_t trailing = vandq_s32(V, Constants::QNaNTest);
			int32x4_t isExponentZero = vceqq_s32(Constants::Zero, rawBiased);

			// Compute exponent and significand for normals.
			int32x4_t biased = vshrq_n_u32(rawBiased, 23);
			int32x4_t exponentNor = vsubq_s32(biased, Constants::ExponentBias);
			int32x4_t trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			int32x4_t leading = Internal::GetLeadingBit(trailing);
			int32x4_t shift = vsubq_s32(Constants::NumTrailing, leading);
			int32x4_t exponentSub = vsubq_s32(Constants::SubnormalExponent, shift);
			int32x4_t trailingSub = vshlq_u32(trailing, shift);
			trailingSub = vandq_s32(trailingSub, Constants::QNaNTest);
			int32x4_t e = vbslq_f32(isExponentZero, exponentSub, exponentNor);
			int32x4_t t = vbslq_f32(isExponentZero, trailingSub, trailingNor);

			// Compute the approximation.
			int32x4_t tmp = vorrq_s32(Constants::One, t);
			float32x4_t y = vsubq_f32(tmp, Constants::One);

			float32x4_t log2 = vmlaq_f32(Constants::LogEst6, Constants::LogEst7, y);
			log2 = vmlaq_f32(Constants::LogEst5, log2, y);
			log2 = vmlaq_f32(Constants::LogEst4, log2, y);
			log2 = vmlaq_f32(Constants::LogEst3, log2, y);
			log2 = vmlaq_f32(Constants::LogEst2, log2, y);
			log2 = vmlaq_f32(Constants::LogEst1, log2, y);
			log2 = vmlaq_f32(Constants::LogEst0, log2, y);
			log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

			log2 = vmulq_f32(Constants::InvLgE, log2);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			int32x4_t isInfinite = vandq_s32((V), Constants::AbsMask);
			isInfinite = vceqq_s32(isInfinite, Constants::Infinity);

			int32x4_t isGreaterZero = vcgtq_s32((V), Constants::Zero);
			int32x4_t isNotFinite = vcgtq_s32((V), Constants::Infinity);
			int32x4_t isPositive = vbicq_s32(isGreaterZero, isNotFinite);

			int32x4_t isZero = vandq_s32((V), Constants::AbsMask);
			isZero = vceqq_s32(isZero, Constants::Zero);

			int32x4_t t0 = vandq_s32((V), Constants::QNaNTest);
			int32x4_t t1 = vandq_s32((V), Constants::Infinity);
			t0 = vceqq_s32(t0, Constants::Zero);
			t1 = vceqq_s32(t1, Constants::Infinity);
			int32x4_t isNaN = vbicq_s32(t1, t0);

			float32x4_t result = vbslq_f32(isInfinite, Constants::Infinity, log2);
			tmp = vbslq_f32(isZero, Constants::NegInfinity, Constants::NegQNaN);
			result = vbslq_f32(isPositive, result, tmp);
			result = vbslq_f32(isNaN, Constants::QNaN, result);
			return result;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), Constants::Infinity);
			__m128i trailing = _mm_and_si128(_mm_castps_si128(V), Constants::QNaNTest);
			__m128i isExponentZero = _mm_cmpeq_epi32(Constants::Zero, rawBiased);

			// Compute exponent and significand for normals.
			__m128i biased = _mm_srli_epi32(rawBiased, 23);
			__m128i exponentNor = _mm_sub_epi32(biased, Constants::ExponentBias);
			__m128i trailingNor = trailing;

			// Compute exponent and significand for subnormals.
			__m128i leading = Internal::GetLeadingBit(trailing);
			__m128i shift = _mm_sub_epi32(Constants::NumTrailing, leading);
			__m128i exponentSub = _mm_sub_epi32(Constants::SubnormalExponent, shift);
			__m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
			trailingSub = _mm_and_si128(trailingSub, Constants::QNaNTest);

			__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
			__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
			__m128i e = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isExponentZero, trailingSub);
			select1 = _mm_andnot_si128(isExponentZero, trailingNor);
			__m128i t = _mm_or_si128(select0, select1);

			// Compute the approximation.
			__m128i tmp = _mm_or_si128(Constants::One, t);
			__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), Constants::One);

			__m128 log2 = _mm_mul_ps(Constants::LogEst7, y);
			log2 = _mm_add_ps(Constants::LogEst6, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst5, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst4, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst3, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst2, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst1, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(Constants::LogEst0, log2);
			log2 = _mm_mul_ps(log2, y);
			log2 = _mm_add_ps(log2, _mm_cvtepi32_ps(e));

			log2 = _mm_mul_ps(Constants::InvLgE, log2);

			//  if (x is NaN) -> QNaN
			//  else if (V is positive)
			//      if (V is infinite) -> +inf
			//      else -> log2(V)
			//  else
			//      if (V is zero) -> -inf
			//      else -> -QNaN

			__m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), Constants::AbsMask);
			isInfinite = _mm_cmpeq_epi32(isInfinite, Constants::Infinity);

			__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), Constants::Zero);
			__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), Constants::Infinity);
			__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

			__m128i isZero = _mm_and_si128(_mm_castps_si128(V), Constants::AbsMask);
			isZero = _mm_cmpeq_epi32(isZero, Constants::Zero);

			__m128i t0 = _mm_and_si128(_mm_castps_si128(V), Constants::QNaNTest);
			__m128i t1 = _mm_and_si128(_mm_castps_si128(V), Constants::Infinity);
			t0 = _mm_cmpeq_epi32(t0, Constants::Zero);
			t1 = _mm_cmpeq_epi32(t1, Constants::Infinity);
			__m128i isNaN = _mm_andnot_si128(t0, t1);

			select0 = _mm_and_si128(isInfinite, Constants::Infinity);
			select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
			__m128i result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isZero, Constants::NegInfinity);
			select1 = _mm_andnot_si128(isZero, Constants::NegQNaN);
			tmp = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isPositive, result);
			select1 = _mm_andnot_si128(isPositive, tmp);
			result = _mm_or_si128(select0, select1);

			select0 = _mm_and_si128(isNaN, Constants::QNaN);
			select1 = _mm_andnot_si128(isNaN, result);
			result = _mm_or_si128(select0, select1);

			return _mm_castsi128_ps(result);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorLog
		(
			FVectorType V
		)
		{
			return VectorLog2(V);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorPow
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					powf(V1.vector4_f32[0], V2.vector4_f32[0]),
					powf(V1.vector4_f32[1], V2.vector4_f32[1]),
					powf(V1.vector4_f32[2], V2.vector4_f32[2]),
					powf(V1.vector4_f32[3], V2.vector4_f32[3])
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorF32 vResult = { { {
					powf(vgetq_lane_f32(V1, 0), vgetq_lane_f32(V2, 0)),
					powf(vgetq_lane_f32(V1, 1), vgetq_lane_f32(V2, 1)),
					powf(vgetq_lane_f32(V1, 2), vgetq_lane_f32(V2, 2)),
					powf(vgetq_lane_f32(V1, 3), vgetq_lane_f32(V2, 3))
				} } };
			return vResult.v;
#elif defined(RAY_SSE_INTRINSICS)
			RAY_ALIGNED(16) float a[4];
			RAY_ALIGNED(16) float b[4];
			_mm_store_ps(a, V1);
			_mm_store_ps(b, V2);
			VectorType vResult = _mm_setr_ps(
				powf(a[0], b[0]),
				powf(a[1], b[1]),
				powf(a[2], b[2]),
				powf(a[3], b[3]));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorAbs
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { {
					fabsf(V.vector4_f32[0]),
					fabsf(V.vector4_f32[1]),
					fabsf(V.vector4_f32[2]),
					fabsf(V.vector4_f32[3])
				} } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			return vabsq_f32(V);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = _mm_setzero_ps();
			vResult = _mm_sub_ps(vResult, V);
			vResult = _mm_max_ps(vResult, V);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorMod
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			// V1 % V2 = V1 - V2 * truncate(V1 / V2)

#if defined(RAY_NO_INTRINSICS)

			Vector Quotient = Vector::Divide(V1, V2);
			Quotient = Quotient.Truncate();
			VectorType Result = VectorNegativeMultiplySubtract(V2, Quotient, V1);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vResult = Vector::Divide(V1, V2);
			vResult = Vector::Truncate(vResult);
			return vmlsq_f32(V1, vResult, V2);
#elif defined(RAY_SSE_INTRINSICS)
			Vector vResult = _mm_div_ps(V1, V2);
			vResult = vResult.Truncate();
			vResult = _mm_mul_ps(vResult, V2);
			vResult = _mm_sub_ps(V1, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorModAngles
		(
			FVectorType Angles
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector V;
			VectorType Result;

			// Modulo the range of the given angles such that -Constants::PI <= Angles < Constants::PI
			V = Vector::Multiply(Angles, Constants::ReciprocalTwoPi.v);
			V = V.Round();
			Result = VectorNegativeMultiplySubtract(Constants::TwoPi.v, V, Angles);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Modulo the range of the given angles such that -Constants::PI <= Angles < Constants::PI
			VectorType vResult = vmulq_f32(Angles, Constants::ReciprocalTwoPi);
			// Use the inline function due to complexity for rounding
			vResult = Vector::Round(vResult);
			return vmlsq_f32(Angles, vResult, Constants::TwoPi);
#elif defined(RAY_SSE_INTRINSICS)
			// Modulo the range of the given angles such that -Constants::PI <= Angles < Constants::PI
			Vector vResult = _mm_mul_ps(Angles, Constants::ReciprocalTwoPi);
			// Use the inline function due to complexity for rounding
			vResult = vResult.Round();
			vResult = _mm_mul_ps(vResult, Constants::TwoPi);
			vResult = _mm_sub_ps(Angles, vResult);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSin
		(
			FVectorType V
		)
		{
			// 11-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType SC1 = Constants::SinCoefficients1;
			const VectorType SC0 = Constants::SinCoefficients0;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			Result = vmulq_f32(Result, x);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			__m128 sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType SC1 = Constants::SinCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const VectorType SC0 = Constants::SinCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, x);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorCos
		(
			FVectorType V
		)
		{
			// 10-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Map V to x in [-pi,pi].
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType CC1 = Constants::CosCoefficients1;
			const VectorType CC0 = Constants::CosCoefficients0;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			Result = vmulq_f32(Result, sign);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
	// Map V to x in [-pi,pi].
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			VectorType sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, Constants::One);
			select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType CC1 = Constants::CosCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const VectorType CC0 = Constants::CosCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, sign);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------


		inline void RAYMATH_CALLCONV VectorSinCos
		(
			VectorType* pSin,
			VectorType* pCos,
			FVectorType V
		)
		{
			assert(pSin != nullptr);
			assert(pCos != nullptr);

			// 11/10-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Sin = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };

			VectorF32 Cos = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };

			*pSin = Sin.v;
			*pCos = Cos.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t  rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation for sine
			const VectorType SC1 = Constants::SinCoefficients1;
			const VectorType SC0 = Constants::SinCoefficients0;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			*pSin = vmulq_f32(Result, x);

			// Compute polynomial approximation for cosine
			const VectorType CC1 = Constants::CosCoefficients1;
			const VectorType CC0 = Constants::CosCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
			Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			*pCos = vmulq_f32(Result, sign);
#elif defined(RAY_SSE_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
			VectorType sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, Constants::One);
			select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation of sine
			const VectorType SC1 = Constants::SinCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const VectorType SC0 = Constants::SinCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, x);
			*pSin = Result;

			// Compute polynomial approximation of cosine
			const VectorType CC1 = Constants::CosCoefficients1;
			vConstants = RAYMATH_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_mul_ps(vConstants, x2);

			const VectorType CC0 = Constants::CosCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, sign);
			*pCos = Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorTan
		(
			FVectorType V
		)
		{
			// Cody and Waite algorithm to compute tangent.

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					tanf(V.vector4_f32[0]),
					tanf(V.vector4_f32[1]),
					tanf(V.vector4_f32[2]),
					tanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_SSE_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS) 

			static const VectorF32 TanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
			static const VectorF32 TanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
			static const VectorF32 TanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
			static const VectorU32 Mask = { { { 0x1, 0x1, 0x1, 0x1 } } };

			VectorType TwoDivPi = Vector::SplatW(TanConstants.v);

			VectorType Zero = Vector::Zero();

			VectorType C0 = Vector::SplatX(TanConstants.v);
			VectorType C1 = Vector::SplatY(TanConstants.v);
			VectorType Epsilon = Vector::SplatZ(TanConstants.v);

			Vector VA = Vector::Multiply(V, TwoDivPi);

			VA = VA.Round();

			Vector VC = VectorNegativeMultiplySubtract(VA, C0, V);

			VectorType VB = VectorAbs(VA);

			VC = VectorNegativeMultiplySubtract(VA, C1, VC);

#if defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			VB = vcvtq_u32_f32(VB);
#elif defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			reinterpret_cast<__m128i*>(&VB)[0] = _mm_cvttps_epi32(VB);
#else
			for (size_t i = 0; i < 4; i++)
			{
				VB.vector4_u32[i] = static_cast<uint32_t>(VB.vector4_f32[i]);
			}
#endif

			VectorType VC2 = Vector::Multiply(VC, VC);

			VectorType T7 = Vector::SplatW(TanCoefficients1.v);
			VectorType T6 = Vector::SplatZ(TanCoefficients1.v);
			VectorType T4 = Vector::SplatX(TanCoefficients1.v);
			VectorType T3 = Vector::SplatW(TanCoefficients0.v);
			VectorType T5 = Vector::SplatY(TanCoefficients1.v);
			VectorType T2 = Vector::SplatZ(TanCoefficients0.v);
			VectorType T1 = Vector::SplatY(TanCoefficients0.v);
			VectorType T0 = Vector::SplatX(TanCoefficients0.v);

			VectorType VBIsEven = VectorAndInt(VB, Mask.v);
			VBIsEven = Vector::EqualInt(VBIsEven, Zero);

			VectorType N = VectorMultiplyAdd(VC2, T7, T6);
			VectorType D = VectorMultiplyAdd(VC2, T4, T3);
			N = VectorMultiplyAdd(VC2, N, T5);
			D = VectorMultiplyAdd(VC2, D, T2);
			N = Vector::Multiply(VC2, N);
			D = VectorMultiplyAdd(VC2, D, T1);
			N = VectorMultiplyAdd(VC, N, VC);
			VectorType VCNearZero = VC.InBounds(Epsilon);
			D = VectorMultiplyAdd(VC2, D, T0);

			N = Vector::Select(N, VC, VCNearZero);
			D = Vector::Select(D, Constants::One.v, VCNearZero);

			VectorType R0 = Vector::Negate(N);
			VectorType R1 = Vector::Divide(N, D);
			R0 = Vector::Divide(D, R0);

			VectorType VIsZero = Vector::Equal(V, Zero);

			VectorType Result = Vector::Select(R0, R1, VBIsEven);

			Result = Vector::Select(Result, Zero, VIsZero);

			return Result;

#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSinH
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					sinhf(V.vector4_f32[0]),
					sinhf(V.vector4_f32[1]),
					sinhf(V.vector4_f32[2]),
					sinhf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			VectorType V1 = vmlaq_f32(Constants::NegativeOne.v, V, Scale.v);
			VectorType V2 = vmlsq_f32(Constants::NegativeOne.v, V, Scale.v);
			VectorType E1 = VectorExp(V1);
			VectorType E2 = VectorExp(V2);

			return vsubq_f32(E1, E2);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			VectorType V1 = _mm_mul_ps(V, Scale);
			V1 = _mm_add_ps(V1, Constants::NegativeOne);
			VectorType V2 = _mm_mul_ps(V, Scale);
			V2 = _mm_sub_ps(Constants::NegativeOne, V2);
			VectorType E1 = VectorExp(V1);
			VectorType E2 = VectorExp(V2);

			return _mm_sub_ps(E1, E2);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorCosH
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					coshf(V.vector4_f32[0]),
					coshf(V.vector4_f32[1]),
					coshf(V.vector4_f32[2]),
					coshf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			VectorType V1 = vmlaq_f32(Constants::NegativeOne.v, V, Scale.v);
			VectorType V2 = vmlsq_f32(Constants::NegativeOne.v, V, Scale.v);
			VectorType E1 = VectorExp(V1);
			VectorType E2 = VectorExp(V2);
			return vaddq_f32(E1, E2);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

			VectorType V1 = _mm_mul_ps(V, Scale.v);
			V1 = _mm_add_ps(V1, Constants::NegativeOne.v);
			VectorType V2 = _mm_mul_ps(V, Scale.v);
			V2 = _mm_sub_ps(Constants::NegativeOne.v, V2);
			VectorType E1 = VectorExp(V1);
			VectorType E2 = VectorExp(V2);
			return _mm_add_ps(E1, E2);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorTanH
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					tanhf(V.vector4_f32[0]),
					tanhf(V.vector4_f32[1]),
					tanhf(V.vector4_f32[2]),
					tanhf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

			Vector E = vmulq_f32(V, Scale.v);
			E = VectorExp(E);
			E = vmlaq_f32(Constants::OneHalf.v, E, Constants::OneHalf.v);
			E = E.Reciprocal();
			return vsubq_f32(Constants::One.v, E);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

			VectorType E = _mm_mul_ps(V, Scale.v);
			E = VectorExp(E);
			E = _mm_mul_ps(E, Constants::OneHalf.v);
			E = _mm_add_ps(E, Constants::OneHalf.v);
			E = _mm_div_ps(Constants::One.v, E);
			return _mm_sub_ps(Constants::One.v, E);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorASin
		(
			FVectorType V
		)
		{
			// 7-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					asinf(V.vector4_f32[0]),
					asinf(V.vector4_f32[1]),
					asinf(V.vector4_f32[2]),
					asinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t nonnegative = vcgeq_f32(V, Constants::Zero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(Constants::One, x);
			Vector clampOneMValue = vmaxq_f32(Constants::Zero, oneMValue);
			float32x4_t root = clampOneMValue.Sqrt();

			// Compute polynomial approximation
			const VectorType AC1 = Constants::ArcCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
			VectorType t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			const VectorType AC0 = Constants::ArcCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(Constants::Pi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			t0 = vsubq_f32(Constants::HalfPi, t0);
			return t0;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 nonnegative = _mm_cmpge_ps(V, Constants::Zero);
			__m128 mvalue = _mm_sub_ps(Constants::Zero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(Constants::One, x);
			__m128 clampOneMValue = _mm_max_ps(Constants::Zero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const VectorType AC1 = Constants::ArcCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			const VectorType AC0 = Constants::ArcCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(Constants::Pi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			t0 = _mm_sub_ps(Constants::HalfPi, t0);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorACos
		(
			FVectorType V
		)
		{
			// 7-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					acosf(V.vector4_f32[0]),
					acosf(V.vector4_f32[1]),
					acosf(V.vector4_f32[2]),
					acosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t nonnegative = vcgeq_f32(V, Constants::Zero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(Constants::One, x);
			Vector clampOneMValue = vmaxq_f32(Constants::Zero, oneMValue);
			float32x4_t root = clampOneMValue.Sqrt();

			// Compute polynomial approximation
			const VectorType AC1 = Constants::ArcCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
			VectorType t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			const VectorType AC0 = Constants::ArcCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(Constants::Pi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			return t0;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 nonnegative = _mm_cmpge_ps(V, Constants::Zero);
			__m128 mvalue = _mm_sub_ps(Constants::Zero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(Constants::One, x);
			__m128 clampOneMValue = _mm_max_ps(Constants::Zero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const VectorType AC1 = Constants::ArcCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			const VectorType AC0 = Constants::ArcCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(Constants::Pi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorATan
		(
			FVectorType V
		)
		{
			// 17-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					atanf(V.vector4_f32[0]),
					atanf(V.vector4_f32[1]),
					atanf(V.vector4_f32[2]),
					atanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t absV = vabsq_f32(V);
			float32x4_t invV = Vector(V).Reciprocal();
			uint32x4_t comp = vcgtq_f32(V, Constants::One);
			uint32x4_t sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);
			comp = vcleq_f32(absV, Constants::One);
			sign = vbslq_f32(comp, Constants::Zero, sign);
			uint32x4_t x = vbslq_f32(comp, V, invV);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType TC1 = Constants::ATanCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(TC1), 0);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(TC1), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(TC1), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC1), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			const VectorType TC0 = Constants::ATanCoefficients0;
			vConstants = vdupq_lane_f32(vget_high_f32(TC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_high_f32(TC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC0), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(TC0), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			Result = vmulq_f32(Result, x);

			float32x4_t result1 = vmulq_f32(sign, Constants::HalfPi);
			result1 = vsubq_f32(result1, Result);

			comp = vceqq_f32(sign, Constants::Zero);
			Result = vbslq_f32(comp, Result, result1);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 absV = VectorAbs(V);
			__m128 invV = _mm_div_ps(Constants::One, V);
			__m128 comp = _mm_cmpgt_ps(V, Constants::One);
			__m128 select0 = _mm_and_ps(comp, Constants::One);
			__m128 select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			__m128 sign = _mm_or_ps(select0, select1);
			comp = _mm_cmple_ps(absV, Constants::One);
			select0 = _mm_and_ps(comp, Constants::Zero);
			select1 = _mm_andnot_ps(comp, sign);
			sign = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, V);
			select1 = _mm_andnot_ps(comp, invV);
			__m128 x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType TC1 = Constants::ATanCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			const VectorType TC0 = Constants::ATanCoefficients0;
			vConstants = RAYMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, x);
			__m128 result1 = _mm_mul_ps(sign, Constants::HalfPi);
			result1 = _mm_sub_ps(result1, Result);

			comp = _mm_cmpeq_ps(sign, Constants::Zero);
			select0 = _mm_and_ps(comp, Result);
			select1 = _mm_andnot_ps(comp, result1);
			Result = _mm_or_ps(select0, select1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorATan2
		(
			FVectorType Y,
			FVectorType X
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					atan2f(Y.vector4_f32[0], X.vector4_f32[0]),
					atan2f(Y.vector4_f32[1], X.vector4_f32[1]),
					atan2f(Y.vector4_f32[2], X.vector4_f32[2]),
					atan2f(Y.vector4_f32[3], X.vector4_f32[3])
				} } };
			return Result.v;
#else

			// Return the inverse tangent of Y / X in the range of -Pi to Pi with the following exceptions:

			//     Y == 0 and X is Negative         -> Pi with the sign of Y
			//     y == 0 and x is positive         -> 0 with the sign of y
			//     Y != 0 and X == 0                -> Pi / 2 with the sign of Y
			//     Y != 0 and X is Negative         -> atan(y/x) + (PI with the sign of Y)
			//     X == -Infinity and Finite Y      -> Pi with the sign of Y
			//     X == +Infinity and Finite Y      -> 0 with the sign of Y
			//     Y == Infinity and X is Finite    -> Pi / 2 with the sign of Y
			//     Y == Infinity and X == -Infinity -> 3Pi / 4 with the sign of Y
			//     Y == Infinity and X == +Infinity -> Pi / 4 with the sign of Y

			static const VectorF32 ATan2Constants = { { { Constants::PI, Constants::PIDIV2, Constants::PIDIV4, Constants::PI * 3.0f / 4.0f } } };

			VectorType Zero = Vector::Zero();
			VectorType ATanResultValid = VectorTrueInt();

			VectorType Pi = Vector::SplatX(ATan2Constants);
			VectorType PiOverTwo = Vector::SplatY(ATan2Constants);
			VectorType PiOverFour = Vector::SplatZ(ATan2Constants);
			VectorType ThreePiOverFour = Vector::SplatW(ATan2Constants);

			VectorType YEqualsZero = Vector::Equal(Y, Zero);
			VectorType XEqualsZero = Vector::Equal(X, Zero);
			VectorType XIsPositive = VectorAndInt(X, Constants::NegativeZero.v);
			XIsPositive = Vector::EqualInt(XIsPositive, Zero);
			VectorType YEqualsInfinity = Vector(Y).IsInfinite();
			VectorType XEqualsInfinity = Vector(X).IsInfinite();

			VectorType YSign = VectorAndInt(Y, Constants::NegativeZero.v);
			Pi = VectorOrInt(Pi, YSign);
			PiOverTwo = VectorOrInt(PiOverTwo, YSign);
			PiOverFour = VectorOrInt(PiOverFour, YSign);
			ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

			VectorType R1 = Vector::Select(Pi, YSign, XIsPositive);
			VectorType R2 = Vector::Select(ATanResultValid, PiOverTwo, XEqualsZero);
			VectorType R3 = Vector::Select(R2, R1, YEqualsZero);
			VectorType R4 = Vector::Select(ThreePiOverFour, PiOverFour, XIsPositive);
			VectorType R5 = Vector::Select(PiOverTwo, R4, XEqualsInfinity);
			VectorType Result = Vector::Select(R3, R5, YEqualsInfinity);
			ATanResultValid = Vector::EqualInt(Result, ATanResultValid);

			VectorType V = Vector::Divide(Y, X);

			VectorType R0 = VectorATan(V);

			R1 = Vector::Select(Pi, Constants::NegativeZero, XIsPositive);
			R2 = Vector::Add(R0, R1);

			return Vector::Select(Result, R2, ATanResultValid);

#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorSinEst
		(
			FVectorType V
		)
		{
			// 7-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType SEC = Constants::SinCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			Result = vmulq_f32(Result, x);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			__m128 sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType SEC = Constants::SinCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, x);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorCosEst
		(
			FVectorType V
		)
		{
			// 6-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Map V to x in [-pi,pi].
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType CEC = Constants::CosCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			Result = vmulq_f32(Result, sign);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
	// Map V to x in [-pi,pi].
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			VectorType sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, Constants::One);
			select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType CEC = Constants::CosCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, sign);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------


		inline void RAYMATH_CALLCONV VectorSinCosEst
		(
			VectorType* pSin,
			VectorType* pCos,
			FVectorType  V
		)
		{
			assert(pSin != nullptr);
			assert(pCos != nullptr);

			// 7/6-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Sin = { { {
					sinf(V.vector4_f32[0]),
					sinf(V.vector4_f32[1]),
					sinf(V.vector4_f32[2]),
					sinf(V.vector4_f32[3])
				} } };

			VectorF32 Cos = { { {
					cosf(V.vector4_f32[0]),
					cosf(V.vector4_f32[1]),
					cosf(V.vector4_f32[2]),
					cosf(V.vector4_f32[3])
				} } };

			*pSin = Sin.v;
			*pCos = Cos.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			uint32x4_t sign = vandq_u32(x, Constants::NegativeZero);
			uint32x4_t c = vorrq_u32(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x);
			float32x4_t rflx = vsubq_f32(c, x);
			uint32x4_t comp = vcleq_f32(absx, Constants::HalfPi);
			x = vbslq_f32(comp, x, rflx);
			sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation for sine
			const VectorType SEC = Constants::SinCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			*pSin = vmulq_f32(Result, x);

			// Compute polynomial approximation
			const VectorType CEC = Constants::CosCoefficients1;
			vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
			Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			Result = vmlaq_f32(Constants::One, Result, x2);
			*pCos = vmulq_f32(Result, sign);
#elif defined(RAY_SSE_INTRINSICS)
	// Force the value within the bounds of pi
			VectorType x = VectorModAngles(V);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
			VectorType sign = _mm_and_ps(x, Constants::NegativeZero);
			__m128 c = _mm_or_ps(Constants::Pi, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, Constants::HalfPi);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, Constants::One);
			select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation for sine
			const VectorType SEC = Constants::SinCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, x);
			*pSin = Result;

			// Compute polynomial approximation for cosine
			const VectorType CEC = Constants::CosCoefficients1;
			vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			Result = _mm_add_ps(Result, Constants::One);
			Result = _mm_mul_ps(Result, sign);
			*pCos = Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorTanEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					tanf(V.vector4_f32[0]),
					tanf(V.vector4_f32[1]),
					tanf(V.vector4_f32[2]),
					tanf(V.vector4_f32[3])
				} } };
			return Result.v;
#else

			VectorType OneOverPi = Vector::SplatW(Constants::TanEstCoefficients.v);

			Vector V1 = Vector::Multiply(V, OneOverPi);
			V1 = V1.Round();

			V1 = VectorNegativeMultiplySubtract(Constants::Pi.v, V1, V);

			VectorType T0 = Vector::SplatX(Constants::TanEstCoefficients.v);
			VectorType T1 = Vector::SplatY(Constants::TanEstCoefficients.v);
			VectorType T2 = Vector::SplatZ(Constants::TanEstCoefficients.v);

			VectorType V2T2 = VectorNegativeMultiplySubtract(V1, V1, T2);
			VectorType V2 = Vector::Multiply(V1, V1);
			VectorType V1T0 = Vector::Multiply(V1, T0);
			VectorType V1T1 = Vector::Multiply(V1, T1);

			VectorType D = Vector(V2T2).ReciprocalEst();
			VectorType N = VectorMultiplyAdd(V2, V1T1, V1T0);

			return Vector::Multiply(N, D);

#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorASinEst
		(
			FVectorType V
		)
		{
			// 3-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result;
			Result.f[0] = asinf(V.vector4_f32[0]);
			Result.f[1] = asinf(V.vector4_f32[1]);
			Result.f[2] = asinf(V.vector4_f32[2]);
			Result.f[3] = asinf(V.vector4_f32[3]);
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t nonnegative = vcgeq_f32(V, Constants::Zero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(Constants::One, x);
			Vector clampOneMValue = vmaxq_f32(Constants::Zero, oneMValue);
			float32x4_t root = clampOneMValue.Sqrt();

			// Compute polynomial approximation
			const VectorType AEC = Constants::ArcEstCoefficients;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			VectorType t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(Constants::Pi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			t0 = vsubq_f32(Constants::HalfPi, t0);
			return t0;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 nonnegative = _mm_cmpge_ps(V, Constants::Zero);
			__m128 mvalue = _mm_sub_ps(Constants::Zero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(Constants::One, x);
			__m128 clampOneMValue = _mm_max_ps(Constants::Zero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const VectorType AEC = Constants::ArcEstCoefficients;
			VectorType vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(Constants::Pi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			t0 = _mm_sub_ps(Constants::HalfPi, t0);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorACosEst
		(
			FVectorType V
		)
		{
			// 3-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					acosf(V.vector4_f32[0]),
					acosf(V.vector4_f32[1]),
					acosf(V.vector4_f32[2]),
					acosf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t nonnegative = vcgeq_f32(V, Constants::Zero);
			float32x4_t x = vabsq_f32(V);

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			float32x4_t oneMValue = vsubq_f32(Constants::One, x);
			Vector clampOneMValue = vmaxq_f32(Constants::Zero, oneMValue);
			float32x4_t root = clampOneMValue.Sqrt();

			// Compute polynomial approximation
			const VectorType AEC = Constants::ArcEstCoefficients;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			VectorType t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			t0 = vmlaq_f32(vConstants, t0, x);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			t0 = vmlaq_f32(vConstants, t0, x);
			t0 = vmulq_f32(t0, root);

			float32x4_t t1 = vsubq_f32(Constants::Pi, t0);
			t0 = vbslq_f32(nonnegative, t0, t1);
			return t0;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 nonnegative = _mm_cmpge_ps(V, Constants::Zero);
			__m128 mvalue = _mm_sub_ps(Constants::Zero, V);
			__m128 x = _mm_max_ps(V, mvalue);  // |V|

			// Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
			__m128 oneMValue = _mm_sub_ps(Constants::One, x);
			__m128 clampOneMValue = _mm_max_ps(Constants::Zero, oneMValue);
			__m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

			// Compute polynomial approximation
			const VectorType AEC = Constants::ArcEstCoefficients;
			VectorType vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 t0 = _mm_mul_ps(vConstants, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, x);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			t0 = _mm_add_ps(t0, vConstants);
			t0 = _mm_mul_ps(t0, root);

			__m128 t1 = _mm_sub_ps(Constants::Pi, t0);
			t0 = _mm_and_ps(nonnegative, t0);
			t1 = _mm_andnot_ps(nonnegative, t1);
			t0 = _mm_or_ps(t0, t1);
			return t0;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorATanEst
		(
			FVectorType V
		)
		{
			// 9-degree minimax approximation

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					atanf(V.vector4_f32[0]),
					atanf(V.vector4_f32[1]),
					atanf(V.vector4_f32[2]),
					atanf(V.vector4_f32[3])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t absV = vabsq_f32(V);
			float32x4_t invV = Vector(V).ReciprocalEst();
			uint32x4_t comp = vcgtq_f32(V, Constants::One);
			uint32x4_t sign = vbslq_f32(comp, Constants::One, Constants::NegativeOne);
			comp = vcleq_f32(absV, Constants::One);
			sign = vbslq_f32(comp, Constants::Zero, sign);
			uint32x4_t x = vbslq_f32(comp, V, invV);

			float32x4_t x2 = vmulq_f32(x, x);

			// Compute polynomial approximation
			const VectorType AEC = Constants::ATanEstCoefficients1;
			VectorType vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
			VectorType Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(AEC), 1);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
			Result = vmlaq_f32(vConstants, Result, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
			Result = vmlaq_f32(vConstants, Result, x2);

			// ATanEstCoefficients0 is already splatted
			Result = vmlaq_f32(Constants::ATanEstCoefficients0, Result, x2);
			Result = vmulq_f32(Result, x);

			float32x4_t result1 = vmulq_f32(sign, Constants::HalfPi);
			result1 = vsubq_f32(result1, Result);

			comp = vceqq_f32(sign, Constants::Zero);
			Result = vbslq_f32(comp, Result, result1);
			return Result;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 absV = VectorAbs(V);
			__m128 invV = _mm_div_ps(Constants::One, V);
			__m128 comp = _mm_cmpgt_ps(V, Constants::One);
			__m128 select0 = _mm_and_ps(comp, Constants::One);
			__m128 select1 = _mm_andnot_ps(comp, Constants::NegativeOne);
			__m128 sign = _mm_or_ps(select0, select1);
			comp = _mm_cmple_ps(absV, Constants::One);
			select0 = _mm_and_ps(comp, Constants::Zero);
			select1 = _mm_andnot_ps(comp, sign);
			sign = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, V);
			select1 = _mm_andnot_ps(comp, invV);
			__m128 x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const VectorType AEC = Constants::ATanEstCoefficients1;
			VectorType vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = RAYMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			// ATanEstCoefficients0 is already splatted
			Result = _mm_add_ps(Result, Constants::ATanEstCoefficients0);
			Result = _mm_mul_ps(Result, x);
			__m128 result1 = _mm_mul_ps(sign, Constants::HalfPi);
			result1 = _mm_sub_ps(result1, Result);

			comp = _mm_cmpeq_ps(sign, Constants::Zero);
			select0 = _mm_and_ps(comp, Result);
			select1 = _mm_andnot_ps(comp, result1);
			Result = _mm_or_ps(select0, select1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorATan2Est
		(
			FVectorType Y,
			FVectorType X
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					atan2f(Y.vector4_f32[0], X.vector4_f32[0]),
					atan2f(Y.vector4_f32[1], X.vector4_f32[1]),
					atan2f(Y.vector4_f32[2], X.vector4_f32[2]),
					atan2f(Y.vector4_f32[3], X.vector4_f32[3]),
				} } };
			return Result.v;
#else

			static const VectorF32 ATan2Constants = { { { Constants::PI, Constants::PIDIV2, Constants::PIDIV4, 2.3561944905f /* Pi*3/4 */ } } };

			const VectorType Zero = Vector::Zero();
			VectorType ATanResultValid = VectorTrueInt();

			VectorType Pi = Vector::SplatX(ATan2Constants);
			VectorType PiOverTwo = Vector::SplatY(ATan2Constants);
			VectorType PiOverFour = Vector::SplatZ(ATan2Constants);
			VectorType ThreePiOverFour = Vector::SplatW(ATan2Constants);

			VectorType YEqualsZero = Vector::Equal(Y, Zero);
			VectorType XEqualsZero = Vector::Equal(X, Zero);
			VectorType XIsPositive = VectorAndInt(X, Constants::NegativeZero.v);
			XIsPositive = Vector::EqualInt(XIsPositive, Zero);
			VectorType YEqualsInfinity = Vector(Y).IsInfinite();
			VectorType XEqualsInfinity = Vector(X).IsInfinite();

			VectorType YSign = VectorAndInt(Y, Constants::NegativeZero.v);
			Pi = VectorOrInt(Pi, YSign);
			PiOverTwo = VectorOrInt(PiOverTwo, YSign);
			PiOverFour = VectorOrInt(PiOverFour, YSign);
			ThreePiOverFour = VectorOrInt(ThreePiOverFour, YSign);

			VectorType R1 = Vector::Select(Pi, YSign, XIsPositive);
			VectorType R2 = Vector::Select(ATanResultValid, PiOverTwo, XEqualsZero);
			VectorType R3 = Vector::Select(R2, R1, YEqualsZero);
			VectorType R4 = Vector::Select(ThreePiOverFour, PiOverFour, XIsPositive);
			VectorType R5 = Vector::Select(PiOverTwo, R4, XEqualsInfinity);
			VectorType Result = Vector::Select(R3, R5, YEqualsInfinity);
			ATanResultValid = Vector::EqualInt(Result, ATanResultValid);

			VectorType Reciprocal = Vector(X).ReciprocalEst();
			VectorType V = Vector::Multiply(Y, Reciprocal);
			VectorType R0 = VectorATanEst(V);

			R1 = Vector::Select(Pi, Constants::NegativeZero, XIsPositive);
			R2 = Vector::Add(R0, R1);

			Result = Vector::Select(Result, R2, ATanResultValid);

			return Result;

#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorLerp
		(
			FVectorType V0,
			FVectorType V1,
			float    t
		)
		{
			// V0 + t * (V1 - V0)

#if defined(RAY_NO_INTRINSICS)

			VectorType Scale = Vector::Replicate(t);
			VectorType Length = Vector::Subtract(V1, V0);
			return VectorMultiplyAdd(Length, Scale, V0);

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType L = vsubq_f32(V1, V0);
			return vmlaq_n_f32(V0, L, t);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType L = _mm_sub_ps(V1, V0);
			VectorType S = _mm_set_ps1(t);
			VectorType Result = _mm_mul_ps(L, S);
			return _mm_add_ps(Result, V0);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorLerpV
		(
			FVectorType V0,
			FVectorType V1,
			FVectorType T
		)
		{
			// V0 + T * (V1 - V0)

#if defined(RAY_NO_INTRINSICS)

			VectorType Length = Vector::Subtract(V1, V0);
			return VectorMultiplyAdd(Length, T, V0);

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType L = vsubq_f32(V1, V0);
			return vmlaq_f32(V0, L, T);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType Length = _mm_sub_ps(V1, V0);
			VectorType Result = _mm_mul_ps(Length, T);
			return _mm_add_ps(Result, V0);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorHermite
		(
			FVectorType Position0,
			FVectorType Tangent0,
			FVectorType Position1,
			GVectorType Tangent1,
			float    t
		)
		{
			// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
			//          (t^3 - 2 * t^2 + t) * Tangent0 +
			//          (-2 * t^3 + 3 * t^2) * Position1 +
			//          (t^3 - t^2) * Tangent1

#if defined(RAY_NO_INTRINSICS)

			float t2 = t * t;
			float t3 = t * t2;

			VectorType P0 = Vector::Replicate(2.0f * t3 - 3.0f * t2 + 1.0f);
			VectorType T0 = Vector::Replicate(t3 - 2.0f * t2 + t);
			VectorType P1 = Vector::Replicate(-2.0f * t3 + 3.0f * t2);
			VectorType T1 = Vector::Replicate(t3 - t2);

			VectorType Result = Vector::Multiply(P0, Position0);
			Result = VectorMultiplyAdd(T0, Tangent0, Result);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(T1, Tangent1, Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float t2 = t * t;
			float t3 = t * t2;

			float p0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
			float t0 = t3 - 2.0f * t2 + t;
			float p1 = -2.0f * t3 + 3.0f * t2;
			float t1 = t3 - t2;

			VectorType vResult = vmulq_n_f32(Position0, p0);
			vResult = vmlaq_n_f32(vResult, Tangent0, t0);
			vResult = vmlaq_n_f32(vResult, Position1, p1);
			vResult = vmlaq_n_f32(vResult, Tangent1, t1);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			float t2 = t * t;
			float t3 = t * t2;

			VectorType P0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
			VectorType T0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
			VectorType P1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
			VectorType T1 = _mm_set_ps1(t3 - t2);

			VectorType vResult = _mm_mul_ps(P0, Position0);
			VectorType vTemp = _mm_mul_ps(T0, Tangent0);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = _mm_mul_ps(P1, Position1);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = _mm_mul_ps(T1, Tangent1);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorHermiteV
		(
			FVectorType Position0,
			FVectorType Tangent0,
			FVectorType Position1,
			GVectorType Tangent1,
			HVectorType T
		)
		{
			// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
			//          (t^3 - 2 * t^2 + t) * Tangent0 +
			//          (-2 * t^3 + 3 * t^2) * Position1 +
			//          (t^3 - t^2) * Tangent1

#if defined(RAY_NO_INTRINSICS)

			VectorType T2 = Vector::Multiply(T, T);
			VectorType T3 = Vector::Multiply(T, T2);

			VectorType P0 = Vector::Replicate(2.0f * T3.vector4_f32[0] - 3.0f * T2.vector4_f32[0] + 1.0f);
			VectorType T0 = Vector::Replicate(T3.vector4_f32[1] - 2.0f * T2.vector4_f32[1] + T.vector4_f32[1]);
			VectorType P1 = Vector::Replicate(-2.0f * T3.vector4_f32[2] + 3.0f * T2.vector4_f32[2]);
			VectorType T1 = Vector::Replicate(T3.vector4_f32[3] - T2.vector4_f32[3]);

			VectorType Result = Vector::Multiply(P0, Position0);
			Result = VectorMultiplyAdd(T0, Tangent0, Result);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(T1, Tangent1, Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
			static const VectorF32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

			VectorType T2 = vmulq_f32(T, T);
			VectorType T3 = vmulq_f32(T, T2);
			// Mul by the constants against t^2
			T2 = vmulq_f32(T2, CatMulT2);
			// Mul by the constants against t^3
			T3 = vmlaq_f32(T2, T3, CatMulT3);
			// T3 now has the pre-result.
			// I need to add t.y only
			T2 = vandq_u32(T, Constants::MaskY);
			T3 = vaddq_f32(T3, T2);
			// Add 1.0f to x
			T3 = vaddq_f32(T3, Constants::IdentityR0);
			// Now, I have the constants created
			// Mul the x constant to Position0
			VectorType vResult = vmulq_lane_f32(Position0, vget_low_f32(T3), 0); // T3[0]
			// Mul the y constant to Tangent0
			vResult = vmlaq_lane_f32(vResult, Tangent0, vget_low_f32(T3), 1); // T3[1]
			// Mul the z constant to Position1
			vResult = vmlaq_lane_f32(vResult, Position1, vget_high_f32(T3), 0); // T3[2]
			// Mul the w constant to Tangent1
			vResult = vmlaq_lane_f32(vResult, Tangent1, vget_high_f32(T3), 1); // T3[3]
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
			static const VectorF32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

			VectorType T2 = _mm_mul_ps(T, T);
			VectorType T3 = _mm_mul_ps(T, T2);
			// Mul by the constants against t^2
			T2 = _mm_mul_ps(T2, CatMulT2);
			// Mul by the constants against t^3
			T3 = _mm_mul_ps(T3, CatMulT3);
			// T3 now has the pre-result.
			T3 = _mm_add_ps(T3, T2);
			// I need to add t.y only
			T2 = _mm_and_ps(T, Constants::MaskY);
			T3 = _mm_add_ps(T3, T2);
			// Add 1.0f to x
			T3 = _mm_add_ps(T3, Constants::IdentityR0);
			// Now, I have the constants created
			// Mul the x constant to Position0
			VectorType vResult = RAYMATH_PERMUTE_PS(T3, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, Position0);
			// Mul the y constant to Tangent0
			T2 = RAYMATH_PERMUTE_PS(T3, _MM_SHUFFLE(1, 1, 1, 1));
			T2 = _mm_mul_ps(T2, Tangent0);
			vResult = _mm_add_ps(vResult, T2);
			// Mul the z constant to Position1
			T2 = RAYMATH_PERMUTE_PS(T3, _MM_SHUFFLE(2, 2, 2, 2));
			T2 = _mm_mul_ps(T2, Position1);
			vResult = _mm_add_ps(vResult, T2);
			// Mul the w constant to Tangent1
			T3 = RAYMATH_PERMUTE_PS(T3, _MM_SHUFFLE(3, 3, 3, 3));
			T3 = _mm_mul_ps(T3, Tangent1);
			vResult = _mm_add_ps(vResult, T3);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorCatmullRom
		(
			FVectorType Position0,
			FVectorType Position1,
			FVectorType Position2,
			GVectorType Position3,
			float    t
		)
		{
			// Result = ((-t^3 + 2 * t^2 - t) * Position0 +
			//           (3 * t^3 - 5 * t^2 + 2) * Position1 +
			//           (-3 * t^3 + 4 * t^2 + t) * Position2 +
			//           (t^3 - t^2) * Position3) * 0.5

#if defined(RAY_NO_INTRINSICS)

			float t2 = t * t;
			float t3 = t * t2;

			VectorType P0 = Vector::Replicate((-t3 + 2.0f * t2 - t) * 0.5f);
			VectorType P1 = Vector::Replicate((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
			VectorType P2 = Vector::Replicate((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
			VectorType P3 = Vector::Replicate((t3 - t2) * 0.5f);

			VectorType Result = Vector::Multiply(P0, Position0);
			Result = VectorMultiplyAdd(P1, Position1, Result);
			Result = VectorMultiplyAdd(P2, Position2, Result);
			Result = VectorMultiplyAdd(P3, Position3, Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float t2 = t * t;
			float t3 = t * t2;

			float p0 = (-t3 + 2.0f * t2 - t) * 0.5f;
			float p1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
			float p2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
			float p3 = (t3 - t2) * 0.5f;

			VectorType P1 = vmulq_n_f32(Position1, p1);
			VectorType P0 = vmlaq_n_f32(P1, Position0, p0);
			VectorType P3 = vmulq_n_f32(Position3, p3);
			VectorType P2 = vmlaq_n_f32(P3, Position2, p2);
			P0 = vaddq_f32(P0, P2);
			return P0;
#elif defined(RAY_SSE_INTRINSICS)
			float t2 = t * t;
			float t3 = t * t2;

			VectorType P0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
			VectorType P1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
			VectorType P2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
			VectorType P3 = _mm_set_ps1((t3 - t2) * 0.5f);

			P0 = _mm_mul_ps(P0, Position0);
			P1 = _mm_mul_ps(P1, Position1);
			P2 = _mm_mul_ps(P2, Position2);
			P3 = _mm_mul_ps(P3, Position3);
			P0 = _mm_add_ps(P0, P1);
			P2 = _mm_add_ps(P2, P3);
			P0 = _mm_add_ps(P0, P2);
			return P0;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorCatmullRomV
		(
			FVectorType Position0,
			FVectorType Position1,
			FVectorType Position2,
			GVectorType Position3,
			HVectorType T
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float fx = T.vector4_f32[0];
			float fy = T.vector4_f32[1];
			float fz = T.vector4_f32[2];
			float fw = T.vector4_f32[3];
			VectorF32 vResult = { { {
					0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * Position0.vector4_f32[0]
					+ (3 * fx * fx * fx - 5 * fx * fx + 2) * Position1.vector4_f32[0]
					+ (-3 * fx * fx * fx + 4 * fx * fx + fx) * Position2.vector4_f32[0]
					+ (fx * fx * fx - fx * fx) * Position3.vector4_f32[0]),

					0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * Position0.vector4_f32[1]
					+ (3 * fy * fy * fy - 5 * fy * fy + 2) * Position1.vector4_f32[1]
					+ (-3 * fy * fy * fy + 4 * fy * fy + fy) * Position2.vector4_f32[1]
					+ (fy * fy * fy - fy * fy) * Position3.vector4_f32[1]),

					0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * Position0.vector4_f32[2]
					+ (3 * fz * fz * fz - 5 * fz * fz + 2) * Position1.vector4_f32[2]
					+ (-3 * fz * fz * fz + 4 * fz * fz + fz) * Position2.vector4_f32[2]
					+ (fz * fz * fz - fz * fz) * Position3.vector4_f32[2]),

					0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * Position0.vector4_f32[3]
					+ (3 * fw * fw * fw - 5 * fw * fw + 2) * Position1.vector4_f32[3]
					+ (-3 * fw * fw * fw + 4 * fw * fw + fw) * Position2.vector4_f32[3]
					+ (fw * fw * fw - fw * fw) * Position3.vector4_f32[3])
				}
 } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
			static const VectorF32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
			static const VectorF32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
			static const VectorF32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
			// Cache T^2 and T^3
			VectorType T2 = vmulq_f32(T, T);
			VectorType T3 = vmulq_f32(T, T2);
			// Perform the Position0 term
			VectorType vResult = vaddq_f32(T2, T2);
			vResult = vsubq_f32(vResult, T);
			vResult = vsubq_f32(vResult, T3);
			vResult = vmulq_f32(vResult, Position0);
			// Perform the Position1 term and add
			VectorType vTemp = vmulq_f32(T3, Catmul3);
			vTemp = vmlsq_f32(vTemp, T2, Catmul5);
			vTemp = vaddq_f32(vTemp, Catmul2);
			vResult = vmlaq_f32(vResult, vTemp, Position1);
			// Perform the Position2 term and add
			vTemp = vmulq_f32(T2, Catmul4);
			vTemp = vmlsq_f32(vTemp, T3, Catmul3);
			vTemp = vaddq_f32(vTemp, T);
			vResult = vmlaq_f32(vResult, vTemp, Position2);
			// Position3 is the last term
			T3 = vsubq_f32(T3, T2);
			vResult = vmlaq_f32(vResult, T3, Position3);
			// Multiply by 0.5f and exit
			vResult = vmulq_f32(vResult, Constants::OneHalf);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
			static const VectorF32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
			static const VectorF32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
			static const VectorF32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
			// Cache T^2 and T^3
			VectorType T2 = _mm_mul_ps(T, T);
			VectorType T3 = _mm_mul_ps(T, T2);
			// Perform the Position0 term
			VectorType vResult = _mm_add_ps(T2, T2);
			vResult = _mm_sub_ps(vResult, T);
			vResult = _mm_sub_ps(vResult, T3);
			vResult = _mm_mul_ps(vResult, Position0);
			// Perform the Position1 term and add
			VectorType vTemp = _mm_mul_ps(T3, Catmul3);
			VectorType vTemp2 = _mm_mul_ps(T2, Catmul5);
			vTemp = _mm_sub_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, Catmul2);
			vTemp = _mm_mul_ps(vTemp, Position1);
			vResult = _mm_add_ps(vResult, vTemp);
			// Perform the Position2 term and add
			vTemp = _mm_mul_ps(T2, Catmul4);
			vTemp2 = _mm_mul_ps(T3, Catmul3);
			vTemp = _mm_sub_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, T);
			vTemp = _mm_mul_ps(vTemp, Position2);
			vResult = _mm_add_ps(vResult, vTemp);
			// Position3 is the last term
			T3 = _mm_sub_ps(T3, T2);
			T3 = _mm_mul_ps(T3, Position3);
			vResult = _mm_add_ps(vResult, T3);
			// Multiply by 0.5f and exit
			vResult = _mm_mul_ps(vResult, Constants::OneHalf);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorBaryCentric
		(
			FVectorType Position0,
			FVectorType Position1,
			FVectorType Position2,
			float    f,
			float    g
		)
		{
			// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(RAY_NO_INTRINSICS)

			VectorType P10 = Vector::Subtract(Position1, Position0);
			VectorType ScaleF = Vector::Replicate(f);

			VectorType P20 = Vector::Subtract(Position2, Position0);
			VectorType ScaleG = Vector::Replicate(g);

			VectorType Result = VectorMultiplyAdd(P10, ScaleF, Position0);
			Result = VectorMultiplyAdd(P20, ScaleG, Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType R1 = vsubq_f32(Position1, Position0);
			VectorType R2 = vsubq_f32(Position2, Position0);
			R1 = vmlaq_n_f32(Position0, R1, f);
			return vmlaq_n_f32(R1, R2, g);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType R1 = _mm_sub_ps(Position1, Position0);
			VectorType SF = _mm_set_ps1(f);
			VectorType R2 = _mm_sub_ps(Position2, Position0);
			VectorType SG = _mm_set_ps1(g);
			R1 = _mm_mul_ps(R1, SF);
			R2 = _mm_mul_ps(R2, SG);
			R1 = _mm_add_ps(R1, Position0);
			R1 = _mm_add_ps(R1, R2);
			return R1;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV VectorBaryCentricV
		(
			FVectorType Position0,
			FVectorType Position1,
			FVectorType Position2,
			GVectorType F,
			HVectorType G
		)
		{
			// Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)

#if defined(RAY_NO_INTRINSICS)

			VectorType P10 = Vector::Subtract(Position1, Position0);
			VectorType P20 = Vector::Subtract(Position2, Position0);

			VectorType Result = VectorMultiplyAdd(P10, F, Position0);
			Result = VectorMultiplyAdd(P20, G, Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType R1 = vsubq_f32(Position1, Position0);
			VectorType R2 = vsubq_f32(Position2, Position0);
			R1 = vmlaq_f32(Position0, R1, F);
			return vmlaq_f32(R1, R2, G);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType R1 = _mm_sub_ps(Position1, Position0);
			VectorType R2 = _mm_sub_ps(Position2, Position0);
			R1 = _mm_mul_ps(R1, F);
			R2 = _mm_mul_ps(R2, G);
			R1 = _mm_add_ps(R1, Position0);
			R1 = _mm_add_ps(R1, R2);
			return R1;
#endif
		}

		/****************************************************************************
		 *
		 * 2D VectorType
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2Equal
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}


		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector2EqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2EqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector2EqualIntR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;
			if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] == V2.vector4_u32[1]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] != V2.vector4_u32[1]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2NearEqual
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType Epsilon
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			float dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			return ((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]));
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t vDelta = vsub_f32(vget_low_u32(V1), vget_low_u32(V2));
			uint32x2_t vTemp = vacle_f32(vDelta, vget_low_u32(Epsilon));
			uint64_t r = vget_lane_u64(vTemp, 0);
			return (r == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Get the difference
			VectorType vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			VectorType vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 0x3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2NotEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) != 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2NotEqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vceq_u32(vget_low_u32(V1), vget_low_u32(V2));
			return (vget_lane_u64(vTemp, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) != 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2Greater
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vcgt_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			// z and w are don't care
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector2GreaterR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] > V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] <= V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vcgt_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2GreaterOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vcge_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector2GreaterOrEqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vcge_f32(vget_low_f32(V1), vget_low_f32(V2));
			uint64_t r = vget_lane_u64(vTemp, 0);
			uint32_t CR = 0;
			if (r == 0xFFFFFFFFFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 3;
			uint32_t CR = 0;
			if (iTest == 3)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2Less
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vclt_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmplt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2LessOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x2_t vTemp = vcle_f32(vget_low_f32(V1), vget_low_f32(V2));
			return (vget_lane_u64(vTemp, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmple_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2InBounds
		(
			FVectorType V,
			FVectorType Bounds
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			float32x2_t B = vget_low_f32(Bounds);
			// Test if less than or equal
			uint32x2_t ivTemp1 = vcle_f32(VL, B);
			// Negate the bounds
			float32x2_t vTemp2 = vneg_f32(B);
			// Test if greater or equal (Reversed)
			uint32x2_t ivTemp2 = vcle_f32(vTemp2, VL);
			// Blend answers
			ivTemp1 = vand_u32(ivTemp1, ivTemp2);
			// x and y in bounds?
			return (vget_lane_u64(ivTemp1, 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			VectorType vTemp2 = _mm_mul_ps(Bounds, Constants::NegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// x and y in bounds? (z and w are don't care)
			return (((_mm_movemask_ps(vTemp1) & 0x3) == 0x3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2IsNaN
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (RAYMATH_ISNAN(V.vector4_f32[0]) ||
				RAYMATH_ISNAN(V.vector4_f32[1]));
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Test against itself. NaN is always not equal
			uint32x2_t vTempNan = vceq_f32(VL, VL);
			// If x or y are NaN, the mask is zero
			return (vget_lane_u64(vTempNan, 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test against itself. NaN is always not equal
			VectorType vTempNan = _mm_cmpneq_ps(V, V);
			// If x or y are NaN, the mask is non-zero
			return ((_mm_movemask_ps(vTempNan) & 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector2IsInfinite
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			return (RAYMATH_ISINF(V.vector4_f32[0]) ||
				RAYMATH_ISINF(V.vector4_f32[1]));
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Mask off the sign bit
			uint32x2_t vTemp = vand_u32(vget_low_f32(V), vget_low_f32(Constants::AbsMask));
			// Compare to infinity
			vTemp = vceq_f32(vTemp, vget_low_f32(Constants::Infinity));
			// If any are infinity, the signs are true.
			return vget_lane_u64(vTemp, 0) != 0;
#elif defined(RAY_SSE_INTRINSICS)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(V, Constants::AbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, Constants::Infinity);
			// If x or z are infinity, the signs are true.
			return ((_mm_movemask_ps(vTemp) & 3) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Dot
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1];
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Perform the dot product on x and y
			float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vget_low_f32(V2));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_dp_ps(V1, V2, 0x3f);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vDot = _mm_mul_ps(V1, V2);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_moveldup_ps(vDot);
			return vDot;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V1, V2);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Cross
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			// [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]

#if defined(RAY_NO_INTRINSICS)
			float fCross = (V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]);
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fCross;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Negate = { { { 1.f, -1.f, 0, 0 } } };

			float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2)));
			vTemp = vmul_f32(vTemp, vget_low_f32(Negate));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(RAY_SSE_INTRINSICS)
	// Swap x and y
			VectorType vResult = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 1, 0, 1));
			// Perform the muls
			vResult = _mm_mul_ps(vResult, V1);
			// Splat y
			VectorType vTemp = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
			// Sub the values
			vResult = _mm_sub_ss(vResult, vTemp);
			// Splat the cross product
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 0, 0, 0));
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2LengthSq
		(
			FVectorType V
		)
		{
			return Vector2Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2ReciprocalLengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = Result.ReciprocalSqrtEst();
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt (estimate)
			vTemp = vrsqrte_f32(vTemp);
			return vcombine_f32(vTemp, vTemp);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_rsqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ss(vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2ReciprocalLength
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = Result.ReciprocalSqrt();
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(vTemp);
			float32x2_t  P0 = vmul_f32(vTemp, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(vTemp, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x3f);
			VectorType vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(Constants::One, vLengthSq);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = _mm_div_ss(Constants::One, vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_sqrt_ss(vLengthSq);
			vLengthSq = _mm_div_ss(Constants::One, vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2LengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = Result.SqrtEst();
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(vTemp);
			Result = vmul_f32(vTemp, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_sqrt_ss(vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Length
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;
			Result = Vector2LengthSq(V);
			Result = Result.Sqrt();
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(vTemp, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x3f);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ss(vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector2NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline VectorType RAYMATH_CALLCONV Vector2NormalizeEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Result;
			Result = Vector2ReciprocalLength(V);
			Result = Vector::Multiply(V, Result);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			// Reciprocal sqrt (estimate)
			vTemp = vrsqrte_f32(vTemp);
			// Normalize
			float32x2_t Result = vmul_f32(VL, vTemp);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x3f);
			VectorType vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_mul_ps(vLengthSq, V);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has y splatted
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			// x+y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_rsqrt_ss(vLengthSq);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_mul_ps(vLengthSq, V);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Normalize
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType vResult = Vector2Length(V);
			float fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(vTemp, vget_low_f32(Constants::Infinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			vTemp = vmul_f32(S1, R1);
			// Normalize
			float32x2_t Result = vmul_f32(VL, vTemp);
			Result = vbsl_f32(VEqualsZero, vdup_n_f32(0), Result);
			Result = vbsl_f32(VEqualsInf, vget_low_f32(Constants::QNaN), Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vLengthSq = _mm_dp_ps(V, V, 0x3f);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE3_INTRINSICS)
			// Perform the dot product on x and y only
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_moveldup_ps(vLengthSq);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x and y only
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2ClampLength
		(
			FVectorType V,
			float    LengthMin,
			float    LengthMax
		)
		{
			VectorType ClampMax = Vector::Replicate(LengthMax);
			VectorType ClampMin = Vector::Replicate(LengthMin);
			return Vector2ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2ClampLengthV
		(
			FVectorType V,
			FVectorType LengthMin,
			FVectorType LengthMax
		)
		{
			assert((Vector(LengthMin).GetY() == Vector(LengthMin).GetX()));
			assert((Vector(LengthMax).GetY() == Vector(LengthMax).GetX()));
			assert(Vector2GreaterOrEqual(LengthMin, Constants::Zero));
			assert(Vector2GreaterOrEqual(LengthMax, Constants::Zero));
			assert(Vector2GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector2LengthSq(V);

			const VectorType Zero = Vector::Zero();

			VectorType RcpLength = LengthSq.ReciprocalSqrt();

			VectorType InfiniteLength = Vector::EqualInt(LengthSq, Constants::Infinity.v);
			VectorType ZeroLength = Vector::Equal(LengthSq, Zero);

			Vector Length = Vector::Multiply(LengthSq, RcpLength);

			VectorType Normal = Vector::Multiply(V, RcpLength);

			VectorType Select = Vector::EqualInt(InfiniteLength, ZeroLength);
			Length = Vector::Select(LengthSq, Length, Select);
			Normal = Vector::Select(LengthSq, Normal, Select);

			VectorType ControlMax = Length.Greater(LengthMax);
			VectorType ControlMin = Length.Less(LengthMin);

			VectorType ClampLength = Vector::Select(Length, LengthMax, ControlMax);
			ClampLength = Vector::Select(ClampLength, LengthMin, ControlMin);

			VectorType Result = Vector::Multiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			VectorType Control = Vector::EqualInt(ControlMax, ControlMin);
			Result = Vector::Select(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Reflect
		(
			FVectorType Incident,
			FVectorType Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			VectorType Result;
			Result = Vector2Dot(Incident, Normal);
			Result = Vector::Add(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Refract
		(
			FVectorType Incident,
			FVectorType Normal,
			float    RefractionIndex
		)
		{
			VectorType Index = Vector::Replicate(RefractionIndex);
			return Vector2RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		// Return the refraction of a 2D vector
		inline VectorType RAYMATH_CALLCONV Vector2RefractV
		(
			FVectorType Incident,
			FVectorType Normal,
			FVectorType RefractionIndex
		)
		{
			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(RAY_NO_INTRINSICS)

			float IDotN = (Incident.vector4_f32[0] * Normal.vector4_f32[0]) + (Incident.vector4_f32[1] * Normal.vector4_f32[1]);
			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float RY = 1.0f - (IDotN * IDotN);
			float RX = 1.0f - (RY * RefractionIndex.vector4_f32[0] * RefractionIndex.vector4_f32[0]);
			RY = 1.0f - (RY * RefractionIndex.vector4_f32[1] * RefractionIndex.vector4_f32[1]);
			if (RX >= 0.0f) {
				RX = (RefractionIndex.vector4_f32[0] * Incident.vector4_f32[0]) - (Normal.vector4_f32[0] * ((RefractionIndex.vector4_f32[0] * IDotN) + sqrtf(RX)));
			}
			else {
				RX = 0.0f;
			}
			if (RY >= 0.0f) {
				RY = (RefractionIndex.vector4_f32[1] * Incident.vector4_f32[1]) - (Normal.vector4_f32[1] * ((RefractionIndex.vector4_f32[1] * IDotN) + sqrtf(RY)));
			}
			else {
				RY = 0.0f;
			}

			VectorType vResult;
			vResult.vector4_f32[0] = RX;
			vResult.vector4_f32[1] = RY;
			vResult.vector4_f32[2] = 0.0f;
			vResult.vector4_f32[3] = 0.0f;
			return vResult;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t IL = vget_low_f32(Incident);
			float32x2_t NL = vget_low_f32(Normal);
			float32x2_t RIL = vget_low_f32(RefractionIndex);
			// Get the 2D Dot product of Incident-Normal
			float32x2_t vTemp = vmul_f32(IL, NL);
			float32x2_t IDotN = vpadd_f32(vTemp, vTemp);
			// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			vTemp = vmls_f32(vget_low_f32(Constants::One), IDotN, IDotN);
			vTemp = vmul_f32(vTemp, RIL);
			vTemp = vmls_f32(vget_low_f32(Constants::One), vTemp, RIL);
			// If any terms are <=0, sqrt() will fail, punt to zero
			uint32x2_t vMask = vcgt_f32(vTemp, vget_low_f32(Constants::Zero));
			// Sqrt(vTemp)
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t S2 = vmul_f32(S1, R1);
			vTemp = vmul_f32(vTemp, S2);
			// R = RefractionIndex * IDotN + sqrt(R)
			vTemp = vmla_f32(vTemp, RIL, IDotN);
			// Result = RefractionIndex * Incident - Normal * R
			float32x2_t vResult = vmul_f32(RIL, IL);
			vResult = vmls_f32(vResult, vTemp, NL);
			vResult = vand_u32(vResult, vMask);
			return vcombine_f32(vResult, vResult);
#elif defined(RAY_SSE_INTRINSICS)
	// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
	// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
	// Get the 2D Dot product of Incident-Normal
			VectorType IDotN = Vector2Dot(Incident, Normal);
			// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			VectorType vTemp = _mm_mul_ps(IDotN, IDotN);
			vTemp = _mm_sub_ps(Constants::One, vTemp);
			vTemp = _mm_mul_ps(vTemp, RefractionIndex);
			vTemp = _mm_mul_ps(vTemp, RefractionIndex);
			vTemp = _mm_sub_ps(Constants::One, vTemp);
			// If any terms are <=0, sqrt() will fail, punt to zero
			VectorType vMask = _mm_cmpgt_ps(vTemp, Constants::Zero);
			// R = RefractionIndex * IDotN + sqrt(R)
			vTemp = _mm_sqrt_ps(vTemp);
			VectorType vResult = _mm_mul_ps(RefractionIndex, IDotN);
			vTemp = _mm_add_ps(vTemp, vResult);
			// Result = RefractionIndex * Incident - Normal * R
			vResult = _mm_mul_ps(RefractionIndex, Incident);
			vTemp = _mm_mul_ps(vTemp, Normal);
			vResult = _mm_sub_ps(vResult, vTemp);
			vResult = _mm_and_ps(vResult, vMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Orthogonal
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					-V.vector4_f32[1],
					V.vector4_f32[0],
					0.f,
					0.f
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Negate = { { { -1.f, 1.f, 0, 0 } } };
			const float32x2_t zero = vdup_n_f32(0);

			float32x2_t VL = vget_low_f32(V);
			float32x2_t Result = vmul_f32(vrev64_f32(VL), vget_low_f32(Negate));
			return vcombine_f32(Result, zero);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
			vResult = _mm_mul_ps(vResult, Constants::NegateX);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2AngleBetweenNormalsEst
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector2Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne.v, Constants::One.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2AngleBetweenNormals
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector2Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne, Constants::One);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2AngleBetweenVectors
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			VectorType L1 = Vector2ReciprocalLength(V1);
			VectorType L2 = Vector2ReciprocalLength(V2);

			VectorType Dot = Vector2Dot(V1, V2);

			L1 = Vector::Multiply(L1, L2);

			Vector CosAngle = Vector::Multiply(Dot, L1);
			CosAngle = CosAngle.Clamp(Constants::NegativeOne.v, Constants::One.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2LinePointDistance
		(
			FVectorType LinePoint1,
			FVectorType LinePoint2,
			FVectorType Point
		)
		{
			// Given a vector PointVector from LinePoint1 to Point and a vector
			// LineVector from LinePoint1 to LinePoint2, the scaled distance 
			// PointProjectionScale from LinePoint1 to the perpendicular projection
			// of PointVector onto the line is defined as:
			//
			//     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

			VectorType PointVector = Vector::Subtract(Point, LinePoint1);
			VectorType LineVector = Vector::Subtract(LinePoint2, LinePoint1);

			VectorType LengthSq = Vector2LengthSq(LineVector);

			VectorType PointProjectionScale = Vector2Dot(PointVector, LineVector);
			PointProjectionScale = Vector::Divide(PointProjectionScale, LengthSq);

			VectorType DistanceVector = Vector::Multiply(LineVector, PointProjectionScale);
			DistanceVector = Vector::Subtract(PointVector, DistanceVector);

			return Vector2Length(DistanceVector);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2IntersectLine
		(
			FVectorType Line1Point1,
			FVectorType Line1Point2,
			FVectorType Line2Point1,
			GVectorType Line2Point2
		)
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			VectorType V1 = Vector::Subtract(Line1Point2, Line1Point1);
			VectorType V2 = Vector::Subtract(Line2Point2, Line2Point1);
			VectorType V3 = Vector::Subtract(Line1Point1, Line2Point1);

			Vector C1 = Vector2Cross(V1, V2);
			VectorType C2 = Vector2Cross(V2, V3);

			VectorType Result;
			const VectorType Zero = Vector::Zero();
			if (Vector2NearEqual(C1, Zero, Constants::Epsilon.v))
			{
				if (Vector2NearEqual(C2, Zero, Constants::Epsilon.v))
				{
					// Coincident
					Result = Constants::Infinity.v;
				}
				else
				{
					// Parallel
					Result = Constants::QNaN.v;
				}
			}
			else
			{
				// Intersection point = Line1Point1 + V1 * (C2 / C1)
				VectorType Scale = C1.Reciprocal();
				Scale = Vector::Multiply(C2, Scale);
				Result = VectorMultiplyAdd(V1, Scale, Line1Point1);
			}

			return Result;

#elif defined(RAY_SSE_INTRINSICS)
			VectorType V1 = _mm_sub_ps(Line1Point2, Line1Point1);
			VectorType V2 = _mm_sub_ps(Line2Point2, Line2Point1);
			VectorType V3 = _mm_sub_ps(Line1Point1, Line2Point1);
			// Generate the cross products
			VectorType C1 = Vector2Cross(V1, V2);
			VectorType C2 = Vector2Cross(V2, V3);
			// If C1 is not close to epsilon, use the calculated value
			VectorType vResultMask = _mm_setzero_ps();
			vResultMask = _mm_sub_ps(vResultMask, C1);
			vResultMask = _mm_max_ps(vResultMask, C1);
			// 0xFFFFFFFF if the calculated value is to be used
			vResultMask = _mm_cmpgt_ps(vResultMask, Constants::Epsilon);
			// If C1 is close to epsilon, which fail type is it? INFINITY or NAN?
			VectorType vFailMask = _mm_setzero_ps();
			vFailMask = _mm_sub_ps(vFailMask, C2);
			vFailMask = _mm_max_ps(vFailMask, C2);
			vFailMask = _mm_cmple_ps(vFailMask, Constants::Epsilon);
			VectorType vFail = _mm_and_ps(vFailMask, Constants::Infinity);
			vFailMask = _mm_andnot_ps(vFailMask, Constants::QNaN);
			// vFail is NAN or INF
			vFail = _mm_or_ps(vFail, vFailMask);
			// Intersection point = Line1Point1 + V1 * (C2 / C1)
			VectorType vResult = _mm_div_ps(C2, C1);
			vResult = _mm_mul_ps(vResult, V1);
			vResult = _mm_add_ps(vResult, Line1Point1);
			// Use result, or failure value
			vResult = _mm_and_ps(vResult, vResultMask);
			vResultMask = _mm_andnot_ps(vResultMask, vFail);
			vResult = _mm_or_ps(vResult, vResultMask);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2Transform
		(
			FVectorType V,
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = VectorMultiplyAdd(Y, M.r[1], M.r[3]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			float32x4_t Result = vmlaq_lane_f32(M.r[3], M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(Result, M.r[0], VL, 0); // X
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			VectorType vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vResult = _mm_add_ps(vResult, M.r[3]);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------


		inline Float4* RAYMATH_CALLCONV Vector2TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			assert(OutputStride >= sizeof(Float4));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = VectorMultiplyAdd(Y, row1, row3);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						VectorType vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						VectorType vResult3 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						float32x4x4_t R;
						R.val[0] = vResult0;
						R.val[1] = vResult1;
						R.val[2] = vResult2;
						R.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), R);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				VectorType vResult = vmlaq_lane_f32(row3, row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
					{
						// Packed input, aligned output
						for (size_t j = 0; j < two; ++j)
						{
							VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Y, row1);
							VectorType vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
					else
					{
						// Packed input, unaligned output
						for (size_t j = 0; j < two; ++j)
						{
							VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Y, row1);
							VectorType vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
				{
					// Aligned input, aligned output
					for (; i < VectorCount; i++)
					{
						VectorType V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
						pInputVector += InputStride;

						VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

						VectorType vTemp = _mm_mul_ps(Y, row1);
						VectorType vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);

						RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Aligned input, unaligned output
					for (; i < VectorCount; i++)
					{
						VectorType V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
						pInputVector += InputStride;

						VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

						VectorType vTemp = _mm_mul_ps(Y, row1);
						VectorType vTemp2 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, row3);
						vTemp = _mm_add_ps(vTemp, vTemp2);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
						pOutputVector += OutputStride;
					}
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					VectorType Y = RAYMATH_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					VectorType X = RAYMATH_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Y, row1);
					VectorType vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2TransformCoord
		(
			FVectorType V,
			FMatrix M
		)
		{
			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = VectorMultiplyAdd(Y, M.r[1], M.r[3]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			VectorType W = Vector::SplatW(Result);
			return Vector::Divide(Result, W);
		}

		//------------------------------------------------------------------------------


		inline Float2* RAYMATH_CALLCONV Vector2TransformCoordStream
		(
			Float2* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			assert(OutputStride >= sizeof(Float2));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = VectorMultiplyAdd(Y, row1, row3);
				Result = VectorMultiplyAdd(X, row0, Result);

				VectorType W = Vector::SplatW(Result);

				Result = Vector::Divide(Result, W);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat2(reinterpret_cast<Float2*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float2)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						VectorType W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
#endif

						vst2q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float2) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				VectorType vResult = vmlaq_lane_f32(row3, row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				V = vget_high_f32(vResult);
				float32x2_t W = vdup_lane_f32(V, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
				V = vget_low_f32(vResult);
				V = vdiv_f32(V, W);
#else
				// 2 iterations of Newton-Raphson refinement of reciprocal for W
				float32x2_t Reciprocal = vrecpe_f32(W);
				float32x2_t S = vrecps_f32(Reciprocal, W);
				Reciprocal = vmul_f32(S, Reciprocal);
				S = vrecps_f32(Reciprocal, W);
				Reciprocal = vmul_f32(S, Reciprocal);

				V = vget_low_f32(vResult);
				V = vmul_f32(V, Reciprocal);
#endif

				vst1_f32(reinterpret_cast<float*>(pOutputVector), V);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (OutputStride == sizeof(Float2))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Y, row1);
								VectorType vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								VectorType V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								VectorType V2 = _mm_div_ps(vTemp, W);

								vTemp = _mm_movelh_ps(V1, V2);

								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Y, row1);
								VectorType vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								VectorType V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								VectorType V2 = _mm_div_ps(vTemp, W);

								vTemp = _mm_movelh_ps(V1, V2);

								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < two; ++j)
						{
							VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							// Result 1
							VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Y, row1);
							VectorType vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							// Result 2
							Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				// Aligned input
				for (; i < VectorCount; i++)
				{
					VectorType V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
					pInputVector += InputStride;

					VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Y, row1);
					VectorType vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					VectorType Y = RAYMATH_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					VectorType X = RAYMATH_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Y, row1);
					VectorType vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);

					VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

					vTemp = _mm_div_ps(vTemp, W);
					vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector2TransformNormal
		(
			FVectorType V,
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = Vector::Multiply(Y, M.r[1]);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			float32x4_t Result = vmulq_lane_f32(M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(Result, M.r[0], VL, 0); // X
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			VectorType vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------


		inline Float2* RAYMATH_CALLCONV Vector2TransformNormalStream
		(
			Float2* pOutputStream,
			size_t          OutputStride,
			const Float2* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float2));
			assert(OutputStride >= sizeof(Float2));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat2(reinterpret_cast<const Float2*>(pInputVector));
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = Vector::Multiply(Y, row1);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat2(reinterpret_cast<Float2*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float2)) && (OutputStride == sizeof(Float2)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x2_t V = vld2q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float2) * 4;

						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						VectorType vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);
						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));
						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						V.val[0] = vResult0;
						V.val[1] = vResult1;

						vst2q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float2) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t V = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				VectorType vResult = vmulq_lane_f32(row0, V, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, V, 1); // Y

				V = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), V);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];

			size_t i = 0;
			size_t two = VectorCount >> 1;
			if (two > 0)
			{
				if (InputStride == sizeof(Float2))
				{
					if (OutputStride == sizeof(Float2))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Y, row1);
								VectorType vTemp2 = _mm_mul_ps(X, row0);
								VectorType V1 = _mm_add_ps(vTemp, vTemp2);

								// Result 2
								Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								VectorType V2 = _mm_add_ps(vTemp, vTemp2);

								vTemp = _mm_movelh_ps(V1, V2);

								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < two; ++j)
							{
								VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								pInputVector += sizeof(Float2) * 2;

								// Result 1
								VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Y, row1);
								VectorType vTemp2 = _mm_mul_ps(X, row0);
								VectorType V1 = _mm_add_ps(vTemp, vTemp2);

								// Result 2
								Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
								X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

								vTemp = _mm_mul_ps(Y, row1);
								vTemp2 = _mm_mul_ps(X, row0);
								VectorType V2 = _mm_add_ps(vTemp, vTemp2);

								vTemp = _mm_movelh_ps(V1, V2);

								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
								pOutputVector += sizeof(Float2) * 2;

								i += 2;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < two; ++j)
						{
							VectorType V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							pInputVector += sizeof(Float2) * 2;

							// Result 1
							VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Y, row1);
							VectorType vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							// Result 2
							Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
							X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));

							vTemp = _mm_mul_ps(Y, row1);
							vTemp2 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

							_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
							_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
							pOutputVector += OutputStride;

							i += 2;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pInputVector) & 0xF) && !(InputStride & 0xF))
			{
				// Aligned input
				for (; i < VectorCount; i++)
				{
					VectorType V = _mm_castsi128_ps(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(pInputVector)));
					pInputVector += InputStride;

					VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Y, row1);
					VectorType vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned input
				for (; i < VectorCount; i++)
				{
					__m128 x = _mm_load_ss(reinterpret_cast<const float*>(pInputVector));
					__m128 y = _mm_load_ss(reinterpret_cast<const float*>(pInputVector + 4));
					pInputVector += InputStride;

					VectorType Y = RAYMATH_PERMUTE_PS(y, _MM_SHUFFLE(0, 0, 0, 0));
					VectorType X = RAYMATH_PERMUTE_PS(x, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Y, row1);
					VectorType vTemp2 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp2 = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));

					_mm_store_ss(reinterpret_cast<float*>(pOutputVector), vTemp);
					_mm_store_ss(reinterpret_cast<float*>(pOutputVector + 4), vTemp2);
					pOutputVector += OutputStride;
				}
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

		/****************************************************************************
		 *
		 * 3D VectorType
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3Equal
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector3EqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] == V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] != V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp) & 7;
			uint32_t CR = 0;
			if (iTest == 7)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3EqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector3EqualIntR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if ((V1.vector4_u32[0] == V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] == V2.vector4_u32[1]) &&
				(V1.vector4_u32[2] == V2.vector4_u32[2]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_u32[0] != V2.vector4_u32[0]) &&
				(V1.vector4_u32[1] != V2.vector4_u32[1]) &&
				(V1.vector4_u32[2] != V2.vector4_u32[2]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTemp = _mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7;
			uint32_t CR = 0;
			if (iTemp == 7)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTemp)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3NearEqual
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType Epsilon
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float dx, dy, dz;

			dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			dz = fabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
			return (((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]) &&
				(dz <= Epsilon.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vDelta = vsubq_f32(V1, V2);
			uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Get the difference
			VectorType vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			VectorType vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			// w is don't care
			return (((_mm_movemask_ps(vTemp) & 7) == 0x7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3NotEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) != 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3NotEqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) != 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3Greater
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector3GreaterR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] > V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] > V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] > V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] <= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] <= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] <= V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp) & 7;
			if (iTest == 7)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3GreaterOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector3GreaterOrEqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] >= V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] < V2.vector4_f32[2]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU;

			uint32_t CR = 0;
			if (r == 0xFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			uint32_t CR = 0;
			int iTest = _mm_movemask_ps(vTemp) & 7;
			if (iTest == 7)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3Less
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcltq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmplt_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3LessOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcleq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmple_ps(V1, V2);
			return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3InBounds
		(
			FVectorType V,
			FVectorType Bounds
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
				(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test if less than or equal
			uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			float32x4_t vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
			// in bounds?
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(ivTemp1), vget_high_u8(ivTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			VectorType vTemp2 = _mm_mul_ps(Bounds, Constants::NegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// x,y and z in bounds? (w is don't care)
			return (((_mm_movemask_ps(vTemp1) & 0x7) == 0x7) != 0);
#else
			return ComparisonAllInBounds(Vector3InBoundsR(V, Bounds));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3IsNaN
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			return (RAYMATH_ISNAN(V.vector4_f32[0]) ||
				RAYMATH_ISNAN(V.vector4_f32[1]) ||
				RAYMATH_ISNAN(V.vector4_f32[2]));

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(V, V);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempNan), vget_high_u8(vTempNan));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			// If x or y or z are NaN, the mask is zero
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test against itself. NaN is always not equal
			VectorType vTempNan = _mm_cmpneq_ps(V, V);
			// If x or y or z are NaN, the mask is non-zero
			return ((_mm_movemask_ps(vTempNan) & 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector3IsInfinite
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (RAYMATH_ISINF(V.vector4_f32[0]) ||
				RAYMATH_ISINF(V.vector4_f32[1]) ||
				RAYMATH_ISINF(V.vector4_f32[2]));
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Mask off the sign bit
			uint32x4_t vTempInf = vandq_u32(V, Constants::AbsMask);
			// Compare to infinity
			vTempInf = vceqq_f32(vTempInf, Constants::Infinity);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempInf), vget_high_u8(vTempInf));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return ((vget_lane_u32(vTemp.val[1], 1) & 0xFFFFFFU) != 0);
#elif defined(RAY_SSE_INTRINSICS)
			// Mask off the sign bit
			__m128 vTemp = _mm_and_ps(V, Constants::AbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, Constants::Infinity);
			// If x,y or z are infinity, the signs are true.
			return ((_mm_movemask_ps(vTemp) & 7) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Dot
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float fValue = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2];
			VectorF32 vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fValue;
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vTemp = vmulq_f32(V1, V2);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			return vcombine_f32(v1, v1);
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_dp_ps(V1, V2, 0x7f);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vTemp = _mm_mul_ps(V1, V2);
			vTemp = _mm_and_ps(vTemp, Constants::Mask3);
			vTemp = _mm_hadd_ps(vTemp, vTemp);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product
			VectorType vDot = _mm_mul_ps(V1, V2);
			// x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
			VectorType vTemp = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.vector4_f32[0] = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.vector4_f32[2]
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.vector4_f32[0] = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			return RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Cross
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]

#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { {
					(V1.vector4_f32[1] * V2.vector4_f32[2]) - (V1.vector4_f32[2] * V2.vector4_f32[1]),
					(V1.vector4_f32[2] * V2.vector4_f32[0]) - (V1.vector4_f32[0] * V2.vector4_f32[2]),
					(V1.vector4_f32[0] * V2.vector4_f32[1]) - (V1.vector4_f32[1] * V2.vector4_f32[0]),
					0.0f
				}
 }
			};
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t v1xy = vget_low_f32(V1);
			float32x2_t v2xy = vget_low_f32(V2);

			float32x2_t v1yx = vrev64_f32(v1xy);
			float32x2_t v2yx = vrev64_f32(v2xy);

			float32x2_t v1zz = vdup_lane_f32(vget_high_f32(V1), 0);
			float32x2_t v2zz = vdup_lane_f32(vget_high_f32(V2), 0);

			VectorType vResult = vmulq_f32(vcombine_f32(v1yx, v1xy), vcombine_f32(v2zz, v2yx));
			vResult = vmlsq_f32(vResult, vcombine_f32(v1zz, v1yx), vcombine_f32(v2yx, v2xy));
			vResult = veorq_u32(vResult, Constants::FlipY);
			return vandq_u32(vResult, Constants::Mask3);
#elif defined(RAY_SSE_INTRINSICS)
	// y1,z1,x1,w1
			VectorType vTemp1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(3, 0, 2, 1));
			// z2,x2,y2,w2
			VectorType vTemp2 = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(3, 1, 0, 2));
			// Perform the left operation
			VectorType vResult = _mm_mul_ps(vTemp1, vTemp2);
			// z1,x1,y1,w1
			vTemp1 = RAYMATH_PERMUTE_PS(vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
			// y2,z2,x2,w2
			vTemp2 = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
			// Perform the right operation
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			// Subract the right from left, and return answer
			vResult = _mm_sub_ps(vResult, vTemp1);
			// Set w to zero
			return _mm_and_ps(vResult, Constants::Mask3);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3LengthSq
		(
			FVectorType V
		)
		{
			return Vector3Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3ReciprocalLengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = Result.ReciprocalSqrtEst();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			return vcombine_f32(v2, v2);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_rsqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, Constants::Mask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y and z
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3ReciprocalLength
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = Result.ReciprocalSqrt();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(v1);
			float32x2_t  P0 = vmul_f32(v1, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(v1, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x7f);
			VectorType vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(Constants::One, vLengthSq);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vDot = _mm_mul_ps(V, V);
			vDot = _mm_and_ps(vDot, Constants::Mask3);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_sqrt_ps(vDot);
			vDot = _mm_div_ps(Constants::One, vDot);
			return vDot;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product
			VectorType vDot = _mm_mul_ps(V, V);
			// x=Dot.y, y=Dot.z
			VectorType vTemp = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_sqrt_ps(vDot);
			// Get the reciprocal
			vDot = _mm_div_ps(Constants::One, vDot);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3LengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = Result.SqrtEst();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(v1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, Constants::Mask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y and z
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Length
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector3LengthSq(V);
			Result = Result.Sqrt();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x7f);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, Constants::Mask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y and z
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and y
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			// x+z, y
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// y,y,y,y
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// x+z+y,??,??,??
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			// Splat the length squared
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector3NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline VectorType RAYMATH_CALLCONV Vector3NormalizeEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Result;
			Result = Vector3ReciprocalLength(V);
			Result = Vector::Multiply(V, Result);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			// Normalize
			return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0x7f);
			VectorType vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vDot = _mm_mul_ps(V, V);
			vDot = _mm_and_ps(vDot, Constants::Mask3);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_rsqrt_ps(vDot);
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product
			VectorType vDot = _mm_mul_ps(V, V);
			// x=Dot.y, y=Dot.z
			VectorType vTemp = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_rsqrt_ps(vDot);
			// Perform the normalization
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Normalize
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float fLength;
			VectorType vResult;

			vResult = Vector3Length(V);
			fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot3
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(Constants::Infinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			VectorType vResult = vmulq_f32(V, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vcombine_f32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
			return vbslq_f32(vcombine_f32(VEqualsInf, VEqualsInf), Constants::QNaN, vResult);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vLengthSq = _mm_dp_ps(V, V, 0x7f);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE3_INTRINSICS)
			// Perform the dot product on x,y and z only
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_and_ps(vLengthSq, Constants::Mask3);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y and z only
			VectorType vLengthSq = _mm_mul_ps(V, V);
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3ClampLength
		(
			FVectorType V,
			float    LengthMin,
			float    LengthMax
		)
		{
			VectorType ClampMax = Vector::Replicate(LengthMax);
			VectorType ClampMin = Vector::Replicate(LengthMin);

			return Vector3ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3ClampLengthV
		(
			FVectorType V,
			FVectorType LengthMin,
			FVectorType LengthMax
		)
		{
			assert((Vector(LengthMin).GetY() == Vector(LengthMin).GetX()) && (Vector(LengthMin).GetZ() == Vector(LengthMin).GetX()));
			assert((Vector(LengthMax).GetY() == Vector(LengthMax).GetX()) && (Vector(LengthMax).GetZ() == Vector(LengthMax).GetX()));
			assert(Vector3GreaterOrEqual(LengthMin, Vector::Zero()));
			assert(Vector3GreaterOrEqual(LengthMax, Vector::Zero()));
			assert(Vector3GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector3LengthSq(V);

			const VectorType Zero = Vector::Zero();

			VectorType RcpLength = LengthSq.ReciprocalSqrt();

			VectorType InfiniteLength = Vector::EqualInt(LengthSq, Constants::Infinity.v);
			VectorType ZeroLength = Vector::Equal(LengthSq, Zero);

			VectorType Normal = Vector::Multiply(V, RcpLength);

			Vector Length = Vector::Multiply(LengthSq, RcpLength);

			VectorType Select = Vector::EqualInt(InfiniteLength, ZeroLength);
			Length = Vector::Select(LengthSq, Length, Select);
			Normal = Vector::Select(LengthSq, Normal, Select);

			VectorType ControlMax = Length.Greater(LengthMax);
			VectorType ControlMin = Length.Less(LengthMin);

			VectorType ClampLength = Vector::Select(Length, LengthMax, ControlMax);
			ClampLength = Vector::Select(ClampLength, LengthMin, ControlMin);

			VectorType Result = Vector::Multiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			VectorType Control = Vector::EqualInt(ControlMax, ControlMin);
			Result = Vector::Select(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Reflect
		(
			FVectorType Incident,
			FVectorType Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			VectorType Result = Vector3Dot(Incident, Normal);
			Result = Vector::Add(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Refract
		(
			FVectorType Incident,
			FVectorType Normal,
			float    RefractionIndex
		)
		{
			VectorType Index = Vector::Replicate(RefractionIndex);
			return Vector3RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3RefractV
		(
			FVectorType Incident,
			FVectorType Normal,
			FVectorType RefractionIndex
		)
		{
			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(RAY_NO_INTRINSICS)

			const VectorType  Zero = Vector::Zero();

			VectorType IDotN = Vector3Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			Vector R = VectorNegativeMultiplySubtract(IDotN, IDotN, Constants::One.v);
			R = Vector::Multiply(R, RefractionIndex);
			R = VectorNegativeMultiplySubtract(R, RefractionIndex, Constants::One.v);

			if (Vector4LessOrEqual(R, Zero))
			{
				// Total internal reflection
				return Zero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = R.Sqrt();
				R = VectorMultiplyAdd(RefractionIndex, IDotN, R);

				// Result = RefractionIndex * Incident - Normal * R
				VectorType Result = Vector::Multiply(RefractionIndex, Incident);
				Result = VectorNegativeMultiplySubtract(Normal, R, Result);

				return Result;
			}

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType IDotN = Vector3Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float32x4_t R = vmlsq_f32(Constants::One, IDotN, IDotN);
			R = vmulq_f32(R, RefractionIndex);
			R = vmlsq_f32(Constants::One, R, RefractionIndex);

			uint32x4_t vResult = vcleq_f32(R, Constants::Zero);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			if (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU)
			{
				// Total internal reflection
				vResult = Constants::Zero;
			}
			else
			{
				// Sqrt(R)
				float32x4_t S0 = vrsqrteq_f32(R);
				float32x4_t P0 = vmulq_f32(R, S0);
				float32x4_t R0 = vrsqrtsq_f32(P0, S0);
				float32x4_t S1 = vmulq_f32(S0, R0);
				float32x4_t P1 = vmulq_f32(R, S1);
				float32x4_t R1 = vrsqrtsq_f32(P1, S1);
				float32x4_t S2 = vmulq_f32(S1, R1);
				R = vmulq_f32(R, S2);
				// R = RefractionIndex * IDotN + sqrt(R)
				R = vmlaq_f32(R, RefractionIndex, IDotN);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = vmulq_f32(RefractionIndex, Incident);
				vResult = vmlsq_f32(vResult, R, Normal);
			}
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
	// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
	// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
			VectorType IDotN = Vector3Dot(Incident, Normal);
			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			VectorType R = _mm_mul_ps(IDotN, IDotN);
			R = _mm_sub_ps(Constants::One, R);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_sub_ps(Constants::One, R);

			VectorType vResult = _mm_cmple_ps(R, Constants::Zero);
			if (_mm_movemask_ps(vResult) == 0x0f)
			{
				// Total internal reflection
				vResult = Constants::Zero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = _mm_sqrt_ps(R);
				vResult = _mm_mul_ps(RefractionIndex, IDotN);
				R = _mm_add_ps(R, vResult);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = _mm_mul_ps(RefractionIndex, Incident);
				R = _mm_mul_ps(R, Normal);
				vResult = _mm_sub_ps(vResult, R);
			}
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Orthogonal
		(
			FVectorType V
		)
		{
			VectorType Zero = Vector::Zero();
			Vector Z = Vector::SplatZ(V);
			Vector YZYY = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(V);

			VectorType NegativeV = Vector::Subtract(Zero, V);

			VectorType ZIsNegative = Z.Less(Zero);
			VectorType YZYYIsNegative = YZYY.Less(Zero);

			VectorType S = Vector::Add(YZYY, Z);
			VectorType D = Vector::Subtract(YZYY, Z);

			VectorType Select = Vector::EqualInt(ZIsNegative, YZYYIsNegative);

			VectorType R0 = VectorPermute<RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0X>(NegativeV, S);
			VectorType R1 = VectorPermute<RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0X>(V, D);

			return Vector::Select(R1, R0, Select);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3AngleBetweenNormalsEst
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector3Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne.v, Constants::One.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3AngleBetweenNormals
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector3Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne.v, Constants::One.v);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3AngleBetweenVectors
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			VectorType L1 = Vector3ReciprocalLength(V1);
			VectorType L2 = Vector3ReciprocalLength(V2);

			VectorType Dot = Vector3Dot(V1, V2);

			L1 = Vector::Multiply(L1, L2);

			Vector CosAngle = Vector::Multiply(Dot, L1);
			CosAngle = CosAngle.Clamp(Constants::NegativeOne.v, Constants::One.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3LinePointDistance
		(
			FVectorType LinePoint1,
			FVectorType LinePoint2,
			FVectorType Point
		)
		{
			// Given a vector PointVector from LinePoint1 to Point and a vector
			// LineVector from LinePoint1 to LinePoint2, the scaled distance 
			// PointProjectionScale from LinePoint1 to the perpendicular projection
			// of PointVector onto the line is defined as:
			//
			//     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

			VectorType PointVector = Vector::Subtract(Point, LinePoint1);
			VectorType LineVector = Vector::Subtract(LinePoint2, LinePoint1);

			VectorType LengthSq = Vector3LengthSq(LineVector);

			VectorType PointProjectionScale = Vector3Dot(PointVector, LineVector);
			PointProjectionScale = Vector::Divide(PointProjectionScale, LengthSq);

			VectorType DistanceVector = Vector::Multiply(LineVector, PointProjectionScale);
			DistanceVector = Vector::Subtract(PointVector, DistanceVector);

			return Vector3Length(DistanceVector);
		}

		//------------------------------------------------------------------------------


		inline void RAYMATH_CALLCONV Vector3ComponentsFromNormal
		(
			VectorType* pParallel,
			VectorType* pPerpendicular,
			FVectorType  V,
			FVectorType  Normal
		)
		{
			assert(pParallel != nullptr);
			assert(pPerpendicular != nullptr);

			VectorType Scale = Vector3Dot(V, Normal);

			VectorType Parallel = Vector::Multiply(Normal, Scale);

			*pParallel = Parallel;
			*pPerpendicular = Vector::Subtract(V, Parallel);
		}

		//------------------------------------------------------------------------------
		// Transform a vector using a rotation expressed as a unit quaternion

		inline VectorType RAYMATH_CALLCONV Vector3Rotate
		(
			FVectorType V,
			FQuaternion RotationFromQuaternion
		)
		{
			VectorType A = Vector::Select(Constants::Select1110.v, V, Constants::Select1110.v);
			VectorType Q = RotationFromQuaternion.Conjugate();
			VectorType Result = Quaternion::Multiply(Q, A);
			return Quaternion::Multiply(Result, RotationFromQuaternion);
		}

		//------------------------------------------------------------------------------
		// Transform a vector using the inverse of a rotation expressed as a unit quaternion

		inline VectorType RAYMATH_CALLCONV Vector3InverseRotate
		(
			FVectorType V,
			FQuaternion RotationFromQuaternion
		)
		{
			VectorType A = Vector::Select(Constants::Select1110.v, V, Constants::Select1110.v);
			Quaternion Result = Quaternion::Multiply(RotationFromQuaternion, A);
			VectorType Q = RotationFromQuaternion.Conjugate();
			return Quaternion::Multiply(Result, Q);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Transform
		(
			FVectorType V,
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Z = Vector::SplatZ(V);
			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = VectorMultiplyAdd(Z, M.r[2], M.r[3]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			VectorType vResult = vmlaq_lane_f32(M.r[3], M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			VectorType vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			vTemp = _mm_mul_ps(vTemp, M.r[2]);
			vResult = _mm_add_ps(vResult, vTemp);
			vResult = _mm_add_ps(vResult, M.r[3]);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float4* RAYMATH_CALLCONV Vector3TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			assert(OutputStride >= sizeof(Float4));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				VectorType Z = Vector::SplatZ(V);
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = VectorMultiplyAdd(Z, row2, row3);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						VectorType vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						VectorType vResult3 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						vResult3 = vmlaq_lane_f32(vResult3, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

						float32x4x4_t R;
						R.val[0] = vResult0;
						R.val[1] = vResult1;
						R.val[2] = vResult2;
						R.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), R);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				VectorType vResult = vmlaq_lane_f32(row3, row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
					{
						// Packed input, aligned output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, row2);
							VectorType vTemp2 = _mm_mul_ps(Y, row1);
							VectorType vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
					else
					{
						// Packed input, unaligned output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, row2);
							VectorType vTemp2 = _mm_mul_ps(Y, row1);
							VectorType vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);
							_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
			{
				// Aligned output
				for (; i < VectorCount; ++i)
				{
					VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
					pInputVector += InputStride;

					VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Z, row2);
					VectorType vTemp2 = _mm_mul_ps(Y, row1);
					VectorType vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

					RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}
			else
			{
				// Unaligned output
				for (; i < VectorCount; ++i)
				{
					VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
					pInputVector += InputStride;

					VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
					VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
					VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

					VectorType vTemp = _mm_mul_ps(Z, row2);
					VectorType vTemp2 = _mm_mul_ps(Y, row1);
					VectorType vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

					_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTemp);
					pOutputVector += OutputStride;
				}
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3TransformCoord
		(
			FVectorType V,
			FMatrix M
		)
		{
			VectorType Z = Vector::SplatZ(V);
			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = VectorMultiplyAdd(Z, M.r[2], M.r[3]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			VectorType W = Vector::SplatW(Result);
			return Vector::Divide(Result, W);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* RAYMATH_CALLCONV Vector3TransformCoordStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			assert(OutputStride >= sizeof(Float3));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				VectorType Z = Vector::SplatZ(V);
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = VectorMultiplyAdd(Z, row2, row3);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				VectorType W = Vector::SplatW(Result);

				Result = Vector::Divide(Result, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(row3);
						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(row3);
						r = vget_high_f32(row0);
						VectorType vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						VectorType W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
						V.val[2] = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
						V.val[2] = vmulq_f32(vResult2, Reciprocal);
#endif

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				VectorType vResult = vmlaq_lane_f32(row3, row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				VH = vget_high_f32(vResult);
				VectorType W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
				vResult = vdivq_f32(vResult, W);
#else
				// 2 iterations of Newton-Raphson refinement of reciprocal for W
				float32x4_t Reciprocal = vrecpeq_f32(W);
				float32x4_t S = vrecpsq_f32(Reciprocal, W);
				Reciprocal = vmulq_f32(S, Reciprocal);
				S = vrecpsq_f32(Reciprocal, W);
				Reciprocal = vmulq_f32(S, Reciprocal);

				vResult = vmulq_f32(vResult, Reciprocal);
#endif

				VL = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
				vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, row2);
								VectorType vTemp2 = _mm_mul_ps(Y, row1);
								VectorType vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, row2);
								VectorType vTemp2 = _mm_mul_ps(Y, row1);
								VectorType vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, row3);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, row2);
							VectorType vTemp2 = _mm_mul_ps(Y, row1);
							VectorType vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, row3);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

							vTemp = _mm_div_ps(vTemp, W);
							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				VectorType vTemp = _mm_mul_ps(Z, row2);
				VectorType vTemp2 = _mm_mul_ps(Y, row1);
				VectorType vTemp3 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, row3);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));

				vTemp = _mm_div_ps(vTemp, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3TransformNormal
		(
			FVectorType V,
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Z = Vector::SplatZ(V);
			VectorType Y = Vector::SplatY(V);
			VectorType X = Vector::SplatX(V);

			VectorType Result = Vector::Multiply(Z, M.r[2]);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			VectorType vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			vResult = _mm_mul_ps(vResult, M.r[0]);
			VectorType vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			vTemp = _mm_mul_ps(vTemp, M.r[1]);
			vResult = _mm_add_ps(vResult, vTemp);
			vTemp = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			vTemp = _mm_mul_ps(vTemp, M.r[2]);
			vResult = _mm_add_ps(vResult, vTemp);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* RAYMATH_CALLCONV Vector3TransformNormalStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			assert(OutputStride >= sizeof(Float3));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				VectorType Z = Vector::SplatZ(V);
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = Vector::Multiply(Z, row2);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						VectorType vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);

						r = vget_high_f32(row0);
						VectorType vResult2 = vmulq_lane_f32(V.val[0], r, 0); // Cx

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

						V.val[0] = vResult0;
						V.val[1] = vResult1;
						V.val[2] = vResult2;

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
				float32x2_t zero = vdup_n_f32(0);
				float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
				pInputVector += InputStride;

				VectorType vResult = vmulq_lane_f32(row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z

				VL = vget_low_f32(vResult);
				vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
				vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, row2);
								VectorType vTemp2 = _mm_mul_ps(Y, row1);
								VectorType vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V1 = _mm_add_ps(vTemp, vTemp3);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V2 = _mm_add_ps(vTemp, vTemp3);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V3 = _mm_add_ps(vTemp, vTemp3);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V4 = _mm_add_ps(vTemp, vTemp3);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, row2);
								VectorType vTemp2 = _mm_mul_ps(Y, row1);
								VectorType vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V1 = _mm_add_ps(vTemp, vTemp3);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V2 = _mm_add_ps(vTemp, vTemp3);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V3 = _mm_add_ps(vTemp, vTemp3);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, row2);
								vTemp2 = _mm_mul_ps(Y, row1);
								vTemp3 = _mm_mul_ps(X, row0);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								V4 = _mm_add_ps(vTemp, vTemp3);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, row2);
							VectorType vTemp2 = _mm_mul_ps(Y, row1);
							VectorType vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, row2);
							vTemp2 = _mm_mul_ps(Y, row1);
							vTemp3 = _mm_mul_ps(X, row0);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				VectorType vTemp = _mm_mul_ps(Z, row2);
				VectorType vTemp2 = _mm_mul_ps(Y, row1);
				VectorType vTemp3 = _mm_mul_ps(X, row0);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Project
		(
			FVectorType V,
			float    ViewportX,
			float    ViewportY,
			float    ViewportWidth,
			float    ViewportHeight,
			float    ViewportMinZ,
			float    ViewportMaxZ,
			FMatrix Projection,
			CMatrix View,
			CMatrix World
		)
		{
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			VectorType Scale = Vector::Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
			VectorType Offset = Vector::Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);

			VectorType Result = Vector3TransformCoord(V, Transform);

			Result = VectorMultiplyAdd(Result, Scale, Offset);

			return Result;
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* RAYMATH_CALLCONV Vector3ProjectStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			float           ViewportX,
			float           ViewportY,
			float           ViewportWidth,
			float           ViewportHeight,
			float           ViewportMinZ,
			float           ViewportMaxZ,
			FMatrix     Projection,
			CMatrix     View,
			CMatrix     World
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			assert(OutputStride >= sizeof(Float3));

#if defined(RAY_NO_INTRINSICS)

			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			VectorType Scale = Vector::Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
			VectorType Offset = Vector::Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));

				VectorType Result = Vector3TransformCoord(V, Transform);
				Result = VectorMultiplyAdd(Result, Scale, Offset);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					VectorType ScaleX = vdupq_n_f32(HalfViewportWidth);
					VectorType ScaleY = vdupq_n_f32(-HalfViewportHeight);
					VectorType ScaleZ = vdupq_n_f32(ViewportMaxZ - ViewportMinZ);

					VectorType OffsetX = vdupq_n_f32(ViewportX + HalfViewportWidth);
					VectorType OffsetY = vdupq_n_f32(ViewportY + HalfViewportHeight);
					VectorType OffsetZ = vdupq_n_f32(ViewportMinZ);

					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						float32x2_t r3 = vget_low_f32(Transform.r[3]);
						float32x2_t r = vget_low_f32(Transform.r[0]);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(Transform.r[3]);
						r = vget_high_f32(Transform.r[0]);
						VectorType vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), V.val[0], r, 0); // Cx+O
						VectorType W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), V.val[0], r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(Transform.r[1]);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(Transform.r[1]);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, V.val[1], r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(Transform.r[2]);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(Transform.r[2]);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, V.val[2], r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						vResult0 = vdivq_f32(vResult0, W);
						vResult1 = vdivq_f32(vResult1, W);
						vResult2 = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						vResult0 = vmulq_f32(vResult0, Reciprocal);
						vResult1 = vmulq_f32(vResult1, Reciprocal);
						vResult2 = vmulq_f32(vResult2, Reciprocal);
#endif

						V.val[0] = vmlaq_f32(OffsetX, vResult0, ScaleX);
						V.val[1] = vmlaq_f32(OffsetY, vResult1, ScaleY);
						V.val[2] = vmlaq_f32(OffsetZ, vResult2, ScaleZ);

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			if (i < VectorCount)
			{
				VectorType Scale = Vector::Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
				VectorType Offset = Vector::Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

				for (; i < VectorCount; i++)
				{
					float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
					float32x2_t zero = vdup_n_f32(0);
					float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
					pInputVector += InputStride;

					VectorType vResult = vmlaq_lane_f32(Transform.r[3], Transform.r[0], VL, 0); // X
					vResult = vmlaq_lane_f32(vResult, Transform.r[1], VL, 1); // Y
					vResult = vmlaq_lane_f32(vResult, Transform.r[2], VH, 0); // Z

					VH = vget_high_f32(vResult);
					VectorType W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
					vResult = vdivq_f32(vResult, W);
#else
					// 2 iterations of Newton-Raphson refinement of reciprocal for W
					float32x4_t Reciprocal = vrecpeq_f32(W);
					float32x4_t S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);
					S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);

					vResult = vmulq_f32(vResult, Reciprocal);
#endif

					vResult = vmlaq_f32(Offset, vResult, Scale);

					VL = vget_low_f32(vResult);
					vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
					vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
					pOutputVector += OutputStride;
				}
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			const float HalfViewportWidth = ViewportWidth * 0.5f;
			const float HalfViewportHeight = ViewportHeight * 0.5f;

			VectorType Scale = Vector::Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 1.0f);
			VectorType Offset = Vector::Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
								VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V1 = _mm_add_ps(vTemp, Offset);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V2 = _mm_add_ps(vTemp, Offset);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V3 = _mm_add_ps(vTemp, Offset);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V4 = _mm_add_ps(vTemp, Offset);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
								VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V1 = _mm_add_ps(vTemp, Offset);

								// Result 2
								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V2 = _mm_add_ps(vTemp, Offset);

								// Result 3
								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V3 = _mm_add_ps(vTemp, Offset);

								// Result 4
								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								vTemp = _mm_div_ps(vTemp, W);

								vTemp = _mm_mul_ps(vTemp, Scale);
								V4 = _mm_add_ps(vTemp, Offset);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
							VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							vTemp = _mm_mul_ps(vTemp, Scale);
							vTemp = _mm_add_ps(vTemp, Offset);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
				VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
				VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
				vTemp = _mm_add_ps(vTemp, Transform.r[3]);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
				vTemp = _mm_div_ps(vTemp, W);

				vTemp = _mm_mul_ps(vTemp, Scale);
				vTemp = _mm_add_ps(vTemp, Offset);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector3Unproject
		(
			FVectorType V,
			float     ViewportX,
			float     ViewportY,
			float     ViewportWidth,
			float     ViewportHeight,
			float     ViewportMinZ,
			float     ViewportMaxZ,
			FMatrix Projection,
			CMatrix View,
			CMatrix World
		)
		{
			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = Vector::Set(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = Scale.Reciprocal();

			VectorType Offset = Vector::Set(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = VectorMultiplyAdd(Scale, Offset, D.v);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);
			Transform = Transform.Inverse();

			VectorType Result = VectorMultiplyAdd(V, Scale, Offset);

			return Vector3TransformCoord(Result, Transform);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015 26019, "PREfast noise: Esp:1307" )
#endif


		inline Float3* RAYMATH_CALLCONV Vector3UnprojectStream
		(
			Float3* pOutputStream,
			size_t          OutputStride,
			const Float3* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			float           ViewportX,
			float           ViewportY,
			float           ViewportWidth,
			float           ViewportHeight,
			float           ViewportMinZ,
			float           ViewportMaxZ,
			FMatrix       Projection,
			CMatrix       View,
			CMatrix       World)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float3));
			assert(OutputStride >= sizeof(Float3));

#if defined(RAY_NO_INTRINSICS)

			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = Vector::Set(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = Scale.Reciprocal();

			VectorType Offset = Vector::Set(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = VectorMultiplyAdd(Scale, Offset, D.v);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);
			Transform = Transform.Inverse();

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));

				VectorType Result = VectorMultiplyAdd(V, Scale, Offset);

				Result = Vector3TransformCoord(Result, Transform);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), Result);

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);
			Transform = Transform.Inverse();

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			float sx = 1.f / (ViewportWidth * 0.5f);
			float sy = 1.f / (-ViewportHeight * 0.5f);
			float sz = 1.f / (ViewportMaxZ - ViewportMinZ);

			float ox = (-ViewportX * sx) - 1.f;
			float oy = (-ViewportY * sy) + 1.f;
			float oz = (-ViewportMinZ * sz);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float3)) && (OutputStride == sizeof(Float3)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x3_t V = vld3q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float3) * 4;

						VectorType ScaleX = vdupq_n_f32(sx);
						VectorType OffsetX = vdupq_n_f32(ox);
						VectorType VX = vmlaq_f32(OffsetX, ScaleX, V.val[0]);

						float32x2_t r3 = vget_low_f32(Transform.r[3]);
						float32x2_t r = vget_low_f32(Transform.r[0]);
						VectorType vResult0 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), VX, r, 0); // Ax+M
						VectorType vResult1 = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), VX, r, 1); // Bx+N

						__prefetch(pInputVector);

						r3 = vget_high_f32(Transform.r[3]);
						r = vget_high_f32(Transform.r[0]);
						VectorType vResult2 = vmlaq_lane_f32(vdupq_lane_f32(r3, 0), VX, r, 0); // Cx+O
						VectorType W = vmlaq_lane_f32(vdupq_lane_f32(r3, 1), VX, r, 1); // Dx+P

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						VectorType ScaleY = vdupq_n_f32(sy);
						VectorType OffsetY = vdupq_n_f32(oy);
						VectorType VY = vmlaq_f32(OffsetY, ScaleY, V.val[1]);

						r = vget_low_f32(Transform.r[1]);
						vResult0 = vmlaq_lane_f32(vResult0, VY, r, 0); // Ax+Ey+M
						vResult1 = vmlaq_lane_f32(vResult1, VY, r, 1); // Bx+Fy+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(Transform.r[1]);
						vResult2 = vmlaq_lane_f32(vResult2, VY, r, 0); // Cx+Gy+O
						W = vmlaq_lane_f32(W, VY, r, 1); // Dx+Hy+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						VectorType ScaleZ = vdupq_n_f32(sz);
						VectorType OffsetZ = vdupq_n_f32(oz);
						VectorType VZ = vmlaq_f32(OffsetZ, ScaleZ, V.val[2]);

						r = vget_low_f32(Transform.r[2]);
						vResult0 = vmlaq_lane_f32(vResult0, VZ, r, 0); // Ax+Ey+Iz+M
						vResult1 = vmlaq_lane_f32(vResult1, VZ, r, 1); // Bx+Fy+Jz+N

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(Transform.r[2]);
						vResult2 = vmlaq_lane_f32(vResult2, VZ, r, 0); // Cx+Gy+Kz+O
						W = vmlaq_lane_f32(W, VZ, r, 1); // Dx+Hy+Lz+P

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
						V.val[0] = vdivq_f32(vResult0, W);
						V.val[1] = vdivq_f32(vResult1, W);
						V.val[2] = vdivq_f32(vResult2, W);
#else
						// 2 iterations of Newton-Raphson refinement of reciprocal
						float32x4_t Reciprocal = vrecpeq_f32(W);
						float32x4_t S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);
						S = vrecpsq_f32(Reciprocal, W);
						Reciprocal = vmulq_f32(S, Reciprocal);

						V.val[0] = vmulq_f32(vResult0, Reciprocal);
						V.val[1] = vmulq_f32(vResult1, Reciprocal);
						V.val[2] = vmulq_f32(vResult2, Reciprocal);
#endif

						vst3q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float3) * 4;

						i += 4;
					}
				}
			}

			if (i < VectorCount)
			{
				float32x2_t ScaleL = vcreate_f32(
					static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sx))
					| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sy)) << 32));
				float32x2_t ScaleH = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&sz)));

				float32x2_t OffsetL = vcreate_f32(
					static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&ox))
					| (static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&oy)) << 32));
				float32x2_t OffsetH = vcreate_f32(static_cast<uint64_t>(*reinterpret_cast<const uint32_t*>(&oz)));

				for (; i < VectorCount; i++)
				{
					float32x2_t VL = vld1_f32(reinterpret_cast<const float*>(pInputVector));
					float32x2_t zero = vdup_n_f32(0);
					float32x2_t VH = vld1_lane_f32(reinterpret_cast<const float*>(pInputVector) + 2, zero, 0);
					pInputVector += InputStride;

					VL = vmla_f32(OffsetL, VL, ScaleL);
					VH = vmla_f32(OffsetH, VH, ScaleH);

					VectorType vResult = vmlaq_lane_f32(Transform.r[3], Transform.r[0], VL, 0); // X
					vResult = vmlaq_lane_f32(vResult, Transform.r[1], VL, 1); // Y
					vResult = vmlaq_lane_f32(vResult, Transform.r[2], VH, 0); // Z

					VH = vget_high_f32(vResult);
					VectorType W = vdupq_lane_f32(VH, 1);

#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
					vResult = vdivq_f32(vResult, W);
#else
					// 2 iterations of Newton-Raphson refinement of reciprocal for W
					float32x4_t Reciprocal = vrecpeq_f32(W);
					float32x4_t S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);
					S = vrecpsq_f32(Reciprocal, W);
					Reciprocal = vmulq_f32(S, Reciprocal);

					vResult = vmulq_f32(vResult, Reciprocal);
#endif

					VL = vget_low_f32(vResult);
					vst1_f32(reinterpret_cast<float*>(pOutputVector), VL);
					vst1q_lane_f32(reinterpret_cast<float*>(pOutputVector) + 2, vResult, 2);
					pOutputVector += OutputStride;
				}
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

			Vector Scale = Vector::Set(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
			Scale = Scale.Reciprocal();

			VectorType Offset = Vector::Set(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
			Offset = _mm_mul_ps(Scale, Offset);
			Offset = _mm_add_ps(Offset, D);

			Matrix Transform = Matrix::Multiply(World, View);
			Transform = Matrix::Multiply(Transform, Projection);
			Transform = Transform.Inverse();

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if (InputStride == sizeof(Float3))
				{
					if (OutputStride == sizeof(Float3))
					{
						if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF))
						{
							// Packed input, aligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								V1 = _mm_mul_ps(V1, Scale);
								V1 = _mm_add_ps(V1, Offset);

								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
								VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								V2 = _mm_mul_ps(V2, Scale);
								V2 = _mm_add_ps(V2, Offset);

								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								V3 = _mm_mul_ps(V3, Scale);
								V3 = _mm_add_ps(V3, Offset);

								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								V4 = _mm_mul_ps(V4, Scale);
								V4 = _mm_add_ps(V4, Offset);

								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), V1);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
						else
						{
							// Packed input, unaligned & packed output
							for (size_t j = 0; j < four; ++j)
							{
								__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
								__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
								__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
								pInputVector += sizeof(Float3) * 4;

								// Unpack the 4 vectors (.w components are junk)
								RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

								// Result 1
								V1 = _mm_mul_ps(V1, Scale);
								V1 = _mm_add_ps(V1, Offset);

								VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
								VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
								VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

								VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
								VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V1 = _mm_div_ps(vTemp, W);

								// Result 2
								V2 = _mm_mul_ps(V2, Scale);
								V2 = _mm_add_ps(V2, Offset);

								Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V2 = _mm_div_ps(vTemp, W);

								// Result 3
								V3 = _mm_mul_ps(V3, Scale);
								V3 = _mm_add_ps(V3, Offset);

								Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V3 = _mm_div_ps(vTemp, W);

								// Result 4
								V4 = _mm_mul_ps(V4, Scale);
								V4 = _mm_add_ps(V4, Offset);

								Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
								Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
								X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

								vTemp = _mm_mul_ps(Z, Transform.r[2]);
								vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
								vTemp3 = _mm_mul_ps(X, Transform.r[0]);
								vTemp = _mm_add_ps(vTemp, Transform.r[3]);
								vTemp = _mm_add_ps(vTemp, vTemp2);
								vTemp = _mm_add_ps(vTemp, vTemp3);

								W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
								V4 = _mm_div_ps(vTemp, W);

								// Pack and store the vectors
								RAYMATH3_PACK4_INTO3(vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), V1);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 16), vTemp);
								_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector + 32), V3);
								pOutputVector += sizeof(Float3) * 4;
								i += 4;
							}
						}
					}
					else
					{
						// Packed input, unpacked output
						for (size_t j = 0; j < four; ++j)
						{
							__m128 V1 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
							__m128 L2 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 16));
							__m128 L3 = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector + 32));
							pInputVector += sizeof(Float3) * 4;

							// Unpack the 4 vectors (.w components are junk)
							RAYMATH3_UNPACK3_INTO4(V1, L2, L3);

							// Result 1
							V1 = _mm_mul_ps(V1, Scale);
							V1 = _mm_add_ps(V1, Offset);

							VectorType Z = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
							VectorType Y = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
							VectorType X = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

							VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
							VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 2
							V2 = _mm_mul_ps(V2, Scale);
							V2 = _mm_add_ps(V2, Offset);

							Z = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 3
							V3 = _mm_mul_ps(V3, Scale);
							V3 = _mm_add_ps(V3, Offset);

							Z = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							// Result 4
							V4 = _mm_mul_ps(V4, Scale);
							V4 = _mm_add_ps(V4, Offset);

							Z = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
							Y = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
							X = RAYMATH_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

							vTemp = _mm_mul_ps(Z, Transform.r[2]);
							vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
							vTemp3 = _mm_mul_ps(X, Transform.r[0]);
							vTemp = _mm_add_ps(vTemp, Transform.r[3]);
							vTemp = _mm_add_ps(vTemp, vTemp2);
							vTemp = _mm_add_ps(vTemp, vTemp3);

							W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
							vTemp = _mm_div_ps(vTemp, W);

							StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
							pOutputVector += OutputStride;

							i += 4;
						}
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				VectorType V = LoadFloat3(reinterpret_cast<const Float3*>(pInputVector));
				pInputVector += InputStride;

				V = _mm_mul_ps(V, Scale);
				V = _mm_add_ps(V, Offset);

				VectorType Z = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
				VectorType Y = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
				VectorType X = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

				VectorType vTemp = _mm_mul_ps(Z, Transform.r[2]);
				VectorType vTemp2 = _mm_mul_ps(Y, Transform.r[1]);
				VectorType vTemp3 = _mm_mul_ps(X, Transform.r[0]);
				vTemp = _mm_add_ps(vTemp, Transform.r[3]);
				vTemp = _mm_add_ps(vTemp, vTemp2);
				vTemp = _mm_add_ps(vTemp, vTemp3);

				VectorType W = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(3, 3, 3, 3));
				vTemp = _mm_div_ps(vTemp, W);

				StoreFloat3(reinterpret_cast<Float3*>(pOutputVector), vTemp);
				pOutputVector += OutputStride;
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		/****************************************************************************
		 *
		 * 4D VectorType
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4Equal
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] == V2.vector4_f32[0]) && (V1.vector4_f32[1] == V2.vector4_f32[1]) && (V1.vector4_f32[2] == V2.vector4_f32[2]) && (V1.vector4_f32[3] == V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4EqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector4EqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			uint32_t CR = 0;

			if ((V1.vector4_f32[0] == V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] == V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] == V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] == V2.vector4_f32[3]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] != V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] != V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] != V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] != V2.vector4_f32[3]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpeq_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			uint32_t CR = 0;
			if (iTest == 0xf)     // All equal?
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (iTest == 0)  // All not equal?
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4EqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] == V2.vector4_u32[0]) && (V1.vector4_u32[1] == V2.vector4_u32[1]) && (V1.vector4_u32[2] == V2.vector4_u32[2]) && (V1.vector4_u32[3] == V2.vector4_u32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) == 0xf) != 0);
#else
			return ComparisonAllTrue(Vector4EqualIntR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector4EqualIntR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if (V1.vector4_u32[0] == V2.vector4_u32[0] &&
				V1.vector4_u32[1] == V2.vector4_u32[1] &&
				V1.vector4_u32[2] == V2.vector4_u32[2] &&
				V1.vector4_u32[3] == V2.vector4_u32[3])
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (V1.vector4_u32[0] != V2.vector4_u32[0] &&
				V1.vector4_u32[1] != V2.vector4_u32[1] &&
				V1.vector4_u32[2] != V2.vector4_u32[2] &&
				V1.vector4_u32[3] != V2.vector4_u32[3])
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			int iTest = _mm_movemask_ps(_mm_castsi128_ps(vTemp));
			uint32_t CR = 0;
			if (iTest == 0xf)     // All equal?
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (iTest == 0)  // All not equal?
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		inline bool RAYMATH_CALLCONV Vector4NearEqual
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType Epsilon
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float dx, dy, dz, dw;

			dx = fabsf(V1.vector4_f32[0] - V2.vector4_f32[0]);
			dy = fabsf(V1.vector4_f32[1] - V2.vector4_f32[1]);
			dz = fabsf(V1.vector4_f32[2] - V2.vector4_f32[2]);
			dw = fabsf(V1.vector4_f32[3] - V2.vector4_f32[3]);
			return (((dx <= Epsilon.vector4_f32[0]) &&
				(dy <= Epsilon.vector4_f32[1]) &&
				(dz <= Epsilon.vector4_f32[2]) &&
				(dw <= Epsilon.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vDelta = vsubq_f32(V1, V2);
			uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Get the difference
			VectorType vDelta = _mm_sub_ps(V1, V2);
			// Get the absolute value of the difference
			VectorType vTemp = _mm_setzero_ps();
			vTemp = _mm_sub_ps(vTemp, vDelta);
			vTemp = _mm_max_ps(vTemp, vDelta);
			vTemp = _mm_cmple_ps(vTemp, Epsilon);
			return ((_mm_movemask_ps(vTemp) == 0xf) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4NotEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] != V2.vector4_f32[0]) || (V1.vector4_f32[1] != V2.vector4_f32[1]) || (V1.vector4_f32[2] != V2.vector4_f32[2]) || (V1.vector4_f32[3] != V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpneq_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp)) != 0);
#else
			return ComparisonAnyFalse(Vector4EqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4NotEqualInt
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_u32[0] != V2.vector4_u32[0]) || (V1.vector4_u32[1] != V2.vector4_u32[1]) || (V1.vector4_u32[2] != V2.vector4_u32[2]) || (V1.vector4_u32[3] != V2.vector4_u32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vceqq_u32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
			return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0xF) != 0);
#else
			return ComparisonAnyFalse(Vector4EqualIntR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4Greater
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] > V2.vector4_f32[0]) && (V1.vector4_f32[1] > V2.vector4_f32[1]) && (V1.vector4_f32[2] > V2.vector4_f32[2]) && (V1.vector4_f32[3] > V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector4GreaterR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if (V1.vector4_f32[0] > V2.vector4_f32[0] &&
				V1.vector4_f32[1] > V2.vector4_f32[1] &&
				V1.vector4_f32[2] > V2.vector4_f32[2] &&
				V1.vector4_f32[3] > V2.vector4_f32[3])
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (V1.vector4_f32[0] <= V2.vector4_f32[0] &&
				V1.vector4_f32[1] <= V2.vector4_f32[1] &&
				V1.vector4_f32[2] <= V2.vector4_f32[2] &&
				V1.vector4_f32[3] <= V2.vector4_f32[3])
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgtq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			uint32_t CR = 0;
			VectorType vTemp = _mm_cmpgt_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0xf) {
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4GreaterOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] >= V2.vector4_f32[0]) && (V1.vector4_f32[1] >= V2.vector4_f32[1]) && (V1.vector4_f32[2] >= V2.vector4_f32[2]) && (V1.vector4_f32[3] >= V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterOrEqualR(V1, V2));
#endif
		}

		//------------------------------------------------------------------------------

		inline uint32_t RAYMATH_CALLCONV Vector4GreaterOrEqualR
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			uint32_t CR = 0;
			if ((V1.vector4_f32[0] >= V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] >= V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] >= V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] >= V2.vector4_f32[3]))
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if ((V1.vector4_f32[0] < V2.vector4_f32[0]) &&
				(V1.vector4_f32[1] < V2.vector4_f32[1]) &&
				(V1.vector4_f32[2] < V2.vector4_f32[2]) &&
				(V1.vector4_f32[3] < V2.vector4_f32[3]))
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcgeq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);

			uint32_t CR = 0;
			if (r == 0xFFFFFFFFU)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!r)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#elif defined(RAY_SSE_INTRINSICS)
			uint32_t CR = 0;
			VectorType vTemp = _mm_cmpge_ps(V1, V2);
			int iTest = _mm_movemask_ps(vTemp);
			if (iTest == 0x0f)
			{
				CR = RAYMATH_CRMASK_CR6TRUE;
			}
			else if (!iTest)
			{
				CR = RAYMATH_CRMASK_CR6FALSE;
			}
			return CR;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4Less
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] < V2.vector4_f32[0]) && (V1.vector4_f32[1] < V2.vector4_f32[1]) && (V1.vector4_f32[2] < V2.vector4_f32[2]) && (V1.vector4_f32[3] < V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcltq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmplt_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterR(V2, V1));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4LessOrEqual
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V1.vector4_f32[0] <= V2.vector4_f32[0]) && (V1.vector4_f32[1] <= V2.vector4_f32[1]) && (V1.vector4_f32[2] <= V2.vector4_f32[2]) && (V1.vector4_f32[3] <= V2.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			uint32x4_t vResult = vcleq_f32(V1, V2);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp = _mm_cmple_ps(V1, V2);
			return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#else
			return ComparisonAllTrue(Vector4GreaterOrEqualR(V2, V1));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4InBounds
		(
			FVectorType V,
			FVectorType Bounds
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (((V.vector4_f32[0] <= Bounds.vector4_f32[0] && V.vector4_f32[0] >= -Bounds.vector4_f32[0]) &&
				(V.vector4_f32[1] <= Bounds.vector4_f32[1] && V.vector4_f32[1] >= -Bounds.vector4_f32[1]) &&
				(V.vector4_f32[2] <= Bounds.vector4_f32[2] && V.vector4_f32[2] >= -Bounds.vector4_f32[2]) &&
				(V.vector4_f32[3] <= Bounds.vector4_f32[3] && V.vector4_f32[3] >= -Bounds.vector4_f32[3])) != 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test if less than or equal
			uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
			// Negate the bounds
			float32x4_t vTemp2 = vnegq_f32(Bounds);
			// Test if greater or equal (Reversed)
			uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
			// Blend answers
			ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
			// in bounds?
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(ivTemp1), vget_high_u8(ivTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test if less than or equal
			VectorType vTemp1 = _mm_cmple_ps(V, Bounds);
			// Negate the bounds
			VectorType vTemp2 = _mm_mul_ps(Bounds, Constants::NegativeOne);
			// Test if greater or equal (Reversed)
			vTemp2 = _mm_cmple_ps(vTemp2, V);
			// Blend answers
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			// All in bounds?
			return ((_mm_movemask_ps(vTemp1) == 0x0f) != 0);
#else
			return ComparisonAllInBounds(Vector4InBoundsR(V, Bounds));
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4IsNaN
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			return (RAYMATH_ISNAN(V.vector4_f32[0]) ||
				RAYMATH_ISNAN(V.vector4_f32[1]) ||
				RAYMATH_ISNAN(V.vector4_f32[2]) ||
				RAYMATH_ISNAN(V.vector4_f32[3]));
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Test against itself. NaN is always not equal
			uint32x4_t vTempNan = vceqq_f32(V, V);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempNan), vget_high_u8(vTempNan));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			// If any are NaN, the mask is zero
			return (vget_lane_u32(vTemp.val[1], 1) != 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			// Test against itself. NaN is always not equal
			VectorType vTempNan = _mm_cmpneq_ps(V, V);
			// If any are NaN, the mask is non-zero
			return (_mm_movemask_ps(vTempNan) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Vector4IsInfinite
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			return (RAYMATH_ISINF(V.vector4_f32[0]) ||
				RAYMATH_ISINF(V.vector4_f32[1]) ||
				RAYMATH_ISINF(V.vector4_f32[2]) ||
				RAYMATH_ISINF(V.vector4_f32[3]));

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Mask off the sign bit
			uint32x4_t vTempInf = vandq_u32(V, Constants::AbsMask);
			// Compare to infinity
			vTempInf = vceqq_f32(vTempInf, Constants::Infinity);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTempInf), vget_high_u8(vTempInf));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			return (vget_lane_u32(vTemp.val[1], 1) != 0);
#elif defined(RAY_SSE_INTRINSICS)
			// Mask off the sign bit
			VectorType vTemp = _mm_and_ps(V, Constants::AbsMask);
			// Compare to infinity
			vTemp = _mm_cmpeq_ps(vTemp, Constants::Infinity);
			// If any are infinity, the signs are true.
			return (_mm_movemask_ps(vTemp) != 0);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Dot
		(
			FVectorType V1,
			FVectorType V2
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = V1.vector4_f32[0] * V2.vector4_f32[0] + V1.vector4_f32[1] * V2.vector4_f32[1] + V1.vector4_f32[2] * V2.vector4_f32[2] + V1.vector4_f32[3] * V2.vector4_f32[3];
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4_t vTemp = vmulq_f32(V1, V2);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			return vcombine_f32(v1, v1);
#elif defined(RAY_SSE4_INTRINSICS)
			return _mm_dp_ps(V1, V2, 0xff);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vTemp = _mm_mul_ps(V1, V2);
			vTemp = _mm_hadd_ps(vTemp, vTemp);
			return _mm_hadd_ps(vTemp, vTemp);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp2 = V2;
			VectorType vTemp = _mm_mul_ps(V1, vTemp2);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
			vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
			vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
			vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
			return RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Cross
		(
			FVectorType V1,
			FVectorType V2,
			FVectorType V3
		)
		{
			// [ ((v2.z*v3.w-v2.w*v3.z)*v1.y)-((v2.y*v3.w-v2.w*v3.y)*v1.z)+((v2.y*v3.z-v2.z*v3.y)*v1.w),
			//   ((v2.w*v3.z-v2.z*v3.w)*v1.x)-((v2.w*v3.x-v2.x*v3.w)*v1.z)+((v2.z*v3.x-v2.x*v3.z)*v1.w),
			//   ((v2.y*v3.w-v2.w*v3.y)*v1.x)-((v2.x*v3.w-v2.w*v3.x)*v1.y)+((v2.x*v3.y-v2.y*v3.x)*v1.w),
			//   ((v2.z*v3.y-v2.y*v3.z)*v1.x)-((v2.z*v3.x-v2.x*v3.z)*v1.y)+((v2.y*v3.x-v2.x*v3.y)*v1.z) ]

#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					(((V2.vector4_f32[2] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[2])) * V1.vector4_f32[1]) - (((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[2]) + (((V2.vector4_f32[1] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[1])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[3] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[3])) * V1.vector4_f32[0]) - (((V2.vector4_f32[3] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[3])) * V1.vector4_f32[2]) + (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[0]) - (((V2.vector4_f32[0] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[0])) * V1.vector4_f32[1]) + (((V2.vector4_f32[0] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[0])) * V1.vector4_f32[3]),
					(((V2.vector4_f32[2] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[2])) * V1.vector4_f32[0]) - (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[1]) + (((V2.vector4_f32[1] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[1])) * V1.vector4_f32[2]),
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			const float32x2_t select = vget_low_f32(Constants::MaskX);

			// Term1: V2zwyz * V3wzwy
			const float32x2_t v2xy = vget_low_f32(V2);
			const float32x2_t v2zw = vget_high_f32(V2);
			const float32x2_t v2yx = vrev64_f32(v2xy);
			const float32x2_t v2wz = vrev64_f32(v2zw);
			const float32x2_t v2yz = vbsl_f32(select, v2yx, v2wz);

			const float32x2_t v3zw = vget_high_f32(V3);
			const float32x2_t v3wz = vrev64_f32(v3zw);
			const float32x2_t v3xy = vget_low_f32(V3);
			const float32x2_t v3wy = vbsl_f32(select, v3wz, v3xy);

			float32x4_t vTemp1 = vcombine_f32(v2zw, v2yz);
			float32x4_t vTemp2 = vcombine_f32(v3wz, v3wy);
			VectorType vResult = vmulq_f32(vTemp1, vTemp2);

			// - V2wzwy * V3zwyz
			const float32x2_t v2wy = vbsl_f32(select, v2wz, v2xy);

			const float32x2_t v3yx = vrev64_f32(v3xy);
			const float32x2_t v3yz = vbsl_f32(select, v3yx, v3wz);

			vTemp1 = vcombine_f32(v2wz, v2wy);
			vTemp2 = vcombine_f32(v3zw, v3yz);
			vResult = vmlsq_f32(vResult, vTemp1, vTemp2);

			// term1 * V1yxxx
			const float32x2_t v1xy = vget_low_f32(V1);
			const float32x2_t v1yx = vrev64_f32(v1xy);

			vTemp1 = vcombine_f32(v1yx, vdup_lane_f32(v1yx, 1));
			vResult = vmulq_f32(vResult, vTemp1);

			// Term2: V2ywxz * V3wxwx
			const float32x2_t v2yw = vrev64_f32(v2wy);
			const float32x2_t v2xz = vbsl_f32(select, v2xy, v2wz);

			const float32x2_t v3wx = vbsl_f32(select, v3wz, v3yx);

			vTemp1 = vcombine_f32(v2yw, v2xz);
			vTemp2 = vcombine_f32(v3wx, v3wx);
			float32x4_t vTerm = vmulq_f32(vTemp1, vTemp2);

			// - V2wxwx * V3ywxz
			const float32x2_t v2wx = vbsl_f32(select, v2wz, v2yx);

			const float32x2_t v3yw = vrev64_f32(v3wy);
			const float32x2_t v3xz = vbsl_f32(select, v3xy, v3wz);

			vTemp1 = vcombine_f32(v2wx, v2wx);
			vTemp2 = vcombine_f32(v3yw, v3xz);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

			// vResult - term2 * V1zzyy
			const float32x2_t v1zw = vget_high_f32(V1);

			vTemp1 = vcombine_f32(vdup_lane_f32(v1zw, 0), vdup_lane_f32(v1yx, 0));
			vResult = vmlsq_f32(vResult, vTerm, vTemp1);

			// Term3: V2yzxy * V3zxyx
			const float32x2_t v3zx = vrev64_f32(v3xz);

			vTemp1 = vcombine_f32(v2yz, v2xy);
			vTemp2 = vcombine_f32(v3zx, v3yx);
			vTerm = vmulq_f32(vTemp1, vTemp2);

			// - V2zxyx * V3yzxy
			const float32x2_t v2zx = vrev64_f32(v2xz);

			vTemp1 = vcombine_f32(v2zx, v2yx);
			vTemp2 = vcombine_f32(v3yz, v3xy);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

			// vResult + term3 * V1wwwz
			const float32x2_t v1wz = vrev64_f32(v1zw);

			vTemp1 = vcombine_f32(vdup_lane_f32(v1wz, 0), v1wz);
			return vmlaq_f32(vResult, vTerm, vTemp1);
#elif defined(RAY_SSE_INTRINSICS)
	// V2zwyz * V3wzwy
			VectorType vResult = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 1, 3, 2));
			VectorType vTemp3 = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 3, 2, 3));
			vResult = _mm_mul_ps(vResult, vTemp3);
			// - V2wzwy * V3zwyz
			VectorType vTemp2 = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 3, 2, 3));
			vTemp3 = RAYMATH_PERMUTE_PS(vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp2);
			// term1 * V1yxxx
			VectorType vTemp1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 1));
			vResult = _mm_mul_ps(vResult, vTemp1);

			// V2ywxz * V3wxwx
			vTemp2 = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp3 = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 3, 0, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			// - V2wxwx * V3ywxz
			vTemp2 = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 1, 2, 1));
			vTemp1 = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp1);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp2);
			// vResult - temp * V1zzyy
			vTemp1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 2, 2));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp1);

			// V2yzxy * V3zxyx
			vTemp2 = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp3 = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 1, 0, 2));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			// - V2zxyx * V3yzxy
			vTemp2 = RAYMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
			vTemp1 = RAYMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp1);
			// vResult + term * V1wwwz
			vTemp1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 3, 3, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp1);
			vResult = _mm_add_ps(vResult, vTemp3);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4LengthSq
		(
			FVectorType V
		)
		{
			return Vector4Dot(V, V);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4ReciprocalLengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = Result.ReciprocalSqrtEst();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			return vcombine_f32(v2, v2);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_rsqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			vLengthSq = _mm_rsqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4ReciprocalLength
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = Result.ReciprocalSqrt();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt
			float32x2_t  S0 = vrsqrte_f32(v1);
			float32x2_t  P0 = vmul_f32(v1, S0);
			float32x2_t  R0 = vrsqrts_f32(P0, S0);
			float32x2_t  S1 = vmul_f32(S0, R0);
			float32x2_t  P1 = vmul_f32(v1, S1);
			float32x2_t  R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0xff);
			VectorType vLengthSq = _mm_sqrt_ps(vTemp);
			return _mm_div_ps(Constants::One, vLengthSq);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			vLengthSq = _mm_div_ps(Constants::One, vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			// Accurate!
			vLengthSq = _mm_div_ps(Constants::One, vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4LengthEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = Result.SqrtEst();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt (estimate)
			float32x2_t Result = vrsqrte_f32(v1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Length
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS) 

			Vector Result;

			Result = Vector4LengthSq(V);
			Result = Result.Sqrt();

			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			return vcombine_f32(Result, Result);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0xff);
			return _mm_sqrt_ps(vTemp);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return vLengthSq;
#endif
		}

		//------------------------------------------------------------------------------
		// Vector4NormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline VectorType RAYMATH_CALLCONV Vector4NormalizeEst
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType Result;
			Result = Vector4ReciprocalLength(V);
			Result = Vector::Multiply(V, Result);
			return Result;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			// Reciprocal sqrt (estimate)
			v2 = vrsqrte_f32(v1);
			// Normalize
			return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(V, V, 0xff);
			VectorType vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, V);
#elif defined(RAY_SSE3_INTRINSICS)
			VectorType vDot = _mm_mul_ps(V, V);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_hadd_ps(vDot, vDot);
			vDot = _mm_rsqrt_ps(vDot);
			vDot = _mm_mul_ps(vDot, V);
			return vDot;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the reciprocal
			VectorType vResult = _mm_rsqrt_ps(vLengthSq);
			// Reciprocal mul to perform the normalization
			vResult = _mm_mul_ps(vResult, V);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Normalize
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float fLength;
			VectorType vResult;

			vResult = Vector4Length(V);
			fLength = vResult.vector4_f32[0];

			// Prevent divide by zero
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.vector4_f32[0] = V.vector4_f32[0] * fLength;
			vResult.vector4_f32[1] = V.vector4_f32[1] * fLength;
			vResult.vector4_f32[2] = V.vector4_f32[2] * fLength;
			vResult.vector4_f32[3] = V.vector4_f32[3] * fLength;
			return vResult;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Dot4
			float32x4_t vTemp = vmulq_f32(V, V);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(Constants::Infinity));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			VectorType vResult = vmulq_f32(V, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vcombine_f32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
			return vbslq_f32(vcombine_f32(VEqualsInf, VEqualsInf), Constants::QNaN, vResult);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vLengthSq = _mm_dp_ps(V, V, 0xff);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE3_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y,z and w
			VectorType vLengthSq = _mm_mul_ps(V, V);
			// vTemp has z and w
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Create zero with a single instruction
			VectorType vZeroMask = _mm_setzero_ps();
			// Test for a divide by zero (Must be FP to detect -0.0)
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Divide to perform the normalization
			vResult = _mm_div_ps(V, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vZeroMask);
			// Select qnan or result based on infinite length
			VectorType vTemp1 = _mm_andnot_ps(vLengthSq, Constants::QNaN);
			VectorType vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4ClampLength
		(
			FVectorType V,
			float    LengthMin,
			float    LengthMax
		)
		{
			VectorType ClampMax = Vector::Replicate(LengthMax);
			VectorType ClampMin = Vector::Replicate(LengthMin);

			return Vector4ClampLengthV(V, ClampMin, ClampMax);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4ClampLengthV
		(
			FVectorType V,
			FVectorType LengthMin,
			FVectorType LengthMax
		)
		{
			assert((Vector(LengthMin).GetY() == Vector(LengthMin).GetX()) && (Vector(LengthMin).GetZ() == Vector(LengthMin).GetX()) && (Vector(LengthMin).GetW() == Vector(LengthMin).GetX()));
			assert((Vector(LengthMax).GetY() == Vector(LengthMax).GetX()) && (Vector(LengthMax).GetZ() == Vector(LengthMax).GetX()) && (Vector(LengthMax).GetW() == Vector(LengthMax).GetX()));
			assert(Vector4GreaterOrEqual(LengthMin, Vector::Zero()));
			assert(Vector4GreaterOrEqual(LengthMax, Vector::Zero()));
			assert(Vector4GreaterOrEqual(LengthMax, LengthMin));

			Vector LengthSq = Vector4LengthSq(V);

			const VectorType Zero = Vector::Zero();

			VectorType RcpLength = LengthSq.ReciprocalSqrt();

			VectorType InfiniteLength = Vector::EqualInt(LengthSq, Constants::Infinity.v);
			VectorType ZeroLength = Vector::Equal(LengthSq, Zero);

			VectorType Normal = Vector::Multiply(V, RcpLength);

			Vector Length = Vector::Multiply(LengthSq, RcpLength);

			VectorType Select = Vector::EqualInt(InfiniteLength, ZeroLength);
			Length = Vector::Select(LengthSq, Length, Select);
			Normal = Vector::Select(LengthSq, Normal, Select);

			VectorType ControlMax = Length.Greater(LengthMax);
			VectorType ControlMin = Length.Less(LengthMin);

			VectorType ClampLength = Vector::Select(Length, LengthMax, ControlMax);
			ClampLength = Vector::Select(ClampLength, LengthMin, ControlMin);

			VectorType Result = Vector::Multiply(Normal, ClampLength);

			// Preserve the original vector (with no precision loss) if the length falls within the given range
			VectorType Control = Vector::EqualInt(ControlMax, ControlMin);
			Result = Vector::Select(Result, V, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Reflect
		(
			FVectorType Incident,
			FVectorType Normal
		)
		{
			// Result = Incident - (2 * dot(Incident, Normal)) * Normal

			VectorType Result = Vector4Dot(Incident, Normal);
			Result = Vector::Add(Result, Result);
			Result = VectorNegativeMultiplySubtract(Result, Normal, Incident);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Refract
		(
			FVectorType Incident,
			FVectorType Normal,
			float    RefractionIndex
		)
		{
			VectorType Index = Vector::Replicate(RefractionIndex);
			return Vector4RefractV(Incident, Normal, Index);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4RefractV
		(
			FVectorType Incident,
			FVectorType Normal,
			FVectorType RefractionIndex
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorType        IDotN;
			Vector			  R;
			const VectorType  Zero = Vector::Zero();

			// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + 
			// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

			IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			R = VectorNegativeMultiplySubtract(IDotN, IDotN, Constants::One.v);
			R = Vector::Multiply(R, RefractionIndex);
			R = VectorNegativeMultiplySubtract(R, RefractionIndex, Constants::One.v);

			if (Vector4LessOrEqual(R, Zero))
			{
				// Total internal reflection
				return Zero;
			}
			else
			{
				VectorType Result;

				// R = RefractionIndex * IDotN + sqrt(R)
				R = R.Sqrt();
				R = VectorMultiplyAdd(RefractionIndex, IDotN, R);

				// Result = RefractionIndex * Incident - Normal * R
				Result = Vector::Multiply(RefractionIndex, Incident);
				Result = VectorNegativeMultiplySubtract(Normal, R, Result);

				return Result;
			}

#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			float32x4_t R = vmlsq_f32(Constants::One, IDotN, IDotN);
			R = vmulq_f32(R, RefractionIndex);
			R = vmlsq_f32(Constants::One, R, RefractionIndex);

			uint32x4_t vResult = vcleq_f32(R, Constants::Zero);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vResult), vget_high_u8(vResult));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			if (vget_lane_u32(vTemp.val[1], 1) == 0xFFFFFFFFU)
			{
				// Total internal reflection
				vResult = Constants::Zero;
			}
			else
			{
				// Sqrt(R)
				float32x4_t S0 = vrsqrteq_f32(R);
				float32x4_t P0 = vmulq_f32(R, S0);
				float32x4_t R0 = vrsqrtsq_f32(P0, S0);
				float32x4_t S1 = vmulq_f32(S0, R0);
				float32x4_t P1 = vmulq_f32(R, S1);
				float32x4_t R1 = vrsqrtsq_f32(P1, S1);
				float32x4_t S2 = vmulq_f32(S1, R1);
				R = vmulq_f32(R, S2);
				// R = RefractionIndex * IDotN + sqrt(R)
				R = vmlaq_f32(R, RefractionIndex, IDotN);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = vmulq_f32(RefractionIndex, Incident);
				vResult = vmlsq_f32(vResult, R, Normal);
			}
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			VectorType IDotN = Vector4Dot(Incident, Normal);

			// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
			VectorType R = _mm_mul_ps(IDotN, IDotN);
			R = _mm_sub_ps(Constants::One, R);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_mul_ps(R, RefractionIndex);
			R = _mm_sub_ps(Constants::One, R);

			VectorType vResult = _mm_cmple_ps(R, Constants::Zero);
			if (_mm_movemask_ps(vResult) == 0x0f)
			{
				// Total internal reflection
				vResult = Constants::Zero;
			}
			else
			{
				// R = RefractionIndex * IDotN + sqrt(R)
				R = _mm_sqrt_ps(R);
				vResult = _mm_mul_ps(RefractionIndex, IDotN);
				R = _mm_add_ps(R, vResult);
				// Result = RefractionIndex * Incident - Normal * R
				vResult = _mm_mul_ps(RefractionIndex, Incident);
				R = _mm_mul_ps(R, Normal);
				vResult = _mm_sub_ps(vResult, R);
			}
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Orthogonal
		(
			FVectorType V
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 Result = { { {
					V.vector4_f32[2],
					V.vector4_f32[3],
					-V.vector4_f32[0],
					-V.vector4_f32[1]
				} } };
			return Result.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 Negate = { { { 1.f, 1.f, -1.f, -1.f } } };

			float32x4_t Result = vcombine_f32(vget_high_f32(V), vget_low_f32(V));
			return vmulq_f32(Result, Negate);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 FlipZW = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			VectorType vResult = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 0, 3, 2));
			vResult = _mm_mul_ps(vResult, FlipZW);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4AngleBetweenNormalsEst
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector4Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne.v, Constants::One.v);
			Result = VectorACosEst(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4AngleBetweenNormals
		(
			FVectorType N1,
			FVectorType N2
		)
		{
			Vector Result = Vector4Dot(N1, N2);
			Result = Result.Clamp(Constants::NegativeOne.v, Constants::One.v);
			Result = VectorACos(Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4AngleBetweenVectors
		(
			FVectorType V1,
			FVectorType V2
		)
		{
			VectorType L1 = Vector4ReciprocalLength(V1);
			VectorType L2 = Vector4ReciprocalLength(V2);

			VectorType Dot = Vector4Dot(V1, V2);

			L1 = Vector::Multiply(L1, L2);

			Vector CosAngle = Vector::Multiply(Dot, L1);
			CosAngle = CosAngle.Clamp(Constants::NegativeOne.v, Constants::One.v);

			return VectorACos(CosAngle);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV Vector4Transform
		(
			FVectorType V,
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			float fX = (M.m[0][0] * V.vector4_f32[0]) + (M.m[1][0] * V.vector4_f32[1]) + (M.m[2][0] * V.vector4_f32[2]) + (M.m[3][0] * V.vector4_f32[3]);
			float fY = (M.m[0][1] * V.vector4_f32[0]) + (M.m[1][1] * V.vector4_f32[1]) + (M.m[2][1] * V.vector4_f32[2]) + (M.m[3][1] * V.vector4_f32[3]);
			float fZ = (M.m[0][2] * V.vector4_f32[0]) + (M.m[1][2] * V.vector4_f32[1]) + (M.m[2][2] * V.vector4_f32[2]) + (M.m[3][2] * V.vector4_f32[3]);
			float fW = (M.m[0][3] * V.vector4_f32[0]) + (M.m[1][3] * V.vector4_f32[1]) + (M.m[2][3] * V.vector4_f32[2]) + (M.m[3][3] * V.vector4_f32[3]);
			VectorF32 vResult = { { { fX, fY, fZ, fW } } };
			return vResult.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(V);
			VectorType vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
			vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
			float32x2_t VH = vget_high_f32(V);
			vResult = vmlaq_lane_f32(vResult, M.r[2], VH, 0); // Z
			return vmlaq_lane_f32(vResult, M.r[3], VH, 1); // W
#elif defined(RAY_SSE_INTRINSICS)
			// Splat x,y,z and w
			VectorType vTempX = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
			VectorType vTempY = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType vTempZ = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			VectorType vTempW = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
			// Mul by the matrix
			vTempX = _mm_mul_ps(vTempX, M.r[0]);
			vTempY = _mm_mul_ps(vTempY, M.r[1]);
			vTempZ = _mm_mul_ps(vTempZ, M.r[2]);
			vTempW = _mm_mul_ps(vTempW, M.r[3]);
			// Add them all together
			vTempX = _mm_add_ps(vTempX, vTempY);
			vTempZ = _mm_add_ps(vTempZ, vTempW);
			vTempX = _mm_add_ps(vTempX, vTempZ);
			return vTempX;
#endif
		}

		//------------------------------------------------------------------------------

		inline Float4* RAYMATH_CALLCONV Vector4TransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float4* pInputStream,
			size_t          InputStride,
			size_t          VectorCount,
			FMatrix       M
		)
		{
			assert(pOutputStream != nullptr);
			assert(pInputStream != nullptr);

			assert(InputStride >= sizeof(Float4));
			assert(OutputStride >= sizeof(Float4));

#if defined(RAY_NO_INTRINSICS)

			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			for (size_t i = 0; i < VectorCount; i++)
			{
				VectorType V = LoadFloat4(reinterpret_cast<const Float4*>(pInputVector));
				VectorType W = Vector::SplatW(V);
				VectorType Z = Vector::SplatZ(V);
				VectorType Y = Vector::SplatY(V);
				VectorType X = Vector::SplatX(V);

				VectorType Result = Vector::Multiply(W, row3);
				Result = VectorMultiplyAdd(Z, row2, Result);
				Result = VectorMultiplyAdd(Y, row1, Result);
				Result = VectorMultiplyAdd(X, row0, Result);

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable : 26015, "PREfast noise: Esp:1307" )
#endif

				StoreFloat4(reinterpret_cast<Float4*>(pOutputVector), Result);

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

				pInputVector += InputStride;
				pOutputVector += OutputStride;
			}

			return pOutputStream;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			size_t i = 0;
			size_t four = VectorCount >> 2;
			if (four > 0)
			{
				if ((InputStride == sizeof(Float4)) && (OutputStride == sizeof(Float4)))
				{
					for (size_t j = 0; j < four; ++j)
					{
						float32x4x4_t V = vld4q_f32(reinterpret_cast<const float*>(pInputVector));
						pInputVector += sizeof(Float4) * 4;

						float32x2_t r = vget_low_f32(row0);
						VectorType vResult0 = vmulq_lane_f32(V.val[0], r, 0); // Ax
						VectorType vResult1 = vmulq_lane_f32(V.val[0], r, 1); // Bx

						__prefetch(pInputVector);

						r = vget_high_f32(row0);
						VectorType vResult2 = vmulq_lane_f32(V.val[0], r, 0); // Cx
						VectorType vResult3 = vmulq_lane_f32(V.val[0], r, 1); // Dx

						__prefetch(pInputVector + RAYMATH_CACHE_LINE_SIZE);

						r = vget_low_f32(row1);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[1], r, 0); // Ax+Ey
						vResult1 = vmlaq_lane_f32(vResult1, V.val[1], r, 1); // Bx+Fy

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 2));

						r = vget_high_f32(row1);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[1], r, 0); // Cx+Gy
						vResult3 = vmlaq_lane_f32(vResult3, V.val[1], r, 1); // Dx+Hy

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 3));

						r = vget_low_f32(row2);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[2], r, 0); // Ax+Ey+Iz
						vResult1 = vmlaq_lane_f32(vResult1, V.val[2], r, 1); // Bx+Fy+Jz

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 4));

						r = vget_high_f32(row2);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[2], r, 0); // Cx+Gy+Kz
						vResult3 = vmlaq_lane_f32(vResult3, V.val[2], r, 1); // Dx+Hy+Lz

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 5));

						r = vget_low_f32(row3);
						vResult0 = vmlaq_lane_f32(vResult0, V.val[3], r, 0); // Ax+Ey+Iz+Mw
						vResult1 = vmlaq_lane_f32(vResult1, V.val[3], r, 1); // Bx+Fy+Jz+Nw

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 6));

						r = vget_high_f32(row3);
						vResult2 = vmlaq_lane_f32(vResult2, V.val[3], r, 0); // Cx+Gy+Kz+Ow
						vResult3 = vmlaq_lane_f32(vResult3, V.val[3], r, 1); // Dx+Hy+Lz+Pw

						__prefetch(pInputVector + (RAYMATH_CACHE_LINE_SIZE * 7));

						V.val[0] = vResult0;
						V.val[1] = vResult1;
						V.val[2] = vResult2;
						V.val[3] = vResult3;

						vst4q_f32(reinterpret_cast<float*>(pOutputVector), V);
						pOutputVector += sizeof(Float4) * 4;

						i += 4;
					}
				}
			}

			for (; i < VectorCount; i++)
			{
				VectorType V = vld1q_f32(reinterpret_cast<const float*>(pInputVector));
				pInputVector += InputStride;

				float32x2_t VL = vget_low_f32(V);
				VectorType vResult = vmulq_lane_f32(row0, VL, 0); // X
				vResult = vmlaq_lane_f32(vResult, row1, VL, 1); // Y
				float32x2_t VH = vget_high_f32(V);
				vResult = vmlaq_lane_f32(vResult, row2, VH, 0); // Z
				vResult = vmlaq_lane_f32(vResult, row3, VH, 1); // W

				vst1q_f32(reinterpret_cast<float*>(pOutputVector), vResult);
				pOutputVector += OutputStride;
			}

			return pOutputStream;
#elif defined(RAY_SSE_INTRINSICS)
			auto pInputVector = reinterpret_cast<const uint8_t*>(pInputStream);
			auto pOutputVector = reinterpret_cast<uint8_t*>(pOutputStream);

			const VectorType row0 = M.r[0];
			const VectorType row1 = M.r[1];
			const VectorType row2 = M.r[2];
			const VectorType row3 = M.r[3];

			if (!(reinterpret_cast<uintptr_t>(pOutputStream) & 0xF) && !(OutputStride & 0xF))
			{
				if (!(reinterpret_cast<uintptr_t>(pInputStream) & 0xF) && !(InputStride & 0xF))
				{
					// Aligned input, aligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_load_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						VectorType vTempX = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VectorType vTempY = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType vTempZ = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						VectorType vTempW = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Unaligned input, aligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						VectorType vTempX = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VectorType vTempY = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType vTempZ = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						VectorType vTempW = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						RAYMATH_STREAM_PS(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
			}
			else
			{
				if (!(reinterpret_cast<uintptr_t>(pInputStream) & 0xF) && !(InputStride & 0xF))
				{
					// Aligned input, unaligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_load_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						VectorType vTempX = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VectorType vTempY = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType vTempZ = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						VectorType vTempW = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
				else
				{
					// Unaligned input, unaligned output
					for (size_t i = 0; i < VectorCount; i++)
					{
						__m128 V = _mm_loadu_ps(reinterpret_cast<const float*>(pInputVector));
						pInputVector += InputStride;

						VectorType vTempX = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
						VectorType vTempY = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
						VectorType vTempZ = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
						VectorType vTempW = RAYMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));

						vTempX = _mm_mul_ps(vTempX, row0);
						vTempY = _mm_mul_ps(vTempY, row1);
						vTempZ = _mm_mul_ps(vTempZ, row2);
						vTempW = _mm_mul_ps(vTempW, row3);

						vTempX = _mm_add_ps(vTempX, vTempY);
						vTempZ = _mm_add_ps(vTempZ, vTempW);
						vTempX = _mm_add_ps(vTempX, vTempZ);

						_mm_storeu_ps(reinterpret_cast<float*>(pOutputVector), vTempX);
						pOutputVector += OutputStride;
					}
				}
			}

			RAYMATH_SFENCE();

			return pOutputStream;
#endif
		}

		/****************************************************************************
		 *
		 * VectorType operators
		 *
		 ****************************************************************************/

#ifndef RAYMATH_NO_VECTOR_OVERLOADS

		 //------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator+ (FVectorType V)
		{
			return V;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator- (FVectorType V)
		{
			return Vector::Negate(V);
		}

		//------------------------------------------------------------------------------

		inline VectorType& RAYMATH_CALLCONV operator+=
			(
				VectorType& V1,
				FVectorType       V2
				)
		{
			V1 = Vector::Add(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline VectorType& RAYMATH_CALLCONV operator-=
			(
				VectorType& V1,
				FVectorType       V2
				)
		{
			V1 = Vector::Subtract(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline VectorType& RAYMATH_CALLCONV operator*=
			(
				VectorType& V1,
				FVectorType       V2
				)
		{
			V1 = Vector::Multiply(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline VectorType& RAYMATH_CALLCONV operator/=
			(
				VectorType& V1,
				FVectorType       V2
				)
		{
			V1 = Vector::Divide(V1, V2);
			return V1;
		}

		//------------------------------------------------------------------------------

		inline VectorType& operator*=
			(
				VectorType& V,
				const float S
				)
		{
			V = Vector(V).Scale(S);
			return V;
		}

		//------------------------------------------------------------------------------

		inline VectorType& operator/=
			(
				VectorType& V,
				const float S
				)
		{
			VectorType vS = Vector::Replicate(S);
			V = Vector::Divide(V, vS);
			return V;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator+
			(
				FVectorType V1,
				FVectorType V2
				)
		{
			return Vector::Add(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator-
			(
				FVectorType V1,
				FVectorType V2
				)
		{
			return Vector::Subtract(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator*
			(
				FVectorType V1,
				FVectorType V2
				)
		{
			return Vector::Multiply(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator/
			(
				FVectorType V1,
				FVectorType V2
				)
		{
			return Vector::Divide(V1, V2);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator*
			(
				FVectorType      V,
				const float    S
				)
		{
			return Vector(V).Scale(S);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator/
			(
				FVectorType      V,
				const float    S
				)
		{
			VectorType vS = Vector::Replicate(S);
			return Vector::Divide(V, vS);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV operator*
			(
				float           S,
				FVectorType  	    V
				)
		{
			return Vector(V).Scale(S);
		}

#endif /* !RAYMATH_NO_VECTOR_OVERLOADS */

#if defined(RAY_NO_INTRINSICS)
#undef RAYMATH_ISNAN
#undef RAYMATH_ISINF
#endif

#if defined(RAY_SSE_INTRINSICS)
#undef RAYMATH3_UNPACK3_INTO4
#undef RAYMATH3_PACK4_INTO3
#endif



		//-------------------------------------------------------------------------------------
	// DirectXMathMatrix.inl -- SIMD C++ Math library
	//
	// Copyright (c) Microsoft Corporation. All rights reserved.
	// Licensed under the MIT License.
	//
	// http://go.microsoft.com/fwlink/?LinkID=615560
	//-------------------------------------------------------------------------------------


	/****************************************************************************
	 *
	 * Matrix
	 *
	 ****************************************************************************/

	 //------------------------------------------------------------------------------
	 // Comparison operations
	 //------------------------------------------------------------------------------

	 //------------------------------------------------------------------------------

	 // Return true if any entry in the matrix is NaN
		inline bool RAYMATH_CALLCONV Matrix::IsNaN() const
		{
#if defined(RAY_NO_INTRINSICS)
			size_t i = 16;
			auto pWork = reinterpret_cast<const uint32_t*>(&m[0][0]);
			do {
				// Fetch value into integer unit
				uint32_t uTest = pWork[0];
				// Remove sign
				uTest &= 0x7FFFFFFFU;
				// NaN is 0x7F800001 through 0x7FFFFFFF inclusive
				uTest -= 0x7F800001U;
				if (uTest < 0x007FFFFFU) {
					break;      // NaN found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Load in registers
			VectorType vX = r[0];
			VectorType vY = r[1];
			VectorType vZ = r[2];
			VectorType vW = r[3];
			// Test themselves to check for NaN
			vX = vmvnq_u32(vceqq_f32(vX, vX));
			vY = vmvnq_u32(vceqq_f32(vY, vY));
			vZ = vmvnq_u32(vceqq_f32(vZ, vZ));
			vW = vmvnq_u32(vceqq_f32(vW, vW));
			// Or all the results
			vX = vorrq_u32(vX, vZ);
			vY = vorrq_u32(vY, vW);
			vX = vorrq_u32(vX, vY);
			// If any tested true, return true
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vX), vget_high_u8(vX));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r != 0);
#elif defined(RAY_SSE_INTRINSICS)
			// Load in registers
			VectorType vX = r[0];
			VectorType vY = r[1];
			VectorType vZ = r[2];
			VectorType vW = r[3];
			// Test themselves to check for NaN
			vX = _mm_cmpneq_ps(vX, vX);
			vY = _mm_cmpneq_ps(vY, vY);
			vZ = _mm_cmpneq_ps(vZ, vZ);
			vW = _mm_cmpneq_ps(vW, vW);
			// Or all the results
			vX = _mm_or_ps(vX, vZ);
			vY = _mm_or_ps(vY, vW);
			vX = _mm_or_ps(vX, vY);
			// If any tested true, return true
			return (_mm_movemask_ps(vX) != 0);
#else
#endif
		}

		//------------------------------------------------------------------------------

		// Return true if any entry in the matrix is +/-INF
		inline bool RAYMATH_CALLCONV Matrix::IsInfinite() const
		{
#if defined(RAY_NO_INTRINSICS)
			size_t i = 16;
			auto pWork = reinterpret_cast<const uint32_t*>(&m[0][0]);
			do {
				// Fetch value into integer unit
				uint32_t uTest = pWork[0];
				// Remove sign
				uTest &= 0x7FFFFFFFU;
				// INF is 0x7F800000
				if (uTest == 0x7F800000U) {
					break;      // INF found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#elif defined(RAY_ARM_NEON_INTRINSICS)
			// Mask off the sign bits
			VectorType vTemp1 = vandq_u32(r[0], Constants::AbsMask);
			VectorType vTemp2 = vandq_u32(r[1], Constants::AbsMask);
			VectorType vTemp3 = vandq_u32(r[2], Constants::AbsMask);
			VectorType vTemp4 = vandq_u32(r[3], Constants::AbsMask);
			// Compare to infinity
			vTemp1 = vceqq_f32(vTemp1, Constants::Infinity);
			vTemp2 = vceqq_f32(vTemp2, Constants::Infinity);
			vTemp3 = vceqq_f32(vTemp3, Constants::Infinity);
			vTemp4 = vceqq_f32(vTemp4, Constants::Infinity);
			// Or the answers together
			vTemp1 = vorrq_u32(vTemp1, vTemp2);
			vTemp3 = vorrq_u32(vTemp3, vTemp4);
			vTemp1 = vorrq_u32(vTemp1, vTemp3);
			// If any are infinity, the signs are true.
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r != 0);
#elif defined(RAY_SSE_INTRINSICS)
			// Mask off the sign bits
			VectorType vTemp1 = _mm_and_ps(r[0], Constants::AbsMask);
			VectorType vTemp2 = _mm_and_ps(r[1], Constants::AbsMask);
			VectorType vTemp3 = _mm_and_ps(r[2], Constants::AbsMask);
			VectorType vTemp4 = _mm_and_ps(r[3], Constants::AbsMask);
			// Compare to infinity
			vTemp1 = _mm_cmpeq_ps(vTemp1, Constants::Infinity);
			vTemp2 = _mm_cmpeq_ps(vTemp2, Constants::Infinity);
			vTemp3 = _mm_cmpeq_ps(vTemp3, Constants::Infinity);
			vTemp4 = _mm_cmpeq_ps(vTemp4, Constants::Infinity);
			// Or the answers together
			vTemp1 = _mm_or_ps(vTemp1, vTemp2);
			vTemp3 = _mm_or_ps(vTemp3, vTemp4);
			vTemp1 = _mm_or_ps(vTemp1, vTemp3);
			// If any are infinity, the signs are true.
			return (_mm_movemask_ps(vTemp1) != 0);
#endif
		}

		//------------------------------------------------------------------------------

		// Return true if the Matrix is equal to identity
		inline bool RAYMATH_CALLCONV Matrix::IsIdentity() const
		{
#if defined(RAY_NO_INTRINSICS)
			// Use the integer pipeline to reduce branching to a minimum
			auto pWork = reinterpret_cast<const uint32_t*>(&m[0][0]);
			// Convert 1.0f to zero and or them together
			uint32_t uOne = pWork[0] ^ 0x3F800000U;
			// Or all the 0.0f entries together
			uint32_t uZero = pWork[1];
			uZero |= pWork[2];
			uZero |= pWork[3];
			// 2nd row
			uZero |= pWork[4];
			uOne |= pWork[5] ^ 0x3F800000U;
			uZero |= pWork[6];
			uZero |= pWork[7];
			// 3rd row
			uZero |= pWork[8];
			uZero |= pWork[9];
			uOne |= pWork[10] ^ 0x3F800000U;
			uZero |= pWork[11];
			// 4th row
			uZero |= pWork[12];
			uZero |= pWork[13];
			uZero |= pWork[14];
			uOne |= pWork[15] ^ 0x3F800000U;
			// If all zero entries are zero, the uZero==0
			uZero &= 0x7FFFFFFF;    // Allow -0.0f
			// If all 1.0f entries are 1.0f, then uOne==0
			uOne |= uZero;
			return (uOne == 0);
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vTemp1 = vceqq_f32(r[0], Constants::IdentityR0);
			VectorType vTemp2 = vceqq_f32(r[1], Constants::IdentityR1);
			VectorType vTemp3 = vceqq_f32(r[2], Constants::IdentityR2);
			VectorType vTemp4 = vceqq_f32(r[3], Constants::IdentityR3);
			vTemp1 = vandq_u32(vTemp1, vTemp2);
			vTemp3 = vandq_u32(vTemp3, vTemp4);
			vTemp1 = vandq_u32(vTemp1, vTemp3);
			int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
			vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
			uint32_t r = vget_lane_u32(vTemp.val[1], 1);
			return (r == 0xFFFFFFFFU);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vTemp1 = _mm_cmpeq_ps(r[0], Constants::IdentityR0);
			VectorType vTemp2 = _mm_cmpeq_ps(r[1], Constants::IdentityR1);
			VectorType vTemp3 = _mm_cmpeq_ps(r[2], Constants::IdentityR2);
			VectorType vTemp4 = _mm_cmpeq_ps(r[3], Constants::IdentityR3);
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			vTemp3 = _mm_and_ps(vTemp3, vTemp4);
			vTemp1 = _mm_and_ps(vTemp1, vTemp3);
			return (_mm_movemask_ps(vTemp1) == 0x0f);
#endif
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Perform a 4x4 matrix multiply by a 4x4 matrix
		inline Matrix RAYMATH_CALLCONV Matrix::Multiply
		(
			FMatrix M1,
			CMatrix M2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			Matrix mResult;
			// Cache the invariants in registers
			float x = M1.m[0][0];
			float y = M1.m[0][1];
			float z = M1.m[0][2];
			float w = M1.m[0][3];
			// Perform the operation on the first row
			mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			// Repeat for all the other rows
			x = M1.m[1][0];
			y = M1.m[1][1];
			z = M1.m[1][2];
			w = M1.m[1][3];
			mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			x = M1.m[2][0];
			y = M1.m[2][1];
			z = M1.m[2][2];
			w = M1.m[2][3];
			mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			x = M1.m[3][0];
			y = M1.m[3][1];
			z = M1.m[3][2];
			w = M1.m[3][3];
			mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
			mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
			mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
			mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
			return mResult;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			Matrix mResult;
			float32x2_t VL = vget_low_f32(M1.r[0]);
			float32x2_t VH = vget_high_f32(M1.r[0]);
			// Perform the operation on the first row
			VectorType vX = vmulq_lane_f32(M2.r[0], VL, 0);
			VectorType vY = vmulq_lane_f32(M2.r[1], VL, 1);
			VectorType vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			VectorType vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[0] = vaddq_f32(vZ, vW);
			// Repeat for the other 3 rows
			VL = vget_low_f32(M1.r[1]);
			VH = vget_high_f32(M1.r[1]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[1] = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[2]);
			VH = vget_high_f32(M1.r[2]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[2] = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[3]);
			VH = vget_high_f32(M1.r[3]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			mResult.r[3] = vaddq_f32(vZ, vW);
			return mResult;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix mResult;
			// Splat the component X,Y,Z then W
#if defined(RAY_AVC_INTRINSICS)
			VectorType vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
			VectorType vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
			VectorType vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
			VectorType vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
	// Use vW to hold the original row
			VectorType vW = M1.r[0];
			VectorType vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			VectorType vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[0] = vX;
			// Repeat for the other 3 rows
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
			vW = M1.r[1];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[1] = vX;
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
			vW = M1.r[2];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[2] = vX;
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
			vW = M1.r[3];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.r[3] = vX;
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::MultiplyTranspose
		(
			FMatrix M1,
			CMatrix M2
		)
		{
#if defined(RAY_NO_INTRINSICS)
			Matrix mResult;
			// Cache the invariants in registers
			float x = M2.m[0][0];
			float y = M2.m[1][0];
			float z = M2.m[2][0];
			float w = M2.m[3][0];
			// Perform the operation on the first row
			mResult.m[0][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[0][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[0][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[0][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			// Repeat for all the other rows
			x = M2.m[0][1];
			y = M2.m[1][1];
			z = M2.m[2][1];
			w = M2.m[3][1];
			mResult.m[1][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[1][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[1][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[1][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			x = M2.m[0][2];
			y = M2.m[1][2];
			z = M2.m[2][2];
			w = M2.m[3][2];
			mResult.m[2][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[2][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[2][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[2][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			x = M2.m[0][3];
			y = M2.m[1][3];
			z = M2.m[2][3];
			w = M2.m[3][3];
			mResult.m[3][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
			mResult.m[3][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
			mResult.m[3][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
			mResult.m[3][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
			return mResult;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x2_t VL = vget_low_f32(M1.r[0]);
			float32x2_t VH = vget_high_f32(M1.r[0]);
			// Perform the operation on the first row
			VectorType vX = vmulq_lane_f32(M2.r[0], VL, 0);
			VectorType vY = vmulq_lane_f32(M2.r[1], VL, 1);
			VectorType vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			VectorType vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r0 = vaddq_f32(vZ, vW);
			// Repeat for the other 3 rows
			VL = vget_low_f32(M1.r[1]);
			VH = vget_high_f32(M1.r[1]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r1 = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[2]);
			VH = vget_high_f32(M1.r[2]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r2 = vaddq_f32(vZ, vW);
			VL = vget_low_f32(M1.r[3]);
			VH = vget_high_f32(M1.r[3]);
			vX = vmulq_lane_f32(M2.r[0], VL, 0);
			vY = vmulq_lane_f32(M2.r[1], VL, 1);
			vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
			vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
			float32x4_t r3 = vaddq_f32(vZ, vW);

			// Transpose result
			float32x4x2_t P0 = vzipq_f32(r0, r2);
			float32x4x2_t P1 = vzipq_f32(r1, r3);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			Matrix mResult;
			mResult.r[0] = T0.val[0];
			mResult.r[1] = T0.val[1];
			mResult.r[2] = T1.val[0];
			mResult.r[3] = T1.val[1];
			return mResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Splat the component X,Y,Z then W
#if defined(RAY_AVC_INTRINSICS)
			VectorType vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
			VectorType vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
			VectorType vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
			VectorType vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
	// Use vW to hold the original row
			VectorType vW = M1.r[0];
			VectorType vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			VectorType vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			VectorType vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			VectorType r0 = vX;
			// Repeat for the other 3 rows
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
			vW = M1.r[1];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			VectorType r1 = vX;
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
			vW = M1.r[2];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			VectorType r2 = vX;
#if defined(RAY_AVC_INTRINSICS)
			vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
			vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
			vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
			vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
			vW = M1.r[3];
			vX = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = RAYMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
			vX = _mm_mul_ps(vX, M2.r[0]);
			vY = _mm_mul_ps(vY, M2.r[1]);
			vZ = _mm_mul_ps(vZ, M2.r[2]);
			vW = _mm_mul_ps(vW, M2.r[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			VectorType r3 = vX;

			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(3, 2, 3, 2));

			Matrix mResult;
			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Transpose() const
		{
#if defined(RAY_NO_INTRINSICS)

			// Original matrix:
			//
			//     m00m01m02m03
			//     m10m11m12m13
			//     m20m21m22m23
			//     m30m31m32m33

			Matrix P;
			P.r[0] = Vector::MergeXY(r[0], r[2]); // m00m20m01m21
			P.r[1] = Vector::MergeXY(r[1], r[3]); // m10m30m11m31
			P.r[2] = Vector::MergeZW(r[0], r[2]); // m02m22m03m23
			P.r[3] = Vector::MergeZW(r[1], r[3]); // m12m32m13m33

			Matrix MT;
			MT.r[0] = Vector::MergeXY(P.r[0], P.r[1]); // m00m10m20m30
			MT.r[1] = Vector::MergeZW(P.r[0], P.r[1]); // m01m11m21m31
			MT.r[2] = Vector::MergeXY(P.r[2], P.r[3]); // m02m12m22m32
			MT.r[3] = Vector::MergeZW(P.r[2], P.r[3]); // m03m13m23m33
			return MT;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float32x4x2_t P0 = vzipq_f32(r[0], r[2]);
			float32x4x2_t P1 = vzipq_f32(r[1], r[3]);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			Matrix mResult;
			mResult.r[0] = T0.val[0];
			mResult.r[1] = T0.val[1];
			mResult.r[2] = T1.val[0];
			mResult.r[3] = T1.val[1];
			return mResult;
#elif defined(RAY_SSE_INTRINSICS)
			// x.x,x.y,y.x,y.y
			VectorType vTemp1 = _mm_shuffle_ps(r[0], r[1], _MM_SHUFFLE(1, 0, 1, 0));
			// x.z,x.w,y.z,y.w
			VectorType vTemp3 = _mm_shuffle_ps(r[0], r[1], _MM_SHUFFLE(3, 2, 3, 2));
			// z.x,z.y,w.x,w.y
			VectorType vTemp2 = _mm_shuffle_ps(r[2], r[3], _MM_SHUFFLE(1, 0, 1, 0));
			// z.z,z.w,w.z,w.w
			VectorType vTemp4 = _mm_shuffle_ps(r[2], r[3], _MM_SHUFFLE(3, 2, 3, 2));
			Matrix mResult;

			// x.x,y.x,z.x,w.x
			mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			// x.y,y.y,z.y,w.y
			mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			// x.z,y.z,z.z,w.z
			mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			// x.w,y.w,z.w,w.w
			mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------
		// Return the inverse and the determinant of a 4x4 matrix

		inline Matrix RAYMATH_CALLCONV Matrix::Inverse(VectorType* pDeterminant) const
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			Matrix MT = Transpose();

			VectorType V0[4], V1[4];
			V0[0] = VectorSwizzle<RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(MT.r[2]);
			V1[0] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W>(MT.r[3]);
			V0[1] = VectorSwizzle<RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(MT.r[0]);
			V1[1] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W>(MT.r[1]);
			V0[2] = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Z>(MT.r[2], MT.r[0]);
			V1[2] = VectorPermute<RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1W>(MT.r[3], MT.r[1]);

			VectorType D0 = Vector::Multiply(V0[0], V1[0]);
			VectorType D1 = Vector::Multiply(V0[1], V1[1]);
			VectorType D2 = Vector::Multiply(V0[2], V1[2]);

			V0[0] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W>(MT.r[2]);
			V1[0] = VectorSwizzle<RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(MT.r[3]);
			V0[1] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W>(MT.r[0]);
			V1[1] = VectorSwizzle<RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(MT.r[1]);
			V0[2] = VectorPermute<RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1W>(MT.r[2], MT.r[0]);
			V1[2] = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Z>(MT.r[3], MT.r[1]);

			D0 = VectorNegativeMultiplySubtract(V0[0], V1[0], D0);
			D1 = VectorNegativeMultiplySubtract(V0[1], V1[1], D1);
			D2 = VectorNegativeMultiplySubtract(V0[2], V1[2], D2);

			V0[0] = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y>(MT.r[1]);
			V1[0] = VectorPermute<RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_0X>(D0, D2);
			V0[1] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X>(MT.r[0]);
			V1[1] = VectorPermute<RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0Z>(D0, D2);
			V0[2] = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y>(MT.r[3]);
			V1[2] = VectorPermute<RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_0X>(D1, D2);
			V0[3] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X>(MT.r[2]);
			V1[3] = VectorPermute<RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0Z>(D1, D2);

			VectorType C0 = Vector::Multiply(V0[0], V1[0]);
			VectorType C2 = Vector::Multiply(V0[1], V1[1]);
			VectorType C4 = Vector::Multiply(V0[2], V1[2]);
			VectorType C6 = Vector::Multiply(V0[3], V1[3]);

			V0[0] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z>(MT.r[1]);
			V1[0] = VectorPermute<RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1X>(D0, D2);
			V0[1] = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Y>(MT.r[0]);
			V1[1] = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_0X>(D0, D2);
			V0[2] = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z>(MT.r[3]);
			V1[2] = VectorPermute<RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1Z>(D1, D2);
			V0[3] = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Y>(MT.r[2]);
			V1[3] = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0X>(D1, D2);

			C0 = VectorNegativeMultiplySubtract(V0[0], V1[0], C0);
			C2 = VectorNegativeMultiplySubtract(V0[1], V1[1], C2);
			C4 = VectorNegativeMultiplySubtract(V0[2], V1[2], C4);
			C6 = VectorNegativeMultiplySubtract(V0[3], V1[3], C6);

			V0[0] = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X>(MT.r[1]);
			V1[0] = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_0Z>(D0, D2);
			V0[1] = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Z>(MT.r[0]);
			V1[1] = VectorPermute<RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1X>(D0, D2);
			V0[2] = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X>(MT.r[3]);
			V1[2] = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0Z>(D1, D2);
			V0[3] = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Z>(MT.r[2]);
			V1[3] = VectorPermute<RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0W, RAYMATH_PERMUTE_1Z>(D1, D2);

			VectorType C1 = VectorNegativeMultiplySubtract(V0[0], V1[0], C0);
			C0 = VectorMultiplyAdd(V0[0], V1[0], C0);
			VectorType C3 = VectorMultiplyAdd(V0[1], V1[1], C2);
			C2 = VectorNegativeMultiplySubtract(V0[1], V1[1], C2);
			VectorType C5 = VectorNegativeMultiplySubtract(V0[2], V1[2], C4);
			C4 = VectorMultiplyAdd(V0[2], V1[2], C4);
			VectorType C7 = VectorMultiplyAdd(V0[3], V1[3], C6);
			C6 = VectorNegativeMultiplySubtract(V0[3], V1[3], C6);

			Matrix R;
			R.r[0] = Vector::Select(C0, C1, Constants::Select0101.v);
			R.r[1] = Vector::Select(C2, C3, Constants::Select0101.v);
			R.r[2] = Vector::Select(C4, C5, Constants::Select0101.v);
			R.r[3] = Vector::Select(C6, C7, Constants::Select0101.v);

			Vector Determinant = Vector4Dot(R.r[0], MT.r[0]);

			if (pDeterminant != nullptr)
				*pDeterminant = Determinant;

			VectorType Reciprocal = Determinant.Reciprocal();

			Matrix Result;
			Result.r[0] = Vector::Multiply(R.r[0], Reciprocal);
			Result.r[1] = Vector::Multiply(R.r[1], Reciprocal);
			Result.r[2] = Vector::Multiply(R.r[2], Reciprocal);
			Result.r[3] = Vector::Multiply(R.r[3], Reciprocal);
			return Result;

#elif defined(RAY_SSE_INTRINSICS)
			Matrix MT = Transpose();
			VectorType V00 = RAYMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
			VectorType V10 = RAYMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
			VectorType V01 = RAYMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
			VectorType V11 = RAYMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
			VectorType V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
			VectorType V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

			VectorType D0 = _mm_mul_ps(V00, V10);
			VectorType D1 = _mm_mul_ps(V01, V11);
			VectorType D2 = _mm_mul_ps(V02, V12);

			V00 = RAYMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
			V10 = RAYMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
			V01 = RAYMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
			V11 = RAYMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
			V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
			V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			D0 = _mm_sub_ps(D0, V00);
			D1 = _mm_sub_ps(D1, V01);
			D2 = _mm_sub_ps(D2, V02);
			// V11 = D0Y,D0W,D2Y,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
			V00 = RAYMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
			V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
			V01 = RAYMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
			V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
			// V13 = D1Y,D1W,D2W,D2W
			VectorType V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
			V02 = RAYMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
			V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
			VectorType V03 = RAYMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
			V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

			VectorType C0 = _mm_mul_ps(V00, V10);
			VectorType C2 = _mm_mul_ps(V01, V11);
			VectorType C4 = _mm_mul_ps(V02, V12);
			VectorType C6 = _mm_mul_ps(V03, V13);

			// V11 = D0X,D0Y,D2X,D2X
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
			V00 = RAYMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
			V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V01 = RAYMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
			V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
			// V13 = D1X,D1Y,D2Z,D2Z
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
			V02 = RAYMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
			V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
			V03 = RAYMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
			V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			C0 = _mm_sub_ps(C0, V00);
			C2 = _mm_sub_ps(C2, V01);
			C4 = _mm_sub_ps(C4, V02);
			C6 = _mm_sub_ps(C6, V03);

			V00 = RAYMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
			// V10 = D0Z,D0Z,D2X,D2Y
			V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
			V10 = RAYMATH_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
			V01 = RAYMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
			// V11 = D0X,D0W,D2X,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
			V11 = RAYMATH_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
			V02 = RAYMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
			// V12 = D1Z,D1Z,D2Z,D2W
			V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
			V12 = RAYMATH_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
			V03 = RAYMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
			// V13 = D1X,D1W,D2Z,D2W
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
			V13 = RAYMATH_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			VectorType C1 = _mm_sub_ps(C0, V00);
			C0 = _mm_add_ps(C0, V00);
			VectorType C3 = _mm_add_ps(C2, V01);
			C2 = _mm_sub_ps(C2, V01);
			VectorType C5 = _mm_sub_ps(C4, V02);
			C4 = _mm_add_ps(C4, V02);
			VectorType C7 = _mm_add_ps(C6, V03);
			C6 = _mm_sub_ps(C6, V03);

			C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
			C0 = RAYMATH_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = RAYMATH_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = RAYMATH_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = RAYMATH_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
			// Get the determinate
			VectorType vTemp = Vector4Dot(C0, MT.r[0]);
			if (pDeterminant != nullptr)
				*pDeterminant = vTemp;
			vTemp = _mm_div_ps(Constants::One, vTemp);
			Matrix mResult;
			mResult.r[0] = _mm_mul_ps(C0, vTemp);
			mResult.r[1] = _mm_mul_ps(C2, vTemp);
			mResult.r[2] = _mm_mul_ps(C4, vTemp);
			mResult.r[3] = _mm_mul_ps(C6, vTemp);
			return mResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Vector RAYMATH_CALLCONV Matrix::Determinant() const
		{
			static const VectorF32 Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };

			VectorType V0 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X>(r[2]);
			VectorType V1 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(r[3]);
			VectorType V2 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X>(r[2]);
			VectorType V3 = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z>(r[3]);
			VectorType V4 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(r[2]);
			VectorType V5 = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z>(r[3]);

			VectorType P0 = Vector::Multiply(V0, V1);
			VectorType P1 = Vector::Multiply(V2, V3);
			VectorType P2 = Vector::Multiply(V4, V5);

			V0 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(r[2]);
			V1 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X>(r[3]);
			V2 = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z>(r[2]);
			V3 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X>(r[3]);
			V4 = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z>(r[2]);
			V5 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(r[3]);

			P0 = VectorNegativeMultiplySubtract(V0, V1, P0);
			P1 = VectorNegativeMultiplySubtract(V2, V3, P1);
			P2 = VectorNegativeMultiplySubtract(V4, V5, P2);

			V0 = VectorSwizzle<RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_W, RAYMATH_SWIZZLE_Z>(r[1]);
			V1 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Y>(r[1]);
			V2 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X>(r[1]);

			VectorType S = Vector::Multiply(r[0], Sign.v);
			VectorType R = Vector::Multiply(V0, P0);
			R = VectorNegativeMultiplySubtract(V1, P1, R);
			R = VectorMultiplyAdd(V2, P2, R);

			return Vector4Dot(S, R);
		}

#define RAYMATH3_RANKED_DECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }

#define RAYMATH3_DECOMP_EPSILON 0.0001f


		inline bool RAYMATH_CALLCONV Matrix::Decompose
		(
			VectorType* outScale,
			VectorType* outRotQuat,
			VectorType* outTrans
		) const
		{
			static const VectorType* pvCanonicalBasis[3] = {
				&Constants::IdentityR0.v,
				&Constants::IdentityR1.v,
				&Constants::IdentityR2.v
			};

			assert(outScale != nullptr);
			assert(outRotQuat != nullptr);
			assert(outTrans != nullptr);

			// Get the translation
			outTrans[0] = r[3];

			Vector* ppvBasis[3];
			Matrix matTemp;
			// RAYMATH_TODO: Cast possibly invalid
			ppvBasis[0] = (Vector*)&matTemp.r[0];
			ppvBasis[1] = (Vector*)&matTemp.r[1];
			ppvBasis[2] = (Vector*)&matTemp.r[2];

			matTemp.r[0] = r[0];
			matTemp.r[1] = r[1];
			matTemp.r[2] = r[2];
			matTemp.r[3] = Constants::IdentityR3.v;

			auto pfScales = reinterpret_cast<float*>(outScale);

			size_t a, b, c;
			VectorGetXPtr(&pfScales[0], Vector3Length(ppvBasis[0][0]));
			VectorGetXPtr(&pfScales[1], Vector3Length(ppvBasis[1][0]));
			VectorGetXPtr(&pfScales[2], Vector3Length(ppvBasis[2][0]));
			pfScales[3] = 0.f;

			RAYMATH3_RANKED_DECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2])

				if (pfScales[a] < RAYMATH3_DECOMP_EPSILON)
				{
					ppvBasis[a][0] = pvCanonicalBasis[a][0];
				}
			ppvBasis[a][0] = Vector3Normalize(ppvBasis[a][0]);

			if (pfScales[b] < RAYMATH3_DECOMP_EPSILON)
			{
				size_t aa, bb, cc;
				float fAbsX, fAbsY, fAbsZ;

				fAbsX = fabsf(ppvBasis[a][0].GetX());
				fAbsY = fabsf(ppvBasis[a][0].GetY());
				fAbsZ = fabsf(ppvBasis[a][0].GetZ());

				RAYMATH3_RANKED_DECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ)

					ppvBasis[b][0] = Vector3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
			}

			ppvBasis[b][0] = Vector3Normalize(ppvBasis[b][0]);

			if (pfScales[c] < RAYMATH3_DECOMP_EPSILON)
			{
				ppvBasis[c][0] = Vector3Cross(ppvBasis[a][0], ppvBasis[b][0]);
			}

			ppvBasis[c][0] = Vector3Normalize(ppvBasis[c][0]);

			float fDet = matTemp.Determinant().GetX();

			// use Kramer's rule to check for handedness of coordinate system
			if (fDet < 0.0f)
			{
				// switch coordinate system by negating the scale and inverting the basis vector on the x-axis
				pfScales[a] = -pfScales[a];
				ppvBasis[a][0] = Vector::Negate(ppvBasis[a][0]);

				fDet = -fDet;
			}

			fDet -= 1.0f;
			fDet *= fDet;

			if (RAYMATH3_DECOMP_EPSILON < fDet)
			{
				// Non-SRT matrix encountered
				return false;
			}

			// generate the quaternion from the matrix
			outRotQuat[0] = Quaternion::RotationMatrix(matTemp);
			return true;
		}

#undef RAYMATH3_DECOMP_EPSILON
#undef RAYMATH3_RANKED_DECOMPOSE

		//------------------------------------------------------------------------------
		// Transformation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Identity()
		{
			Matrix M;
			M.r[0] = Constants::IdentityR0.v;
			M.r[1] = Constants::IdentityR1.v;
			M.r[2] = Constants::IdentityR2.v;
			M.r[3] = Constants::IdentityR3.v;
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Set
		(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			Matrix M;
#if defined(RAY_NO_INTRINSICS)
			M.m[0][0] = m00; M.m[0][1] = m01; M.m[0][2] = m02; M.m[0][3] = m03;
			M.m[1][0] = m10; M.m[1][1] = m11; M.m[1][2] = m12; M.m[1][3] = m13;
			M.m[2][0] = m20; M.m[2][1] = m21; M.m[2][2] = m22; M.m[2][3] = m23;
			M.m[3][0] = m30; M.m[3][1] = m31; M.m[3][2] = m32; M.m[3][3] = m33;
#else
			M.r[0] = Vector::Set(m00, m01, m02, m03);
			M.r[1] = Vector::Set(m10, m11, m12, m13);
			M.r[2] = Vector::Set(m20, m21, m22, m23);
			M.r[3] = Vector::Set(m30, m31, m32, m33);
#endif
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Translation
		(
			float OffsetX,
			float OffsetY,
			float OffsetZ
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = OffsetX;
			M.m[3][1] = OffsetY;
			M.m[3][2] = OffsetZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_SSE_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)
			Matrix M;
			M.r[0] = Constants::IdentityR0.v;
			M.r[1] = Constants::IdentityR1.v;
			M.r[2] = Constants::IdentityR2.v;
			M.r[3] = Vector::Set(OffsetX, OffsetY, OffsetZ, 1.f);
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Translation(const Float3& d)
		{
			return Translation(d.x, d.y, d.z);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Scaling(const Float3& d)
		{
			return Scaling(d.x, d.y, d.z);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Translation
		(
			FVectorType Offset
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = Offset.vector4_f32[0];
			M.m[3][1] = Offset.vector4_f32[1];
			M.m[3][2] = Offset.vector4_f32[2];
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_SSE_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)
			Matrix M;
			M.r[0] = Constants::IdentityR0.v;
			M.r[1] = Constants::IdentityR1.v;
			M.r[2] = Constants::IdentityR2.v;
			M.r[3] = Vector::Select(Constants::IdentityR3.v, Offset, Constants::Select1110.v);
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Scaling
		(
			float ScaleX,
			float ScaleY,
			float ScaleZ
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.m[0][0] = ScaleX;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ScaleY;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = ScaleZ;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ScaleX, Zero, 0);
			M.r[1] = vsetq_lane_f32(ScaleY, Zero, 1);
			M.r[2] = vsetq_lane_f32(ScaleZ, Zero, 2);
			M.r[3] = Constants::IdentityR3.v;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_set_ps(0, 0, 0, ScaleX);
			M.r[1] = _mm_set_ps(0, 0, ScaleY, 0);
			M.r[2] = _mm_set_ps(0, ScaleZ, 0, 0);
			M.r[3] = Constants::IdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Scaling
		(
			FVectorType Scale
		)
		{
#if defined(RAY_NO_INTRINSICS)

			Matrix M;
			M.m[0][0] = Scale.vector4_f32[0];
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Scale.vector4_f32[1];
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = Scale.vector4_f32[2];
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			Matrix M;
			M.r[0] = vandq_u32(Scale, Constants::MaskX);
			M.r[1] = vandq_u32(Scale, Constants::MaskY);
			M.r[2] = vandq_u32(Scale, Constants::MaskZ);
			M.r[3] = Constants::IdentityR3.v;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			M.r[0] = _mm_and_ps(Scale, Constants::MaskX);
			M.r[1] = _mm_and_ps(Scale, Constants::MaskY);
			M.r[2] = _mm_and_ps(Scale, Constants::MaskZ);
			M.r[3] = Constants::IdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationX
		(
			float Angle
		)
		{
#if defined(RAY_NO_INTRINSICS)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = fSinAngle;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = -fSinAngle;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const VectorType Zero = vdupq_n_f32(0);

			VectorType T1 = vsetq_lane_f32(fCosAngle, Zero, 1);
			T1 = vsetq_lane_f32(fSinAngle, T1, 2);

			VectorType T2 = vsetq_lane_f32(-fSinAngle, Zero, 1);
			T2 = vsetq_lane_f32(fCosAngle, T2, 2);

			Matrix M;
			M.r[0] = Constants::IdentityR0.v;
			M.r[1] = T1;
			M.r[2] = T2;
			M.r[3] = Constants::IdentityR3.v;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			VectorType vSin = _mm_set_ss(SinAngle);
			VectorType vCos = _mm_set_ss(CosAngle);
			// x = 0,y = cos,z = sin, w = 0
			vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
			Matrix M;
			M.r[0] = Constants::IdentityR0;
			M.r[1] = vCos;
			// x = 0,y = sin,z = cos, w = 0
			vCos = RAYMATH_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
			// x = 0,y = -sin,z = cos, w = 0
			vCos = _mm_mul_ps(vCos, Constants::NegateY);
			M.r[2] = vCos;
			M.r[3] = Constants::IdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationY
		(
			float Angle
		)
		{
#if defined(RAY_NO_INTRINSICS)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = 0.0f;
			M.m[0][2] = -fSinAngle;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = fSinAngle;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const VectorType Zero = vdupq_n_f32(0);

			VectorType T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
			T0 = vsetq_lane_f32(-fSinAngle, T0, 2);

			VectorType T2 = vsetq_lane_f32(fSinAngle, Zero, 0);
			T2 = vsetq_lane_f32(fCosAngle, T2, 2);

			Matrix M;
			M.r[0] = T0;
			M.r[1] = Constants::IdentityR1.v;
			M.r[2] = T2;
			M.r[3] = Constants::IdentityR3.v;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			VectorType vSin = _mm_set_ss(SinAngle);
			VectorType vCos = _mm_set_ss(CosAngle);
			// x = sin,y = 0,z = cos, w = 0
			vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
			Matrix M;
			M.r[2] = vSin;
			M.r[1] = Constants::IdentityR1;
			// x = cos,y = 0,z = sin, w = 0
			vSin = RAYMATH_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
			// x = cos,y = 0,z = -sin, w = 0
			vSin = _mm_mul_ps(vSin, Constants::NegateZ);
			M.r[0] = vSin;
			M.r[3] = Constants::IdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationZ
		(
			float Angle
		)
		{
#if defined(RAY_NO_INTRINSICS)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			Matrix M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = fSinAngle;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = -fSinAngle;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			const VectorType Zero = vdupq_n_f32(0);

			VectorType T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
			T0 = vsetq_lane_f32(fSinAngle, T0, 1);

			VectorType T1 = vsetq_lane_f32(-fSinAngle, Zero, 0);
			T1 = vsetq_lane_f32(fCosAngle, T1, 1);

			Matrix M;
			M.r[0] = T0;
			M.r[1] = T1;
			M.r[2] = Constants::IdentityR2.v;
			M.r[3] = Constants::IdentityR3.v;
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			float    SinAngle;
			float    CosAngle;
			ScalarSinCos(&SinAngle, &CosAngle, Angle);

			VectorType vSin = _mm_set_ss(SinAngle);
			VectorType vCos = _mm_set_ss(CosAngle);
			// x = cos,y = sin,z = 0, w = 0
			vCos = _mm_unpacklo_ps(vCos, vSin);
			Matrix M;
			M.r[0] = vCos;
			// x = sin,y = cos,z = 0, w = 0
			vCos = RAYMATH_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
			// x = cos,y = -sin,z = 0, w = 0
			vCos = _mm_mul_ps(vCos, Constants::NegateX);
			M.r[1] = vCos;
			M.r[2] = Constants::IdentityR2;
			M.r[3] = Constants::IdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationRollPitchYaw
		(
			float Pitch,
			float Yaw,
			float Roll
		)
		{
			VectorType Angles = Vector::Set(Pitch, Yaw, Roll, 0.0f);
			return Matrix::RotationRollPitchYawFromVector(Angles);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationRollPitchYawFromVector
		(
			FVectorType Angles // <Pitch, Yaw, Roll, undefined>
		)
		{
			VectorType Q = Quaternion::RotationRollPitchYawFromVector(Angles);
			return Matrix::RotationFromQuaternion(Q);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationNormal
		(
			FVectorType NormalAxis,
			float     Angle
		)
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			VectorType A = Vector::Set(fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f);

			VectorType C2 = Vector::SplatZ(A);
			VectorType C1 = Vector::SplatY(A);
			VectorType C0 = Vector::SplatX(A);

			VectorType N0 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_W>(NormalAxis);
			VectorType N1 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_W>(NormalAxis);

			VectorType V0 = Vector::Multiply(C2, N0);
			V0 = Vector::Multiply(V0, N1);

			VectorType R0 = Vector::Multiply(C2, NormalAxis);
			R0 = VectorMultiplyAdd(R0, NormalAxis, C1);

			VectorType R1 = VectorMultiplyAdd(C0, NormalAxis, V0);
			VectorType R2 = VectorNegativeMultiplySubtract(C0, NormalAxis, V0);

			V0 = Vector::Select(A, R0, Constants::Select1110.v);
			VectorType V1 = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0X>(R1, R2);
			VectorType V2 = VectorPermute<RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1X>(R1, R2);

			Matrix M;
			M.r[0] = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0W>(V0, V1);
			M.r[1] = VectorPermute<RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_0W>(V0, V1);
			M.r[2] = VectorPermute<RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0W>(V0, V2);
			M.r[3] = Constants::IdentityR3.v;
			return M;

#elif defined(RAY_SSE_INTRINSICS)
			float    fSinAngle;
			float    fCosAngle;
			ScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			VectorType C2 = _mm_set_ps1(1.0f - fCosAngle);
			VectorType C1 = _mm_set_ps1(fCosAngle);
			VectorType C0 = _mm_set_ps1(fSinAngle);

			VectorType N0 = RAYMATH_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
			VectorType N1 = RAYMATH_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

			VectorType V0 = _mm_mul_ps(C2, N0);
			V0 = _mm_mul_ps(V0, N1);

			VectorType R0 = _mm_mul_ps(C2, NormalAxis);
			R0 = _mm_mul_ps(R0, NormalAxis);
			R0 = _mm_add_ps(R0, C1);

			VectorType R1 = _mm_mul_ps(C0, NormalAxis);
			R1 = _mm_add_ps(R1, V0);
			VectorType R2 = _mm_mul_ps(C0, NormalAxis);
			R2 = _mm_sub_ps(V0, R2);

			V0 = _mm_and_ps(R0, Constants::Mask3);
			VectorType V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
			V1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
			VectorType V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
			V2 = RAYMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
			R2 = RAYMATH_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

			Matrix M;
			M.r[0] = R2;

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
			R2 = RAYMATH_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
			M.r[1] = R2;

			V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
			M.r[2] = V2;
			M.r[3] = Constants::IdentityR3.v;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationAxis
		(
			FVectorType Axis,
			float     Angle
		)
		{
			assert(!Vector3Equal(Axis, Vector::Zero()));
			assert(!Vector3IsInfinite(Axis));

			VectorType Normal = Vector3Normalize(Axis);
			return Matrix::RotationNormal(Normal, Angle);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::RotationFromQuaternion
		(
			const Quaternion& Quaternion
		)
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			static const VectorF32 Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

			VectorType Q0 = Vector::Add(Quaternion, Quaternion);
			VectorType Q1 = Vector::Multiply(Quaternion, Q0);

			VectorType V0 = VectorPermute<RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1W>(Q1, Constant1110.v);
			VectorType V1 = VectorPermute<RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1W>(Q1, Constant1110.v);
			VectorType R0 = Vector::Subtract(Constant1110, V0);
			R0 = Vector::Subtract(R0, V1);

			V0 = VectorSwizzle<RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_W>(Quaternion);
			V1 = VectorSwizzle<RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_W>(Q0);
			V0 = Vector::Multiply(V0, V1);

			V1 = Vector::SplatW(Quaternion);
			VectorType V2 = VectorSwizzle<RAYMATH_SWIZZLE_Y, RAYMATH_SWIZZLE_Z, RAYMATH_SWIZZLE_X, RAYMATH_SWIZZLE_W>(Q0);
			V1 = Vector::Multiply(V1, V2);

			VectorType R1 = Vector::Add(V0, V1);
			VectorType R2 = Vector::Subtract(V0, V1);

			V0 = VectorPermute<RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Z>(R1, R2);
			V1 = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1Z>(R1, R2);

			Matrix M;
			M.r[0] = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0W>(R0, V0);
			M.r[1] = VectorPermute<RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1W, RAYMATH_PERMUTE_0W>(R0, V0);
			M.r[2] = VectorPermute<RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_0W>(R0, V1);
			M.r[3] = Constants::IdentityR3.v;
			return M;

#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32  Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

			VectorType Q0 = _mm_add_ps(Quaternion, Quaternion);
			VectorType Q1 = _mm_mul_ps(Quaternion, Q0);

			VectorType V0 = RAYMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
			V0 = _mm_and_ps(V0, Constants::Mask3);
			VectorType V1 = RAYMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
			V1 = _mm_and_ps(V1, Constants::Mask3);
			VectorType R0 = _mm_sub_ps(Constant1110, V0);
			R0 = _mm_sub_ps(R0, V1);

			V0 = RAYMATH_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
			V1 = RAYMATH_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
			V0 = _mm_mul_ps(V0, V1);

			V1 = RAYMATH_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
			VectorType V2 = RAYMATH_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
			V1 = _mm_mul_ps(V1, V2);

			VectorType R1 = _mm_add_ps(V0, V1);
			VectorType R2 = _mm_sub_ps(V0, V1);

			V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
			V0 = RAYMATH_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
			V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
			V1 = RAYMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

			Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
			Q1 = RAYMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

			Matrix M;
			M.r[0] = Q1;

			Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
			Q1 = RAYMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
			M.r[1] = Q1;

			Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
			M.r[2] = Q1;
			M.r[3] = Constants::IdentityR3;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Transformation2D
		(
			FVectorType ScalingOrigin,
			float     ScalingOrientation,
			FVectorType Scaling,
			FVectorType RotationOrigin,
			float     Rotation,
			GVectorType Translation
		)
		{
			// M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
			//         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			VectorType VScalingOrigin = Vector::Select(Constants::Select1100.v, ScalingOrigin, Constants::Select1100.v);
			VectorType NegScalingOrigin = Vector::Negate(VScalingOrigin);

			Matrix MScalingOriginI = Matrix::Translation(NegScalingOrigin);
			Matrix MScalingOrientation = Matrix::RotationZ(ScalingOrientation);
			Matrix MScalingOrientationT = MScalingOrientation.Transpose();
			VectorType VScaling = Vector::Select(Constants::One.v, Scaling, Constants::Select1100.v);
			Matrix MScaling = Matrix::Scaling(VScaling);
			VectorType VRotationOrigin = Vector::Select(Constants::Select1100.v, RotationOrigin, Constants::Select1100.v);
			Matrix MRotation = Matrix::RotationZ(Rotation);
			VectorType VTranslation = Vector::Select(Constants::Select1100.v, Translation, Constants::Select1100.v);

			Matrix M = Matrix::Multiply(MScalingOriginI, MScalingOrientationT);
			M = Matrix::Multiply(M, MScaling);
			M = Matrix::Multiply(M, MScalingOrientation);
			M.r[3] = Vector::Add(M.r[3], VScalingOrigin);
			M.r[3] = Vector::Subtract(M.r[3], VRotationOrigin);
			M = Matrix::Multiply(M, MRotation);
			M.r[3] = Vector::Add(M.r[3], VRotationOrigin);
			M.r[3] = Vector::Add(M.r[3], VTranslation);

			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Transformation
		(
			FVectorType ScalingOrigin,
			FVectorType ScalingOrientationQuaternion,
			FVectorType Scaling,
			GVectorType RotationOrigin,
			HVectorType RotationFromQuaternion,
			HVectorType Translation
		)
		{
			// M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
			//         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			VectorType VScalingOrigin = Vector::Select(Constants::Select1110.v, ScalingOrigin, Constants::Select1110.v);
			VectorType NegScalingOrigin = Vector::Negate(ScalingOrigin);

			Matrix MScalingOriginI = Matrix::Translation(NegScalingOrigin);
			Matrix MScalingOrientation = Matrix::RotationFromQuaternion(ScalingOrientationQuaternion);
			Matrix MScalingOrientationT = MScalingOrientation.Transpose();
			Matrix MScaling = Matrix::Scaling(Scaling);
			VectorType VRotationOrigin = Vector::Select(Constants::Select1110.v, RotationOrigin, Constants::Select1110.v);
			Matrix MRotation = Matrix::RotationFromQuaternion(RotationFromQuaternion);
			VectorType VTranslation = Vector::Select(Constants::Select1110.v, Translation, Constants::Select1110.v);

			Matrix M;
			M = Matrix::Multiply(MScalingOriginI, MScalingOrientationT);
			M = Matrix::Multiply(M, MScaling);
			M = Matrix::Multiply(M, MScalingOrientation);
			M.r[3] = Vector::Add(M.r[3], VScalingOrigin);
			M.r[3] = Vector::Subtract(M.r[3], VRotationOrigin);
			M = Matrix::Multiply(M, MRotation);
			M.r[3] = Vector::Add(M.r[3], VRotationOrigin);
			M.r[3] = Vector::Add(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::AffineTransformation2D
		(
			FVectorType Scaling,
			FVectorType RotationOrigin,
			float     Rotation,
			FVectorType Translation
		)
		{
			// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			VectorType VScaling = Vector::Select(Constants::One.v, Scaling, Constants::Select1100.v);
			Matrix MScaling = Matrix::Scaling(VScaling);
			VectorType VRotationOrigin = Vector::Select(Constants::Select1100.v, RotationOrigin, Constants::Select1100.v);
			Matrix MRotation = Matrix::RotationZ(Rotation);
			VectorType VTranslation = Vector::Select(Constants::Select1100.v, Translation, Constants::Select1100.v);

			Matrix M;
			M = MScaling;
			M.r[3] = Vector::Subtract(M.r[3], VRotationOrigin);
			M = Matrix::Multiply(M, MRotation);
			M.r[3] = Vector::Add(M.r[3], VRotationOrigin);
			M.r[3] = Vector::Add(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::AffineTransformation
		(
			FVectorType Scaling,
			FVectorType RotationOrigin,
			FVectorType RotationFromQuaternion,
			GVectorType Translation
		)
		{
			// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			Matrix MScaling = Matrix::Scaling(Scaling);
			VectorType VRotationOrigin = Vector::Select(Constants::Select1110.v, RotationOrigin, Constants::Select1110.v);
			Matrix MRotation = Matrix::RotationFromQuaternion(RotationFromQuaternion);
			VectorType VTranslation = Vector::Select(Constants::Select1110.v, Translation, Constants::Select1110.v);

			Matrix M;
			M = MScaling;
			M.r[3] = Vector::Subtract(M.r[3], VRotationOrigin);
			M = Matrix::Multiply(M, MRotation);
			M.r[3] = Vector::Add(M.r[3], VRotationOrigin);
			M.r[3] = Vector::Add(M.r[3], VTranslation);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Reflect
		(
			FVectorType ReflectionPlane
		)
		{
			assert(!Vector3Equal(ReflectionPlane, Vector::Zero()));
			assert(!PlaneIsInfinite(ReflectionPlane));

			static const VectorF32 NegativeTwo = { { { -2.0f, -2.0f, -2.0f, 0.0f } } };

			VectorType P = PlaneNormalize(ReflectionPlane);
			VectorType S = Vector::Multiply(P, NegativeTwo);

			VectorType A = Vector::SplatX(P);
			VectorType B = Vector::SplatY(P);
			VectorType C = Vector::SplatZ(P);
			VectorType D = Vector::SplatW(P);

			Matrix M;
			M.r[0] = VectorMultiplyAdd(A, S, Constants::IdentityR0.v);
			M.r[1] = VectorMultiplyAdd(B, S, Constants::IdentityR1.v);
			M.r[2] = VectorMultiplyAdd(C, S, Constants::IdentityR2.v);
			M.r[3] = VectorMultiplyAdd(D, S, Constants::IdentityR3.v);
			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::Shadow
		(
			FVectorType ShadowPlane,
			FVectorType LightPosition
		)
		{
			static const VectorU32 Select0001 = { { { RAYMATH_SELECT_0, RAYMATH_SELECT_0, RAYMATH_SELECT_0, RAYMATH_SELECT_1 } } };

			assert(!Vector3Equal(ShadowPlane, Vector::Zero()));
			assert(!PlaneIsInfinite(ShadowPlane));

			VectorType P = PlaneNormalize(ShadowPlane);
			VectorType Dot = PlaneDot(P, LightPosition);
			P = Vector::Negate(P);
			VectorType D = Vector::SplatW(P);
			VectorType C = Vector::SplatZ(P);
			VectorType B = Vector::SplatY(P);
			VectorType A = Vector::SplatX(P);
			Dot = Vector::Select(Select0001.v, Dot, Select0001.v);

			Matrix M;
			M.r[3] = VectorMultiplyAdd(D, LightPosition, Dot);
			Dot = Vector::RotateLeft(Dot, 1);
			M.r[2] = VectorMultiplyAdd(C, LightPosition, Dot);
			Dot = Vector::RotateLeft(Dot, 1);
			M.r[1] = VectorMultiplyAdd(B, LightPosition, Dot);
			Dot = Vector::RotateLeft(Dot, 1);
			M.r[0] = VectorMultiplyAdd(A, LightPosition, Dot);
			return M;
		}

		//------------------------------------------------------------------------------
		// View and projection initialization operations
		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::LookAtLH
		(
			FVectorType EyePosition,
			FVectorType FocusPosition,
			FVectorType UpDirection
		)
		{
			VectorType EyeDirection = Vector::Subtract(FocusPosition, EyePosition);
			return Matrix::LookToLH(EyePosition, EyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::LookAtRH
		(
			FVectorType EyePosition,
			FVectorType FocusPosition,
			FVectorType UpDirection
		)
		{
			VectorType NegEyeDirection = Vector::Subtract(EyePosition, FocusPosition);
			return Matrix::LookToLH(EyePosition, NegEyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::LookToLH
		(
			FVectorType EyePosition,
			FVectorType EyeDirection,
			FVectorType UpDirection
		)
		{
			assert(!Vector3Equal(EyeDirection, Vector::Zero()));
			assert(!Vector3IsInfinite(EyeDirection));
			assert(!Vector3Equal(UpDirection, Vector::Zero()));
			assert(!Vector3IsInfinite(UpDirection));

			VectorType R2 = Vector3Normalize(EyeDirection);

			VectorType R0 = Vector3Cross(UpDirection, R2);
			R0 = Vector3Normalize(R0);

			VectorType R1 = Vector3Cross(R2, R0);

			VectorType NegEyePosition = Vector::Negate(EyePosition);

			VectorType D0 = Vector3Dot(R0, NegEyePosition);
			VectorType D1 = Vector3Dot(R1, NegEyePosition);
			VectorType D2 = Vector3Dot(R2, NegEyePosition);

			Matrix M;
			M.r[0] = Vector::Select(D0, R0, Constants::Select1110.v);
			M.r[1] = Vector::Select(D1, R1, Constants::Select1110.v);
			M.r[2] = Vector::Select(D2, R2, Constants::Select1110.v);
			M.r[3] = Constants::IdentityR3.v;

			M = M.Transpose();

			return M;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::LookToRH
		(
			FVectorType EyePosition,
			FVectorType EyeDirection,
			FVectorType UpDirection
		)
		{
			VectorType NegEyeDirection = Vector::Negate(EyeDirection);
			return Matrix::LookToLH(EyePosition, NegEyeDirection, UpDirection);
		}

		//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable:28931, "PREfast noise: Esp:1266")
#endif

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveLH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = TwoNearZ / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Constants::IdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vValues = _mm_shuffle_ps(vValues, Constants::IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,1.0f
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveRH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = TwoNearZ / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			const VectorType Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Constants::NegIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,-1.0f
			vValues = _mm_shuffle_ps(vValues, Constants::NegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,-1.0f
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveFovLH
		(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
			assert(!ScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = Width;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Height;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float fRange = FarZ / (FarZ - NearZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const VectorType Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(Width, Zero, 0);
			M.r[1] = vsetq_lane_f32(Height, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Constants::IdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			float Height = CosFov / SinFov;
			VectorType rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// CosFov / SinFov,0,0,0
			Matrix M;
			M.r[0] = vTemp;
			// 0,Height / AspectRatio,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, Constants::IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveFovRH
		(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
			assert(!ScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = Width;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Height;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
			float fRange = FarZ / (NearZ - FarZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const VectorType Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(Width, Zero, 0);
			M.r[1] = vsetq_lane_f32(Height, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Constants::NegIdentityR3.v, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			float Height = CosFov / SinFov;
			VectorType rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// CosFov / SinFov,0,0,0
			Matrix M;
			M.r[0] = vTemp;
			// 0,Height / AspectRatio,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,-1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, Constants::NegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,-1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,fRange * NearZ,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveOffCenterLH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = TwoNearZ * ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ * ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[2][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[2][2] = fRange;
			M.m[2][3] = 1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			const VectorType Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
			M.r[2] = Vector::Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				1.0f);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				TwoNearZ * ReciprocalWidth,
				TwoNearZ * ReciprocalHeight,
				-fRange * NearZ,
				0
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ*ReciprocalWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ*ReciprocalHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// 0,0,fRange,1.0f
			M.r[2] = Vector::Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				1.0f);
			// 0,0,-fRange * NearZ,0.0f
			vValues = _mm_and_ps(vValues, Constants::MaskZ);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::PerspectiveOffCenterRH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = TwoNearZ * ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = TwoNearZ * ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = (ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[2][1] = (ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			const VectorType Zero = vdupq_n_f32(0);

			Matrix M;
			M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
			M.r[2] = Vector::Set((ViewLeft + ViewRight) * ReciprocalWidth,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				-1.0f);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				TwoNearZ * ReciprocalWidth,
				TwoNearZ * ReciprocalHeight,
				fRange * NearZ,
				0
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// TwoNearZ*ReciprocalWidth,0,0,0
			M.r[0] = vTemp;
			// 0,TwoNearZ*ReciprocalHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// 0,0,fRange,1.0f
			M.r[2] = Vector::Set((ViewLeft + ViewRight) * ReciprocalWidth,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				-1.0f);
			// 0,0,-fRange * NearZ,0.0f
			vValues = _mm_and_ps(vValues, Constants::MaskZ);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::OrthographicLH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float fRange = 1.0f / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = 2.0f / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 2.0f / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float fRange = 1.0f / (FarZ - NearZ);

			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = vsetq_lane_f32(-fRange * NearZ, Constants::IdentityR3.v, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float fRange = 1.0f / (FarZ - NearZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// 2.0f / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,2.0f / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=-fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, Constants::IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,-fRange * NearZ,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::OrthographicRH
		(
			float ViewWidth,
			float ViewHeight,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float fRange = 1.0f / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = 2.0f / ViewWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 2.0f / ViewHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float fRange = 1.0f / (NearZ - FarZ);

			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = vsetq_lane_f32(fRange * NearZ, Constants::IdentityR3.v, 2);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float fRange = 1.0f / (NearZ - FarZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				fRange * NearZ
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// 2.0f / ViewWidth,0,0,0
			M.r[0] = vTemp;
			// 0,2.0f / ViewHeight,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			M.r[1] = vTemp;
			// x=fRange,y=fRange * NearZ,0,1.0f
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, Constants::IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
			// 0,0,fRange,0.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.r[2] = vTemp;
			// 0,0,fRange * NearZ,1.0f
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.r[3] = vTemp;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::OrthographicOffCenterLH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);

			Matrix M;
			M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.m[3][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.m[3][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.m[3][2] = -fRange * NearZ;
			M.m[3][3] = 1.0f;
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = Vector::Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				-fRange * NearZ,
				1.0f);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			VectorType rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				-NearZ,
				1.0f
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// fReciprocalWidth*2,0,0,0
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.r[0] = vTemp;
			// 0,fReciprocalHeight*2,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.r[1] = vTemp;
			// 0,0,fRange,0.0f
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskZ);
			M.r[2] = vTemp;
			// -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
			vValues = _mm_mul_ps(vValues, rMem2);
			M.r[3] = vValues;
			return M;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::OrthographicOffCenterRH
		(
			float ViewLeft,
			float ViewRight,
			float ViewBottom,
			float ViewTop,
			float NearZ,
			float FarZ
		)
		{
			assert(!ScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
			assert(!ScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(RAY_NO_INTRINSICS)

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);

			Matrix M;
			M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M.r[3] = Vector::Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange * NearZ,
				1.0f);
			return M;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			const VectorType Zero = vdupq_n_f32(0);
			Matrix M;
			M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
			M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
			M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
			M.r[3] = Vector::Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange * NearZ,
				1.0f);
			return M;
#elif defined(RAY_SSE_INTRINSICS)
			Matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			// Note: This is recorded on the stack
			VectorType rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			VectorType rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				NearZ,
				1.0f
			};
			// Copy from memory to SSE register
			VectorType vValues = rMem;
			VectorType vTemp = _mm_setzero_ps();
			// Copy x only
			vTemp = _mm_move_ss(vTemp, vValues);
			// fReciprocalWidth*2,0,0,0
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.r[0] = vTemp;
			// 0,fReciprocalHeight*2,0,0
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskY);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.r[1] = vTemp;
			// 0,0,fRange,0.0f
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, Constants::MaskZ);
			M.r[2] = vTemp;
			// -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
			vValues = _mm_mul_ps(vValues, rMem2);
			M.r[3] = vValues;
			return M;
#endif
		}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

		/****************************************************************************
		 *
		 * Matrix operators and methods
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Matrix::Matrix
		(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			r[0] = Vector::Set(m00, m01, m02, m03);
			r[1] = Vector::Set(m10, m11, m12, m13);
			r[2] = Vector::Set(m20, m21, m22, m23);
			r[3] = Vector::Set(m30, m31, m32, m33);
		}

		//------------------------------------------------------------------------------

		inline Matrix::Matrix
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);
			r[0] = LoadFloat4(reinterpret_cast<const Float4*>(pArray));
			r[1] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 4));
			r[2] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 8));
			r[3] = LoadFloat4(reinterpret_cast<const Float4*>(pArray + 12));
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator- () const
		{
			Matrix R;
			R.r[0] = Vector::Negate(r[0]);
			R.r[1] = Vector::Negate(r[1]);
			R.r[2] = Vector::Negate(r[2]);
			R.r[3] = Vector::Negate(r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix& RAYMATH_CALLCONV Matrix::operator+= (FMatrix M)
		{
			r[0] = Vector::Add(r[0], M.r[0]);
			r[1] = Vector::Add(r[1], M.r[1]);
			r[2] = Vector::Add(r[2], M.r[2]);
			r[3] = Vector::Add(r[3], M.r[3]);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& RAYMATH_CALLCONV Matrix::operator-= (FMatrix M)
		{
			r[0] = Vector::Subtract(r[0], M.r[0]);
			r[1] = Vector::Subtract(r[1], M.r[1]);
			r[2] = Vector::Subtract(r[2], M.r[2]);
			r[3] = Vector::Subtract(r[3], M.r[3]);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& RAYMATH_CALLCONV Matrix::operator*=(FMatrix M)
		{
			*this = Matrix::Multiply(*this, M);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& Matrix::operator*= (float S)
		{
			r[0] = Vector(r[0]).Scale(S);
			r[1] = Vector(r[1]).Scale(S);
			r[2] = Vector(r[2]).Scale(S);
			r[3] = Vector(r[3]).Scale(S);
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Matrix& Matrix::operator/= (float S)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorType vS = Vector::Replicate(S);
			r[0] = Vector::Divide(r[0], vS);
			r[1] = Vector::Divide(r[1], vS);
			r[2] = Vector::Divide(r[2], vS);
			r[3] = Vector::Divide(r[3], vS);
			return *this;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t vS = vdupq_n_f32(S);
			r[0] = vdivq_f32(r[0], vS);
			r[1] = vdivq_f32(r[1], vS);
			r[2] = vdivq_f32(r[2], vS);
			r[3] = vdivq_f32(r[3], vS);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x2_t vS = vdup_n_f32(S);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vcombine_u32(R0, R0);
			r[0] = vmulq_f32(r[0], Reciprocal);
			r[1] = vmulq_f32(r[1], Reciprocal);
			r[2] = vmulq_f32(r[2], Reciprocal);
			r[3] = vmulq_f32(r[3], Reciprocal);
#endif
			return *this;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 vS = _mm_set_ps1(S);
			r[0] = _mm_div_ps(r[0], vS);
			r[1] = _mm_div_ps(r[1], vS);
			r[2] = _mm_div_ps(r[2], vS);
			r[3] = _mm_div_ps(r[3], vS);
			return *this;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::operator+ (FMatrix M) const
		{
			Matrix R;
			R.r[0] = Vector::Add(r[0], M.r[0]);
			R.r[1] = Vector::Add(r[1], M.r[1]);
			R.r[2] = Vector::Add(r[2], M.r[2]);
			R.r[3] = Vector::Add(r[3], M.r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::operator- (FMatrix M) const
		{
			Matrix R;
			R.r[0] = Vector::Subtract(r[0], M.r[0]);
			R.r[1] = Vector::Subtract(r[1], M.r[1]);
			R.r[2] = Vector::Subtract(r[2], M.r[2]);
			R.r[3] = Vector::Subtract(r[3], M.r[3]);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV Matrix::operator*(FMatrix M) const
		{
			return Matrix::Multiply(*this, M);
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator* (float S) const
		{
			Matrix R;
			R.r[0] = Vector(r[0]).Scale(S);
			R.r[1] = Vector(r[1]).Scale(S);
			R.r[2] = Vector(r[2]).Scale(S);
			R.r[3] = Vector(r[3]).Scale(S);
			return R;
		}

		//------------------------------------------------------------------------------

		inline Matrix Matrix::operator/ (float S) const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorType vS = Vector::Replicate(S);
			Matrix R;
			R.r[0] = Vector::Divide(r[0], vS);
			R.r[1] = Vector::Divide(r[1], vS);
			R.r[2] = Vector::Divide(r[2], vS);
			R.r[3] = Vector::Divide(r[3], vS);
			return R;
#elif defined(RAY_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
			float32x4_t vS = vdupq_n_f32(S);
			Matrix R;
			R.r[0] = vdivq_f32(r[0], vS);
			R.r[1] = vdivq_f32(r[1], vS);
			R.r[2] = vdivq_f32(r[2], vS);
			R.r[3] = vdivq_f32(r[3], vS);
#else
			// 2 iterations of Newton-Raphson refinement of reciprocal
			float32x2_t vS = vdup_n_f32(S);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vcombine_u32(R0, R0);
			Matrix R;
			R.r[0] = vmulq_f32(r[0], Reciprocal);
			R.r[1] = vmulq_f32(r[1], Reciprocal);
			R.r[2] = vmulq_f32(r[2], Reciprocal);
			R.r[3] = vmulq_f32(r[3], Reciprocal);
#endif
			return R;
#elif defined(RAY_SSE_INTRINSICS)
			__m128 vS = _mm_set_ps1(S);
			Matrix R;
			R.r[0] = _mm_div_ps(r[0], vS);
			R.r[1] = _mm_div_ps(r[1], vS);
			R.r[2] = _mm_div_ps(r[2], vS);
			R.r[3] = _mm_div_ps(r[3], vS);
			return R;
#endif
		}

		//------------------------------------------------------------------------------

		inline Matrix RAYMATH_CALLCONV operator*
			(
				float S,
				FMatrix M
				)
		{
			Matrix R;
			R.r[0] = Vector(M.r[0]).Scale(S);
			R.r[1] = Vector(M.r[1]).Scale(S);
			R.r[2] = Vector(M.r[2]).Scale(S);
			R.r[3] = Vector(M.r[3]).Scale(S);
			return R;
		}

		/****************************************************************************
		 *
		 * Float3X3 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float3X3::Float3X3
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);
			for (size_t Row = 0; Row < 3; Row++)
			{
				for (size_t Column = 0; Column < 3; Column++)
				{
					m[Row][Column] = pArray[Row * 3 + Column];
				}
			}
		}

		/****************************************************************************
		 *
		 * Float4X3 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float4X3::Float4X3
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];

			m[1][0] = pArray[3];
			m[1][1] = pArray[4];
			m[1][2] = pArray[5];

			m[2][0] = pArray[6];
			m[2][1] = pArray[7];
			m[2][2] = pArray[8];

			m[3][0] = pArray[9];
			m[3][1] = pArray[10];
			m[3][2] = pArray[11];
		}

		/****************************************************************************
		*
		* Float3X4 operators
		*
		****************************************************************************/

		//------------------------------------------------------------------------------

		inline Float3X4::Float3X4
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];
			m[0][3] = pArray[3];

			m[1][0] = pArray[4];
			m[1][1] = pArray[5];
			m[1][2] = pArray[6];
			m[1][3] = pArray[7];

			m[2][0] = pArray[8];
			m[2][1] = pArray[9];
			m[2][2] = pArray[10];
			m[2][3] = pArray[11];
		}

		/****************************************************************************
		 *
		 * Float4X4 operators
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline Float4X4::Float4X4
		(
			const float* pArray
		)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];
			m[0][3] = pArray[3];

			m[1][0] = pArray[4];
			m[1][1] = pArray[5];
			m[1][2] = pArray[6];
			m[1][3] = pArray[7];

			m[2][0] = pArray[8];
			m[2][1] = pArray[9];
			m[2][2] = pArray[10];
			m[2][3] = pArray[11];

			m[3][0] = pArray[12];
			m[3][1] = pArray[13];
			m[3][2] = pArray[14];
			m[3][3] = pArray[15];
		}


		//-------------------------------------------------------------------------------------
		// DirectXMathMisc.inl -- SIMD C++ Math library
		//
		// Copyright (c) Microsoft Corporation. All rights reserved.
		// Licensed under the MIT License.
		//
		// http://go.microsoft.com/fwlink/?LinkID=615560
		//-------------------------------------------------------------------------------------


		/****************************************************************************
		 *
		 * Quaternion
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Quaternion::operator==(FQuaternion other) const
		{
			return Vector4Equal(*this, other);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Quaternion::operator!=(FQuaternion other) const
		{
			return Vector4NotEqual(*this, other);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Quaternion::IsNaN() const
		{
			return Vector4IsNaN(*this);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Quaternion::IsInfinite() const
		{
			return Vector4IsInfinite(*this);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV Quaternion::IsIdentity() const
		{
			return Vector4Equal(*this, Constants::IdentityR3.v);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Dot
		(
			FQuaternion Q1,
			FQuaternion Q2
		)
		{
			return Vector4Dot(Q1, Q2);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Multiply(FQuaternion Q1, FQuaternion Q2)
		{
			// Returns the product Q2*Q1 (which is the concatenation of a rotation Q1 followed by the rotation Q2)

			// [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
			//   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
			//   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
			//   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]

#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					(Q2.v.vector4_f32[3] * Q1.v.vector4_f32[0]) + (Q2.v.vector4_f32[0] * Q1.v.vector4_f32[3]) + (Q2.v.vector4_f32[1] * Q1.v.vector4_f32[2]) - (Q2.v.vector4_f32[2] * Q1.v.vector4_f32[1]),
					(Q2.v.vector4_f32[3] * Q1.v.vector4_f32[1]) - (Q2.v.vector4_f32[0] * Q1.v.vector4_f32[2]) + (Q2.v.vector4_f32[1] * Q1.v.vector4_f32[3]) + (Q2.v.vector4_f32[2] * Q1.v.vector4_f32[0]),
					(Q2.v.vector4_f32[3] * Q1.v.vector4_f32[2]) + (Q2.v.vector4_f32[0] * Q1.v.vector4_f32[1]) - (Q2.v.vector4_f32[1] * Q1.v.vector4_f32[0]) + (Q2.v.vector4_f32[2] * Q1.v.vector4_f32[3]),
					(Q2.v.vector4_f32[3] * Q1.v.vector4_f32[3]) - (Q2.v.vector4_f32[0] * Q1.v.vector4_f32[0]) - (Q2.v.vector4_f32[1] * Q1.v.vector4_f32[1]) - (Q2.v.vector4_f32[2] * Q1.v.vector4_f32[2])
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
			static const VectorF32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			static const VectorF32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };

			float32x2_t Q2L = vget_low_f32(Q2);
			float32x2_t Q2H = vget_high_f32(Q2);

			float32x4_t Q2X = vdupq_lane_f32(Q2L, 0);
			float32x4_t Q2Y = vdupq_lane_f32(Q2L, 1);
			float32x4_t Q2Z = vdupq_lane_f32(Q2H, 0);
			VectorType vResult = vmulq_lane_f32(Q1, Q2H, 1);

			// Mul by Q1WZYX
			float32x4_t vTemp = vrev64q_f32(Q1);
			vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
			Q2X = vmulq_f32(Q2X, vTemp);
			vResult = vmlaq_f32(vResult, Q2X, ControlWZYX);

			// Mul by Q1ZWXY
			vTemp = vrev64q_u32(vTemp);
			Q2Y = vmulq_f32(Q2Y, vTemp);
			vResult = vmlaq_f32(vResult, Q2Y, ControlZWXY);

			// Mul by Q1YXWZ
			vTemp = vrev64q_u32(vTemp);
			vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
			Q2Z = vmulq_f32(Q2Z, vTemp);
			vResult = vmlaq_f32(vResult, Q2Z, ControlYXWZ);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
			static const VectorF32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
			static const VectorF32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };
			// Copy to SSE registers and use as few as possible for x86
			VectorType Q2X = Q2;
			VectorType Q2Y = Q2;
			VectorType Q2Z = Q2;
			VectorType vResult = Q2;
			// Splat with one instruction
			vResult = RAYMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
			Q2X = RAYMATH_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
			Q2Y = RAYMATH_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
			Q2Z = RAYMATH_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
			// Retire Q1 and perform Q1*Q2W
			vResult = _mm_mul_ps(vResult, Q1);
			VectorType Q1Shuffle = Q1;
			// Shuffle the copies of Q1
			Q1Shuffle = RAYMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
			// Mul by Q1WZYX
			Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
			Q1Shuffle = RAYMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
			// Flip the signs on y and z
			Q2X = _mm_mul_ps(Q2X, ControlWZYX);
			// Mul by Q1ZWXY
			Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
			Q1Shuffle = RAYMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
			// Flip the signs on z and w
			Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
			// Mul by Q1YXWZ
			Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
			vResult = _mm_add_ps(vResult, Q2X);
			// Flip the signs on x and w
			Q2Z = _mm_mul_ps(Q2Z, ControlYXWZ);
			Q2Y = _mm_add_ps(Q2Y, Q2Z);
			vResult = _mm_add_ps(vResult, Q2Y);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::operator*(FQuaternion Q2) const
		{
			return Multiply(*this, Q2);
		}

		//------------------------------------------------------------------------------

		inline Quaternion& RAYMATH_CALLCONV Quaternion::operator*=(FQuaternion other)
		{
			*this = *this * other;
			return *this;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::LengthSq() const
		{
			return Vector4LengthSq(*this);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::ReciprocalLength() const
		{
			return Vector4ReciprocalLength(*this);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Length() const
		{
			return Vector4Length(*this);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::NormalizeEst() const
		{
			return Vector4NormalizeEst(*this);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Normalize() const
		{
			return Vector4Normalize(*this);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Conjugate() const
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 Result = { { {
					-v.vector4_f32[0],
					-v.vector4_f32[1],
					-v.vector4_f32[2],
					v.vector4_f32[3]
				} } };
			return Result.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
			return vmulq_f32(*this, NegativeOne3.v);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
			return _mm_mul_ps(*this, NegativeOne3);
#endif
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Inverse() const
		{
			const VectorType  Zero = Vector::Zero();

			Vector L = Vector4LengthSq(*this);
			Quaternion Conjugated = Conjugate();

			VectorType Control = L.LessOrEqual(Constants::Epsilon.v);

			VectorType Result = Vector::Divide(Conjugated, L);

			Result = Vector::Select(Result, Zero, Control);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Ln() const
		{
			static const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

			Vector QW = Vector::SplatW(*this);
			VectorType Q0 = Vector::Select(Constants::Select1110.v, *this, Constants::Select1110.v);

			VectorType ControlW = QW.InBounds(OneMinusEpsilon.v);

			VectorType Theta = VectorACos(QW);
			VectorType SinTheta = VectorSin(Theta);

			VectorType S = Vector::Divide(Theta, SinTheta);

			VectorType Result = Vector::Multiply(Q0, S);
			Result = Vector::Select(Q0, Result, ControlW);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Exp() const
		{
			VectorType Theta = Vector3Length(*this);

			VectorType SinTheta, CosTheta;
			VectorSinCos(&SinTheta, &CosTheta, Theta);

			VectorType S = Vector::Divide(SinTheta, Theta);

			VectorType Result = Vector::Multiply(*this, S);

			const VectorType Zero = Vector::Zero();
			VectorType Control = Vector::NearEqual(Theta, Zero, Constants::Epsilon.v);
			Result = Vector::Select(Result, *this, Control);

			Result = Vector::Select(CosTheta, Result, Constants::Select1110.v);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Slerp
		(
			FQuaternion Q0,
			FQuaternion Q1,
			float    t
		)
		{
			VectorType T = Vector::Replicate(t);
			return Quaternion::SlerpV(Q0, Q1, T);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::SlerpV
		(
			FQuaternion Q0,
			FQuaternion Q1,
			FQuaternion T
		)
		{
			assert((Vector(T).GetY() == Vector(T).GetX()) && (Vector(T).GetZ() == Vector(T).GetX()) && (Vector(T).GetW() == Vector(T).GetX()));

			// Result = Q0 * sin((1.0 - t) * Omega) / sin(Omega) + Q1 * sin(t * Omega) / sin(Omega)

#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

			Vector CosOmega = Vector(Quaternion::Dot(Q0, Q1));

			const VectorType Zero = Vector::Zero();
			VectorType Control = CosOmega.Less(Zero);
			VectorType Sign = Vector::Select(Constants::One.v, Constants::NegativeOne.v, Control);

			CosOmega = Vector::Multiply(CosOmega, Sign);

			Control = CosOmega.Less(OneMinusEpsilon);

			Vector SinOmega = VectorNegativeMultiplySubtract(CosOmega, CosOmega, Constants::One.v);
			SinOmega = SinOmega.Sqrt();

			VectorType Omega = VectorATan2(SinOmega, CosOmega);

			VectorType SignMask = Vector::SplatSignMask();
			VectorType V01 = Vector::ShiftLeft(T, Zero, 2);
			SignMask = Vector::ShiftLeft(SignMask, Zero, 3);
			V01 = VectorXorInt(V01, SignMask);
			V01 = Vector::Add(Constants::IdentityR0.v, V01);

			VectorType InvSinOmega = SinOmega.Reciprocal();

			VectorType S0 = Vector::Multiply(V01, Omega);
			S0 = VectorSin(S0);
			S0 = Vector::Multiply(S0, InvSinOmega);

			S0 = Vector::Select(V01, S0, Control);

			VectorType S1 = Vector::SplatY(S0);
			S0 = Vector::SplatX(S0);

			S1 = Vector::Multiply(S1, Sign);

			VectorType Result = Vector::Multiply(Q0, S0);
			Result = VectorMultiplyAdd(Q1, S1, Result);

			return Result;

#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };
			static const VectorU32 SignMask2 = { { { 0x80000000, 0x00000000, 0x00000000, 0x00000000 } } };

			Vector CosOmega = (Vector)Quaternion::Dot(Q0, Q1);

			const VectorType Zero = Vector::Zero();
			VectorType Control = CosOmega.Less(Zero);
			VectorType Sign = Vector::Select(Constants::One, Constants::NegativeOne, Control);

			CosOmega = _mm_mul_ps(CosOmega, Sign);

			Control = CosOmega.Less(OneMinusEpsilon);

			VectorType SinOmega = _mm_mul_ps(CosOmega, CosOmega);
			SinOmega = _mm_sub_ps(Constants::One, SinOmega);
			SinOmega = _mm_sqrt_ps(SinOmega);

			VectorType Omega = VectorATan2(SinOmega, CosOmega);

			VectorType V01 = RAYMATH_PERMUTE_PS(T, _MM_SHUFFLE(2, 3, 0, 1));
			V01 = _mm_and_ps(V01, Constants::MaskXY);
			V01 = _mm_xor_ps(V01, SignMask2);
			V01 = _mm_add_ps(Constants::IdentityR0, V01);

			VectorType S0 = _mm_mul_ps(V01, Omega);
			S0 = VectorSin(S0);
			S0 = _mm_div_ps(S0, SinOmega);

			S0 = Vector::Select(V01, S0, Control);

			VectorType S1 = Vector::SplatY(S0);
			S0 = Vector::SplatX(S0);

			S1 = _mm_mul_ps(S1, Sign);
			VectorType Result = _mm_mul_ps(Q0, S0);
			S1 = _mm_mul_ps(S1, Q1);
			Result = _mm_add_ps(Result, S1);
			return Result;
#endif
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Squad
		(
			FQuaternion Q0,
			FQuaternion Q1,
			FQuaternion Q2,
			GQuaternion Q3,
			float    t
		)
		{
			VectorType T = Vector::Replicate(t);
			return Quaternion::SquadV(Q0, Q1, Q2, Q3, T);
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::SquadV
		(
			FQuaternion Q0,
			FQuaternion Q1,
			FQuaternion Q2,
			GQuaternion Q3,
			HQuaternion T
		)
		{
			assert((Vector(T).GetY() == Vector(T).GetX()) && (Vector(T).GetZ() == Vector(T).GetX()) && (Vector(T).GetW() == Vector(T).GetX()));

			VectorType TP = T;
			const VectorType Two = VectorSplatConstant(2, 0);

			VectorType Q03 = Quaternion::SlerpV(Q0, Q3, T);
			VectorType Q12 = Quaternion::SlerpV(Q1, Q2, T);

			TP = VectorNegativeMultiplySubtract(TP, TP, TP);
			TP = Vector::Multiply(TP, Two);

			VectorType Result = Quaternion::SlerpV(Q03, Q12, TP);

			return Result;
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV Quaternion::SquadSetup
		(
			VectorType* pA,
			VectorType* pB,
			VectorType* pC,
			FQuaternion  Q0,
			FQuaternion  Q1,
			FQuaternion  Q2,
			GQuaternion  Q3
		)
		{
			assert(pA);
			assert(pB);
			assert(pC);

			Quaternion LS12 = Quaternion(Vector::Add(Q1, Q2)).LengthSq();
			Quaternion LD12 = Quaternion(Vector::Subtract(Q1, Q2)).LengthSq();
			Quaternion SQ2 = Vector::Negate(Q2);

			VectorType Control1 = Vector(LS12).Less(LD12);
			SQ2 = Vector::Select(Q2, SQ2, Control1);

			Quaternion LS01 = Quaternion(Vector::Add(Q0, Q1)).LengthSq();
			Quaternion LD01 = Quaternion(Vector::Subtract(Q0, Q1)).LengthSq();
			Quaternion SQ0 = Vector::Negate(Q0);

			Quaternion LS23 = Quaternion(Vector::Add(SQ2, Q3)).LengthSq();
			Quaternion LD23 = Quaternion(Vector::Subtract(SQ2, Q3)).LengthSq();
			VectorType SQ3 = Vector::Negate(Q3);

			VectorType Control0 = Vector(LS01).Less(LD01);
			VectorType Control2 = Vector(LS23).Less(LD23);

			SQ0 = Vector::Select(Q0, SQ0, Control0);
			SQ3 = Vector::Select(Q3, SQ3, Control2);

			VectorType InvQ1 = Q1.Inverse();
			VectorType InvQ2 = SQ2.Inverse();

			VectorType LnQ0 = Quaternion::Multiply(InvQ1, SQ0).Ln();
			VectorType LnQ2 = Quaternion::Multiply(InvQ1, SQ2).Ln();
			VectorType LnQ1 = Quaternion::Multiply(InvQ2, Q1).Ln();
			VectorType LnQ3 = Quaternion::Multiply(InvQ2, SQ3).Ln();

			const VectorType NegativeOneQuarter = VectorSplatConstant(-1, 2);

			Quaternion ExpQ02 = Vector::Multiply(Vector::Add(LnQ0, LnQ2), NegativeOneQuarter);
			Quaternion ExpQ13 = Vector::Multiply(Vector::Add(LnQ1, LnQ3), NegativeOneQuarter);
			ExpQ02 = ExpQ02.Exp();
			ExpQ13 = ExpQ13.Exp();

			*pA = Quaternion::Multiply(Q1, ExpQ02);
			*pB = Quaternion::Multiply(SQ2, ExpQ13);
			*pC = SQ2;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::BaryCentric
		(
			FQuaternion Q0,
			FQuaternion Q1,
			FQuaternion Q2,
			float    f,
			float    g
		)
		{
			float s = f + g;

			VectorType Result;
			if ((s < 0.00001f) && (s > -0.00001f))
			{
				Result = Q0;
			}
			else
			{
				VectorType Q01 = Quaternion::Slerp(Q0, Q1, s);
				VectorType Q02 = Quaternion::Slerp(Q0, Q2, s);

				Result = Quaternion::Slerp(Q01, Q02, g / s);
			}

			return Result;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::BaryCentricV
		(
			FQuaternion Q0,
			FQuaternion Q1,
			FQuaternion Q2,
			GQuaternion F,
			HQuaternion G
		)
		{
			assert((Vector(F).GetY() == Vector(F).GetX()) && (Vector(F).GetZ() == Vector(F).GetX()) && (Vector(F).GetW() == Vector(F).GetX()));
			assert((Vector(G).GetY() == Vector(G).GetX()) && (Vector(G).GetZ() == Vector(G).GetX()) && (Vector(G).GetW() == Vector(G).GetX()));

			const VectorType Epsilon = VectorSplatConstant(1, 16);

			Vector S = Vector::Add(F, G);

			VectorType Result;
			if (Vector4InBounds(S, Epsilon))
			{
				Result = Q0;
			}
			else
			{
				Quaternion Q01 = Quaternion::SlerpV(Q0, Q1, S);
				Quaternion Q02 = Quaternion::SlerpV(Q0, Q2, S);
				VectorType GS = S.Reciprocal();
				GS = Vector::Multiply(G, GS);

				Result = Quaternion::SlerpV(Q01, Q02, GS);
			}

			return Result;
		}

		//------------------------------------------------------------------------------
		// Transformation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::Identity()
		{
			return Constants::IdentityR3.v;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::RotationRollPitchYaw
		(
			float Pitch,
			float Yaw,
			float Roll
		)
		{
			VectorType Angles = Vector::Set(Pitch, Yaw, Roll, 0.0f);
			VectorType Q = RotationRollPitchYawFromVector(Angles);
			return Q;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::RotationRollPitchYawFromVector
		(
			FVectorType Angles // <Pitch, Yaw, Roll, 0>
		)
		{
			static const VectorF32  Sign = { { { 1.0f, -1.0f, -1.0f, 1.0f } } };

			VectorType HalfAngles = Vector::Multiply(Angles, Constants::OneHalf.v);

			VectorType SinAngles, CosAngles;
			VectorSinCos(&SinAngles, &CosAngles, HalfAngles);

			VectorType P0 = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1X>(SinAngles, CosAngles);
			VectorType Y0 = VectorPermute<RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1Y>(SinAngles, CosAngles);
			VectorType R0 = VectorPermute<RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1Z>(SinAngles, CosAngles);
			VectorType P1 = VectorPermute<RAYMATH_PERMUTE_0X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1X, RAYMATH_PERMUTE_1X>(CosAngles, SinAngles);
			VectorType Y1 = VectorPermute<RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_0Y, RAYMATH_PERMUTE_1Y, RAYMATH_PERMUTE_1Y>(CosAngles, SinAngles);
			VectorType R1 = VectorPermute<RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_1Z, RAYMATH_PERMUTE_0Z, RAYMATH_PERMUTE_1Z>(CosAngles, SinAngles);

			VectorType Q1 = Vector::Multiply(P1, Sign.v);
			VectorType Q0 = Vector::Multiply(P0, Y0);
			Q1 = Vector::Multiply(Q1, Y1);
			Q0 = Vector::Multiply(Q0, R0);
			VectorType Q = VectorMultiplyAdd(Q1, R1, Q0);

			return Q;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::RotationNormal
		(
			FVectorType NormalAxis,
			float    Angle
		)
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			VectorType N = Vector::Select(Constants::One.v, NormalAxis, Constants::Select1110.v);

			float SinV, CosV;
			ScalarSinCos(&SinV, &CosV, 0.5f * Angle);

			VectorType Scale = Vector::Set(SinV, SinV, SinV, CosV);
			return Vector::Multiply(N, Scale);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType N = _mm_and_ps(NormalAxis, Constants::Mask3);
			N = _mm_or_ps(N, Constants::IdentityR3);
			VectorType Scale = _mm_set_ps1(0.5f * Angle);
			VectorType vSine;
			VectorType vCosine;
			VectorSinCos(&vSine, &vCosine, Scale);
			Scale = _mm_and_ps(vSine, Constants::Mask3);
			vCosine = _mm_and_ps(vCosine, Constants::MaskW);
			Scale = _mm_or_ps(Scale, vCosine);
			N = _mm_mul_ps(N, Scale);
			return N;
#endif
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::RotationAxis
		(
			FVectorType Axis,
			float    Angle
		)
		{
			assert(!Vector3Equal(Axis, Vector::Zero()));
			assert(!Vector3IsInfinite(Axis));

			VectorType Normal = Vector3Normalize(Axis);
			VectorType Q = RotationNormal(Normal, Angle);
			return Q;
		}

		//------------------------------------------------------------------------------

		inline Quaternion RAYMATH_CALLCONV Quaternion::RotationMatrix
		(
			FMatrix M
		)
		{
#if defined(RAY_NO_INTRINSICS)

			VectorF32 q;
			float r22 = M.m[2][2];
			if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
			{
				float dif10 = M.m[1][1] - M.m[0][0];
				float omr22 = 1.f - r22;
				if (dif10 <= 0.f)  // x^2 >= y^2
				{
					float fourXSqr = omr22 - dif10;
					float inv4x = 0.5f / sqrtf(fourXSqr);
					q.f[0] = fourXSqr * inv4x;
					q.f[1] = (M.m[0][1] + M.m[1][0]) * inv4x;
					q.f[2] = (M.m[0][2] + M.m[2][0]) * inv4x;
					q.f[3] = (M.m[1][2] - M.m[2][1]) * inv4x;
				}
				else  // y^2 >= x^2
				{
					float fourYSqr = omr22 + dif10;
					float inv4y = 0.5f / sqrtf(fourYSqr);
					q.f[0] = (M.m[0][1] + M.m[1][0]) * inv4y;
					q.f[1] = fourYSqr * inv4y;
					q.f[2] = (M.m[1][2] + M.m[2][1]) * inv4y;
					q.f[3] = (M.m[2][0] - M.m[0][2]) * inv4y;
				}
			}
			else  // z^2 + w^2 >= x^2 + y^2
			{
				float sum10 = M.m[1][1] + M.m[0][0];
				float opr22 = 1.f + r22;
				if (sum10 <= 0.f)  // z^2 >= w^2
				{
					float fourZSqr = opr22 - sum10;
					float inv4z = 0.5f / sqrtf(fourZSqr);
					q.f[0] = (M.m[0][2] + M.m[2][0]) * inv4z;
					q.f[1] = (M.m[1][2] + M.m[2][1]) * inv4z;
					q.f[2] = fourZSqr * inv4z;
					q.f[3] = (M.m[0][1] - M.m[1][0]) * inv4z;
				}
				else  // w^2 >= z^2
				{
					float fourWSqr = opr22 + sum10;
					float inv4w = 0.5f / sqrtf(fourWSqr);
					q.f[0] = (M.m[1][2] - M.m[2][1]) * inv4w;
					q.f[1] = (M.m[2][0] - M.m[0][2]) * inv4w;
					q.f[2] = (M.m[0][1] - M.m[1][0]) * inv4w;
					q.f[3] = fourWSqr * inv4w;
				}
			}
			return q.v;

#elif defined(RAY_ARM_NEON_INTRINSICS)
			static const VectorF32 XMPMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };
			static const VectorU32 Select0110 = { { { RAYMATH_SELECT_0, RAYMATH_SELECT_1, RAYMATH_SELECT_1, RAYMATH_SELECT_0 } } };
			static const VectorU32 Select0010 = { { { RAYMATH_SELECT_0, RAYMATH_SELECT_0, RAYMATH_SELECT_1, RAYMATH_SELECT_0 } } };

			VectorType r0 = M.r[0];
			VectorType r1 = M.r[1];
			VectorType r2 = M.r[2];

			VectorType r00 = vdupq_lane_f32(vget_low_f32(r0), 0);
			VectorType r11 = vdupq_lane_f32(vget_low_f32(r1), 1);
			VectorType r22 = vdupq_lane_f32(vget_high_f32(r2), 0);

			// x^2 >= y^2 equivalent to r11 - r00 <= 0
			VectorType r11mr00 = vsubq_f32(r11, r00);
			VectorType x2gey2 = vcleq_f32(r11mr00, Constants::Zero);

			// z^2 >= w^2 equivalent to r11 + r00 <= 0
			VectorType r11pr00 = vaddq_f32(r11, r00);
			VectorType z2gew2 = vcleq_f32(r11pr00, Constants::Zero);

			// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
			VectorType x2py2gez2pw2 = vcleq_f32(r22, Constants::Zero);

			// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
			VectorType t0 = vmulq_f32(XMPMMP, r00);
			VectorType x2y2z2w2 = vmlaq_f32(t0, XMMPMP, r11);
			x2y2z2w2 = vmlaq_f32(x2y2z2w2, XMMMPP, r22);
			x2y2z2w2 = vaddq_f32(x2y2z2w2, Constants::One);

			// (r01, r02, r12, r11)
			t0 = vextq_f32(r0, r0, 1);
			VectorType t1 = vextq_f32(r1, r1, 1);
			t0 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_low_f32(t1)));

			// (r10, r20, r21, r10)
			t1 = vextq_f32(r2, r2, 3);
			VectorType r10 = vdupq_lane_f32(vget_low_f32(r1), 0);
			t1 = vbslq_f32(Select0110, t1, r10);

			// (4*x*y, 4*x*z, 4*y*z, unused)
			VectorType xyxzyz = vaddq_f32(t0, t1);

			// (r21, r20, r10, r10)
			t0 = vcombine_f32(vrev64_f32(vget_low_f32(r2)), vget_low_f32(r10));

			// (r12, r02, r01, r12)
			VectorType t2 = vcombine_f32(vrev64_f32(vget_high_f32(r0)), vrev64_f32(vget_low_f32(r0)));
			VectorType t3 = vdupq_lane_f32(vget_high_f32(r1), 0);
			t1 = vbslq_f32(Select0110, t2, t3);

			// (4*x*w, 4*y*w, 4*z*w, unused)
			VectorType xwywzw = vsubq_f32(t0, t1);
			xwywzw = vmulq_f32(XMMPMP, xwywzw);

			// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
			t0 = vextq_f32(xyxzyz, xyxzyz, 3);
			t1 = vbslq_f32(Select0110, t0, x2y2z2w2);
			t2 = vdupq_lane_f32(vget_low_f32(xwywzw), 0);
			VectorType tensor0 = vbslq_f32(Constants::Select1110, t1, t2);

			// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
			t0 = vbslq_f32(Constants::Select1011, xyxzyz, x2y2z2w2);
			t1 = vdupq_lane_f32(vget_low_f32(xwywzw), 1);
			VectorType tensor1 = vbslq_f32(Constants::Select1110, t0, t1);

			// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
			t0 = vextq_f32(xyxzyz, xyxzyz, 1);
			t1 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_high_f32(xwywzw)));
			VectorType tensor2 = vbslq_f32(Select0010, x2y2z2w2, t1);

			// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
			VectorType tensor3 = vbslq_f32(Constants::Select1110, xwywzw, x2y2z2w2);

			// Select the row of the tensor-product matrix that has the largest
			// magnitude.
			t0 = vbslq_f32(x2gey2, tensor0, tensor1);
			t1 = vbslq_f32(z2gew2, tensor2, tensor3);
			t2 = vbslq_f32(x2py2gez2pw2, t0, t1);

			// Normalize the row.  No division by zero is possible because the
			// quaternion is unit-length (and the row is a nonzero multiple of
			// the quaternion).
			t0 = Vector4Length(t2);
			return Vector::Divide(t2, t0);
#elif defined(RAY_SSE_INTRINSICS)
			static const VectorF32 XMPMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
			static const VectorF32 XMMMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };

			VectorType r0 = M.r[0];  // (r00, r01, r02, 0)
			VectorType r1 = M.r[1];  // (r10, r11, r12, 0)
			VectorType r2 = M.r[2];  // (r20, r21, r22, 0)

			// (r00, r00, r00, r00)
			VectorType r00 = RAYMATH_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
			// (r11, r11, r11, r11)
			VectorType r11 = RAYMATH_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
			// (r22, r22, r22, r22)
			VectorType r22 = RAYMATH_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));

			// x^2 >= y^2 equivalent to r11 - r00 <= 0
			// (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
			VectorType r11mr00 = _mm_sub_ps(r11, r00);
			VectorType x2gey2 = _mm_cmple_ps(r11mr00, Constants::Zero);

			// z^2 >= w^2 equivalent to r11 + r00 <= 0
			// (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
			VectorType r11pr00 = _mm_add_ps(r11, r00);
			VectorType z2gew2 = _mm_cmple_ps(r11pr00, Constants::Zero);

			// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
			VectorType x2py2gez2pw2 = _mm_cmple_ps(r22, Constants::Zero);

			// (+r00, -r00, -r00, +r00)
			VectorType t0 = _mm_mul_ps(XMPMMP, r00);

			// (-r11, +r11, -r11, +r11)
			VectorType t1 = _mm_mul_ps(XMMPMP, r11);

			// (-r22, -r22, +r22, +r22)
			VectorType t2 = _mm_mul_ps(XMMMPP, r22);

			// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
			VectorType x2y2z2w2 = _mm_add_ps(t0, t1);
			x2y2z2w2 = _mm_add_ps(t2, x2y2z2w2);
			x2y2z2w2 = _mm_add_ps(x2y2z2w2, Constants::One);

			// (r01, r02, r12, r11)
			t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
			// (r10, r10, r20, r21)
			t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
			// (r10, r20, r21, r10)
			t1 = RAYMATH_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
			// (4*x*y, 4*x*z, 4*y*z, unused)
			VectorType xyxzyz = _mm_add_ps(t0, t1);

			// (r21, r20, r10, r10)
			t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
			// (r12, r12, r02, r01)
			t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
			// (r12, r02, r01, r12)
			t1 = RAYMATH_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
			// (4*x*w, 4*y*w, 4*z*w, unused)
			VectorType xwywzw = _mm_sub_ps(t0, t1);
			xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

			// (4*x^2, 4*y^2, 4*x*y, unused)
			t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
			// (4*z^2, 4*w^2, 4*z*w, unused)
			t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
			// (4*x*z, 4*y*z, 4*x*w, 4*y*w)
			t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

			// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
			VectorType tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
			// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
			VectorType tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
			// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
			VectorType tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
			// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
			VectorType tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

			// Select the row of the tensor-product matrix that has the largest
			// magnitude.
			t0 = _mm_and_ps(x2gey2, tensor0);
			t1 = _mm_andnot_ps(x2gey2, tensor1);
			t0 = _mm_or_ps(t0, t1);
			t1 = _mm_and_ps(z2gew2, tensor2);
			t2 = _mm_andnot_ps(z2gew2, tensor3);
			t1 = _mm_or_ps(t1, t2);
			t0 = _mm_and_ps(x2py2gez2pw2, t0);
			t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
			t2 = _mm_or_ps(t0, t1);

			// Normalize the row.  No division by zero is possible because the
			// quaternion is unit-length (and the row is a nonzero multiple of
			// the quaternion).
			t0 = Vector4Length(t2);
			return _mm_div_ps(t2, t0);
#endif
		}

		//------------------------------------------------------------------------------
		// Conversion operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV Quaternion::ToAxisAngle
		(
			VectorType* pAxis,
			float* pAngle
		) const
		{
			assert(pAxis);
			assert(pAngle);

			*pAxis = *this;

			*pAngle = 2.0f * ScalarACos(w);
		}

		/****************************************************************************
		 *
		 * Plane
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV PlaneEqual
		(
			FVectorType P1,
			FVectorType P2
		)
		{
			return Vector4Equal(P1, P2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV PlaneNearEqual
		(
			FVectorType P1,
			FVectorType P2,
			FVectorType Epsilon
		)
		{
			VectorType NP1 = PlaneNormalize(P1);
			VectorType NP2 = PlaneNormalize(P2);
			return Vector4NearEqual(NP1, NP2, Epsilon);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV PlaneNotEqual
		(
			FVectorType P1,
			FVectorType P2
		)
		{
			return Vector4NotEqual(P1, P2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV PlaneIsNaN
		(
			FVectorType P
		)
		{
			return Vector4IsNaN(P);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV PlaneIsInfinite
		(
			FVectorType P
		)
		{
			return Vector4IsInfinite(P);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneDot
		(
			FVectorType P,
			FVectorType V
		)
		{
			return Vector4Dot(P, V);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneDotCoord
		(
			FVectorType P,
			FVectorType V
		)
		{
			// Result = P[0] * V[0] + P[1] * V[1] + P[2] * V[2] + P[3]

			VectorType V3 = Vector::Select(Constants::One.v, V, Constants::Select1110.v);
			VectorType Result = Vector4Dot(P, V3);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneDotNormal
		(
			FVectorType P,
			FVectorType V
		)
		{
			return Vector3Dot(P, V);
		}

		//------------------------------------------------------------------------------
		// PlaneNormalizeEst uses a reciprocal estimate and
		// returns QNaN on zero and infinite vectors.

		inline VectorType RAYMATH_CALLCONV PlaneNormalizeEst
		(
			FVectorType P
		)
		{
#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			VectorType Result = Vector3ReciprocalLengthEst(P);
			return Vector::Multiply(P, Result);

#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vTemp = _mm_dp_ps(P, P, 0x7f);
			VectorType vResult = _mm_rsqrt_ps(vTemp);
			return _mm_mul_ps(vResult, P);
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product
			VectorType vDot = _mm_mul_ps(P, P);
			// x=Dot.y, y=Dot.z
			VectorType vTemp = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
			// Result.x = x+y
			vDot = _mm_add_ss(vDot, vTemp);
			// x=Dot.z
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			// Result.x = (x+y)+z
			vDot = _mm_add_ss(vDot, vTemp);
			// Splat x
			vDot = RAYMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
			// Get the reciprocal
			vDot = _mm_rsqrt_ps(vDot);
			// Get the reciprocal
			vDot = _mm_mul_ps(vDot, P);
			return vDot;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneNormalize
		(
			FVectorType P
		)
		{
#if defined(RAY_NO_INTRINSICS)
			float fLengthSq = sqrtf((P.vector4_f32[0] * P.vector4_f32[0]) + (P.vector4_f32[1] * P.vector4_f32[1]) + (P.vector4_f32[2] * P.vector4_f32[2]));
			// Prevent divide by zero
			if (fLengthSq > 0)
			{
				fLengthSq = 1.0f / fLengthSq;
			}
			VectorF32 vResult = { { {
					P.vector4_f32[0] * fLengthSq,
					P.vector4_f32[1] * fLengthSq,
					P.vector4_f32[2] * fLengthSq,
					P.vector4_f32[3] * fLengthSq
				} }
			};
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vLength = Vector3ReciprocalLength(P);
			return Vector::Multiply(P, vLength);
#elif defined(RAY_SSE4_INTRINSICS)
			VectorType vLengthSq = _mm_dp_ps(P, P, 0x7f);
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(P, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vLengthSq);
			return vResult;
#elif defined(RAY_SSE_INTRINSICS)
			// Perform the dot product on x,y and z only
			VectorType vLengthSq = _mm_mul_ps(P, P);
			VectorType vTemp = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = RAYMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = RAYMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			// Prepare for the division
			VectorType vResult = _mm_sqrt_ps(vLengthSq);
			// Failsafe on zero (Or epsilon) length planes
			// If the length is infinity, set the elements to zero
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Constants::Infinity);
			// Reciprocal mul to perform the normalization
			vResult = _mm_div_ps(P, vResult);
			// Any that are infinity, set to zero
			vResult = _mm_and_ps(vResult, vLengthSq);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneIntersectLine
		(
			FVectorType P,
			FVectorType LinePoint1,
			FVectorType LinePoint2
		)
		{
			VectorType V1 = Vector3Dot(P, LinePoint1);
			VectorType V2 = Vector3Dot(P, LinePoint2);
			VectorType D = Vector::Subtract(V1, V2);

			VectorType VT = PlaneDotCoord(P, LinePoint1);
			VT = Vector::Divide(VT, D);

			VectorType Point = Vector::Subtract(LinePoint2, LinePoint1);
			Point = VectorMultiplyAdd(Point, VT, LinePoint1);

			const VectorType Zero = Vector::Zero();
			VectorType Control = Vector::NearEqual(D, Zero, Constants::Epsilon.v);

			return Vector::Select(Point, Constants::QNaN.v, Control);
		}

		//------------------------------------------------------------------------------

		inline void RAYMATH_CALLCONV PlaneIntersectPlane
		(
			VectorType* pLinePoint1,
			VectorType* pLinePoint2,
			FVectorType  P1,
			FVectorType  P2
		)
		{
			assert(pLinePoint1);
			assert(pLinePoint2);

			VectorType V1 = Vector3Cross(P2, P1);

			Vector LengthSq = Vector3LengthSq(V1);

			VectorType V2 = Vector3Cross(P2, V1);

			VectorType P1W = Vector::SplatW(P1);
			VectorType Point = Vector::Multiply(V2, P1W);

			VectorType V3 = Vector3Cross(V1, P1);

			VectorType P2W = Vector::SplatW(P2);
			Point = VectorMultiplyAdd(V3, P2W, Point);

			VectorType LinePoint1 = Vector::Divide(Point, LengthSq);

			VectorType LinePoint2 = Vector::Add(LinePoint1, V1);

			VectorType Control = LengthSq.LessOrEqual(Constants::Epsilon.v);
			*pLinePoint1 = Vector::Select(LinePoint1, Constants::QNaN.v, Control);
			*pLinePoint2 = Vector::Select(LinePoint2, Constants::QNaN.v, Control);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneTransform
		(
			FVectorType P,
			FMatrix M
		)
		{
			VectorType W = Vector::SplatW(P);
			VectorType Z = Vector::SplatZ(P);
			VectorType Y = Vector::SplatY(P);
			VectorType X = Vector::SplatX(P);

			VectorType Result = Vector::Multiply(W, M.r[3]);
			Result = VectorMultiplyAdd(Z, M.r[2], Result);
			Result = VectorMultiplyAdd(Y, M.r[1], Result);
			Result = VectorMultiplyAdd(X, M.r[0], Result);
			return Result;
		}

		//------------------------------------------------------------------------------

		inline Float4* RAYMATH_CALLCONV PlaneTransformStream
		(
			Float4* pOutputStream,
			size_t          OutputStride,
			const Float4* pInputStream,
			size_t          InputStride,
			size_t          PlaneCount,
			FMatrix       M
		)
		{
			return Vector4TransformStream(pOutputStream,
				OutputStride,
				pInputStream,
				InputStride,
				PlaneCount,
				M);
		}

		//------------------------------------------------------------------------------
		// Conversion operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneFromPointNormal
		(
			FVectorType Point,
			FVectorType Normal
		)
		{
			VectorType W = Vector3Dot(Point, Normal);
			W = Vector::Negate(W);
			return Vector::Select(W, Normal, Constants::Select1110.v);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV PlaneFromPoints
		(
			FVectorType Point1,
			FVectorType Point2,
			FVectorType Point3
		)
		{
			VectorType V21 = Vector::Subtract(Point1, Point2);
			VectorType V31 = Vector::Subtract(Point1, Point3);

			VectorType N = Vector3Cross(V21, V31);
			N = Vector3Normalize(N);

			VectorType D = PlaneDotNormal(N, Point1);
			D = Vector::Negate(D);

			VectorType Result = Vector::Select(D, N, Constants::Select1110.v);

			return Result;
		}

		/****************************************************************************
		 *
		 * Color
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------
		 // Comparison operations
		 //------------------------------------------------------------------------------

		 //------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorEqual
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4Equal(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorNotEqual
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4NotEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorGreater
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4Greater(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorGreaterOrEqual
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4GreaterOrEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorLess
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4Less(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorLessOrEqual
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector4LessOrEqual(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorIsNaN
		(
			FVectorType C
		)
		{
			return Vector4IsNaN(C);
		}

		//------------------------------------------------------------------------------

		inline bool RAYMATH_CALLCONV ColorIsInfinite
		(
			FVectorType C
		)
		{
			return Vector4IsInfinite(C);
		}

		//------------------------------------------------------------------------------
		// Computation operations
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorNegative
		(
			FVectorType vColor
		)
		{
#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { {
					1.0f - vColor.vector4_f32[0],
					1.0f - vColor.vector4_f32[1],
					1.0f - vColor.vector4_f32[2],
					vColor.vector4_f32[3]
				} } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vTemp = veorq_u32(vColor, Constants::Negate3);
			return vaddq_f32(vTemp, Constants::One3);
#elif defined(RAY_SSE_INTRINSICS)
			// Negate only x,y and z.
			VectorType vTemp = _mm_xor_ps(vColor, Constants::Negate3);
			// Add 1,1,1,0 to -x,-y,-z,w
			return _mm_add_ps(vTemp, Constants::One3);
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorModulate
		(
			FVectorType C1,
			FVectorType C2
		)
		{
			return Vector::Multiply(C1, C2);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorAdjustSaturation
		(
			FVectorType vColor,
			float    fSaturation
		)
		{
			// Luminance = 0.2125f * C[0] + 0.7154f * C[1] + 0.0721f * C[2];
			// Result = (C - Luminance) * Saturation + Luminance;

			const VectorF32 gvLuminance = { { { 0.2125f, 0.7154f, 0.0721f, 0.0f } } };
#if defined(RAY_NO_INTRINSICS)
			float fLuminance = (vColor.vector4_f32[0] * gvLuminance.f[0]) + (vColor.vector4_f32[1] * gvLuminance.f[1]) + (vColor.vector4_f32[2] * gvLuminance.f[2]);
			VectorType vResult;
			vResult.vector4_f32[0] = ((vColor.vector4_f32[0] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[1] = ((vColor.vector4_f32[1] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[2] = ((vColor.vector4_f32[2] - fLuminance) * fSaturation) + fLuminance;
			vResult.vector4_f32[3] = vColor.vector4_f32[3];
			return vResult;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vLuminance = Vector3Dot(vColor, gvLuminance);
			VectorType vResult = vsubq_f32(vColor, vLuminance);
			vResult = vmlaq_n_f32(vLuminance, vResult, fSaturation);
			return vbslq_f32(Constants::Select1110, vResult, vColor);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vLuminance = Vector3Dot(vColor, gvLuminance);
			// Splat fSaturation
			VectorType vSaturation = _mm_set_ps1(fSaturation);
			// vResult = ((vColor-vLuminance)*vSaturation)+vLuminance;
			VectorType vResult = _mm_sub_ps(vColor, vLuminance);
			vResult = _mm_mul_ps(vResult, vSaturation);
			vResult = _mm_add_ps(vResult, vLuminance);
			// Retain w from the source color
			vLuminance = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
			vResult = _mm_shuffle_ps(vResult, vLuminance, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorAdjustContrast
		(
			FVectorType vColor,
			float    fContrast
		)
		{
			// Result = (vColor - 0.5f) * fContrast + 0.5f;

#if defined(RAY_NO_INTRINSICS)
			VectorF32 vResult = { { {
					((vColor.vector4_f32[0] - 0.5f) * fContrast) + 0.5f,
					((vColor.vector4_f32[1] - 0.5f) * fContrast) + 0.5f,
					((vColor.vector4_f32[2] - 0.5f) * fContrast) + 0.5f,
					vColor.vector4_f32[3]        // Leave W untouched
				} } };
			return vResult.v;
#elif defined(RAY_ARM_NEON_INTRINSICS)
			VectorType vResult = vsubq_f32(vColor, Constants::OneHalf.v);
			vResult = vmlaq_n_f32(Constants::OneHalf.v, vResult, fContrast);
			return vbslq_f32(Constants::Select1110, vResult, vColor);
#elif defined(RAY_SSE_INTRINSICS)
			VectorType vScale = _mm_set_ps1(fContrast);           // Splat the scale
			VectorType vResult = _mm_sub_ps(vColor, Constants::OneHalf);  // Subtract 0.5f from the source (Saving source)
			vResult = _mm_mul_ps(vResult, vScale);               // Mul by scale
			vResult = _mm_add_ps(vResult, Constants::OneHalf);          // Add 0.5f
		// Retain w from the source color
			vScale = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
			vResult = _mm_shuffle_ps(vResult, vScale, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToHSL(FVectorType rgb)
		{
			VectorType r = Vector::SplatX(rgb);
			VectorType g = Vector::SplatY(rgb);
			VectorType b = Vector::SplatZ(rgb);

			VectorType min = Vector::Min(r, Vector::Min(g, b));
			VectorType max = Vector::Max(r, Vector::Max(g, b));

			VectorType l = Vector::Multiply(Vector::Add(min, max), Constants::OneHalf);

			VectorType d = Vector::Subtract(max, min);

			VectorType la = Vector::Select(rgb, l, Constants::Select1110);

			if (Vector3Less(d, Constants::Epsilon))
			{
				// Achromatic, assume H and S of 0
				return Vector::Select(la, Constants::Zero, Constants::Select1100);
			}
			else
			{
				VectorType s, h;

				VectorType d2 = Vector::Add(min, max);

				if (Vector3Greater(l, Constants::OneHalf))
				{
					// d / (2-max-min)
					s = Vector::Divide(d, Vector::Subtract(Constants::Two, d2));
				}
				else
				{
					// d / (max+min)
					s = Vector::Divide(d, d2);
				}

				if (Vector3Equal(r, max))
				{
					// Red is max
					h = Vector::Divide(Vector::Subtract(g, b), d);
				}
				else if (Vector3Equal(g, max))
				{
					// Green is max
					h = Vector::Divide(Vector::Subtract(b, r), d);
					h = Vector::Add(h, Constants::Two);
				}
				else
				{
					// Blue is max
					h = Vector::Divide(Vector::Subtract(r, g), d);
					h = Vector::Add(h, Constants::Four);
				}

				h = Vector::Divide(h, Constants::Six);

				if (Vector3Less(h, Constants::Zero))
					h = Vector::Add(h, Constants::One);

				VectorType lha = Vector::Select(la, h, Constants::Select1100);
				return Vector::Select(s, lha, Constants::Select1011);
			}
		}

		//------------------------------------------------------------------------------

		namespace Internal
		{

			inline VectorType RAYMATH_CALLCONV ColorHue2Clr(FVectorType p, FVectorType q, FVectorType h)
			{
				static const VectorF32 oneSixth = { { { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f } } };
				static const VectorF32 twoThirds = { { { 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f } } };

				VectorType t = h;

				if (Vector3Less(t, Constants::Zero))
					t = Vector::Add(t, Constants::One);

				if (Vector3Greater(t, Constants::One))
					t = Vector::Subtract(t, Constants::One);

				if (Vector3Less(t, oneSixth))
				{
					// p + (q - p) * 6 * t
					VectorType t1 = Vector::Subtract(q, p);
					VectorType t2 = Vector::Multiply(Constants::Six, t);
					return VectorMultiplyAdd(t1, t2, p);
				}

				if (Vector3Less(t, Constants::OneHalf))
					return q;

				if (Vector3Less(t, twoThirds))
				{
					// p + (q - p) * 6 * (2/3 - t)
					VectorType t1 = Vector::Subtract(q, p);
					VectorType t2 = Vector::Multiply(Constants::Six, Vector::Subtract(twoThirds, t));
					return VectorMultiplyAdd(t1, t2, p);
				}

				return p;
			}

		} // namespace Internal

		inline VectorType RAYMATH_CALLCONV ColorHSLToRGB(FVectorType hsl)
		{
			static const VectorF32 oneThird = { { { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f } } };

			VectorType s = Vector::SplatY(hsl);
			VectorType l = Vector::SplatZ(hsl);

			if (Vector3NearEqual(s, Constants::Zero, Constants::Epsilon))
			{
				// Achromatic
				return Vector::Select(hsl, l, Constants::Select1110);
			}
			else
			{
				VectorType h = Vector::SplatX(hsl);

				VectorType q;
				if (Vector3Less(l, Constants::OneHalf))
				{
					q = Vector::Multiply(l, Vector::Add(Constants::One, s));
				}
				else
				{
					q = Vector::Subtract(Vector::Add(l, s), Vector::Multiply(l, s));
				}

				VectorType p = Vector::Subtract(Vector::Multiply(Constants::Two, l), q);

				VectorType r = Internal::ColorHue2Clr(p, q, Vector::Add(h, oneThird));
				VectorType g = Internal::ColorHue2Clr(p, q, h);
				VectorType b = Internal::ColorHue2Clr(p, q, Vector::Subtract(h, oneThird));

				VectorType rg = Vector::Select(g, r, Constants::Select1000);
				VectorType ba = Vector::Select(hsl, b, Constants::Select1110);

				return Vector::Select(ba, rg, Constants::Select1100);
			}
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToHSV(FVectorType rgb)
		{
			VectorType r = Vector::SplatX(rgb);
			VectorType g = Vector::SplatY(rgb);
			VectorType b = Vector::SplatZ(rgb);

			VectorType min = Vector::Min(r, Vector::Min(g, b));
			VectorType v = Vector::Max(r, Vector::Max(g, b));

			VectorType d = Vector::Subtract(v, min);

			VectorType s = (Vector3NearEqual(v, Constants::Zero, Constants::Epsilon)) ? Constants::Zero : Vector::Divide(d, v);

			if (Vector3Less(d, Constants::Epsilon))
			{
				// Achromatic, assume H of 0
				VectorType hv = Vector::Select(v, Constants::Zero, Constants::Select1000);
				VectorType hva = Vector::Select(rgb, hv, Constants::Select1110);
				return Vector::Select(s, hva, Constants::Select1011);
			}
			else
			{
				VectorType h;

				if (Vector3Equal(r, v))
				{
					// Red is max
					h = Vector::Divide(Vector::Subtract(g, b), d);

					if (Vector3Less(g, b))
						h = Vector::Add(h, Constants::Six);
				}
				else if (Vector3Equal(g, v))
				{
					// Green is max
					h = Vector::Divide(Vector::Subtract(b, r), d);
					h = Vector::Add(h, Constants::Two);
				}
				else
				{
					// Blue is max
					h = Vector::Divide(Vector::Subtract(r, g), d);
					h = Vector::Add(h, Constants::Four);
				}

				h = Vector::Divide(h, Constants::Six);

				VectorType hv = Vector::Select(v, h, Constants::Select1000);
				VectorType hva = Vector::Select(rgb, hv, Constants::Select1110);
				return Vector::Select(s, hva, Constants::Select1011);
			}
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorHSVToRGB(FVectorType hsv)
		{
			VectorType h = Vector::SplatX(hsv);
			VectorType s = Vector::SplatY(hsv);
			VectorType v = Vector::SplatZ(hsv);

			Vector h6 = Vector::Multiply(h, Constants::Six);

			VectorType i = h6.Floor();
			VectorType f = Vector::Subtract(h6, i);

			// p = v* (1-s)
			VectorType p = Vector::Multiply(v, Vector::Subtract(Constants::One, s));

			// q = v*(1-f*s)
			VectorType q = Vector::Multiply(v, Vector::Subtract(Constants::One, Vector::Multiply(f, s)));

			// t = v*(1 - (1-f)*s)
			VectorType t = Vector::Multiply(v, Vector::Subtract(Constants::One, Vector::Multiply(Vector::Subtract(Constants::One, f), s)));

			auto ii = static_cast<int>(Vector(VectorMod(i, Constants::Six)).GetX());

			VectorType _rgb;

			switch (ii)
			{
			case 0: // rgb = vtp
			{
				VectorType vt = Vector::Select(t, v, Constants::Select1000);
				_rgb = Vector::Select(p, vt, Constants::Select1100);
			}
			break;
			case 1: // rgb = qvp
			{
				VectorType qv = Vector::Select(v, q, Constants::Select1000);
				_rgb = Vector::Select(p, qv, Constants::Select1100);
			}
			break;
			case 2: // rgb = pvt
			{
				VectorType pv = Vector::Select(v, p, Constants::Select1000);
				_rgb = Vector::Select(t, pv, Constants::Select1100);
			}
			break;
			case 3: // rgb = pqv
			{
				VectorType pq = Vector::Select(q, p, Constants::Select1000);
				_rgb = Vector::Select(v, pq, Constants::Select1100);
			}
			break;
			case 4: // rgb = tpv
			{
				VectorType tp = Vector::Select(p, t, Constants::Select1000);
				_rgb = Vector::Select(v, tp, Constants::Select1100);
			}
			break;
			default: // rgb = vpq
			{
				VectorType vp = Vector::Select(p, v, Constants::Select1000);
				_rgb = Vector::Select(q, vp, Constants::Select1100);
			}
			break;
			}

			return Vector::Select(hsv, _rgb, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToYUV(FVectorType rgb)
		{
			static const VectorF32 Scale0 = { { { 0.299f, -0.147f, 0.615f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.587f, -0.289f, -0.515f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.114f, 0.436f, -0.100f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(rgb, M);

			return Vector::Select(rgb, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorYUVToRGB(FVectorType yuv)
		{
			static const VectorF32 Scale1 = { { { 0.0f, -0.395f, 2.032f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 1.140f, -0.581f, 0.0f, 0.0f } } };

			Matrix M(Constants::One, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(yuv, M);

			return Vector::Select(yuv, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToYUV_HD(FVectorType rgb)
		{
			static const VectorF32 Scale0 = { { { 0.2126f, -0.0997f, 0.6150f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.7152f, -0.3354f, -0.5586f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.0722f, 0.4351f, -0.0564f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(rgb, M);

			return Vector::Select(rgb, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorYUVToRGB_HD(FVectorType yuv)
		{
			static const VectorF32 Scale1 = { { { 0.0f, -0.2153f, 2.1324f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 1.2803f, -0.3806f, 0.0f, 0.0f } } };

			Matrix M(Constants::One, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(yuv, M);

			return Vector::Select(yuv, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToXYZ(FVectorType rgb)
		{
			static const VectorF32 Scale0 = { { { 0.4887180f, 0.1762044f, 0.0000000f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.3106803f, 0.8129847f, 0.0102048f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.2006017f, 0.0108109f, 0.9897952f, 0.0f } } };
			static const VectorF32 Scale = { { { 1.f / 0.17697f, 1.f / 0.17697f, 1.f / 0.17697f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector::Multiply(Vector3Transform(rgb, M), Scale);

			return Vector::Select(rgb, clr, Constants::Select1110);
		}

		inline VectorType RAYMATH_CALLCONV ColorXYZToRGB(FVectorType xyz)
		{
			static const VectorF32 Scale0 = { { { 2.3706743f, -0.5138850f, 0.0052982f, 0.0f } } };
			static const VectorF32 Scale1 = { { { -0.9000405f, 1.4253036f, -0.0146949f, 0.0f } } };
			static const VectorF32 Scale2 = { { { -0.4706338f, 0.0885814f, 1.0093968f, 0.0f } } };
			static const VectorF32 Scale = { { { 0.17697f, 0.17697f, 0.17697f, 0.0f } } };

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(Vector::Multiply(xyz, Scale), M);

			return Vector::Select(xyz, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorXYZToSRGB(FVectorType xyz)
		{
			static const VectorF32 Scale0 = { { { 3.2406f, -0.9689f, 0.0557f, 0.0f } } };
			static const VectorF32 Scale1 = { { { -1.5372f, 1.8758f, -0.2040f, 0.0f } } };
			static const VectorF32 Scale2 = { { { -0.4986f, 0.0415f, 1.0570f, 0.0f } } };
			static const VectorF32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 0.0f } } };
			static const VectorF32 Exp = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.0f } } };

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			Vector lclr = Vector3Transform(xyz, M);

			VectorType sel = lclr.Greater(Cutoff);

			// clr = 12.92 * lclr for lclr <= 0.0031308f
			VectorType smallC = Vector::Multiply(lclr, Constants::srgbScale);

			// clr = (1+a)*pow(lclr, 1/2.4) - a for lclr > 0.0031308 (where a = 0.055)
			VectorType largeC = Vector::Subtract(Vector::Multiply(Constants::srgbA1, VectorPow(lclr, Exp)), Constants::srgbA);

			VectorType clr = Vector::Select(smallC, largeC, sel);

			return Vector::Select(xyz, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorSRGBToXYZ(FVectorType srgb)
		{
			static const VectorF32 Scale0 = { { { 0.4124f, 0.2126f, 0.0193f, 0.0f } } };
			static const VectorF32 Scale1 = { { { 0.3576f, 0.7152f, 0.1192f, 0.0f } } };
			static const VectorF32 Scale2 = { { { 0.1805f, 0.0722f, 0.9505f, 0.0f } } };
			static const VectorF32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 0.0f } } };
			static const VectorF32 Exp = { { { 2.4f, 2.4f, 2.4f, 1.0f } } };

			VectorType sel = Vector(srgb).Greater(Cutoff);

			// lclr = clr / 12.92
			VectorType smallC = Vector::Divide(srgb, Constants::srgbScale);

			// lclr = pow( (clr + a) / (1+a), 2.4 )
			VectorType largeC = VectorPow(Vector::Divide(Vector::Add(srgb, Constants::srgbA), Constants::srgbA1), Exp);

			VectorType lclr = Vector::Select(smallC, largeC, sel);

			Matrix M(Scale0, Scale1, Scale2, Constants::Zero);
			VectorType clr = Vector3Transform(lclr, M);

			return Vector::Select(srgb, clr, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorRGBToSRGB(FVectorType rgb)
		{
			static const VectorF32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 1.f } } };
			static const VectorF32 Linear = { { { 12.92f, 12.92f, 12.92f, 1.f } } };
			static const VectorF32 Scale = { { { 1.055f, 1.055f, 1.055f, 1.f } } };
			static const VectorF32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
			static const VectorF32 InvGamma = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.f } } };

			Vector V = Vector(rgb).Saturate();
			VectorType V0 = Vector::Multiply(V, Linear);
			VectorType V1 = Vector::Subtract(Vector::Multiply(Scale, VectorPow(V, InvGamma)), Bias);
			VectorType select = V.Less(Cutoff);
			V = Vector::Select(V1, V0, select);
			return Vector::Select(rgb, V, Constants::Select1110);
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV ColorSRGBToRGB(FVectorType srgb)
		{
			static const VectorF32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 1.f } } };
			static const VectorF32 ILinear = { { { 1.f / 12.92f, 1.f / 12.92f, 1.f / 12.92f, 1.f } } };
			static const VectorF32 Scale = { { { 1.f / 1.055f, 1.f / 1.055f, 1.f / 1.055f, 1.f } } };
			static const VectorF32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
			static const VectorF32 Gamma = { { { 2.4f, 2.4f, 2.4f, 1.f } } };

			Vector V = Vector(srgb).Saturate();
			VectorType V0 = Vector::Multiply(V, ILinear);
			VectorType V1 = VectorPow(Vector::Multiply(Vector::Add(V, Bias), Scale), Gamma);
			VectorType select = V.Greater(Cutoff);
			V = Vector::Select(V0, V1, select);
			return Vector::Select(srgb, V, Constants::Select1110);
		}

		/****************************************************************************
		 *
		 * Miscellaneous
		 *
		 ****************************************************************************/

		 //------------------------------------------------------------------------------

		inline bool VerifyCPUSupport()
		{
#if defined(RAY_SSE_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			int CPUInfo[4] = { -1 };
			__cpuid(CPUInfo, 0);

#ifdef __AVX2__
			if (CPUInfo[0] < 7)
				return false;
#else
			if (CPUInfo[0] < 1)
				return false;
#endif

			__cpuid(CPUInfo, 1);

#if defined(__AVX2__) || defined(RAY_AVX2_INTRINSICS)
			// The compiler can emit FMA3 instructions even without explicit intrinsics use
			if ((CPUInfo[2] & 0x38081001) != 0x38081001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(RAY_FMA3_INTRINSICS) && defined(RAY_F16C_INTRINSICS)
			if ((CPUInfo[2] & 0x38081001) != 0x38081001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(RAY_FMA3_INTRINSICS)
			if ((CPUInfo[2] & 0x18081001) != 0x18081001)
				return false; // No AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(RAY_F16C_INTRINSICS)
			if ((CPUInfo[2] & 0x38080001) != 0x38080001)
				return false; // No F16C/AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(__AVX__) || defined(RAY_AVC_INTRINSICS)
			if ((CPUInfo[2] & 0x18080001) != 0x18080001)
				return false; // No AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(RAY_SSE4_INTRINSICS)
			if ((CPUInfo[2] & 0x80001) != 0x80001)
				return false; // No SSE3/SSE4.1 support
#elif defined(RAY_SSE3_INTRINSICS)
			if (!(CPUInfo[2] & 0x1))
				return false; // No SSE3 support  
#endif

	// The x64 processor model requires SSE2 support, but no harm in checking
			if ((CPUInfo[3] & 0x6000000) != 0x6000000)
				return false; // No SSE2/SSE support

#if defined(__AVX2__) || defined(RAY_AVX2_INTRINSICS)
			__cpuidex(CPUInfo, 7, 0);
			if (!(CPUInfo[1] & 0x20))
				return false; // No AVX2 support
#endif

			return true;
#elif defined(RAY_ARM_NEON_INTRINSICS) && !defined(RAY_NO_INTRINSICS)
			// ARM-NEON support is required for the Windows on ARM platform
			return true;
#else
			// No intrinsics path always supported
			return true;
#endif
		}

		//------------------------------------------------------------------------------

		inline VectorType RAYMATH_CALLCONV FresnelTerm
		(
			FVectorType CosIncidentAngle,
			FVectorType RefractionIndex
		)
		{
			assert(!Vector4IsInfinite(CosIncidentAngle));

			// Result = 0.5f * (g - c)^2 / (g + c)^2 * ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1) where
			// c = CosIncidentAngle
			// g = sqrt(c^2 + RefractionIndex^2 - 1)

#if defined(RAY_NO_INTRINSICS) || defined(RAY_ARM_NEON_INTRINSICS)

			Vector G = VectorMultiplyAdd(RefractionIndex, RefractionIndex, Constants::NegativeOne.v);
			G = VectorMultiplyAdd(CosIncidentAngle, CosIncidentAngle, G);
			G = VectorAbs(G);
			G = G.Sqrt();

			VectorType S = Vector::Add(G, CosIncidentAngle);
			VectorType D = Vector::Subtract(G, CosIncidentAngle);

			VectorType V0 = Vector::Multiply(D, D);
			Vector V1 = Vector::Multiply(S, S);
			V1 = V1.Reciprocal();
			V0 = Vector::Multiply(Constants::OneHalf.v, V0);
			V0 = Vector::Multiply(V0, V1);

			VectorType V2 = VectorMultiplyAdd(CosIncidentAngle, S, Constants::NegativeOne.v);
			Vector V3 = VectorMultiplyAdd(CosIncidentAngle, D, Constants::One.v);
			V2 = Vector::Multiply(V2, V2);
			V3 = Vector::Multiply(V3, V3);
			V3 = V3.Reciprocal();
			V2 = VectorMultiplyAdd(V2, V3, Constants::One.v);

			Vector Result = Vector::Multiply(V0, V2);

			Result = Result.Saturate();

			return Result;

#elif defined(RAY_SSE_INTRINSICS)
	// G = sqrt(abs((RefractionIndex^2-1) + CosIncidentAngle^2))
			VectorType G = _mm_mul_ps(RefractionIndex, RefractionIndex);
			VectorType vTemp = _mm_mul_ps(CosIncidentAngle, CosIncidentAngle);
			G = _mm_sub_ps(G, Constants::One);
			vTemp = _mm_add_ps(vTemp, G);
			// max((0-vTemp),vTemp) == abs(vTemp)
			// The abs is needed to deal with refraction and cosine being zero
			G = _mm_setzero_ps();
			G = _mm_sub_ps(G, vTemp);
			G = _mm_max_ps(G, vTemp);
			// Last operation, the sqrt()
			G = _mm_sqrt_ps(G);

			// Calc G-C and G+C
			VectorType GAddC = _mm_add_ps(G, CosIncidentAngle);
			VectorType GSubC = _mm_sub_ps(G, CosIncidentAngle);
			// Perform the term (0.5f *(g - c)^2) / (g + c)^2 
			VectorType vResult = _mm_mul_ps(GSubC, GSubC);
			vTemp = _mm_mul_ps(GAddC, GAddC);
			vResult = _mm_mul_ps(vResult, Constants::OneHalf);
			vResult = _mm_div_ps(vResult, vTemp);
			// Perform the term ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1)
			GAddC = _mm_mul_ps(GAddC, CosIncidentAngle);
			GSubC = _mm_mul_ps(GSubC, CosIncidentAngle);
			GAddC = _mm_sub_ps(GAddC, Constants::One);
			GSubC = _mm_add_ps(GSubC, Constants::One);
			GAddC = _mm_mul_ps(GAddC, GAddC);
			GSubC = _mm_mul_ps(GSubC, GSubC);
			GAddC = _mm_div_ps(GAddC, GSubC);
			GAddC = _mm_add_ps(GAddC, Constants::One);
			// Multiply the two term parts
			vResult = _mm_mul_ps(vResult, GAddC);
			// Clamp to 0.0 - 1.0f
			vResult = _mm_max_ps(vResult, Constants::Zero);
			vResult = _mm_min_ps(vResult, Constants::One);
			return vResult;
#endif
		}

		//------------------------------------------------------------------------------

		inline bool ScalarNearEqual
		(
			float S1,
			float S2,
			float Epsilon
		)
		{
			float Delta = S1 - S2;
			return (fabsf(Delta) <= Epsilon);
		}

		//------------------------------------------------------------------------------
		// Modulo the range of the given angle such that -Constants::PI <= Angle < Constants::PI
		inline float ScalarModAngle
		(
			float Angle
		)
		{
			// Note: The modulo is performed with unsigned math only to work
			// around a precision error on numbers that are close to PI

			// Normalize the range from 0.0f to Constants::RAY_2PI
			Angle = Angle + Constants::PI;
			// Perform the modulo, unsigned
			float fTemp = fabsf(Angle);
			fTemp = fTemp - (Constants::RAY_2PI * static_cast<float>(static_cast<int32_t>(fTemp / Constants::RAY_2PI)));
			// Restore the number to the range of -Constants::PI to Constants::PI-epsilon
			fTemp = fTemp - Constants::PI;
			// If the modulo'd value was negative, restore negation
			if (Angle < 0.0f) {
				fTemp = -fTemp;
			}
			return fTemp;
		}

		//------------------------------------------------------------------------------

		inline float ScalarSin
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
			}

			// 11-degree minimax approximation
			float y2 = y * y;
			return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
		}

		//------------------------------------------------------------------------------

		inline float ScalarSinEst
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
			}

			// 7-degree minimax approximation
			float y2 = y * y;
			return (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;
		}

		//------------------------------------------------------------------------------

		inline float ScalarCos
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
			float sign;
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
				sign = -1.0f;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			// 10-degree minimax approximation
			float y2 = y * y;
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			return sign * p;
		}

		//------------------------------------------------------------------------------

		inline float ScalarCosEst
		(
			float Value
		)
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
			float sign;
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
				sign = -1.0f;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			// 6-degree minimax approximation
			float y2 = y * y;
			float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
			return sign * p;
		}

		//------------------------------------------------------------------------------


		inline void ScalarSinCos
		(
			float* pSin,
			float* pCos,
			float  Value
		)
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
				sign = -1.0f;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		//------------------------------------------------------------------------------


		inline void ScalarSinCosEst
		(
			float* pSin,
			float* pCos,
			float  Value
		)
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = Constants::RAY_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - Constants::RAY_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > Constants::PIDIV2)
			{
				y = Constants::PI - y;
				sign = -1.0f;
			}
			else if (y < -Constants::PIDIV2)
			{
				y = -Constants::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 7-degree minimax approximation
			*pSin = (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;

			// 6-degree minimax approximation
			float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		//------------------------------------------------------------------------------

		inline float ScalarASin
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 7-degree minimax approximation
			float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
			result *= root;  // acos(|x|)

			// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
			return (nonnegative ? Constants::PIDIV2 - result : result - Constants::PIDIV2);
		}

		//------------------------------------------------------------------------------

		inline float ScalarASinEst
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 3-degree minimax approximation
			float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
			result *= root;  // acos(|x|)

			// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
			return (nonnegative ? Constants::PIDIV2 - result : result - Constants::PIDIV2);
		}

		//------------------------------------------------------------------------------

		inline float ScalarACos
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 7-degree minimax approximation
			float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
			result *= root;

			// acos(x) = pi - acos(-x) when x < 0
			return (nonnegative ? result : Constants::PI - result);
		}

		//------------------------------------------------------------------------------

		inline float ScalarACosEst
		(
			float Value
		)
		{
			// Clamp input to [-1,1].
			bool nonnegative = (Value >= 0.0f);
			float x = fabsf(Value);
			float omx = 1.0f - x;
			if (omx < 0.0f)
			{
				omx = 0.0f;
			}
			float root = sqrtf(omx);

			// 3-degree minimax approximation
			float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
			result *= root;

			// acos(x) = pi - acos(-x) when x < 0
			return (nonnegative ? result : Constants::PI - result);
		}



#ifdef _PREFAST_
#pragma prefast(pop)
#endif

#pragma warning(pop)

	}
}
