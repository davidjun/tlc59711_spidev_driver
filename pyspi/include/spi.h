#ifndef MY_SPI_H_
#define MY_SPI_H_

int spi_setup(const char *device, uint32_t speed);
int spi_write(int fd, uint8_t *tx, int len);
int spi_try_lock(void);
int spi_unlock(void);

#endif /* MY_SPI_H_ */
