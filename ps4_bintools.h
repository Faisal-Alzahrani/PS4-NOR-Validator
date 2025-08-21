#ifndef PS4_BINTOOLS
#define PS4_BINTOOLS

int ps4_is_blank(const unsigned char* buffer, unsigned int len);
void ps4_hex_to_str(unsigned char* strout, const unsigned char* strin, unsigned int len);
void ps4_md5(unsigned char* strout, const unsigned char* strin, unsigned int len);
double ps4_entropy(const unsigned char* buffer, const double size);
void ps4_create_imgdiff(unsigned char* imgDiff, const unsigned char* next);

#endif //PS4_BINTOOLS