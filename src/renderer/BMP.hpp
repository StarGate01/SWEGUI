#ifndef BMP_H
#define BMP_H

#define BMP_BITCOUNT            24
#define BMP_BYTECOUNT           3
#define BMP_PLANES              1

#define BMP_HEADER_SIZE         54
#define BMP_FILEHEADER_SIZE     14
#define BMP_INFOHEADER_SIZE     40

#define BMP_MAGIC_1             0x42
#define BMP_MAGIC_2             0x4D

#define BMP_OFFSET_BF_TYPE      0
#define BMP_OFFSET_BF_SIZE      2
#define BMP_OFFSET_BF_OFFBITS   10

#define BMP_OFFSET_BI_SIZE      14
#define BMP_OFFSET_BI_WIDTH     18
#define BMP_OFFSET_BI_HEIGHT    22
#define BMP_OFFSET_BI_PLANES    26
#define BMP_OFFSET_BI_BITCOUNT  28
#define BMP_OFFSET_BI_SIZEIMAGE 34

#endif