/**
 * mandelbrot - Simple mandelbrot generator
 * Author: Renê de Souza Pinto
 * Licence: Public Domain
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mandelbrot.h"

#define PALLETE_SIZE 384

void mandelbrot(unsigned char *buffer, unsigned int width, unsigned int height);

int main(int argc, const char *argv[])
{
	char *outfile;
	FILE *fout;
	BMPFileHeader_t fileHeader;
	BMPHeader_t bmpHeader;
	unsigned char *bitmap;
	unsigned int bmapSize;
	int r, width, height;

	/* Check args */
	if (argc != 4) {
		printf("Use: %s <width> <height> <output_file>\n", argv[0]);
		return EXIT_FAILURE;
	} else {
		width   = atoi(argv[1]);
		height  = atoi(argv[2]);
		outfile = (char*)argv[3];

		/* Force width to be multiple of 4, so we don't need to
		 * pad pixmap bitmap data */
		r = (width % 4);
		if (r != 0) {
			width -= r;
		}

		if (width <= 0 || height <= 0) {
			printf("Invalid width/height value.\n");
			return EXIT_FAILURE;
		}
	}

	/* Create output file */
	fout = fopen(outfile, "w+");
	if (fout == NULL) {
		perror(outfile);
		return 1;
	}

	memset(&fileHeader, 0, sizeof(fileHeader));
	memset(&bmpHeader, 0, sizeof(bmpHeader));

	/* 3 bytes per pixel (RGB) */
	bmapSize = width * height * 3;

	/* Build headers */
	strncpy(fileHeader.FileType, BMP_SIGNATURE, 2);
	fileHeader.FileSize    = sizeof(fileHeader) + sizeof(bmpHeader) + bmapSize;
	bmpHeader.Size         = sizeof(bmpHeader);
	bmpHeader.Width        = width;
	bmpHeader.Height       = height;
	bmpHeader.Planes       = 1;
	bmpHeader.BitsPerPixel = 24;
	bmpHeader.SizeOfBitmap = bmapSize;

	/* Allocate memory */
	bitmap = (unsigned char*)malloc(bmpHeader.SizeOfBitmap);
	if (bitmap == NULL) {
		perror("malloc()");
		fclose(fout);
		return 1;
	}

	/* Mandelbrot */
	mandelbrot(bitmap, width, height);

	/* Write output information */
	printf("Output information:\n");
	printf("File Name: %s\n", outfile);
	printf("File size: %d bytes\n", fileHeader.FileSize);
	printf("Width:  %d\n", bmpHeader.Width);
	printf("Height: %d\n\n", bmpHeader.Height);

	/* Write output file */
	fileHeader.BitmapOffset = sizeof(fileHeader) + sizeof(bmpHeader);
	fwrite(&fileHeader, sizeof(fileHeader), 1, fout);
	fwrite(&bmpHeader, sizeof(bmpHeader), 1, fout);
	fwrite(bitmap, bmpHeader.SizeOfBitmap, 1, fout);
	
	fclose(fout);
	free(bitmap);
	return EXIT_SUCCESS;
}

void mandelbrot(unsigned char *buffer, unsigned int width, unsigned int height)
{
	unsigned int i, j, k, p, c;
	int it, maxit;
	double x, y, xtemp, x0, y0;
	unsigned char palette[PALLETE_SIZE][3];

	/* Build color palette */
	i = 0;
	j = 128;
	k = 256;
	while (i < 128) {
		palette[i][0] = (i*2);
		palette[i][1] = 0;
		palette[i][2] = 0;

		palette[j][0] = (i*2);
		palette[j][1] = 127 + i;
		palette[j][2] = 0;

		palette[k][0] = 255 - i;
		palette[k][1] = 255;
		palette[k][2] = 0;

		i++;
		j++;
		k++;
	}
	palette[0][0] = 0;
	palette[0][1] = 0;
	palette[0][2] = 0;

	/* Mandelbrot */
	maxit = 1000;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			x0 = ((3.5/(double)width) * j) - 2.5;
			y0 = ((2.0/(double)height) * i) - 1.0;
			x  = 0.0;
			y  = 0.0;
			it = 0;
			while (x*x + y*y <= 4 && it < maxit) {
				xtemp = x*x - y*y + x0;
				y     = 2*x*y + y0;
				x     = xtemp;
				
				it++;
			}

			if (it == maxit) {
				c = 0;
			} else {
				c = it % PALLETE_SIZE;
			}
			p = ((i * width) + j) * 3;
			buffer[p]   = palette[c][2]; /* B */
			buffer[p+1] = palette[c][1]; /* G */
			buffer[p+2] = palette[c][0]; /* R */
		}
	}
}

