from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

spi_extension = Extension(
    name="pyspi",
    sources=["pyspi/pyspi.pyx"],
    libraries=["spi"],
    library_dirs=["pyspi/lib"],
    include_dirs=["pyspi/include"]
)
setup(
    name="pyspi",
    ext_modules=cythonize([spi_extension])
)

from setuptools import find_packages
setup(
    name='tlc59711',
    version='0.1.0',
    description='A driver for the TLC59711 using pyspi',
    packages=find_packages(),
    install_requires=[
        'pyspi',
    ],
)
