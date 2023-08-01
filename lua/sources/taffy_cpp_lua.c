#include <taffy_cpp_lua.h>

/* taffy_cpp C library */
#include <taffy_cpp_c.h>

/* Lua C api */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h> /* for: malloc(), free() */
#include <string.h> /* for: strcmp() */

/* -------------------------------------------------------------------------- */
/* Option<float> */

#define LUA_META_OBJECT_taffy_Option_float "taffy_Option_float_mt"

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
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    taffy_Option_float_delete(*object);

    return 0; /* number of results */
}

static int lua_taffy_Option_float_is_some(lua_State* L)
{
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    const int is_some = taffy_Option_float_is_some(*object);

    lua_pushboolean(L, is_some);

    return 1; /* number of results */
}

static int lua_taffy_Option_float_get_value(lua_State* L)
{
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    if( taffy_Option_float_is_some(*object) )
    {
        const float value = taffy_Option_float_get_value(*object);

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
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    switch( lua_type(L, 2) ) {

    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_float_set_value(*object, NULL);

        return 0; /* number of results */
    } break;

    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 2);
        const float f_value = (float)(value);

        taffy_Option_float_set_value(*object, &f_value);

        return 0; /* number of results */
    } break;
    }

    return luaL_error(L, "Failed to set_value taffy_Option_float : incorrect argument type");
}

/* -------------------------------------------------------------------------- */
/* Point<float> */

#define LUA_META_OBJECT_taffy_Point_of_float "taffy_Point_of_float_mt"

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
                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        const lua_Number value_value = lua_tonumber(L, -1); /* pop 'value' */
                        const lua_Number key_value   = lua_tonumber(L, -2); /* pop 'key'   */

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
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    taffy_Point_of_float* copy = taffy_Point_of_float_new_copy(*object);

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
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    taffy_Point_of_float_delete(*object);

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
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const float x = taffy_Point_of_float_get_x(*object);

    lua_pushnumber(L, x);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_get_y(lua_State* L)
{
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const float y = taffy_Point_of_float_get_y(*object);

    lua_pushnumber(L, y);

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_set_x(lua_State* L)
{
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const lua_Number x = luaL_checknumber(L, 2);

    taffy_Point_of_float_set_x(*object, x);

    return 0; /* number of results */
}

static int lua_taffy_Point_of_float_set_y(lua_State* L)
{
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);

    const lua_Number y = luaL_checknumber(L, 2);

    taffy_Point_of_float_set_y(*object, y);

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

/* -------------------------------------------------------------------------- */
/* Size<float> */

#define LUA_META_OBJECT_taffy_Size_of_float "taffy_Size_of_float_mt"

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
                        lua_pushvalue(L, -2); /* copy 'key'   */
                        lua_pushvalue(L, -2); /* copy 'value' */

                        const lua_Number value_value = lua_tonumber(L, -1); /* pop 'value' */
                        const lua_Number key_value   = lua_tonumber(L, -2); /* pop 'key'   */

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
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float* copy = taffy_Size_of_float_new_copy(*object);

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
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float_delete(*object);

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
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const float width = taffy_Size_of_float_get_width(*object);

    lua_pushnumber(L, width);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_get_height(lua_State* L)
{
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const float height = taffy_Size_of_float_get_height(*object);

    lua_pushnumber(L, height);

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_set_width(lua_State* L)
{
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const lua_Number width = luaL_checknumber(L, 2);

    taffy_Size_of_float_set_width(*object, width);

    return 0; /* number of results */
}

static int lua_taffy_Size_of_float_set_height(lua_State* L)
{
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    const lua_Number height = luaL_checknumber(L, 2);

    taffy_Size_of_float_set_height(*object, height);

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

/* -------------------------------------------------------------------------- */
/* AlignContent */

static void lua_push_table_taffy_AlignContent(lua_State* L)
{
    /* Register AlignContent enum. Same as in Lua:

        AlignContent = {
            ["Start"] = 0,
            ["End"]   = 1,
            ...
        }
    */
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_AlignContent_Start       ); lua_setfield(L, -2, "Start");
        lua_pushnumber(L, taffy_AlignContent_End         ); lua_setfield(L, -2, "End");
        lua_pushnumber(L, taffy_AlignContent_FlexStart   ); lua_setfield(L, -2, "FlexStart");
        lua_pushnumber(L, taffy_AlignContent_FlexEnd     ); lua_setfield(L, -2, "FlexEnd");
        lua_pushnumber(L, taffy_AlignContent_Center      ); lua_setfield(L, -2, "Center");
        lua_pushnumber(L, taffy_AlignContent_Stretch     ); lua_setfield(L, -2, "Stretch");
        lua_pushnumber(L, taffy_AlignContent_SpaceBetween); lua_setfield(L, -2, "SpaceBetween");
        lua_pushnumber(L, taffy_AlignContent_SpaceEvenly ); lua_setfield(L, -2, "SpaceEvenly");
        lua_pushnumber(L, taffy_AlignContent_SpaceAround ); lua_setfield(L, -2, "SpaceAround");
    }
}

/* -------------------------------------------------------------------------- */
/* JustifyContent (same as AlignContent) */

static void lua_push_table_taffy_JustifyContent(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_JustifyContent_Start       ); lua_setfield(L, -2, "Start");
        lua_pushnumber(L, taffy_JustifyContent_End         ); lua_setfield(L, -2, "End");
        lua_pushnumber(L, taffy_JustifyContent_FlexStart   ); lua_setfield(L, -2, "FlexStart");
        lua_pushnumber(L, taffy_JustifyContent_FlexEnd     ); lua_setfield(L, -2, "FlexEnd");
        lua_pushnumber(L, taffy_JustifyContent_Center      ); lua_setfield(L, -2, "Center");
        lua_pushnumber(L, taffy_JustifyContent_Stretch     ); lua_setfield(L, -2, "Stretch");
        lua_pushnumber(L, taffy_JustifyContent_SpaceBetween); lua_setfield(L, -2, "SpaceBetween");
        lua_pushnumber(L, taffy_JustifyContent_SpaceEvenly ); lua_setfield(L, -2, "SpaceEvenly");
        lua_pushnumber(L, taffy_JustifyContent_SpaceAround ); lua_setfield(L, -2, "SpaceAround");
    }
}

/* -------------------------------------------------------------------------- */
/* AlignItems */

static void lua_push_table_taffy_AlignItems(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_AlignItems_Start    ); lua_setfield(L, -2, "Start");
        lua_pushnumber(L, taffy_AlignItems_End      ); lua_setfield(L, -2, "End");
        lua_pushnumber(L, taffy_AlignItems_FlexStart); lua_setfield(L, -2, "FlexStart");
        lua_pushnumber(L, taffy_AlignItems_FlexEnd  ); lua_setfield(L, -2, "FlexEnd");
        lua_pushnumber(L, taffy_AlignItems_Center   ); lua_setfield(L, -2, "Center");
        lua_pushnumber(L, taffy_AlignItems_Baseline ); lua_setfield(L, -2, "Baseline");
        lua_pushnumber(L, taffy_AlignItems_Stretch  ); lua_setfield(L, -2, "Stretch");
    }
}

/* -------------------------------------------------------------------------- */
/* AlignSelf (same as AlignItems) */

static void lua_push_table_taffy_AlignSelf(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_AlignSelf_Start    ); lua_setfield(L, -2, "Start");
        lua_pushnumber(L, taffy_AlignSelf_End      ); lua_setfield(L, -2, "End");
        lua_pushnumber(L, taffy_AlignSelf_FlexStart); lua_setfield(L, -2, "FlexStart");
        lua_pushnumber(L, taffy_AlignSelf_FlexEnd  ); lua_setfield(L, -2, "FlexEnd");
        lua_pushnumber(L, taffy_AlignSelf_Center   ); lua_setfield(L, -2, "Center");
        lua_pushnumber(L, taffy_AlignSelf_Baseline ); lua_setfield(L, -2, "Baseline");
        lua_pushnumber(L, taffy_AlignSelf_Stretch  ); lua_setfield(L, -2, "Stretch");
    }
}

/* -------------------------------------------------------------------------- */
/* AvailableSpace */

#define LUA_META_OBJECT_taffy_AvailableSpace "taffy_AvailableSpace_mt"

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
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    taffy_AvailableSpace* copy = taffy_AvailableSpace_new_copy(*object);

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
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    taffy_AvailableSpace_delete(*object);

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
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_Definite = taffy_AvailableSpace_is_Definite(*object);

    lua_pushboolean(L, is_Definite);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_is_MinContent(lua_State* L)
{
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_MinContent = taffy_AvailableSpace_is_MinContent(*object);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_is_MaxContent(lua_State* L)
{
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    const int is_MaxContent = taffy_AvailableSpace_is_MaxContent(*object);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_get_value(lua_State* L)
{
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    if( taffy_AvailableSpace_is_Definite(*object) )
    {
        const float value = taffy_AvailableSpace_get_value(*object);

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

/* -------------------------------------------------------------------------- */
/* LengthPercentage */

#define LUA_META_OBJECT_taffy_LengthPercentage "taffy_LengthPercentage_mt"

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
    taffy_LengthPercentage** object = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentage* copy = taffy_LengthPercentage_new_copy(*object);

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
    taffy_LengthPercentage** object = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentage_delete(*object);

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

/* -------------------------------------------------------------------------- */
/* LengthPercentageAuto */

#define LUA_META_OBJECT_taffy_LengthPercentageAuto "taffy_LengthPercentageAuto_mt"

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
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_LengthPercentageAuto* copy = taffy_LengthPercentageAuto_new_copy(*object);

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
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    taffy_LengthPercentageAuto_delete(*object);

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
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Length = taffy_LengthPercentageAuto_is_Length(*object);

    lua_pushboolean(L, is_Length);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_is_Percent(lua_State* L)
{
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Percent = taffy_LengthPercentageAuto_is_Percent(*object);

    lua_pushboolean(L, is_Percent);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_is_Auto(lua_State* L)
{
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    const int is_Auto = taffy_LengthPercentageAuto_is_Auto(*object);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentageAuto_get_value(lua_State* L)
{
    taffy_LengthPercentageAuto** object = (taffy_LengthPercentageAuto**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentageAuto);

    if( taffy_LengthPercentageAuto_is_Length (*object) ||
        taffy_LengthPercentageAuto_is_Percent(*object) )
    {
        const int value = taffy_LengthPercentageAuto_get_value(*object);

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

/* -------------------------------------------------------------------------- */
/* Dimension */

#define LUA_META_OBJECT_taffy_Dimension "taffy_Dimension_mt"

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
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    taffy_Dimension* copy = taffy_Dimension_new_copy(*object);

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
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    taffy_Dimension_delete(*object);

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
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Length = taffy_Dimension_is_Length(*object);

    lua_pushboolean(L, is_Length);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_is_Percent(lua_State* L)
{
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Percent = taffy_Dimension_is_Percent(*object);

    lua_pushboolean(L, is_Percent);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_is_Auto(lua_State* L)
{
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    const int is_Auto = taffy_Dimension_is_Auto(*object);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_Dimension_get_value(lua_State* L)
{
    taffy_Dimension** object = (taffy_Dimension**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Dimension);

    if( taffy_Dimension_is_Length (*object) ||
        taffy_Dimension_is_Percent(*object) )
    {
        const int value = taffy_Dimension_get_value(*object);

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

/* -------------------------------------------------------------------------- */
/* FlexDirection */

static void lua_push_table_taffy_FlexDirection(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_FlexDirection_Row          ); lua_setfield(L, -2, "Row");
        lua_pushnumber(L, taffy_FlexDirection_Column       ); lua_setfield(L, -2, "Column");
        lua_pushnumber(L, taffy_FlexDirection_RowReverse   ); lua_setfield(L, -2, "RowReverse");
        lua_pushnumber(L, taffy_FlexDirection_ColumnReverse); lua_setfield(L, -2, "ColumnReverse");

        lua_pushnumber(L, taffy_FlexDirection_default()    ); lua_setfield(L, -2, "Default");
    }
}

/* -------------------------------------------------------------------------- */
/* FlexWrap */

static void lua_push_table_taffy_FlexWrap(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_FlexWrap_NoWrap     ); lua_setfield(L, -2, "NoWrap");
        lua_pushnumber(L, taffy_FlexWrap_Wrap       ); lua_setfield(L, -2, "Wrap");
        lua_pushnumber(L, taffy_FlexWrap_WrapReverse); lua_setfield(L, -2, "WrapReverse");

        lua_pushnumber(L, taffy_FlexWrap_default()  ); lua_setfield(L, -2, "Default");
    }
}

/* -------------------------------------------------------------------------- */
/* GridAutoFlow */

static void lua_push_table_taffy_GridAutoFlow(lua_State* L)
{
    lua_newtable(L);
    {
        lua_pushnumber(L, taffy_GridAutoFlow_Row        ); lua_setfield(L, -2, "Row");
        lua_pushnumber(L, taffy_GridAutoFlow_Column     ); lua_setfield(L, -2, "Column");
        lua_pushnumber(L, taffy_GridAutoFlow_RowDense   ); lua_setfield(L, -2, "RowDense");
        lua_pushnumber(L, taffy_GridAutoFlow_ColumnDense); lua_setfield(L, -2, "ColumnDense");

        lua_pushnumber(L, taffy_GridAutoFlow_default()  ); lua_setfield(L, -2, "Default");
    }
}

/* -------------------------------------------------------------------------- */
/* GridPlacement */

#define LUA_META_OBJECT_taffy_GridPlacement "taffy_GridPlacement_mt"

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
    const lua_Number value = luaL_checknumber(L, 1);

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
    const lua_Number value = luaL_checknumber(L, 1);

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
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_GridPlacement* copy = taffy_GridPlacement_new_copy(*object);

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
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    taffy_GridPlacement_delete(*object);

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
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Auto = taffy_GridPlacement_is_Auto(*object);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_is_Line(lua_State* L)
{
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Line = taffy_GridPlacement_is_Line(*object);

    lua_pushboolean(L, is_Line);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_is_Span(lua_State* L)
{
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    const int is_Span = taffy_GridPlacement_is_Span(*object);

    lua_pushboolean(L, is_Span);

    return 1; /* number of results */
}

static int lua_taffy_GridPlacement_get_line(lua_State* L)
{
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    if( taffy_GridPlacement_is_Line(*object) )
    {
        const int16_t line = taffy_GridPlacement_get_line(*object);

        lua_pushnumber(L, line);

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
    taffy_GridPlacement** object = (taffy_GridPlacement**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridPlacement);

    if( taffy_GridPlacement_is_Span(*object) )
    {
        const uint16_t span = taffy_GridPlacement_get_span(*object);

        lua_pushnumber(L, span);

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
    const lua_Number value = luaL_checknumber(L, 1);

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
    const lua_Number value = luaL_checknumber(L, 1);

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

/* -------------------------------------------------------------------------- */
/* GridTrackRepetition */

#define LUA_META_OBJECT_taffy_GridTrackRepetition "taffy_GridTrackRepetition_mt"

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
    const lua_Number value = luaL_checknumber(L, 1);

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
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    taffy_GridTrackRepetition* copy = taffy_GridTrackRepetition_new_copy(*object);

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
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    taffy_GridTrackRepetition_delete(*object);

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
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_AutoFill = taffy_GridTrackRepetition_is_AutoFill(*object);

    lua_pushboolean(L, is_AutoFill);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_is_AutoFit(lua_State* L)
{
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_AutoFit = taffy_GridTrackRepetition_is_AutoFit(*object);

    lua_pushboolean(L, is_AutoFit);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_is_Count(lua_State* L)
{
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    const int is_Count = taffy_GridTrackRepetition_is_Count(*object);

    lua_pushboolean(L, is_Count);

    return 1; /* number of results */
}

static int lua_taffy_GridTrackRepetition_get_value(lua_State* L)
{
    taffy_GridTrackRepetition** object = (taffy_GridTrackRepetition**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_GridTrackRepetition);

    if( taffy_GridTrackRepetition_is_Count(*object) )
    {
        const uint16_t value = taffy_GridTrackRepetition_get_value(*object);

        lua_pushnumber(L, value);

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
    switch( lua_type(L, 1) )
    {
    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 1);

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

/* -------------------------------------------------------------------------- */
/* MaxTrackSizingFunction */

#define LUA_META_OBJECT_taffy_MaxTrackSizingFunction "taffy_MaxTrackSizingFunction_mt"

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
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    taffy_MaxTrackSizingFunction* copy = taffy_MaxTrackSizingFunction_new_copy(*object);

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
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    taffy_MaxTrackSizingFunction_delete(*object);

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
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Fixed = taffy_MaxTrackSizingFunction_is_Fixed(*object);

    lua_pushboolean(L, is_Fixed);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_MinContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_MinContent = taffy_MaxTrackSizingFunction_is_MinContent(*object);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_MaxContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_MaxContent = taffy_MaxTrackSizingFunction_is_MaxContent(*object);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_FitContent(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_FitContent = taffy_MaxTrackSizingFunction_is_FitContent(*object);

    lua_pushboolean(L, is_FitContent);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_Auto(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Auto = taffy_MaxTrackSizingFunction_is_Auto(*object);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_is_Fraction(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    const int is_Fraction = taffy_MaxTrackSizingFunction_is_Fraction(*object);

    lua_pushboolean(L, is_Fraction);

    return 1; /* number of results */
}

static int lua_taffy_MaxTrackSizingFunction_get_length_percentage(lua_State* L)
{
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    if( taffy_MaxTrackSizingFunction_is_Fixed     (*object) ||
        taffy_MaxTrackSizingFunction_is_FitContent(*object) )
    {
        taffy_LengthPercentage* object_ptr = taffy_MaxTrackSizingFunction_get_new_length_percentage(*object);
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
    taffy_MaxTrackSizingFunction** object = (taffy_MaxTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);

    if( taffy_MaxTrackSizingFunction_is_Fraction(*object) )
    {
        const float fraction = taffy_MaxTrackSizingFunction_get_fraction(*object);

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

/* -------------------------------------------------------------------------- */
/* MinTrackSizingFunction */

#define LUA_META_OBJECT_taffy_MinTrackSizingFunction "taffy_MinTrackSizingFunction_mt"

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
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    taffy_MinTrackSizingFunction* copy = taffy_MinTrackSizingFunction_new_copy(*object);

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
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    taffy_MinTrackSizingFunction_delete(*object);

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
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_Fixed = taffy_MinTrackSizingFunction_is_Fixed(*object);

    lua_pushboolean(L, is_Fixed);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_MinContent(lua_State* L)
{
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_MinContent = taffy_MinTrackSizingFunction_is_MinContent(*object);

    lua_pushboolean(L, is_MinContent);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_MaxContent(lua_State* L)
{
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_MaxContent = taffy_MinTrackSizingFunction_is_MaxContent(*object);

    lua_pushboolean(L, is_MaxContent);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_is_Auto(lua_State* L)
{
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    const int is_Auto = taffy_MinTrackSizingFunction_is_Auto(*object);

    lua_pushboolean(L, is_Auto);

    return 1; /* number of results */
}

static int lua_taffy_MinTrackSizingFunction_get_value(lua_State* L)
{
    taffy_MinTrackSizingFunction** object = (taffy_MinTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_MinTrackSizingFunction);

    if( taffy_MinTrackSizingFunction_is_Fixed(*object) )
    {
        taffy_LengthPercentage* object_ptr = taffy_MinTrackSizingFunction_get_new_value(*object);
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

/* -------------------------------------------------------------------------- */
/* NonRepeatedTrackSizingFunction */

#define LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction "taffy_NonRepeatedTrackSizingFunction_mt"

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
    taffy_NonRepeatedTrackSizingFunction** object = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction* copy = taffy_NonRepeatedTrackSizingFunction_new_copy(*object);

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
    taffy_NonRepeatedTrackSizingFunction** object = (taffy_NonRepeatedTrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);

    taffy_NonRepeatedTrackSizingFunction_delete(*object);

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

/* -------------------------------------------------------------------------- */
/* TrackSizingFunction */

#define LUA_META_OBJECT_taffy_TrackSizingFunction "taffy_TrackSizingFunction_mt"

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

    taffy_NonRepeatedTrackSizingFunction** funcs = NULL;
    size_t funcs_count = 0;

    {
        if( lua_type(L, 2) == LUA_TTABLE )
        {
            const size_t table_size = lua_rawlen(L, 2);
            if(table_size > 0)
            {
                /* Allocate array with pointers into 'NonRepeatedTrackSizingFunction's */
                funcs = (taffy_NonRepeatedTrackSizingFunction**)malloc(table_size * sizeof(taffy_NonRepeatedTrackSizingFunction*));
                funcs_count = table_size;

                size_t index = 0;

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

                    funcs[index] = *value_value;

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

    taffy_TrackSizingFunction* object_ptr = taffy_TrackSizingFunction_new_Repeat(*repetition, funcs, funcs_count);

    /* Dont forget to 'free()' memory, allocated by 'malloc()' */
    if(funcs != NULL)
    {
        free(funcs);
        funcs = NULL;
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
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    taffy_TrackSizingFunction* copy = taffy_TrackSizingFunction_new_copy(*object);

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
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    taffy_TrackSizingFunction_delete(*object);

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
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    const int is_Single = taffy_TrackSizingFunction_is_Single(*object);

    lua_pushboolean(L, is_Single);

    return 1; /* number of results */
}

static int lua_taffy_TrackSizingFunction_is_Repeat(lua_State* L)
{
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    const int is_Repeat = taffy_TrackSizingFunction_is_Repeat(*object);

    lua_pushboolean(L, is_Repeat);

    return 1; /* number of results */
}

static int lua_taffy_TrackSizingFunction_get_single_func(lua_State* L)
{
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Single(*object) )
    {
        const taffy_NonRepeatedTrackSizingFunction* single_func = taffy_TrackSizingFunction_get_single_func(*object);
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
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Repeat(*object) )
    {
        const taffy_GridTrackRepetition* repetition = taffy_TrackSizingFunction_get_repetition(*object);
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
    taffy_TrackSizingFunction** object = (taffy_TrackSizingFunction**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_TrackSizingFunction);

    if( taffy_TrackSizingFunction_is_Repeat(*object) )
    {
        taffy_GridTrackVec_of_NonRepeatedTrackSizingFunction repeat_funcs = taffy_TrackSizingFunction_get_repeat_funcs(*object);

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

    TODO: is 'metatable protection' must be used for enums?
*/
static const char LUA_METATABLE_PROTECTION_MESSAGE[] = "protected metatable";

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
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_float);
            {
                /* metatable.__index = metatable */
                lua_pushvalue(L, -1);
                lua_setfield(L, -2, "__index");

                lua_pushcfunction(L, lua_taffy_Option_float_delete);
                lua_setfield(L, -2, "__gc");

                /* metatable.__metatable = "message" <-- metatable protection */
                lua_pushstring(L, LUA_METATABLE_PROTECTION_MESSAGE);
                lua_setfield(L, -2, "__metatable");

                lua_pushcfunction(L, lua_taffy_Option_float_new);
                lua_setfield(L, -2, "new");

                lua_pushcfunction(L, lua_taffy_Option_float_copy);
                lua_setfield(L, -2, "copy");

                lua_pushcfunction(L, lua_taffy_Option_float_is_some);
                lua_setfield(L, -2, "is_some");

                lua_pushcfunction(L, lua_taffy_Option_float_get_value);
                lua_setfield(L, -2, "get_value");

                lua_pushcfunction(L, lua_taffy_Option_float_set_value);
                lua_setfield(L, -2, "set_value");
            }
            lua_setfield(L, -2, "Option_float");
        }

        /* Register Point<float> */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);
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

                lua_pushcfunction(L, lua_taffy_Point_of_float_new);
                lua_setfield(L, -2, "new");

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

                lua_pushcfunction(L, lua_taffy_Point_of_float_ZERO);
                lua_setfield(L, -2, "ZERO");
            }
            lua_setfield(L, -2, "Point_of_float");
        }

        /* Register Size<float> */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);
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

                lua_pushcfunction(L, lua_taffy_Size_of_float_new);
                lua_setfield(L, -2, "new");

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

                lua_pushcfunction(L, lua_taffy_Size_of_float_ZERO);
                lua_setfield(L, -2, "ZERO");
            }
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
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);
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

                lua_pushcfunction(L, lua_taffy_AvailableSpace_Definite);
                lua_setfield(L, -2, "Definite");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_MinContent);
                lua_setfield(L, -2, "MinContent");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_MaxContent);
                lua_setfield(L, -2, "MaxContent");

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
            lua_setfield(L, -2, "AvailableSpace");
        }

        /* Register LengthPercentage */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);
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

                lua_pushcfunction(L, lua_taffy_LengthPercentage_Length);
                lua_setfield(L, -2, "Length");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_Percent);
                lua_setfield(L, -2, "Percent");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_copy);
                lua_setfield(L, -2, "copy");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_is_Length);
                lua_setfield(L, -2, "is_Length");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_is_Percent);
                lua_setfield(L, -2, "is_Percent");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_get_value);
                lua_setfield(L, -2, "get_value");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_ZERO);
                lua_setfield(L, -2, "ZERO");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_from_length);
                lua_setfield(L, -2, "from_length");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_from_percent);
                lua_setfield(L, -2, "from_percent");
            }
            lua_setfield(L, -2, "LengthPercentage");
        }

        /* Register LengthPercentageAuto */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_LengthPercentageAuto);
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

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Length);
                lua_setfield(L, -2, "Length");

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Percent);
                lua_setfield(L, -2, "Percent");

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_Auto);
                lua_setfield(L, -2, "Auto");

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

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_ZERO);
                lua_setfield(L, -2, "ZERO");

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from_length);
                lua_setfield(L, -2, "from_length");

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from_percent);
                lua_setfield(L, -2, "from_percent");

                lua_pushcfunction(L, lua_taffy_LengthPercentageAuto_from);
                lua_setfield(L, -2, "from");
            }
            lua_setfield(L, -2, "LengthPercentageAuto");
        }

        /* Register Dimension */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Dimension);
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

                lua_pushcfunction(L, lua_taffy_Dimension_Length);
                lua_setfield(L, -2, "Length");

                lua_pushcfunction(L, lua_taffy_Dimension_Percent);
                lua_setfield(L, -2, "Percent");

                lua_pushcfunction(L, lua_taffy_Dimension_Auto);
                lua_setfield(L, -2, "Auto");

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

                lua_pushcfunction(L, lua_taffy_Dimension_ZERO);
                lua_setfield(L, -2, "ZERO");

                lua_pushcfunction(L, lua_taffy_Dimension_from_length);
                lua_setfield(L, -2, "from_length");

                lua_pushcfunction(L, lua_taffy_Dimension_from_percent);
                lua_setfield(L, -2, "from_percent");

                lua_pushcfunction(L, lua_taffy_Dimension_from);
                lua_setfield(L, -2, "from");
            }
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
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridPlacement);
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

                lua_pushcfunction(L, lua_taffy_GridPlacement_new);
                lua_setfield(L, -2, "new");

                lua_pushcfunction(L, lua_taffy_GridPlacement_Auto);
                lua_setfield(L, -2, "Auto");

                lua_pushcfunction(L, lua_taffy_GridPlacement_Line);
                lua_setfield(L, -2, "Line");

                lua_pushcfunction(L, lua_taffy_GridPlacement_Span);
                lua_setfield(L, -2, "Span");

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

                lua_pushcfunction(L, lua_taffy_GridPlacement_AUTO);
                lua_setfield(L, -2, "AUTO");

                lua_pushcfunction(L, lua_taffy_GridPlacement_from_line_index);
                lua_setfield(L, -2, "from_line_index");

                lua_pushcfunction(L, lua_taffy_GridPlacement_from_span);
                lua_setfield(L, -2, "from_span");
            }
            lua_setfield(L, -2, "GridPlacement");
        }

        /* Register GridTrackRepetition */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_GridTrackRepetition);
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

                lua_pushcfunction(L, lua_taffy_GridTrackRepetition_AutoFill);
                lua_setfield(L, -2, "AutoFill");

                lua_pushcfunction(L, lua_taffy_GridTrackRepetition_AutoFit);
                lua_setfield(L, -2, "AutoFit");

                lua_pushcfunction(L, lua_taffy_GridTrackRepetition_Count);
                lua_setfield(L, -2, "Count");

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

                lua_pushcfunction(L, lua_taffy_GridTrackRepetition_try_from);
                lua_setfield(L, -2, "try_from");
            }
            lua_setfield(L, -2, "GridTrackRepetition");
        }

        /* Register MaxTrackSizingFunction */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_MaxTrackSizingFunction);
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
            lua_setfield(L, -2, "MaxTrackSizingFunction");
        }

        /* Register MinTrackSizingFunction */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_MinTrackSizingFunction);
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

                lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_Fixed);
                lua_setfield(L, -2, "Fixed");

                lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MinContent);
                lua_setfield(L, -2, "MinContent");

                lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_MaxContent);
                lua_setfield(L, -2, "MaxContent");

                lua_pushcfunction(L, lua_taffy_MinTrackSizingFunction_Auto);
                lua_setfield(L, -2, "Auto");

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
            lua_setfield(L, -2, "MinTrackSizingFunction");
        }

        /* Register NonRepeatedTrackSizingFunction */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_NonRepeatedTrackSizingFunction);
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

                lua_pushcfunction(L, lua_taffy_NonRepeatedTrackSizingFunction_new);
                lua_setfield(L, -2, "new");

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
            lua_setfield(L, -2, "NonRepeatedTrackSizingFunction");
        }

        /* Register TrackSizingFunction */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_TrackSizingFunction);
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

                lua_pushcfunction(L, lua_taffy_TrackSizingFunction_Single);
                lua_setfield(L, -2, "Single");

                lua_pushcfunction(L, lua_taffy_TrackSizingFunction_Repeat);
                lua_setfield(L, -2, "Repeat");

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
            lua_setfield(L, -2, "TrackSizingFunction");
        }
    }

    return 1; /* number of results */
}