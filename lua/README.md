# `taffy_cpp` Lua 5.3 bindings

## Dependencies

```shell
$ sudo apt-get install lua5.3 liblua5.3-dev
```

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
   $ cd taffy_cpp_bindings/lua/
   ```
4. Make 'build' directory and go into it:
   ```shell
   $ mkdir build; cd build
   ```
5. Generate `Makefile`:
   ```shell
   $ cmake ../ -DCMAKE_BUILD_TYPE=Release -DTAFFY_CPP_PATH=`pwd`/../../../taffy_cpp/
   ```
6. Build (static and shared) libraries and execute tests:
    ```shell
    $ make -j4 && lua5.3 ../tests/test.lua
    ```

## References

- [Lua 5.3 manual](https://www.lua.org/manual/5.3/manual.html)
- [How to add your C Library in Lua](https://blog.devgenius.io/how-to-add-your-c-library-in-lua-46fd246f0fa8)
- [lua-users :: BindingCodeToLua](http://lua-users.org/wiki/BindingCodeToLua)
- [lua-users :: Binding Enums To Lua](http://lua-users.org/wiki/BindingEnumsToLua)
- [StackOverflow :: Lua c API - Add number to new lib](https://stackoverflow.com/questions/46517127/lua-c-api-add-number-to-new-lib)
- [StackOverflow :: Wrapping a C library for Lua: how do I create nested tables of functions?](https://stackoverflow.com/questions/9527417/wrapping-a-c-library-for-lua-how-do-i-create-nested-tables-of-functions)
- [StackOverflow :: How to dump a table to console?](https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console)
- [StackOverflow :: luaL_openlib replacement for Lua 5.2](https://stackoverflow.com/a/19041283/)
- [StackOverflow :: Accessing user data as a member of another user data in LUA C++](https://stackoverflow.com/questions/61775019/accessing-user-data-as-a-member-of-another-user-data-in-lua-c)
- [lua-users :: Luna Four Code](http://lua-users.org/wiki/LunaFourCode)
- [StackOverflow :: Lua userdata array access and methods](https://stackoverflow.com/questions/26970316/lua-userdata-array-access-and-methods)
- [StackOverflow :: implementing __index metafunction in C/c++](https://stackoverflow.com/questions/50880830/implementing-index-metafunction-in-c-c)
- [StackOverflow :: C/C++: lua_rawget() vs. __index](https://stackoverflow.com/questions/25466890/c-c-lua-rawget-vs-index)
- [Creating Solid APIs with Lua by Tyler Neylon :: Chapter 4. Making Your API Classy](https://www.oreilly.com/library/view/creating-solid-apis/9781491986301/ch04.html)
