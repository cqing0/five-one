#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"

char *LoadShaderData(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file) {
		printf("Cannot open file, %s\n", filename);
		return NULL;
	}

	printf("Loaded file %s\n", filename);

	fseek(file, 0L, SEEK_END);
	long length = ftell(file);
	rewind(file);

	char *content ;
	content = (char*)malloc(length + 1);
	if (!content) {
		printf("Cannot alloc content, %s\n", content);
		fclose(file);
		return NULL;
	}

	fread(content, 1, length, file);
	content[length] = '\0';

	if (fclose(file) != 0) {
		printf("File %s could not be closed!\n", filename);
	}
	else {
		printf("Closed file %s\n", filename);
	}

	return content;
}

char *LoadBitmap_GL(const char *filename)
{
	// Bitmap details
	unsigned char header[54];
	unsigned int data_pos;
	unsigned int width;
	unsigned int height;
	unsigned int image_size;
	char *rgb_data;

	FILE *file = fopen(filename, "rb");
	if (!file) {
		printf("Cannot open file, %s\n", filename);
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54) {
		printf("Error: file %s does not have a 54 byte header!\n", filename);
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Error file %s is not a correct bmp file!\n", filename);
		return NULL;
	}

	// Read in the bytes
	data_pos = *(int *)&(header[0x0A]);
	image_size = *(int *)&(header[0x22]);
	width = *(int *)&(header[0x12]);
	height = *(int *)&(header[0x0A]);

	if (image_size == 0) {
		image_size = width * height * 3;
	}
	if (data_pos == 0) {
		data_pos = 54;
	}

	rgb_data = (char *)malloc(image_size * sizeof(unsigned char));

	fread(rgb_data, 1, image_size, file);

	if (fclose(file) != 0) {
		printf("Error could not close file %s\n", filename);
		return NULL;
	}

	return rgb_data;
}
