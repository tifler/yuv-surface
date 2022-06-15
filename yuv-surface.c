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

#define	BITS_PER_BYTE						(8)

/*****************************************************************************/

typedef struct YUVSurface {
	uint32_t width;
	uint32_t stride;
	uint32_t height;
	uint32_t pixfmt;
	uint32_t bpp;
	uint32_t bytes_per_line;
	uint32_t channels;
	void *chanBuffer[4];

	/* mandatory */
	void (*setPixel)(struct YUVSurface *s, uint32_t x, uint32_t y, uint8_t *color);
	unsigned void (*getPixel)(struct YUVSurface *s, uint32_t x, uint32_t y, uint8_t *color);
	void (*drawHLine)(struct YUVSurface *,
			uint32_t x, uint32_t y, uint32_t w, uint8_t *color);
	void (*drawVLine)(struct YUVSurface *,
			uint32_t x, uint32_t y, uint32_t h, uint8_t *color);
	/* optional */
	void (*drawLine)(struct YUVSurface *,
			uint32_t x, uint32_t y, uint32_t h, uint8_t *color);
} YUVSurface_t;

/*****************************************************************************/

static int safeWrite(int fd, const void *buf, int size)
{
	int ret;
	int wrcnt;

	for (wrcnt = 0; size > wrcnt; ) {
		ret = write(fd, &((const char *)buf)[wrcnt], size - wrcnt);
		if (ret < 0) {
			if( errno == EINTR)
				continue;
			break;
		}
		wrcnt += ret;
	}

	return wrcnt == size ? 0 : -1;
}

static void mapDrawingFunc(struct YUVSurface *surface)
{
	switch (surface->bpp) {
		case 1:
			surface->setPixel = setPixel_BPP1;
			surface->getPixel = getPixel_BPP1;
			surface->drawHLine = drawHLine_BPP1;
			surface->drawVLine = drawVLine_BPP1;
			break;
		case 8:
			surface->setPixel = setPixel_BPP8;
			surface->getPixel = getPixel_BPP8;
			surface->drawHLine = drawHLine_BPP8;
			surface->drawVLine = drawVLine_BPP8;
			break;
		default:
			DIE("Not supported BPP.\n");
			break;
	}
}

/*****************************************************************************/

struct YUVSurface *createSurface(
		int width, int stride, int height, uint32_t pixfmt)
{
	struct YUVSurface *surface;

	ASSERT(width > 0);
	ASSERT(height > 0);
	ASSERT(bpp == 1 || bpp == 8);
	ASSERT(stride >= width);

	surface = (struct YUVSurface *)malloc(sizeof(*surface));
	ASSERT(surface);
	surface->width = width;
	surface->stride = stride;
	surface->height = height;
	surface->bpp = bpp;
	surface->bytes_per_line =
		ROUND_UP(stride * bpp, BITS_PER_BYTE) / BITS_PER_BYTE;
	surface->data = calloc(surface->height, surface->bytes_per_line);
	ASSERT(surface->data);

	mapDrawingFunc(surface);

	DBG("YUVSurface: width=%d, height=%d, stride=%d, bpp=%d, bytes_per_line=%d, %dbytes\n",
			surface->width,
			surface->height,
			surface->stride,
			surface->bpp,
			surface->bytes_per_line,
			surface->height * surface->bytes_per_line);

	return surface;
}

void destroySurface(struct YUVSurface *surface)
{
	ASSERT(surface);
	if(surface->data)
		free(surface->data);
	free(surface);
}

static void renderSurface_YUV420File_BPP8(struct YUVSurface *s, int fd)
{
	ASSERT(s);
	ASSERT(s->bpp == 8);

	unsigned char *ybuf = (unsigned char *)s->data;
	unsigned char *cbuf = (unsigned char *)calloc(1, s->width);
	ASSERT(cbuf);

	// y
	for (uint32_t y = 0; y < s->height; y++) {
		safeWrite(fd, &ybuf[s->stride * y], s->width);
	}

	// uv
	for (uint32_t y = 0; y < (s->height + 1) / 2; y++) {
		safeWrite(fd, cbuf, s->width);
	}

	free(cbuf);
}

static void renderSurface_YUV420File_BPP1(struct YUVSurface *s, int fd)
{
	ASSERT(s);
	ASSERT(s->bpp == 1);

	unsigned char *ybuf = (unsigned char *)calloc(1, s->width);
	ASSERT(ybuf);
	unsigned char *cbuf = (unsigned char *)calloc(1, s->width);
	ASSERT(cbuf);

	// y
	for (uint32_t y = 0; y < s->height; y++) {
		for (uint32_t x = 0; x < s->width; x++) {
			ybuf[x] = s->getPixel(s, x, y) ? 0xf0 : 0;
		}
		safeWrite(fd, ybuf, s->width);
	}

	// uv
	for (uint32_t y = 0; y < (s->height + 1) / 2; y++) {
		safeWrite(fd, cbuf, s->width);
	}

	free(cbuf);
}

void renderSurface_YUV420File(struct YUVSurface *s, int fd)
{
	if (s->bpp == 1)
		renderSurface_YUV420File_BPP1(s, fd);
	else if (s->bpp == 8)
		renderSurface_YUV420File_BPP8(s, fd);
	else {
		DIE("Unsupported surface");
	}
}

