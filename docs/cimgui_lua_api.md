
# Information:
  Note it should have similar format. Since it c language it need to format correctly. Note there may be inorrect set up. Need to have sample test for each.

# InputText:
```lua
local new_text, changed = imgui.InputText("Label", current_text, buf_size, flags)
```

```lua
local input_text = "Edit me!" -- New state for text input
--...
local new_text, changed = imgui.InputText("Input", input_text, 256)
if changed then
    print("Text changed to: " .. new_text)
end
input_text = new_text
```

# lua_igCombo_Str:
  Render bug when list appear.
```lua
local combo_index = 1 -- New state for combo box
local combo_items = {"Option 1", "Option 2", "Option 3"} -- Combo box options
--...
-- local new_index, changed = imgui.Combo("Label", current_index, {"Item1", "Item2", "Item3"})
combo_index = imgui.Combo("Combo", combo_index, combo_items)
imgui.Text(string.format("Selected: %s", combo_items[combo_index]))
```

```lua
```

# ComboStrArr :
 Working.
```lua
combo_index = imgui.ComboStrArr("Combo", combo_index, combo_items)
```

```lua

```

## Combo Comparison:
- igCombo_Str: Converts the Lua table to a single null-separated string, which is how the original binding worked. It’s compatible with the existing script.lua.

- igCombo_Str_arr: Passes an array of string pointers, which is more direct and potentially more efficient for large lists, but requires slightly different C-side handling.

# igBeginCombo and igEndCombo:
   These are used for custom combo box rendering, where you manually handle the dropdown list (e.g., with igSelectable). They’re more complex to bind to Lua due to the need to iterate over items in Lua during rendering.

# igCombo_FnStrPtr:
  This uses a callback function to retrieve items dynamically, which is useful for dynamic or large lists. Binding this to Lua would require passing a Lua function as a callback, adding complexity. It’s not needed for your current use case (static list of strings).


#  ProgressBar:

```lua
imgui.ProgressBar(0.5, {x = 100, y = 0}, "50%")
```

```lua
imgui.ProgressBar(0.5)
```

```lua
imgui.ProgressBar(progress, {x = -1, y = 0}, string.format("%.0f%%", progress * 100))
```


## Input Parameters:
- fraction: A number between 0.0 and 1.0 (clamped by ImGui) representing the progress.
- size: A Lua table {x, y} for the progress bar’s width and height. If omitted, defaults to {0, 0} (ImGui’s default size).
- overlay: An optional string to display over the progress bar (e.g., “50%”). If nil, no overlay is shown.




#  RadioButton:

```lua
local clicked = imgui.RadioButton("Option 1", selected_index == 1)
```

```lua

```

# Separator:

```lua
imgui.Separator()
```


# Spacing:

```lua
imgui.Spacing()
```

#  SetTooltip:

```lua
show_another_window = imgui.Checkbox("Another Window", show_another_window)
if imgui.IsItemHovered() then
    imgui.SetTooltip("Toggle the demo window") -- Simple tooltip
end
```

```lua
-- need to be gui here before like input or label display tool tips
if imgui.IsItemHovered() then
    imgui.BeginTooltip()
    imgui.Text("test")
    imgui.Text("test")
    imgui.EndTooltip()
end
```

# Columns:

```lua
imgui.BeginColumns("MyColumns", 2)
imgui.Text("Column 1")
imgui.NextColumn()
imgui.Text("Column 2")
imgui.SetColumnWidth(0, 200) -- Set first column width
imgui.EndColumns()
```

```lua
imgui.BeginColumns("ControlColumns", 2) -- Two-column layout
imgui.SetColumnWidth(0, 200) -- Set first column width to 200 pixels
show_demo_window = imgui.Checkbox("Demo Window", show_demo_window)
imgui.SetTooltip("Toggle the demo window")
show_another_window = imgui.Checkbox("Another Window", show_another_window)
imgui.SetTooltip("Toggle another window")
imgui.NextColumn() -- Switch to second column
imgui.Spacing()
f = imgui.SliderFloat("float", f, 0.0, 1.0, "%.3f")
if imgui.IsItemHovered() then
    imgui.BeginTooltip()
    imgui.Text(string.format("Slider value: %.3f", f))
    imgui.Text("Adjusts a floating-point value")
    imgui.EndTooltip()
end
imgui.EndColumns() -- End column layout
```

# BeginChild:

```lua
if imgui.BeginChild("StatusChild", {x = 0, y = 150}, true) then
    imgui.Text("Status Child Window")
    imgui.EndChild()
end
```
  Wrong code.

```lua
if imgui.BeginChild("StatusChild", {x = 0, y = 150}, true) then
    imgui.Text("Status Child Window")
end
imgui.EndChild()
```
  This should work else the error.

```c++
if (ImGui::BeginChild("MyChildWindow", ImVec2(200, 150), true, ImGuiChildFlags_None))
{
    ImGui::Text("This is content inside the child window.");
    ImGui::Button("Child Button");
    // You can add more widgets and logic here
}
ImGui::EndChild(); // Always call EndChild for each BeginChild
```
  Example from c++.


# TreeNode:

```lua
if imgui.TreeNode("NodeID", "Node Label") then
    imgui.Text("Node content")
    imgui.TreePop()
end
```

```lua
if imgui.TreeNode("RadioTree", "Radio Options") then
    imgui.Text("Radio Selection:")
    if imgui.RadioButton("Choice 1", radio_selection == 1) then radio_selection = 1 end
    imgui.SameLine()
    if imgui.RadioButton("Choice 2", radio_selection == 2) then radio_selection = 2 end
    imgui.SameLine()
    if imgui.RadioButton("Choice 3", radio_selection == 3) then radio_selection = 3 end
    imgui.Text(string.format("Selected Choice: %d", radio_selection))
    if imgui.IsItemHovered() then
        imgui.BeginTooltip()
        imgui.Text("Radio Options Node")
        imgui.Text(string.format("Current selection: %d", radio_selection))
        imgui.EndTooltip()
    end
    imgui.TreePop()
end
```

# igInputTextMultiline:
```c
CIMGUI_API bool igInputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2 size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
```
## Parameters:
- label: The text label for the input field.
- buf: A character buffer to store the input text.
- buf_size: The size of the buffer (to prevent overflow).
- size: The size of the input field (ImVec2 for width and height).
- flags: Optional ImGuiInputTextFlags (e.g., ImGuiInputTextFlags_EnterReturnsTrue).
- callback and user_data: Optional for custom input handling (we’ll omit these for simplicity).

- Return: true if the text was edited this frame, false

```lua
local multiline_text = "Enter notes here...\nLine 2\nLine 3" -- New state for multiline text

--...

local new_text, changed = imgui.InputTextMultiline("Notes", multiline_text, 1024, {x = 0, y = 100})
if changed then
    multiline_text = new_text
end
```

# window events:

```lua
local height = imgui.GetWindowHeight()
local width = imgui.GetWindowWidth()
if imgui.IsItemActive() then
    print("Item is active")
end
if imgui.IsItemClicked() then
    print("Item was clicked")
end
```

# IsItemClicked:
  Note this has be under widget.


```lua
--widget here
if imgui.IsItemClicked() then
    print("radio_selection IsItemClicked")
end
```

```lua
local radio_selection = 1
function render_frame(clear_color)
    imgui.Begin("Demo Window", show_demo_window)
        imgui.Text("ImGui Demo Window (native)")
        imgui.Text("Radio Selection:")
        if imgui.RadioButton("Choice 1", radio_selection == 1) then radio_selection = 1 end
        imgui.SameLine()
        if imgui.RadioButton("Choice 2", radio_selection == 2) then radio_selection = 2 end
        imgui.SameLine()
        if imgui.RadioButton("Choice 3", radio_selection == 3) then radio_selection = 3 end
        if imgui.IsItemClicked() then
            print("radio_selection IsItemClicked")
        end
        imgui.Text(string.format("Selected Choice: %d", radio_selection))
    imgui.End()
    return clear_color
end
```

# window bg:

```lua
local color = imgui.GetStyleColorVec4(imgui.Col.WindowBg) -- Returns {r, g, b, a}
imgui.PushStyleColor(imgui.Col.WindowBg, {r = 0.2, g = 0.3, b = 0.4, a = 1.0})
imgui.PopStyleColor() -- Pop the style
```