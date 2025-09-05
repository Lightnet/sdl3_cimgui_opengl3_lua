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
    -- Update progress animation
    timer = timer + (1.0 / imgui.GetFramerate())
    progress = (math.sin(timer) + 1) / 2

    -- Demo window
    if show_demo_window then
        imgui.Begin("Demo Window", show_demo_window)
        imgui.Text("ImGui Demo Window (native)")
        imgui.End()
    end

    imgui.Begin("Child", true)
        imgui.Text("Status Child Window")
        if imgui.BeginChild("StatusChild", {x = 0, y = 150}, true) then
            imgui.Text("Status Child Window")
            imgui.EndChild()
        end
    imgui.End()

    -- Simple window
    imgui.Begin("Hello, world!", true)
    imgui.Text("This is some useful text.")

    -- Tab bar
    if imgui.BeginTabBar("MainTabBar") then
        -- Tab 1: Checkboxes and Slider in Columns
        local p_open, is_active = imgui.BeginTabItem("Controls", tab1_open)
        if is_active then
            imgui.BeginColumns("ControlColumns", 2)
            imgui.SetColumnWidth(0, 200)
            show_demo_window = imgui.Checkbox("Demo Window", show_demo_window)
            imgui.SetTooltip("Toggle the demo window")
            show_another_window = imgui.Checkbox("Another Window", show_another_window)
            imgui.SetTooltip("Toggle another window")
            imgui.NextColumn()
            imgui.Spacing()
            f = imgui.SliderFloat("float", f, 0.0, 1.0, "%.3f")
            if imgui.IsItemHovered() then
                if imgui.BeginTooltip() then
                    imgui.Text(string.format("Slider value: %.3f", f))
                    imgui.Text("Adjusts a floating-point value")
                    imgui.EndTooltip()
                end
            end
            imgui.EndColumns()
            imgui.EndTabItem()
        end
        tab1_open = p_open or tab1_open

        -- Tab 2: Text Input and Combo
        p_open, is_active = imgui.BeginTabItem("Input", tab2_open)
        if is_active then
            if imgui.IsItemHovered() then
                imgui.SetTooltip("This is the Input tab")
            end
            imgui.Spacing()
            clear_color = imgui.ColorEdit4("clear color", clear_color)
            imgui.Spacing()
            input_text = imgui.InputText("Input", input_text, 256)
            imgui.Spacing()
            combo_index = imgui.Combo("Combo", combo_index, combo_items)
            imgui.Text(string.format("Selected: %s", combo_items[combo_index]))
            imgui.EndTabItem()
        end
        tab2_open = p_open or tab2_open

        -- Tab 3: Progress Bar and Radio Buttons in Child Window
        p_open, is_active = imgui.BeginTabItem("Status", tab3_open)
        if is_active then
            imgui.Spacing()
            -- Child window for progress bar and radio buttons
            if imgui.BeginChild("StatusChild", {x = 0, y = 150}, true) then
                imgui.ProgressBar(progress, {x = -1, y = 0}, string.format("%.0f%%", progress * 100))
                imgui.Separator()
                imgui.Spacing()
                imgui.Text("Radio Selection:")
                if imgui.RadioButton("Choice 1", radio_selection == 1) then radio_selection = 1 end
                imgui.SameLine()
                if imgui.RadioButton("Choice 2", radio_selection == 2) then radio_selection = 2 end
                imgui.SameLine()
                if imgui.RadioButton("Choice 3", radio_selection == 3) then radio_selection = 3 end
                imgui.Text(string.format("Selected Choice: %d", radio_selection))
                if imgui.IsWindowHovered() then
                    if imgui.BeginTooltip() then
                        imgui.Text("Status Child Window")
                        imgui.Text("Contains progress and radio buttons")
                        imgui.EndTooltip()
                    end
                end
                imgui.EndChild()
            end
            imgui.EndTabItem()
        end
        tab3_open = p_open or tab3_open

        imgui.EndTabBar()
    end

    imgui.Separator()
    imgui.Spacing()
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