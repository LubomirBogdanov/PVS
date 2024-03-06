#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int i;
	int fd;
	
	fd = open("/dev/firstmod", O_RDWR);

	if ( fd == -1 ) {
		perror("open failed");
		exit(-1);
	}

	printf("open: successful\n");

	for(i = 0; i < 10; i++){
		write(fd, "1", 1);
		usleep(100000);
		write(fd, "0", 1);
		usleep(100000);
	}

	close(fd);

	return 0;
}
