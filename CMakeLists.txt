cmake_minimum_required(VERSION 2.8.3)
project(tlc59711_spidev_driver)

include_directories(
  include
)

set(CMAKE_POSITION_INDEPENDENT_CODE "True")
add_library(spi STATIC
  src/spi.c
)

add_executable(spidev_test
  src/spi.c
)

target_link_libraries(spidev_test
  -lpthread
#  spi
)
