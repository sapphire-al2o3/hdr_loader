#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include "image.h"

struct BGR {
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

template <typename T>
int SaveBitmap(const char *fileName, const Image<T> *src);

#pragma pack(1)

struct BitmapFileHeader { 
	unsigned short bfType;			// "BM"
	unsigned int   bfSize;			// ファイルサイズ
	unsigned short bfReserved1;		// 0
	unsigned short bfReserved2;		// 0
	unsigned int   bfOffBits;		// 画像データまでのオフセット
};

struct BitmapInfoHeader {
	unsigned int   biSize;			// 構造体のサイズ
	long           biWidth;			// 幅
	long           biHeight;		// 高さ
	unsigned short biPlanes;		// 1
	unsigned short biBitCount;		// ビット数
	unsigned int   biCompression;	// 圧縮方式
	unsigned int   biSizeImage;		// 画像データのサイズ
	long           biXPelsPerMeter;	// 水平解像度
	long           biYPelsPerMeter;	// 垂直解像度
	unsigned int   biClrUsed;		// 色数
	unsigned int   biClrImportant;	// 重要な色数
};

#pragma pack()

inline void WriteLine24(const BGR *src, FILE *fp, int length)
{
	fwrite(src, sizeof(BGR), length, fp);
}

/*
 * Save Bitmap file by 24 bit
 */
template <typename T>
int SaveBitmap(const char *fileName, const Image<T>& src)
{
	int i, linemod, linebyte;
	BitmapFileHeader head;
	BitmapInfoHeader info;
	FILE *fp;
	
	if(src.lines == nullptr) {
		printf("%s save failed!\n", fileName);
		return 0;
	}
	
	linemod = src.width * 3 % 4;
	
	if(linemod == 0)
		linebyte = src.width * 3;
	else
		linebyte = src.width * 3 + (4 - linemod);
	
	// write bitmap file header
	head.bfType = 'B' | 'M' << 8;
	head.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + linebyte * src.height;
	head.bfReserved1 = 0;
	head.bfReserved2 = 0;
	head.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	
	// write bitmap info header
	info.biSize = sizeof(BitmapInfoHeader);
	info.biWidth = src.width;
	info.biHeight = src.height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = linebyte * src.height;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant= 0;
	
	fopen_s(&fp, fileName, "wb");
	
	if(fileName == nullptr || fp == nullptr) return 0;
	
	// write header
	fwrite(&head, sizeof(BitmapFileHeader), 1, fp);
	fwrite(&info, sizeof(BitmapInfoHeader), 1, fp);
	
	const int padding = 0;
	
	// write data
	for(i = 0; i < src.height; i++) {
		WriteLine24(src.lines[info.biHeight - i - 1], fp, info.biWidth);
		// 4byteの境界にあわせて0を書き込む
		if(linemod != 0) fwrite(&padding, 4 - linemod, 1, fp);
	}
	
	fclose(fp);
	return 1;
}
#endif //_BMP_H
