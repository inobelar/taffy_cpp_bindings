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

static const char LUA_META_OBJECT_taffy_Size_of_float[] = "taffy_Size_of_float_mt";
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
    {
        if( lua_type(L, 2) == LUA_TTABLE )
        {
            const size_t table_size = lua_rawlen(L, 2);
            if(table_size > 0)
            {
                size_t index = 0;

                /* Allocate array with pointers into 'NonRepeatedTrackSizingFunction's */
                items = (const taffy_NonRepeatedTrackSizingFunction**)malloc(table_size * sizeof(taffy_NonRepeatedTrackSizingFunction*));
                items_count = table_size;


                /* Iterate table */
                lua_pushnil(L); /* key ( reusable by 'lua_next()' ) */
                while( lua_next(L, 2) != 0 )
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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_TrackSizingFunction : second argument is not table of taffy_NonRepeatedTrackSizingFunction");
        }
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

        lua_newtable(L);
        {
            size_t i = 0;
            for(i = 0; i < repeat_funcs.items_count; ++i)
            {
                const taffy_NonRepeatedTrackSizingFunction* func = repeat_funcs.items[i];

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
                            x_found = 1; /* true */
                            x = *value_value;
                        }
                        else if(key_value == 2.0f) /* 'second' index (in C its '1', in Lua its '2') is 'y' */
                        {
                            y_found = 1; /* true */
                            y = *value_value;
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

                    {x = 35, y = 42}

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
/* luaopen_<name_as_required>*/
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
    }

    return 1; /* number of results */
}