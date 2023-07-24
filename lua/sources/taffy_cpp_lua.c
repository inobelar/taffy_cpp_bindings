#include <taffy_cpp_lua.h>

/* taffy_cpp c library */
#include <taffy_cpp_c.h>

/* lua c api */
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new_default() failed");
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
        }
        else
        {
            return luaL_error(L, "Failed to create taffy_Point_of_float : taffy_Point_of_float_new() failed");
        }
    } break;

    default:
    {
        return luaL_error(L, "Failed to create taffy_Point_of_float : wrong arguments count");
    } break;
    }

    return 1; /* number of results */
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

/* TODO: mutators */

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
                lua_pushcfunction(L, lua_taffy_Option_float_delete);
                lua_setfield(L, -2, "__gc");
            }

            lua_pushvalue(L, -1);
            lua_setfield(L, -2, "__index");
            {
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
                lua_pushcfunction(L, lua_taffy_Point_of_float_delete);
                lua_setfield(L, -2, "__gc");
            }

            lua_pushvalue(L, -1);
            lua_setfield(L, -2, "__index");
            {
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

                /* TODO: mutators */

                lua_pushcfunction(L, lua_taffy_Point_of_float_ZERO);
                lua_setfield(L, -2, "ZERO");
            }
            lua_setfield(L, -2, "Point_of_float");
        }

        /* Register Size<float> */
        {
            luaL_newmetatable(L, LUA_META_OBJECT_taffy_Size_of_float);
            {
                lua_pushcfunction(L, lua_taffy_Size_of_float_delete);
                lua_setfield(L, -2, "__gc");
            }

            lua_pushvalue(L, -1);
            lua_setfield(L, -2, "__index");
            {
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
                lua_pushcfunction(L, lua_taffy_AvailableSpace_delete);
                lua_setfield(L, -2, "__gc");
            }

            lua_pushvalue(L, -1);
            lua_setfield(L, -2, "__index");
            {
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
                lua_pushcfunction(L, lua_taffy_LengthPercentage_delete);
                lua_setfield(L, -2, "__gc");
            }

            lua_pushvalue(L, -1);
            lua_setfield(L, -2, "__index");
            {
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