#include "display904.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "font.h"
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include "touchpanel904.h"

cursor crsr;
int lcd;
long int location = 0;
int fbfd = 0;
char *fbp = 0;
long int screensize = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
stLcdIoctlRegion region;

int main() {
	init_display904();
	color_t green = COLOR16_GREEN;
	clear_vram_display904();
	draw_line(0, 0, 100, 200, green);
	commit_display904();
	cleanup_display904();
	return 0;
}

void commit_display904() {
	ioctl(lcd, LCD_IOC_REFRESH, &region);
}

void clear_vram_display904() {
	memset(fbp, 0, DISPLAY_HEIGHT * DISPLAY_WIDTH * 2);
}

void clear_display904() {
	clear_vram_display904();
	commit_display904();
}

void init_display904() {
	region.tl.x = 0;
	region.tl.y = 0;
	region.br.x = 319;
	region.br.y = 239;

    // Open the file for reading and writing
    lcd = open("/dev/fb0", O_RDWR);
    if (lcd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(lcd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(lcd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

}

void cleanup_display904() {
    munmap(fbp, screensize);
    close(lcd);
}

void draw_line(int x0, int y0, int x1, int y1, color_t color) {
	int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; 

	for(;;) {  
		DISPLAY(x0, y0, color);
		if(x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if(e2 > dy) {
			err += dy; x0 += sx; 
		}
		if(e2 < dx) {
			err += dx; y0 += sy; 
		}
	}
}

void draw_rect(int x, int y, int width, int height, color_t color) {
	int i = 0, j = 0;

	for(i = 0; i < width; i++) {
		//top line
		DISPLAY(x + i, y, color);
		//bottom line
		DISPLAY(x + i, y + height - 1, color);
	}

	for(j = 0; j < height; j++) {
		//left line
		DISPLAY(x, y + j, color);
		//right line
		DISPLAY(x + width - 1, y + j, color);
	}
}

void printdsp(char *string, color_t color) {
	
	int k = 0, l = 0;
	for(l = 0; l < 4; l++) {
		for(k = 0; k < 30; k++) {
			const char *test = font8x8 + (k * 8) + (l * 30 * 8);
			int i, j = 0;
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 8; j++) {
					if(*(test + i) & (1 << j)) {
						DISPLAY(crsr.x + j, crsr.y + i, color);
					}
				}
			}
			crsr.x += 8;
		}
		crsr.y += 9;
		crsr.x = 0;
	}
}


