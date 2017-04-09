#pragma once

#include <stdio.h>

int search_file(const char* fname, const char* search,
				int (*callback)(FILE* out, const char** iterator));