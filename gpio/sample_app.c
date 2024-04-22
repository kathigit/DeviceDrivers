#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<errno.h>
#include<string.h>

#define DEVICE_FILE "/dev/testD"
#define NUMBER 127

int main(int argc, char *argv[]) {
    int fd;
    char buffer[100] = "default_buffer";
    int cmd = 'B';
    fd = open(DEVICE_FILE,O_RDWR);
    if(fd == -1) {
        perror("Error in opening the device file:");
        return -1;
    }

    if(argc > 1) {
        if(ioctl(fd,'C',argv[1]) < 0) {
            perror("Error in ioctl:");
            return -1;
        }
        return 0;
    }

    return 0;
}

