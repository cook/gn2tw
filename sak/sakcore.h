/******************************************************************************
 * SAK, is a base library for image process and analysis.
 * Copyright (C) 2010 Yantao Xie(leeward.xie@gmail.com)
 *
 * This file is part of SAK.
 *
 * SAK is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * SAK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

#ifndef __SAK_SAKCORE_H__
#define __SAK_SAKCORE_H__

#include <string.h>
#include <float.h>
#include <limits.h>
#include <assert.h>

/**
 * Platforms
 */

///  Machine
#if	defined(__i386__)
#	define SAK_IS_X86	1
#else
#	define SAK_IS_X86	0
#endif
#if	defined(__alpha)
#	define SAK_IS_ALPHA	1
#else
#	define SAK_IS_ALPHA	0
#endif

/// OS
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#	define SAK_IS_WIN32	1
#else
#	define SAK_IS_WIN32 0
#endif
#if defined(WIN64) || defined(_WIN64)
#	define SAK_IS_WIN64	1
#else
#	define SAK_IS_WIN64	0
#endif
#if SAK_IS_WIN32 || SAK_IS_WIN64
#	define SAK_IS_WINDOWS	1
#else
#	define SAK_IS_WINDOWS	0
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
#	define SAK_IS_LINUX	1
#else
#	define SAK_IS_LINUX	0
#endif
#if defined(__unix__) || defined(__unix)
#	define SAK_IS_UNIX	1
#else
#	define SAK_IS_UNIX	0
#endif
#if defined(__APPLE__)
#	define SAK_IS_MACOSX	1
#else
#	define SAK_IS_MACOSX	0
#endif

/// Compiler
#if defined(__GNUC__)
#	define SAK_IS_GNUC	1
#else
#	define SAK_IS_GNUC	0
#endif
#if	defined(_MSC_VER)
#	define SAK_IS_VC	1
#else
#	define SAK_IS_VC	0
#endif

#if SAK_IS_LINUX && SAK_IS_GNUC
#	define HAS_GCC_VISIBILITY		/* make sure supporting of visibility. */
#endif

#if SAK_IS_VC && SAK_IS_WINDOWS
#	if _MSC_VER >= 1500
#		define SAK_VC9	/* Visual Studio 2008, version 9 */
#	elif _MSC_VER >= 1400
#		define SAK_VC8	/* Visual Studio 2005, version 8 */
#	elif _MSC_VER >= 1310
#		define SAK_VC7	/* Visual Studio .Net, version 7 */
#	elif _MSC_VER >= 1200
#		define SAK_VC6	/* Visual C++ 6.0 */
#	endif
#endif

/**
 * Function Decorations
 */

#ifndef SAK_STATIC				/* Dynamic Library */
#	if SAK_IS_WIN32 && !SAK_IS_GNUC
#		ifdef SAK_DLL
#			define SAK_EXPORTS __declspec(dllexport)
#		else
#			define SAK_EXPORTS __declspec(dllimport)
#		endif
#	elif SAK_IS_LINUX || SAK_IS_UNIX 
#		if defined(SAK_DLL) && defined(HAS_GCC_VISIBILITY)
#			define SAK_EXPORTS __attribute__((visibility("default")))
#		else
#			define SAK_EXPORTS
#		endif
#	else
#		error "The OS is not supported."
#	endif
#else  /* Static Library */
#	define SAK_EXPORTS
#endif

#if SAK_IS_WINDOWS
#	define SAK_CDECL __cdecl
#	define SAK_STDCALL __stdcall
#else
#	define SAK_CDECL
#	define SAK_STDCALL
#endif

#ifndef SAK_EXTERN_C
#    ifdef __cplusplus
#        define SAK_EXTERN_C extern "C"
#        define SAK_DEFAULT(val) = val
#    else
#        define SAK_EXTERN_C
#        define SAK_DEFAULT(val)
#    endif
#endif

#ifndef SAK_INLINE
#	if defined __cplusplus
#		define SAK_INLINE inline
#	elif SAK_IS_WINDOWS && !SAK_IS_GNUC
#   	define SAK_INLINE __inline
#	else
#		define SAK_INLINE static
#	endif
#endif

#ifndef SAKAPI
#	define SAKAPI(rettype) SAK_EXTERN_C SAK_EXPORTS rettype SAK_CDECL
#endif

#define SAK_IMPL SAK_EXTERN_C

#ifndef SAK_EXTERN_C_FUNCPTR
    #ifdef __cplusplus
        #define SAK_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
    #else
        #define SAK_EXTERN_C_FUNCPTR(x) typedef x
    #endif
#endif

/**
 * Basic Types
 */

typedef void			kvoid;
//typedef char*			any;
typedef char			kchar;
typedef short			kshort;
typedef int				kint;
typedef long			klong;
typedef unsigned char	kuchar;
typedef unsigned short	kushort;
typedef unsigned int	kuint;
typedef unsigned long	kulong;

typedef char	kint8;
typedef short	kint16;
#ifdef MACHINE16
typedef long	kint32;
typedef kulong	kuint32;
#elif defined(MACHINE64)
typedef int		kint32;
typedef long	kint64;
typedef kuint	kuint32;
typedef kulong	kuint64;
#else
typedef int		kint32;
typedef kuint	kuint32;
#	if SAK_IS_GNUC
typedef long long kint64;
typedef unsigned long long kuint64; 
#	endif
#endif
typedef unsigned char	kuint8;
typedef unsigned char	kbyte;
typedef unsigned short	kuint16;
typedef unsigned short	kword;
typedef unsigned long	kdword;

typedef kint	kbool;
typedef kulong	ksize_t;
typedef kvoid*	khandle;
typedef kvoid*	kany_ptr;
typedef char*	kchar_ptr;
typedef unsigned char* kbyte_ptr;
typedef short*	kshort_ptr;
typedef unsigned short* kword_ptr;
typedef long*	klong_ptr;
typedef unsigned long* kdword_ptr;

#ifdef __cplusplus
#	define knull		(0)
#else
#	define knull		((kvoid*)0)
#endif
#define ktrue		(1)
#define kfalse		(0)
#define ksuccess	(1)
#define kfail		(0)


/******************************************************************************
 ******************************   ERROR HANDLING ******************************
 ******************************************************************************/
typedef int kstat;

/**
 * Error Numbers */
#define SAK_StsOk                    0  /* everithing is ok                */
#define SAK_StsBackTrace            -1  /* pseudo error for back trace     */
#define SAK_StsError                -2  /* unknown /unspecified error      */
#define SAK_StsInternal             -3  /* internal error (bad state)      */
#define SAK_StsNoMem                -4  /* insufficient memory             */
#define SAK_StsBadArg               -5  /* function arg/param is bad       */
#define SAK_StsBadFunc              -6  /* unsupported function            */
#define SAK_StsNoConv               -7  /* iter. didn't converge           */
#define SAK_StsAutoTrace            -8  /* tracing                         */

#define SAK_HeaderIsNull            -9  /* image header is NULL            */
#define SAK_BadImageSize            -10 /* image size is invalid           */
#define SAK_BadOffset               -11 /* offset is invalid               */
#define SAK_BadDataPtr              -12 /**/
#define SAK_BadStep                 -13 /**/
#define SAK_BadModelOrChSeq         -14 /**/
#define SAK_BadNumChannels          -15 /**/
#define SAK_BadNumChannel1U         -16 /**/
#define SAK_BadDepth                -17 /**/
#define SAK_BadAlphaChannel         -18 /**/
#define SAK_BadOrder                -19 /**/
#define SAK_BadOrigin               -20 /**/
#define SAK_BadAlign                -21 /**/
#define SAK_BadCallBack             -22 /**/
#define SAK_BadTileSize             -23 /**/
#define SAK_BadCOI                  -24 /**/
#define SAK_BadROISize              -25 /**/

#define SAK_MaskIsTiled             -26 /**/

#define SAK_StsNullPtr                -27 /* null pointer */
#define SAK_StsVecLengthErr           -28 /* incorrect vector length */
#define SAK_StsFilterStructContentErr -29 /* incorr. filter structure content */
#define SAK_StsKernelStructContentErr -30 /* incorr. transform kernel content */
#define SAK_StsFilterOffsetErr        -31 /* incorrect filter ofset value */
#define SAK_StsBadSize				  -32  /* Incorrect size of input array */
#define SAK_StsDivByZero			  -33 /* Divizion by zero occured */
#define SAK_StsInplaceNotSupported 		-34  /* Inplace operation is not supported */
#define SAK_StsObjectNotFound			-35 /* Requested object was not found */
#define SAK_StsUnmatchedFormats 		-36 /* Formats of input arguments do not match */
#define SAK_StsUnmatchedSizes 			-37 /* Sizes of input arguments do not match */
#define SAK_StsOutOfRange		 		-38 /* One of arguments\' values is out of range */
#define SAK_StsUnsupportedFormat 		-39 /* Unsupported format or combination of formats */
#define SAK_StsBadFlag 					-40 /* Bad flag (parameter or structure field) */
#define SAK_StsBadPoint 				-41 /* Bad parameter of type CvPoint */
#define SAK_StsBadMask 					-42 /* Bad type of mask argument */
#define SAK_StsParseError				-43 /* Parsing error */
#define SAK_StsNotImplemented 			-44 /* The function/feature is not implemented */
#define SAK_StsBadMemBlock		 		-45 /* Memory block has been corrupted */
#define SAK_StsIO						-46 /* IO error */
#define SAK_StsBadCall					-47 /* Programer's error: calling is not correct. */

/*** END of Error Numbers ***/

/**
 * Error handling Macros */

#define SAK_SUCCESSED(Stat)	((Stat) >= 0)
#define SAK_FAILED(Stat)	(!SAK_SUCCESSED(Stat))

#define SAK_EXIT    goto sak_exit

#define XSAK_ERROR(status,func,context)						\
	sakError((status),(func),(context),__FILE__,__LINE__)

#define XSAK_ERRCHK(func,context)									 \
	{if (sakGetErrStatus() >= 0)									 \
		{XSAK_ERROR(SAK_StsBackTrace,(func),(context));}}

#define XSAK_ASSERT(expr,func,context)								\
	{if (expr)														\
		{XSAK_ERROR(SAK_StsInternal,(func),(context));}}

#define XSAK_RSTERR() (sakSetErrStatus(SAK_StsOk))

#define XSAK_CALL( Func )											\
	{																\
		Func;														\
	}

#ifdef SAK_NO_FUNC_NAMES
#	define SAK_FUNCNAME( Name )
#	define s_sakFuncName ""
#else    
#	define SAK_FUNCNAME( Name )					\
    static char s_sakFuncName[] = Name
#endif

/*
  SAK_ERROR macro unconditionally raises error with passed code and message.
  After raising error, control will be transferred to the exit label.
*/
#define SAK_ERROR( Code, Msg )                                       \
{                                                                   \
     sakError( (Code), s_sakFuncName, Msg, __FILE__, __LINE__ );        \
     SAK_EXIT;                                                          \
}

#define SAK_RSTERR() (sakSetErrStatus(SAK_StsOk))

/* Simplified form of SAK_ERROR */
#define SAK_ERROR_FROM_CODE( code )   \
    SAK_ERROR( code, "" )

/*
 SAK_CHECK macro checks error status after sak
 function call. If error detected, control will be transferred to the exit
 label.
*/
#define SAK_CHECK()                                                  \
{                                                                   \
    if( sakGetErrStatus() < 0 )                                      \
        SAK_ERROR( SAK_StsBackTrace, "Inner function failed." );      \
}


/*
 SAK_CALL macro calls SAK function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
*/
#define SAK_CALL( Func )											\
	{																\
    Func;                                                           \
    SAK_CHECK();													\
	}

#define SAK_STD_CALL(Func)											\
	{																\
		if (!(Func))												\
			SAK_ERROR(SAK_StsBackTrace, "Call " #Func " failed");	\
	}

#define SAK_CALL_EVAL(Func, Eval)									\
	{																\
		if ((Func) != (Eval))										\
			SAK_ERROR(SAK_StsBackTrace, "Call " #Func " failed");	\
	}

/* Runtime assertion macro */
#define SAK_ASSERT_CODE( Condition, Code )								\
{                                                                       \
    if( !(Condition) )                                                  \
        SAK_ERROR( (Code), "Assertion: " #Condition " failed" );		\
}

#define SAK_ASSERT( Condition )      	SAK_ASSERT_CODE(Condition, SAK_StsInternal)
#define SAK_ASSERT_PARAM (Condition)	SAK_ASSERT_CODE(Condition, SAK_StsBadArg)
#define sakAssert	assert

#define __SAK_BEGIN__       {
#define __SAK_END__         goto sak_exit; sak_exit: ; }
#define __CLEANUP__


typedef int (SAK_CDECL *SakErrorCallback)( int status, const char* func_name,
										   const char* err_msg, const char* file_name,
										   int line, void* userdata );

/**
 * Error Handling API */

SAKAPI(kvoid) sakSetPrjName(kchar* name);

/* Get current SAK error status */
SAKAPI(int) sakGetErrStatus( void );

/* Sets error status silently */
SAKAPI(void) sakSetErrStatus( int status );



#define SAK_ErrModeLeaf     0   /* Print error and exit program */
#define SAK_ErrModeParent   1   /* Print error and continue */
#define SAK_ErrModeSilent   2   /* Don't print and continue */

/* Retrives current error processing mode */
SAKAPI(int)  sakGetErrMode( void );

/* Sets error processing mode, returns previously used mode */
SAKAPI(int) sakSetErrMode( int mode );

/* Sets error status and performs some additonal actions (displaying message box,
   writing message to stderr, terminating application etc.)
   depending on the current error mode */
SAKAPI(void) sakError( int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line );

/* Retrieves textual description of the error given its code */
SAKAPI(const char*) sakErrorStr( int status );

/* Retrieves detailed information about the last error occured */
SAKAPI(int) sakGetErrInfo( const char** errcode_desc, const char** description,
                        const char** filename, int* line );

/* Maps IPP error codes to the counterparts from OpenCV */
SAKAPI(int) sakErrorFromIppStatus( int ipp_status );

/* Assigns a new error-handling function */
SAKAPI(SakErrorCallback) sakRedirectError( SakErrorCallback error_handler,
                                       void* userdata SAK_DEFAULT(knull),
                                       void** prev_userdata SAK_DEFAULT(knull) );

/*
    Output to:
        sakNulDevReport - nothing
        sakStdErrReport - console(fprintf(stderr,...))
        sakGuiBoxReport - MessageBox(WIN32)
*/
SAKAPI(int) sakNulDevReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

SAKAPI(int) sakStdErrReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

SAKAPI(int) sakGuiBoxReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

/******************************************************************************
 ******************************   Memory Handle  ******************************
 ******************************************************************************/
#define SAK_ALLOC_ALIGN	(4)

typedef khandle (*SakMemCreate_Func)(ksize_t);
typedef kvoid	(*SakMemDestroy_Func)(khandle *handle);
typedef kany_ptr (*SakMemAlloc_Func)(khandle, ksize_t);
typedef kvoid	(*SakMemRelease_Func)(khandle, kany_ptr);

typedef struct SakMemory {
	SakMemCreate_Func	create;
	SakMemDestroy_Func	destroy;
	SakMemAlloc_Func	alloc;
	SakMemRelease_Func	release;
} SakMemory, *SakMemoryPtr;

//@{
/// memory management
SAKAPI(kbool)		sakMemRegister(SakMemoryPtr pMem SAK_DEFAULT(0));
SAKAPI(khandle)		sakMemCreate(ksize_t size SAK_DEFAULT(0));
SAKAPI(kany_ptr)	sakMemAlloc(ksize_t size);
SAKAPI(kvoid)		sakMemRelease(kany_ptr p);
SAKAPI(kvoid)		sakMemDestroy();

#define				sakAlloc(n)		sakMemAlloc(n)
#define				sakMemFree(p)	(sakMemRelease((kany_ptr)p), ((p) = 0))
#define				sakFree(p)		sakMemFree(p)
//@}

#define				sakMemSet(s,c,n)	memset((s),(c),(n))

//@{
/// memory copy

SAK_INLINE kbool	sakMemEqual(kvoid* a, kvoid* b, ksize_t size)
{
	int rem = size % 4;

	size >>= 2;

	{
		kint* pa = (kint*) a;
		kint* pb = (kint*) b;
		while (size-- > 0) {
			if (*pa++ != *pb++)
				return kfalse;
		}
	}

	if (rem != 0) {
		kchar* pa = (kchar*) a;
		kchar* pb = (kchar*) b;
		while (rem-- > 0) {
			if (*pa++ != *pb++)
				return kfalse;
		}
	}

	return ktrue;
}

SAK_INLINE kvoid*	sakMemCpy_blk(kvoid* dst, ksize_t dstride, kvoid* src, ksize_t sstride, ksize_t esize, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		memcpy(dst, src, esize);
		dst = (kchar*)dst + dstride;
		src = (kchar*)src + sstride;
	}

	return dst;
}

#define SAK_MEMCPY_E(dst, dtype, dstep, src, stype, sstep, nobj)	\
	{																\
		dtype* pdst = (dtype*) (dst);								\
		stype* psrc = (stype*) (src);								\
		kint n = (kint) nobj;										\
		while (n-- > 0) {											\
			*pdst = (dtype) *psrc;									\
			pdst += (dstep);										\
			psrc += (sstep);										\
		}															\
	}

#define sakMemCpy_c(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, kchar, dstep, src, kchar, sstep, nobj)

#define sakMemCpy_s(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, kshort, dstep, src, kshort, sstep, nobj)

#define sakMemCpy_i(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kint, dstep, src, kint, sstep, nobj)

#define sakMemCpy_l(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, klong, dstep, src, klong, sstep, nobj)

#define sakMemCpy_f(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, float, dstep, src, float, sstep, nobj)

#define sakMemCpy_d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, double, sstep, nobj)

#define sakMemCpy_c2i(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kint, dstep, src, kchar, sstep, nobj)

#define sakMemCpy_uc2i(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kint, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_uc2ui(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kuint, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_uc2us(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kushort, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_c2s(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, kshort, dstep, src, kchar, sstep, nobj)

#define sakMemCpy_uc2s(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kshort, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_s2i(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, kint, dstep, src, kshort, sstep, nobj)

#define sakMemCpy_us2i(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kint, dstep, src, kushort, sstep, nobj)

#define sakMemCpy_us2ui(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, kuint, dstep, src, kushort, sstep, nobj)

#define sakMemCpy_c2f(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, float, dstep, src, kchar, sstep, nobj)

#define sakMemCpy_uc2f(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, float, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_s2f(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, float, dstep, src, kshort, sstep, nobj)

#define sakMemCpy_us2f(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, float, dstep, src, kushort, sstep, nobj)

#define sakMemCpy_i2f(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, float, dstep, src, kint, sstep, nobj)

#define sakMemCpy_ui2f(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, float, dstep, src, kuint, sstep, nobj)

#define sakMemCpy_l2f(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, float, dstep, src, klong, sstep, nobj)

#define sakMemCpy_ul2f(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, float, dstep, src, kulong, sstep, nobj)

#define sakMemCpy_c2d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, kchar, sstep, nobj)

#define sakMemCpy_uc2d(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, double, dstep, src, kuchar, sstep, nobj)

#define sakMemCpy_s2d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, kshort, sstep, nobj)

#define sakMemCpy_us2d(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, double, dstep, src, kushort, sstep, nobj)

#define sakMemCpy_i2d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, kint, sstep, nobj)

#define sakMemCpy_ui2d(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, double, dstep, src, kuint, sstep, nobj)

#define sakMemCpy_l2d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, klong, sstep, nobj)

#define sakMemCpy_ul2d(dst, dstep, src, sstep, nobj)			\
	SAK_MEMCPY_E(dst, double, dstep, src, kulong, sstep, nobj)

#define sakMemCpy_f2d(dst, dstep, src, sstep, nobj)				\
	SAK_MEMCPY_E(dst, double, dstep, src, float, sstep, nobj)

SAK_INLINE kvoid*	sakMemCpy_e(kchar* dst, ksize_t dstep, kchar* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		*dst = *src;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_s2c(kchar* dst, ksize_t dstep, kshort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < SCHAR_MIN) *dst = SCHAR_MIN;
		else if (v > SCHAR_MAX) *dst = SCHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_i2c(kchar* dst, ksize_t dstep, kint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = *src;
		if (v < SCHAR_MIN) *dst = SCHAR_MIN;
		else if (v > SCHAR_MAX) *dst = SCHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_i2s(kshort* dst, ksize_t dstep, kint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = *src;
		if (v < SHRT_MIN) *dst = SHRT_MIN;
		else if (v > SHRT_MAX) *dst = SHRT_MAX;
		else *dst = (kshort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_us2uc(kuchar* dst, ksize_t dstep,  kushort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = (kuint) *src;
		if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_ui2uc(kuchar* dst, ksize_t dstep, kuint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = *src;
		if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_ui2us(kushort* dst, ksize_t dstep, kuint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = *src;
		if (v > USHRT_MAX) *dst = USHRT_MAX;
		else *dst = (kushort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_c2uc(kuchar* dst, ksize_t dstep, kchar* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_c2us(kushort* dst, ksize_t dstep, kchar* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kushort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_c2ui(kuint* dst, ksize_t dstep, kchar* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_s2uc(kuchar* dst, ksize_t dstep, kshort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_s2us(kushort* dst, ksize_t dstep, kshort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kushort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_s2ui(kuint* dst, ksize_t dstep, kshort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_i2uc(kuchar* dst, ksize_t dstep, kint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_i2us(kushort* dst, ksize_t dstep, kint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else if (v > USHRT_MAX) *dst = USHRT_MAX;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_i2ui(kuint* dst, ksize_t dstep, kint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v < 0) *dst = 0;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_uc2c(kchar* dst, ksize_t dstep, kuchar* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v > CHAR_MAX) *dst = CHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_us2s(kshort* dst, ksize_t dstep, kushort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v > SHRT_MAX) *dst = SHRT_MAX;
		else *dst = (kshort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_us2c(kchar* dst, ksize_t dstep, kushort* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kint v = (kint) *src;
		if (v > CHAR_MAX) *dst = CHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_ui2i(kint* dst, ksize_t dstep, kuint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = (kuint) *src;
		if (v > INT_MAX) *dst = INT_MAX;
		else *dst = (kint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_ui2s(kshort* dst, ksize_t dstep, kuint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = (kuint) *src;
		if (v > SHRT_MAX) *dst = SHRT_MAX;
		else *dst = (kshort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_ui2c(kchar* dst, ksize_t dstep, kuint* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		kuint v = (kuint) *src;
		if (v > CHAR_MAX) *dst = CHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2c(kchar* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > CHAR_MAX) *dst = CHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2uc(kuchar* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2s(kshort* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > SHRT_MAX) *dst = SHRT_MAX;
		else *dst = (kshort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2us(kushort* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > USHRT_MAX) *dst = USHRT_MAX;
		else *dst = (kushort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2i(kint* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > INT_MAX) *dst = INT_MAX;
		else *dst = (kint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_f2ui(kuint* dst, ksize_t dstep, float* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		float v = (float) *src;
		if (v > UINT_MAX) *dst = UINT_MAX;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2c(kchar* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > CHAR_MAX) *dst = CHAR_MAX;
		else *dst = (kchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2uc(kuchar* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > UCHAR_MAX) *dst = UCHAR_MAX;
		else *dst = (kuchar) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2s(kshort* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > SHRT_MAX) *dst = SHRT_MAX;
		else *dst = (kshort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2us(kushort* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > USHRT_MAX) *dst = USHRT_MAX;
		else *dst = (kushort) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2i(kint* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > INT_MAX) *dst = INT_MAX;
		else *dst = (kint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2ui(kuint* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > UINT_MAX) *dst = UINT_MAX;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

SAK_INLINE kvoid*	sakMemCpy_d2f(float* dst, ksize_t dstep, double* src, ksize_t sstep, ksize_t nobj)
{
	while ((kint)nobj-- > 0) {
		double v = (double) *src;
		if (v > FLT_MAX) *dst = FLT_MAX;
		else if (v < FLT_MIN) *dst = FLT_MIN;
		else *dst = (kuint) v;
		dst += dstep;
		src += sstep;
	}

	return dst;
}

//@}

/******************************************************************************
 ******************************   BASIC STRUCTS  ******************************
 ******************************************************************************/
/**
 * Basic Structs
 * 
 */

typedef struct KPoint {
	kint x;
	kint y;
} KPoint;

typedef struct KSize {
	union {
		kint width;
		kint cols;
	};
	union {
		kint height;
		kint rows;
	};
} KSize;

typedef struct KRect {
	union {
		kint	x;
		kint	left;
	};
	union {
		kint	y;
		kint	top;
	};
	kint	width;
	kint	height;
} KRect;

typedef struct KVersion {
	kuint	major;
	kuint	minor;
	kuint	build_num;
	kuint	bugs;
	kchar*	build_date;
	kchar*	copyright;
} KVersion;

SAK_INLINE KPoint kPoint(kint x, kint y)
{
	KPoint pt;
	pt.x = x;
	pt.y = y;
	return pt;
}

SAK_INLINE KSize kSize(kint rows /*height*/, kint cols /*width*/)
{
	KSize size;
	size.rows = rows;
	size.cols = cols;
	return size;
}

SAK_INLINE KRect kRect(kint top, kint left, kint width, kint height)
{
	KRect rect;
	rect.top = top;
	rect.left = left;
	rect.width = width;
	rect.height = height;
	return rect;
}

/**
 * Basic Numbers
 */
/// Mathematic Constant Number
#define SAK_PI   	(3.1415926535897932384626433832795)
#define SAK_LOG2 	(0.69314718055994530941723212145818)

/// Magic Mask
#define SAK_MAGIC_MASK		0xFFFF0000
#define SAK_MAGIC_ARR_MASK	0xFFF30000
/// Matrix magic value, SakMat signature.
#define SAK_ARR_MAGIC_VAL	0xA5200000
#define SAK_MAT_MAGIC_VAL	0xA5210000

#define sakTrunc_c(v)													\
	(((v) > CHAR_MAX ? ((v) = CHAR_MAX) : 0 ) || ((v) < CHAR_MIN ? ((v) = CHAR_MIN) : 0), (v))

#define sakTrunc_uc(v)													\
	(((v) > UCHAR_MAX ? ((v) = UCHAR_MAX) : 0 ) || ((v) < 0 ? ((v) = 0) : 0), (v))

#define sakTrunc_s(v)													\
	(((v) > SHRT_MAX ? ((v) = SHRT_MAX) : 0 ) || ((v) < SHRT_MIN ? ((v) = SHRT_MIN) : 0), (v))

#define sakTrunc_us(v)													\
	(((v) > USHRT_MAX ? ((v) = USHRT_MAX) : 0 ) || ((v) < 0 ? ((v) = 0) : 0), (v))

#define sakTrunc_i(v)													\
	(((v) > INT_MAX ? ((v) = INT_MAX) : 0 ) || ((v) < INT_MIN ? ((v) = INT_MIN) : 0), (v))

#define sakTrunc_ui(v)													\
	(((v) > UINT_MAX ? ((v) = UINT_MAX) : 0 ) || ((v) < 0 ? ((v) = 0) : 0), (v))

#define sakTrunc_l(v)													\
	(((v) > LONG_MAX ? ((v) = LONG_MAX) : 0 ) || ((v) < LONG_MIN ? ((v) = LONG_MIN) : 0), (v))

#define sakTrunc_ul(v)													\
	(((v) > ULONG_MAX ? ((v) = ULONG_MAX) : 0 ) || ((v) < 0 ? ((v) = 0) : 0), (v))



SAKAPI(KVersion*)		sakGetVersion();

//@{
/// Alignment

#define					sakAlign2(x)	(((x) + 1) & ~1)
#define					sakAlign4(x)	(((x) + 3) & ~3)
#define					sakAlign8(x)	(((x) + 7) & ~7)
#define					sakAlign16(x)	(((x) + 15) & ~15)
#define					sakAlign32(x)	(((x) + 31) & ~31)
#define					sakAlign64(x)	(((x) + 63) & ~63)
#define					sakAlign(n, x)	(((x) + (n) - 1) / (n) * (n))
//@}

//@{
/// Arithmetic
#define					sakSwap(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
/// XOR swap algorithm
#define					sakSwap2(a,b)	((a) = ((a) ^ (b)), (b) = ((a) ^ (b)), (a) = ((a) ^ (b)))

#define 				sakMin(a,b)  ((a) > (b) ? (b) : (a))
#define 				sakMax(a,b)  ((a) < (b) ? (b) : (a))
/// min & max without jumps
#define  				sakIMin(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define  				sakIMax(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

/// absolute value without jumps
#ifndef __cplusplus
#	define  			sakIAbs(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#	define  			sakIAbs(a)     abs(a)
#endif
#define  				sakCmp(a,b)    (((a) > (b)) - ((a) < (b)))
#define  				sakSign(a)     sakCmp((a),0)

SAKAPI(kbool)			sakIs2Power(kuint x);
//@}

/******************************************************************************
 ******************************      String      ******************************
 ******************************************************************************/
#include <stdarg.h>

SAKAPI(char) *sakStrSub(const char *s, int i, int j);
SAKAPI(char) *sakStrDup(const char *s, int i, int j, int n);
SAKAPI(char) *sakStrCat(const char *s1, int i1, int j1, const char *s2, int i2, int j2);
SAKAPI(char) *sakStrCatv   (const char *s, ...);
SAKAPI(char) *sakStrReverse(const char *s, int i, int j);
SAKAPI(char) *sakStrMap    (const char *s, int i, int j, const char *from, const char *to);
SAKAPI(int) sakStrPos(const char *s, int i);
SAKAPI(int) sakStrLen(const char *s, int i, int j);
SAKAPI(int) sakStrCmp(const char *s1, int i1, int j1, const char *s2, int i2, int j2);
SAKAPI(int) sakStrChr  (const char *s, int i, int j, int c);
SAKAPI(int) sakStrRChr (const char *s, int i, int j, int c);
SAKAPI(int) sakStrUpto (const char *s, int i, int j, const char *set);
SAKAPI(int) sakStrRUpto(const char *s, int i, int j, const char *set);
SAKAPI(int) sakStrFind (const char *s, int i, int j, const char *str);
SAKAPI(int) sakStrRFind(const char *s, int i, int j, const char *str);
SAKAPI(int) sakStrAny   (const char *s, int i, const char *set);
SAKAPI(int) sakStrMany  (const char *s, int i, int j, const char *set);
SAKAPI(int) sakStrRMany (const char *s, int i, int j, const char *set);
SAKAPI(int) sakStrMatch (const char *s, int i, int j, const char *str);
SAKAPI(int) sakStrRMatch(const char *s, int i, int j, const char *str);
//SAKAPI(void) sakStrFmt(int code, va_list *app,	int put(int c, void *cl), void *cl,	unsigned char flags[], int width, int precision);

SAKAPI(kvoid)		sakStrReverseInplace(kchar s[]);
SAKAPI(kvoid)		sakItoa(kint n, kchar s[]);
SAKAPI(kbool)		sakStrIsSpace(kchar* line);
SAKAPI(kvoid)		sakStrLower(kchar* line);
/* File Name */
SAKAPI(kchar*)		sakFirstNoneSapce(kchar* name);
SAKAPI(kchar*)		sakBaseName(kchar* name);
SAKAPI(kchar*)		sakDirName(kchar* name);
SAKAPI(kchar*)		sakExtName(kchar* name);


/******************************************************************************
 ******************************     Basic ADT    ******************************
 ******************************************************************************/

typedef struct SakList {
	struct SakList* rest;
	void *first;
} SakList, *SakListPtr;

SAKAPI(SakListPtr)	sakListAppend (SakListPtr list, SakListPtr tail);
SAKAPI(SakListPtr)	sakListCopy   (SakListPtr list);
SAKAPI(SakListPtr)	sakListList   (void *x, ...);
SAKAPI(SakListPtr)	sakListPop    (SakListPtr list, void **x);
SAKAPI(SakListPtr)	sakListPush   (SakListPtr list, void *x);
SAKAPI(SakListPtr)	sakListReverse(SakListPtr list);
SAKAPI(kint)	sakListLength (SakListPtr list);
SAKAPI(kvoid)	sakListFree   (SakListPtr *list);
SAKAPI(kvoid)	sakListMap    (SakListPtr list, void apply(void **x, void *cl), void *cl);
SAKAPI(kvoid**)	sakListToArray(SakListPtr list, void *end);

#if 0
/******************************************************************************
 ******************************      MATRIX      ******************************
 ******************************************************************************/

//@{
/// SakMat. Sak Matrix Type.
#define SAK_CN_MAX     64
#define SAK_CN_SHIFT   3
#define SAK_DEPTH_MAX  (1 << SAK_CN_SHIFT)

#define SAK_8U   0
#define SAK_8S   1
#define SAK_16U  2
#define SAK_16S  3
#define SAK_32S  4
#define SAK_32F  5
#define SAK_64F  6
#define SAK_USRTYPE1 7

#define SAK_MAT_MAKETYPE(depth,cn) ((depth) + (((cn)-1) << SAK_CN_SHIFT))
#define SAK_MAT_MAKE_TYPE SAK_MAT_MAKETYPE

#define SAK_8UC1 SAK_MAT_MAKETYPE(SAK_8U,1)
#define SAK_8UC2 SAK_MAT_MAKETYPE(SAK_8U,2)
#define SAK_8UC3 SAK_MAT_MAKETYPE(SAK_8U,3)
#define SAK_8UC4 SAK_MAT_MAKETYPE(SAK_8U,4)
#define SAK_8UC(n) SAK_MAT_MAKETYPE(SAK_8U,(n))

#define SAK_8SC1 SAK_MAT_MAKETYPE(SAK_8S,1)
#define SAK_8SC2 SAK_MAT_MAKETYPE(SAK_8S,2)
#define SAK_8SC3 SAK_MAT_MAKETYPE(SAK_8S,3)
#define SAK_8SC4 SAK_MAT_MAKETYPE(SAK_8S,4)
#define SAK_8SC(n) SAK_MAT_MAKETYPE(SAK_8S,(n))

#define SAK_16UC1 SAK_MAT_MAKETYPE(SAK_16U,1)
#define SAK_16UC2 SAK_MAT_MAKETYPE(SAK_16U,2)
#define SAK_16UC3 SAK_MAT_MAKETYPE(SAK_16U,3)
#define SAK_16UC4 SAK_MAT_MAKETYPE(SAK_16U,4)
#define SAK_16UC(n) SAK_MAT_MAKETYPE(SAK_16U,(n))

#define SAK_16SC1 SAK_MAT_MAKETYPE(SAK_16S,1)
#define SAK_16SC2 SAK_MAT_MAKETYPE(SAK_16S,2)
#define SAK_16SC3 SAK_MAT_MAKETYPE(SAK_16S,3)
#define SAK_16SC4 SAK_MAT_MAKETYPE(SAK_16S,4)
#define SAK_16SC(n) SAK_MAT_MAKETYPE(SAK_16S,(n))

#define SAK_32SC1 SAK_MAT_MAKETYPE(SAK_32S,1)
#define SAK_32SC2 SAK_MAT_MAKETYPE(SAK_32S,2)
#define SAK_32SC3 SAK_MAT_MAKETYPE(SAK_32S,3)
#define SAK_32SC4 SAK_MAT_MAKETYPE(SAK_32S,4)
#define SAK_32SC(n) SAK_MAT_MAKETYPE(SAK_32S,(n))

#define SAK_32FC1 SAK_MAT_MAKETYPE(SAK_32F,1)
#define SAK_32FC2 SAK_MAT_MAKETYPE(SAK_32F,2)
#define SAK_32FC3 SAK_MAT_MAKETYPE(SAK_32F,3)
#define SAK_32FC4 SAK_MAT_MAKETYPE(SAK_32F,4)
#define SAK_32FC(n) SAK_MAT_MAKETYPE(SAK_32F,(n))

#define SAK_64FC1 SAK_MAT_MAKETYPE(SAK_64F,1)
#define SAK_64FC2 SAK_MAT_MAKETYPE(SAK_64F,2)
#define SAK_64FC3 SAK_MAT_MAKETYPE(SAK_64F,3)
#define SAK_64FC4 SAK_MAT_MAKETYPE(SAK_64F,4)
#define SAK_64FC(n) SAK_MAT_MAKETYPE(SAK_64F,(n))

/// The 3th-8th bits are used for the number of channels.
#define SAK_MAT_CN_MASK          ((SAK_CN_MAX - 1) << SAK_CN_SHIFT)
#define SAK_MAT_CN(flags)        ((((flags) & SAK_MAT_CN_MASK) >> SAK_CN_SHIFT) + 1)
/// The 0th, 1th and 2th bits are used for bit depth.
#define SAK_MAT_DEPTH_MASK       (SAK_DEPTH_MAX - 1)
#define SAK_MAT_DEPTH(flags)     ((flags) & SAK_MAT_DEPTH_MASK)
/// The 0th-8th bits are type specified, i.e., type = <bit depth><number of channels>
#define SAK_MAT_TYPE_MASK        (SAK_DEPTH_MAX*SAK_CN_MAX - 1)
#define SAK_MAT_TYPE(flags)      ((flags) & SAK_MAT_TYPE_MASK)
/// The 14th bit is used for the flag of continue.
#define SAK_MAT_CONT_FLAG_SHIFT  14
#define SAK_MAT_CONT_FLAG        (1 << SAK_MAT_CONT_FLAG_SHIFT)
#define SAK_IS_MAT_CONT(flags)   ((flags) & SAK_MAT_CONT_FLAG)
#define SAK_IS_CONT_MAT          SAK_IS_MAT_CONT
/* #define SAK_MAT_TEMP_FLAG_SHIFT  15 */
/* #define SAK_MAT_TEMP_FLAG        (1 << SAK_MAT_TEMP_FLAG_SHIFT) */
/* #define SAK_IS_TEMP_MAT(flags)   ((flags) & SAK_MAT_TEMP_FLAG) */

#define SAK_DEFAULT_MAT_ROW_ALIGN (4)

typedef void	SakArr;

#define SAK_MAT_FIELDS()											\
	kuint	type;			  /**< signature, type and flags. */	\
	KSize	size;			  /**< rows and cols. */				\
	ksize_t	step;													\
	ksize_t	stride;			  /**< row's bytes width. */			\
	kbool	support_refcount; /**< if support reference count. */	\
	union {					  /**< pointer to data. */				\
			kint8*		pc;											\
			kint16*		ps;											\
			kint32*		pi;											\
			kuint8*		puc;										\
			kuint16*	pus;										\
			kuint32*	pui;										\
			float*		pf;											\
			double*		pd;											\
	};

typedef struct SakMat {
	SAK_MAT_FIELDS()
} SakMat;

//@}

//@{
///  Matrix
						
#define					sakIsMatHdr(mat)								\
    ((mat) != knull &&													\
	 (((const SakMat*)(mat))->type & SAK_MAGIC_MASK)					\
	 == SAK_MAT_MAGIC_VAL &&											\
	 ((const SakMat*)(mat))->size.cols > 0 &&							\
	 ((const SakMat*)(mat))->size.rows > 0)

#define					sakIsMat(mat)								\
    (SAK_IS_MAT_HDR(mat) && ((const SakMat*)(mat))->pc != knull)

/// Size of each channel item,
/// 0x8442211 = 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem)
#define					sakMatElemSize1(type)								\
    ((((sizeof(size_t)<<28)|0x8442211) >> SAK_MAT_DEPTH(type)*4) & 15)

/// 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem))
#define					sakMatElemSize(type)								\
    (SAK_MAT_CN(type) <<												\
	 (((((sizeof(size_t)/4+1)<<14)|0x3a50) >> SAK_MAT_DEPTH(type)*2) & 3))

#define					sakMatMakeType			SAK_MAT_MAKETYPE

#define					sakMatData(mat)			((mat)->puc)
#define					sakMatData_c(mat)		((mat)->pc)
#define					sakMatData_s(mat)		((mat)->ps)
#define					sakMatData_i(mat)		((mat)->pi)
#define					sakMatData_uc(mat)		((mat)->puc)
#define					sakMatData_us(mat)		((mat)->pus)
#define					sakMatData_ui(mat)		((mat)->pui)
#define					sakMatData_f(mat)		((mat)->pf)
#define					sakMatData_d(mat)		((mat)->pd)
#define					sakMatSize(mat)			((mat)->size)
#define					sakMatStep(mat)			((mat)->step)
#define					sakMatType(mat)			(SAK_MAT_TYPE((mat)->type))
#define					sakMatCN(mat)			(SAK_MAT_CN(sakMatType(mat)))
#define					sakMatDepth(mat)		(8 << (SAK_MAT_DEPTH(sakMatType(mat)) >> 1))
#define					sakMatDepthType(mat)	(SAK_MAT_DEPTH(sakMatType(mat)))
#define					sakMatStride(mat)		((mat)->stride)
#define					sakMatRC(mat)			((mat)->support_refcount)
#define					sakMatRefcount			sakMatRC
SAKAPI(SakMat*)			sakMatSupportRefcount(SakMat* mat, kbool to_support);
SAKAPI(kvoid*)			sakMatPtrAt(SakMat* mat, KPoint pt);
SAKAPI(SakMat*)			sakMatStepSet(SakMat* mat, ksize_t step);
#define					sakMatRCSet				sakMatSupportRefcount
#define					sakMatRefcountSet		sakMatSupportRefcount

#define					sakMatValidSize(size) ((size).rows > 0 && (size).cols > 0)
//#define					sakMatValidType(type) ((type))

SAKAPI(SakMat*)			sakMatCreate(KSize size, kuint type);
SAKAPI(kvoid)			sakMatRelease(SakMat* mat);
#define					sakMatRetain(mat) sakMatDataRetain(mat)
SAKAPI(SakMat*)			sakMatClone(SakMat* mat);
SAKAPI(SakMat*)			sakMatCopyFromRange(SakMat* mat, const kvoid* data, kuint type, ksize_t stride, ksize_t step);
SAKAPI(SakMat*)			sakMatCopyFromMat(SakMat* mat, const SakMat*  from);

SAKAPI(SakMat*)			sakMatHeaderCreate(KSize size, kuint type);
SAKAPI(SakMat*)			sakMatHeaderInit(SakMat* mat, KSize size, kuint type);
SAKAPI(kvoid)			sakMatHeaderRelease(SakMat* mat);
#define					sakMatCreateHeader	sakMatHeaderCreate
#define					sakMatInitHeader	sakMatInitHeader
#define					sakMatReleaseHeader	sakMatReleaseHeader

SAKAPI(SakMat*)			sakMatDataSet(SakMat* mat, kvoid* data, ksize_t stride, ksize_t step);
SAKAPI(SakMat*)			sakMatDataCreate(SakMat* mat);
SAKAPI(kvoid)			sakMatDataRelease(SakMat* mat);
SAKAPI(SakMat*)			sakMatDataRetain(SakMat* mat);
#define					sakMatSetData		sakMatDataSet
#define					sakMatCreateData	sakMatDataCreate
#define					sakMatReleaseData	sakMatDataRelease
#define					sakMatRetainData	sakMatDataRetain

//@}

#endif//0

#endif//__SAK_SAKCORE_H__
