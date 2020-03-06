#ifndef __I2CDEVS_DEF
#define __I2CDEVS_DEF

int get_ds3231();
int read_register(int busfd, __uint16_t reg, unsigned char *buf, int bufsize);


#endif