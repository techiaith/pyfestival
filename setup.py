#!/usr/bin/env python
from distutils.core import setup, Extension
from distutils.util import get_platform
import os

festival_include = os.environ.get("FESTIVAL_INCLUDE", '/home/ubuntu/Projects/Fest_test/buildfestival')
speech_tools_include = os.environ.get("SPECCH_INCLUDE", '/home/ubuntu/Projects/Fest_test/buildspeech_tools')
festival_lib = os.environ.get("FESTIVAL_LIB", '/home/ubuntu/Projects/Fest_test/build/bin/festival')

festival_classifiers = [
"Programming Language :: Python :: 2",
"Programming Language :: Python :: 3",
"Intended Audience :: Developers",
"License :: OSI Approved :: BSD License",
"Topic :: Software Development :: Libraries",
"Topic :: Utilities",
"Topic :: Multimedia :: Sound/Audio :: Sound Synthesis",
"Topic :: Multimedia :: Sound/Audio :: Speech"
]

long_description = """A Python wrapper around the Festival Speech Synthesis System:
http://www.cstr.ed.ac.uk/projects/festival/

pyfestival creates a C module built on top of the festival source code, making it a self-contained Python library
(there is no need to run festival-server alongside).

pyfestival supports (and is tested on) Python 2.7+ including Python 3
"""

libraries = ['Festival', 'estools', 'estbase', 'eststring']

if get_platform().startswith('macosx-'):
    macos_frameworks = ['CoreAudio', 'AudioToolbox', 'Carbon']
    libraries.append('ncurses')
else:
    macos_frameworks = []

setup(
    name='pyfestival',
    description='Python Festival module',
    long_description=long_description,
    url="https://github.com/techiaith/pyfestival",
    author="Patrick Robertson",
    author_email="techiaith@bangor.ac.uk",
    license="BSD",
    py_modules=['festival'],
    ext_modules=[
        Extension(
            '_festival',
            ['_festival.cpp'],
            include_dirs=[festival_include, speech_tools_include],
            library_dirs=[festival_lib],
            libraries=libraries,
            extra_link_args=[x for name in macos_frameworks for x in ('-framework', name)],
        ),
    ],
    platforms=["*nix"],
    bugtrack_url="https://github.com/techiaith/pyfestival/issues",
    version="0.5",
)
