import pyspi

dev = b"/dev/spidev0.0"
speed = 1000000
fd = pyspi.pyspi_setup(dev, speed)

tx = bytes(bytearray.fromhex("FF FF FF FF FF FF 40 00 00 00 00 95 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF DE AD BE EF BA AD F0 0D"))
pyspi.pyspi_write(fd, tx, len(tx))
