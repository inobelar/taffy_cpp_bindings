# `taffy_cpp` C89 bindings

## Build

1. Clone `taffy_cpp` - the main dependency:
   ```shell
   $ git clone https://github.com/inobelar/taffy_cpp.git
   ```
2. Clone this repository:
   ```shell
   $ git clone https://github.com/inobelar/taffy_cpp_bindings.git
   ```
3. Go into this directory:
   ```shell
   $ cd taffy_cpp_bindings/c/
   ```
4. Make 'build' directory and go into it:
   ```shell
   $ mkdir build; cd build
   ```
5. Generate `Makefile`:
   ```shell
   $ cmake ../ -DCMAKE_BUILD_TYPE=Release -DTAFFY_CPP_PATH=`pwd`/../../../taffy_cpp/
   ```
6. Build (static and shared) libraries:
    ```shell
    $ make -j4
    ``` 
