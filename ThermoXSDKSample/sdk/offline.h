#ifndef OFFLINE_H
#define OFFLINE_H

#include "camera.h"

#pragma pack(push, 1)

#ifndef WIN32
typedef struct tagBITMAPFILEHEADER{
	u16 bfType;
	u32 bfSize;
	u16 bfReserved1;
	u16 bfReserved2;
	u32 bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	u32 biSize;
	s32 biWidth;
	s32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	s32 biXPelsPerMeter;
	s32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
	u8 rgbBlue;
	u8 rgbGreen;
	u8 rgbRed;
	u8 rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO;
#endif

typedef struct _bmp_file_header{
	BITMAPFILEHEADER bfHeader;
	BITMAPINFOHEADER biHeader;
	u8 pad[10];
}bmp_file_header;

typedef struct _frame_file_header{
	//basic
	char camera_id[32];
	s64 captured_time;

	u16 width;
	u16 height;

	//params
	FixInfo fix_info;

	//
	u8 reserved[12];
}frame_file_header;

typedef struct _frame_file{
	bmp_file_header* bfh;
	bgra* bfd;
	bgra* palette;
	frame_file_header* ffh;
	DataFrameHeader* dfh;
	s16* temp;
	void* measure;//4+measure_object+...
}frame_file;

typedef struct _stream_file_header{
	//basic
	char camera_id[32];
	s64 captured_time;
	u16 width;
	u16 height;

	//params
	FixInfo fix_info;

	//
	u8 fps;
	u8 pad[3];
	u32 frame_count;

	u8 reserved[4];

}stream_file_header;

typedef struct _stream_file{
	bmp_file_header* bfh;
	bgra* bfd;
	bgra* palette;
	stream_file_header* sfh;
}stream_file;

#pragma pack(pop)

#endif