/*
 * images.h
 *
 *  Created on: 2025Äê1ÔÂ16ÈÕ
 *      Author: yydy
 */

#ifndef CODE_IMAGES_H_
#define CODE_IMAGES_H_
#include "zf_common_headfile.h"
#include "headfile_code.h"

extern uint8  mt9v03x_image_Band[MT9V03X_H][MT9V03X_W];
void Set_image_towvalues(uint8 value);
uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height);
uint8 otsuThreshold_fast(uint8 *image);
void Find_Mid_Line(uint8 mt9v03x_image_BandW[MT9V03X_H][MT9V03X_W]);
void draw_points(uint8 x,uint8 y,uint16 color);


#endif /* CODE_IMAGES_H_ */
