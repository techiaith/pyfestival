#!/usr/bin/env python
from distutils.core import setup, Extension
import os

festival_include = os.environ.get("FESTIVAL_INCLUDE", '/usr/include/festival')
speech_tools_include = os.environ.get("SPECCH_INCLUDE", '/usr/include/speech_tools')
festival_lib = os.environ.get("FESTIVAL_LIB", '/usr/lib')

setup(
    name='festival',
    description='Python Festival module',
    py_modules=['festival'],
    ext_modules=[
        Extension(
            '_festival',
            ['_festival.cpp'],
            include_dirs=[festival_include, speech_tools_include],
            library_dirs=[festival_lib],
            libraries=['Festival', 'estools', 'estbase', 'eststring'],
        ),
    ],
    version="0.1",
)
