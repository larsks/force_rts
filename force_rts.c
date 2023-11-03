#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fnmatch.h>
#include <string.h>
#include <errno.h>

// This is necessary to get the definition of RTLD_NEXT
#define _GNU_SOURCE
#include <dlfcn.h>

// This is necessary to get the definition of O_TMPFILE
#define __USE_GNU
#include <fcntl.h>

int _wrap_open( int (*real_open)(const char *, int, ...), const char *pathname, int flags, va_list args) {
    char *valid_patterns;
    int fd;

    if (flags & (O_CREAT | O_TMPFILE)) {
        mode_t mode = va_arg(args, mode_t);
        fd = real_open(pathname, flags, mode);
    } else {
        fd = real_open(pathname, flags);
    }

    valid_patterns = getenv("FORCE_RTS_DEVICES");

    if (fd >= 0  && valid_patterns != NULL) {
        char *pattern = strtok(valid_patterns, ":");
        int controlbits = TIOCM_RTS;
        while (pattern) {
            if (fnmatch(pattern, pathname, FNM_PATHNAME) == 0) {
                int ioctl_ret;
                fprintf(stderr, "*** setting rts low on %s\n", pathname);
                if (ioctl(fd, TIOCMBIC, &controlbits) == -1) {
                    fprintf(stderr, "*** setting rts failed: %s\n", strerror(errno));
                }
                break;
            }

            pattern = strtok(NULL, ":");
        }
    }

    return fd;
}

int open(const char *pathname, int flags, ...) {
    va_list ap;
    int fd;
    static int (*real_open)(const char *, int, ...) = NULL;

    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open");

    va_start(ap, flags);
    fd = _wrap_open(real_open, pathname, flags, ap);
    va_end(ap);

    return fd;
}

int open64(const char *pathname, int flags, ...) {
    va_list ap;
    int fd;
    static int (*real_open)(const char *, int, ...) = NULL;

    if (!real_open)
        real_open = dlsym(RTLD_NEXT, "open64");

    va_start(ap, flags);
    fd = _wrap_open(real_open, pathname, flags, ap);
    va_end(ap);

    return fd;
}
