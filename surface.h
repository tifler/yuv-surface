/**
 * @file 폰트를 렌더링하기 위한 surface
 *
 * History
 * Date			Author		Desciption
 * ----------------------------------------------------------------------------
 * 2013.01.12	tifler		Created.
 */

#ifndef	__SURFACE_H__
#define	__SURFACE_H__

#include "gr-types.h"
#include "util.h"

/*****************************************************************************/

typedef struct Surface {
	unsigned int width;
	unsigned int stride;
	unsigned int height;
	unsigned int bpp;
	unsigned int bytes_per_line;
	void *data;
} Surface_t;

/*****************************************************************************/

static inline void setPixel_BPP8(
		struct Surface *s, int x, int y, unsigned char color)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	bitmap[s->stride * y + x] = color;
}

static inline unsigned char getPixel_BPP8(struct Surface *s, int x, int y)
{
	unsigned char *bitmap = (unsigned char *)s->data;
	return bitmap[s->stride * y + x];
}

/*****************************************************************************/

struct Surface *createSurface(int width, int stride, int height, int bpp);
void destroySurface(struct Surface *surface);
void renderSurface_YUV420File(struct Surface *s, int fd);

#endif	/*__SURFACE_H__*/
