/**
 * george's awesome ld_preload interposing /dev/fb0 ioctl dumper
 *
 * usage:
 * LD_PRELOAD=/path/to/libioctldumper.so /bin/ebrmain
 */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>

static int _fd = 0;

int open(const char *pathname, int flags, mode_t mode)
{
	static int (*func)(const char *, int, mode_t);

	if (!func) {
		printf("w00t.. we're hooking open!\n");
		func = (int (*)(const char *, int, mode_t)) dlsym(RTLD_NEXT, "open");
	}

	int fd = func(pathname, flags, mode);
	if (strcmp(pathname, "/dev/fb0") == 0) {
		printf("open(%s, %d, %d) is called.. starting to dump ioctls\n", pathname, flags, mode);
		_fd = fd;
	}

	return fd;
}

int close(int fd)
{
	static int (*func)(int);

	if (!func) {
		printf("w00t.. we're hooking close!\n");
		func = (int (*)(int)) dlsym(RTLD_NEXT, "close");
	}

	if (fd == _fd) {
		printf("close(%d) is called.. stopping ioctl dumps to /dev/fb0!\n", _fd);
		_fd = 0;
	}

	return func(fd);
}

int ioctl(int fd, int request, ...)
{
	static int (*func)(int fd, int request, ...);

	if (!func) {
		printf("w00t.. we're hooking ioctl!\n");
		func = (int (*)(int d, int request, ...)) dlsym(RTLD_NEXT, "ioctl");
	}

	va_list args;

	va_start(args, request);
	void *p = va_arg(args, void *);
	va_end(args);

	if (fd > 0 /*&& _fd == fd*/) {
		printf("ioctl(%d, 0x%x, %p)\n", fd, request, p);

		if (request >= 0x4539 && request <= 4559) {
			/* partial image update */

			/**
			 * struct ioctl_command {
			 *	u32 x;
			 * 	u32 y;
			 *	u32 width;
			 *	u32 height;
			 *	uchar buf[800 * 600];
			 * };
			 */
			unsigned int *x = (unsigned int *) p;
			printf("%u %u %u %u %u %u ", x[0], x[1], x[2], x[3], x[4], x[5]);

			/* dump the first 128 bytes of whatever was passed into this ioctl */

			unsigned char *y = (unsigned char *) (p + (sizeof(unsigned int) * 6));
			int i;
			for (i = 0; y != NULL && i < 512; i++) {
				printf("%d ", y[i]);
			}
			printf("\n");
		} else if (request == 0x4540) {
			/* status ??? */

			/**
			 * u32 my32 = ???;
			 * ioctl(/dev/fb0, 0x4540, &my_u32);
			 */
			unsigned int *x = (unsigned int *) p;
			printf("%u %u %u %u %u %u ", x[0], x[1], x[2], x[3], x[4], x[5]);
			printf("\n");
		}
	}

	int rc = func(fd, request, p);
	printf(" == %d\n", rc);

	return rc;
}
