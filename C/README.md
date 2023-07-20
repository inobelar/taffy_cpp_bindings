# `taffy_cpp` C89 bindings

## Build

1. `$ git clone https://github.com/inobelar/taffy_cpp.git`
2. `$ git clone https://github.com/inobelar/taffy_cpp_bindings.git` - clone this repository
3. `$ cd taffy_cpp_bindings/C/`
4. `$ mkdir build; cd build` - make 'build' directory and go into it
5. `$ cmake ../ -DCMAKE_BUILD_TYPE=Release -DTAFFY_CPP_PATH=`pwd`/../../../taffy_cpp/` - generate `Makefile`
6. `$ make -j4` - build libraries