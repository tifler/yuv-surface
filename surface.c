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
#include "XDebug.h"

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


/*****************************************************************************/

struct Surface *createSurface(int width, int stride, int height, int bpp)
{
	struct Surface *surface;

	ASSERT(width > 0);
	ASSERT(height > 0);
	ASSERT(bpp > 0);
	ASSERT(bpp == 8);
	ASSERT(stride >= width);

	surface = (struct Surface *)malloc(sizeof(*surface));
	ASSERT(surface);
	surface->width = width;
	surface->stride = stride;
	surface->height = height;
	surface->bpp = bpp;
	surface->bytes_per_line = stride;
	surface->data = calloc(surface->height, surface->bytes_per_line);
	ASSERT(surface->data);

	DBG("Surface: width=%d, height=%d, stride=%d, bpp=%d, bytes_per_line=%d, %dbytes\n",
			surface->width,
			surface->height,
			surface->stride,
			surface->bpp,
			surface->bytes_per_line,
			surface->height * surface->bytes_per_line);

	return surface;
}

void destroySurface(struct Surface *surface)
{
	ASSERT(surface);
	if(surface->data)
		free(surface->data);
	free(surface);
}

void renderSurface_YUV420File(struct Surface *s, int fd)
{
	ASSERT(s);
	ASSERT(s->bpp == 8);

	unsigned char *ybuf = (unsigned char *)s->data;
	unsigned char *cbuf = (unsigned char *)calloc(1, s->width);
	ASSERT(cbuf);

	// y
	for (int y = 0; y < s->height; y++) {
		safeWrite(fd, &ybuf[s->stride * y], s->width);
	}

	// uv
	for (int y = 0; y < (s->height + 1) / 2; y++) {
		safeWrite(fd, cbuf, s->width);
	}

	free(cbuf);
}
