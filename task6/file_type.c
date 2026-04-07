#include "file_type.h"
#include <sys/stat.h>

const char *get_file_type(mode_t mode) {
	if (S_ISREG(mode)) {
		return "regular file";
	}
	if (S_ISDIR(mode)) {
		return "directory";
	}
	if (S_ISCHR(mode)) {
		return "character device";
	}
	if (S_ISBLK(mode)) {
		return "block device";
	}
	if (S_ISFIFO(mode)) {
		return "fifo";
	}
	if (S_ISLNK(mode)) {
		return "symbolic link";
	}
	if (S_ISSOCK(mode)) {
		return "socket";
	}
	return "unknown";
}
