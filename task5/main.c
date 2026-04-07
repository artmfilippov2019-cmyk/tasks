#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PRINT_ERRNO_NAME(err) \
	do { \
		if (err == ENOENT) printf("ENOENT"); \
		else if (err == EACCES) printf("EACCES"); \
		else if (err == EINTR) printf("EINTR"); \
		else if (err == EISDIR) printf("EISDIR"); \
		else printf("UNKNOWN(%d)", err); \
	} while(0)

int main() {
	int fd = open("data.txt", O_RDONLY);

	if (fd == -1) {
		printf("Произошла ошибка при открытии файла\n\n");
		printf("Errno = ");
		PRINT_ERRNO_NAME(errno);
		printf("\n");
		printf("Сообщение из strerror: %s\n", strerror(errno));
		printf("Вывод через perror:\n   ");
		perror("open");
	} 
	else {
		printf("Файл успешно открыт!\n");
		close(fd);
	}

	return 0;
}
