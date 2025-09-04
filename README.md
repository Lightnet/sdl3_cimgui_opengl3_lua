# sdl3_cimgui_opengl3_lua

# License: MIT

# Information:
  Very simple SDL 3.2.22 cimgui. OpenGL 3. 330. Lua script.



# Cmake:
 Note that cimgui has cmake build which should not be used. Still need to find correct way to config.

 Note there different way to handle the outputs.
## msys2
- There no debug or release folder output.
- Goes to the _dps/sdl3-build/
- Goes to the _dps/"name"-build/

## Window VS2022
- There debug or release folder output.
- Goes to the build folder.

# cimgui api
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

# Credits:
  - cimgui
  - Grok AI on x