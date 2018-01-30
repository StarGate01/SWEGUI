/**
 * @brief Defines constants for generating bitmap images
 * @file BMP.hpp
*/

#ifndef BMP_H
#define BMP_H

#define BMP_BITCOUNT            24      ///<Number of bits per pixel
#define BMP_BYTECOUNT           3       ///<Number of bytes per pixel
#define BMP_PLANES              1       ///<Number of planes in the image (deprecated)

#define BMP_HEADER_SIZE         54      ///<Size of the BMP header, in bytes
#define BMP_FILEHEADER_SIZE     14      ///<Size of the BMP file header, in bytes
#define BMP_INFOHEADER_SIZE     40      ///<Size of the BMP info header, in size

#define BMP_MAGIC_1             0x42    ///<Magic number of the BMP format, part one
#define BMP_MAGIC_2             0x4D    ///<Magic number of the BMP format, part two

#define BMP_OFFSET_BF_TYPE      0       ///<BMP configuration type
#define BMP_OFFSET_BF_SIZE      2       ///<BMP configuration size
#define BMP_OFFSET_BF_OFFBITS   10      ///<BMP configuration offset

#define BMP_OFFSET_BI_SIZE      14      ///<Offset of the image configuration block
#define BMP_OFFSET_BI_WIDTH     18      ///<Offset of the pixel width configuration
#define BMP_OFFSET_BI_HEIGHT    22      ///<Offset of the pixel height configuration
#define BMP_OFFSET_BI_PLANES    26      ///<Offset of the planes configuration
#define BMP_OFFSET_BI_BITCOUNT  28      ///<Offset of the bitcount configuration
#define BMP_OFFSET_BI_SIZEIMAGE 34      ///<Config of the size configuration

#endif