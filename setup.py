from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

spi_extension = Extension(
    name="pyspi",
    sources=["pyspi.pyx"],
    libraries=["spi"],
    library_dirs=["lib"],
    include_dirs=["include"]
)
setup(
    name="pyspi",
    ext_modules=cythonize([spi_extension])
)
