CC		:= gcc
CFLAGS	:= -Wall -g
CFLAGS	+= -DXDEBUG

OBJS	+= main.o surface.o
TARGET	:= surface

.PHONY: clean tags

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET) tags

tags:
	ctags -R .
