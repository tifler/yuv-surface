#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "surface.h"
#include "draw.h"
#include "XDebug.h"

static void draw(struct Surface *s)
{
	int sx, sy, dx, dy;

#if	0
	sx = 100;
	sy = 0;
	dx = 102;
	dy = 1079;
	drawLine(s, sx, sy, dx, dy, 0xf0);
#endif	/*0*/

#if	0
	sx = 0;
	sy = 1079;
	dx = 2;
	dy = 0;
	drawLine(s, sx, sy, dx, dy, 0xf0);
#endif	/*0*/

#if	1
	sx = 1079;
	sy = 0;
	dx = 0;
	dy = 1079;
	drawLine(s, sx, sy, dx, dy, 0xf0);
#endif	/*0*/

#if	0
	sx = 1919;
	sy = 100;
	dx = 0;
	dy = 102;
	drawLine(s, sx, sy, dx, dy, 0xf0);
#endif	/*0*/

#if	1
	sx = 0;
	sy = 0;
	dy = 1079;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine(s, sx, sy, dx, dy, 0xf0);

	sx = 0;
	sy = 1079;
	dy = 0;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine(s, sx, sy, dx, dy, 0xf0);

	sx = 1919;
	sy = 0;
	dy = 1079;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine(s, sx, sy, dx, dy, 0xf0);

	sx = 1919;
	sy = 1079;
	dy = 0;
	for (int dx = 0; dx < 1920; dx += 8)
		drawLine(s, sx, sy, dx, dy, 0xf0);
#endif	/*0*/
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
	struct Surface *s = createSurface(1920, 1920, 1080, 1);
	ASSERT(s);

	draw(s);
	dumpToFile(s, "1920x1080.yuv");
	system("ffmpeg -y -s 1920x1080 -i 1920x1080.yuv 1920x1080.bmp");

	return 0;
}
