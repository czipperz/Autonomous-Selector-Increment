// ConsoleApplication1.cpp : Defines the e\ntry point for the console application.
//

#include "targetver.h"
#include "search.h"
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

static int search_callback(FILE* file, const char** iterator) {
	time_t t = time(0);
	tm* info = localtime(&t);
	char* str;
	assert(info);
	str = asctime(info);
	assert(str);
	{
		char* i;
		if (i = strchr(str, '\n')) {
			*i = '\0';
		}
	}
	fputs(str, file);
	fflush(file);
	*iterator = strchr(*iterator, '"');
	assert(*iterator);
	return 0;
}

int main() {
	return search_file(
		"C:/Users/999/git/2017SteamBot/src/org/usfirst/frc/team997/robot/Robot.java",
		"final String autoChoicesNumber = \"",
		search_callback);
}

