#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "surface.h"
#include "XDebug.h"

static void drawHLine_BPP8(
		struct Surface *s, int x, int y, int width, unsigned char color)
{
	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(width > 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	//DBG("HLine (%d, %d, w=%d)\n", x, y, width);
	for (int i = 0; i < width && (x + i) < s->width; i++) {
		setPixel_BPP8(s, x + i, y, color);
	}
}

static void drawVLine_BPP8(
		struct Surface *s, int x, int y, int height, unsigned char color)
{
	ASSERT(s);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(height > 0);
	ASSERT(x < s->width);
	ASSERT(y < s->height);

	//DBG("VLine (%d, %d, h=%d)\n", x, y, height);
	for (int i = 0; i < height && (y + i) < s->height; i++) {
		setPixel_BPP8(s, x, y + i, color);
	}
}

static void drawLine_BPP8(
		struct Surface *s, int x1, int y1, int x2, int y2, unsigned char color)
{
	if (x1 == x2)
		return drawVLine_BPP8(s, x1, MIN(y1, y2), abs(y1 - y2) + 1, color);

	if (y1 == y2)
		return drawHLine_BPP8(s, MIN(x1, x2), y1, abs(x1 - x2) + 1, color);

	// y = ax + b
	double a = (double)(y2 - y1) / (double)(x2 - x1);
	double b = y1 - a * x1;

	if (a > 1.0F || a < -1.0F) {
		// process horizontally
		int step = (x2 > x1) ? 1 : -1;

		double y0, y;
		y0 = y1;
		for (int x = x1 + step; x != (x2 + step); x += step) {
			y = (a * x) + b;
			drawVLine_BPP8(s, x - step, (int)y0, abs((int)y - (int)y0), color);
			y0 = y;
		}
		y = y2;
		drawVLine_BPP8(s, (int)x2, y0, MAX(abs((int)y - (int)y0), 1), color);
	} else {
		// prcess vertically
		int step = (y2 > y1) ? 1 : -1;

		double x0, x;
		x0 = x1;
		for (int y = y1 + step; y != (y2 + step); y += step) {
			x = (y - b) / a;
			drawHLine_BPP8(s, (int)x0, y - step, abs((int)x - (int)x0), color);
			x0 = x;
		}
		x = x2;
		drawHLine_BPP8(s, (int)x0, y2, MAX(abs((int)x - (int)x0), 1), color);
	}
}

static void fillRect_BPP8(
		struct Surface *s, int x, int y, int width, int height, unsigned char color)
{
	for (int i = 0; i < height && (y + i) < s->height; i++) {
		drawHLine_BPP8(s, x, y + i, width, color);
	}
}

static void drawRect_BPP8(
		struct Surface *s, int x, int y, int width, int height, unsigned char color)
{
	drawHLine_BPP8(s, x, y, width, color);
	drawVLine_BPP8(s, x, y, height, color);
	drawHLine_BPP8(s, x, y + height - 1, width, color);
	drawVLine_BPP8(s, x + width - 1, y, height, color);
}

static void draw(struct Surface *s)
{
	//fillRect_BPP8(s, 100, 100, 1920 - 200, 1080 - 200, 0xf0);
	//drawRect_BPP8(s, 200, 200, 1920 - 400, 1080 - 400, 0x0);
	//drawLine_BPP8(s, 300, 300, 1300, 500, 0xf0);

	int sx, sy, dx, dy;

	sx = 0;
	sy = 0;
	dy = 1079;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine_BPP8(s, sx, sy, dx, dy, 0xf0);

	sx = 0;
	sy = 1079;
	dy = 0;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine_BPP8(s, sx, sy, dx, dy, 0xf0);

	sx = 1919;
	sy = 0;
	dy = 1079;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine_BPP8(s, sx, sy, dx, dy, 0xf0);

	sx = 1919;
	sy = 1079;
	dy = 0;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine_BPP8(s, sx, sy, dx, dy, 0xf0);
}

static void dumpToFile(struct Surface *s, const char *path)
{
	int fd;

	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	ASSERT(fd > 0);

	renderSurface_YUV420File(s, fd);

	close(fd);
}

int main(int argc, char **argv)
{
	struct Surface *s = createSurface(1920, 1920, 1080, 8);
	ASSERT(s);

	draw(s);
	dumpToFile(s, "1920x1080.yuv");
	system("ffmpeg -y -s 1920x1080 -i 1920x1080.yuv 1920x1080.bmp");

	return 0;
}
