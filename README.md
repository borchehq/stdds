# stdds
This library implements common data types in C in a generic fashion, that is
which can be used with any built-in type and in principle, with any user-defined
data type. The Standard Template Library (STL) from C ++ serves as inspiration.
Accordingly, this library tries to imitate most of the features of the STL and
thus makes them accessible in C. This project is at an experimental stage. The
API may change.

### Installation

stdds requires CMake and GCC for compilation. Other C-compilers may work. To
compile the library, first, navigate into the git project. Then execute the
following steps.

```sh
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../src
$ cmake install
```
