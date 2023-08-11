#include <taffy_cpp_lua.h>

/* taffy_cpp C library */
#include <taffy_cpp_c.h>

/* Lua C api */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h> /* for: malloc(), free() */
#include <string.h> /* for: strcmp() */

/*
    NOTES

    ----------------------------------------------------------------------------

    - lua_rawseti() / lua_rawgeti() usage with 'size_t' index:

      Functions for setting/getting table value by index have the next
      signature:

        LUA_API void (lua_rawgeti) (lua_State *L, int idx, int n);
        LUA_API void (lua_rawseti) (lua_State *L, int idx, int n);

      As you can see, `n` argument have `int` type, so when we use it like:

        size_t index = 0;
        for(index = 0; index < items_count; ++index) {
            ...
            lua_rawseti(L, -2, (int)(index+1));
        }

      for very large 'index' here may be `int` truncation.

      Here is answer of 'Roberto Ierusalimschy' about this behavior:

        https://lua-l.lua.narkive.com/YMA6HK9B/lua-rawgeti-and-lua-rawseti-signatures

        > The size of tables is already limited to 2147483647 elements. Lua
        > internally uses 'int' to index all its arrays (except for strings/byte
        > arrays). It is a pain to work with unsigned values (such as size_t)
        > everywhere; ptrdiff_t has no garanties at all.

    ----------------------------------------------------------------------------

    - For getting / pushing float-point and integer numbers we use specialized
      functions:

        - Getting:
            - luaL_checknumber()
            - luaL_checkinteger()
        - Pushing:
            - lua_pushnumber()
            - lua_pushinteger()

      and types: lua_Number and lua_Integer - to show explicetely what we need
      (instead using only lua_Number everywhere).

    ----------------------------------------------------------------------------

    - Strict / Strong / Scoped enumeration definition:

      Instead of representing enums in naive way (as table of named integers):

        Lua: Describing AlignContent enum

            AlignContent = {
                ["Start"] = 0,
                ["End"]   = 1,
                ...
            }

        C: Describing AlignContent enum

            {
                / * root namespace table on stack * /

                lua_newtable(L);
                {
                    lua_pushinteger(L, taffy_AlignContent_Start ); lua_setfield(L, -2, "Start");
                    lua_pushinteger(L, taffy_AlignContent_End   ); lua_setfield(L, -2, "End");
                    ...
                }
                lua_setfield(L, -2, "AlignContent");
            }


      we represent enums as independant types: userdata + metatable. The next
      reasons why we need it:
        - To be able to distinguish enums from float-point or integer values,
          when its needed (and to not allow to use arbitrary numbers instead of
          known enum values).
        - To be able to distinguish enums from other enums:
            - t.AlignContent.Start == t.JustifyContent.Start --> error
        - To disable any additions and modifications of enum values. For
          example:
            - t.AlignContent.Wrong        --> nil
            - t.AlignContent.Wrong = 42   --> error
            - t.AlignContent.Start = 42   --> error
            - t.AlignContent.Start.x      --> nil
            - t.AlignContent.Start.x = 42 --> error
*/

/* -------------------------------------------------------------------------- */

/*
    Special message string, used for metatable protection (to disallow change it
    via 'setmeatable()' call). Reference: https://www.lua.org/pil/13.3.html

    ----------------------------------------------------------------------------

    NOTE: instead of string, assigned as:

        Lua:
            metatable.__metatable = "message"
        Lua C API:
            lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
            lua_setfield(L, -2, "__metatable");

    for optimization, we can assign 'boolean' value. For example:

        Lua:
            metatable.__metatable = false
        Lua C API:
            lua_pushboolean(L, 0);
            lua_setfield(L, -2, "__metatable");

    But its good to show 'explicit message' for users, that not familiar with
    such Lua tricks, instead some silent 'false' magic :)

    ----------------------------------------------------------------------------

    Notice, that we DONT use 'metatable protection' for 'namespace'-like
    metatables, to make them transparently visible during 'inspection'.
*/
static const char LUA_METATABLE_PROTECTION_MESSAGE[] = "protected metatable";

/* -------------------------------------------------------------------------- */

/*
    Special function, used when we need to disable '__newindex' meta-method -
    to not allow to add new entities into table, or modify existing.

    Typically used in all 'namespace'-like metatables to disable addition or
    modification.

    Reference: http://lua-users.org/wiki/ReadOnlyTables
*/
static int lua_newindex_disabled(lua_State* L)
{
    return luaL_error(L, "Attempt to modify read-only table");
}

/* -------------------------------------------------------------------------- */
/* Option<float> */

static const char LUA_META_OBJECT_taffy_Option_float[]           = "taffy_Option_float_mt";
static const char LUA_META_OBJECT_taffy_Option_float_namespace[] = "taffy_Option_float_namespace_mt";

static int lua_taffy_Option_float_new(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNONE:
    {
        taffy_Option_float* object_ptr = taffy_Option_float_new_default();
        if(object_ptr != NULL)
        {
            taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_float : taffy_Option_float_new_default() failed");
        }
    } break;

    case LUA_TNIL:
    {
        taffy_Option_float* object_ptr = NULL;

        lua_pop(L, 1);

        object_ptr = taffy_Option_float_new(NULL);
        if(object_ptr != NULL)
        {
            taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_float : taffy_Option_float_new_some() failed");
        }
    } break;

    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 1);

        taffy_Option_float* object_ptr = taffy_Option_float_new_some(value);
        if(object_ptr != NULL)
        {
            taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_float : taffy_Option_float_new_some() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Option_float : incorrect argument type");
}

static int lua_taffy_Option_float_copy(lua_State* L)
{
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    taffy_Option_float* copy = taffy_Option_float_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_float : taffy_Option_float_new_copy() failed");
    }
}

static int lua_taffy_Option_float_delete(lua_State* L)
{
    taffy_Option_float** self = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    taffy_Option_float_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Option_float_eq(lua_State* L)
{
    taffy_Option_float** object_lhs = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);
    taffy_Option_float** object_rhs = (taffy_Option_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_float);

    const int is_equal = taffy_Option_float_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Option_float_is_some(lua_State* L)
{
    taffy_Option_float** self = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    const int is_some = taffy_Option_float_is_some(*self);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_float_get_value(lua_State* L)
{
    taffy_Option_float** self = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    if( taffy_Option_float_is_some(*self) )
    {
        const float value = taffy_Option_float_get_value(*self);

        lua_pushnumber(L, value);

        return 1; /* number of results */
    }
    else /* None */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Option_float_set_value(lua_State* L)
{
    taffy_Option_float** self = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_float_set_value(*self, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 2);
        const float f_value = (float)(value);

        taffy_Option_float_set_value(*self, &f_value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_float : incorrect argument type");
}

static void lua_push_table_taffy_Option_float(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_float) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Option_float_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Option_float_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_float_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Option_float_is_some);
        lua_setfield(L, -2, "is_some");

        lua_pushcfunction(L, lua_taffy_Option_float_get_value);
        lua_setfield(L, -2, "get_value");

        lua_pushcfunction(L, lua_taffy_Option_float_set_value);
        lua_setfield(L, -2, "set_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_float_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_float_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float_namespace);
}

/* -------------------------------------------------------------------------- */
/* Point<float> */

static const char LUA_META_OBJECT_taffy_Point_of_float[]           = "taffy_Point_of_float_mt";
static const char LUA_META_OBJECT_taffy_Point_of_float_namespace[] = "taffy_Point_of_float_namespace_mt";

static int lua_taffy_Point_of_float_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 0:
    {
        taffy_Point_of_float* object_ptr = taffy_Point_of_float_new_default();
        if(object_ptr != NULL)
        {
            taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new_default() failed");
        }
    } break;

    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {35, 42}

                    {[1] = 35, [2] = 42}
                    {[2] = 42, [1] = 35}

                and make sure, that here is proper indices (default 1 and 2),
                not something like:

                    {[5] = 35, [3] = 42}
                    {35, [1] = 42}       <-- table with single item '35'
                    {[1] = 35, [1] = 42} <-- table with single item '42'

                ----------------------------------------------------------------

                Notice, that during investigations where 'x' and 'y', we looking
                only for table's 'key' value (that represent index), and NOT
                expect, that first 'lua_next()' call returns 'first index' and
                second 'lua_next()' call returns 'second index', since it may
                not be happened.

                For example - for simple case like '{35, 42}' first 'lua_next()'
                returns '1' index and second call returns '2' index, BUT for
                uncommon case like '{[1] = 35, [2] = 42}' order is undefined,
                and first item returned by 'lua_next()' may be
                {key: 2, value: 42}.
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int x_found = 0; /* false */
                /* bool */ int y_found = 0; /* false */

                float x = 0.0f;
                float y = 0.0f;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TNUMBER))
                    {
                        lua_Number value_value = 0.0f;
                        lua_Number key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = lua_tonumber(L, -1); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2); /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'x' */
                        {
                            x_found = 1; /* true */
                            x = value_value;
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'y' */
                        {
                            y_found = 1; /* true */
                            y = value_value;
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (x_found == /* true */ 1) && (y_found == /* true */ 1) )
                {
                    taffy_Point_of_float* object_ptr = taffy_Point_of_float_new(x, y);
                    if(object_ptr != NULL)
                    {
                        taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {x = 35, y = 42}

                if table size != 2 OR 'x' and 'y' not in indexes '1' and '2'
            */
            {
                /* bool */ int x_found = 0; /* false */
                /* bool */ int y_found = 0; /* false */

                float x = 0.0f;
                float y = 0.0f;

                /* Try to get 'x' */
                {
                    const int x_type = lua_getfield(L, 1, "x");
                    if(x_type == LUA_TNUMBER)
                    {
                        const lua_Number x_value = lua_tonumber(L, -1);

                        x_found = 1; /* true */
                        x = x_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'y' */
                {
                    const int y_type = lua_getfield(L, 1, "y");
                    if(y_type == LUA_TNUMBER)
                    {
                        const lua_Number y_value = lua_tonumber(L, -1);

                        y_found = 1; /* true */
                        y = y_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (x_found == /* true */ 1) && (y_found == /* true */ 1) )
                {
                    taffy_Point_of_float* object_ptr = taffy_Point_of_float_new(x, y);
                    if(object_ptr != NULL)
                    {
                        taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Point_of_float : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_float : provided argument is not a table");
        }
    } break;

    case 2:
    {
        const lua_Number x = luaL_checknumber(L, 1);
        const lua_Number y = luaL_checknumber(L, 2);

        taffy_Point_of_float* object_ptr = taffy_Point_of_float_new(x, y);
        if(object_ptr != NULL)
        {
            taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Point_of_float : wrong arguments count");
}

static int lua_taffy_Point_of_float_copy(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    taffy_Point_of_float* copy = taffy_Point_of_float_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Point_of_float : taffy_Point_of_float_new_copy() failed");
    }
}

static int lua_taffy_Point_of_float_delete(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    taffy_Point_of_float_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_float_eq(lua_State* L)
{
    taffy_Point_of_float** object_lhs = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);
    taffy_Point_of_float** object_rhs = (taffy_Point_of_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Point_of_float);

    const int is_equal = taffy_Point_of_float_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_add(lua_State* L)
{
    taffy_Point_of_float** object_lhs = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);
    taffy_Point_of_float** object_rhs = (taffy_Point_of_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Point_of_float);

    taffy_Point_of_float* object_ptr = taffy_Point_of_float_new_add(*object_lhs, *object_rhs);
    if(object_ptr != NULL)
    {
        taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new_add() failed");
    }
}

static int lua_taffy_Point_of_float_get_x(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const float x = taffy_Point_of_float_get_x(*self);

    lua_pushnumber(L, x);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_get_y(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const float y = taffy_Point_of_float_get_y(*self);

    lua_pushnumber(L, y);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_set_x(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const lua_Number x = luaL_checknumber(L, 2);

    taffy_Point_of_float_set_x(*self, x);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_float_set_y(lua_State* L)
{
    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const lua_Number y = luaL_checknumber(L, 2);

    taffy_Point_of_float_set_y(*self, y);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_float_ZERO(lua_State* L)
{
    taffy_Point_of_float* object_ptr = taffy_Point_of_float_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_Point_of_float** user_data = (taffy_Point_of_float**)lua_newuserdata(L, sizeof(taffy_Point_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new_ZERO() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Point_of_float_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "x") == 0)
    {
        const float x = taffy_Point_of_float_get_x(*self);

        lua_pushnumber(L, x);

        return 1; /* number of results */
    }
    else if(strcmp(key, "y") == 0)
    {
        const float y = taffy_Point_of_float_get_y(*self);

        lua_pushnumber(L, y);

        return 1; /* number of results */
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Point_of_float** self = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);
    const char* key = luaL_checkstring(L, 2);
    const lua_Number value = luaL_checknumber(L, 3);

    if(strcmp(key, "x") == 0)
    {
        taffy_Point_of_float_set_x(*self, value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "y") == 0)
    {
        taffy_Point_of_float_set_y(*self, value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Point_of_float 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Point_of_float(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Point_of_float) )
    {
        lua_pushcfunction(L, lua_taffy_Point_of_float_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Point_of_float_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Point_of_float_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Point_of_float_eq);
        lua_setfield(L, -2, "__eq");

        lua_pushcfunction(L, lua_taffy_Point_of_float_add);
        lua_setfield(L, -2, "__add");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Point_of_float_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Point_of_float_get_x);
        lua_setfield(L, -2, "get_x");

        lua_pushcfunction(L, lua_taffy_Point_of_float_get_y);
        lua_setfield(L, -2, "get_y");

        lua_pushcfunction(L, lua_taffy_Point_of_float_set_x);
        lua_setfield(L, -2, "set_x");

        lua_pushcfunction(L, lua_taffy_Point_of_float_set_y);
        lua_setfield(L, -2, "set_y");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Point_of_float_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Point_of_float_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Point_of_float_ZERO);
        lua_setfield(L, -2, "ZERO");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_float_namespace);
}

/* -------------------------------------------------------------------------- */
/* Size<float> */

static const char LUA_META_OBJECT_taffy_Size_of_float[]           = "taffy_Size_of_float_mt";
static const char LUA_META_OBJECT_taffy_Size_of_float_namespace[] = "taffy_Size_of_float_namespace_mt";

static int lua_taffy_Size_of_float_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 0:
    {
        taffy_Size_of_float* object_ptr = taffy_Size_of_float_new_default();
        if(object_ptr != NULL)
        {
            taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_default() failed");
        }
    } break;

    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {35, 42}

                    {[1] = 35, [2] = 42}
                    {[2] = 42, [1] = 35}
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int width_found = 0;  /* false */
                /* bool */ int height_found = 0; /* false */

                float width = 0.0f;
                float height = 0.0f;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TNUMBER))
                    {
                        lua_Number value_value = 0.0f;
                        lua_Number key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = lua_tonumber(L, -1); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2); /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'width' */
                        {
                            width_found = 1; /* true */
                            width = value_value;
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'height' */
                        {
                            height_found = 1; /* true */
                            height = value_value;
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {width = 35, height = 42}

                if table size != 2 OR 'width' and 'height' not in indexes '1' and '2'
            */
            {
                /* bool */ int width_found = 0;  /* false */
                /* bool */ int height_found = 0; /* false */

                float width = 0.0f;
                float height = 0.0f;

                /* Try to get 'width' */
                {
                    const int width_type = lua_getfield(L, 1, "width");
                    if(width_type == LUA_TNUMBER)
                    {
                        const lua_Number width_value = lua_tonumber(L, -1);

                        width_found = 1; /* true */
                        width = width_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'height' */
                {
                    const int height_type = lua_getfield(L, 1, "height");
                    if(height_type == LUA_TNUMBER)
                    {
                        const lua_Number height_value = lua_tonumber(L, -1);

                        height_found = 1; /* true */
                        height = height_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Size_of_float : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : provided argument is not a table");
        }
    } break;

    case 2:
    {
        const lua_Number width  = luaL_checknumber(L, 1);
        const lua_Number height = luaL_checknumber(L, 2);

        taffy_Size_of_float* object_ptr = taffy_Size_of_float_new(width, height);
        if(object_ptr != NULL)
        {
            taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Size_of_float : wrong arguments count");
}

static int lua_taffy_Size_of_float_copy(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float* copy = taffy_Size_of_float_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Size_of_float : taffy_Size_of_float_new_copy() failed");
    }
}

static int lua_taffy_Size_of_float_delete(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_float_eq(lua_State* L)
{
    taffy_Size_of_float** object_lhs = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);
    taffy_Size_of_float** object_rhs = (taffy_Size_of_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_float);

    const int is_equal = taffy_Size_of_float_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_add(lua_State* L)
{
    taffy_Size_of_float** object_lhs = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);
    taffy_Size_of_float** object_rhs = (taffy_Size_of_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new_add(*object_lhs, *object_rhs);
    if(object_ptr != NULL)
    {
        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_add() failed");
    }
}

static int lua_taffy_Size_of_float_sub(lua_State* L)
{
    taffy_Size_of_float** object_lhs = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);
    taffy_Size_of_float** object_rhs = (taffy_Size_of_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new_sub(*object_lhs, *object_rhs);
    if(object_ptr != NULL)
    {
        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_sub() failed");
    }
}

static int lua_taffy_Size_of_float_get_width(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const float width = taffy_Size_of_float_get_width(*self);

    lua_pushnumber(L, width);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_get_height(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const float height = taffy_Size_of_float_get_height(*self);

    lua_pushnumber(L, height);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_set_width(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const lua_Number width = luaL_checknumber(L, 2);

    taffy_Size_of_float_set_width(*self, width);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_float_set_height(lua_State* L)
{
    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const lua_Number height = luaL_checknumber(L, 2);

    taffy_Size_of_float_set_height(*self, height);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_float_ZERO(lua_State* L)
{
    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_ZERO() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Size_of_float_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "width") == 0)
    {
        const float width = taffy_Size_of_float_get_width(*self);

        lua_pushnumber(L, width);

        return 1; /* number of results */
    }
    else if(strcmp(key, "height") == 0)
    {
        const float height = taffy_Size_of_float_get_height(*self);

        lua_pushnumber(L, height);

        return 1; /* number of results */
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Size_of_float** self = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);
    const char* key = luaL_checkstring(L, 2);
    const lua_Number value = luaL_checknumber(L, 3);

    if(strcmp(key, "width") == 0)
    {
        taffy_Size_of_float_set_width(*self, value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "height") == 0)
    {
        taffy_Size_of_float_set_height(*self, value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Size_of_float 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Size_of_float(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_float) )
    {
        lua_pushcfunction(L, lua_taffy_Size_of_float_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Size_of_float_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Size_of_float_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Size_of_float_eq);
        lua_setfield(L, -2, "__eq");

        lua_pushcfunction(L, lua_taffy_Size_of_float_add);
        lua_setfield(L, -2, "__add");

        lua_pushcfunction(L, lua_taffy_Size_of_float_sub);
        lua_setfield(L, -2, "__sub");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_float_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Size_of_float_get_width);
        lua_setfield(L, -2, "get_width");

        lua_pushcfunction(L, lua_taffy_Size_of_float_get_height);
        lua_setfield(L, -2, "get_height");

        lua_pushcfunction(L, lua_taffy_Size_of_float_set_width);
        lua_setfield(L, -2, "set_width");

        lua_pushcfunction(L, lua_taffy_Size_of_float_set_height);
        lua_setfield(L, -2, "set_height");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_float_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_float_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Size_of_float_ZERO);
        lua_setfield(L, -2, "ZERO");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float_namespace);
}

/* -------------------------------------------------------------------------- */
/* AlignContent (enum) */

static const char LUA_META_OBJECT_taffy_AlignContent[]           = "taffy_AlignContent_mt";
static const char LUA_META_OBJECT_taffy_AlignContent_namespace[] = "taffy_AlignContent_namespace_mt";

static int lua_taffy_AlignContent_eq(lua_State* L)
{
    taffy_AlignContent* self = (taffy_AlignContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignContent);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignContent* other = (taffy_AlignContent*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignContent);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_AlignContent with other value - invalid type");
}

static int lua_taffy_AlignContent_to_int(lua_State* L)
{
    taffy_AlignContent* self = (taffy_AlignContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignContent);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_AlignContent_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_AlignContent_Start        :
    case taffy_AlignContent_End          :
    case taffy_AlignContent_FlexStart    :
    case taffy_AlignContent_FlexEnd      :
    case taffy_AlignContent_Center       :
    case taffy_AlignContent_Stretch      :
    case taffy_AlignContent_SpaceBetween :
    case taffy_AlignContent_SpaceEvenly  :
    case taffy_AlignContent_SpaceAround  :
    {
        taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
        *object = (taffy_AlignContent) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_AlignContent(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignContent) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_AlignContent_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignContent_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignContent_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignContent_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_Start;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "Start");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_End;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "End");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_FlexStart;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "FlexStart");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_FlexEnd;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "FlexEnd");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_Center;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "Center");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_Stretch;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "Stretch");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_SpaceBetween;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "SpaceBetween");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_SpaceEvenly;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "SpaceEvenly");

        {
            taffy_AlignContent* object = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
            *object = taffy_AlignContent_SpaceAround;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);
        }
        lua_setfield(L, -2, "SpaceAround");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent_namespace);
}

/* -------------------------------------------------------------------------- */
/* JustifyContent (enum) (same as AlignContent) */

static const char LUA_META_OBJECT_taffy_JustifyContent[]           = "taffy_JustifyContent";
static const char LUA_META_OBJECT_taffy_JustifyContent_namespace[] = "taffy_JustifyContent_namespace_mt";

static int lua_taffy_JustifyContent_eq(lua_State* L)
{
    taffy_JustifyContent* self = (taffy_JustifyContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_JustifyContent);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_JustifyContent* other = (taffy_JustifyContent*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_JustifyContent);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_JustifyContent with other value - invalid type");
}

static int lua_taffy_JustifyContent_to_int(lua_State* L)
{
    taffy_JustifyContent* self = (taffy_JustifyContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_JustifyContent);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_JustifyContent_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_JustifyContent_Start        :
    case taffy_JustifyContent_End          :
    case taffy_JustifyContent_FlexStart    :
    case taffy_JustifyContent_FlexEnd      :
    case taffy_JustifyContent_Center       :
    case taffy_JustifyContent_Stretch      :
    case taffy_JustifyContent_SpaceBetween :
    case taffy_JustifyContent_SpaceEvenly  :
    case taffy_JustifyContent_SpaceAround  :
    {
        taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
        *object = (taffy_JustifyContent) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_JustifyContent(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_JustifyContent) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_JustifyContent_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_JustifyContent_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_JustifyContent_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_JustifyContent_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_Start;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "Start");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_End;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "End");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_FlexStart;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "FlexStart");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_FlexEnd;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "FlexEnd");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_Center;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "Center");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_Stretch;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "Stretch");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_SpaceBetween;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "SpaceBetween");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_SpaceEvenly;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "SpaceEvenly");

        {
            taffy_JustifyContent* object = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
            *object = taffy_JustifyContent_SpaceAround;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);
        }
        lua_setfield(L, -2, "SpaceAround");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent_namespace);
}

/* -------------------------------------------------------------------------- */
/* AlignItems (enum) */

static const char LUA_META_OBJECT_taffy_AlignItems[]           = "taffy_AlignItems_mt";
static const char LUA_META_OBJECT_taffy_AlignItems_namespace[] = "taffy_AlignItems_namespace_mt";

static int lua_taffy_AlignItems_eq(lua_State* L)
{
    taffy_AlignItems* self = (taffy_AlignItems*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignItems);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignItems* other = (taffy_AlignItems*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignItems);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_AlignItems with other value - invalid type");
}

static int lua_taffy_AlignItems_to_int(lua_State* L)
{
    taffy_AlignItems* self = (taffy_AlignItems*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignItems);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_AlignItems_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_AlignItems_Start     :
    case taffy_AlignItems_End       :
    case taffy_AlignItems_FlexStart :
    case taffy_AlignItems_FlexEnd   :
    case taffy_AlignItems_Center    :
    case taffy_AlignItems_Baseline  :
    case taffy_AlignItems_Stretch   :
    {
        taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
        *object = (taffy_AlignItems) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_AlignItems(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignItems) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_AlignItems_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignItems_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignItems_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignItems_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_Start;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "Start");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_End;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "End");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_FlexStart;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "FlexStart");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_FlexEnd;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "FlexEnd");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_Center;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "Center");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_Baseline;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "Baseline");

        {
            taffy_AlignItems* object = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
            *object = taffy_AlignItems_Stretch;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);
        }
        lua_setfield(L, -2, "Stretch");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems_namespace);
}

/* -------------------------------------------------------------------------- */
/* AlignSelf (enum) (same as AlignItems) */

static const char LUA_META_OBJECT_taffy_AlignSelf[]           = "taffy_AlignSelf_mt";
static const char LUA_META_OBJECT_taffy_AlignSelf_namespace[] = "taffy_AlignSelf_namespace_mt";

static int lua_taffy_AlignSelf_eq(lua_State* L)
{
    taffy_AlignSelf* self = (taffy_AlignSelf*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignSelf);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignSelf* other = (taffy_AlignSelf*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignSelf);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_AlignSelf with other value - invalid type");
}

static int lua_taffy_AlignSelf_to_int(lua_State* L)
{
    taffy_AlignSelf* self = (taffy_AlignSelf*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignSelf);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_AlignSelf_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_AlignSelf_Start     :
    case taffy_AlignSelf_End       :
    case taffy_AlignSelf_FlexStart :
    case taffy_AlignSelf_FlexEnd   :
    case taffy_AlignSelf_Center    :
    case taffy_AlignSelf_Baseline  :
    case taffy_AlignSelf_Stretch   :
    {
        taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
        *object = (taffy_AlignSelf) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_AlignSelf(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignSelf) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_AlignSelf_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignSelf_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AlignSelf_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AlignSelf_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_Start;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "Start");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_End;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "End");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_FlexStart;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "FlexStart");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_FlexEnd;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "FlexEnd");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_Center;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "Center");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_Baseline;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "Baseline");

        {
            taffy_AlignSelf* object = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
            *object = taffy_AlignSelf_Stretch;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);
        }
        lua_setfield(L, -2, "Stretch");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf_namespace);
}

/* -------------------------------------------------------------------------- */
/* AvailableSpace */

static const char LUA_META_OBJECT_taffy_AvailableSpace[]           = "taffy_AvailableSpace_mt";
static const char LUA_META_OBJECT_taffy_AvailableSpace_namespace[] = "taffy_AvailableSpace_namespace_mt";

static int lua_taffy_AvailableSpace_Definite(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_Definite(value);
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_Definite() failed");
    }
}

static int lua_taffy_AvailableSpace_MinContent(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MinContent();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MinContent() failed");
    }
}

static int lua_taffy_AvailableSpace_MaxContent(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MaxContent();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MaxContent() failed");
    }
}

static int lua_taffy_AvailableSpace_copy(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    taffy_AvailableSpace* copy = taffy_AvailableSpace_new_copy(*self);

    if(copy != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_AvailableSpace : taffy_AvailableSpace_new_copy() failed");
    }
}

static int lua_taffy_AvailableSpace_delete(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    taffy_AvailableSpace_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_AvailableSpace_eq(lua_State* L)
{
    taffy_AvailableSpace** object_lhs = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);
    taffy_AvailableSpace** object_rhs = (taffy_AvailableSpace**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_equal = taffy_AvailableSpace_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_is_Definite(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_Definite = taffy_AvailableSpace_is_Definite(*self);

    lua_pushboolean(L, is_Definite);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_is_MinContent(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_MinContent = taffy_AvailableSpace_is_MinContent(*self);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_is_MaxContent(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_MaxContent = taffy_AvailableSpace_is_MaxContent(*self);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_get_value(lua_State* L)
{
    taffy_AvailableSpace** self = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    if( taffy_AvailableSpace_is_Definite(*self) )
    {
        const float value = taffy_AvailableSpace_get_value(*self);

        lua_pushnumber(L, value);

        return 1; /* number of results */
    }
    else /* not Definite --> nil */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_AvailableSpace_ZERO(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_ZERO() failed");
    }
}

static int lua_taffy_AvailableSpace_MIN_CONTENT(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MIN_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MIN_CONTENT() failed");
    }
}

static int lua_taffy_AvailableSpace_MAX_CONTENT(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MAX_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MAX_CONTENT() failed");
    }
}

static int lua_taffy_AvailableSpace_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_from_length() failed");
    }
}

static int lua_taffy_AvailableSpace_from(lua_State* L)
{
    switch( lua_type(L, 1) ) {

    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 1);
        const float f_value = (float)(value);

        taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_from_float(f_value);
        if(object_ptr != NULL)
        {
            taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_from_float() failed");
        }
    } break;

    case LUA_TUSERDATA:
    {
        taffy_Option_float** option = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

        taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_from_option(*option);
        if(object_ptr != NULL)
        {
            taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_from_option() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to call 'from' of taffy_AvailableSpace : incorrect argument type");
}

static void lua_push_table_taffy_AvailableSpace(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AvailableSpace_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_is_Definite);
        lua_setfield(L, -2, "is_Definite");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_is_MinContent);
        lua_setfield(L, -2, "is_MinContent");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_is_MaxContent);
        lua_setfield(L, -2, "is_MaxContent");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AvailableSpace_Definite);
        lua_setfield(L, -2, "Definite");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_MinContent);
        lua_setfield(L, -2, "MinContent");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_MaxContent);
        lua_setfield(L, -2, "MaxContent");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_AvailableSpace_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_MIN_CONTENT);
        lua_setfield(L, -2, "MIN_CONTENT");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_MAX_CONTENT);
        lua_setfield(L, -2, "MAX_CONTENT");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_AvailableSpace_from);
        lua_setfield(L, -2, "from");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace_namespace);
}

/* -------------------------------------------------------------------------- */
/* LengthPercentage */

static const char LUA_META_OBJECT_taffy_LengthPercentage[]           = "taffy_LengthPercentage_mt";
static const char LUA_META_OBJECT_taffy_LengthPercentage_namespace[] = "taffy_LengthPercentage_namespace_mt";

static int lua_taffy_LengthPercentage_Length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_Length(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_Length() failed");
    }
}

static int lua_taffy_LengthPercentage_Percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_Percent(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_Percent() failed");
    }
}

static int lua_taffy_LengthPercentage_copy(lua_State* L)
{
    taffy_LengthPercentage** self = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(*self);

    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_LengthPercentage_delete(lua_State* L)
{
    taffy_LengthPercentage** self = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentage_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_LengthPercentage_eq(lua_State* L)
{
    taffy_LengthPercentage** object_lhs = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);
    taffy_LengthPercentage** object_rhs = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    const int is_equal = taffy_LengthPercentage_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentage_is_Length(lua_State* L)
{
    taffy_LengthPercentage** self = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    const int is_Length = taffy_LengthPercentage_is_Length(*self);

    lua_pushboolean(L, is_Length);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentage_is_Percent(lua_State* L)
{
    taffy_LengthPercentage** self = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    const int is_Percent = taffy_LengthPercentage_is_Percent(*self);

    lua_pushboolean(L, is_Percent);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentage_get_value(lua_State* L)
{
    taffy_LengthPercentage** self = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    const float value = taffy_LengthPercentage_get_value(*self);

    lua_pushnumber(L, value);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentage_ZERO(lua_State* L)
{
    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_ZERO() failed");
    }
}

static int lua_taffy_LengthPercentage_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_from_length() failed");
    }
}

static int lua_taffy_LengthPercentage_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_from_percent() failed");
    }
}

static void lua_push_table_taffy_LengthPercentage(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentage_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_is_Length);
        lua_setfield(L, -2, "is_Length");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_is_Percent);
        lua_setfield(L, -2, "is_Percent");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentage_Length);
        lua_setfield(L, -2, "Length");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_Percent);
        lua_setfield(L, -2, "Percent");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentage_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_LengthPercentage_from_percent);
        lua_setfield(L, -2, "from_percent");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage_namespace);
}

/* -------------------------------------------------------------------------- */
/* LengthPercentageAuto */

static const char LUA_META_OBJECT_taffy_LengthPercentageAuto[]           = "taffy_LengthPercentageAuto_mt";
static const char LUA_META_OBJECT_taffy_LengthPercentageAuto_namespace[] = "taffy_LengthPercentageAuto_namespace_mt";

static int lua_taffy_LengthPercentageAuto_Length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_Length(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_Length() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_Percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_Percent(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_Percent() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_Auto(lua_State* L)
{
    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_Auto();
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_Auto() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_copy(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(*self);

    if(copy != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_delete(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_LengthPercentageAuto_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_eq(lua_State* L)
{
    taffy_LengthPercentageAuto** object_lhs = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
    taffy_LengthPercentageAuto** object_rhs = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_equal = taffy_LengthPercentageAuto_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */

}

static int lua_taffy_LengthPercentageAuto_is_Length(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Length = taffy_LengthPercentageAuto_is_Length(*self);

    lua_pushboolean(L, is_Length);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_is_Percent(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Percent = taffy_LengthPercentageAuto_is_Percent(*self);

    lua_pushboolean(L, is_Percent);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_is_Auto(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Auto = taffy_LengthPercentageAuto_is_Auto(*self);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_get_value(lua_State* L)
{
    taffy_LengthPercentageAuto** self = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    if( taffy_LengthPercentageAuto_is_Length (*self) ||
        taffy_LengthPercentageAuto_is_Percent(*self) )
    {
        const float value = taffy_LengthPercentageAuto_get_value(*self);

        lua_pushnumber(L, value);

        return 1; /* number of results */
    }
    else /* !Length && !Percent */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_LengthPercentageAuto_ZERO(lua_State* L)
{
    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_ZERO() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_from_length() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_from_percent() failed");
    }
}

static int lua_taffy_LengthPercentageAuto_from(lua_State* L)
{
    taffy_LengthPercentage** length_percentage = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentageAuto* object_ptr = taffy_LengthPercentageAuto_new_from_LengthPercentage(*length_percentage);
    if(object_ptr != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_from_LengthPercentage() failed");
    }
}

static void lua_push_table_taffy_LengthPercentageAuto(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_is_Length);
        lua_setfield(L, -2, "is_Length");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_is_Percent);
        lua_setfield(L, -2, "is_Percent");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_is_Auto);
        lua_setfield(L, -2, "is_Auto");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Length);
        lua_setfield(L, -2, "Length");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Percent);
        lua_setfield(L, -2, "Percent");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Auto);
        lua_setfield(L, -2, "Auto");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from_percent);
        lua_setfield(L, -2, "from_percent");

        lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from);
        lua_setfield(L, -2, "from");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto_namespace);
}

/* -------------------------------------------------------------------------- */
/* Dimension */

static const char LUA_META_OBJECT_taffy_Dimension[]           = "taffy_Dimension_mt";
static const char LUA_META_OBJECT_taffy_Dimension_namespace[] = "taffy_Dimension_namespace_mt";

static int lua_taffy_Dimension_Length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_Dimension* object_ptr = taffy_Dimension_new_Length(value);
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_Length() failed");
    }
}

static int lua_taffy_Dimension_Percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_Dimension* object_ptr = taffy_Dimension_new_Percent(value);
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_Percent() failed");
    }
}

static int lua_taffy_Dimension_Auto(lua_State* L)
{
    taffy_Dimension* object_ptr = taffy_Dimension_new_Auto();
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_Auto() failed");
    }
}

static int lua_taffy_Dimension_copy(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    taffy_Dimension* copy = taffy_Dimension_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Dimension : taffy_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Dimension_delete(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    taffy_Dimension_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Dimension_eq(lua_State* L)
{
    taffy_Dimension** object_lhs = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);
    taffy_Dimension** object_rhs = (taffy_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Dimension);

    const int is_equal = taffy_Dimension_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */

}

static int lua_taffy_Dimension_is_Length(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Length = taffy_Dimension_is_Length(*self);

    lua_pushboolean(L, is_Length);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_is_Percent(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Percent = taffy_Dimension_is_Percent(*self);

    lua_pushboolean(L, is_Percent);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_is_Auto(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Auto = taffy_Dimension_is_Auto(*self);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_get_value(lua_State* L)
{
    taffy_Dimension** self = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    if( taffy_Dimension_is_Length (*self) ||
        taffy_Dimension_is_Percent(*self) )
    {
        const float value = taffy_Dimension_get_value(*self);

        lua_pushnumber(L, value);

        return 1; /* number of results */
    }
    else /* !Length && !Percent */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Dimension_ZERO(lua_State* L)
{
    taffy_Dimension* object_ptr = taffy_Dimension_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_ZERO() failed");
    }
}

static int lua_taffy_Dimension_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_Dimension* object_ptr = taffy_Dimension_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_from_length() failed");
    }
}

static int lua_taffy_Dimension_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_Dimension* object_ptr = taffy_Dimension_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_from_percent() failed");
    }
}

static int lua_taffy_Dimension_from(lua_State* L)
{
    /* Try to construct 'Dimension' from 'LengthPercentage' */
    {
        taffy_LengthPercentage** length_percentage = luaL_testudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);
        if(length_percentage != NULL)
        {
            taffy_Dimension* object_ptr = taffy_Dimension_new_from_LengthPercentage(*length_percentage);
            if(object_ptr != NULL)
            {
                taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
                *user_data = object_ptr;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

                return 1; /* number of results */
            }
            else
            {
                return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_from_LengthPercentage() failed");
            }
        }
    }

    /* Try to construct 'Dimension' from 'LengthPercentageAuto' */
    {
        taffy_LengthPercentageAuto** length_percentage_auto = luaL_testudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
        if(length_percentage_auto != NULL)
        {
            taffy_Dimension* object_ptr = taffy_Dimension_new_from_LengthPercentageAuto(*length_percentage_auto);
            if(object_ptr != NULL)
            {
                taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
                *user_data = object_ptr;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

                return 1; /* number of results */
            }
            else
            {
                return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_from_LengthPercentageAuto() failed");
            }
        }
    }

    return luaL_error(L, "Failed to create taffy_Dimension : 'from()' failed - invalid argument");
}

static void lua_push_table_taffy_Dimension(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Dimension) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Dimension_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Dimension_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Dimension_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Dimension_is_Length);
        lua_setfield(L, -2, "is_Length");

        lua_pushcfunction(L, lua_taffy_Dimension_is_Percent);
        lua_setfield(L, -2, "is_Percent");

        lua_pushcfunction(L, lua_taffy_Dimension_is_Auto);
        lua_setfield(L, -2, "is_Auto");

        lua_pushcfunction(L, lua_taffy_Dimension_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Dimension_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Dimension_Length);
        lua_setfield(L, -2, "Length");

        lua_pushcfunction(L, lua_taffy_Dimension_Percent);
        lua_setfield(L, -2, "Percent");

        lua_pushcfunction(L, lua_taffy_Dimension_Auto);
        lua_setfield(L, -2, "Auto");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Dimension_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_Dimension_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_Dimension_from_percent);
        lua_setfield(L, -2, "from_percent");

        lua_pushcfunction(L, lua_taffy_Dimension_from);
        lua_setfield(L, -2, "from");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension_namespace);
}

/* -------------------------------------------------------------------------- */
/* FlexDirection (enum) */

static const char LUA_META_OBJECT_taffy_FlexDirection[]           = "taffy_FlexDirection_mt";
static const char LUA_META_OBJECT_taffy_FlexDirection_namespace[] = "taffy_FlexDirection_namespace_mt";

static int lua_taffy_FlexDirection_eq(lua_State* L)
{
    taffy_FlexDirection* self = (taffy_FlexDirection*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_FlexDirection);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_FlexDirection* other = (taffy_FlexDirection*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_FlexDirection);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_FlexDirection with other value - invalid type");
}

static int lua_taffy_FlexDirection_to_int(lua_State* L)
{
    taffy_FlexDirection* self = (taffy_FlexDirection*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_FlexDirection);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_FlexDirection_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_FlexDirection_Row           :
    case taffy_FlexDirection_Column        :
    case taffy_FlexDirection_RowReverse    :
    case taffy_FlexDirection_ColumnReverse :
    {
        taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
        *object = (taffy_FlexDirection) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_FlexDirection(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_FlexDirection) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_FlexDirection_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_FlexDirection_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_FlexDirection_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_FlexDirection_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
            *object = taffy_FlexDirection_Row;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);
        }
        lua_setfield(L, -2, "Row");

        {
            taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
            *object = taffy_FlexDirection_Column;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);
        }
        lua_setfield(L, -2, "Column");

        {
            taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
            *object = taffy_FlexDirection_RowReverse;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);
        }
        lua_setfield(L, -2, "RowReverse");

        {
            taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
            *object = taffy_FlexDirection_ColumnReverse;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);
        }
        lua_setfield(L, -2, "ColumnReverse");

        {
            taffy_FlexDirection* object = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
            *object = taffy_FlexDirection_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection_namespace);
}

/* -------------------------------------------------------------------------- */
/* FlexWrap (enum) */

static const char LUA_META_OBJECT_taffy_FlexWrap[]           = "taffy_FlexWrap_mt";
static const char LUA_META_OBJECT_taffy_FlexWrap_namespace[] = "taffy_FlexWrap_namespace_mt";

static int lua_taffy_FlexWrap_eq(lua_State* L)
{
    taffy_FlexWrap* self = (taffy_FlexWrap*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_FlexWrap);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_FlexWrap* other = (taffy_FlexWrap*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_FlexWrap);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_FlexWrap with other value - invalid type");
}

static int lua_taffy_FlexWrap_to_int(lua_State* L)
{
    taffy_FlexWrap* self = (taffy_FlexWrap*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_FlexWrap);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_FlexWrap_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_FlexWrap_NoWrap      :
    case taffy_FlexWrap_Wrap        :
    case taffy_FlexWrap_WrapReverse :
    {
        taffy_FlexWrap* object = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
        *object = (taffy_FlexWrap) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_FlexWrap(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_FlexWrap) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_FlexWrap_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_FlexWrap_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_FlexWrap_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_FlexWrap_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_FlexWrap* object = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
            *object = taffy_FlexWrap_NoWrap;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);
        }
        lua_setfield(L, -2, "NoWrap");

        {
            taffy_FlexWrap* object = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
            *object = taffy_FlexWrap_Wrap;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);
        }
        lua_setfield(L, -2, "Wrap");

        {
            taffy_FlexWrap* object = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
            *object = taffy_FlexWrap_WrapReverse;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);
        }
        lua_setfield(L, -2, "WrapReverse");

        {
            taffy_FlexWrap* object = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
            *object = taffy_FlexWrap_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap_namespace);
}

/* -------------------------------------------------------------------------- */
/* GridAutoFlow (enum) */

static const char LUA_META_OBJECT_taffy_GridAutoFlow[]           = "taffy_GridAutoFlow_mt";
static const char LUA_META_OBJECT_taffy_GridAutoFlow_namespace[] = "taffy_GridAutoFlow_namespace_mt";

static int lua_taffy_GridAutoFlow_eq(lua_State* L)
{
    taffy_GridAutoFlow* self = (taffy_GridAutoFlow*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridAutoFlow);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_GridAutoFlow* other = (taffy_GridAutoFlow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridAutoFlow);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_GridAutoFlow with other value - invalid type");
}

static int lua_taffy_GridAutoFlow_to_int(lua_State* L)
{
    taffy_GridAutoFlow* self = (taffy_GridAutoFlow*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridAutoFlow);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_GridAutoFlow_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_GridAutoFlow_Row         :
    case taffy_GridAutoFlow_Column      :
    case taffy_GridAutoFlow_RowDense    :
    case taffy_GridAutoFlow_ColumnDense :
    {
        taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
        *object = (taffy_GridAutoFlow) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_GridAutoFlow(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_GridAutoFlow_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridAutoFlow_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridAutoFlow_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
            *object = taffy_GridAutoFlow_Row;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);
        }
        lua_setfield(L, -2, "Row");

        {
            taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
            *object = taffy_GridAutoFlow_Column;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);
        }
        lua_setfield(L, -2, "Column");

        {
            taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
            *object = taffy_GridAutoFlow_RowDense;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);
        }
        lua_setfield(L, -2, "RowDense");

        {
            taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
            *object = taffy_GridAutoFlow_ColumnDense;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);
        }
        lua_setfield(L, -2, "ColumnDense");

        {
            taffy_GridAutoFlow* object = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
            *object = taffy_GridAutoFlow_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow_namespace);
}

/* -------------------------------------------------------------------------- */
/* GridPlacement */

static const char LUA_META_OBJECT_taffy_GridPlacement[]           = "taffy_GridPlacement_mt";
static const char LUA_META_OBJECT_taffy_GridPlacement_namespace[] = "taffy_GridPlacement_namespace_mt";

static int lua_taffy_GridPlacement_new(lua_State* L)
{
    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_default();
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_default() failed");
    }
}

static int lua_taffy_GridPlacement_Auto(lua_State* L)
{
    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_Auto();
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_Auto() failed");
    }
}

static int lua_taffy_GridPlacement_Line(lua_State* L)
{
    const lua_Integer value = luaL_checkinteger(L, 1);

    /* TODO: add check is value is 'int16_t' & warning */

    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_Line( (int16_t)value );
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_Line() failed");
    }
}

static int lua_taffy_GridPlacement_Span(lua_State* L)
{
    const lua_Integer value = luaL_checkinteger(L, 1);

    /* TODO: add check is value is 'uint16_t' & warning */

    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_Span( (uint16_t)value );
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_Span() failed");
    }
}

static int lua_taffy_GridPlacement_copy(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(*self);

    if(copy != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_GridPlacement : taffy_GridPlacement_new_copy() failed");
    }
}

static int lua_taffy_GridPlacement_delete(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_GridPlacement_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_GridPlacement_eq(lua_State* L)
{
    taffy_GridPlacement** object_lhs = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);
    taffy_GridPlacement** object_rhs = (taffy_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_equal = taffy_GridPlacement_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_is_Auto(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Auto = taffy_GridPlacement_is_Auto(*self);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_is_Line(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Line = taffy_GridPlacement_is_Line(*self);

    lua_pushboolean(L, is_Line);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_is_Span(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Span = taffy_GridPlacement_is_Span(*self);

    lua_pushboolean(L, is_Span);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_get_line(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    if( taffy_GridPlacement_is_Line(*self) )
    {
        const int16_t line = taffy_GridPlacement_get_line(*self);

        lua_pushinteger(L, line);

        return 1;
    }
    else /* !Line */
    {
        lua_pushnil(L);

        return 1;
    }
}

static int lua_taffy_GridPlacement_get_span(lua_State* L)
{
    taffy_GridPlacement** self = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    if( taffy_GridPlacement_is_Span(*self) )
    {
        const uint16_t span = taffy_GridPlacement_get_span(*self);

        lua_pushinteger(L, span);

        return 1;
    }
    else /* !Span */
    {
        lua_pushnil(L);

        return 1;
    }
}

static int lua_taffy_GridPlacement_AUTO(lua_State* L)
{
    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_AUTO() failed");
    }
}

static int lua_taffy_GridPlacement_from_line_index(lua_State* L)
{
    const lua_Integer value = luaL_checkinteger(L, 1);

    /* TODO: add check is value is 'int16_t' & warning */

    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_from_line_index( (int16_t)value );
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_from_line_index() failed");
    }
}

static int lua_taffy_GridPlacement_from_span(lua_State* L)
{
    const lua_Integer value = luaL_checkinteger(L, 1);

    /* TODO: add check is value is 'uint16_t' & warning */

    taffy_GridPlacement* object_ptr = taffy_GridPlacement_new_from_span( (uint16_t)value );
    if(object_ptr != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridPlacement : taffy_GridPlacement_new_from_line_index() failed");
    }
}

static void lua_push_table_taffy_GridPlacement(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridPlacement) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_GridPlacement_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_GridPlacement_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridPlacement_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_GridPlacement_is_Auto);
        lua_setfield(L, -2, "is_Auto");

        lua_pushcfunction(L, lua_taffy_GridPlacement_is_Line);
        lua_setfield(L, -2, "is_Line");

        lua_pushcfunction(L, lua_taffy_GridPlacement_is_Span);
        lua_setfield(L, -2, "is_Span");

        lua_pushcfunction(L, lua_taffy_GridPlacement_get_line);
        lua_setfield(L, -2, "get_line");

        lua_pushcfunction(L, lua_taffy_GridPlacement_get_span);
        lua_setfield(L, -2, "get_span");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridPlacement_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridPlacement_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_GridPlacement_Auto);
        lua_setfield(L, -2, "Auto");

        lua_pushcfunction(L, lua_taffy_GridPlacement_Line);
        lua_setfield(L, -2, "Line");

        lua_pushcfunction(L, lua_taffy_GridPlacement_Span);
        lua_setfield(L, -2, "Span");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridPlacement_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_GridPlacement_from_line_index);
        lua_setfield(L, -2, "from_line_index");

        lua_pushcfunction(L, lua_taffy_GridPlacement_from_span);
        lua_setfield(L, -2, "from_span");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement_namespace);
}

/* -------------------------------------------------------------------------- */
/* GridTrackRepetition */

static const char LUA_META_OBJECT_taffy_GridTrackRepetition[]           = "taffy_GridTrackRepetition_mt";
static const char LUA_META_OBJECT_taffy_GridTrackRepetition_namespace[] = "taffy_GridTrackRepetition_namespace_mt";

static int lua_taffy_GridTrackRepetition_AutoFill(lua_State* L)
{
    taffy_GridTrackRepetition* object_ptr = taffy_GridTrackRepetition_new_AutoFill();
    if(object_ptr != NULL)
    {
        taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridTrackRepetition : taffy_GridTrackRepetition_new_AutoFill() failed");
    }
}

static int lua_taffy_GridTrackRepetition_AutoFit(lua_State* L)
{
    taffy_GridTrackRepetition* object_ptr = taffy_GridTrackRepetition_new_AutoFit();
    if(object_ptr != NULL)
    {
        taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridTrackRepetition : taffy_GridTrackRepetition_new_AutoFit() failed");
    }
}

static int lua_taffy_GridTrackRepetition_Count(lua_State* L)
{
    const lua_Integer value = luaL_checkinteger(L, 1);

    /* TODO: add check is value is 'uint16_t' & warning */

    taffy_GridTrackRepetition* object_ptr = taffy_GridTrackRepetition_new_Count( (uint16_t)value );
    if(object_ptr != NULL)
    {
        taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_GridTrackRepetition : taffy_GridTrackRepetition_new_Count() failed");
    }
}

static int lua_taffy_GridTrackRepetition_copy(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    taffy_GridTrackRepetition* copy = taffy_GridTrackRepetition_new_copy(*self);

    if(copy != NULL)
    {
        taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_GridTrackRepetition : taffy_GridTrackRepetition_new_copy() failed");
    }
}

static int lua_taffy_GridTrackRepetition_delete(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    taffy_GridTrackRepetition_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_GridTrackRepetition_eq(lua_State* L)
{
    taffy_GridTrackRepetition** object_lhs = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);
    taffy_GridTrackRepetition** object_rhs = (taffy_GridTrackRepetition**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_equal = taffy_GridTrackRepetition_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_is_AutoFill(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_AutoFill = taffy_GridTrackRepetition_is_AutoFill(*self);

    lua_pushboolean(L, is_AutoFill);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_is_AutoFit(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_AutoFit = taffy_GridTrackRepetition_is_AutoFit(*self);

    lua_pushboolean(L, is_AutoFit);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_is_Count(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_Count = taffy_GridTrackRepetition_is_Count(*self);

    lua_pushboolean(L, is_Count);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_get_value(lua_State* L)
{
    taffy_GridTrackRepetition** self = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    if( taffy_GridTrackRepetition_is_Count(*self) )
    {
        const uint16_t value = taffy_GridTrackRepetition_get_value(*self);

        lua_pushinteger(L, value);

        return 1; /* number of results */
    }
    else /* !Count */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_GridTrackRepetition_try_from(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer value = luaL_checkinteger(L, 1);

        /* TODO: add check is value is 'uint16_t' & warning */

        taffy_GridTrackRepetition* object_ptr = taffy_GridTrackRepetition_try_from_u16( (uint16_t)value );
        if(object_ptr != NULL)
        {
            taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

            return 1; /* number of results */
        }
        else /* 'try_from_u16' failed - dont raise error, simply return 'nil' */
        {
            lua_pushnil(L);

            return 1; /* number of results */
        }

    } break;

    case LUA_TSTRING:
    {
        const char* str = lua_tostring(L, 1);

        taffy_GridTrackRepetition* object_ptr = taffy_GridTrackRepetition_try_from_str( str );
        if(object_ptr != NULL)
        {
            taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

            return 1; /* number of results */
        }
        else /* 'try_from_str' failed - dont raise error, simply return 'nil' */
        {
            lua_pushnil(L);

            return 1; /* number of results */
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_GridTrackRepetition : invalid argument type");
}

static void lua_push_table_taffy_GridTrackRepetition(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_is_AutoFill);
        lua_setfield(L, -2, "is_AutoFill");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_is_AutoFit);
        lua_setfield(L, -2, "is_AutoFit");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_is_Count);
        lua_setfield(L, -2, "is_Count");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_AutoFill);
        lua_setfield(L, -2, "AutoFill");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_AutoFit);
        lua_setfield(L, -2, "AutoFit");

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_Count);
        lua_setfield(L, -2, "Count");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_GridTrackRepetition_try_from);
        lua_setfield(L, -2, "try_from");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition_namespace);
}

/* -------------------------------------------------------------------------- */
/* MaxTrackSizingFunction */

static const char LUA_META_OBJECT_taffy_MaxTrackSizingFunction[]           = "taffy_MaxTrackSizingFunction_mt";
static const char LUA_META_OBJECT_taffy_MaxTrackSizingFunction_namespace[] = "taffy_MaxTrackSizingFunction_namespace_mt";

static int lua_taffy_MaxTrackSizingFunction_Fixed(lua_State* L)
{
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_Fixed(*value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_Fixed() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_MinContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_MinContent();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_MinContent() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_MaxContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_MaxContent();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_Auto() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_FitContent(lua_State* L)
{
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_FitContent(*value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_FitContent() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_Auto(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_Auto();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_Auto() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_Fraction(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_Fraction(value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_Fraction() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_copy(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    taffy_MaxTrackSizingFunction* copy = taffy_MaxTrackSizingFunction_new_copy(*self);

    if(copy != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_delete(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    taffy_MaxTrackSizingFunction_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_eq(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object_lhs = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);
    taffy_MaxTrackSizingFunction** object_rhs = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_equal = taffy_MaxTrackSizingFunction_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_Fixed(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Fixed = taffy_MaxTrackSizingFunction_is_Fixed(*self);

    lua_pushboolean(L, is_Fixed);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_MinContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_MinContent = taffy_MaxTrackSizingFunction_is_MinContent(*self);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_MaxContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_MaxContent = taffy_MaxTrackSizingFunction_is_MaxContent(*self);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_FitContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_FitContent = taffy_MaxTrackSizingFunction_is_FitContent(*self);

    lua_pushboolean(L, is_FitContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_Auto(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Auto = taffy_MaxTrackSizingFunction_is_Auto(*self);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_Fraction(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Fraction = taffy_MaxTrackSizingFunction_is_Fraction(*self);

    lua_pushboolean(L, is_Fraction);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_get_length_percentage(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    if( taffy_MaxTrackSizingFunction_is_Fixed     (*self) ||
        taffy_MaxTrackSizingFunction_is_FitContent(*self) )
    {
        taffy_LengthPercentage* object_ptr = taffy_MaxTrackSizingFunction_get_new_length_percentage(*self);
        if(object_ptr != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_MaxTrackSizingFunction_get_new_length_percentage() failed");
        }
    }
    else /* !Fixed && !FitContent */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_MaxTrackSizingFunction_get_fraction(lua_State* L)
{
    taffy_MaxTrackSizingFunction** self = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    if( taffy_MaxTrackSizingFunction_is_Fraction(*self) )
    {
        const float fraction = taffy_MaxTrackSizingFunction_get_fraction(*self);

        lua_pushnumber(L, fraction);

        return 1; /* number of results */
    }
    else /* !Fraction */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_MaxTrackSizingFunction_AUTO(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_AUTO() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_MIN_CONTENT(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_MIN_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_MIN_CONTENT() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_MAX_CONTENT(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_MAX_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_MAX_CONTENT() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_ZERO(lua_State* L)
{
    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_ZERO() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_fit_content(lua_State* L)
{
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_fit_content(*value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_fit_content() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_from_length() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_from_percent() failed");
    }
}

static int lua_taffy_MaxTrackSizingFunction_from_flex(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MaxTrackSizingFunction* object_ptr = taffy_MaxTrackSizingFunction_new_from_flex(value);
    if(object_ptr != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_from_flex() failed");
    }
}

static void lua_push_table_taffy_MaxTrackSizingFunction(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_Fixed);
        lua_setfield(L, -2, "is_Fixed");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_MinContent);
        lua_setfield(L, -2, "is_MinContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_MaxContent);
        lua_setfield(L, -2, "is_MaxContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_FitContent);
        lua_setfield(L, -2, "is_FitContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_Auto);
        lua_setfield(L, -2, "is_Auto");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_is_Fraction);
        lua_setfield(L, -2, "is_Fraction");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_get_length_percentage);
        lua_setfield(L, -2, "get_length_percentage");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_get_fraction);
        lua_setfield(L, -2, "get_fraction");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_Fixed);
        lua_setfield(L, -2, "Fixed");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_MinContent);
        lua_setfield(L, -2, "MinContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_MaxContent);
        lua_setfield(L, -2, "MaxContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_FitContent);
        lua_setfield(L, -2, "FitContent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_Auto);
        lua_setfield(L, -2, "Auto");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_Fraction);
        lua_setfield(L, -2, "Fraction");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_MIN_CONTENT);
        lua_setfield(L, -2, "MIN_CONTENT");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_MAX_CONTENT);
        lua_setfield(L, -2, "MAX_CONTENT");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_fit_content);
        lua_setfield(L, -2, "fit_content");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_from_percent);
        lua_setfield(L, -2, "from_percent");

        lua_pushcfunction(L, lua_taffy_MaxTrackSizingFunction_from_flex);
        lua_setfield(L, -2, "from_flex");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction_namespace);
}

/* -------------------------------------------------------------------------- */
/* MinTrackSizingFunction */

static const char LUA_META_OBJECT_taffy_MinTrackSizingFunction[]           = "taffy_MinTrackSizingFunction_mt";
static const char LUA_META_OBJECT_taffy_MinTrackSizingFunction_namespace[] = "taffy_MinTrackSizingFunction_namespace_mt";

static int lua_taffy_MinTrackSizingFunction_Fixed(lua_State* L)
{
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_Fixed(*value);
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_Fixed() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_MinContent(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_MinContent();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_MinContent() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_MaxContent(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_MaxContent();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_MaxContent() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_Auto(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_Auto();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_Auto() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_copy(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    taffy_MinTrackSizingFunction* copy = taffy_MinTrackSizingFunction_new_copy(*self);

    if(copy != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_delete(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    taffy_MinTrackSizingFunction_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_eq(lua_State* L)
{
    taffy_MinTrackSizingFunction** object_lhs = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);
    taffy_MinTrackSizingFunction** object_rhs = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_equal = taffy_MinTrackSizingFunction_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_Fixed(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_Fixed = taffy_MinTrackSizingFunction_is_Fixed(*self);

    lua_pushboolean(L, is_Fixed);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_MinContent(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_MinContent = taffy_MinTrackSizingFunction_is_MinContent(*self);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_MaxContent(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_MaxContent = taffy_MinTrackSizingFunction_is_MaxContent(*self);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_Auto(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_Auto = taffy_MinTrackSizingFunction_is_Auto(*self);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_get_value(lua_State* L)
{
    taffy_MinTrackSizingFunction** self = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    if( taffy_MinTrackSizingFunction_is_Fixed(*self) )
    {
        taffy_LengthPercentage* object_ptr = taffy_MinTrackSizingFunction_get_new_value(*self);
        if(object_ptr != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_MinTrackSizingFunction_get_new_value() failed");
        }
    }
    else /* !Fixed */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_MinTrackSizingFunction_AUTO(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_AUTO() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_MIN_CONTENT(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_MIN_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_MIN_CONTENT() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_MAX_CONTENT(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_MAX_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_ZERO() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_ZERO(lua_State* L)
{
    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_ZERO() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_from_length() failed");
    }
}

static int lua_taffy_MinTrackSizingFunction_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_MinTrackSizingFunction* object_ptr = taffy_MinTrackSizingFunction_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_from_percent() failed");
    }
}

static void lua_push_table_taffy_MinTrackSizingFunction(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_is_Fixed);
        lua_setfield(L, -2, "is_Fixed");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_is_MinContent);
        lua_setfield(L, -2, "is_MinContent");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_is_MaxContent);
        lua_setfield(L, -2, "is_MaxContent");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_is_Auto);
        lua_setfield(L, -2, "is_Auto");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_get_value);
        lua_setfield(L, -2, "get_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_Fixed);
        lua_setfield(L, -2, "Fixed");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MinContent);
        lua_setfield(L, -2, "MinContent");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MaxContent);
        lua_setfield(L, -2, "MaxContent");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_Auto);
        lua_setfield(L, -2, "Auto");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MIN_CONTENT);
        lua_setfield(L, -2, "MIN_CONTENT");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MAX_CONTENT);
        lua_setfield(L, -2, "MAX_CONTENT");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_from_percent);
        lua_setfield(L, -2, "from_percent");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction_namespace);
}

/* -------------------------------------------------------------------------- */
/* NonRepeatedTrackSizingFunction */

static const char LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction[]           = "taffy_NonRepeatedTrackSizingFunction_mt";
static const char LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction_namespace[] = "taffy_NonRepeatedTrackSizingFunction_namespace_mt";

static int lua_taffy_NonRepeatedTrackSizingFunction_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    { MinTrackSizingFunction, MaxTrackSizingFunction }

                    {[1] = MinTrackSizingFunction, [2] = MaxTrackSizingFunction}
                    {[2] = MaxTrackSizingFunction, [1] = MinTrackSizingFunction}
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int min_found = 0; /* false */
                /* bool */ int max_found = 0; /* false */

                taffy_MinTrackSizingFunction** min = NULL;
                taffy_MaxTrackSizingFunction** max = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        /* try to get 'min' */
                        {
                            taffy_MinTrackSizingFunction** min_value = luaL_testudata(L, -1, LUA_META_OBJECT_taffy_MinTrackSizingFunction); /* pop 'value' */
                            if(min_value != NULL)
                            {
                                const lua_Number key_value = lua_tonumber(L, -2); /* pop 'key' */

                                if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'min' */
                                {
                                    min_found = 1; /* true */
                                    min = min_value;
                                }
                            }
                        }

                        /* try to get 'max' */
                        {
                            taffy_MaxTrackSizingFunction** max_value = luaL_testudata(L, -1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction); /* pop 'value' */
                            if(max_value != NULL)
                            {
                                const lua_Number key_value = lua_tonumber(L, -2); /* pop 'key' */

                                if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'max' */
                                {
                                    max_found = 1; /* true */
                                    max = max_value;
                                }
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (min_found == /* true */ 1) && (max_found == /* true */ 1) )
                {
                    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new(*min, *max);
                    if(object_ptr != NULL)
                    {
                        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {min = MinTrackSizingFunction, max = MaxTrackSizingFunction}

                if table size != 2 OR 'min' and 'max' not in indexes '1' and '2'
            */
            {
                /* bool */ int min_found = 0; /* false */
                /* bool */ int max_found = 0; /* false */

                taffy_MinTrackSizingFunction** min = NULL;
                taffy_MaxTrackSizingFunction** max = NULL;

                /* Try to get 'min' */
                {
                    const int min_type = lua_getfield(L, 1, "min");
                    if(min_type == LUA_TUSERDATA)
                    {
                        taffy_MinTrackSizingFunction** min_value = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, -1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

                        min_found = 1; /* true */
                        min = min_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'max' */
                {
                    const int max_type = lua_getfield(L, 1, "max");
                    if(max_type == LUA_TUSERDATA)
                    {
                        taffy_MaxTrackSizingFunction** max_value = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, -1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

                        max_found = 1; /* true */
                        max = max_value;
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (min_found == /* true */ 1) && (max_found == /* true */ 1) )
                {
                    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new(*min, *max);
                    if(object_ptr != NULL)
                    {
                        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : provided argument is not a table");
        }
    } break;

    case 2:
    {
        taffy_MinTrackSizingFunction** min = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);
        taffy_MaxTrackSizingFunction** max = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new(*min, *max);
        if(object_ptr != NULL)
        {
            taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : wrong arguments count");
}

static int lua_taffy_NonRepeatedTrackSizingFunction_copy(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction* copy = taffy_NonRepeatedTrackSizingFunction_new_copy(*self);

    if(copy != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_delete(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_NonRepeatedTrackSizingFunction_eq(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** object_lhs = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);
    taffy_NonRepeatedTrackSizingFunction** object_rhs = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    const int is_equal = taffy_NonRepeatedTrackSizingFunction_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_NonRepeatedTrackSizingFunction_get_min(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    const taffy_MinTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_get_min(*self);

    taffy_MinTrackSizingFunction* copy = taffy_MinTrackSizingFunction_new_copy(object_ptr);

    if(copy != NULL)
    {
        taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_get_max(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    const taffy_MaxTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_get_max(*self);

    taffy_MaxTrackSizingFunction* copy = taffy_MaxTrackSizingFunction_new_copy(object_ptr);

    if(copy != NULL)
    {
        taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_set_min(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_MinTrackSizingFunction** value = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction_set_min(*self, *value);

    return 0; /* number of results */
}

static int lua_taffy_NonRepeatedTrackSizingFunction_set_max(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_MaxTrackSizingFunction** value = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction_set_max(*self, *value);

    return 0; /* number of results */
}

static int lua_taffy_NonRepeatedTrackSizingFunction_AUTO(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_AUTO() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_MIN_CONTENT(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_MIN_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_MIN_CONTENT() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_MAX_CONTENT(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_MAX_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_MAX_CONTENT() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_fit_content(lua_State* L)
{
    taffy_LengthPercentage** length_percentage = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_fit_content(*length_percentage);
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_fit_content() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_ZERO(lua_State* L)
{
    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_ZERO() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_from_length(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_from_length() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_from_percent(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_from_percent() failed");
    }
}

static int lua_taffy_NonRepeatedTrackSizingFunction_from_flex(lua_State* L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_NonRepeatedTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_new_from_flex(value);
    if(object_ptr != NULL)
    {
        taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_from_flex() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_NonRepeatedTrackSizingFunction_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "min") == 0)
    {
        const taffy_MinTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_get_min(*self);

        taffy_MinTrackSizingFunction* copy = taffy_MinTrackSizingFunction_new_copy(object_ptr);

        if(copy != NULL)
        {
            taffy_MinTrackSizingFunction** user_data = (taffy_MinTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MinTrackSizingFunction*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_MinTrackSizingFunction : taffy_MinTrackSizingFunction_new_copy() failed");
        }
    }
    else if(strcmp(key, "max") == 0)
    {
        const taffy_MaxTrackSizingFunction* object_ptr = taffy_NonRepeatedTrackSizingFunction_get_max(*self);

        taffy_MaxTrackSizingFunction* copy = taffy_MaxTrackSizingFunction_new_copy(object_ptr);

        if(copy != NULL)
        {
            taffy_MaxTrackSizingFunction** user_data = (taffy_MaxTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_MaxTrackSizingFunction*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_MaxTrackSizingFunction : taffy_MaxTrackSizingFunction_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_NonRepeatedTrackSizingFunction_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_NonRepeatedTrackSizingFunction** self = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);
    const char* key = luaL_checkstring(L, 2);

    if(strcmp(key, "min") == 0)
    {
        taffy_MinTrackSizingFunction** value = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

        taffy_NonRepeatedTrackSizingFunction_set_min(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "max") == 0)
    {
        taffy_MaxTrackSizingFunction** value = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

        taffy_NonRepeatedTrackSizingFunction_set_max(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_NonRepeatedTrackSizingFunction 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_NonRepeatedTrackSizingFunction(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction) )
    {
        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_get_min);
        lua_setfield(L, -2, "get_min");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_get_max);
        lua_setfield(L, -2, "get_max");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_set_min);
        lua_setfield(L, -2, "set_min");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_set_max);
        lua_setfield(L, -2, "set_max");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_new);
        lua_setfield(L, -2, "new");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_MIN_CONTENT);
        lua_setfield(L, -2, "MIN_CONTENT");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_MAX_CONTENT);
        lua_setfield(L, -2, "MAX_CONTENT");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_fit_content);
        lua_setfield(L, -2, "fit_content");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_from_percent);
        lua_setfield(L, -2, "from_percent");

        lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_from_flex);
        lua_setfield(L, -2, "from_flex");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction_namespace);
}

/* -------------------------------------------------------------------------- */
/* TrackSizingFunction */

/* Utility function: GridTrackVec<NonRepeatedTrackSizingFunction> --> lua table */
static void lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
    lua_State* L, const taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction* vec
)
{
    lua_newtable(L);
    {
        size_t i = 0;
        for(i = 0; i < vec->items_count; ++i)
        {
            const taffy_NonRepeatedTrackSizingFunction* func = vec->items[i];

            taffy_NonRepeatedTrackSizingFunction* copy = taffy_NonRepeatedTrackSizingFunction_new_copy(func);
            if(copy != NULL)
            {
                taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
                *user_data = copy;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

                /* set: table[index] = 'value' */
                lua_rawseti(L, -2, (i+1)); /* NOTE: in Lua indexes starts from 1, thats why here: 'Lua-index = C-index + 1;' */
            }
        }
    }
}

/*
    Utility function: lua table --> GridTrackVec<NonRepeatedTrackSizingFunction>

    ATTENTION: after use - dont forget to 'free()' memory, allocated by 'malloc()':

        if(items != NULL)
        {
            free(items);
            items = NULL;
        }
*/
static void lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
    lua_State* L, int table_index,

    const taffy_NonRepeatedTrackSizingFunction** * out_items,
    size_t * out_items_count
)
{
    const taffy_NonRepeatedTrackSizingFunction** items = NULL;
    size_t items_count = 0;

    const size_t table_size = lua_rawlen(L, table_index);
    if(table_size > 0)
    {
        size_t index = 0;

        /* Allocate array with pointers into 'NonRepeatedTrackSizingFunction's */
        items = (const taffy_NonRepeatedTrackSizingFunction**)malloc(table_size * sizeof(taffy_NonRepeatedTrackSizingFunction*));
        items_count = table_size;


        /* Iterate table */
        lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
        while( lua_next(L, table_index) != 0 )
        {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            /* const int value_type = lua_type(L, -1); */
            /* const int key_type   = lua_type(L, -2); */

            /*
                NOTE: here (for simplicity) we dont care about 'key'
                type and order - table may be array, sparced array,
                dictionary, anything, BUT its 'values' must be only
                'taffy_NonRepeatedTrackSizingFunction' type.
            */
            taffy_NonRepeatedTrackSizingFunction** value_value = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, -1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

            items[index] = *value_value;

            index += 1;

            /* pop 'value' from the stack*/
            lua_pop(L, 1);
        }
        lua_pop(L, 1); /* pop 'key' from the stack */
    }

    /* return */
    *out_items       = items;
    *out_items_count = items_count;
}

static const char LUA_META_OBJECT_taffy_TrackSizingFunction[]           = "taffy_TrackSizingFunction_mt";
static const char LUA_META_OBJECT_taffy_TrackSizingFunction_namespace[] = "taffy_TrackSizingFunction_namespace_mt";

static int lua_taffy_TrackSizingFunction_Single(lua_State *L)
{
    taffy_NonRepeatedTrackSizingFunction** value = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_Single(*value);
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_Single() failed");
    }
}

static int lua_taffy_TrackSizingFunction_Repeat(lua_State *L)
{
    taffy_GridTrackRepetition** repetition = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const taffy_NonRepeatedTrackSizingFunction** items = NULL;
    size_t items_count = 0;

    taffy_TrackSizingFunction* object_ptr = NULL;

    /* Interpret table - extract pointers to items */
    if( lua_type(L, 2) == LUA_TTABLE )
    {
        lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
            L, 2,

            &items, &items_count
        );
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
    }

    object_ptr = taffy_TrackSizingFunction_new_Repeat(*repetition, items, items_count);

    /* Dont forget to 'free()' memory, allocated by 'malloc()' */
    if(items != NULL)
    {
        free(items);
        items = NULL;
    }


    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_Repeat() failed");
    }
}

static int lua_taffy_TrackSizingFunction_copy(lua_State *L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    taffy_TrackSizingFunction* copy = taffy_TrackSizingFunction_new_copy(*self);

    if(copy != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_copy() failed");
    }
}

static int lua_taffy_TrackSizingFunction_delete(lua_State *L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    taffy_TrackSizingFunction_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_TrackSizingFunction_eq(lua_State* L)
{
    taffy_TrackSizingFunction** object_lhs = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);
    taffy_TrackSizingFunction** object_rhs = (taffy_TrackSizingFunction**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_TrackSizingFunction);

    const int is_equal = taffy_TrackSizingFunction_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_TrackSizingFunction_is_Single(lua_State* L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    const int is_Single = taffy_TrackSizingFunction_is_Single(*self);

    lua_pushboolean(L, is_Single);

    return 1; /* number of results */
}

static int lua_taffy_TrackSizingFunction_is_Repeat(lua_State* L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    const int is_Repeat = taffy_TrackSizingFunction_is_Repeat(*self);

    lua_pushboolean(L, is_Repeat);

    return 1; /* number of results */
}

static int lua_taffy_TrackSizingFunction_get_single_func(lua_State* L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Single(*self) )
    {
        const taffy_NonRepeatedTrackSizingFunction* single_func = taffy_TrackSizingFunction_get_single_func(*self);
        if(single_func != NULL)
        {
            taffy_NonRepeatedTrackSizingFunction* copy = taffy_NonRepeatedTrackSizingFunction_new_copy(single_func);
            if(copy != NULL)
            {
                taffy_NonRepeatedTrackSizingFunction** user_data = (taffy_NonRepeatedTrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_NonRepeatedTrackSizingFunction*));
                *user_data = copy;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

                return 1; /* number of results */
            }
            else
            {
                return luaL_error(L, "Failed to copy taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_new_copy() failed");
            }
        }
        else
        {
            return luaL_error(L, "Failed to get taffy_NonRepeatedTrackSizingFunction : taffy_NonRepeatedTrackSizingFunction_get_single_func() failed");
        }
    }
    else /* !Single */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_TrackSizingFunction_get_repetition(lua_State* L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Repeat(*self) )
    {
        const taffy_GridTrackRepetition* repetition = taffy_TrackSizingFunction_get_repetition(*self);
        if(repetition != NULL)
        {
            taffy_GridTrackRepetition* copy = taffy_GridTrackRepetition_new_copy(repetition);
            if(copy != NULL)
            {
                taffy_GridTrackRepetition** user_data = (taffy_GridTrackRepetition**)lua_newuserdata(L, sizeof(taffy_GridTrackRepetition*));
                *user_data = copy;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);

                return 1; /* number of results */
            }
            else
            {
                return luaL_error(L, "Failed to copy taffy_GridTrackRepetition : taffy_GridTrackRepetition_new_copy() failed");
            }
        }
        else
        {
            return luaL_error(L, "Failed to get taffy_GridTrackRepetition : taffy_NonRepeatedTrackSizingFunction_get_repetition() failed");
        }
    }
    else /* !Repeat */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_TrackSizingFunction_get_repeat_funcs(lua_State* L)
{
    taffy_TrackSizingFunction** self = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Repeat(*self) )
    {
        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction repeat_funcs = taffy_TrackSizingFunction_get_repeat_funcs(*self);

        lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(L, &repeat_funcs);

        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction_delete(&repeat_funcs);

        return 1; /* number of results */

    }
    else /* !Repeat */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_TrackSizingFunction_AUTO(lua_State *L)
{
    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_AUTO() failed");
    }
}

static int lua_taffy_TrackSizingFunction_MIN_CONTENT(lua_State *L)
{
    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_MIN_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_MIN_CONTENT() failed");
    }
}

static int lua_taffy_TrackSizingFunction_MAX_CONTENT(lua_State *L)
{
    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_MAX_CONTENT();
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_MAX_CONTENT() failed");
    }
}

static int lua_taffy_TrackSizingFunction_ZERO(lua_State *L)
{
    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_ZERO() failed");
    }
}

static int lua_taffy_TrackSizingFunction_fit_content(lua_State *L)
{
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_fit_content(*value);
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_fit_content() failed");
    }
}

static int lua_taffy_TrackSizingFunction_from_length(lua_State *L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_from_length(value);
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_from_length() failed");
    }
}

static int lua_taffy_TrackSizingFunction_from_percent(lua_State *L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_from_percent(value);
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_from_percent() failed");
    }
}

static int lua_taffy_TrackSizingFunction_from_flex(lua_State *L)
{
    const lua_Number value = luaL_checknumber(L, 1);

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_from_flex(value);
    if(object_ptr != NULL)
    {
        taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_TrackSizingFunction : taffy_TrackSizingFunction_new_from_flex() failed");
    }
}

static void lua_push_table_taffy_TrackSizingFunction(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_is_Single);
        lua_setfield(L, -2, "is_Single");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_is_Repeat);
        lua_setfield(L, -2, "is_Repeat");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_get_single_func);
        lua_setfield(L, -2, "get_single_func");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_get_repetition);
        lua_setfield(L, -2, "get_repetition");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_get_repeat_funcs);
        lua_setfield(L, -2, "get_repeat_funcs");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_Single);
        lua_setfield(L, -2, "Single");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_Repeat);
        lua_setfield(L, -2, "Repeat");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_MIN_CONTENT);
        lua_setfield(L, -2, "MIN_CONTENT");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_MAX_CONTENT);
        lua_setfield(L, -2, "MAX_CONTENT");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_ZERO);
        lua_setfield(L, -2, "ZERO");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_fit_content);
        lua_setfield(L, -2, "fit_content");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_from_length);
        lua_setfield(L, -2, "from_length");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_from_percent);
        lua_setfield(L, -2, "from_percent");

        lua_pushcfunction(L, lua_taffy_TrackSizingFunction_from_flex);
        lua_setfield(L, -2, "from_flex");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction_namespace);
}

/* -------------------------------------------------------------------------- */
/* Display (enum) */

static const char LUA_META_OBJECT_taffy_Display[]           = "taffy_Display_mt";
static const char LUA_META_OBJECT_taffy_Display_namespace[] = "taffy_Display_namespace_mt";

static int lua_taffy_Display_eq(lua_State* L)
{
    taffy_Display* self = (taffy_Display*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Display);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_Display* other = (taffy_Display*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Display);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_Display with other value - invalid type");
}

static int lua_taffy_Display_to_int(lua_State* L)
{
    taffy_Display* self = (taffy_Display*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Display);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_Display_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_Display_Flex  :
    case taffy_Display_Grid  :
    case taffy_Display_Block :
    case taffy_Display_None  :
    {
        taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
        *object = (taffy_Display) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_Display(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Display) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Display_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Display_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Display_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Display_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
            *object = taffy_Display_Flex;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);
        }
        lua_setfield(L, -2, "Flex");

        {
            taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
            *object = taffy_Display_Grid;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);
        }
        lua_setfield(L, -2, "Grid");

        {
            taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
            *object = taffy_Display_Block;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);
        }
        lua_setfield(L, -2, "Block");

        {
            taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
            *object = taffy_Display_None;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);
        }
        lua_setfield(L, -2, "None");

        {
            taffy_Display* object = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
            *object = taffy_Display_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display_namespace);
}

/* -------------------------------------------------------------------------- */
/* Overflow (enum) */

static const char LUA_META_OBJECT_taffy_Overflow[]           = "taffy_Overflow_mt";
static const char LUA_META_OBJECT_taffy_Overflow_namespace[] = "taffy_Overflow_namespace_mt";

static int lua_taffy_Overflow_eq(lua_State* L)
{
    taffy_Overflow* self = (taffy_Overflow*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Overflow);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_Overflow* other = (taffy_Overflow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Overflow);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_Overflow with other value - invalid type");
}

static int lua_taffy_Overflow_to_int(lua_State* L)
{
    taffy_Overflow* self = (taffy_Overflow*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Overflow);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_Overflow_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_Overflow_Visible :
    case taffy_Overflow_Hidden  :
    case taffy_Overflow_Scroll  :
    {
        taffy_Overflow* object = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
        *object = (taffy_Overflow) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_Overflow(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Overflow) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Overflow_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Overflow_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Overflow_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Overflow_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_Overflow* object = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
            *object = taffy_Overflow_Visible;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);
        }
        lua_setfield(L, -2, "Visible");

        {
            taffy_Overflow* object = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
            *object = taffy_Overflow_Hidden;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);
        }
        lua_setfield(L, -2, "Hidden");

        {
            taffy_Overflow* object = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
            *object = taffy_Overflow_Scroll;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);
        }
        lua_setfield(L, -2, "Scroll");

        {
            taffy_Overflow* object = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
            *object = taffy_Overflow_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow_namespace);
}

/* -------------------------------------------------------------------------- */
/* Position (enum) */

static const char LUA_META_OBJECT_taffy_Position[]           = "taffy_Position_mt";
static const char LUA_META_OBJECT_taffy_Position_namespace[] = "taffy_Position_namespace_mt";

static int lua_taffy_Position_eq(lua_State* L)
{
    taffy_Position* self = (taffy_Position*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Position);

    switch( lua_type(L, 2) ) {
    case LUA_TNUMBER:
    {
        const lua_Integer number = luaL_checkinteger(L, 2);

        const int is_equal = *self == number;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_Position* other = (taffy_Position*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Position);

        const int is_equal = *self == *other;

        lua_pushboolean(L, is_equal);

        return 1; /* number of results */
    } break;
    }

    return luaL_error(L, "Cannot compare taffy_Position with other value - invalid type");
}

static int lua_taffy_Position_to_int(lua_State* L)
{
    taffy_Position* self = (taffy_Position*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Position);

    lua_pushinteger(L, *self);

    return 1; /* number of results */
}

static int lua_taffy_Position_from_int(lua_State* L)
{
    const lua_Integer integer = luaL_checkinteger(L, 1);

    switch(integer) {
    case taffy_Position_Relative :
    case taffy_Position_Absolute :
    {
        taffy_Position* object = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
        *object = (taffy_Position) integer;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);

        return 1; /* number of results */
    } break;

    default:
    {
        lua_pushnil(L);

        return 1; /* number of results */
    } break;
    }
}

static void lua_push_table_taffy_Position(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Position) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Position_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Position_to_int);
        lua_setfield(L, -2, "to_int");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Position_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Position_from_int);
        lua_setfield(L, -2, "from_int");

        {
            taffy_Position* object = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
            *object = taffy_Position_Relative;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);
        }
        lua_setfield(L, -2, "Relative");

        {
            taffy_Position* object = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
            *object = taffy_Position_Absolute;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);
        }
        lua_setfield(L, -2, "Absolute");

        {
            taffy_Position* object = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
            *object = taffy_Position_default();
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);
        }
        lua_setfield(L, -2, "Default");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position_namespace);
}

/* -------------------------------------------------------------------------- */
/* Point<Overflow> */

static const char LUA_META_OBJECT_taffy_Point_of_Overflow[]           = "taffy_Point_of_Overflow_mt";
static const char LUA_META_OBJECT_taffy_Point_of_Overflow_namespace[] = "taffy_Point_of_Overflow_namespace_mt";

static int lua_taffy_Point_of_Overflow_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 0:
    {
        taffy_Point_of_Overflow* object_ptr = taffy_Point_of_Overflow_new_default();
        if(object_ptr != NULL)
        {
            taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_Overflow : taffy_Point_of_Overflow_new_default() failed");
        }
    } break;

    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {Overflow.Visible, Overflow.Hidden}

                    {[1] = Overflow.Visible, [2] = Overflow.Hidden}
                    {[2] = Overflow.Hidden, [1] = Overflow.Visible}
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int x_found = 0; /* false */
                /* bool */ int y_found = 0; /* false */

                taffy_Overflow x = 0;
                taffy_Overflow y = 0;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_Overflow* value_value = NULL;
                        lua_Number      key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_Overflow*)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Overflow); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                    /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'x' */
                        {
                            if(value_value != NULL)
                            {
                                x_found = 1; /* true */
                                x = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'y' */
                        {
                            if(value_value != NULL)
                            {
                                y_found = 1; /* true */
                                y = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (x_found == /* true */ 1) && (y_found == /* true */ 1) )
                {
                    taffy_Point_of_Overflow* object_ptr = taffy_Point_of_Overflow_new(x, y);
                    if(object_ptr != NULL)
                    {
                        taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Point_of_Overflow : taffy_Point_of_Overflow_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {x = Overflow.Visible, y = Overflow.Hidden}

                if table size != 2 OR 'x' and 'y' not in indexes '1' and '2'
            */
            {
                /* bool */ int x_found = 0; /* false */
                /* bool */ int y_found = 0; /* false */

                taffy_Overflow x = 0;
                taffy_Overflow y = 0;

                /* Try to get 'x' */
                {
                    const int x_type = lua_getfield(L, 1, "x");
                    if(x_type == LUA_TUSERDATA)
                    {
                        taffy_Overflow* x_value = (taffy_Overflow*)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Overflow);
                        if(x_value != NULL)
                        {
                            x_found = 1; /* true */
                            x = *x_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'y' */
                {
                    const int y_type = lua_getfield(L, 1, "y");
                    if(y_type == LUA_TUSERDATA)
                    {
                        taffy_Overflow* y_value = (taffy_Overflow*)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Overflow);
                        if(y_value != NULL)
                        {
                            y_found = 1; /* true */
                            y = *y_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (x_found == /* true */ 1) && (y_found == /* true */ 1) )
                {
                    taffy_Point_of_Overflow* object_ptr = taffy_Point_of_Overflow_new(x, y);
                    if(object_ptr != NULL)
                    {
                        taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Point_of_Overflow : taffy_Point_of_Overflow_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Point_of_Overflow : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_Overflow : provided argument is not a table");
        }
    } break;

    case 2:
    {
        taffy_Overflow* x = (taffy_Overflow*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Overflow);
        taffy_Overflow* y = (taffy_Overflow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Overflow);

        taffy_Point_of_Overflow* object_ptr = taffy_Point_of_Overflow_new(*x, *y);
        if(object_ptr != NULL)
        {
            taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_Overflow : taffy_Point_of_Overflow_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Point_of_Overflow : wrong arguments count");
}

static int lua_taffy_Point_of_Overflow_copy(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    taffy_Point_of_Overflow* copy = taffy_Point_of_Overflow_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Point_of_Overflow : taffy_Point_of_Overflow_new_copy() failed");
    }
}

static int lua_taffy_Point_of_Overflow_delete(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    taffy_Point_of_Overflow_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_Overflow_eq(lua_State* L)
{
    taffy_Point_of_Overflow** object_lhs = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);
    taffy_Point_of_Overflow** object_rhs = (taffy_Point_of_Overflow**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Point_of_Overflow);

    const int is_equal = taffy_Point_of_Overflow_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_Overflow_get_x(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    const taffy_Overflow x = taffy_Point_of_Overflow_get_x(*self);

    taffy_Overflow* user_data = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
    *user_data = x;
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_Overflow_get_y(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    const taffy_Overflow y = taffy_Point_of_Overflow_get_y(*self);

    taffy_Overflow* user_data = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
    *user_data = y;
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_Overflow_set_x(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    taffy_Overflow* x = (taffy_Overflow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Overflow);

    taffy_Point_of_Overflow_set_x(*self, *x);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_Overflow_set_y(lua_State* L)
{
    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);

    taffy_Overflow* y = (taffy_Overflow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Overflow);

    taffy_Point_of_Overflow_set_y(*self, *y);

    return 0; /* number of results */
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Point_of_Overflow_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "x") == 0)
    {
        const taffy_Overflow x = taffy_Point_of_Overflow_get_x(*self);

        taffy_Overflow* user_data = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
        *user_data = x;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);

        return 1; /* number of results */
    }
    else if(strcmp(key, "y") == 0)
    {
        const taffy_Overflow y = taffy_Point_of_Overflow_get_y(*self);

        taffy_Overflow* user_data = (taffy_Overflow*)lua_newuserdata(L, sizeof(taffy_Overflow));
        *user_data = y;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Overflow);

        return 1; /* number of results */
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_Overflow_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Point_of_Overflow** self = (taffy_Point_of_Overflow**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_Overflow);
    const char* key = luaL_checkstring(L, 2);
    taffy_Overflow* value = (taffy_Overflow*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Overflow);

    if(strcmp(key, "x") == 0)
    {
        taffy_Point_of_Overflow_set_x(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "y") == 0)
    {
        taffy_Point_of_Overflow_set_y(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Point_of_Overflow 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Point_of_Overflow(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow) )
    {
        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_get_x);
        lua_setfield(L, -2, "get_x");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_get_y);
        lua_setfield(L, -2, "get_y");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_set_x);
        lua_setfield(L, -2, "set_x");

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_set_y);
        lua_setfield(L, -2, "set_y");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Point_of_Overflow_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow_namespace);
}

/* -------------------------------------------------------------------------- */
/* Rect<LengthPercentage> */

static const char LUA_META_OBJECT_taffy_Rect_of_LengthPercentage[]           = "taffy_Rect_of_LengthPercentage_mt";
static const char LUA_META_OBJECT_taffy_Rect_of_LengthPercentage_namespace[] = "taffy_Rect_of_LengthPercentage_namespace_mt";

static int lua_taffy_Rect_of_LengthPercentage_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {
                        LengthPercentage.Length (10),
                        LengthPercentage.Percent(20),
                        LengthPercentage.Length (30),
                        LengthPercentage.Percent(40)
                    }
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 4)
            {
                /* bool */ int left_found   = 0; /* false */
                /* bool */ int right_found  = 0; /* false */
                /* bool */ int top_found    = 0; /* false */
                /* bool */ int bottom_found = 0; /* false */

                taffy_LengthPercentage* left   = NULL;
                taffy_LengthPercentage* right  = NULL;
                taffy_LengthPercentage* top    = NULL;
                taffy_LengthPercentage* bottom = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_LengthPercentage** value_value = NULL;
                        lua_Number               key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                                     /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'left' */
                        {
                            if(value_value != NULL)
                            {
                                left_found = 1; /* true */
                                left = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'right' */
                        {
                            if(value_value != NULL)
                            {
                                right_found = 1; /* true */
                                right = *value_value;
                            }
                        }
                        else if(key_value == 3.0f) /* 'third' index (in C its '2', in Lua its '3') is 'top' */
                        {
                            if(value_value != NULL)
                            {
                                top_found = 1; /* true */
                                top = *value_value;
                            }
                        }
                        else if(key_value == 4.0f) /* 'fourth' index (in C its '3', in Lua its '4') is 'bottom' */
                        {
                            if(value_value != NULL)
                            {
                                bottom_found = 1; /* true */
                                bottom = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (left_found == /* true */ 1) && (right_found == /* true */ 1) && (top_found == /* true */ 1) && (bottom_found == /* true */ 1) )
                {
                    taffy_Rect_of_LengthPercentage* object_ptr = taffy_Rect_of_LengthPercentage_new(left, right, top, bottom);
                    if(object_ptr != NULL)
                    {
                        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {
                        left   = LengthPercentage.Length (10),
                        right  = LengthPercentage.Percent(20),
                        top    = LenghtPercentage.Length (30),
                        bottom = LengthPercentage.Percent(40)
                    }

                if table size != 4 OR 'left','right','top','bottom' not in indexes '1','2','3','4'
            */
            {
                /* bool */ int left_found   = 0; /* false */
                /* bool */ int right_found  = 0; /* false */
                /* bool */ int top_found    = 0; /* false */
                /* bool */ int bottom_found = 0; /* false */

                taffy_LengthPercentage* left   = NULL;
                taffy_LengthPercentage* right  = NULL;
                taffy_LengthPercentage* top    = NULL;
                taffy_LengthPercentage* bottom = NULL;

                /* Try to get 'left' */
                {
                    const int left_type = lua_getfield(L, 1, "left");
                    if(left_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** left_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(left_value != NULL)
                        {
                            left_found = 1; /* true */
                            left = *left_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'right' */
                {
                    const int right_type = lua_getfield(L, 1, "right");
                    if(right_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** right_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(right_value != NULL)
                        {
                            right_found = 1; /* true */
                            right = *right_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'top' */
                {
                    const int top_type = lua_getfield(L, 1, "top");
                    if(top_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** top_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(top_value != NULL)
                        {
                            top_found = 1; /* true */
                            top = *top_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'bottom' */
                {
                    const int bottom_type = lua_getfield(L, 1, "bottom");
                    if(bottom_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** bottom_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(bottom_value != NULL)
                        {
                            bottom_found = 1; /* true */
                            bottom = *bottom_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (left_found == /* true */ 1) && (right_found == /* true */ 1) && (top_found == /* true */ 1)  && (bottom_found == /* true */ 1) )
                {
                    taffy_Rect_of_LengthPercentage* object_ptr = taffy_Rect_of_LengthPercentage_new(left, right, top, bottom);
                    if(object_ptr != NULL)
                    {
                        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : provided argument is not a table");
        }
    } break;

    case 4:
    {
        taffy_LengthPercentage** left   = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);
        taffy_LengthPercentage** right  = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);
        taffy_LengthPercentage** top    = (taffy_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_LengthPercentage);
        taffy_LengthPercentage** bottom = (taffy_LengthPercentage**)luaL_checkudata(L, 4, LUA_META_OBJECT_taffy_LengthPercentage);


        taffy_Rect_of_LengthPercentage* object_ptr = taffy_Rect_of_LengthPercentage_new(*left, *right, *top, *bottom);
        if(object_ptr != NULL)
        {
            taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : wrong arguments count");
}

static int lua_taffy_Rect_of_LengthPercentage_copy(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_Rect_of_LengthPercentage* copy = taffy_Rect_of_LengthPercentage_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentage_delete(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_Rect_of_LengthPercentage_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_eq(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** object_lhs = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);
    taffy_Rect_of_LengthPercentage** object_rhs = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    const int is_equal = taffy_Rect_of_LengthPercentage_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_get_left(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    const taffy_LengthPercentage* left = taffy_Rect_of_LengthPercentage_get_left(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(left);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentage_get_right(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    const taffy_LengthPercentage* right = taffy_Rect_of_LengthPercentage_get_right(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(right);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffyLengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentage_get_top(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    const taffy_LengthPercentage* top = taffy_Rect_of_LengthPercentage_get_top(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(top);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentage_get_bottom(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    const taffy_LengthPercentage* bottom = taffy_Rect_of_LengthPercentage_get_bottom(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(bottom);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentage_set_left(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_LengthPercentage** left = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Rect_of_LengthPercentage_set_left(*self, *left);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_set_right(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_LengthPercentage** right = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Rect_of_LengthPercentage_set_right(*self, *right);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_set_top(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_LengthPercentage** top = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Rect_of_LengthPercentage_set_top(*self, *top);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_set_bottom(lua_State* L)
{
    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_LengthPercentage** bottom = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Rect_of_LengthPercentage_set_bottom(*self, *bottom);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_zero(lua_State* L)
{
    taffy_Rect_of_LengthPercentage* object_ptr = taffy_Rect_of_LengthPercentage_new_zero();
    if(object_ptr != NULL)
    {
        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_zero() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Rect_of_LengthPercentage_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "left") == 0)
    {
        const taffy_LengthPercentage* left = taffy_Rect_of_LengthPercentage_get_left(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(left);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "right") == 0)
    {
        const taffy_LengthPercentage* right = taffy_Rect_of_LengthPercentage_get_right(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(right);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "top") == 0)
    {
        const taffy_LengthPercentage* top = taffy_Rect_of_LengthPercentage_get_top(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(top);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "bottom") == 0)
    {
        const taffy_LengthPercentage* bottom = taffy_Rect_of_LengthPercentage_get_bottom(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(bottom);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentage_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Rect_of_LengthPercentage** self = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);
    const char* key = luaL_checkstring(L, 2);
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_LengthPercentage);

    if(strcmp(key, "left") == 0)
    {
        taffy_Rect_of_LengthPercentage_set_left(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "right") == 0)
    {
        taffy_Rect_of_LengthPercentage_set_right(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "top") == 0)
    {
        taffy_Rect_of_LengthPercentage_set_top(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "bottom") == 0)
    {
        taffy_Rect_of_LengthPercentage_set_bottom(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Rect_of_LengthPercentage 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Rect_of_LengthPercentage(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage) )
    {
        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_get_left);
        lua_setfield(L, -2, "get_left");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_get_right);
        lua_setfield(L, -2, "get_right");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_get_top);
        lua_setfield(L, -2, "get_top");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_get_bottom);
        lua_setfield(L, -2, "get_bottom");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_set_left);
        lua_setfield(L, -2, "set_left");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_set_right);
        lua_setfield(L, -2, "set_right");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_set_top);
        lua_setfield(L, -2, "set_top");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_set_bottom);
        lua_setfield(L, -2, "set_bottom");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentage_zero);
        lua_setfield(L, -2, "zero");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage_namespace);
}

/* -------------------------------------------------------------------------- */
/* Rect<LengthPercentageAuto> */

static const char LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto[]           = "taffy_Rect_of_LengthPercentageAuto_mt";
static const char LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto_namespace[] = "taffy_Rect_of_LengthPercentageAuto_namespace_mt";

static int lua_taffy_Rect_of_LengthPercentageAuto_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {
                        LengthPercentageAuto.Length (10),
                        LengthPercentageAuto.Percent(20),
                        LengthPercentageAuto.Length (30),
                        LengthPercentageAuto.Percent(40)
                    }
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 4)
            {
                /* bool */ int left_found   = 0; /* false */
                /* bool */ int right_found  = 0; /* false */
                /* bool */ int top_found    = 0; /* false */
                /* bool */ int bottom_found = 0; /* false */

                taffy_LengthPercentageAuto* left   = NULL;
                taffy_LengthPercentageAuto* right  = NULL;
                taffy_LengthPercentageAuto* top    = NULL;
                taffy_LengthPercentageAuto* bottom = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_LengthPercentageAuto** value_value = NULL;
                        lua_Number                   key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_LengthPercentageAuto**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentageAuto); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                                             /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'left' */
                        {
                            if(value_value != NULL)
                            {
                                left_found = 1; /* true */
                                left = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'right' */
                        {
                            if(value_value != NULL)
                            {
                                right_found = 1; /* true */
                                right = *value_value;
                            }
                        }
                        else if(key_value == 3.0f) /* 'third' index (in C its '2', in Lua its '3') is 'top' */
                        {
                            if(value_value != NULL)
                            {
                                top_found = 1; /* true */
                                top = *value_value;
                            }
                        }
                        else if(key_value == 4.0f) /* 'fourth' index (in C its '3', in Lua its '4') is 'bottom' */
                        {
                            if(value_value != NULL)
                            {
                                bottom_found = 1; /* true */
                                bottom = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (left_found == /* true */ 1) && (right_found == /* true */ 1) && (top_found == /* true */ 1) && (bottom_found == /* true */ 1) )
                {
                    taffy_Rect_of_LengthPercentageAuto* object_ptr = taffy_Rect_of_LengthPercentageAuto_new(left, right, top, bottom);
                    if(object_ptr != NULL)
                    {
                        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {
                        left   = LengthPercentageAuto.Length (10),
                        right  = LengthPercentageAuto.Percent(20),
                        top    = LenghtPercentageAuto.Length (30),
                        bottom = LengthPercentageAuto.Percent(40)
                    }

                if table size != 4 OR 'left','right','top','bottom' not in indexes '1','2','3','4'
            */
            {
                /* bool */ int left_found   = 0; /* false */
                /* bool */ int right_found  = 0; /* false */
                /* bool */ int top_found    = 0; /* false */
                /* bool */ int bottom_found = 0; /* false */

                taffy_LengthPercentageAuto* left   = NULL;
                taffy_LengthPercentageAuto* right  = NULL;
                taffy_LengthPercentageAuto* top    = NULL;
                taffy_LengthPercentageAuto* bottom = NULL;

                /* Try to get 'left' */
                {
                    const int left_type = lua_getfield(L, 1, "left");
                    if(left_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentageAuto** left_value = (taffy_LengthPercentageAuto**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
                        if(left_value != NULL)
                        {
                            left_found = 1; /* true */
                            left = *left_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'right' */
                {
                    const int right_type = lua_getfield(L, 1, "right");
                    if(right_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentageAuto** right_value = (taffy_LengthPercentageAuto**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
                        if(right_value != NULL)
                        {
                            right_found = 1; /* true */
                            right = *right_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'top' */
                {
                    const int top_type = lua_getfield(L, 1, "top");
                    if(top_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentageAuto** top_value = (taffy_LengthPercentageAuto**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
                        if(top_value != NULL)
                        {
                            top_found = 1; /* true */
                            top = *top_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'bottom' */
                {
                    const int bottom_type = lua_getfield(L, 1, "bottom");
                    if(bottom_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentageAuto** bottom_value = (taffy_LengthPercentageAuto**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
                        if(bottom_value != NULL)
                        {
                            bottom_found = 1; /* true */
                            bottom = *bottom_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (left_found == /* true */ 1) && (right_found == /* true */ 1) && (top_found == /* true */ 1)  && (bottom_found == /* true */ 1) )
                {
                    taffy_Rect_of_LengthPercentageAuto* object_ptr = taffy_Rect_of_LengthPercentageAuto_new(left, right, top, bottom);
                    if(object_ptr != NULL)
                    {
                        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : provided argument is not a table");
        }
    } break;

    case 4:
    {
        taffy_LengthPercentageAuto** left   = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);
        taffy_LengthPercentageAuto** right  = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);
        taffy_LengthPercentageAuto** top    = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_LengthPercentageAuto);
        taffy_LengthPercentageAuto** bottom = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 4, LUA_META_OBJECT_taffy_LengthPercentageAuto);


        taffy_Rect_of_LengthPercentageAuto* object_ptr = taffy_Rect_of_LengthPercentageAuto_new(*left, *right, *top, *bottom);
        if(object_ptr != NULL)
        {
            taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : wrong arguments count");
}

static int lua_taffy_Rect_of_LengthPercentageAuto_copy(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto* copy = taffy_Rect_of_LengthPercentageAuto_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_delete(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_eq(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** object_lhs = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);
    taffy_Rect_of_LengthPercentageAuto** object_rhs = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    const int is_equal = taffy_Rect_of_LengthPercentageAuto_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_get_left(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    const taffy_LengthPercentageAuto* left = taffy_Rect_of_LengthPercentageAuto_get_left(*self);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(left);
    if(copy != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_get_right(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    const taffy_LengthPercentageAuto* right = taffy_Rect_of_LengthPercentageAuto_get_right(*self);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(right);
    if(copy != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffyLengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_get_top(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    const taffy_LengthPercentageAuto* top = taffy_Rect_of_LengthPercentageAuto_get_top(*self);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(top);
    if(copy != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_get_bottom(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    const taffy_LengthPercentageAuto* bottom = taffy_Rect_of_LengthPercentageAuto_get_bottom(*self);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(bottom);
    if(copy != NULL)
    {
        taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
        *user_data = copy;
        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_set_left(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_LengthPercentageAuto** left = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto_set_left(*self, *left);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_set_right(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_LengthPercentageAuto** right = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto_set_right(*self, *right);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_set_top(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_LengthPercentageAuto** top = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto_set_top(*self, *top);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_set_bottom(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_LengthPercentageAuto** bottom = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_Rect_of_LengthPercentageAuto_set_bottom(*self, *bottom);

    return 0; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_AUTO(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto* object_ptr = taffy_Rect_of_LengthPercentageAuto_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_AUTO() failed");
    }
}

static int lua_taffy_Rect_of_LengthPercentageAuto_zero(lua_State* L)
{
    taffy_Rect_of_LengthPercentageAuto* object_ptr = taffy_Rect_of_LengthPercentageAuto_new_zero();
    if(object_ptr != NULL)
    {
        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_zero() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Rect_of_LengthPercentageAuto_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "left") == 0)
    {
        const taffy_LengthPercentageAuto* left = taffy_Rect_of_LengthPercentageAuto_get_left(*self);

        taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(left);
        if(copy != NULL)
        {
            taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
        }
    }
    else if(strcmp(key, "right") == 0)
    {
        const taffy_LengthPercentageAuto* right = taffy_Rect_of_LengthPercentageAuto_get_right(*self);

        taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(right);
        if(copy != NULL)
        {
            taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
        }
    }
    else if(strcmp(key, "top") == 0)
    {
        const taffy_LengthPercentageAuto* top = taffy_Rect_of_LengthPercentageAuto_get_top(*self);

        taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(top);
        if(copy != NULL)
        {
            taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
        }
    }
    else if(strcmp(key, "bottom") == 0)
    {
        const taffy_LengthPercentageAuto* bottom = taffy_Rect_of_LengthPercentageAuto_get_bottom(*self);

        taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(bottom);
        if(copy != NULL)
        {
            taffy_LengthPercentageAuto** user_data = (taffy_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_LengthPercentageAuto*));
            *user_data = copy;
            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentageAuto : taffy_LengthPercentageAuto_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Rect_of_LengthPercentageAuto_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Rect_of_LengthPercentageAuto** self = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);
    const char* key = luaL_checkstring(L, 2);
    taffy_LengthPercentageAuto** value = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    if(strcmp(key, "left") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto_set_left(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "right") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto_set_right(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "top") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto_set_top(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "bottom") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto_set_bottom(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Rect_of_LengthPercentageAuto 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Rect_of_LengthPercentageAuto(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto) )
    {
        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_get_left);
        lua_setfield(L, -2, "get_left");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_get_right);
        lua_setfield(L, -2, "get_right");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_get_top);
        lua_setfield(L, -2, "get_top");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_get_bottom);
        lua_setfield(L, -2, "get_bottom");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_set_left);
        lua_setfield(L, -2, "set_left");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_set_right);
        lua_setfield(L, -2, "set_right");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_set_top);
        lua_setfield(L, -2, "set_top");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_set_bottom);
        lua_setfield(L, -2, "set_bottom");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_Rect_of_LengthPercentageAuto_zero);
        lua_setfield(L, -2, "zero");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto_namespace);
}

/* -------------------------------------------------------------------------- */
/* Size<LengthPercentage> */

static const char LUA_META_OBJECT_taffy_Size_of_LengthPercentage[]           = "taffy_Size_of_LengthPercentage_mt";
static const char LUA_META_OBJECT_taffy_Size_of_LengthPercentage_namespace[] = "taffy_Size_of_LengthPercentage_namespace_mt";

static int lua_taffy_Size_of_LengthPercentage_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {
                        LengthPercentage.Length(10),
                        LengthPercentage.Percent(20)
                    }

                    {
                        [1] = LengthPercentage.Length(10),
                        [2] = LengthPercentage.Percent(20)
                    }
                    {
                        [2] = LengthPercentage.Percent(20),
                        [1] = LengthPercentage.Length(10)
                    }
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int width_found  = 0; /* false */
                /* bool */ int height_found = 0; /* false */

                taffy_LengthPercentage* width  = NULL;
                taffy_LengthPercentage* height = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_LengthPercentage** value_value = NULL;
                        lua_Number               key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                                     /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'width' */
                        {
                            if(value_value != NULL)
                            {
                                width_found = 1; /* true */
                                width = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'height' */
                        {
                            if(value_value != NULL)
                            {
                                height_found = 1; /* true */
                                height = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_LengthPercentage* object_ptr = taffy_Size_of_LengthPercentage_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {
                        width  = LengthPercentage.Length(10),
                        height = LengthPercentage.Percent(20)
                    }

                if table size != 2 OR 'width' and 'height' not in indexes '1' and '2'
            */
            {
                /* bool */ int width_found  = 0; /* false */
                /* bool */ int height_found = 0; /* false */

                taffy_LengthPercentage* width  = NULL;
                taffy_LengthPercentage* height = NULL;

                /* Try to get 'width' */
                {
                    const int width_type = lua_getfield(L, 1, "width");
                    if(width_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** width_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(width_value != NULL)
                        {
                            width_found = 1; /* true */
                            width = *width_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'height' */
                {
                    const int height_type = lua_getfield(L, 1, "height");
                    if(height_type == LUA_TUSERDATA)
                    {
                        taffy_LengthPercentage** height_value = (taffy_LengthPercentage**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_LengthPercentage);
                        if(height_value != NULL)
                        {
                            height_found = 1; /* true */
                            height = *height_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_LengthPercentage* object_ptr = taffy_Size_of_LengthPercentage_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : provided argument is not a table");
        }
    } break;

    case 2:
    {
        taffy_LengthPercentage** width  = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);
        taffy_LengthPercentage** height = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

        taffy_Size_of_LengthPercentage* object_ptr = taffy_Size_of_LengthPercentage_new(*width, *height);
        if(object_ptr != NULL)
        {
            taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : wrong arguments count");
}

static int lua_taffy_Size_of_LengthPercentage_copy(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    taffy_Size_of_LengthPercentage* copy = taffy_Size_of_LengthPercentage_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Size_of_LengthPercentage_delete(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    taffy_Size_of_LengthPercentage_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_LengthPercentage_eq(lua_State* L)
{
    taffy_Size_of_LengthPercentage** object_lhs = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);
    taffy_Size_of_LengthPercentage** object_rhs = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    const int is_equal = taffy_Size_of_LengthPercentage_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_LengthPercentage_get_width(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    const taffy_LengthPercentage* width = taffy_Size_of_LengthPercentage_get_width(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(width);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Size_of_LengthPercentage_get_height(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    const taffy_LengthPercentage* height = taffy_Size_of_LengthPercentage_get_height(*self);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(height);
    if(copy != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Size_of_LengthPercentage_set_width(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    taffy_LengthPercentage** width = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Size_of_LengthPercentage_set_width(*self, *width);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_LengthPercentage_set_height(lua_State* L)
{
    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    taffy_LengthPercentage** height = (taffy_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_Size_of_LengthPercentage_set_height(*self, *height);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_LengthPercentage_zero(lua_State* L)
{
    taffy_Size_of_LengthPercentage* object_ptr = taffy_Size_of_LengthPercentage_new_zero();
    if(object_ptr != NULL)
    {
        taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new_zero() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Size_of_LengthPercentage_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "width") == 0)
    {
        const taffy_LengthPercentage* width = taffy_Size_of_LengthPercentage_get_width(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(width);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "height") == 0)
    {
        const taffy_LengthPercentage* height = taffy_Size_of_LengthPercentage_get_height(*self);

        taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(height);
        if(copy != NULL)
        {
            taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_LengthPercentage_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Size_of_LengthPercentage** self = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);
    const char* key = luaL_checkstring(L, 2);
    taffy_LengthPercentage** value = (taffy_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_LengthPercentage);


    if(strcmp(key, "width") == 0)
    {
        taffy_Size_of_LengthPercentage_set_width(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "height") == 0)
    {
        taffy_Size_of_LengthPercentage_set_height(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Size_of_LengthPercentage 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Size_of_LengthPercentage(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage) )
    {
        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_get_width);
        lua_setfield(L, -2, "get_width");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_get_height);
        lua_setfield(L, -2, "get_height");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_set_width);
        lua_setfield(L, -2, "set_width");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_set_height);
        lua_setfield(L, -2, "set_height");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Size_of_LengthPercentage_zero);
        lua_setfield(L, -2, "zero");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage_namespace);
}

/* -------------------------------------------------------------------------- */
/* Size<Dimension> */

static const char LUA_META_OBJECT_taffy_Size_of_Dimension[]           = "taffy_Size_of_Dimension_mt";
static const char LUA_META_OBJECT_taffy_Size_of_Dimension_namespace[] = "taffy_Size_of_Dimension_namespace_mt";

static int lua_taffy_Size_of_Dimension_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {
                        Dimension.Length(10),
                        Dimension.Percent(20)
                    }

                    {
                        [1] = Dimension.Length(10),
                        [2] = Dimension.Percent(20)
                    }
                    {
                        [2] = Dimension.Percent(20),
                        [1] = Dimension.Length(10)
                    }
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int width_found  = 0; /* false */
                /* bool */ int height_found = 0; /* false */

                taffy_Dimension* width  = NULL;
                taffy_Dimension* height = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_Dimension** value_value = NULL;
                        lua_Number        key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_Dimension**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Dimension); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                       /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'width' */
                        {
                            if(value_value != NULL)
                            {
                                width_found = 1; /* true */
                                width = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'height' */
                        {
                            if(value_value != NULL)
                            {
                                height_found = 1; /* true */
                                height = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_Dimension* object_ptr = taffy_Size_of_Dimension_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {
                        width  = Dimension.Length(10),
                        height = Dimension.Percent(20)
                    }

                if table size != 2 OR 'width' and 'height' not in indexes '1' and '2'
            */
            {
                /* bool */ int width_found  = 0; /* false */
                /* bool */ int height_found = 0; /* false */

                taffy_Dimension* width  = NULL;
                taffy_Dimension* height = NULL;

                /* Try to get 'width' */
                {
                    const int width_type = lua_getfield(L, 1, "width");
                    if(width_type == LUA_TUSERDATA)
                    {
                        taffy_Dimension** width_value = (taffy_Dimension**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Dimension);
                        if(width_value != NULL)
                        {
                            width_found = 1; /* true */
                            width = *width_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'height' */
                {
                    const int height_type = lua_getfield(L, 1, "height");
                    if(height_type == LUA_TUSERDATA)
                    {
                        taffy_Dimension** height_value = (taffy_Dimension**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_Dimension);
                        if(height_value != NULL)
                        {
                            height_found = 1; /* true */
                            height = *height_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (width_found == /* true */ 1) && (height_found == /* true */ 1) )
                {
                    taffy_Size_of_Dimension* object_ptr = taffy_Size_of_Dimension_new(width, height);
                    if(object_ptr != NULL)
                    {
                        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Size_of_Dimension : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_Dimension : provided argument is not a table");
        }
    } break;

    case 2:
    {
        taffy_Dimension** width  = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);
        taffy_Dimension** height = (taffy_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Dimension);

        taffy_Size_of_Dimension* object_ptr = taffy_Size_of_Dimension_new(*width, *height);
        if(object_ptr != NULL)
        {
            taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Size_of_Dimension : wrong arguments count");
}

static int lua_taffy_Size_of_Dimension_copy(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Size_of_Dimension_delete(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Size_of_Dimension_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_Dimension_eq(lua_State* L)
{
    taffy_Size_of_Dimension** object_lhs = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);
    taffy_Size_of_Dimension** object_rhs = (taffy_Size_of_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_Dimension);

    const int is_equal = taffy_Size_of_Dimension_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_Dimension_get_width(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    const taffy_Dimension* width = taffy_Size_of_Dimension_get_width(*self);

    taffy_Dimension* copy = taffy_Dimension_new_copy(width);
    if(copy != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Dimension : taffy_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Size_of_Dimension_get_height(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    const taffy_Dimension* height = taffy_Size_of_Dimension_get_height(*self);

    taffy_Dimension* copy = taffy_Dimension_new_copy(height);
    if(copy != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Dimension : taffy_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Size_of_Dimension_set_width(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Dimension** width = (taffy_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Dimension);

    taffy_Size_of_Dimension_set_width(*self, *width);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_Dimension_set_height(lua_State* L)
{
    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Dimension** height = (taffy_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Dimension);

    taffy_Size_of_Dimension_set_height(*self, *height);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_Dimension_AUTO(lua_State* L)
{
    taffy_Size_of_Dimension* object_ptr = taffy_Size_of_Dimension_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_AUTO() failed");
    }
}

static int lua_taffy_Size_of_Dimension_zero(lua_State* L)
{
    taffy_Size_of_Dimension* object_ptr = taffy_Size_of_Dimension_new_zero();
    if(object_ptr != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_zero() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Size_of_Dimension_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "width") == 0)
    {
        const taffy_Dimension* width = taffy_Size_of_Dimension_get_width(*self);

        taffy_Dimension* copy = taffy_Dimension_new_copy(width);
        if(copy != NULL)
        {
            taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_Dimension : taffy_Dimension_new_copy() failed");
        }
    }
    else if(strcmp(key, "height") == 0)
    {
        const taffy_Dimension* height = taffy_Size_of_Dimension_get_height(*self);

        taffy_Dimension* copy = taffy_Dimension_new_copy(height);
        if(copy != NULL)
        {
            taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_Dimension : taffy_Dimension_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_Dimension_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Size_of_Dimension** self = (taffy_Size_of_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_Dimension);
    const char* key = luaL_checkstring(L, 2);
    taffy_Dimension** value = (taffy_Dimension**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Dimension);


    if(strcmp(key, "width") == 0)
    {
        taffy_Size_of_Dimension_set_width(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "height") == 0)
    {
        taffy_Size_of_Dimension_set_height(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Size_of_Dimension 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Size_of_Dimension(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension) )
    {
        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_get_width);
        lua_setfield(L, -2, "get_width");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_get_height);
        lua_setfield(L, -2, "get_height");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_set_width);
        lua_setfield(L, -2, "set_width");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_set_height);
        lua_setfield(L, -2, "set_height");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_AUTO);
        lua_setfield(L, -2, "AUTO");

        lua_pushcfunction(L, lua_taffy_Size_of_Dimension_zero);
        lua_setfield(L, -2, "zero");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension_namespace);
}

/* -------------------------------------------------------------------------- */
/* Line<GridPlacement> */

/*
    IMPORTANT NOTE: in Lua (lowercase) 'end' is reserved keyword, so we must
    name 'end' property with different name - that's why we choose (uppercase)
    'End' for it (and, for consistency - 'start' is also named uppercase as
    'Start').

    It affects the next cases:

        -- Initialization with table-as-dictionary
        local line = taffy.Line_of_GridPlacement {
            Start = taffy.GridPlacement.Auto()
            End   = taffy.GridPlacement.Span(42)
        }

        -- Property getting
        local line_start = line.Start
        local line_end   = line.End

        -- Property settings
        line.Start = line_start
        line.End   = line_end

*/

static const char LUA_META_OBJECT_taffy_Line_of_GridPlacement[]           = "taffy_Line_of_GridPlacement_mt";
static const char LUA_META_OBJECT_taffy_Line_of_GridPlacement_namespace[] = "taffy_Line_of_GridPlacement_namespace_mt";

static int lua_taffy_Line_of_GridPlacement_new(lua_State* L)
{
    const int n = lua_gettop(L); /* Number of arguments */

    switch(n) {
    case 0:
    {
        taffy_Line_of_GridPlacement* object_ptr = taffy_Line_of_GridPlacement_new_default();
        if(object_ptr != NULL)
        {
            taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_default() failed");
        }
    } break;

    case 1:
    {
        if(lua_type(L, 1) == LUA_TTABLE)
        {
            /*
                First attempt - try to interpret table like 'array':

                    {
                        GridPlacement.Auto(),
                        GridPlacement.Span(20)
                    }

                    {
                        [1] = GridPlacement.Auto(),
                        [2] = GridPlacement.Span(20)
                    }
                    {
                        [2] = GridPlacement.Auto(),
                        [1] = GridPlacement.Span(10)
                    }
            */
            const size_t table_size = lua_rawlen(L, 1);
            if(table_size == 2)
            {
                /* bool */ int start_found = 0; /* false */
                /* bool */ int end_found   = 0; /* false */

                taffy_GridPlacement* start = NULL;
                taffy_GridPlacement* end   = NULL;

                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 1) != 0 )
                {
                    /* uses 'key' (at index -2) and 'value' (at index -1) */
                    const int value_type = lua_type(L, -1);
                    const int key_type   = lua_type(L, -2);

                    if((key_type == LUA_TNUMBER) && (value_type == LUA_TUSERDATA))
                    {
                        taffy_GridPlacement** value_value = NULL;
                        lua_Number            key_value   = 0.0f;

                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        value_value = (taffy_GridPlacement**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_GridPlacement); /* pop 'value' */
                        key_value   = lua_tonumber(L, -2);                                                               /* pop 'key'   */

                        if(key_value == 1.0f) /* 'first' index (in C its '0', in Lua its '1') is 'start' */
                        {
                            if(value_value != NULL)
                            {
                                start_found = 1; /* true */
                                start = *value_value;
                            }
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'end' */
                        {
                            if(value_value != NULL)
                            {
                                end_found = 1; /* true */
                                end = *value_value;
                            }
                        }
                    }

                    /* removes 'value'; keeps 'key' for next iteration */
                    lua_pop(L, 1);
                }
                lua_pop(L, 1); /* pop 'key' from the stack */

                if( (start_found == /* true */ 1) && (end_found == /* true */ 1) )
                {
                    taffy_Line_of_GridPlacement* object_ptr = taffy_Line_of_GridPlacement_new(start, end);
                    if(object_ptr != NULL)
                    {
                        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new() failed");
                    }
                }
            }

            /*
                Second attempt - try to interpret table like 'dictionary':

                    {
                        Start = GridPlacement.Auto(),
                        End   = GridPlacement.Span(20)
                    }

                if table size != 2 OR 'start' and 'end' not in indexes '1' and '2'
            */
            {
                /* bool */ int start_found = 0; /* false */
                /* bool */ int end_found   = 0; /* false */

                taffy_GridPlacement* start = NULL;
                taffy_GridPlacement* end   = NULL;

                /* Try to get 'start' */
                {
                    const int start_type = lua_getfield(L, 1, "Start");
                    if(start_type == LUA_TUSERDATA)
                    {
                        taffy_GridPlacement** start_value = (taffy_GridPlacement**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_GridPlacement);
                        if(start_value != NULL)
                        {
                            start_found = 1; /* true */
                            start = *start_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                /* Try to get 'end' */
                {
                    const int end_type = lua_getfield(L, 1, "End");
                    if(end_type == LUA_TUSERDATA)
                    {
                        taffy_GridPlacement** end_value = (taffy_GridPlacement**)luaL_testudata(L, -1, LUA_META_OBJECT_taffy_GridPlacement);
                        if(end_value != NULL)
                        {
                            end_found = 1; /* true */
                            end = *end_value;
                        }
                    }
                    else
                    {
                        lua_pop(L, 1); /* pop 'value' pushed by 'lua_getfield' */
                    }
                }

                if( (start_found == /* true */ 1) && (end_found == /* true */ 1) )
                {
                    taffy_Line_of_GridPlacement* object_ptr = taffy_Line_of_GridPlacement_new(start, end);
                    if(object_ptr != NULL)
                    {
                        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
                        *user_data = object_ptr;

                        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

                        return 1; /* number of results */
                    }
                    else
                    {
                        return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new() failed");
                    }
                }
            }

            /* After all, at this line all attempts to parse table are failed */
            return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : provided table is invalid");
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : provided argument is not a table");
        }
    } break;

    case 2:
    {
        taffy_GridPlacement** start = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);
        taffy_GridPlacement** end   = (taffy_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridPlacement);

        taffy_Line_of_GridPlacement* object_ptr = taffy_Line_of_GridPlacement_new(*start, *end);
        if(object_ptr != NULL)
        {
            taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : wrong arguments count");
}

static int lua_taffy_Line_of_GridPlacement_copy(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_Line_of_GridPlacement* copy = taffy_Line_of_GridPlacement_new_copy(*self);

    if(copy != NULL)
    {
        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_copy() failed");
    }
}

static int lua_taffy_Line_of_GridPlacement_delete(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_Line_of_GridPlacement_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Line_of_GridPlacement_eq(lua_State* L)
{
    taffy_Line_of_GridPlacement** object_lhs = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);
    taffy_Line_of_GridPlacement** object_rhs = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    const int is_equal = taffy_Line_of_GridPlacement_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Line_of_GridPlacement_get_start(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    const taffy_GridPlacement* start = taffy_Line_of_GridPlacement_get_start(*self);

    taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(start);
    if(copy != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_GridPlacement : taffy_GridPlacement_new_copy() failed");
    }
}

static int lua_taffy_Line_of_GridPlacement_get_end(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    const taffy_GridPlacement* end = taffy_Line_of_GridPlacement_get_end(*self);

    taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(end);
    if(copy != NULL)
    {
        taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_GridPlacement : taffy_GridPlacement_new_copy() failed");
    }
}

static int lua_taffy_Line_of_GridPlacement_set_start(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_GridPlacement** start = (taffy_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_Line_of_GridPlacement_set_start(*self, *start);

    return 0; /* number of results */
}

static int lua_taffy_Line_of_GridPlacement_set_end(lua_State* L)
{
    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_GridPlacement** end = (taffy_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_Line_of_GridPlacement_set_end(*self, *end);

    return 0; /* number of results */
}

static int lua_taffy_Line_of_GridPlacement_AUTO(lua_State* L)
{
    taffy_Line_of_GridPlacement* object_ptr = taffy_Line_of_GridPlacement_new_AUTO();
    if(object_ptr != NULL)
    {
        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_AUTO() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Line_of_GridPlacement_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "Start") == 0)
    {
        const taffy_GridPlacement* start = taffy_Line_of_GridPlacement_get_start(*self);

        taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(start);
        if(copy != NULL)
        {
            taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_GridPlacement : taffy_GridPlacement_new_copy() failed");
        }
    }
    else if(strcmp(key, "End") == 0)
    {
        const taffy_GridPlacement* end = taffy_Line_of_GridPlacement_get_end(*self);

        taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(end);
        if(copy != NULL)
        {
            taffy_GridPlacement** user_data = (taffy_GridPlacement**)lua_newuserdata(L, sizeof(taffy_GridPlacement*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to copy taffy_GridPlacement : taffy_GridPlacement_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Line_of_GridPlacement_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Line_of_GridPlacement** self = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Line_of_GridPlacement);
    const char* key = luaL_checkstring(L, 2);
    taffy_GridPlacement** value = (taffy_GridPlacement**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_GridPlacement);


    if(strcmp(key, "Start") == 0)
    {
        taffy_Line_of_GridPlacement_set_start(*self, *value);

        return 0; /* number of results */
    }
    else if( strcmp(key, "End") == 0)
    {
        taffy_Line_of_GridPlacement_set_end(*self, *value);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Line_of_GridPlacement 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Line_of_GridPlacement(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement) )
    {
        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_get_start);
        lua_setfield(L, -2, "get_start");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_get_end);
        lua_setfield(L, -2, "get_end");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_set_start);
        lua_setfield(L, -2, "set_start");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_set_end);
        lua_setfield(L, -2, "set_end");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Line_of_GridPlacement_AUTO);
        lua_setfield(L, -2, "AUTO");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement_namespace);
}

/* -------------------------------------------------------------------------- */
/* Option<AlingContent> */

static const char LUA_META_OBJECT_taffy_Option_AlignContent[]           = "taffy_Option_AlignContent_mt";
static const char LUA_META_OBJECT_taffy_Option_AlignContent_namespace[] = "taffy_Option_AlignContent_namespace_mt";

static int lua_taffy_Option_AlignContent_new(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNONE:
    {
        taffy_Option_AlignContent* object_ptr = taffy_Option_AlignContent_new_default();
        if(object_ptr != NULL)
        {
            taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignContent : taffy_Option_AlignContent_new_default() failed");
        }
    } break;

    case LUA_TNIL:
    {
        taffy_Option_AlignContent* object_ptr = NULL;

        lua_pop(L, 1);

        object_ptr = taffy_Option_AlignContent_new(NULL);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignContent : taffy_Option_AlignContent_new_some() failed");
        }
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignContent* value = (taffy_AlignContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignContent);

        taffy_Option_AlignContent* object_ptr = taffy_Option_AlignContent_new_some(*value);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignContent : taffy_Option_AlignContent_new_some() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Option_AlignContent : incorrect argument type");
}

static int lua_taffy_Option_AlignContent_copy(lua_State* L)
{
    taffy_Option_AlignContent** object = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);

    taffy_Option_AlignContent* copy = taffy_Option_AlignContent_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_AlignContent : taffy_Option_AlignContent_new_copy() failed");
    }
}

static int lua_taffy_Option_AlignContent_delete(lua_State* L)
{
    taffy_Option_AlignContent** self = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);

    taffy_Option_AlignContent_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Option_AlignContent_eq(lua_State* L)
{
    taffy_Option_AlignContent** object_lhs = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);
    taffy_Option_AlignContent** object_rhs = (taffy_Option_AlignContent**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignContent);

    const int is_equal = taffy_Option_AlignContent_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignContent_is_some(lua_State* L)
{
    taffy_Option_AlignContent** self = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);

    const int is_some = taffy_Option_AlignContent_is_some(*self);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignContent_get_value(lua_State* L)
{
    taffy_Option_AlignContent** self = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);

    if( taffy_Option_AlignContent_is_some(*self) )
    {
        const taffy_AlignContent value = taffy_Option_AlignContent_get_value(*self);

        taffy_AlignContent* user_data = (taffy_AlignContent*)lua_newuserdata(L, sizeof(taffy_AlignContent));
        *user_data = value;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignContent);

        return 1; /* number of results */
    }
    else /* None */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Option_AlignContent_set_value(lua_State* L)
{
    taffy_Option_AlignContent** self = (taffy_Option_AlignContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignContent);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_AlignContent_set_value(*self, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignContent* value = (taffy_AlignContent*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignContent);

        taffy_Option_AlignContent_set_value(*self, value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_AlignContent : incorrect argument type");
}

static void lua_push_table_taffy_Option_AlignContent(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_is_some);
        lua_setfield(L, -2, "is_some");

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_get_value);
        lua_setfield(L, -2, "get_value");

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_set_value);
        lua_setfield(L, -2, "set_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignContent_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent_namespace);
}

/* -------------------------------------------------------------------------- */
/* Option<JustifyContent> */

static const char LUA_META_OBJECT_taffy_Option_JustifyContent[]           = "taffy_Option_JustifyContent_mt";
static const char LUA_META_OBJECT_taffy_Option_JustifyContent_namespace[] = "taffy_Option_JustifyContent_namespace_mt";

static int lua_taffy_Option_JustifyContent_new(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNONE:
    {
        taffy_Option_JustifyContent* object_ptr = taffy_Option_JustifyContent_new_default();
        if(object_ptr != NULL)
        {
            taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_default() failed");
        }
    } break;

    case LUA_TNIL:
    {
        taffy_Option_JustifyContent* object_ptr = NULL;

        lua_pop(L, 1);

        object_ptr = taffy_Option_JustifyContent_new(NULL);
        if(object_ptr != NULL)
        {
            taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_some() failed");
        }
    } break;

    case LUA_TUSERDATA:
    {
        taffy_JustifyContent* value = (taffy_JustifyContent*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_JustifyContent);

        taffy_Option_JustifyContent* object_ptr = taffy_Option_JustifyContent_new_some(*value);
        if(object_ptr != NULL)
        {
            taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_some() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Option_JustifyContent : incorrect argument type");
}

static int lua_taffy_Option_JustifyContent_copy(lua_State* L)
{
    taffy_Option_JustifyContent** object = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);

    taffy_Option_JustifyContent* copy = taffy_Option_JustifyContent_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_copy() failed");
    }
}

static int lua_taffy_Option_JustifyContent_delete(lua_State* L)
{
    taffy_Option_JustifyContent** self = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);

    taffy_Option_JustifyContent_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Option_JustifyContent_eq(lua_State* L)
{
    taffy_Option_JustifyContent** object_lhs = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);
    taffy_Option_JustifyContent** object_rhs = (taffy_Option_JustifyContent**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_JustifyContent);

    const int is_equal = taffy_Option_JustifyContent_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Option_JustifyContent_is_some(lua_State* L)
{
    taffy_Option_JustifyContent** self = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);

    const int is_some = taffy_Option_JustifyContent_is_some(*self);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_JustifyContent_get_value(lua_State* L)
{
    taffy_Option_JustifyContent** self = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);

    if( taffy_Option_JustifyContent_is_some(*self) )
    {
        const taffy_JustifyContent value = taffy_Option_JustifyContent_get_value(*self);

        taffy_JustifyContent* user_data = (taffy_JustifyContent*)lua_newuserdata(L, sizeof(taffy_JustifyContent));
        *user_data = value;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_JustifyContent);

        return 1; /* number of results */
    }
    else /* None */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Option_JustifyContent_set_value(lua_State* L)
{
    taffy_Option_JustifyContent** self = (taffy_Option_JustifyContent**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_JustifyContent);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_JustifyContent_set_value(*self, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_JustifyContent* value = (taffy_JustifyContent*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_JustifyContent);

        taffy_Option_JustifyContent_set_value(*self, value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_JustifyContent : incorrect argument type");
}

static void lua_push_table_taffy_Option_JustifyContent(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_is_some);
        lua_setfield(L, -2, "is_some");

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_get_value);
        lua_setfield(L, -2, "get_value");

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_set_value);
        lua_setfield(L, -2, "set_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_JustifyContent_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent_namespace);
}

/* -------------------------------------------------------------------------- */
/* Option<AlignItems> */

static const char LUA_META_OBJECT_taffy_Option_AlignItems[]           = "taffy_Option_AlignItems_mt";
static const char LUA_META_OBJECT_taffy_Option_AlignItems_namespace[] = "taffy_Option_AlignItems_namespace_mt";

static int lua_taffy_Option_AlignItems_new(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNONE:
    {
        taffy_Option_AlignItems* object_ptr = taffy_Option_AlignItems_new_default();
        if(object_ptr != NULL)
        {
            taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignItems : taffy_Option_AlignItems_new_default() failed");
        }
    } break;

    case LUA_TNIL:
    {
        taffy_Option_AlignItems* object_ptr = NULL;

        lua_pop(L, 1);

        object_ptr = taffy_Option_AlignItems_new(NULL);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignItems : taffy_Option_AlignItems_new_some() failed");
        }
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignItems* value = (taffy_AlignItems*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignItems);

        taffy_Option_AlignItems* object_ptr = taffy_Option_AlignItems_new_some(*value);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignItems : taffy_Option_AlignItems_new_some() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Option_AlignItems : incorrect argument type");
}

static int lua_taffy_Option_AlignItems_copy(lua_State* L)
{
    taffy_Option_AlignItems** object = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);

    taffy_Option_AlignItems* copy = taffy_Option_AlignItems_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_AlignItems : taffy_Option_AlignItems_new_copy() failed");
    }
}

static int lua_taffy_Option_AlignItems_delete(lua_State* L)
{
    taffy_Option_AlignItems** self = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);

    taffy_Option_AlignItems_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Option_AlignItems_eq(lua_State* L)
{
    taffy_Option_AlignItems** object_lhs = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);
    taffy_Option_AlignItems** object_rhs = (taffy_Option_AlignItems**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignItems);

    const int is_equal = taffy_Option_AlignItems_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignItems_is_some(lua_State* L)
{
    taffy_Option_AlignItems** self = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);

    const int is_some = taffy_Option_AlignItems_is_some(*self);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignItems_get_value(lua_State* L)
{
    taffy_Option_AlignItems** self = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);

    if( taffy_Option_AlignItems_is_some(*self) )
    {
        const taffy_AlignItems value = taffy_Option_AlignItems_get_value(*self);

        taffy_AlignItems* user_data = (taffy_AlignItems*)lua_newuserdata(L, sizeof(taffy_AlignItems));
        *user_data = value;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignItems);

        return 1; /* number of results */
    }
    else /* None */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Option_AlignItems_set_value(lua_State* L)
{
    taffy_Option_AlignItems** self = (taffy_Option_AlignItems**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignItems);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_AlignItems_set_value(*self, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignItems* value = (taffy_AlignItems*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignItems);

        taffy_Option_AlignItems_set_value(*self, value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_AlignItems : incorrect argument type");
}

static void lua_push_table_taffy_Option_AlignItems(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_is_some);
        lua_setfield(L, -2, "is_some");

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_get_value);
        lua_setfield(L, -2, "get_value");

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_set_value);
        lua_setfield(L, -2, "set_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignItems_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems_namespace);
}

/* -------------------------------------------------------------------------- */
/* Option<AlignSelf> */

static const char LUA_META_OBJECT_taffy_Option_AlignSelf[]           = "taffy_Option_AlignSelf_mt";
static const char LUA_META_OBJECT_taffy_Option_AlignSelf_namespace[] = "taffy_Option_AlignSelf_namespace_mt";

static int lua_taffy_Option_AlignSelf_new(lua_State* L)
{
    switch( lua_type(L, 1) ) {
    case LUA_TNONE:
    {
        taffy_Option_AlignSelf* object_ptr = taffy_Option_AlignSelf_new_default();
        if(object_ptr != NULL)
        {
            taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_default() failed");
        }
    } break;

    case LUA_TNIL:
    {
        taffy_Option_AlignSelf* object_ptr = NULL;

        lua_pop(L, 1);

        object_ptr = taffy_Option_AlignSelf_new(NULL);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_some() failed");
        }
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignSelf* value = (taffy_AlignSelf*)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AlignSelf);

        taffy_Option_AlignSelf* object_ptr = taffy_Option_AlignSelf_new_some(*value);
        if(object_ptr != NULL)
        {
            taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
            *user_data = object_ptr;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_some() failed");
        }
    } break;
    }

    return luaL_error(L, "Failed to create taffy_Option_AlignSelf : incorrect argument type");
}

static int lua_taffy_Option_AlignSelf_copy(lua_State* L)
{
    taffy_Option_AlignSelf** object = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);

    taffy_Option_AlignSelf* copy = taffy_Option_AlignSelf_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_copy() failed");
    }
}

static int lua_taffy_Option_AlignSelf_delete(lua_State* L)
{
    taffy_Option_AlignSelf** self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);

    taffy_Option_AlignSelf_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Option_AlignSelf_eq(lua_State* L)
{
    taffy_Option_AlignSelf** object_lhs = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);
    taffy_Option_AlignSelf** object_rhs = (taffy_Option_AlignSelf**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignSelf);

    const int is_equal = taffy_Option_AlignSelf_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignSelf_is_some(lua_State* L)
{
    taffy_Option_AlignSelf** self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);

    const int is_some = taffy_Option_AlignSelf_is_some(*self);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_AlignSelf_get_value(lua_State* L)
{
    taffy_Option_AlignSelf** self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);

    if( taffy_Option_AlignSelf_is_some(*self) )
    {
        const taffy_AlignSelf value = taffy_Option_AlignSelf_get_value(*self);

        taffy_AlignSelf* user_data = (taffy_AlignSelf*)lua_newuserdata(L, sizeof(taffy_AlignSelf));
        *user_data = value;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AlignSelf);

        return 1; /* number of results */
    }
    else /* None */
    {
        lua_pushnil(L);

        return 1; /* number of results */
    }
}

static int lua_taffy_Option_AlignSelf_set_value(lua_State* L)
{
    taffy_Option_AlignSelf** self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_AlignSelf);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_AlignSelf_set_value(*self, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TUSERDATA:
    {
        taffy_AlignSelf* value = (taffy_AlignSelf*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_AlignSelf);

        taffy_Option_AlignSelf_set_value(*self, value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_AlignSelf : incorrect argument type");
}

static void lua_push_table_taffy_Option_AlignSelf(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_copy);
        lua_setfield(L, -2, "copy");

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_is_some);
        lua_setfield(L, -2, "is_some");

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_get_value);
        lua_setfield(L, -2, "get_value");

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_set_value);
        lua_setfield(L, -2, "set_value");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Option_AlignSelf_new);
        lua_setfield(L, -2, "new");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf_namespace);
}

/* -------------------------------------------------------------------------- */
/* Style */

/* Utility function: GridTracVec<TrackSizingFunction> --> lua table */
static void lua_push_table_from_taffy_GridTrackVec_of_TrackSizingFunction(lua_State* L, const taffy_GridTrackVec_of_TrackSizingFunction* vec)
{
    lua_newtable(L);
    {
        size_t i = 0;
        for(i = 0; i < vec->items_count; ++i)
        {
            const taffy_TrackSizingFunction* func = vec->items[i];

            taffy_TrackSizingFunction* copy = taffy_TrackSizingFunction_new_copy(func);
            if(copy != NULL)
            {
                taffy_TrackSizingFunction** user_data = (taffy_TrackSizingFunction**)lua_newuserdata(L, sizeof(taffy_TrackSizingFunction*));
                *user_data = copy;

                luaL_setmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);

                /* set: table[index] = 'value' */
                lua_rawseti(L, -2, (i+1)); /* NOTE: in Lua indexes starts from 1, thats why here: 'Lua-index = C-index + 1;' */
            }
        }
    }
}

/*
    Utility function: lua table --> GridTrackVec<TrackSizingFunction>

    ATTENTION: after use - dont forget to 'free()' memory, allocated by 'malloc()':

        if(items != NULL)
        {
            free(items);
            items = NULL;
        }
*/
static void lua_from_table_get_taffy_GridTrackVec_of_TrackSizingFunction(
    lua_State* L, int table_index,

    const taffy_TrackSizingFunction** * out_items,
    size_t * out_items_count
)
{
    const taffy_TrackSizingFunction** items = NULL;
    size_t items_count = 0;

    const size_t table_size = lua_rawlen(L, table_index);
    if(table_size > 0)
    {
        size_t index = 0;

        /* Allocate array with pointers into 'TrackSizingFunction's */
        items = (const taffy_TrackSizingFunction**)malloc(table_size * sizeof(taffy_TrackSizingFunction*));
        items_count = table_size;


        /* Iterate table */
        lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
        while( lua_next(L, table_index) != 0 )
        {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            /* const int value_type = lua_type(L, -1); */
            /* const int key_type   = lua_type(L, -2); */

            /*
                NOTE: here (for simplicity) we dont care about 'key'
                type and order - table may be array, sparced array,
                dictionary, anything, BUT its 'values' must be only
                'taffy_TrackSizingFunction' type.
            */
            taffy_TrackSizingFunction** value_value = (taffy_TrackSizingFunction**)luaL_checkudata(L, -1, LUA_META_OBJECT_taffy_TrackSizingFunction);

            items[index] = *value_value;

            index += 1;

            /* pop 'value' from the stack*/
            lua_pop(L, 1);
        }
        lua_pop(L, 1); /* pop 'key' from the stack */
    }

    /* return */
    *out_items       = items;
    *out_items_count = items_count;
}

static const char LUA_META_OBJECT_taffy_Style[]           = "taffy_Style_mt";
static const char LUA_META_OBJECT_taffy_Style_namespace[] = "taffy_Style_namespace_mt";

static int lua_taffy_Style_new(lua_State* L)
{
    taffy_Style* object_ptr = taffy_Style_new_default();

    if(object_ptr != NULL)
    {
        taffy_Style** user_data = (taffy_Style**)lua_newuserdata(L, sizeof(taffy_Style*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Style);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Style : taffy_Style_new_default() failed");
    }
}

static int lua_taffy_Style_copy(lua_State* L)
{
    taffy_Style** object = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Style* copy = taffy_Style_new_copy(*object);

    if(copy != NULL)
    {
        taffy_Style** user_data = (taffy_Style**)lua_newuserdata(L, sizeof(taffy_Style*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Style);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Style : taffy_Style_new_copy() failed");
    }
}

static int lua_taffy_Style_delete(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Style_delete(*self);

    return 0; /* number of results */
}

static int lua_taffy_Style_eq(lua_State* L)
{
    taffy_Style** object_lhs = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);
    taffy_Style** object_rhs = (taffy_Style**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Style);

    const int is_equal = taffy_Style_eq(*object_lhs, *object_rhs);

    lua_pushboolean(L, is_equal);

    return 1; /* number of results */
}

static int lua_taffy_Style_DEFAULT(lua_State* L)
{
    taffy_Style* object_ptr = taffy_Style_new_DEFAULT();

    if(object_ptr != NULL)
    {
        taffy_Style** user_data = (taffy_Style**)lua_newuserdata(L, sizeof(taffy_Style*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Style);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Style : taffy_Style_new_DEFAULT() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Style :: getters */

static int lua_taffy_Style_get_display(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Display display = taffy_Style_get_display(*self);

    taffy_Display* user_data = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
    *user_data = display;

    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_overflow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Point_of_Overflow* overflow = taffy_Style_get_overflow(*self);

    taffy_Point_of_Overflow* copy = taffy_Point_of_Overflow_new_copy(overflow);
    if(copy != NULL)
    {
        taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Point_of_Overflow : taffy_Point_of_Overflow_new_copy() failed");
    }
}

static int lua_taffy_Style_get_scrollbar_width(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const float scrollbar_width = taffy_Style_get_scrollbar_width(*self);

    lua_pushnumber(L, scrollbar_width);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_position(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Position position = taffy_Style_get_position(*self);

    taffy_Position* user_data = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
    *user_data = position;

    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_inset(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Rect_of_LengthPercentageAuto* inset = taffy_Style_get_inset(*self);

    taffy_Rect_of_LengthPercentageAuto* copy = taffy_Rect_of_LengthPercentageAuto_new_copy(inset);
    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Style_get_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Size_of_Dimension* size = taffy_Style_get_size(*self);

    taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(size);
    if(copy != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Style_get_min_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Size_of_Dimension* min_size = taffy_Style_get_min_size(*self);

    taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(min_size);
    if(copy != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Style_get_max_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Size_of_Dimension* max_size = taffy_Style_get_max_size(*self);

    taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(max_size);
    if(copy != NULL)
    {
        taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Style_get_aspect_ratio(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_float* aspect_ratio = taffy_Style_get_aspect_ratio(*self);

    taffy_Option_float* copy = taffy_Option_float_new_copy(aspect_ratio);
    if(copy != NULL)
    {
        taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_float : taffy_Option_float_new_copy() failed");
    }
}

static int lua_taffy_Style_get_margin(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Rect_of_LengthPercentageAuto* margin = taffy_Style_get_margin(*self);

    taffy_Rect_of_LengthPercentageAuto* copy = taffy_Rect_of_LengthPercentageAuto_new_copy(margin);
    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_copy() failed");
    }
}

static int lua_taffy_Style_get_padding(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Rect_of_LengthPercentage* padding = taffy_Style_get_padding(*self);

    taffy_Rect_of_LengthPercentage* copy = taffy_Rect_of_LengthPercentage_new_copy(padding);
    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Style_get_border(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Rect_of_LengthPercentage* border = taffy_Style_get_border(*self);

    taffy_Rect_of_LengthPercentage* copy = taffy_Rect_of_LengthPercentage_new_copy(border);
    if(copy != NULL)
    {
        taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Style_get_align_items(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_AlignItems* align_items = taffy_Style_get_align_items(*self);

    taffy_Option_AlignItems* copy = taffy_Option_AlignItems_new_copy(align_items);
    if(copy != NULL)
    {
        taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_AlignItems : taffy_Option_AlignItems_new_copy() failed");
    }
}

static int lua_taffy_Style_get_align_self(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_AlignSelf* align_self = taffy_Style_get_align_self(*self);

    taffy_Option_AlignSelf* copy = taffy_Option_AlignSelf_new_copy(align_self);
    if(copy != NULL)
    {
        taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_copy() failed");
    }
}

static int lua_taffy_Style_get_justify_items(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_AlignItems* justify_items = taffy_Style_get_justify_items(*self);

    taffy_Option_AlignItems* copy = taffy_Option_AlignItems_new_copy(justify_items);
    if(copy != NULL)
    {
        taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_AlignItems : taffy_Option_AlignItems_new_copy() failed");
    }
}

static int lua_taffy_Style_get_justify_self(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_AlignSelf* justify_self = taffy_Style_get_justify_self(*self);

    taffy_Option_AlignSelf* copy = taffy_Option_AlignSelf_new_copy(justify_self);
    if(copy != NULL)
    {
        taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_copy() failed");
    }
}

static int lua_taffy_Style_get_align_content(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_AlignContent* align_content = taffy_Style_get_align_content(*self);

    taffy_Option_AlignContent* copy = taffy_Option_AlignContent_new_copy(align_content);
    if(copy != NULL)
    {
        taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_AlignContent : taffy_Option_AlignContent_new_copy() failed");
    }
}

static int lua_taffy_Style_get_justify_content(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Option_JustifyContent* justify_content = taffy_Style_get_justify_content(*self);

    taffy_Option_JustifyContent* copy = taffy_Option_JustifyContent_new_copy(justify_content);
    if(copy != NULL)
    {
        taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_copy() failed");
    }
}

static int lua_taffy_Style_get_gap(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Size_of_LengthPercentage* gap = taffy_Style_get_gap(*self);

    taffy_Size_of_LengthPercentage* copy = taffy_Size_of_LengthPercentage_new_copy(gap);
    if(copy != NULL)
    {
        taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new_copy() failed");
    }
}

static int lua_taffy_Style_get_flex_direction(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_FlexDirection flex_direction = taffy_Style_get_flex_direction(*self);

    taffy_FlexDirection* user_data = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
    *user_data = flex_direction;

    luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_flex_wrap(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_FlexWrap flex_wrap = taffy_Style_get_flex_wrap(*self);

    taffy_FlexWrap* user_data = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
    *user_data = flex_wrap;

    luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_flex_basis(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Dimension* flex_basis = taffy_Style_get_flex_basis(*self);

    taffy_Dimension* copy = taffy_Dimension_new_copy(flex_basis);
    if(copy != NULL)
    {
        taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Dimension : taffy_Dimension_new_copy() failed");
    }
}

static int lua_taffy_Style_get_flex_grow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const float flex_grow = taffy_Style_get_flex_grow(*self);

    lua_pushnumber(L, flex_grow);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_flex_shrink(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const float flex_shrink = taffy_Style_get_flex_shrink(*self);

    lua_pushnumber(L, flex_shrink);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_template_rows(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_GridTrackVec_of_TrackSizingFunction grid_template_rows = taffy_Style_get_grid_template_rows(*self);

    lua_push_table_from_taffy_GridTrackVec_of_TrackSizingFunction(L, &grid_template_rows);

    taffy_GridTrackVec_of_TrackSizingFunction_delete(&grid_template_rows);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_template_columns(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_GridTrackVec_of_TrackSizingFunction grid_template_columns = taffy_Style_get_grid_template_columns(*self);

    lua_push_table_from_taffy_GridTrackVec_of_TrackSizingFunction(L, &grid_template_columns);

    taffy_GridTrackVec_of_TrackSizingFunction_delete(&grid_template_columns);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_auto_rows(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction grid_auto_rows = taffy_Style_get_grid_auto_rows(*self);

    lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(L, &grid_auto_rows);

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction_delete(&grid_auto_rows);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_auto_columns(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction grid_auto_columns = taffy_Style_get_grid_auto_columns(*self);

    lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(L, &grid_auto_columns);

    taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction_delete(&grid_auto_columns);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_auto_flow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_GridAutoFlow grid_auto_flow = taffy_Style_get_grid_auto_flow(*self);

    taffy_GridAutoFlow* user_data = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
    *user_data = grid_auto_flow;

    luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);

    return 1; /* number of results */
}

static int lua_taffy_Style_get_grid_row(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Line_of_GridPlacement* grid_row = taffy_Style_get_grid_row(*self);

    taffy_Line_of_GridPlacement* copy = taffy_Line_of_GridPlacement_new_copy(grid_row);
    if(copy != NULL)
    {
        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_copy() failed");
    }
}

static int lua_taffy_Style_get_grid_column(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const taffy_Line_of_GridPlacement* grid_column = taffy_Style_get_grid_column(*self);

    taffy_Line_of_GridPlacement* copy = taffy_Line_of_GridPlacement_new_copy(grid_column);
    if(copy != NULL)
    {
        taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
        *user_data = copy;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        return 1; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_copy() failed");
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Style :: setters */

static int lua_taffy_Style_set_display(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Display* display = (taffy_Display*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Display);

    taffy_Style_set_display(*self, *display);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_overflow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Point_of_Overflow** overflow = (taffy_Point_of_Overflow**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Point_of_Overflow);

    taffy_Style_set_overflow(*self, *overflow);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_scrollbar_width(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const lua_Number scrollbar_width = luaL_checknumber(L, 2);

    taffy_Style_set_scrollbar_width(*self, scrollbar_width);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_position(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Position* position = (taffy_Position*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Position);

    taffy_Style_set_position(*self, *position);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_inset(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Rect_of_LengthPercentageAuto** inset = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_Style_set_inset(*self, *inset);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Size_of_Dimension** size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Style_set_size(*self, *size);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_min_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Size_of_Dimension** min_size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Style_set_min_size(*self, *min_size);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_max_size(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Size_of_Dimension** max_size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_Dimension);

    taffy_Style_set_max_size(*self, *max_size);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_aspect_ratio(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_float** aspect_ratio = (taffy_Option_float**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_float);

    taffy_Style_set_aspect_ratio(*self, *aspect_ratio);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_margin(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Rect_of_LengthPercentageAuto** margin = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

    taffy_Style_set_margin(*self, *margin);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_padding(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Rect_of_LengthPercentage** padding = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_Style_set_padding(*self, *padding);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_border(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Rect_of_LengthPercentage** border = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

    taffy_Style_set_border(*self, *border);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_align_items(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_AlignItems** align_items = (taffy_Option_AlignItems**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignItems);

    taffy_Style_set_align_items(*self, *align_items);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_align_self(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_AlignSelf** align_self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignSelf);

    taffy_Style_set_align_self(*self, *align_self);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_justify_items(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_AlignItems** justify_items = (taffy_Option_AlignItems**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignItems);

    taffy_Style_set_justify_items(*self, *justify_items);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_justify_self(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_AlignSelf** justify_self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignSelf);

    taffy_Style_set_justify_self(*self, *justify_self);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_align_content(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_AlignContent** align_content = (taffy_Option_AlignContent**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_AlignContent);

    taffy_Style_set_align_content(*self, *align_content);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_justify_content(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Option_JustifyContent** justify_content = (taffy_Option_JustifyContent**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Option_JustifyContent);

    taffy_Style_set_justify_content(*self, *justify_content);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_gap(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Size_of_LengthPercentage** gap = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

    taffy_Style_set_gap(*self, *gap);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_flex_direction(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_FlexDirection* flex_direction = (taffy_FlexDirection*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_FlexDirection);

    taffy_Style_set_flex_direction(*self, *flex_direction);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_flex_wrap(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_FlexWrap* flex_wrap = (taffy_FlexWrap*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_FlexWrap);

    taffy_Style_set_flex_wrap(*self, *flex_wrap);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_flex_basis(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Dimension** flex_basis = (taffy_Dimension**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Dimension);

    taffy_Style_set_flex_basis(*self, *flex_basis);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_flex_grow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const lua_Number flex_grow = luaL_checknumber(L, 2);

    taffy_Style_set_flex_grow(*self, flex_grow);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_flex_shrink(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    const lua_Number flex_shrink = luaL_checknumber(L, 2);

    taffy_Style_set_flex_shrink(*self, flex_shrink);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_grid_template_rows(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    /* Interpret table - extract pointers to items */
    if( lua_type(L, 2) == LUA_TTABLE )
    {
        const taffy_TrackSizingFunction** items = NULL;
        size_t items_count = 0;

        lua_from_table_get_taffy_GridTrackVec_of_TrackSizingFunction(
            L, 2,

            &items, &items_count
        );

        taffy_Style_set_grid_template_rows(*self, items, items_count);

        /* Dont forget to 'free()' memory, allocated by 'malloc()' */
        if(items != NULL)
        {
            free(items);
            items = NULL;
        }

        return 0; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to set 'grid_template_rows' : second argument is not table of taffy_TrackSizingFunction");
    }
}

static int lua_taffy_Style_set_grid_template_columns(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    /* Interpret table - extract pointers to items */
    if( lua_type(L, 2) == LUA_TTABLE )
    {
        const taffy_TrackSizingFunction** items = NULL;
        size_t items_count = 0;

        lua_from_table_get_taffy_GridTrackVec_of_TrackSizingFunction(
            L, 2,

            &items, &items_count
        );

        taffy_Style_set_grid_template_columns(*self, items, items_count);

        /* Dont forget to 'free()' memory, allocated by 'malloc()' */
        if(items != NULL)
        {
            free(items);
            items = NULL;
        }

        return 0; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to set 'grid_template_columns' : second argument is not table of taffy_TrackSizingFunction");
    }
}

static int lua_taffy_Style_set_grid_auto_rows(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    /* Interpret table - extract pointers to items */
    if( lua_type(L, 2) == LUA_TTABLE )
    {
        const taffy_NonRepeatedTrackSizingFunction** items = NULL;
        size_t items_count = 0;

        lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
            L, 2,

            &items, &items_count
        );

        taffy_Style_set_grid_auto_rows(*self, items, items_count);

        /* Dont forget to 'free()' memory, allocated by 'malloc()' */
        if(items != NULL)
        {
            free(items);
            items = NULL;
        }

        return 0; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to set 'grid_auto_rows' : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
    }
}

static int lua_taffy_Style_set_grid_auto_columns(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    /* Interpret table - extract pointers to items */
    if( lua_type(L, 2) == LUA_TTABLE )
    {
        const taffy_NonRepeatedTrackSizingFunction** items = NULL;
        size_t items_count = 0;

        lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
            L, 2,

            &items, &items_count
        );

        taffy_Style_set_grid_auto_columns(*self, items, items_count);

        /* Dont forget to 'free()' memory, allocated by 'malloc()' */
        if(items != NULL)
        {
            free(items);
            items = NULL;
        }

        return 0; /* number of results */
    }
    else
    {
        return luaL_error(L, "Failed to set 'grid_auto_columns' : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
    }
}

static int lua_taffy_Style_set_grid_auto_flow(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_GridAutoFlow* grid_auto_flow = (taffy_GridAutoFlow*)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_GridAutoFlow);

    taffy_Style_set_grid_auto_flow(*self, *grid_auto_flow);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_grid_row(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Line_of_GridPlacement** grid_row = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_Style_set_grid_row(*self, *grid_row);

    return 0; /* number of results */
}

static int lua_taffy_Style_set_grid_column(lua_State* L)
{
    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);

    taffy_Line_of_GridPlacement** grid_column = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 2, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

    taffy_Style_set_grid_column(*self, *grid_column);

    return 0; /* number of results */
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static int lua_taffy_Style_index(lua_State* L)
{
    /*
        function mt.__index(table, key) <-- here is 'table' may be 'userdata'
            return table[key]
        end
    */

    /*
        NOTE: 'key' type may not be 'string' (for example: 'int'), but since we
        use use this function for indexing our known 'userdata', that have only
        function names as keys, we dont care about other types for simplicity.
    */

    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "display") == 0)
    {
        const taffy_Display display = taffy_Style_get_display(*self);

        taffy_Display* user_data = (taffy_Display*)lua_newuserdata(L, sizeof(taffy_Display));
        *user_data = display;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Display);

        return 1; /* number of results */
    }
    else if(strcmp(key, "overflow") == 0)
    {
        const taffy_Point_of_Overflow* overflow = taffy_Style_get_overflow(*self);

        taffy_Point_of_Overflow* copy = taffy_Point_of_Overflow_new_copy(overflow);
        if(copy != NULL)
        {
            taffy_Point_of_Overflow** user_data = (taffy_Point_of_Overflow**)lua_newuserdata(L, sizeof(taffy_Point_of_Overflow*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Point_of_Overflow);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Point_of_Overflow : taffy_Point_of_Overflow_new_copy() failed");
        }
    }
    else if(strcmp(key, "scrollbar_width") == 0)
    {
        const float scrollbar_width = taffy_Style_get_scrollbar_width(*self);

        lua_pushnumber(L, scrollbar_width);

        return 1; /* number of results */
    }
    else if(strcmp(key, "position") == 0)
    {
        const taffy_Position position = taffy_Style_get_position(*self);

        taffy_Position* user_data = (taffy_Position*)lua_newuserdata(L, sizeof(taffy_Position));
        *user_data = position;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Position);

        return 1; /* number of results */
    }
    else if(strcmp(key, "inset") == 0)
    {
        const taffy_Rect_of_LengthPercentageAuto* inset = taffy_Style_get_inset(*self);

        taffy_Rect_of_LengthPercentageAuto* copy = taffy_Rect_of_LengthPercentageAuto_new_copy(inset);
        if(copy != NULL)
        {
            taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_copy() failed");
        }
    }
    else if(strcmp(key, "size") == 0)
    {
        const taffy_Size_of_Dimension* size = taffy_Style_get_size(*self);

        taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(size);
        if(copy != NULL)
        {
            taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
        }
    }
    else if(strcmp(key, "min_size") == 0)
    {
        const taffy_Size_of_Dimension* min_size = taffy_Style_get_min_size(*self);

        taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(min_size);
        if(copy != NULL)
        {
            taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
        }
    }
    else if(strcmp(key, "max_size") == 0)
    {
        const taffy_Size_of_Dimension* max_size = taffy_Style_get_max_size(*self);

        taffy_Size_of_Dimension* copy = taffy_Size_of_Dimension_new_copy(max_size);
        if(copy != NULL)
        {
            taffy_Size_of_Dimension** user_data = (taffy_Size_of_Dimension**)lua_newuserdata(L, sizeof(taffy_Size_of_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Size_of_Dimension : taffy_Size_of_Dimension_new_copy() failed");
        }
    }
    else if(strcmp(key, "aspect_ratio") == 0)
    {
        const taffy_Option_float* aspect_ratio = taffy_Style_get_aspect_ratio(*self);

        taffy_Option_float* copy = taffy_Option_float_new_copy(aspect_ratio);
        if(copy != NULL)
        {
            taffy_Option_float** user_data = (taffy_Option_float**)lua_newuserdata(L, sizeof(taffy_Option_float*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_float);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_float : taffy_Option_float_new_copy() failed");
        }
    }
    else if(strcmp(key, "margin") == 0)
    {
        const taffy_Rect_of_LengthPercentageAuto* margin = taffy_Style_get_margin(*self);

        taffy_Rect_of_LengthPercentageAuto* copy = taffy_Rect_of_LengthPercentageAuto_new_copy(margin);
        if(copy != NULL)
        {
            taffy_Rect_of_LengthPercentageAuto** user_data = (taffy_Rect_of_LengthPercentageAuto**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentageAuto*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Rect_of_LengthPercentageAuto : taffy_Rect_of_LengthPercentageAuto_new_copy() failed");
        }
    }
    else if(strcmp(key, "padding") == 0)
    {
        const taffy_Rect_of_LengthPercentage* padding = taffy_Style_get_padding(*self);

        taffy_Rect_of_LengthPercentage* copy = taffy_Rect_of_LengthPercentage_new_copy(padding);
        if(copy != NULL)
        {
            taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "border") == 0)
    {
        const taffy_Rect_of_LengthPercentage* border = taffy_Style_get_border(*self);

        taffy_Rect_of_LengthPercentage* copy = taffy_Rect_of_LengthPercentage_new_copy(border);
        if(copy != NULL)
        {
            taffy_Rect_of_LengthPercentage** user_data = (taffy_Rect_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Rect_of_LengthPercentage*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Rect_of_LengthPercentage : taffy_Rect_of_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "align_items") == 0)
    {
        const taffy_Option_AlignItems* align_items = taffy_Style_get_align_items(*self);

        taffy_Option_AlignItems* copy = taffy_Option_AlignItems_new_copy(align_items);
        if(copy != NULL)
        {
            taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_AlignItems : taffy_Option_AlignItems_new_copy() failed");
        }
    }
    else if(strcmp(key, "align_self") == 0)
    {
        const taffy_Option_AlignSelf* align_self = taffy_Style_get_align_self(*self);

        taffy_Option_AlignSelf* copy = taffy_Option_AlignSelf_new_copy(align_self);
        if(copy != NULL)
        {
            taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_copy() failed");
        }
    }
    else if(strcmp(key, "justify_items") == 0)
    {
        const taffy_Option_AlignItems* justify_items = taffy_Style_get_justify_items(*self);

        taffy_Option_AlignItems* copy = taffy_Option_AlignItems_new_copy(justify_items);
        if(copy != NULL)
        {
            taffy_Option_AlignItems** user_data = (taffy_Option_AlignItems**)lua_newuserdata(L, sizeof(taffy_Option_AlignItems*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignItems);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_AlignItems : taffy_Option_AlignItems_new_copy() failed");
        }
    }
    else if(strcmp(key, "justify_self") == 0)
    {
        const taffy_Option_AlignSelf* justify_self = taffy_Style_get_justify_self(*self);

        taffy_Option_AlignSelf* copy = taffy_Option_AlignSelf_new_copy(justify_self);
        if(copy != NULL)
        {
            taffy_Option_AlignSelf** user_data = (taffy_Option_AlignSelf**)lua_newuserdata(L, sizeof(taffy_Option_AlignSelf*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignSelf);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_AlignSelf : taffy_Option_AlignSelf_new_copy() failed");
        }
    }
    else if(strcmp(key, "align_content") == 0)
    {
        const taffy_Option_AlignContent* align_content = taffy_Style_get_align_content(*self);

        taffy_Option_AlignContent* copy = taffy_Option_AlignContent_new_copy(align_content);
        if(copy != NULL)
        {
            taffy_Option_AlignContent** user_data = (taffy_Option_AlignContent**)lua_newuserdata(L, sizeof(taffy_Option_AlignContent*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_AlignContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_AlignContent : taffy_Option_AlignContent_new_copy() failed");
        }
    }
    else if(strcmp(key, "justify_content") == 0)
    {
        const taffy_Option_JustifyContent* justify_content = taffy_Style_get_justify_content(*self);

        taffy_Option_JustifyContent* copy = taffy_Option_JustifyContent_new_copy(justify_content);
        if(copy != NULL)
        {
            taffy_Option_JustifyContent** user_data = (taffy_Option_JustifyContent**)lua_newuserdata(L, sizeof(taffy_Option_JustifyContent*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Option_JustifyContent);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Option_JustifyContent : taffy_Option_JustifyContent_new_copy() failed");
        }
    }
    else if(strcmp(key, "gap") == 0)
    {
        const taffy_Size_of_LengthPercentage* gap = taffy_Style_get_gap(*self);

        taffy_Size_of_LengthPercentage* copy = taffy_Size_of_LengthPercentage_new_copy(gap);
        if(copy != NULL)
        {
            taffy_Size_of_LengthPercentage** user_data = (taffy_Size_of_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_Size_of_LengthPercentage*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Size_of_LengthPercentage : taffy_Size_of_LengthPercentage_new_copy() failed");
        }
    }
    else if(strcmp(key, "flex_direction") == 0)
    {
        const taffy_FlexDirection flex_direction = taffy_Style_get_flex_direction(*self);

        taffy_FlexDirection* user_data = (taffy_FlexDirection*)lua_newuserdata(L, sizeof(taffy_FlexDirection));
        *user_data = flex_direction;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexDirection);

        return 1; /* number of results */
    }
    else if(strcmp(key, "flex_wrap") == 0)
    {
        const taffy_FlexWrap flex_wrap = taffy_Style_get_flex_wrap(*self);

        taffy_FlexWrap* user_data = (taffy_FlexWrap*)lua_newuserdata(L, sizeof(taffy_FlexWrap));
        *user_data = flex_wrap;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_FlexWrap);

        return 1; /* number of results */
    }
    else if(strcmp(key, "flex_basis") == 0)
    {
        const taffy_Dimension* flex_basis = taffy_Style_get_flex_basis(*self);

        taffy_Dimension* copy = taffy_Dimension_new_copy(flex_basis);
        if(copy != NULL)
        {
            taffy_Dimension** user_data = (taffy_Dimension**)lua_newuserdata(L, sizeof(taffy_Dimension*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Dimension);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Dimension : taffy_Dimension_new_copy() failed");
        }
    }
    else if(strcmp(key, "flex_grow") == 0)
    {
        const float flex_grow = taffy_Style_get_flex_grow(*self);

        lua_pushnumber(L, flex_grow);

        return 1; /* number of results */
    }
    else if(strcmp(key, "flex_shrink") == 0)
    {
        const float flex_shrink = taffy_Style_get_flex_shrink(*self);

        lua_pushnumber(L, flex_shrink);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_template_rows") == 0)
    {
        taffy_GridTrackVec_of_TrackSizingFunction grid_template_rows = taffy_Style_get_grid_template_rows(*self);

        lua_push_table_from_taffy_GridTrackVec_of_TrackSizingFunction(L, &grid_template_rows);

        taffy_GridTrackVec_of_TrackSizingFunction_delete(&grid_template_rows);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_template_columns") == 0)
    {
        taffy_GridTrackVec_of_TrackSizingFunction grid_template_columns = taffy_Style_get_grid_template_columns(*self);

        lua_push_table_from_taffy_GridTrackVec_of_TrackSizingFunction(L, &grid_template_columns);

        taffy_GridTrackVec_of_TrackSizingFunction_delete(&grid_template_columns);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_auto_rows") == 0)
    {
        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction grid_auto_rows = taffy_Style_get_grid_auto_rows(*self);

        lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(L, &grid_auto_rows);

        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction_delete(&grid_auto_rows);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_auto_columns") == 0)
    {
        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction grid_auto_columns = taffy_Style_get_grid_auto_columns(*self);

        lua_push_table_from_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(L, &grid_auto_columns);

        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction_delete(&grid_auto_columns);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_auto_flow") == 0)
    {
        const taffy_GridAutoFlow grid_auto_flow = taffy_Style_get_grid_auto_flow(*self);

        taffy_GridAutoFlow* user_data = (taffy_GridAutoFlow*)lua_newuserdata(L, sizeof(taffy_GridAutoFlow));
        *user_data = grid_auto_flow;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_GridAutoFlow);

        return 1; /* number of results */
    }
    else if(strcmp(key, "grid_row") == 0)
    {
        const taffy_Line_of_GridPlacement* grid_row = taffy_Style_get_grid_row(*self);

        taffy_Line_of_GridPlacement* copy = taffy_Line_of_GridPlacement_new_copy(grid_row);
        if(copy != NULL)
        {
            taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_copy() failed");
        }
    }
    else if(strcmp(key, "grid_column") == 0)
    {
        const taffy_Line_of_GridPlacement* grid_column = taffy_Style_get_grid_column(*self);

        taffy_Line_of_GridPlacement* copy = taffy_Line_of_GridPlacement_new_copy(grid_column);
        if(copy != NULL)
        {
            taffy_Line_of_GridPlacement** user_data = (taffy_Line_of_GridPlacement**)lua_newuserdata(L, sizeof(taffy_Line_of_GridPlacement*));
            *user_data = copy;

            luaL_setmetatable(L, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

            return 1; /* number of results */
        }
        else
        {
            return luaL_error(L, "Cannot copy taffy_Line_of_GridPlacement : taffy_Line_of_GridPlacement_new_copy() failed");
        }
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Style);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1; /* number of results */
}

static int lua_taffy_Style_newindex(lua_State* L)
{
    /*
        function mt.__newindex(self, key, value)
            foo[key] = value
        end
    */

    taffy_Style** self = (taffy_Style**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Style);
    const char* key = luaL_checkstring(L, 2);


    if(strcmp(key, "display") == 0)
    {
        taffy_Display* display = (taffy_Display*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Display);

        taffy_Style_set_display(*self, *display);

        return 0; /* number of results */
    }
    else if( strcmp(key, "overflow") == 0)
    {
        taffy_Point_of_Overflow** overflow = (taffy_Point_of_Overflow**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Point_of_Overflow);

        taffy_Style_set_overflow(*self, *overflow);

        return 0; /* number of results */
    }
    else if( strcmp(key, "scrollbar_width") == 0)
    {
        const lua_Number scrollbar_width = luaL_checknumber(L, 3);

        taffy_Style_set_scrollbar_width(*self, scrollbar_width);

        return 0; /* number of results */
    }
    else if( strcmp(key, "position") == 0)
    {
        taffy_Position* position = (taffy_Position*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Position);

        taffy_Style_set_position(*self, *position);

        return 0; /* number of results */
    }
    else if( strcmp(key, "inset") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto** inset = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        taffy_Style_set_inset(*self, *inset);

        return 0; /* number of results */
    }
    else if( strcmp(key, "size") == 0)
    {
        taffy_Size_of_Dimension** size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Size_of_Dimension);

        taffy_Style_set_size(*self, *size);

        return 0; /* number of results */
    }
    else if( strcmp(key, "min_size") == 0)
    {
        taffy_Size_of_Dimension** min_size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Size_of_Dimension);

        taffy_Style_set_min_size(*self, *min_size);

        return 0; /* number of results */
    }
    else if( strcmp(key, "max_size") == 0)
    {
        taffy_Size_of_Dimension** max_size = (taffy_Size_of_Dimension**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Size_of_Dimension);

        taffy_Style_set_max_size(*self, *max_size);

        return 0; /* number of results */
    }
    else if( strcmp(key, "aspect_ratio") == 0)
    {
        taffy_Option_float** aspect_ratio = (taffy_Option_float**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_float);

        taffy_Style_set_aspect_ratio(*self, *aspect_ratio);

        return 0; /* number of results */
    }
    else if( strcmp(key, "margin") == 0)
    {
        taffy_Rect_of_LengthPercentageAuto** margin = (taffy_Rect_of_LengthPercentageAuto**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Rect_of_LengthPercentageAuto);

        taffy_Style_set_margin(*self, *margin);

        return 0; /* number of results */
    }
    else if( strcmp(key, "padding") == 0)
    {
        taffy_Rect_of_LengthPercentage** padding = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        taffy_Style_set_padding(*self, *padding);

        return 0; /* number of results */
    }
    else if( strcmp(key, "border") == 0)
    {
        taffy_Rect_of_LengthPercentage** border = (taffy_Rect_of_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Rect_of_LengthPercentage);

        taffy_Style_set_border(*self, *border);

        return 0; /* number of results */
    }
    else if( strcmp(key, "align_items") == 0)
    {
        taffy_Option_AlignItems** align_items = (taffy_Option_AlignItems**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_AlignItems);

        taffy_Style_set_align_items(*self, *align_items);

        return 0; /* number of results */
    }
    else if( strcmp(key, "align_self") == 0)
    {
        taffy_Option_AlignSelf** align_self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_AlignSelf);

        taffy_Style_set_align_self(*self, *align_self);

        return 0; /* number of results */
    }
    else if( strcmp(key, "justify_items") == 0)
    {
        taffy_Option_AlignItems** justify_items = (taffy_Option_AlignItems**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_AlignItems);

        taffy_Style_set_justify_items(*self, *justify_items);

        return 0; /* number of results */
    }
    else if( strcmp(key, "justify_self") == 0)
    {
        taffy_Option_AlignSelf** justify_self = (taffy_Option_AlignSelf**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_AlignSelf);

        taffy_Style_set_justify_self(*self, *justify_self);

        return 0; /* number of results */
    }
    else if( strcmp(key, "align_content") == 0)
    {
        taffy_Option_AlignContent** align_content = (taffy_Option_AlignContent**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_AlignContent);

        taffy_Style_set_align_content(*self, *align_content);

        return 0; /* number of results */
    }
    else if( strcmp(key, "justify_content") == 0)
    {
        taffy_Option_JustifyContent** justify_content = (taffy_Option_JustifyContent**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Option_JustifyContent);

        taffy_Style_set_justify_content(*self, *justify_content);

        return 0; /* number of results */
    }
    else if( strcmp(key, "gap") == 0)
    {
        taffy_Size_of_LengthPercentage** gap = (taffy_Size_of_LengthPercentage**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Size_of_LengthPercentage);

        taffy_Style_set_gap(*self, *gap);

        return 0; /* number of results */
    }
    else if( strcmp(key, "flex_direction") == 0)
    {
        taffy_FlexDirection* flex_direction = (taffy_FlexDirection*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_FlexDirection);

        taffy_Style_set_flex_direction(*self, *flex_direction);

        return 0; /* number of results */
    }
    else if( strcmp(key, "flex_wrap") == 0)
    {
        taffy_FlexWrap* flex_wrap = (taffy_FlexWrap*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_FlexWrap);

        taffy_Style_set_flex_wrap(*self, *flex_wrap);

        return 0; /* number of results */
    }
    else if( strcmp(key, "flex_basis") == 0)
    {
        taffy_Dimension** flex_basis = (taffy_Dimension**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Dimension);

        taffy_Style_set_flex_basis(*self, *flex_basis);

        return 0; /* number of results */
    }
    else if( strcmp(key, "flex_grow") == 0)
    {
        const lua_Number flex_grow = luaL_checknumber(L, 3);

        taffy_Style_set_flex_grow(*self, flex_grow);

        return 0; /* number of results */
    }
    else if( strcmp(key, "flex_shrink") == 0)
    {
        const lua_Number flex_shrink = luaL_checknumber(L, 3);

        taffy_Style_set_flex_shrink(*self, flex_shrink);

        return 0; /* number of results */
    }
    else if( strcmp(key, "grid_template_rows") == 0)
    {
        /* Interpret table - extract pointers to items */
        if( lua_type(L, 3) == LUA_TTABLE )
        {
            const taffy_TrackSizingFunction** items = NULL;
            size_t items_count = 0;

            lua_from_table_get_taffy_GridTrackVec_of_TrackSizingFunction(
                L, 3,

                &items, &items_count
            );

            taffy_Style_set_grid_template_rows(*self, items, items_count);

            /* Dont forget to 'free()' memory, allocated by 'malloc()' */
            if(items != NULL)
            {
                free(items);
                items = NULL;
            }

            return 0; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to set 'grid_template_rows' : second argument is not table of taffy_TrackSizingFunction");
        }
    }
    else if( strcmp(key, "grid_template_columns") == 0)
    {
        /* Interpret table - extract pointers to items */
        if( lua_type(L, 3) == LUA_TTABLE )
        {
            const taffy_TrackSizingFunction** items = NULL;
            size_t items_count = 0;

            lua_from_table_get_taffy_GridTrackVec_of_TrackSizingFunction(
                L, 3,

                &items, &items_count
            );

            taffy_Style_set_grid_template_columns(*self, items, items_count);

            /* Dont forget to 'free()' memory, allocated by 'malloc()' */
            if(items != NULL)
            {
                free(items);
                items = NULL;
            }

            return 0; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to set 'grid_template_columns' : second argument is not table of taffy_TrackSizingFunction");
        }
    }
    else if( strcmp(key, "grid_auto_rows") == 0)
    {
        /* Interpret table - extract pointers to items */
        if( lua_type(L, 3) == LUA_TTABLE )
        {
            const taffy_NonRepeatedTrackSizingFunction** items = NULL;
            size_t items_count = 0;

            lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
                L, 3,

                &items, &items_count
            );

            taffy_Style_set_grid_auto_rows(*self, items, items_count);

            /* Dont forget to 'free()' memory, allocated by 'malloc()' */
            if(items != NULL)
            {
                free(items);
                items = NULL;
            }

            return 0; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to set 'grid_auto_rows' : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
        }
    }
    else if( strcmp(key, "grid_auto_columns") == 0)
    {
        /* Interpret table - extract pointers to items */
        if( lua_type(L, 3) == LUA_TTABLE )
        {
            const taffy_NonRepeatedTrackSizingFunction** items = NULL;
            size_t items_count = 0;

            lua_from_table_get_taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction(
                L, 3,

                &items, &items_count
            );

            taffy_Style_set_grid_auto_columns(*self, items, items_count);

            /* Dont forget to 'free()' memory, allocated by 'malloc()' */
            if(items != NULL)
            {
                free(items);
                items = NULL;
            }

            return 0; /* number of results */
        }
        else
        {
            return luaL_error(L, "Failed to set 'grid_auto_columns' : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
        }
    }
    else if( strcmp(key, "grid_auto_flow") == 0)
    {
        taffy_GridAutoFlow* grid_auto_flow = (taffy_GridAutoFlow*)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_GridAutoFlow);

        taffy_Style_set_grid_auto_flow(*self, *grid_auto_flow);

        return 0; /* number of results */
    }
    else if( strcmp(key, "grid_row") == 0)
    {
        taffy_Line_of_GridPlacement** grid_row = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        taffy_Style_set_grid_row(*self, *grid_row);

        return 0; /* number of results */
    }
    else if( strcmp(key, "grid_column") == 0)
    {
        taffy_Line_of_GridPlacement** grid_column = (taffy_Line_of_GridPlacement**)luaL_checkudata(L, 3, LUA_META_OBJECT_taffy_Line_of_GridPlacement);

        taffy_Style_set_grid_column(*self, *grid_column);

        return 0; /* number of results */
    }

    return luaL_error(L, "taffy_Style 'newindex' failed"); /* TODO: better message*/
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void lua_push_table_taffy_Style(lua_State* L)
{
    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Style) )
    {
        lua_pushcfunction(L, lua_taffy_Style_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_taffy_Style_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_taffy_Style_delete);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_taffy_Style_eq);
        lua_setfield(L, -2, "__eq");

        /* metatable.__metatable = "message" <-- metatable protection */
        lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
        lua_setfield(L, -2, "__metatable");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Style_copy);
        lua_setfield(L, -2, "copy");

        /* ------------------------------------------------------------------ */
        /* Getters */

        lua_pushcfunction(L, lua_taffy_Style_get_display);
        lua_setfield(L, -2, "get_display");

        lua_pushcfunction(L, lua_taffy_Style_get_overflow);
        lua_setfield(L, -2, "get_overflow");

        lua_pushcfunction(L, lua_taffy_Style_get_scrollbar_width);
        lua_setfield(L, -2, "get_scrollbar_width");

        lua_pushcfunction(L, lua_taffy_Style_get_position);
        lua_setfield(L, -2, "get_position");

        lua_pushcfunction(L, lua_taffy_Style_get_inset);
        lua_setfield(L, -2, "get_inset");

        lua_pushcfunction(L, lua_taffy_Style_get_size);
        lua_setfield(L, -2, "get_size");

        lua_pushcfunction(L, lua_taffy_Style_get_min_size);
        lua_setfield(L, -2, "get_min_size");

        lua_pushcfunction(L, lua_taffy_Style_get_max_size);
        lua_setfield(L, -2, "get_max_size");

        lua_pushcfunction(L, lua_taffy_Style_get_aspect_ratio);
        lua_setfield(L, -2, "get_aspect_ratio");

        lua_pushcfunction(L, lua_taffy_Style_get_margin);
        lua_setfield(L, -2, "get_margin");

        lua_pushcfunction(L, lua_taffy_Style_get_padding);
        lua_setfield(L, -2, "get_padding");

        lua_pushcfunction(L, lua_taffy_Style_get_border);
        lua_setfield(L, -2, "get_border");

        lua_pushcfunction(L, lua_taffy_Style_get_align_items);
        lua_setfield(L, -2, "get_align_items");

        lua_pushcfunction(L, lua_taffy_Style_get_align_self);
        lua_setfield(L, -2, "get_align_self");

        lua_pushcfunction(L, lua_taffy_Style_get_justify_items);
        lua_setfield(L, -2, "get_justify_items");

        lua_pushcfunction(L, lua_taffy_Style_get_justify_self);
        lua_setfield(L, -2, "get_justify_self");

        lua_pushcfunction(L, lua_taffy_Style_get_align_content);
        lua_setfield(L, -2, "get_align_content");

        lua_pushcfunction(L, lua_taffy_Style_get_justify_content);
        lua_setfield(L, -2, "get_justify_content");

        lua_pushcfunction(L, lua_taffy_Style_get_gap);
        lua_setfield(L, -2, "get_gap");

        lua_pushcfunction(L, lua_taffy_Style_get_flex_direction);
        lua_setfield(L, -2, "get_flex_direction");

        lua_pushcfunction(L, lua_taffy_Style_get_flex_wrap);
        lua_setfield(L, -2, "get_flex_wrap");

        lua_pushcfunction(L, lua_taffy_Style_get_flex_basis);
        lua_setfield(L, -2, "get_flex_basis");

        lua_pushcfunction(L, lua_taffy_Style_get_flex_grow);
        lua_setfield(L, -2, "get_flex_grow");

        lua_pushcfunction(L, lua_taffy_Style_get_flex_shrink);
        lua_setfield(L, -2, "get_flex_shrink");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_template_rows);
        lua_setfield(L, -2, "get_grid_template_rows");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_template_columns);
        lua_setfield(L, -2, "get_grid_template_columns");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_auto_rows);
        lua_setfield(L, -2, "get_grid_auto_rows");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_auto_columns);
        lua_setfield(L, -2, "get_grid_auto_columns");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_auto_flow);
        lua_setfield(L, -2, "get_grid_auto_flow");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_row);
        lua_setfield(L, -2, "get_grid_row");

        lua_pushcfunction(L, lua_taffy_Style_get_grid_column);
        lua_setfield(L, -2, "get_grid_column");

        /* ------------------------------------------------------------------ */
        /* Setters */

        lua_pushcfunction(L, lua_taffy_Style_set_display);
        lua_setfield(L, -2, "set_display");

        lua_pushcfunction(L, lua_taffy_Style_set_overflow);
        lua_setfield(L, -2, "set_overflow");

        lua_pushcfunction(L, lua_taffy_Style_set_scrollbar_width);
        lua_setfield(L, -2, "set_scrollbar_width");

        lua_pushcfunction(L, lua_taffy_Style_set_position);
        lua_setfield(L, -2, "set_position");

        lua_pushcfunction(L, lua_taffy_Style_set_inset);
        lua_setfield(L, -2, "set_inset");

        lua_pushcfunction(L, lua_taffy_Style_set_size);
        lua_setfield(L, -2, "set_size");

        lua_pushcfunction(L, lua_taffy_Style_set_min_size);
        lua_setfield(L, -2, "set_min_size");

        lua_pushcfunction(L, lua_taffy_Style_set_max_size);
        lua_setfield(L, -2, "set_max_size");

        lua_pushcfunction(L, lua_taffy_Style_set_aspect_ratio);
        lua_setfield(L, -2, "set_aspect_ratio");

        lua_pushcfunction(L, lua_taffy_Style_set_margin);
        lua_setfield(L, -2, "set_margin");

        lua_pushcfunction(L, lua_taffy_Style_set_padding);
        lua_setfield(L, -2, "set_padding");

        lua_pushcfunction(L, lua_taffy_Style_set_border);
        lua_setfield(L, -2, "set_border");

        lua_pushcfunction(L, lua_taffy_Style_set_align_items);
        lua_setfield(L, -2, "set_align_items");

        lua_pushcfunction(L, lua_taffy_Style_set_align_self);
        lua_setfield(L, -2, "set_align_self");

        lua_pushcfunction(L, lua_taffy_Style_set_justify_items);
        lua_setfield(L, -2, "set_justify_items");

        lua_pushcfunction(L, lua_taffy_Style_set_justify_self);
        lua_setfield(L, -2, "set_justify_self");

        lua_pushcfunction(L, lua_taffy_Style_set_align_content);
        lua_setfield(L, -2, "set_align_content");

        lua_pushcfunction(L, lua_taffy_Style_set_justify_content);
        lua_setfield(L, -2, "set_justify_content");

        lua_pushcfunction(L, lua_taffy_Style_set_gap);
        lua_setfield(L, -2, "set_gap");

        lua_pushcfunction(L, lua_taffy_Style_set_flex_direction);
        lua_setfield(L, -2, "set_flex_direction");

        lua_pushcfunction(L, lua_taffy_Style_set_flex_wrap);
        lua_setfield(L, -2, "set_flex_wrap");

        lua_pushcfunction(L, lua_taffy_Style_set_flex_basis);
        lua_setfield(L, -2, "set_flex_basis");

        lua_pushcfunction(L, lua_taffy_Style_set_flex_grow);
        lua_setfield(L, -2, "set_flex_grow");

        lua_pushcfunction(L, lua_taffy_Style_set_flex_shrink);
        lua_setfield(L, -2, "set_flex_shrink");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_template_rows);
        lua_setfield(L, -2, "set_grid_template_rows");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_template_columns);
        lua_setfield(L, -2, "set_grid_template_columns");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_auto_rows);
        lua_setfield(L, -2, "set_grid_auto_rows");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_auto_columns);
        lua_setfield(L, -2, "set_grid_auto_columns");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_auto_flow);
        lua_setfield(L, -2, "set_grid_auto_flow");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_row);
        lua_setfield(L, -2, "set_grid_row");

        lua_pushcfunction(L, lua_taffy_Style_set_grid_column);
        lua_setfield(L, -2, "set_grid_column");
    }
    lua_pop(L, 1);

    if( luaL_newmetatable(L, LUA_META_OBJECT_taffy_Style_namespace) )
    {
        /* metatable.__index = metatable */
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_newindex_disabled);
        lua_setfield(L, -2, "__newindex");

        /* ------------------------------------------------------------------ */

        lua_pushcfunction(L, lua_taffy_Style_new);
        lua_setfield(L, -2, "new");

        lua_pushcfunction(L, lua_taffy_Style_DEFAULT);
        lua_setfield(L, -2, "DEFAULT");
    }
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setmetatable(L, LUA_META_OBJECT_taffy_Style_namespace);
}

/* -------------------------------------------------------------------------- */
/* luaopen_<name_as_required> */
int luaopen_libtaffy_cpp_lua(lua_State* L);
int luaopen_libtaffy_cpp_lua(lua_State* L)
{
    /*
        Checks whether the code making the call and the Lua library being called
        are using the same version of Lua and the same numeric types.
    */
    luaL_checkversion(L);

    /*
        Create an anonymous table to hold: enums, funcs and classes
        (and its methods)
    */
    lua_newtable(L);
    {
        /* Register Option<float> */
        {
            lua_push_table_taffy_Option_float(L);
            lua_setfield(L, -2, "Option_float");
        }

        /* Register Point<float> */
        {
            lua_push_table_taffy_Point_of_float(L);
            lua_setfield(L, -2, "Point_of_float");
        }

        /* Register Size<float> */
        {
            lua_push_table_taffy_Size_of_float(L);
            lua_setfield(L, -2, "Size_of_float");
        }

        /* Register AlignContent (enum) */
        {
            lua_push_table_taffy_AlignContent(L);
            lua_setfield(L, -2, "AlignContent");
        }

        /* Register JustifyContent (enum) (same as AlignContent) */
        {
            lua_push_table_taffy_JustifyContent(L);
            lua_setfield(L, -2, "JustifyContent");
        }

        /* Register AlignItems (enum) */
        {
            lua_push_table_taffy_AlignItems(L);
            lua_setfield(L, -2, "AlignItems");
        }

        /* Register AlignSelf (enum) (same as AlignItems) */
        {
            lua_push_table_taffy_AlignSelf(L);
            lua_setfield(L, -2, "AlignSelf");
        }

        /* Register AvailableSpace */
        {
            lua_push_table_taffy_AvailableSpace(L);
            lua_setfield(L, -2, "AvailableSpace");
        }

        /* Register LengthPercentage */
        {
            lua_push_table_taffy_LengthPercentage(L);
            lua_setfield(L, -2, "LengthPercentage");
        }

        /* Register LengthPercentageAuto */
        {
            lua_push_table_taffy_LengthPercentageAuto(L);
            lua_setfield(L, -2, "LengthPercentageAuto");
        }

        /* Register Dimension */
        {
            lua_push_table_taffy_Dimension(L);
            lua_setfield(L, -2, "Dimension");
        }

        /* Register FlexDirection (enum) */
        {
            lua_push_table_taffy_FlexDirection(L);
            lua_setfield(L, -2, "FlexDirection");
        }

        /* Register FlexWrap (enum) */
        {
            lua_push_table_taffy_FlexWrap(L);
            lua_setfield(L, -2, "FlexWrap");
        }

        /* Register GridAutoFlow (enum) */
        {
            lua_push_table_taffy_GridAutoFlow(L);
            lua_setfield(L, -2, "GridAutoFlow");
        }

        /* Register GridPlacement */
        {
            lua_push_table_taffy_GridPlacement(L);
            lua_setfield(L, -2, "GridPlacement");
        }

        /* Register GridTrackRepetition */
        {
            lua_push_table_taffy_GridTrackRepetition(L);
            lua_setfield(L, -2, "GridTrackRepetition");
        }

        /* Register MaxTrackSizingFunction */
        {
            lua_push_table_taffy_MaxTrackSizingFunction(L);
            lua_setfield(L, -2, "MaxTrackSizingFunction");
        }

        /* Register MinTrackSizingFunction */
        {
            lua_push_table_taffy_MinTrackSizingFunction(L);
            lua_setfield(L, -2, "MinTrackSizingFunction");
        }

        /* Register NonRepeatedTrackSizingFunction */
        {
            lua_push_table_taffy_NonRepeatedTrackSizingFunction(L);
            lua_setfield(L, -2, "NonRepeatedTrackSizingFunction");
        }

        /* Register TrackSizingFunction */
        {
            lua_push_table_taffy_TrackSizingFunction(L);
            lua_setfield(L, -2, "TrackSizingFunction");
        }

        /* Register Display (enum) */
        {
            lua_push_table_taffy_Display(L);
            lua_setfield(L, -2, "Display");
        }

        /* Register Overflow (enum) */
        {
            lua_push_table_taffy_Overflow(L);
            lua_setfield(L, -2, "Overflow");
        }

        /* Register Position (enum) */
        {
            lua_push_table_taffy_Position(L);
            lua_setfield(L, -2, "Position");
        }

        /* Register Point<Overflow> */
        {
            lua_push_table_taffy_Point_of_Overflow(L);
            lua_setfield(L, -2, "Point_of_Overflow");
        }

        /* Register Rect<LengthPercentage> */
        {
            lua_push_table_taffy_Rect_of_LengthPercentage(L);
            lua_setfield(L, -2, "Rect_of_LengthPercentage");
        }

        /* Register Rect<LengthPercentageAuto> */
        {
            lua_push_table_taffy_Rect_of_LengthPercentageAuto(L);
            lua_setfield(L, -2, "Rect_of_LengthPercentageAuto");
        }

        /* Register Size<LengthPercentage> */
        {
            lua_push_table_taffy_Size_of_LengthPercentage(L);
            lua_setfield(L, -2, "Size_of_LengthPercentage");
        }

        /* Register Size<Dimension> */
        {
            lua_push_table_taffy_Size_of_Dimension(L);
            lua_setfield(L, -2, "Size_of_Dimension");
        }

        /* Register Line<GridPlacement> */
        {
            lua_push_table_taffy_Line_of_GridPlacement(L);
            lua_setfield(L, -2, "Line_of_GridPlacement");
        }

        /* Register Option<AlignContent> */
        {
            lua_push_table_taffy_Option_AlignContent(L);
            lua_setfield(L, -2, "Option_AlignContent");
        }

        /* Register Option<JustifyContent> */
        {
            lua_push_table_taffy_Option_JustifyContent(L);
            lua_setfield(L, -2, "Option_JustifyContent");
        }

        /* Register Option<AlignItems> */
        {
            lua_push_table_taffy_Option_AlignItems(L);
            lua_setfield(L, -2, "Option_AlignItems");
        }

        /* Register Option<AlignSelf> */
        {
            lua_push_table_taffy_Option_AlignSelf(L);
            lua_setfield(L, -2, "Option_AlignSelf");
        }

        /* Register Style */
        {
            lua_push_table_taffy_Style(L);
            lua_setfield(L, -2, "Style");
        }
    }

    return 1; /* number of results */
}