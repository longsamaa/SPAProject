#ifndef _BASICS_H_
#define _BASICS_H_

#ifndef GE_INT8
#define GE_INT8 signed char
#endif

#ifndef GE_INT8U
#define GE_INT8U unsigned char
#endif

#ifndef GE_INT16
#define GE_INT16 short
#endif

#ifndef GE_INT16U
#define GE_INT16U unsigned short
#endif

#ifndef GE_INT32
#define GE_INT32 int
#endif

#ifndef GE_INT32U
#define GE_INT32U unsigned
#endif

#ifndef GE_INT64
#if defined(_MSC_VER)
#define GE_INT64 signed __int64
#else
#define GE_INT64 signed long long
#endif
#endif

#ifndef GE_INT64U
#if defined(_MSC_VER)
#define GE_INT64U unsigned __int64
#else
#define GE_INT64U unsigned long long
#endif
#endif

//------------------------------------------------ Some fixes for MS Visual C++
#if defined(_MSC_VER)
#pragma warning(disable:4786) // Identifier was truncated...
#endif

#if defined(_MSC_VER)
#define GE_INLINE __forceinline
#else
#define GE_INLINE inline
#endif

namespace MapEngine
{
    //-------------------------------------------------------------------------
    typedef GE_INT8   int8;         //----int8
    typedef GE_INT8U  int8u;        //----int8u
    typedef GE_INT16  int16;        //----int16
    typedef GE_INT16U int16u;       //----int16u
    typedef GE_INT32  int32;        //----int32
    typedef GE_INT32U int32u;       //----int32u
    typedef GE_INT64  int64;        //----int64
    typedef GE_INT64U int64u;       //----int64u
    typedef float     real32;       //----real32
    typedef double    real64;       //----real64

	#define MakeInt16u(lo, hi)	((int16u)(((int8u)(lo)) | ((int16u)((int8u)(hi))) << 8))
	#define MakeInt32(lo, hi)	((int32)(((int16u)(lo)) | ((int32u)((int16u)(hi))) << 16))
	#define LoInt16u(l)			((int16u)(l))
	#define HiInt16u(l)			((int16u)(((int32u)(l) >> 16) & 0xFFFF))
	#define LoInt8u(w)			((int8u)(w))
	#define HiInt8u(w)			((int8u)(((int16u)(w) >> 8) & 0xFF))
	
	#define LoInt64u(x)	((int32u)(x))
	#define LoInt64(x)	((int32)(x))
#if defined(_MSC_VER) && _MSC_VER < 1300 // bug in MVC 6.0
	#define HiInt64u(x)	(sizeof(x) < 8 ? 0 : ((int32u)((int64u)(x)>>32)))
	#define HiInt64(x)	(sizeof(x) < 8 ? 0 : ((int32)((int64)(x)>>32)))
#else
	#define HiInt64u(x)	((int32u)((int64u)(x)>>32))
	#define HiInt64(x)	((int32)((int64)(x)>>32))
#endif
	#define MakeInt64u(lo, hi)	((((int64u)(hi)) << 32) | (int32u)(lo))
	#define MakeInt64(lo, hi)	((((int64)(hi)) << 32) | (int32u)(lo))
		
	///#define MAX_NAT8  int64u(-1)

} // MapEngine

#endif // _BASICS_H_

