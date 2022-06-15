/**
 * @file YUVSurface
 *
 * History
 * Date			Author		Desciption
 * ----------------------------------------------------------------------------
 * 2021.08.25	tifler		Created.
 */

#ifndef	__YUV_SURFACE_H__
#define	__YUV_SURFACE_H__

#include "gr-types.h"
#include "util.h"

/*****************************************************************************/

#define	FOURCC(a,b,c,d)	((uint32_t)(((d)<<24)|((c)<<16)|((b)<<8)|(a)))

/*****************************************************************************/

struct YUVSurface;

typedef enum {
	PIXFMT_YUV420_NV12 = FOURCC('N', 'V', '1', '2'),
};

/*****************************************************************************/

struct YUVSurface *createYUVSurface(
		uint32_t width, uint32_t stride, uint32_t height, pixfmt_t pixfmt);
struct YUVSurface *createYUVSurfaceFromBuffer(
		uint32_t width, uint32_t stride, uint32_t height, pixfmt_t pixfmt,
		uint8_t *buffers, int bufferCount);
void destroyYUVSurface(struct YUVSurface *surface);
void renderSurface_YUV420File(struct YUVSurface *s, int fd);

unsigned int getPixel(struct YUVSurface *s, int x, int y);
void setPixel(struct YUVSurface *s, int x, int y, unsigned int color);
void drawLine(struct YUVSurface *s,
		int x1, int y1, int x2, int y2, unsigned int color);
void fillRect(struct YUVSurface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color);
void drawRect(struct YUVSurface *s, int x, int y,
		unsigned int width, unsigned int height, unsigned int color);

#endif	/*__SURFACE_H__*/
