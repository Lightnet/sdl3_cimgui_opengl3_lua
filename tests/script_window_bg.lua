-- this for sdl3 clear color background color
local clear_color_test = {r = 0.45, g = 0.55, b = 0.60, a = 1.00}

-- Clear color state (initialized with ImGui's default)
local clear_color = imgui.GetStyleColorVec4(imgui.Col.WindowBg)

-- Helper function to compare color tables
local function colors_equal(c1, c2)
    return c1.r == c2.r and c1.g == c2.g and c1.b == c2.b and c1.a == c2.a
end

-- Getter for clear color
function get_clear_color()
    return clear_color
end

-- Setter for clear color (updated to accept r, g, b, a)
function set_clear_color(r, g, b, a)
    clear_color = {r = r, g = g, b = b, a = a}
    imgui.PushStyleColor(imgui.Col.WindowBg, clear_color)
    imgui.PopStyleColor() -- Ensure style stack is balanced
end

function render_frame()
    imgui.Begin("Demo Window", show_demo_window)
    imgui.Text("ImGui Demo Window (native)")

    if imgui.Button("Reset Color") then
        local default_color = imgui.GetStyleColorVec4(imgui.Col.WindowBg)
        set_clear_color(default_color.r, default_color.g, default_color.b, default_color.a)
    end

    if imgui.Button("Reset Color 2") then
        print("test")
        -- local default_color = imgui.GetStyleColorVec4(imgui.Col.WindowBg)
        set_clear_color(0.1, 0.1, 0.5, 1.0)
    end

    local current_color = imgui.GetStyleColorVec4(imgui.Col.WindowBg)
    -- print(current_color)
    -- print(clear_color_test)

    local new_color, changed = imgui.ColorEdit4("Clear Color", current_color.r, current_color.g, current_color.b, current_color.a)
    if changed then
        set_clear_color(new_color.r, new_color.g, new_color.b, new_color.a)
    end
    -- local new_color = imgui.ColorEdit4("Clear Color", current_color)
    -- local new_color = imgui.ColorEdit4("Clear Color", clear_color_test)
    -- if not colors_equal(new_color, current_color) then
    --     set_clear_color(new_color)
    -- end
    -- if imgui.IsItemHovered() then
    --     imgui.BeginTooltip()
    --     imgui.Text("Clear Color")
    --     imgui.Text(string.format("R: %.2f, G: %.2f, B: %.2f, A: %.2f", new_color.r, new_color.g, new_color.b, new_color.a))
    --     imgui.EndTooltip()
    -- end

    imgui.End()

    return get_clear_color() -- Return clear color from getter
end