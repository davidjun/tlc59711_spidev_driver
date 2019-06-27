from libc.stdint cimport uint32_t, uint8_t

cdef extern from "spi.h":
    int spi_setup(const char *device, uint32_t speed)
    int spi_write(int fd, uint8_t *tx, int len)

def pyspi_setup(device: bytes, speed: int) -> int:
    return spi_setup(device, speed)

def pyspi_write(fd: int, tx: bytes, len: int) -> int:
    return spi_write(fd, tx, len)
