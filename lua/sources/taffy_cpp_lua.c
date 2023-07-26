#include <taffy_cpp_lua.h>

/* taffy_cpp c library */
#include <taffy_cpp_c.h>

/* lua c api */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Option_float : taffy_Option_float_new_some() failed");
        }
    } break;

    default:
    {
        return luaL_error(L, "Failed to create taffy_Option_float : incorrect argument type");
    } break;
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Option_float : taffy_Option_float_new_copy() failed");
    }

    return 1; /* number of results */
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

    const float value = taffy_Option_float_get_value(*object);

    lua_pushnumber(L, value);

    return 1; /* number of results */
}

static int lua_taffy_Option_float_set_value(lua_State* L)
{
    taffy_Option_float** object = (taffy_Option_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Option_float);

    switch( lua_type(L, 2) ) {
    
    case LUA_TNIL:
    {
        lua_pop(L, 1);

        taffy_Option_float_set_value(*object, NULL);
    } break;

    case LUA_TNUMBER:
    {
        const lua_Number value = lua_tonumber(L, 2);
        const float f_value = (float)(value);

        taffy_Option_float_set_value(*object, &f_value);
    } break;

    default:
    {
        return luaL_error(L, "Failed to set_value taffy_Option_float : incorrect argument type");
    } break;
    }

    return 0; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Point_of_float : taffy_Point_of_float_new_copy() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_Point_of_float_delete(lua_State* L)
{
    taffy_Point_of_float** object = (taffy_Point_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Point_of_float);
    
    taffy_Point_of_float_delete(*object);
    
    return 0; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new_ZERO() failed");
    }

    return 1; /* number of results */
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

        return 1;
    }
    else if(strcmp(key, "y") == 0)
    {
        const float y = taffy_Point_of_float_get_y(*self);

        lua_pushnumber(L, y);

        return 1;
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Point_of_float);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1;
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
    }
    else if( strcmp(key, "y") == 0)
    {
        taffy_Point_of_float_set_y(*self, value);
    }
    else
    {
        return luaL_error(L, "taffy_Point_of_float 'newindex' failed"); /* TODO: better message*/
    }

    return 0; /* number of results */
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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_default() failed");
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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new() failed");
        }
    } break;

    default:
    {
        return luaL_error(L, "Failed to create taffy_Size_of_float : wrong arguments count");
    } break;
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_Size_of_float : taffy_Size_of_float_new_copy() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_Size_of_float_delete(lua_State* L)
{
    taffy_Size_of_float** object = (taffy_Size_of_float**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_Size_of_float);

    taffy_Size_of_float_delete(*object);

    return 0; /* number of results */
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

/* TODO: mutators */

static int lua_taffy_Size_of_float_ZERO(lua_State* L)
{
    taffy_Size_of_float* object_ptr = taffy_Size_of_float_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_Size_of_float** user_data = (taffy_Size_of_float**)lua_newuserdata(L, sizeof(taffy_Size_of_float*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_Size_of_float : taffy_Size_of_float_new_ZERO() failed");
    }

    return 1; /* number of results */
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

        return 1;
    }
    else if(strcmp(key, "height") == 0)
    {
        const float height = taffy_Size_of_float_get_height(*self);

        lua_pushnumber(L, height);

        return 1;
    }

    /* default behavior */
    luaL_getmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1;
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
    }
    else if( strcmp(key, "height") == 0)
    {
        taffy_Size_of_float_set_height(*self, value);
    }
    else
    {
        return luaL_error(L, "taffy_Size_of_float 'newindex' failed"); /* TODO: better message*/
    }

    return 0; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_Definite() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_MinContent(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MinContent();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MinContent() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_MaxContent(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_MaxContent();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_MaxContent() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_AvailableSpace : taffy_AvailableSpace_new_copy() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_AvailableSpace_delete(lua_State* L)
{
    taffy_AvailableSpace** object = (taffy_AvailableSpace**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_AvailableSpace);

    taffy_AvailableSpace_delete(*object);

    return 0; /* number of results */
}

static int lua_taffy_AvailableSpace_ZERO(lua_State* L)
{
    taffy_AvailableSpace* object_ptr = taffy_AvailableSpace_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_AvailableSpace** user_data = (taffy_AvailableSpace**)lua_newuserdata(L, sizeof(taffy_AvailableSpace*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_AvailableSpace);
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_ZERO() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_from_length() failed");
    }

    return 1; /* number of results */
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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_AvailableSpace : taffy_AvailableSpace_new_from_option() failed");
        }
    } break;

    default:
    {
        return luaL_error(L, "Failed to call 'from' of taffy_AvailableSpace : incorrect argument type");
    } break;
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_Length() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_Percent() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to copy taffy_LengthPercentage : taffy_LengthPercentage_new_copy() failed");
    }

    return 1; /* number of results */
}

static int lua_taffy_LengthPercentage_delete(lua_State* L)
{
    taffy_LengthPercentage** object = (taffy_LengthPercentage**)luaL_checkudata(L, 1, LUA_META_OBJECT_taffy_LengthPercentage);

    taffy_LengthPercentage_delete(*object);

    return 0; /* number of results */
}

static int lua_taffy_LengthPercentage_ZERO(lua_State* L)
{
    taffy_LengthPercentage* object_ptr = taffy_LengthPercentage_new_ZERO();
    if(object_ptr != NULL)
    {
        taffy_LengthPercentage** user_data = (taffy_LengthPercentage**)lua_newuserdata(L, sizeof(taffy_LengthPercentage*));
        *user_data = object_ptr;

        luaL_setmetatable(L, LUA_META_OBJECT_taffy_LengthPercentage);
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_ZERO() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_from_length() failed");
    }

    return 1; /* number of results */
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
    }
    else
    {
        return luaL_error(L, "Failed to create taffy_LengthPercentage : taffy_LengthPercentage_new_from_percent() failed");
    }

    return 1; /* number of results */
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
        /* ------------------------------------------------------------------ */
        /* Enumerations */

        lua_push_table_taffy_AlignContent(L);
        lua_setfield(L, -2, "AlignContent");

        lua_push_table_taffy_JustifyContent(L);
        lua_setfield(L, -2, "JustifyContent");

        lua_push_table_taffy_AlignItems(L);
        lua_setfield(L, -2, "AlignItems");

        lua_push_table_taffy_AlignSelf(L);
        lua_setfield(L, -2, "AlignSelf");

        /* ------------------------------------------------------------------ */
        /* Classes */

        /* Register Option<float> */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Option_float);
            {
                /* metatable.__index = metatable */
                lua_pushvalue(L, -1);
                lua_setfield(L, -2, "__index");

                lua_pushcfunction(L, lua_taffy_Option_float_delete);
                lua_setfield(L, -2, "__gc");

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

                /* TODO: mutators */

                lua_pushcfunction(L, lua_taffy_Size_of_float_ZERO);
                lua_setfield(L, -2, "ZERO");
            }
            lua_setfield(L, -2, "Size_of_float");
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

                lua_pushcfunction(L, lua_taffy_AvailableSpace_Definite);
                lua_setfield(L, -2, "Definite");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_MinContent);
                lua_setfield(L, -2, "MinContent");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_MaxContent);
                lua_setfield(L, -2, "MaxContent");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_copy);
                lua_setfield(L, -2, "copy");

                lua_pushcfunction(L, lua_taffy_AvailableSpace_ZERO);
                lua_setfield(L, -2, "ZERO");

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
            
                lua_pushcfunction(L, lua_taffy_LengthPercentage_Length);
                lua_setfield(L, -2, "Length");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_Percent);
                lua_setfield(L, -2, "Percent");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_copy);
                lua_setfield(L, -2, "copy");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_ZERO);
                lua_setfield(L, -2, "ZERO");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_from_length);
                lua_setfield(L, -2, "from_length");

                lua_pushcfunction(L, lua_taffy_LengthPercentage_from_percent);
                lua_setfield(L, -2, "from_percent");
            }
            lua_setfield(L, -2, "LengthPercentage");
        }
    }

    return 1;
}