/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <pthread.h>
#include "spi.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static uint32_t _speed;
static uint16_t _delay;
static uint8_t _bits;
static uint8_t _mode;
static bool _setup = false;
static pthread_mutex_t lock;

int spi_try_lock(void)
{
	if(!_setup)
		return -1;
	return pthread_mutex_trylock(&lock);
}

int spi_unlock(void)
{
	if(!_setup)
		return -1;
	return pthread_mutex_unlock(&lock);
}

int spi_setup(const char *device, uint32_t speed)
{
	int fd;
	int ret = 0;
	
	_speed = speed;
	_delay = 0;
	_bits = 8;
	_mode = 0;

	ret = pthread_mutex_init(&lock, NULL);
	if (ret != 0)
        pabort("\n mutex init failed\n");

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &_mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &_mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &_bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &_bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &_speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &_speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", _mode);
	printf("bits per word: %d\n", _bits);
	printf("speed: %d Hz (%d KHz)\n", _speed, _speed/1000);

	_setup = true;
	return fd;
}

int spi_write(int fd, uint8_t *tx, int len)
{
	if(!_setup)
		return -1;
	
	int ret=0;
	uint8_t rx[len];
	memset( rx, 0, len*sizeof(tx[0]) );
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = _delay,
		.speed_hz = _speed,
		.bits_per_word = _bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

#ifdef DEBUG
	for (ret = 0; ret < len; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	puts("");
#endif
	return ret;
}

int main(int argc, char *argv[])
{
	int fd, ret;

	const char *device = "/dev/spidev0.0";
	const uint32_t speed = 10000000;

	fd = spi_setup(device, speed);

	uint8_t tx[] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};

	ret = spi_write(fd, tx, ARRAY_SIZE(tx));

	ret = close(fd);
	// TODO: need a library hook to destroy mutex 
	pthread_mutex_destroy(&lock);

	return ret;
}
