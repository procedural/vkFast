// Source: https://gist.github.com/ocornut/7e9b3ec566a333d725d4/7006383a991ff86f6ebd60d1ef2b007f7b474867

// Creating a node graph editor for ImGui
// Quick demo, not production code! This is more of a demo of how to use ImGui to create custom stuff.
// Better version by @daniel_collin here https://gist.github.com/emoon/b8ff4b4ce4f1b43e79f2
// See https://github.com/ocornut/imgui/issues/306
// v0.02
// Animated gif: https://cloud.githubusercontent.com/assets/8225057/9472357/c0263c04-4b4c-11e5-9fdf-2cd4f33f6582.gif

#include <vector>

enum ImGuiSetCond_
{
    ImGuiSetCond_Always        = 1 << 0, // Set the variable
    ImGuiSetCond_Once          = 1 << 1, // Set the variable once per runtime session (only the first call with succeed)
    ImGuiSetCond_FirstUseEver  = 1 << 2, // Set the variable if the window has no saved data (if doesn't exist in the .ini file)
    ImGuiSetCond_Appearing     = 1 << 3  // Set the variable if the window is appearing after being hidden/inactive (or the first time)
};

enum ImGuiStyleVar_
{
    ImGuiStyleVar_Alpha,               // float
    ImGuiStyleVar_WindowPadding,       // ImVec2
    ImGuiStyleVar_WindowRounding,      // float
    ImGuiStyleVar_WindowMinSize,       // ImVec2
    ImGuiStyleVar_ChildWindowRounding, // float
    ImGuiStyleVar_FramePadding,        // ImVec2
    ImGuiStyleVar_FrameRounding,       // float
    ImGuiStyleVar_ItemSpacing,         // ImVec2
    ImGuiStyleVar_ItemInnerSpacing,    // ImVec2
    ImGuiStyleVar_IndentSpacing,       // float
    ImGuiStyleVar_GrabMinSize,         // float
    ImGuiStyleVar_ButtonTextAlign,     // flags ImGuiAlign_*
    ImGuiStyleVar_Count_
};

enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              // Background of normal windows
    ImGuiCol_ChildWindowBg,         // Background of child windows
    ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_TitleBgActive,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_ComboBg,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Column,
    ImGuiCol_ColumnHovered,
    ImGuiCol_ColumnActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_CloseButton,
    ImGuiCol_CloseButtonHovered,
    ImGuiCol_CloseButtonActive,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_ModalWindowDarkening,  // darken entire screen when a modal window is active
    ImGuiCol_COUNT
};

enum ImGuiWindowFlags_
{
    // Default: 0
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programatically)
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    ImGuiWindowFlags_ShowBorders            = 1 << 7,   // Show borders around windows and items
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    ImGuiWindowFlags_NoInputs               = 1 << 9,   // Disable catching mouse or keyboard inputs
    ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
    // [Internal]
    ImGuiWindowFlags_ChildWindow            = 1 << 20,  // Don't use! For internal use by BeginChild()
    ImGuiWindowFlags_ChildWindowAutoFitX    = 1 << 21,  // Don't use! For internal use by BeginChild()
    ImGuiWindowFlags_ChildWindowAutoFitY    = 1 << 22,  // Don't use! For internal use by BeginChild()
    ImGuiWindowFlags_ComboBox               = 1 << 23,  // Don't use! For internal use by ComboBox()
    ImGuiWindowFlags_Tooltip                = 1 << 24,  // Don't use! For internal use by BeginTooltip()
    ImGuiWindowFlags_Popup                  = 1 << 25,  // Don't use! For internal use by BeginPopup()
    ImGuiWindowFlags_Modal                  = 1 << 26,  // Don't use! For internal use by BeginPopupModal()
    ImGuiWindowFlags_ChildMenu              = 1 << 27   // Don't use! For internal use by BeginMenu()
};

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }

// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
static void ShowExampleAppCustomNodeGraph(bool* opened)
{
    igSetNextWindowSize(ImVec2(700,600), ImGuiSetCond_FirstUseEver);
    if (!igBegin("Example: Custom Node Graph", opened, 0))
    {
        igEnd();
        return;
    }

    // Dummy
    struct Node
    {
        int     ID;
        char    Name[32];
        ImVec2  Pos, Size;
        float   Value;
        ImVec4  Color;
        int     InputsCount, OutputsCount;

        Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count) { ID = id; strncpy(Name, name, 31); Name[31] = 0; Pos = pos; Value = value; Color = color; InputsCount = inputs_count; OutputsCount = outputs_count; }

        ImVec2 GetInputSlotPos(int slot_no) const   { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no+1) / ((float)InputsCount+1)); }
        ImVec2 GetOutputSlotPos(int slot_no) const  { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no+1) / ((float)OutputsCount+1)); }
    };
    struct NodeLink
    {
        int     InputIdx, InputSlot, OutputIdx, OutputSlot;

        NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
    };

    static std::vector<Node> nodes;
    static std::vector<NodeLink> links;
    static bool inited = false;
    static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    static bool show_grid = true;
    static int node_selected = -1;
    if (!inited)
    {
        nodes.push_back(Node(0, "MainTex",  ImVec2(40,50), 0.5f, ImVec4(255/255.f,100/255.f,100/255.f,255/255.f), 1, 1));
        nodes.push_back(Node(1, "BumpMap",  ImVec2(40,150), 0.42f, ImVec4(200/255.f,100/255.f,200/255.f,255/255.f), 1, 1));
        nodes.push_back(Node(2, "Combine", ImVec2(270,80), 1.0f, ImVec4(0/255.f,200/255.f,100/255.f,255/255.f), 2, 2));
        links.push_back(NodeLink(0, 0, 2, 0));
        links.push_back(NodeLink(1, 0, 2, 1));
        inited = true;
    }

    // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    igBeginChild("node_list", ImVec2(100,0), 0, 0);
    igText("Nodes");
    igSeparator();
    for (int node_idx = 0; node_idx < nodes.size(); node_idx++)
    {
        Node* node = &nodes[node_idx];
        igPushIdInt(node->ID);
        if (igSelectable(node->Name, node->ID == node_selected, 0, ImVec2(0,0)))
            node_selected = node->ID;
        if (igIsItemHovered())
        {
            node_hovered_in_list = node->ID;
            open_context_menu |= igIsMouseClicked(1, 0);
        }
        igPopId();
    }
    igEndChild();

    igSameLine(0, 0);
    igBeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

    // Create our child canvas
    igText("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    igSameLine(igGetWindowWidth()-100, 0);
    igCheckbox("Show grid", &show_grid);
    igPushStyleVarVec(ImGuiStyleVar_FramePadding, ImVec2(1,1));
    igPushStyleVarVec(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    igPushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(60/255.f,60/255.f,70/255.f,200/255.f));
    igBeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);
    igPushItemWidth(120.0f);

    ImVec2 cursorScreenPos = {0};
    igGetCursorScreenPos(&cursorScreenPos);
    ImVec2 offset = cursorScreenPos - scrolling;
    ImDrawList* draw_list = igGetWindowDrawList();
    ImDrawList_ChannelsSplit(draw_list, 2);

    // Display grid
    if (show_grid)
    {
        ImU32 GRID_COLOR = igColorConvertFloat4ToU32(ImVec4(200/255.f,200/255.f,200/255.f,40/255.f));
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = cursorScreenPos;
        ImVec2 canvas_sz = {0};
        igGetWindowSize(&canvas_sz);
        for (float x = fmodf(offset.x,GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            ImDrawList_AddLine(draw_list, ImVec2(x,0.0f)+win_pos, ImVec2(x,canvas_sz.y)+win_pos, GRID_COLOR, 2);
        for (float y = fmodf(offset.y,GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            ImDrawList_AddLine(draw_list, ImVec2(0.0f,y)+win_pos, ImVec2(canvas_sz.x,y)+win_pos, GRID_COLOR, 2);
    }

    // Display links
    ImDrawList_ChannelsSetCurrent(draw_list, 0); // Background
    for (int link_idx = 0; link_idx < links.size(); link_idx++)
    {
        NodeLink* link = &links[link_idx];
        Node* node_inp = &nodes[link->InputIdx];
        Node* node_out = &nodes[link->OutputIdx];
        ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
        ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
        ImDrawList_AddBezierCurve(draw_list, p1, p1+ImVec2(+50,0), p2+ImVec2(-50,0), p2, igColorConvertFloat4ToU32(ImVec4(200/255.f,200/255.f,100/255.f,255/255.f)), 2.0f, 0);
    }

    // Display nodes
    for (int node_idx = 0; node_idx < nodes.size(); node_idx++)
    {
        Node* node = &nodes[node_idx];
        igPushIdInt(node->ID);
        ImVec2 node_rect_min = offset + node->Pos;

        // Display node contents first
        ImDrawList_ChannelsSetCurrent(draw_list, 1); // Foreground
        bool old_any_active = igIsAnyItemActive();
        igSetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
        igBeginGroup(); // Lock horizontal position
        igText("%s", node->Name);
        igSliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f", 1);
        igColorEdit3("##color", &node->Color.x);
        igEndGroup();

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!old_any_active && igIsAnyItemActive());
        ImVec2 rect_size = {0};
        igGetItemRectSize(&rect_size);
        node->Size = rect_size + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
        ImVec2 node_rect_max = node_rect_min + node->Size;

        // Display node box
        ImDrawList_ChannelsSetCurrent(draw_list, 0); // Background
        igSetCursorScreenPos(node_rect_min);
        igInvisibleButton("node", node->Size);
        if (igIsItemHovered())
        {
            node_hovered_in_scene = node->ID;
            open_context_menu |= igIsMouseClicked(1, 0);
        }
        bool node_moving_active = igIsItemActive();
        if (node_widgets_active || node_moving_active) {
            node_selected = node->ID;
        }

        {
            static struct ImVec2 gNodeMovingMouseDelta = {0};
            static struct ImVec2 gNodeMovingMouseDeltaOriginal = {0};
            if (node_moving_active && igIsMouseDragging(0, -1.0f)) {
                struct ImVec2 mouseDelta = {0};
                igGetMouseDragDelta(&mouseDelta, 0, -1.0f);
                gNodeMovingMouseDeltaOriginal = mouseDelta;
                mouseDelta = mouseDelta - gNodeMovingMouseDelta;
                node->Pos = node->Pos + mouseDelta;
                gNodeMovingMouseDelta = gNodeMovingMouseDeltaOriginal;
            }
            if (igIsMouseDragging(0, -1.0f) == false) {
                gNodeMovingMouseDelta = {0};
                gNodeMovingMouseDeltaOriginal = {0};
            }
        }

        ImU32 node_bg_color = igColorConvertFloat4ToU32((node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? ImVec4(75/255.f,75/255.f,75/255.f,255/255.f) : ImVec4(60/255.f,60/255.f,60/255.f,255/255.f));
        ImDrawList_AddRectFilled(draw_list, node_rect_min, node_rect_max, node_bg_color, 4.0f, ~0);
        ImDrawList_AddRect(draw_list, node_rect_min, node_rect_max, igColorConvertFloat4ToU32(ImVec4(100/255.f,100/255.f,100/255.f,255/255.f)), 4.0f, ~0, 1);
        for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
            ImDrawList_AddCircleFilled(draw_list, offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, igColorConvertFloat4ToU32(ImVec4(150/255.f,150/255.f,150/255.f,150/255.f)), 12);
        for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
            ImDrawList_AddCircleFilled(draw_list, offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, igColorConvertFloat4ToU32(ImVec4(150/255.f,150/255.f,150/255.f,150/255.f)), 12);

        igPopId();
    }
    ImDrawList_ChannelsMerge(draw_list);

    // Open context menu
    if (!igIsAnyItemHovered() && igIsMouseHoveringWindow() && igIsMouseClicked(1, 0))
    {
        node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
        open_context_menu = true;
    }
    if (open_context_menu)
    {
        igOpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            node_selected = node_hovered_in_scene;
    }

    // Draw context menu
    igPushStyleVarVec(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    if (igBeginPopup("context_menu"))
    {
        Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
        ImVec2 mouse_pos_on_popup = {0};
        igGetMousePosOnOpeningCurrentPopup(&mouse_pos_on_popup);
        ImVec2 scene_pos = mouse_pos_on_popup - offset;
        if (node)
        {
            igText("Node '%s'", node->Name);
            igSeparator();
            if (igMenuItem("Rename..", NULL, false, false)) {}
            if (igMenuItem("Delete", NULL, false, false)) {}
            if (igMenuItem("Copy", NULL, false, false)) {}
        }
        else
        {
            if (igMenuItem("Add", NULL, false, true)) { nodes.push_back(Node(nodes.size(), "New node", scene_pos, 0.5f, ImVec4(100/255.f,100/255.f,200/255.f,255/255.f), 2, 2)); }
            if (igMenuItem("Paste", NULL, false, false)) {}
        }
        igEndPopup();
    }
    igPopStyleVar(1);

    // Scrolling
    {
        static struct ImVec2 gMouseDelta = {0};
        static struct ImVec2 gMouseDeltaOriginal = {0};
        if (igIsWindowHovered() && !igIsAnyItemActive() && igIsMouseDragging(2, 0.0f)) {
            struct ImVec2 mouseDelta = {0};
            igGetMouseDragDelta(&mouseDelta, 2, -1.0f);
            gMouseDeltaOriginal = mouseDelta;
            mouseDelta = mouseDelta - gMouseDelta;
            scrolling = scrolling - mouseDelta;
            gMouseDelta = gMouseDeltaOriginal;
        } else {
            gMouseDelta = {0};
            gMouseDeltaOriginal = {0};
        }
    }

    igPopItemWidth();
    igEndChild();
    igPopStyleColor(1);
    igPopStyleVar(2);
    igEndGroup();

    igEnd();
}
