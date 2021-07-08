#ifndef	__GR_TYPES_H__
#define	__GR_TYPES_H__

#define	IS_EQUAL_RECT(r1, r2)	\
	((r1)->x == (r2)->x && (r1)->y == (r2)->y && \
	 (r1)->width == (r2)->width && (r1)->height == (r2)->height)

#define	ARGB1555(a,r,g,b) \
		( \
		(a ? 0x8000 : 0) | \
		(((r >> 3) & 0x1f) << 10) | \
		(((g >> 3) & 0x1f) << 5) | \
		((b >> 3) & 0x1f) \
		)

#define	ARGB8888(a,r,g,b)	\
	( \
	 ((a & 0xff) << 24) | \
	 ((r & 0xff) << 16) | \
	 ((g & 0xff) << 8) | \
	 ((b & 0xff)) \
	)

typedef struct __POINT {
	int x;
	int y;
} POINT;

typedef struct __SIZE {
	unsigned int width;
	unsigned int height;
} SIZE;

typedef struct __RECT {
	int x;
	int y;
	unsigned int width;
	unsigned int height;
} RECT;

typedef struct __COLOR {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} COLOR;

#endif	/*__GR_TYPES_H__*/
