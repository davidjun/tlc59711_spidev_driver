cdef extern from "spi.h":
    int spi_setup(const char *device, uint32_t speed)
    int spi_write(int fd, uint8_t *tx, int len)

def py_spi_setup(device: bytes, speed: int) -> int:
    return spi_setup(device, speed)

def py_spi_write(fd: int, tx: bytes, len: int) -> int:
    return spi_setup(fd, tx, len)
