#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "i2cdevs.h"
#include "exceptions.h"
#include "files.h"
#include <linux/i2c-dev.h>
#include <inttypes.h>

#include <sys/ioctl.h> // ioctl
#include <unistd.h> // read

#include <linux/i2c.h>


int get_ds3231(){
    int file;
    char *filename = "/dev/i2c-2";
    if ((file = open(filename, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        {FUNCTION_ERROR(-1, "Erro na abertura do bus i2c.")}
        exit(1);
    }
    int addr = 0x68;          // The I2C address of the ADC
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        {FUNCTION_ERROR(-1, "Failed to acquire bus access and/or talk to slave.")}
        //printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    return file;
}

int read_register(int busfd, __uint16_t reg, unsigned char *buf, int bufsize){
	unsigned char reg_buf[2];
	int ret;

	reg_buf[0] = (reg >> 0) & 0xFF;
	reg_buf[1] = (reg >> 8) & 0xFF;

	ret = write(busfd, reg_buf, 2);
	if (ret < 0) {
		printf("Failed to write [0x%02x 0x%02x] (reg: %04x).\n", reg_buf[0], reg_buf[1], reg);
		return ret;
	}

	printf("wrote %d bytes\n", ret);
	return read(busfd, buf, bufsize);
}
/*
uint32_t i2c_read_byte_reg(int file, uint8_t reg){
    char buf[10];
    buf[0] = reg;
    buf[1] = 0x43;
    buf[2] = 0x65;
    if (write(file, buf, 3) != 3) {
    }

    if (read(file, buf, 1) != 1) {
    } else {
    }
    return res;
}*/