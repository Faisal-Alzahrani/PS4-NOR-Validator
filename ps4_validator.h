#ifndef PS4_VALIDATOR_H
#define PS4_VALIDATOR_H

#include <stdio.h>
#include "ps4_def.h"

#define VA_ERROR  0
#define VA_WARNING  2
#define VA_OK  1

typedef struct
{
	unsigned char buffer[NOR_SIZE];
	char name[FILENAME_MAX];
	char md5[36];
	const char* filename;
	FILE* outfile;
	char isRetail;
	char isValid;
	char torusVersion;
	unsigned char region;
	const char* regionStr;
	float version;
	char versionMajor;
	char versionMinor;
	char deltaLevel;
	int countError;
	int countWarning;
	int countOk;
}PS4Nor;


int ps4_init(PS4Nor *obj, const char* filename);
int ps4_validate(PS4Nor* obj, PS4Nor* delta, const char* outfilename);
void ps4_info(PS4Nor *obj);
void ps4_check_system_flags(PS4Nor* obj);
void ps4_out(PS4Nor* obj, Color color, int r, const char* format, ...);

#endif //PS4_VALIDATOR_H