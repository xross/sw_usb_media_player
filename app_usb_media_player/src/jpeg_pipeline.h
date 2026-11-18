#ifndef JPEG_PIPELINE_H_
#define JPEG_PIPELINE_H_

#include "pipeline.h"
/* Maximum compressed JPEG frame, not a USB sector/transfer size. This remains
 * a compile-time bound for the two static read-ahead buffers. */
enum { JPEG_MAX_FRAME_BYTES = 30 * 1024 };
#define JPEG_INPUT_WIDTH 320
#define JPEG_INPUT_HEIGHT 240
#define JPEG_DECODE_SCALE 0
#define JPEG_OUTPUT_WIDTH (JPEG_INPUT_WIDTH >> JPEG_DECODE_SCALE)
#define JPEG_OUTPUT_HEIGHT (JPEG_INPUT_HEIGHT >> JPEG_DECODE_SCALE)
#define PIPELINE_RGB_BYTES (JPEG_OUTPUT_WIDTH * JPEG_OUTPUT_HEIGHT * 3)

#if MSC_JPEG_PIPELINE
void jpeg_pipeline_decode(chanend c_load, chanend c_output);
void jpeg_pipeline_transmit(chanend c_decode, chanend c_image);
void jpeg_pipeline_receive(chanend c_image, chanend c_render);
unsigned jpeg_pipeline_check_lcd(unsigned slot);

#ifdef __XC__
unsigned char * unsafe jpeg_pipeline_lcd_buffer(unsigned slot);
#else
unsigned char *jpeg_pipeline_lcd_buffer(unsigned slot);
unsigned char *jpeg_pipeline_input_buffer(unsigned slot);
#endif
#endif

#endif
