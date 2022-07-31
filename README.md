# Py-festival

A Python wrapper around the [Festival Speech Synthesis System](http://www.cstr.ed.ac.uk/projects/festival/).

#### Installation

1. First install the Python, Festival and their headers    
On Debian:

```
sudo apt-get install python python-dev festival festival-dev
```

On other operating systems (e.g. macOS) you may need to manually install festival. E.g. [[1]](https://github.com/pettarin/setup-festival-mbrola) or [[2]](https://gist.github.com/laic/519deca91d50b1ed19307d0c80cb788e)

2. Then install using either pip or directly from github:

```
pip install pyfestival
```
or    
```
pip install git+https://github.com/techiaith/pyfestival#festival
```    

**Environment Variables**

If your festival/speechtools headers and libs aren't in the standard place, you may need to set the following variables before installing with pip:

* `FESTIVAL_INCLUDE` - festival header directory. Default is `/usr/include/festival`
* `SPEECH_INCLUDE` - speech tools header directory. Default is `/usr/include/speech_tools`
* `FESTIVAL_LIB` - lib directory for festival/speech tools `/usr/lib`

### Threading notes

Festival is not thread-safe. If you attempt to invoke it from a thread other than which is was imported in then you will see the error:
```
SIOD ERROR: the currently assigned stack limit has been exceeded
```
It may be imported locally in each new thread once the previous thread has exited.
