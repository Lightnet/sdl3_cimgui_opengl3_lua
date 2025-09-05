-- script.lua
local show_demo_window = true
local show_another_window = false
local f = 0.0
local counter = 0
local input_text = "Edit me!"
local combo_index = 1
local combo_items = {"Option 1", "Option 2", "Option 3"}
local progress = 0.0
local timer = 0.0
local radio_selection = 1
local tab1_open = true
local tab2_open = true
local tab3_open = true

function render_frame(clear_color)

    -- Demo window
    if show_demo_window then
        imgui.Begin("Demo Window", show_demo_window)
        imgui.Text("ImGui Demo Window (native)")
        imgui.End()
    end

    return clear_color
end