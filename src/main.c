// SDL 3.2.22
// cimgui 1.92.2 or master
// lua 5.4.8

// main.c
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <cimgui.h>
#include <cimgui_impl.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define igGetIO igGetIO_Nil

// Lua binding for igBegin
static int lua_igBegin(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    bool p_open = lua_toboolean(L, 2);
    int flags = luaL_optinteger(L, 3, 0);
    bool *p_open_ptr = p_open ? &p_open : NULL;
    bool result = igBegin(name, p_open_ptr, flags);
    if (p_open_ptr) lua_pushboolean(L, *p_open_ptr);
    else lua_pushnil(L);
    lua_pushboolean(L, result);
    return 2;
}

// Lua binding for igEnd
static int lua_igEnd(lua_State *L) {
    igEnd();
    return 0;
}

// Lua binding for igText
static int lua_igText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    igText("%s", text);
    return 0;
}

// Lua binding for igCheckbox
static int lua_igCheckbox(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    bool value = lua_toboolean(L, 2);
    bool result = igCheckbox(label, &value);
    lua_pushboolean(L, value);
    lua_pushboolean(L, result);
    return 2;
}

// Lua binding for igSliderFloat
static int lua_igSliderFloat(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    float value = (float)luaL_checknumber(L, 2);
    float min = (float)luaL_checknumber(L, 3);
    float max = (float)luaL_checknumber(L, 4);
    const char *format = luaL_optstring(L, 5, "%.3f");
    int flags = luaL_optinteger(L, 6, 0);
    bool result = igSliderFloat(label, &value, min, max, format, flags);
    lua_pushnumber(L, value);
    lua_pushboolean(L, result);
    return 2;
}

// Lua binding for igColorEdit4
static int lua_igColorEdit4(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    float col[4];
    col[0] = (float)luaL_checknumber(L, 2); // r
    col[1] = (float)luaL_checknumber(L, 3); // g
    col[2] = (float)luaL_checknumber(L, 4); // b
    col[3] = (float)luaL_optnumber(L, 5, 1.0f); // a, default 1.0
    int flags = luaL_optinteger(L, 6, 0); // Optional flags
    bool result = igColorEdit4(label, col, flags);
    lua_newtable(L);
    lua_pushnumber(L, col[0]); lua_setfield(L, -2, "r");
    lua_pushnumber(L, col[1]); lua_setfield(L, -2, "g");
    lua_pushnumber(L, col[2]); lua_setfield(L, -2, "b");
    lua_pushnumber(L, col[3]); lua_setfield(L, -2, "a");
    lua_pushboolean(L, result);
    return 2;
}

// Lua binding for igButton
static int lua_igButton(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    ImVec2 size = {0, 0};
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "x"); size.x = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "y"); size.y = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
    }
    bool result = igButton(label, size);
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igSameLine
static int lua_igSameLine(lua_State *L) {
    float offset = (float)luaL_optnumber(L, 1, 0.0f);
    float spacing = (float)luaL_optnumber(L, 2, -1.0f);
    igSameLine(offset, spacing);
    return 0;
}

// Lua binding for igGetFramerate
static int lua_igGetFramerate(lua_State *L) {
    ImGuiIO *io = igGetIO();
    lua_pushnumber(L, io->Framerate);
    return 1;
}

// Lua binding for igInputText
static int lua_igInputText(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    const char *initial_text = luaL_checkstring(L, 2);
    size_t buf_size = (size_t)luaL_optinteger(L, 3, 256); // Default buffer size
    int flags = luaL_optinteger(L, 4, 0);

    // Allocate buffer and copy initial text
    char *buf = (char *)malloc(buf_size);
    if (!buf) {
        luaL_error(L, "Failed to allocate buffer for InputText");
        return 0;
    }
    strncpy(buf, initial_text, buf_size - 1);
    buf[buf_size - 1] = '\0'; // Ensure null-termination

    // Call igInputText
    bool changed = igInputText(label, buf, buf_size, flags, NULL, NULL);

    // Push results to Lua
    lua_pushstring(L, buf); // Updated text
    lua_pushboolean(L, changed); // Whether text was changed
    free(buf); // Clean up
    return 2;
}

// Lua binding for igCombo_Str
static int lua_igCombo_Str(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    int current_item = (int)luaL_checkinteger(L, 2) - 1; // Lua 1-based to C 0-based
    luaL_checktype(L, 3, LUA_TTABLE); // Items must be a table
    int popup_max_height_in_items = luaL_optinteger(L, 4, -1); // Optional height, default -1

    // Build items string (concatenate with \0)
    lua_len(L, 3); // Get table length
    size_t item_count = lua_tointeger(L, -1);
    lua_pop(L, 1);

    size_t total_len = 0;
    for (size_t i = 1; i <= item_count; i++) {
        lua_geti(L, 3, i);
        const char *item = luaL_checkstring(L, -1);
        total_len += strlen(item) + 1; // +1 for \0
        lua_pop(L, 1);
    }

    char *items = (char *)malloc(total_len);
    if (!items) {
        luaL_error(L, "Failed to allocate buffer for Combo items");
        return 0;
    }

    size_t offset = 0;
    for (size_t i = 1; i <= item_count; i++) {
        lua_geti(L, 3, i);
        const char *item = luaL_checkstring(L, -1);
        size_t len = strlen(item);
        strcpy(items + offset, item);
        offset += len + 1; // Move past string and null terminator
        lua_pop(L, 1);
    }
    items[total_len - 1] = '\0'; // Ensure final null terminator

    // Call igCombo_Str
    bool changed = igCombo_Str(label, &current_item, items, popup_max_height_in_items);

    // Clean up
    free(items);

    // Push results to Lua
    lua_pushinteger(L, current_item + 1); // C 0-based to Lua 1-based
    lua_pushboolean(L, changed);
    return 2;
}

// Lua binding for igCombo_Str_arr
static int lua_igCombo_Str_arr(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    int current_item = (int)luaL_checkinteger(L, 2) - 1; // Lua 1-based to C 0-based
    luaL_checktype(L, 3, LUA_TTABLE); // Items must be a table
    int popup_max_height_in_items = luaL_optinteger(L, 4, -1); // Optional height, default -1

    // Get table length
    lua_len(L, 3);
    size_t item_count = lua_tointeger(L, -1);
    lua_pop(L, 1);

    // Allocate array of string pointers
    const char **items = (const char **)malloc(item_count * sizeof(const char *));
    if (!items) {
        luaL_error(L, "Failed to allocate buffer for Combo items");
        return 0;
    }

    // Populate items array
    for (size_t i = 1; i <= item_count; i++) {
        lua_geti(L, 3, i);
        items[i - 1] = luaL_checkstring(L, -1); // Store pointer directly
        lua_pop(L, 1);
    }

    // Call igCombo_Str_arr
    bool changed = igCombo_Str_arr(label, &current_item, items, item_count, popup_max_height_in_items);

    // Clean up
    free(items);

    // Push results to Lua
    lua_pushinteger(L, current_item + 1); // C 0-based to Lua 1-based
    lua_pushboolean(L, changed);
    return 2;
}

// Lua binding for igProgressBar
static int lua_igProgressBar(lua_State *L) {
    float fraction = (float)luaL_checknumber(L, 1);
    ImVec2 size = {0, 0}; // Default size
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "x"); size.x = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "y"); size.y = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
    }
    const char *overlay = luaL_optstring(L, 3, NULL); // Optional overlay

    // Call igProgressBar with size passed by value
    igProgressBar(fraction, size, overlay);

    return 0; // No return values
}

// Lua binding for igRadioButton_Bool
static int lua_igRadioButton(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    bool active = lua_toboolean(L, 2);

    // Call igRadioButton_Bool
    bool clicked = igRadioButton_Bool(label, active);

    // Push result to Lua
    lua_pushboolean(L, clicked);
    return 1;
}

// Lua binding for igSeparator
static int lua_igSeparator(lua_State *L) {
    igSeparator();
    return 0; // No return values
}

// Lua binding for igSpacing
static int lua_igSpacing(lua_State *L) {
    igSpacing();
    return 0; // No return values
}

// Lua binding for igBeginTabBar
static int lua_igBeginTabBar(lua_State *L) {
    const char *str_id = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0); // Default flags to 0

    // Call igBeginTabBar
    bool result = igBeginTabBar(str_id, flags);

    // Push result to Lua
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igEndTabBar
static int lua_igEndTabBar(lua_State *L) {
    igEndTabBar();
    return 0; // No return values
}

// Lua binding for igBeginTabItem
static int lua_igBeginTabItem(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    bool p_open = lua_isboolean(L, 2) ? lua_toboolean(L, 2) : true; // Default to true if not provided
    int flags = luaL_optinteger(L, 3, 0); // Default flags to 0
    bool *p_open_ptr = lua_isboolean(L, 2) ? &p_open : NULL;

    // Call igBeginTabItem
    bool result = igBeginTabItem(label, p_open_ptr, flags);

    // Push results to Lua
    if (p_open_ptr) lua_pushboolean(L, *p_open_ptr); // Updated p_open value
    else lua_pushnil(L);
    lua_pushboolean(L, result); // Whether tab is active
    return 2;
}

// Lua binding for igEndTabItem
static int lua_igEndTabItem(lua_State *L) {
    igEndTabItem();
    return 0; // No return values
}

// Lua binding for igBeginTooltip
static int lua_igBeginTooltip(lua_State *L) {
    igBeginTooltip();
    return 0; // No return values
}

// Lua binding for igEndTooltip
static int lua_igEndTooltip(lua_State *L) {
    igEndTooltip();
    return 0; // No return values
}

// Lua binding for igSetTooltip
static int lua_igSetTooltip(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    igSetTooltip("%s", text);
    return 0; // No return values
}

// Lua binding for igIsItemHovered
static int lua_igIsItemHovered(lua_State *L) {
    int flags = luaL_optinteger(L, 1, 0); // Default flags to 0
    bool result = igIsItemHovered(flags);
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igBeginColumns
static int lua_igBeginColumns(lua_State *L) {
    const char *str_id = luaL_checkstring(L, 1);
    int count = (int)luaL_checkinteger(L, 2);
    int flags = luaL_optinteger(L, 3, 0); // Default flags to 0

    igBeginColumns(str_id, count, flags);
    return 0; // No return values
}

// Lua binding for igEndColumns
static int lua_igEndColumns(lua_State *L) {
    igEndColumns();
    return 0; // No return values
}

// Lua binding for igNextColumn
static int lua_igNextColumn(lua_State *L) {
    igNextColumn();
    return 0; // No return values
}

// Lua binding for igSetColumnWidth
static int lua_igSetColumnWidth(lua_State *L) {
    int column_index = (int)luaL_checkinteger(L, 1);
    float width = (float)luaL_checknumber(L, 2);

    igSetColumnWidth(column_index, width);
    return 0; // No return values
}

// Lua binding for igBeginChild_Str
static int lua_igBeginChild(lua_State *L) {
    const char *str_id = luaL_checkstring(L, 1);
    ImVec2 size = {0, 0}; // Default size
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "x"); size.x = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "y"); size.y = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
    }
    bool border = lua_toboolean(L, 3); // Default to false if not provided
    int flags = luaL_optinteger(L, 4, 0); // Default flags to 0

    // Call igBeginChild_Str
    bool result = igBeginChild_Str(str_id, size, border, flags);

    // Push result to Lua
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igEndChild
static int lua_igEndChild(lua_State *L) {
    igEndChild();
    return 0; // No return values
}

// Lua binding for igIsWindowHovered
static int lua_igIsWindowHovered(lua_State *L) {
    int flags = luaL_optinteger(L, 1, 0); // Default flags to 0
    bool result = igIsWindowHovered(flags);
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igTreeNode_StrStr
static int lua_igTreeNode(lua_State *L) {
    const char *str_id = luaL_checkstring(L, 1);
    const char *label = luaL_checkstring(L, 2);

    // Call igTreeNode_StrStr
    bool result = igTreeNode_StrStr(str_id, "%s", label);

    // Push result to Lua
    lua_pushboolean(L, result);
    return 1;
}

// Lua binding for igTreePop
static int lua_igTreePop(lua_State *L) {
    igTreePop();
    return 0; // No return values
}

// Lua binding for igInputTextMultiline
static int lua_igInputTextMultiline(lua_State *L) {
    const char *label = luaL_checkstring(L, 1);
    const char *text = luaL_checkstring(L, 2);
    size_t buf_size = luaL_optinteger(L, 3, 256); // Default buffer size
    ImVec2 size = {0, 0}; // Default size (auto-resize)
    if (lua_istable(L, 4)) {
        lua_getfield(L, 4, "x"); size.x = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 4, "y"); size.y = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
    }
    int flags = luaL_optinteger(L, 5, 0); // Default flags to 0

    // Allocate buffer
    char *buf = (char *)malloc(buf_size);
    if (!buf) {
        luaL_error(L, "Failed to allocate buffer for InputTextMultiline");
        return 0;
    }
    strncpy(buf, text, buf_size - 1);
    buf[buf_size - 1] = '\0'; // Ensure null-termination

    // Call igInputTextMultiline
    bool changed = igInputTextMultiline(label, buf, buf_size, size, flags, NULL, NULL);

    // Push results to Lua
    lua_pushstring(L, buf); // Updated text
    lua_pushboolean(L, changed); // Whether text was edited
    free(buf); // Clean up
    return 2;
}

// Lua binding for igGetWindowHeight
static int lua_igGetWindowHeight(lua_State *L) {
    float height = igGetWindowHeight();
    lua_pushnumber(L, height);
    return 1;
}

// Lua binding for igGetWindowWidth
static int lua_igGetWindowWidth(lua_State *L) {
    float width = igGetWindowWidth();
    lua_pushnumber(L, width);
    return 1;
}

// Lua binding for igIsItemActive
static int lua_igIsItemActive(lua_State *L) {
    bool active = igIsItemActive();
    lua_pushboolean(L, active);
    return 1;
}

// Lua binding for igIsItemClicked
static int lua_igIsItemClicked(lua_State *L) {
    int mouse_button = luaL_optinteger(L, 1, 0); // Default to left mouse button (0)
    bool clicked = igIsItemClicked(mouse_button);
    lua_pushboolean(L, clicked);
    return 1;
}

// Lua binding for igGetStyleColorVec4
static int lua_igGetStyleColorVec4(lua_State *L) {
    int idx = (int)luaL_checkinteger(L, 1);
    const ImVec4 *color_ptr = igGetStyleColorVec4(idx);
    
    // Return color as a Lua table
    lua_newtable(L);
    lua_pushnumber(L, color_ptr->x); lua_setfield(L, -2, "r");
    lua_pushnumber(L, color_ptr->y); lua_setfield(L, -2, "g");
    lua_pushnumber(L, color_ptr->z); lua_setfield(L, -2, "b");
    lua_pushnumber(L, color_ptr->w); lua_setfield(L, -2, "a");
    return 1;
}

// Lua binding for igPushStyleColor_Vec4
static int lua_igPushStyleColor(lua_State *L) {
    int idx = (int)luaL_checkinteger(L, 1);
    ImVec4 color = {0, 0, 0, 1}; // Default color (black, fully opaque)
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "r"); color.x = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "g"); color.y = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "b"); color.z = (float)luaL_optnumber(L, -1, 0); lua_pop(L, 1);
        lua_getfield(L, 2, "a"); color.w = (float)luaL_optnumber(L, -1, 1); lua_pop(L, 1);
    }
    
    igPushStyleColor_Vec4(idx, color);
    return 0;
}

// Lua binding for igPopStyleColor
static int lua_igPopStyleColor(lua_State *L) {
    int count = luaL_optinteger(L, 1, 1); // Default to popping 1 style
    igPopStyleColor(count);
    return 0;
}


static void lua_set_imgui_constants(lua_State *L) {
    lua_newtable(L);
    lua_pushinteger(L, ImGuiCol_WindowBg); lua_setfield(L, -2, "WindowBg");
    // Add other ImGuiCol constants as needed
    lua_setfield(L, -2, "Col");
}

// Main
int main(int argc, char *argv[]) {
    // Initialize SDL 3.2
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        fprintf(stderr, "Failed to init video! %s\n", SDL_GetError());
        return 1;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL;
    SDL_Window *window = SDL_CreateWindow("Dear cImGui SDL3+OpenGL3 example", (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
    if (!window) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Create OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_GL_MakeCurrent(window, gl_context);

    // Setup Dear ImGui context
    igCreateContext(NULL); // imgui
    ImGuiIO *io = igGetIO(); // imgui
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // imgui
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // imgui

    // Setup Dear ImGui style
    igStyleColorsDark(NULL); // imgui
    ImGuiStyle *style = igGetStyle(); // imgui
    ImGuiStyle_ScaleAllSizes(style, main_scale); // imgui

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context); // imgui
    ImGui_ImplOpenGL3_Init("#version 330"); // imgui

    // Initialize Lua
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Register cimgui functions to Lua
    lua_newtable(L);
    lua_set_imgui_constants(L); // Set ImGuiCol constants
    //...
    lua_pushcfunction(L, lua_igBegin); lua_setfield(L, -2, "Begin");
    lua_pushcfunction(L, lua_igEnd); lua_setfield(L, -2, "End");
    lua_pushcfunction(L, lua_igText); lua_setfield(L, -2, "Text");
    lua_pushcfunction(L, lua_igCheckbox); lua_setfield(L, -2, "Checkbox");
    lua_pushcfunction(L, lua_igSliderFloat); lua_setfield(L, -2, "SliderFloat");
    lua_pushcfunction(L, lua_igColorEdit4); lua_setfield(L, -2, "ColorEdit4");
    lua_pushcfunction(L, lua_igButton); lua_setfield(L, -2, "Button");
    lua_pushcfunction(L, lua_igInputText); lua_setfield(L, -2, "InputText");
    lua_pushcfunction(L, lua_igCombo_Str); lua_setfield(L, -2, "Combo");
    lua_pushcfunction(L, lua_igCombo_Str_arr); lua_setfield(L, -2, "ComboStrArr");
    lua_pushcfunction(L, lua_igProgressBar); lua_setfield(L, -2, "ProgressBar");
    lua_pushcfunction(L, lua_igRadioButton); lua_setfield(L, -2, "RadioButton");

    lua_pushcfunction(L, lua_igSeparator); lua_setfield(L, -2, "Separator");
    lua_pushcfunction(L, lua_igSpacing); lua_setfield(L, -2, "Spacing");

    lua_pushcfunction(L, lua_igBeginTabBar); lua_setfield(L, -2, "BeginTabBar");
    lua_pushcfunction(L, lua_igEndTabBar); lua_setfield(L, -2, "EndTabBar");
    lua_pushcfunction(L, lua_igBeginTabItem); lua_setfield(L, -2, "BeginTabItem");
    lua_pushcfunction(L, lua_igEndTabItem); lua_setfield(L, -2, "EndTabItem");

    lua_pushcfunction(L, lua_igBeginColumns); lua_setfield(L, -2, "BeginColumns");
    lua_pushcfunction(L, lua_igEndColumns); lua_setfield(L, -2, "EndColumns");
    lua_pushcfunction(L, lua_igNextColumn); lua_setfield(L, -2, "NextColumn");
    lua_pushcfunction(L, lua_igSetColumnWidth); lua_setfield(L, -2, "SetColumnWidth");

    lua_pushcfunction(L, lua_igIsItemHovered); lua_setfield(L, -2, "IsItemHovered");

    lua_pushcfunction(L, lua_igBeginTooltip); lua_setfield(L, -2, "BeginTooltip");
    lua_pushcfunction(L, lua_igEndTooltip); lua_setfield(L, -2, "EndTooltip");
    lua_pushcfunction(L, lua_igSetTooltip); lua_setfield(L, -2, "SetTooltip");

    lua_pushcfunction(L, lua_igBeginChild); lua_setfield(L, -2, "BeginChild");
    lua_pushcfunction(L, lua_igEndChild); lua_setfield(L, -2, "EndChild");
    lua_pushcfunction(L, lua_igIsWindowHovered); lua_setfield(L, -2, "IsWindowHovered");

    lua_pushcfunction(L, lua_igTreeNode); lua_setfield(L, -2, "TreeNode");
    lua_pushcfunction(L, lua_igTreePop); lua_setfield(L, -2, "TreePop");

    lua_pushcfunction(L, lua_igInputTextMultiline); lua_setfield(L, -2, "InputTextMultiline");

    lua_pushcfunction(L, lua_igGetWindowHeight); lua_setfield(L, -2, "GetWindowHeight");
    lua_pushcfunction(L, lua_igGetWindowWidth); lua_setfield(L, -2, "GetWindowWidth");
    lua_pushcfunction(L, lua_igIsItemActive); lua_setfield(L, -2, "IsItemActive");
    lua_pushcfunction(L, lua_igIsItemClicked); lua_setfield(L, -2, "IsItemClicked");

    lua_pushcfunction(L, lua_igSameLine); lua_setfield(L, -2, "SameLine");
    lua_pushcfunction(L, lua_igGetFramerate); lua_setfield(L, -2, "GetFramerate");

    lua_pushcfunction(L, lua_igGetStyleColorVec4); lua_setfield(L, -2, "GetStyleColorVec4");
    lua_pushcfunction(L, lua_igPushStyleColor); lua_setfield(L, -2, "PushStyleColor");
    lua_pushcfunction(L, lua_igPopStyleColor); lua_setfield(L, -2, "PopStyleColor");

    lua_setglobal(L, "imgui");

    // Determine Lua script file
    const char *script_file = (argc > 1) ? argv[1] : "script.lua";

    // Check if file exists
    FILE *file = fopen(script_file, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open Lua script file '%s'\n", script_file);
        lua_close(L);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        igDestroyContext(NULL);
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    fclose(file);

    // Load script.lua
    if (luaL_dofile(L, script_file) != LUA_OK) {
        fprintf(stderr, "Error loading Lua script '%s': %s\n", script_file, lua_tostring(L, -1));
        lua_pop(L, 1);
        lua_close(L);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        igDestroyContext(NULL);
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    // Load script.lua
    // if (luaL_dofile(L, "script.lua") != LUA_OK) {
    //     fprintf(stderr, "Error loading script.lua: %s\n", lua_tostring(L, -1));
    //     lua_pop(L, 1);
    //     return -1;
    // }

    // Global clear color
    // ImVec4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};

    // Main loop
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event); // imgui
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame(); // imgui
        ImGui_ImplSDL3_NewFrame(); // imgui
        igNewFrame(); // imgui

        // Call Lua render_frame function
        lua_getglobal(L, "render_frame");
        if (lua_pcall(L, 0, 1, 0) != LUA_OK) { // No arguments passed
            fprintf(stderr, "Error in render_frame: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        } else if (lua_istable(L, -1)) {
            float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
            lua_getfield(L, -1, "r"); r = (float)lua_tonumber(L, -1); lua_pop(L, 1);
            lua_getfield(L, -1, "g"); g = (float)lua_tonumber(L, -1); lua_pop(L, 1);
            lua_getfield(L, -1, "b"); b = (float)lua_tonumber(L, -1); lua_pop(L, 1);
            lua_getfield(L, -1, "a"); a = (float)lua_tonumber(L, -1); lua_pop(L, 1);
            lua_pop(L, 1);
            // Set clear color for OpenGL
            glClearColor(r, g, b, a);
        } else {
            fprintf(stderr, "Error: render_frame did not return a table\n");
            lua_pop(L, 1);
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Fallback color
        }

        // Rendering
        igRender(); // imgui
        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData()); // imgui
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    lua_close(L);
    ImGui_ImplOpenGL3_Shutdown(); // imgui
    ImGui_ImplSDL3_Shutdown(); // imgui
    igDestroyContext(NULL); // imgui
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}