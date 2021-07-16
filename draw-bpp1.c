#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "surface.h"
#include "XDebug.h"

/*****************************************************************************/

void setPixel_BPP1(struct Surface *s, int x, int y, unsigned int color)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	unsigned char mask = 1 << (7 - (x & 7));
	if (color)
		bitmap[s->bytes_per_line * y + x / 8] |= mask;
	else
		bitmap[s->bytes_per_line * y + x / 8] &= ~mask;
}

unsigned int getPixel_BPP1(struct Surface *s, int x, int y)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	unsigned char mask = 1 << (7 - (x & 7));
	return bitmap[s->bytes_per_line * y + x / 8] & mask;
}

void drawHLine_BPP1(
		struct Surface *s, int x, int y, int width, unsigned int color)
{
	//DBG("HLine (%d, %d, w=%d)\n", x, y, width);

	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	if (width == 0)
		return;
	else if (width > 0) {
		for (int i = 0; i < width && (x + i) < s->width; i++) {
			setPixel_BPP1(s, x + i, y, color);
		}
	} else {
		for (int i = 0; i > width && (x + i) >= 0; i--) {
			setPixel_BPP1(s, x + i, y, color);
		}
	}
}

void drawVLine_BPP1(
		struct Surface *s, int x, int y, int height, unsigned int color)
{
	//DBG("VLine (%d, %d, h=%d)\n", x, y, height);

	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	if (height == 0)
		return;

	if (height > 0) {
		for (int i = 0; i < height && (y + i) < s->height; i++) {
			setPixel_BPP1(s, x, y + i, color);
		}
	} else {
		for (int i = 0; i > height && (y + i) >= 0; i--) {
			setPixel_BPP1(s, x, y + i, color);
		}
	}
}

void drawLine_BPP1(
		struct Surface *s, int x1, int y1, int x2, int y2, unsigned int color)
{
	if (x1 == x2)
		return drawVLine_BPP1(s, x1, MIN(y1, y2), abs(y1 - y2) + 1, color);

	if (y1 == y2)
		return drawHLine_BPP1(s, MIN(x1, x2), y1, abs(x1 - x2) + 1, color);

	// y = ax + b
	int plus1x = x2 > x1 ? 1 : -1;
	int plus1y = y2 > y1 ? 1 : -1;
	double a = (double)(y2 - y1 + plus1y) / (double)(x2 - x1 + plus1x);
	double b = y1 - a * x1;

	DBG("y = %f * x + %f\n", a, b);
	if (a > 1.0F || a < -1.0F) {
		// process horizontally
		int step = (x2 > x1) ? 1 : -1;

		double y0, y;
		y0 = y1;
		for (int x = x1 + step; x != (x2 + step); x += step) {
			y = (a * x) + b;
			drawVLine_BPP1(s, x - step, (int)y0, (int)y - (int)y0, color);
			y0 = y;
		}
		y = y2;
		if ((int)y >= (int)y0)
			drawVLine_BPP1(s, (int)x2, y0, (int)y - (int)y0 + 1, color);
		else
			drawVLine_BPP1(s, (int)x2, y0, (int)y - (int)y0 - 1, color);
	} else {
		// prcess vertically
		int step = (y2 > y1) ? 1 : -1;

		double x0, x;
		x0 = x1;
		for (int y = y1 + step; y != (y2 + step); y += step) {
			x = (y - b) / a;
			drawHLine_BPP1(s, (int)x0, y - step, (int)x - (int)x0, color);
			x0 = x;
		}
		x = x2;
		if ((int)x >= (int)x0)
			drawHLine_BPP1(s, (int)x0, y2, (int)x - (int)x0 + 1, color);
		else
			drawHLine_BPP1(s, (int)x0, y2, (int)x - (int)x0 - 1, color);
	}
}

void fillRect_BPP1(
		struct Surface *s, int x, int y, int width, int height, unsigned int color)
{
	for (int i = 0; i < height && (y + i) < s->height; i++) {
		drawHLine_BPP1(s, x, y + i, width, color);
	}
}

void drawRect_BPP1(struct Surface *s, int x, int y, int width, int height, unsigned int color)
{
	drawHLine_BPP1(s, x, y, width, color);
	drawVLine_BPP1(s, x, y, height, color);
	drawHLine_BPP1(s, x, y + height - 1, width, color);
	drawVLine_BPP1(s, x + width - 1, y, height, color);
}

