
local radio_selection = 1

function render_frame(clear_color)
    imgui.Begin("Demo Window", show_demo_window)
        
        imgui.Text("ImGui Demo Window (native)")
        imgui.Text("Radio Selection:")
        if imgui.RadioButton("Choice 1", radio_selection == 1) then radio_selection = 1 end
        if imgui.IsItemActive() then
            print("Item is active")
        end
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