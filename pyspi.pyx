from libc.stdint cimport uint32_t, uint8_t

cdef extern from "spi.h":
    int spi_setup(const char *device, uint32_t speed)
    int spi_write(int fd, uint8_t *tx, int len)
    int spi_try_lock()
    int spi_unlock()

def setup(device: bytes, speed: int) -> int:
    return spi_setup(device, speed)

def write(fd: int, tx: bytes, len: int) -> int:
    return spi_write(fd, tx, len)

def try_lock() -> int:
    return spi_try_lock()

def unlock() -> int:
    return spi_unlock()


