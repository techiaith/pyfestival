#!/usr/bin/env python
from distutils.core import setup, Extension
setup(
    name='_festival',
    description='Python Festival module',
    ext_modules=[
        Extension(
            '_festival',
            ['_festival.cpp'],
            include_dirs=['/usr/include/festival', '/usr/include/speech_tools'],
            library_dirs=['/usr/lib'],
            libraries=['Festival', 'estools', 'estbase', 'eststring'],
        ),
    ],
    version="0.1",
)
