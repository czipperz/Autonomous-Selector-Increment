#include "targetver.h"
#include "search.h"
#include <stdio.h>
#include <string.h>

int search_file(const char* fname, const char* search,
				int (*callback)(FILE* out, const char** iterator)) {
	FILE* file;
	FILE* fileout;
	char foutname[L_tmpnam];
	char buffer[2049];
	size_t index;
	const char* ptr;

	file = fopen(fname, "r");
	if (!file) {
		fprintf(stderr, "Failed to open file %s!\n", fname);
		goto fail_wait;
	}
	tmpnam(foutname);
	fileout = fopen(foutname, "w");
	if (!fileout) {
		fprintf(stderr, "Failed to open file %s!\n", foutname);
		fclose(file);
		goto fail_wait;
	}
	if (1024 != fread(buffer, sizeof(char), 1024, file)) {
		fprintf(stderr, "Failed to read first 1024 bytes from %s!\n", fname);
		goto fail;
	}
	while (0 != (index = fread(buffer + 1024, sizeof(char), 1024, file))) {
		buffer[1024 + index] = '\0';
		if ((ptr = strstr(buffer, search))) {
			goto found;
		}
		fwrite(buffer, sizeof(char), 1024, fileout);
		memmove(buffer, buffer + 1024, 1024);
	}

fail:
	fclose(file);
	fclose(fileout);
fail_wait:
	getchar();
	return 1;

found:
	// Write before search
	fwrite(buffer, sizeof(char), ptr - buffer, fileout);
	// Ensure we have enough space for all the number.  Otherwise it might be spliced between two 1024 byte strings.
	size_t i;
	if (ptr >= buffer + 1024 && index == 1024) {
		memmove(buffer, buffer + 1024, 1024);
		// Ignore EOF return, it will be detected later.
		i = fread(buffer + 1024, sizeof(char), 1024, file);
		buffer[1024 + i] = '\0';
		ptr -= 1024;
	}
	ptr += strlen(search);
	// Write search
	fputs(search, fileout);

	if (callback(fileout, &ptr)) { return 1; }

	// Write after callback
	fwrite(ptr, sizeof(char), buffer + 1024 + index - ptr, fileout);
	while (0 != (index = fread(buffer, sizeof(char), 2048, file))) {
		fwrite(buffer, sizeof(char), index, fileout);
	}
	fclose(file);
	fclose(fileout);
	remove(fname);
	rename(foutname, fname);
	return 0;
}