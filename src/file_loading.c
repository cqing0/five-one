#include <stdio.h>
#include "file_loading.h"

char *LoadShaderData(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		printf("Cannot open file, %s\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *content = (char *)malloc(length + 1);
	if (!content) {
		printf("Cannot alloc content, %s\n", content);
		fclose(file);
		return NULL;
	}

	fread(content, 1, length, file);
	content[length] = '\0';

	fclose(file);
	return content;
}
