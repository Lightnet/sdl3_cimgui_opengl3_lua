


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







#  :

```lua

```

```lua

```

#  :

```lua

```

```lua

```

#  :

```lua

```

```lua

```

#  :

```lua

```

```lua

```