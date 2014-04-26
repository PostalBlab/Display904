#ifndef DISPLAY904_H_
#define DISPLAY904_H_

#include "lcd_904.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240



typedef struct {
	int x;
	int y;
} cursor;

#define DISPLAY(x, y, z) {\
	if((x < DISPLAY_WIDTH - 1) && y < (DISPLAY_HEIGHT - 1)) {\
		location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * vinfo.xres * 2; \
		*((unsigned short int*)(fbp + (location))) = z;\
	}\
}

void clear_vram_display904();
void clear_display904();
void commit_display904();
void init_display904();
void draw_rect_display904(int x, int y, int width, int height, color_t color);
void print_display904(char *string, color_t color);
void draw_line_display904(int x0, int y0, int x1, int y1, color_t color);
void cleanup_display904();

#endif 
