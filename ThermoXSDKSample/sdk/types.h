#ifndef TYPES_H
#define TYPES_H

/*
*/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

#define S8_MIN			(-128)
#define S8_MAX			127
#define S16_MIN			(-32768)
#define S16_MAX			32767
#define S32_MIN			(-2147483647 - 1)

typedef struct _bgra{
	u8 b;
	u8 g;
	u8 r;
	u8 a;
}bgra;

#define STRING_LENGTH_16							16
#define STRING_LENGTH_32							32

/*
*/
#define ARRAY_SIZE(x)			(sizeof(x)/sizeof(x[0]))
#define CLEAR(x)				memset(&(x), 0, sizeof(x))
#define OFFSET(type,member)		(size_t)&(((type*)0)->member)

#define xmalloc(size,alignment)	_aligned_malloc(size,alignment)
#define xfree(x)				_aligned_free(x)

#ifdef CAMERA_EXPORT
#define CAMERA_API __declspec(dllexport)
#else
#define CAMERA_API __declspec(dllimport)
#endif

#endif /* TYPES_H */