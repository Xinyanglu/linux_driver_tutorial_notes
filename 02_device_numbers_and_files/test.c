#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h> 
#include <fcntl.h>

int main(){
	int dev = open("/dev/my_device", O_RDONLY);
	if (dev == -1) {
		printf("Opening device failed\n");
		return -1;
	}
	printf("Opening device successful\n");
	close(dev);
	return 0;
}
