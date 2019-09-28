//------------------------------------------------------------------------------
// jpg2tga.c
// JPEG to TGA file conversion example program.
// Public domain, Rich Geldreich <richgel99@gmail.com>
// Last updated Nov. 26, 2010
//------------------------------------------------------------------------------
#include "picojpeg.h"
#include "JPEG_image.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "LCD.h"
#include "LCD_driver.h"

// #include "stb_image.c"
//------------------------------------------------------------------------------
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
//------------------------------------------------------------------------------
typedef unsigned char uint8;
typedef unsigned int uint;
//------------------------------------------------------------------------------
static FILE *g_pInFile;
static uint g_nInFileSize;
static uint g_nInFileOfs;
char * g_pInData;
//------------------------------------------------------------------------------
unsigned char pjpeg_need_bytes_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data)
{
   uint n;
   pCallback_data;
   
   n = min(g_nInFileSize - g_nInFileOfs, buf_size);
	 memcpy(pBuf, g_pInData,n);
//   if (n && (fread(pBuf, 1, n, g_pInFile) != n))
//      return PJPG_STREAM_READ_ERROR;
   *pBytes_actually_read = (unsigned char)(n);
   // g_nInFileOfs += n;
	 g_pInData += n;
   return 0;
}

//------------------------------------------------------------------------------
typedef struct image_compare_results_tag
{
   double max_err;
   double mean;
   double mean_squared;
   double root_mean_squared;
   double peak_snr;
} image_compare_results;

static void get_pixel(int* pDst, const uint8 *pSrc, int luma_only, int num_comps)
{
   int r, g, b;
   if (num_comps == 1)
   {
      r = g = b = pSrc[0];
   }
   else if (luma_only)
   {
      const int YR = 19595, YG = 38470, YB = 7471;
      r = g = b = (pSrc[0] * YR + pSrc[1] * YG + pSrc[2] * YB + 32768) / 65536;
   }
   else
   {
      r = pSrc[0]; g = pSrc[1]; b = pSrc[2];
   }
   pDst[0] = r; pDst[1] = g; pDst[2] = b;
}

//------------------------------------------------------------------------------
// Decode JPEG image from memory and render on LCD. Returns NULL on failure.
// If reduce is non-zero, the image will be more quickly decoded at approximately
// 1/8 resolution (the actual returned resolution will depend on the JPEG 
// subsampling factor).
uint8 *pjpeg_load_from_memory(const char *pInput, int *x, int *y, int *comps, pjpeg_scan_type_t *pScan_type, int reduce)
{
   pjpeg_image_info_t image_info;
   int mcu_x = 0;
   int mcu_y = 0;
   uint row_pitch;
   uint8 *pImage;
   uint8 status;
   uint decoded_width, decoded_height;
   uint row_blocks_per_mcu, col_blocks_per_mcu;

   *x = 0;
   *y = 0;
   *comps = 0;
   if (pScan_type) *pScan_type = PJPG_GRAYSCALE;

	 g_pInData = (char *)pInput;
   g_nInFileSize = JPEG_image_size;
      
   status = pjpeg_decode_init(&image_info, pjpeg_need_bytes_callback, NULL, (unsigned char)reduce);
         
   if (status)   {
      printf("pjpeg_decode_init() failed with status %u\n", status);
      if (status == PJPG_UNSUPPORTED_MODE) {
         printf("Progressive JPEG files are not supported.\n");
      }
      // fclose(g_pInFile);
      return NULL;
   }
   
   if (pScan_type)
      *pScan_type = image_info.m_scanType;

   // In reduce mode output 1 pixel per 8x8 block.
//   decoded_width = reduce ? (image_info.m_MCUSPerRow * image_info.m_MCUWidth) / 8 : image_info.m_width;
//   decoded_height = reduce ? (image_info.m_MCUSPerCol * image_info.m_MCUHeight) / 8 : image_info.m_height;
   decoded_width = reduce ? ((image_info.m_MCUSPerRow * image_info.m_MCUWidth) >> 3) : image_info.m_width; //SALONI
   decoded_height = reduce ? ((image_info.m_MCUSPerCol * image_info.m_MCUHeight) >> 3) : image_info.m_height;

   row_pitch = decoded_width * image_info.m_comps;
	 //   pImage = (uint8 *)malloc(row_pitch * decoded_height);
   pImage = (uint8 *)malloc(row_pitch);  // Do one row at a time, reset at end of line
	 if (!pImage)   {
      return NULL;
   }

   row_blocks_per_mcu = image_info.m_MCUWidth >> 3;
   col_blocks_per_mcu = image_info.m_MCUHeight >> 3;
   
   for ( ; ; )   {
      int y, x;
      uint8 *pDst_row; //

      status = pjpeg_decode_mcu();    
      if (status) {
         if (status != PJPG_NO_MORE_BLOCKS) {
            printf("pjpeg_decode_mcu() failed with status %u\n", status);
            free(pImage);
            return NULL;
         }
         break;
      }

      if (mcu_y >= image_info.m_MCUSPerCol) {
         free(pImage);
         return NULL;
      }

      if (reduce) {
         // In reduce mode, only the first pixel of each 8x8 block is valid.
         pDst_row = pImage + (mcu_y * col_blocks_per_mcu * row_pitch) + (mcu_x * row_blocks_per_mcu * image_info.m_comps);
         if (image_info.m_scanType == PJPG_GRAYSCALE) {
            *pDst_row = image_info.m_pMCUBufR[0];
         } else {
            uint y, x;
            for (y = 0; y < col_blocks_per_mcu; y++) {
               uint src_ofs = (y * 128U);
               for (x = 0; x < row_blocks_per_mcu; x++) {
                  pDst_row[0] = image_info.m_pMCUBufR[src_ofs];
                  pDst_row[1] = image_info.m_pMCUBufG[src_ofs];
                  pDst_row[2] = image_info.m_pMCUBufB[src_ofs];
                  pDst_row += 3;
                  src_ofs += 64;
               }
               pDst_row += row_pitch - 3 * row_blocks_per_mcu;
            }
         }
      } else {
				// Copy MCU's pixel blocks into the destination bitmap.
				PT_T pos;
 			  COLOR_T color;        

				// pDst_row = pImage + (mcu_y * image_info.m_MCUHeight) * row_pitch + (mcu_x * image_info.m_MCUWidth * image_info.m_comps);
				pDst_row = pImage + (mcu_x * image_info.m_MCUWidth * image_info.m_comps);
				for (y = 0; y < image_info.m_MCUHeight; y += 8) {
           const int by_limit = min(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));
           for (x = 0; x < image_info.m_MCUWidth; x += 8) {
               uint8 *pDst_block = pDst_row + x * image_info.m_comps;

               // Compute source byte offset of the block in the decoder's MCU buffer.
               uint src_ofs = (x * 8U) + (y * 16U);
               const uint8 *pSrcR = image_info.m_pMCUBufR + src_ofs;
               const uint8 *pSrcG = image_info.m_pMCUBufG + src_ofs;
               const uint8 *pSrcB = image_info.m_pMCUBufB + src_ofs;
								//const int bx_limit=8;
               const int bx_limit = min(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

               if (image_info.m_scanType == PJPG_GRAYSCALE) {
                  int bx, by;
								  pos.Y = y + mcu_y * image_info.m_MCUHeight;
                  for (by = 0; by < by_limit; by++) {
                     uint8 *pDst = pDst_block;
										 pos.X = x + mcu_x * image_info.m_MCUWidth;
                     for (bx = 0; bx < bx_limit; bx++) {
												color.R = color.G = color.B = *pSrcR;
											  LCD_Plot_Pixel(&pos, &color);								 
											  pos.X++;											 
                        *pDst++ = *pSrcR++;
										 }
                     pSrcR += (8 - bx_limit);
                     pDst_block += row_pitch;
										 pos.Y++;											 
                  }
               } else {
//                  int bx, by;
								  pos.Y = y + (mcu_y * image_info.m_MCUHeight);
								 pos.X = x + (mcu_x * image_info.m_MCUWidth);
								 PT_T b;
								 //a.Y=pos.Y;
								 //a.X=pos.X;
								 b.Y=pos.Y+by_limit;
								 b.X=pos.X+bx_limit-1;
								 LCD_Start_Rectangle(&pos,&b);
								 
//                  for (by = 0; by < by_limit; by++) {
//                     uint8 *pDst = pDst_block;
//										 pos.X = x + mcu_x * image_info.m_MCUWidth;
//                     for (bx = 0; bx < bx_limit; bx++) {
///*										  pDst[0] = *pSrcR++;
//                        pDst[1] = *pSrcG++;
//                        pDst[2] = *pSrcB++;   */
											  //color.R = *pSrcR++;
                        //color.G = *pSrcG++;
                        //color.B = *pSrcB++;
//											  LCD_Plot_Pixel(&pos, &color);		
//LCD_Write_Rectangle_Pixel_mycode((uint32_t *)pSrcR,(uint32_t *)pSrcG, (uint32_t *)pSrcB, count-1);
LCD_Write_Rectangle_Pixel_mycode((uint32_t *)pSrcR,(uint32_t *)pSrcG, (uint32_t *)pSrcB, ((bx_limit*by_limit)>>2));
//											  pos.X++;											 
//                        pDst += 3;
//                     }
                     //pSrcR += (8 - bx_limit);
                     //pSrcG += (8 - bx_limit);
                     //pSrcB += (8 - bx_limit);
                     //pDst_block += row_pitch;
										 //pos.Y++;
                  //}
               }
            }
           // pDst_row += (row_pitch * 8);						
         }
      }
      mcu_x++;
      if (mcu_x == image_info.m_MCUSPerRow) {
         mcu_x = 0;
         mcu_y++;
      }
   }

   *x = decoded_width;
   *y = decoded_height;
   *comps = image_info.m_comps;

   return pImage;
}

//------------------------------------------------------------------------------

#if 0
// Compute image error metrics.
static void image_compare(image_compare_results *pResults, int width, int height, const uint8 *pComp_image, int comp_image_comps, const uint8 *pUncomp_image_data, int uncomp_comps, int luma_only)
{
   double hist[256];
   double sum = 0.0f, sum2 = 0.0f;
   double total_values;
   const uint first_channel = 0, num_channels = 3;
   int x, y;
   uint i;

   memset(hist, 0, sizeof(hist));
   
   for (y = 0; y < height; y++)
   {
      for (x = 0; x < width; x++)
      {
         uint c;
         int a[3]; 
         int b[3]; 

         get_pixel(a, pComp_image + (y * width + x) * comp_image_comps, luma_only, comp_image_comps);
         get_pixel(b, pUncomp_image_data + (y * width + x) * uncomp_comps, luma_only, uncomp_comps);

         for (c = 0; c < num_channels; c++)
            hist[labs(a[first_channel + c] - b[first_channel + c])]++;
      }
   }

   pResults->max_err = 0;
   
   for (i = 0; i < 256; i++)
   {
      double x;
      if (!hist[i])
         continue;
      if (i > pResults->max_err)
         pResults->max_err = i;
      x = i * hist[i];
      sum += x;
      sum2 += i * x;
   }

   // See http://bmrc.berkeley.edu/courseware/cs294/fall97/assignment/psnr.html
   total_values = width * height;

   pResults->mean = sum / total_values;
   pResults->mean_squared = sum2 / total_values;

   pResults->root_mean_squared = sqrt(pResults->mean_squared);

   if (!pResults->root_mean_squared)
      pResults->peak_snr = 1e+10f;
   else
      pResults->peak_snr = log10(255.0f / pResults->root_mean_squared) * 20.0f;
}
#endif

//------------------------------------------------------------------------------
uint32_t LCD_JPEG(void)
{
  int width, height, comps;
  pjpeg_scan_type_t scan_type;
//  const char* p = "?";
  uint8 *pImage;
  int reduce = 0;
    
	pixel_data_hash = 0;
  pImage = pjpeg_load_from_memory(JPEG_image, &width, &height, &comps, &scan_type, reduce);

	return pixel_data_hash; //  != 
}
//------------------------------------------------------------------------------

