// Automatically generated file. Do not edit if you plan to regenerate it.
#include "region.h"
const REGION_T RegionTable[] = {
	{0x00001b0d, 0x00001b36, "copyY"}, // 0
	{0x00001b49, 0x00002b4a, "decodeNextMCU"}, // 1
	{0x00002b79, 0x00002bf0, "getBits"}, // 2
	{0x00002bf5, 0x00002c44, "getChar"}, // 3
	{0x00002c51, 0x00002c78, "getOctet"}, // 4
	{0x00002c79, 0x00002d22, "huffDecode"}, // 5
	{0x00003641, 0x00003916, "processMarkers"}, // 6
	{0x0000394d, 0x00003962, "stuffChar"}, // 7
	{0x0000396d, 0x00003aae, "upsampleCb"}, // 8
	{0x00003abd, 0x00003b7e, "upsampleCbH"}, // 9
	{0x00003b8d, 0x00003c50, "upsampleCbV"}, // 10
	{0x00003c5d, 0x00003d9e, "upsampleCr"}, // 11
	{0x00003dad, 0x00003e6e, "upsampleCrH"}, // 12
	{0x00003e7d, 0x00003f40, "upsampleCrV"}, // 13
	{0x000001c9, 0x000001d4, "Reset_Handler"}, // 14
	{0x000001d5, 0x000001d6, "NMI_Handler"}, // 15
	{0x000001d7, 0x000001d8, "HardFault_Handler"}, // 16
	{0x000001d9, 0x000001da, "SVC_Handler"}, // 17
	{0x000001db, 0x000001dc, "PendSV_Handler"}, // 18
	{0x000001dd, 0x000001de, "SysTick_Handler"}, // 19
	{0x0000020d, 0x0000026e, "malloc"}, // 20
	{0x0000026f, 0x000002ba, "free"}, // 21
	{0x000003f5, 0x000003fc, "ferror"}, // 22
	{0x000003ff, 0x00000400, "UART0_IRQHandler"}, // 23
	{0x00000aaf, 0x00000abe, "exit"}, // 24
	{0x00000b69, 0x00000b9c, "Control_RGB_LEDs"}, // 25
	{0x00000ba5, 0x00000bb8, "Delay"}, // 26
	{0x00000bbd, 0x00000bc4, "Disable_Profiling"}, // 27
	{0x00000bc9, 0x00000c7a, "Display_Profile"}, // 28
	{0x00000cb5, 0x00000cbc, "Enable_Profiling"}, // 29
	{0x00000cc1, 0x00000d0e, "Init_Profiling"}, // 30
	{0x00000d21, 0x00000d72, "Init_RGB_LEDs"}, // 31
	{0x00000d89, 0x00000d92, "LCD_Erase"}, // 32
	{0x00000d99, 0x00000e68, "LCD_Fill_Buffer"}, // 33
	{0x00000e6d, 0x00000f92, "LCD_GPIO_Init"}, // 34
	{0x00000fa9, 0x0000105a, "LCD_Init"}, // 35
	{0x00001079, 0x0000109a, "LCD_JPEG"}, // 36
	{0x000010a5, 0x0000116e, "LCD_Plot_Pixel"}, // 37
	{0x00001175, 0x00001272, "LCD_Start_Rectangle"}, // 38
	{0x00001279, 0x000012a8, "LCD_TS_Blocking_Read"}, // 39
	{0x000012a9, 0x000012c0, "LCD_TS_Init"}, // 40
	{0x000012c9, 0x0000144c, "LCD_TS_Read"}, // 41
	{0x00001465, 0x0000148e, "LCD_Text_Init"}, // 42
	{0x000014a1, 0x0000163c, "LCD_Text_PrintChar"}, // 43
	{0x00001649, 0x0000168e, "LCD_Text_PrintStr"}, // 44
	{0x00001695, 0x000016b2, "LCD_Text_Set_Colors"}, // 45
	{0x000016bd, 0x00001700, "LCD_Write_Rectangle_Pixe"}, // 46
	{0x00001705, 0x000017c2, "LCD_Write_Rectangle_Pixe"}, // 47
	{0x00001881, 0x000018dc, "PWM_Init"}, // 48
	{0x000018ed, 0x0000194a, "Process_Profile_Sample"}, // 49
	{0x0000195d, 0x000019f4, "Sort_Profile_Regions"}, // 50
	{0x00001a05, 0x00001abc, "SystemInit"}, // 51
	{0x00001aed, 0x00001af8, "TPM0_IRQHandler"}, // 52
	{0x00002b61, 0x00002b72, "fputc"}, // 53
	{0x00002d2d, 0x00002d92, "main"}, // 54
	{0x00002db5, 0x0000326c, "pjpeg_decode_init"}, // 55
	{0x00003281, 0x000032b4, "pjpeg_decode_mcu"}, // 56
	{0x000032b9, 0x00003584, "pjpeg_load_from_memory"}, // 57
	{0x00003615, 0x0000363c, "pjpeg_need_bytes_callbac"}, // 58
}; 
const unsigned NumProfileRegions=59;
volatile unsigned RegionCount[59];
unsigned SortedRegions[59];
