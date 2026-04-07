#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef __linux__
#include <sys/sysmacros.h>
#endif
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "file_type.h"

int main(int argc, char *argv[]) {
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Ошибка: неверное количество аргументов\n");
		return 1;
	}

	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		return 1;
	}

	printf("  File: %s\n", argv[1]);

	printf("  Size: %-10ld Blocks: %-10lld IO Block: %-6d %s\n",
			(long)sb.st_size,
			(long long)sb.st_blocks,
			(int)sb.st_blksize,
			get_file_type(sb.st_mode));

	printf("Device: %ld/%-3ld Inode: %-10ld Links: %-5d\n",
			(long)major(sb.st_dev),
			(long)minor(sb.st_dev),
			(long)sb.st_ino,
			(int)sb.st_nlink);

	printf("Access: (%04o/%-10s)  Uid: (%5d/%-8s)   Gid: (%5d/%-8s)\n",
			(unsigned int)(sb.st_mode & 0777),
			"rwxrwxrwx",
			(int)sb.st_uid,
			getpwuid(sb.st_uid) ? getpwuid(sb.st_uid)->pw_name : "UNKNOWN",
			(int)sb.st_gid,
			getgrgid(sb.st_gid) ? getgrgid(sb.st_gid)->gr_name : "UNKNOWN");

	printf("Access: %s", ctime(&sb.st_atime));
	printf("Modify: %s", ctime(&sb.st_mtime));
	printf("Change: %s", ctime(&sb.st_ctime));

#ifdef __APPLE__
	printf("Birth: %s", ctime(&sb.st_birthtime));
#elif defined(__linux__)
	printf("Birth: -\n");
#endif

	return 0;
}
