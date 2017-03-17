#!/usr/bin/env python3

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import numpy

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("automata",
                             sources=["_automata.pyx", "c_automata.c", "arrays.c", "random.c"],
                             include_dirs=[numpy.get_include()])],
)