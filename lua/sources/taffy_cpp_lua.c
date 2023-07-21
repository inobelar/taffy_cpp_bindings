#include <taffy_cpp_lua.h>

/* taffy_cpp c library */
#include <taffy_cpp_c.h>

/* lua c api */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

/* Method implementation */
static int example(lua_State* L)
{
    (void)L;
    return 0;
}

static const luaL_Reg lib_functions[] =
{
    {"example", example},
    {NULL, NULL}
};

int luaopen_libtaffy_cpp_lua(lua_State* L);

int luaopen_libtaffy_cpp_lua(lua_State* L)
{



    /* 
        Register the library

        The next code is alternative to:
            luaL_openlib(L, "taffy", lib_functions, 0);
        Reference: https://stackoverflow.com/a/19041283/
    */
    lua_getglobal(L, "taffy");
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      lua_newtable(L);
    }
    luaL_setfuncs(L, lib_functions, 0);
    lua_setglobal(L, "taffy");



    lua_getglobal(L, "taffy");
    {
        /* Register AlignContent enum. Same as in Lua:

            AlignContent = {
                ["Start"] = 0,
                ["End"]   = 1,
                ...
            }
        */
        lua_pushliteral(L, "AlignContent");
        lua_newtable(L);
        {
            lua_pushliteral(L, "Start"       ); lua_pushnumber(L, taffy_AlignContent_Start       ); lua_settable(L, -3);
            lua_pushliteral(L, "End"         ); lua_pushnumber(L, taffy_AlignContent_End         ); lua_settable(L, -3);
            lua_pushliteral(L, "FlexStart"   ); lua_pushnumber(L, taffy_AlignContent_FlexStart   ); lua_settable(L, -3);
            lua_pushliteral(L, "FlexEnd"     ); lua_pushnumber(L, taffy_AlignContent_FlexEnd     ); lua_settable(L, -3);
            lua_pushliteral(L, "Center"      ); lua_pushnumber(L, taffy_AlignContent_Center      ); lua_settable(L, -3);
            lua_pushliteral(L, "Stretch"     ); lua_pushnumber(L, taffy_AlignContent_Stretch     ); lua_settable(L, -3);
            lua_pushliteral(L, "SpaceBetween"); lua_pushnumber(L, taffy_AlignContent_SpaceBetween); lua_settable(L, -3);
            lua_pushliteral(L, "SpaceEvenly" ); lua_pushnumber(L, taffy_AlignContent_SpaceEvenly ); lua_settable(L, -3);
            lua_pushliteral(L, "SpaceAround" ); lua_pushnumber(L, taffy_AlignContent_SpaceAround ); lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
    lua_pop(L, 1);

    return 1;
}