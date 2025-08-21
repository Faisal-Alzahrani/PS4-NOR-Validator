#ifndef PS4_HEADERS_H
#define PS4_HEADERS_H


#include "ps4_def.h"

typedef struct {
	uint32_t start_lba;
	uint32_t n_sectors;
	uint8_t flag1;       // maybe part_id
	uint8_t flag2;
	uint16_t unknown;
	uint64_t padding;
} PACK(partition_t);

//NOTE: probably unused code.. fw 1.06 has version 4..
typedef struct {
	uint8_t magic[0x20]; // "SONY COMPUTER ENTERTAINMENT INC."
	uint32_t version;    // 1
	uint32_t mbr1_start; // ex: 0x10
	uint32_t mbr2_start; // ex: 0x18
	uint32_t unk[4];     // ex: (1, 1, 8, 1)
	uint32_t reserved;
	uint8_t unused[0x1C0];
} PACK(master_block_v1_t);

typedef struct {
	uint8_t magic[0x20];   // "Sony Computer Entertainment Inc."
	uint32_t version;      // 4
	uint32_t size;
	uint64_t reserved;
	uint32_t loader_start; // ex: 0x11, 0x309
	uint32_t loader_count; // ex: 0x267
	uint64_t reserved2;
	partition_t partitions[16];
} PACK(master_block_v4_t);

/* SLB2 structure is little endian */
#define SLB2_HEADER_SIZE 512
#define SBL2_ENTRY_MAX 10

typedef struct {
	uint32_t fileStartSector;
	uint32_t fileSizeInByte;
	uint32_t reserved[2]; // padding for alignment
	char     fileName[32];
}PACK(SceSlb2Entry);

typedef struct {
	uint32_t magic;       // "SLB2" 0x32424C53
	uint32_t version;     // ex: 1
	uint32_t flags;       // ex: 0
	uint32_t entryNum;
	uint32_t sizeInSector;//Each sector is 512 byte padded with zero.
	uint32_t reserved[3]; // padding for alignment
	SceSlb2Entry entryList[0];
}PACK(SceSlb2Header);


#define ACF_OFFSET  0x1C9080

typedef struct SceAcf {
	char magic[4];
	uint32_t format_ver;
	char activation_key[16];
	uint32_t issue_no;
	uint32_t start_date;
	uint32_t expire_date;
	uint32_t reserved;
	char key_hash[64];
} SceAcf;

//Virtual Table Rights Management
//region 1:same on the same console of different FW and BIOS versions
//the md5 of this region can be used as a unique console id.
#define VTRM_REGION_1 0x00380000
#define VTRM_REGION_2 0x003A0000
#define VTRM_REGION_LEN 0x60
//VTRM status
#define VTRM_FACTORY  0xFC
#define VTRM_ACTIVE   0x03000000
#define VTRM_DEACTIVE 0x00000000

typedef struct{
	uint8_t flag[4];     //Marks the VTRM status. Factory state = 0xFC (1 byte). Activation state = 0x03000000. Deactivation state = 0x00000000
	uint8_t padding1[60];//0xFF
	uint32_t constant;   //0x01000000
	uint8_t padding2[4]; //0xFF
	uint8_t magic[8];    //SCEVTRM 
	uint32_t count1;     //active/deactive count
	uint8_t padding3[4]; //0x00
	uint32_t count2;
	uint8_t padding4[4]; //0x00
	uint8_t unknown[16]; //const bytes
	uint8_t padding5[8]; //0xFF
	uint8_t ctrlFlag[1]; //Factory state = 0xFF. First activation state = 0xFE. Else it will be always state = 0xFC
	uint8_t padding6[7]; //0xFF
} PACK(vtrmHeader);

#define ACF_OFFSET 0x1C9080 

typedef struct{
	uint8_t magic[8];  //pfsSKKey
	uint8_t keyset[2]; //
	uint8_t padding[6];//0x00
	uint8_t iv[16];
	uint8_t key[32];
	uint8_t sha256[32];
} PACK(PfsSKKey);

#endif //PS4_HEADERS_H
