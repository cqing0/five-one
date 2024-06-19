#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"

char *LoadShaderData(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("Cannot open file, %s\n", filename);
		return NULL;
	}

	printf("Loaded file %s\n", filename);

	fseek(file, 0L, SEEK_END);
	long length = ftell(file);
	rewind(file);

	char *content = (char*)malloc(length + 1);
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
