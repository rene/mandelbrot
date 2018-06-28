/**
 * mandelbrot - Simple mandelbrot generator
 * Author: Renê de Souza Pinto
 * Licence: Public Domain
 */
#ifndef MANDELBROT_H
#define MANDELBROT_H

#define BYTE  char
#define WORD  unsigned short int
#define DWORD unsigned int
#define LONG  unsigned long long int

#define BMP_SIGNATURE "BM"

struct _BMPFileHeader {
	BYTE FileType[2];   /* Signature */
	DWORD FileSize;     /* Total file size (in bytes) */
	WORD  Reserverd1;   /* Reserved */
	WORD  Reserverd2;   /* Reserved */
	DWORD BitmapOffset; /* Bitmap offset */
} __attribute__((packed));

struct _BMPHeader {
	DWORD Size;         /* Header size (in bytes) */
	DWORD Width;        /* Width */
	DWORD Height;       /* Height */
	WORD  Planes;       /* Number of color planes */
	WORD  BitsPerPixel; /* Bits per pixel */
	DWORD Compression;  /* Compression method */
	DWORD SizeOfBitmap; /* Bitmap size (in bytes) */
	DWORD HorzRes;
	DWORD VertRes;
	DWORD ColorsUserd;
	DWORD ColorsImportant;
} __attribute__((packed));

typedef struct _BMPFileHeader BMPFileHeader_t;
typedef struct _BMPHeader BMPHeader_t;

#endif /* MANDELBROT_H */
