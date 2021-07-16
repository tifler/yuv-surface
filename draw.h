/**
 * @file drawing library header
 *
 * History
 * Date			Author		Desciption
 * ----------------------------------------------------------------------------
 * 2021.07.16	tifler		Created.
 */

#ifndef	__DRAW_H__
#define	__DRAW_H__

#include "surface.h"
#include "gr-types.h"
#include "XDebug.h"

/******************************************************************************
 * BPP1
 */

void setPixel_BPP1(struct Surface *s, int x, int y, unsigned int color);
unsigned int getPixel_BPP1(struct Surface *s, int x, int y);
void drawHLine_BPP1(struct Surface *s,
		int x, int y, unsigned int width, unsigned int color);
void drawVLine_BPP1(struct Surface *s,
		int x, int y, unsigned int height, unsigned int color);

/******************************************************************************
 * BPP8
 */

void setPixel_BPP8(struct Surface *s, int x, int y, unsigned int color);
unsigned int getPixel_BPP8(struct Surface *s, int x, int y);
void drawHLine_BPP8(struct Surface *s,
		int x, int y, unsigned int width, unsigned int color);
void drawVLine_BPP8(struct Surface *s,
		int x, int y, unsigned int height, unsigned int color);

/*****************************************************************************/

#endif	/*__DRAW_H__*/
