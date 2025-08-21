#include <stdlib.h>
#include <math.h>

#include "ps4_bintools.h"
#include "ps4_validator.h"



int ps4_is_blank(const unsigned char* buffer, unsigned int len)
{
	unsigned int count_00 = 0;
	unsigned int count_ff = 0;

	for (unsigned int i = 0; i < len; ++i)
	{
		if (buffer[i] == 0xFF)
		{
			++count_ff;
		}
		else if (buffer[i] == 0x00)
		{
			++count_00;
		}
	}

	return count_ff == len || count_00 == len;
}


static const char* hex = "0123456789ABCDEF";

void ps4_hex_to_str(unsigned char* strout, const unsigned char* strin, unsigned int len)
{
    memset(strout, '\0', (len<<1) + 1);

	for (int i = 0; i < len; ++i)
	{
		*strout++ = hex[(*strin >> 4) & 0xF];
		*strout++ = hex[(*strin++) & 0xF];
	}
}

void ps4_md5(unsigned char* strout, const unsigned char* strin, unsigned int len)
{
	static MD5Context md5context;
	md5Init(&md5context);
	md5Update(&md5context, (unsigned char*)strin, len);
	md5Finalize(&md5context);
	ps4_hex_to_str(strout, md5context.digest, 16);
}


double ps4_entropy(const unsigned char* buffer, const double size)
{
	char c = 0;
	double entropy = 0;

	do
	{
		double count = 0;

		for (int i = 0; i < size; ++i)
		{
			 char x = buffer[i];

			if (c == x)
			{
				++count;
			}
		}

		double p = count / size;

		if (p > 0)
		{
			entropy = entropy - p * log2(p);
		}
		++c;
	} while (c != 0);//signed char overflow!

	return entropy;
}


void ps4_create_imgdiff(unsigned char *imgDiff, const unsigned char* next)
{
	for (int j = 0; j < NOR_SIZE; ++j)
	{
		//static 0xFF's are matching and excluded
		if (imgDiff[j] != next[j])
		{
			unsigned char c1 = imgDiff[j];
			unsigned char c2 = next[j];

			//try to find all zero offsets
			if (c2 == 0x00)
			{
				imgDiff[j] = 0x0;
			}
			else if (c1 != 0)
			{
				//replace other bytes with 0xFB
				//effectively creating a map of non-blank offsets..
				imgDiff[j] = 0xFE;
			}
		}
	}
}
