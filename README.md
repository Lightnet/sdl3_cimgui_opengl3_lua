# sdl3_cimgui_opengl3_lua

# License: MIT

# Information:
  Very simple SDL 3.2.22 cimgui. OpenGL 3. 330. Lua script.

# Widgets:
- [x]Window
- [x] Text
- [x] Button
- [x] Input Field
- [x] Sliders
- [x] Checkboxes
- [x] Radio Buttons
- [x] Combo Boxes
- [x] Progress Bar
- [x] Trees and Tree Nodes
- [x] Separators and Spacing
- [x] Tab Bars and Tab Items
- [x] Tooltips
- [x] Child Windows
- [x] Columns

## Notes:
- There might be bug in script while testing.
- There chance it would crash for IsItemHovered, tooltip and other things.

## Widgets community:
- [ ] Plotting Libraries
- [ ] Node Editors
- [ ] Timeline Editors
- [ ] Flame Graphs
- [ ] File Dialogs
- [ ] Message Boxes
- [ ] Gizmos
- [ ] Texture Inspectors
- [ ] Date Pickers
- [ ] Curve Editors


# CMake:
 Note that cimgui has cmake build which should not be used. It need to config mannual but still reference CMake from cimgui.

 Note there different way to handle the outputs.

## msys2
- There no debug or release folder output.
- Goes to the _dps/sdl3-build/
- Goes to the _dps/"name"-build/

## Window VS2022:
- There debug or release folder output.
- Goes to the build folder.

# cimgui cmake config:

```
# Fetch cimgui
FetchContent_Declare(
    cimgui
    GIT_REPOSITORY https://github.com/cimgui/cimgui.git
    GIT_TAG master 
    # GIT_TAG 1.92.1
    EXCLUDE_FROM_ALL #this disable cmake build ?
    GIT_SHALLOW TRUE
    USES_TERMINAL_DOWNLOAD TRUE
)
FetchContent_MakeAvailable(cimgui)

# Define cimgui sources
set(IMGUI_SOURCES
    ${cimgui_SOURCE_DIR}/cimgui.cpp
    ${cimgui_SOURCE_DIR}/imgui/imgui.cpp
    ${cimgui_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${cimgui_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${cimgui_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${cimgui_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
    ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

# Build cimgui as a static library with a custom name
add_library(custom_cimgui STATIC ${IMGUI_SOURCES})

# Set include directories for custom_cimgui
target_include_directories(custom_cimgui PUBLIC
    ${cimgui_SOURCE_DIR}                    # cimgui
    ${cimgui_SOURCE_DIR}/imgui              # cimgui/imgui
    ${cimgui_SOURCE_DIR}/imgui/backends     # imgui/backends
    ${SDL3_SOURCE_DIR}/include              # SDL 3.2
)

# Set compile definitions for custom_cimgui (no CIMGUI_DEFINE_ENUMS_AND_STRUCTS)
target_compile_definitions(custom_cimgui PUBLIC
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_IMPL_API=extern\ \"C\"
)

# Link custom_cimgui with GLFW and OpenGL
target_link_libraries(custom_cimgui PUBLIC 
    OpenGL::GL 
    SDL3::SDL3
)

# Link custom_cimgui with GLFW and OpenGL
target_link_libraries(custom_cimgui PUBLIC 
    OpenGL::GL 
    SDL3::SDL3
)

#...

# for c #define
target_compile_definitions(
	${APP_NAME}
	PUBLIC
    #CIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
	CIMGUI_DEFINE_ENUMS_AND_STRUCTS=1
	CIMGUI_USE_SDL3=1
	CIMGUI_USE_OPENGL3=1
)
```
  After some testing this works.

## cimgui api:
-build\_deps\cimgui-src\cimgui_impl.h  
    - api cimgui
```
#ifdef CIMGUI_USE_OPENGL3
CIMGUI_API bool ImGui_ImplOpenGL3_Init(const char* glsl_version);
CIMGUI_API void ImGui_ImplOpenGL3_Shutdown(void);
CIMGUI_API void ImGui_ImplOpenGL3_NewFrame(void);
CIMGUI_API void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);
CIMGUI_API bool ImGui_ImplOpenGL3_CreateDeviceObjects(void);
CIMGUI_API void ImGui_ImplOpenGL3_DestroyDeviceObjects(void);
CIMGUI_API void ImGui_ImplOpenGL3_UpdateTexture(ImTextureData* tex);
```

# Lua CMake:
  Using the default Lua to keep it simple.

```
FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.8
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lua)

# Lua doesn't have a CMake build, so we manually compile it
# List Lua source files (core and libraries, excluding lua.c and luac.c)
set(LUA_SRC
    ${lua_SOURCE_DIR}/lapi.c
    ${lua_SOURCE_DIR}/lauxlib.c
    ${lua_SOURCE_DIR}/lbaselib.c
    ${lua_SOURCE_DIR}/lcode.c
    ${lua_SOURCE_DIR}/lcorolib.c
    ${lua_SOURCE_DIR}/lctype.c
    ${lua_SOURCE_DIR}/ldblib.c
    ${lua_SOURCE_DIR}/ldebug.c
    ${lua_SOURCE_DIR}/ldo.c
    ${lua_SOURCE_DIR}/ldump.c
    ${lua_SOURCE_DIR}/lfunc.c
    ${lua_SOURCE_DIR}/lgc.c
    ${lua_SOURCE_DIR}/linit.c
    ${lua_SOURCE_DIR}/liolib.c
    ${lua_SOURCE_DIR}/llex.c
    ${lua_SOURCE_DIR}/lmathlib.c
    ${lua_SOURCE_DIR}/lmem.c
    ${lua_SOURCE_DIR}/loadlib.c
    ${lua_SOURCE_DIR}/lobject.c
    ${lua_SOURCE_DIR}/lopcodes.c
    ${lua_SOURCE_DIR}/loslib.c
    ${lua_SOURCE_DIR}/lparser.c
    ${lua_SOURCE_DIR}/lstate.c
    ${lua_SOURCE_DIR}/lstring.c
    ${lua_SOURCE_DIR}/lstrlib.c
    ${lua_SOURCE_DIR}/ltable.c
    ${lua_SOURCE_DIR}/ltablib.c
    ${lua_SOURCE_DIR}/ltm.c
    ${lua_SOURCE_DIR}/lundump.c
    ${lua_SOURCE_DIR}/lvm.c
    ${lua_SOURCE_DIR}/lzio.c
    ${lua_SOURCE_DIR}/lutf8lib.c
)

# Create a static Lua library
add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})
#...
target_link_libraries(${APP_NAME} PRIVATE lua) # lua 5.4.7
```
  This is lua build library.

# Credits:
  - cimgui
  - Grok AI on x