#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

static int kbd_fd = -1;

int open_touchpanel() {
	if((kbd_fd = open("/dev/tp904", O_RDONLY)) < 0) {
		return -1;
	}
	return kbd_fd;

}

void close_touchpanel() {
	if(kbd_fd)
		close(kbd_fd);
}

char old[200];
int read_keypress() {
	char key[200];
	int status = read(kbd_fd, key, 200);

	if(memcmp(key, old, 200)) {
		printf("asd\n");
	}

	memcpy(old, key, 200);
}
