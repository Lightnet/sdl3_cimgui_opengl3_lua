local multiline_text = "Enter notes here...\nLine 2\nLine 3" -- New state for multiline text


function render_frame(clear_color)

    imgui.Begin("Demo Window", show_demo_window)
    imgui.Text("ImGui Demo Window (native)")
    -- Multiline text input
    local new_text, changed = imgui.InputTextMultiline("Notes", multiline_text, 1024, {x = 0, y = 100})
    if changed then
        multiline_text = new_text
    end
    imgui.End()

    return clear_color
end