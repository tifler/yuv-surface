#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "surface.h"
#include "XDebug.h"

/*****************************************************************************/

void setPixel_BPP8(struct Surface *s, int x, int y, unsigned int color)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	bitmap[s->stride * y + x] = color;
}

unsigned int getPixel_BPP8(struct Surface *s, int x, int y)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	return bitmap[s->stride * y + x];
}

void drawHLine_BPP8(struct Surface *s,
		int x, int y, unsigned int width, unsigned int color)
{
	DBG("HLine (%d, %d, w=%d)\n", x, y, width);

	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	if (width == 0)
		return;
	else if (width > 0) {
		for (int i = 0; i < width && (x + i) < s->width; i++) {
			setPixel_BPP8(s, x + i, y, color);
		}
	} else {
		for (int i = 0; i > width && (x + i) >= 0; i--) {
			setPixel_BPP8(s, x + i, y, color);
		}
	}
}

void drawVLine_BPP8(struct Surface *s,
		int x, int y, unsigned int height, unsigned int color)
{
	DBG("VLine (%d, %d, h=%d)\n", x, y, height);

	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	if (height == 0)
		return;

	if (height > 0) {
		for (int i = 0; i < height && (y + i) < s->height; i++) {
			setPixel_BPP8(s, x, y + i, color);
		}
	} else {
		for (int i = 0; i > height && (y + i) >= 0; i--) {
			setPixel_BPP8(s, x, y + i, color);
		}
	}
}

