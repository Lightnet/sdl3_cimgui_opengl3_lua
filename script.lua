-- script.lua
local show_demo_window = true
local show_another_window = false
local f = 0.0
local counter = 0
local input_text = "Edit me!" -- New state for text input
local combo_index = 1 -- New state for combo box
local combo_items = {"Option 1", "Option 2", "Option 3"} -- Combo box options

function render_frame(clear_color)
    -- Demo window (using ImGui's built-in demo, not replicated in Lua)
    if show_demo_window then
        imgui.Begin("Demo Window", show_demo_window)
        imgui.Text("ImGui Demo Window (native)")
        imgui.End()
    end

    -- Simple window
    imgui.Begin("Hello, world!", true)
    imgui.Text("This is some useful text.")
    show_demo_window = imgui.Checkbox("Demo Window", show_demo_window)
    show_another_window = imgui.Checkbox("Another Window", show_another_window)
    f = imgui.SliderFloat("float", f, 0.0, 1.0, "%.3f")
    clear_color = imgui.ColorEdit4("clear color", clear_color)
    -- input_text = imgui.InputText("Input", input_text, 256) -- text input

    local new_text, changed = imgui.InputText("Input", input_text, 256)
    if changed then
        print("Text changed to: " .. new_text)
    end
    input_text = new_text

    -- combo_index = imgui.Combo("Combo", combo_index, combo_items) -- combo box
    combo_index = imgui.ComboStrArr("Combo", combo_index, combo_items)
    imgui.Text(string.format("Selected: %s", combo_items[combo_index]))







    if imgui.Button("Button") then
        counter = counter + 1
    end
    imgui.SameLine()
    imgui.Text(string.format("counter = %d", counter))
    imgui.Text(string.format("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / imgui.GetFramerate(), imgui.GetFramerate()))
    imgui.End()

    -- Another window
    if show_another_window then
        imgui.Begin("Another Window", show_another_window)
        imgui.Text("Hello from another window!")
        if imgui.Button("Close Me") then
            show_another_window = false
        end
        imgui.End()
    end

    return clear_color
end