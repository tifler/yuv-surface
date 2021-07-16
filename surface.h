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
	void (*setPixel)(struct Surface *s, int x, int y, unsigned int color);
	unsigned int (*getPixel)(struct Surface *s, int x, int y);
	void (*drawHLine)(struct Surface *,
			int x, int y, unsigned int w, unsigned int color);
	void (*drawVLine)(struct Surface *,
			int x, int y, unsigned int h, unsigned int color);
} Surface_t;

/*****************************************************************************/

struct Surface *createSurface(int width, int stride, int height, int bpp);
void destroySurface(struct Surface *surface);
void renderSurface_YUV420File(struct Surface *s, int fd);

unsigned int getPixel(struct Surface *s, int x, int y);
void setPixel(struct Surface *s, int x, int y, unsigned int color);
void drawLine(struct Surface *s,
		int x1, int y1, int x2, int y2, unsigned int color);
void fillRect(struct Surface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color);
void drawRect(struct Surface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color);

#endif	/*__SURFACE_H__*/
