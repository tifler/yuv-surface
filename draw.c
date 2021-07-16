/**
 * @file 폰트를 렌더링하기 위한 surface
 *
 * History
 * Date			Author		Desciption
 * ----------------------------------------------------------------------------
 * 2013.01.12	tifler		Created.
 */

/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "surface.h"
#include "draw.h"
#include "XDebug.h"

/*****************************************************************************/

void drawLine(struct Surface *s,
		int x1, int y1, int x2, int y2, unsigned int color)
{
	if (x1 == x2)
		return s->drawVLine(s, x1, MIN(y1, y2), abs(y1 - y2) + 1, color);

	if (y1 == y2)
		return s->drawHLine(s, MIN(x1, x2), y1, abs(x1 - x2) + 1, color);

	// y = ax + b
	int plus1x = x2 > x1 ? 1 : -1;
	int plus1y = y2 > y1 ? 1 : -1;
	double a = (double)(y2 - y1 + plus1y) / (double)(x2 - x1 + plus1x);
	double b = y1 - a * x1;

	//DBG("y = %f * x + %f\n", a, b);
	if (a > 1.0F || a < -1.0F) {
		// process horizontally
		int step = (x2 > x1) ? 1 : -1;

		double y0, y;
		y0 = y1;
		for (int x = x1 + step; x != (x2 + step); x += step) {
			y = (a * x) + b;
			s->drawVLine(s, x - step, (int)y0, (int)y - (int)y0, color);
			y0 = y;
		}
		y = y2;
		if ((int)y >= (int)y0)
			s->drawVLine(s, (int)x2, y0, (int)y - (int)y0 + 1, color);
		else
			s->drawVLine(s, (int)x2, y0, (int)y - (int)y0 - 1, color);
	} else {
		// prcess vertically
		int step = (y2 > y1) ? 1 : -1;

		double x0, x;
		x0 = x1;
		for (int y = y1 + step; y != (y2 + step); y += step) {
			x = (y - b) / a;
			s->drawHLine(s, (int)x0, y - step, (int)x - (int)x0, color);
			x0 = x;
		}
		x = x2;
		if ((int)x >= (int)x0)
			s->drawHLine(s, (int)x0, y2, (int)x - (int)x0 + 1, color);
		else
			s->drawHLine(s, (int)x0, y2, (int)x - (int)x0 - 1, color);
	}
}

void fillRect(struct Surface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color)
{
	for (int i = 0; i < height && (y + i) < s->height; i++) {
		s->drawHLine(s, x, y + i, width, color);
	}
}

void drawRect(struct Surface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color)
{
	s->drawHLine(s, x, y, width, color);
	s->drawVLine(s, x, y, height, color);
	s->drawHLine(s, x, y + height - 1, width, color);
	s->drawVLine(s, x + width - 1, y, height, color);
}

unsigned int getPixel(struct Surface *s, int x, int y)
{
	return s->getPixel(s, x, y);
}

void setPixel(struct Surface *s, int x, int y, unsigned int color)
{
	s->setPixel(s, x, y, color);
}

