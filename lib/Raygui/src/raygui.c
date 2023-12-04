/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** raygui.c
*/

#include "raygui.h"

//----------------------------------------------------------------------------------
// Gui Setup Functions Definition
//----------------------------------------------------------------------------------
// Enable gui global state
// NOTE: We check for STATE_DISABLED to avoid messing custom global state setups
void GuiEnable(void) { if (guiState == STATE_DISABLED) guiState = STATE_NORMAL; }

// Disable gui global state
// NOTE: We check for STATE_NORMAL to avoid messing custom global state setups
void GuiDisable(void) { if (guiState == STATE_NORMAL) guiState = STATE_DISABLED; }

// Lock gui global state
void GuiLock(void) { guiLocked = true; }

// Unlock gui global state
void GuiUnlock(void) { guiLocked = false; }

// Check if gui is locked (global state)
bool GuiIsLocked(void) { return guiLocked; }

// Set gui controls alpha global state
void GuiSetAlpha(float alpha)
{
    if (alpha < 0.0f) alpha = 0.0f;
    else if (alpha > 1.0f) alpha = 1.0f;

    guiAlpha = alpha;
}

// Set gui state (global state)
void GuiSetState(int state) { guiState = (GuiState)state; }

// Get gui state (global state)
int GuiGetState(void) { return guiState; }

// Set custom gui font
// NOTE: Font loading/unloading is external to raygui
void GuiSetFont(Font font)
{
    if (font.texture.id > 0)
    {
        // NOTE: If we try to setup a font but default style has not been
        // lazily loaded before, it will be overwritten, so we need to force
        // default style loading first
        if (!guiStyleLoaded) GuiLoadStyleDefault();

        guiFont = font;
    }
}

// Get custom gui font
Font GuiGetFont(void)
{
    return guiFont;
}

// Set control style property value
void GuiSetStyle(int control, int property, int value)
{
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    guiStyle[control*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property] = value;

    // Default properties are propagated to all controls
    if ((control == 0) && (property < RAYGUI_MAX_PROPS_BASE))
    {
        for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) guiStyle[i*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property] = value;
    }
}

// Get control style property value
int GuiGetStyle(int control, int property)
{
    if (!guiStyleLoaded) GuiLoadStyleDefault();
    return guiStyle[control*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED) + property];
}

//----------------------------------------------------------------------------------
// Gui Controls Functions Definition
//----------------------------------------------------------------------------------

// Window Box control
int GuiWindowBox(Rectangle bounds, const char *title)
{
    // Window title bar height (including borders)
    // NOTE: This define is also used by GuiMessageBox() and GuiTextInputBox()
    #if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
        #define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT        24
    #endif

    int result = 0;
    //GuiState state = guiState;

    int statusBarHeight = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;

    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)statusBarHeight };
    if (bounds.height < statusBarHeight*2.0f) bounds.height = statusBarHeight*2.0f;

    Rectangle windowPanel = { bounds.x, bounds.y + (float)statusBarHeight - 1, bounds.width, bounds.height - (float)statusBarHeight + 1 };
    Rectangle closeButtonRec = { statusBar.x + statusBar.width - GuiGetStyle(STATUSBAR, BORDER_WIDTH) - 20,
                                 statusBar.y + statusBarHeight/2.0f - 18.0f/2.0f, 18, 18 };

    // Update control
    //--------------------------------------------------------------------
    // NOTE: Logic is directly managed by button
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiStatusBar(statusBar, title); // Draw window header as status bar
    GuiPanel(windowPanel, NULL);    // Draw window base

    // Draw window close button
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
#if defined(RAYGUI_NO_ICONS)
    result = GuiButton(closeButtonRec, "x");
#else
    result = GuiButton(closeButtonRec, GuiIconText(ICON_CROSS_SMALL, NULL));
#endif
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlignment);
    //--------------------------------------------------------------------

    return result;      // Window close button clicked: result = 1
}

// Group Box control with text name
int GuiGroupBox(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_GROUPBOX_LINE_THICK)
        #define RAYGUI_GROUPBOX_LINE_THICK     1
    #endif

    int result = 0;
    GuiState state = guiState;

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, bounds.width, RAYGUI_GROUPBOX_LINE_THICK }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - 1, bounds.y, RAYGUI_GROUPBOX_LINE_THICK, bounds.height }, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR)));

    GuiLine(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y - GuiGetStyle(DEFAULT, TEXT_SIZE)/2, bounds.width, (float)GuiGetStyle(DEFAULT, TEXT_SIZE) }, text);
    //--------------------------------------------------------------------

    return result;
}

// Line control
int GuiLine(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_LINE_ORIGIN_SIZE)
        #define RAYGUI_LINE_MARGIN_TEXT  12
    #endif
    #if !defined(RAYGUI_LINE_TEXT_PADDING)
        #define RAYGUI_LINE_TEXT_PADDING  4
    #endif

    int result = 0;
    GuiState state = guiState;

    Color color = GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED : LINE_COLOR));

    // Draw control
    //--------------------------------------------------------------------
    if (text == NULL) GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height/2, bounds.width, 1 }, 0, BLANK, color);
    else
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = bounds.height;
        textBounds.x = bounds.x + RAYGUI_LINE_MARGIN_TEXT;
        textBounds.y = bounds.y;

        // Draw line with embedded text label: "--- text --------------"
        GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height/2, RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, BLANK, color);
        GuiDrawText(text, textBounds, TEXT_ALIGN_LEFT, color);
        GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + 12 + textBounds.width + 4, bounds.y + bounds.height/2, bounds.width - textBounds.width - RAYGUI_LINE_MARGIN_TEXT - RAYGUI_LINE_TEXT_PADDING, 1 }, 0, BLANK, color);
    }
    //--------------------------------------------------------------------

    return result;
}

// Panel control
int GuiPanel(Rectangle bounds, const char *text)
{
    #if !defined(RAYGUI_PANEL_BORDER_WIDTH)
        #define RAYGUI_PANEL_BORDER_WIDTH   1
    #endif

    int result = 0;
    GuiState state = guiState;

    // Text will be drawn as a header bar (if provided)
    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
    if ((text != NULL) && (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f)) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

    if (text != NULL)
    {
        // Move panel bounds after the header bar
        bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
        bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
    }

    // Draw control
    //--------------------------------------------------------------------
    if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

    GuiDrawRectangle(bounds, RAYGUI_PANEL_BORDER_WIDTH, GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BORDER_COLOR_DISABLED: LINE_COLOR)),
                     GetColor(GuiGetStyle(DEFAULT, (state == STATE_DISABLED)? BASE_COLOR_DISABLED : BACKGROUND_COLOR)));
    //--------------------------------------------------------------------

    return result;
}

// Tab Bar control
// NOTE: Using GuiToggle() for the TABS
int GuiTabBar(Rectangle bounds, const char **text, int count, int *active)
{
    #define RAYGUI_TABBAR_ITEM_WIDTH    160

    int result = -1;
    //GuiState state = guiState;

    Rectangle tabBounds = { bounds.x, bounds.y, RAYGUI_TABBAR_ITEM_WIDTH, bounds.height };

    if (*active < 0) *active = 0;
    else if (*active > count - 1) *active = count - 1;

    int offsetX = 0;    // Required in case tabs go out of screen
    offsetX = (*active + 2)*RAYGUI_TABBAR_ITEM_WIDTH - GetScreenWidth();
    if (offsetX < 0) offsetX = 0;

    bool toggle = false;    // Required for individual toggles

    // Draw control
    //--------------------------------------------------------------------
    for (int i = 0; i < count; i++)
    {
        tabBounds.x = bounds.x + (RAYGUI_TABBAR_ITEM_WIDTH + 4)*i - offsetX;

        if (tabBounds.x < GetScreenWidth())
        {
            // Draw tabs as toggle controls
            int textAlignment = GuiGetStyle(TOGGLE, TEXT_ALIGNMENT);
            int textPadding = GuiGetStyle(TOGGLE, TEXT_PADDING);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(TOGGLE, TEXT_PADDING, 8);

            if (i == (*active))
            {
                toggle = true;
                GuiToggle(tabBounds, GuiIconText(12, text[i]), &toggle);
            }
            else
            {
                toggle = false;
                GuiToggle(tabBounds, GuiIconText(12, text[i]), &toggle);
                if (toggle) *active = i;
            }

            GuiSetStyle(TOGGLE, TEXT_PADDING, textPadding);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, textAlignment);

            // Draw tab close button
            // NOTE: Only draw close button for current tab: if (CheckCollisionPointRec(mousePosition, tabBounds))
            int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
            int tempTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
            GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
#if defined(RAYGUI_NO_ICONS)
            if (GuiButton(RAYGUI_CLITERAL(Rectangle){ tabBounds.x + tabBounds.width - 14 - 5, tabBounds.y + 5, 14, 14 }, "x")) result = i;
#else
            if (GuiButton(RAYGUI_CLITERAL(Rectangle){ tabBounds.x + tabBounds.width - 14 - 5, tabBounds.y + 5, 14, 14 }, GuiIconText(ICON_CROSS_SMALL, NULL))) result = i;
#endif
            GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlignment);
        }
    }

    // Draw tab-bar bottom line
    GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, bounds.width, 1 }, 0, BLANK, GetColor(GuiGetStyle(TOGGLE, BORDER_COLOR_NORMAL)));
    //--------------------------------------------------------------------

    return result;     // Return as result the current TAB closing requested
}

// Scroll Panel control
int GuiScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view)
{
    #define RAYGUI_MIN_SCROLLBAR_WIDTH     40
    #define RAYGUI_MIN_SCROLLBAR_HEIGHT    40

    int result = 0;
    GuiState state = guiState;
    float mouseWheelSpeed = 20.0f;      // Default movement speed with mouse wheel

    Rectangle temp = { 0 };
    if (view == NULL) view = &temp;

    Vector2 scrollPos = { 0.0f, 0.0f };
    if (scroll != NULL) scrollPos = *scroll;

    // Text will be drawn as a header bar (if provided)
    Rectangle statusBar = { bounds.x, bounds.y, bounds.width, (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
    if (bounds.height < RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f) bounds.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT*2.0f;

    if (text != NULL)
    {
        // Move panel bounds after the header bar
        bounds.y += (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 1;
        bounds.height -= (float)RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 1;
    }

    bool hasHorizontalScrollBar = (content.width > bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH))? true : false;
    bool hasVerticalScrollBar = (content.height > bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH))? true : false;

    // Recheck to account for the other scrollbar being visible
    if (!hasHorizontalScrollBar) hasHorizontalScrollBar = (hasVerticalScrollBar && (content.width > (bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH))))? true : false;
    if (!hasVerticalScrollBar) hasVerticalScrollBar = (hasHorizontalScrollBar && (content.height > (bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH))))? true : false;

    int horizontalScrollBarWidth = hasHorizontalScrollBar? GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH) : 0;
    int verticalScrollBarWidth =  hasVerticalScrollBar? GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH) : 0;
    Rectangle horizontalScrollBar = {
        (float)((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + verticalScrollBarWidth : (float)bounds.x) + GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)bounds.y + bounds.height - horizontalScrollBarWidth - GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)bounds.width - verticalScrollBarWidth - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)horizontalScrollBarWidth
    };
    Rectangle verticalScrollBar = {
        (float)((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH) : (float)bounds.x + bounds.width - verticalScrollBarWidth - GuiGetStyle(DEFAULT, BORDER_WIDTH)),
        (float)bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH),
        (float)verticalScrollBarWidth,
        (float)bounds.height - horizontalScrollBarWidth - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH)
    };

    // Make sure scroll bars have a minimum width/height
    // NOTE: If content >>> bounds, size could be very small or even 0
    if (horizontalScrollBar.width < RAYGUI_MIN_SCROLLBAR_WIDTH)
    {
        horizontalScrollBar.width = RAYGUI_MIN_SCROLLBAR_WIDTH;
        mouseWheelSpeed = 30.0f;    // TODO: Calculate speed increment based on content.height vs bounds.height
    }
    if (verticalScrollBar.height < RAYGUI_MIN_SCROLLBAR_HEIGHT)
    {
        verticalScrollBar.height = RAYGUI_MIN_SCROLLBAR_HEIGHT;
        mouseWheelSpeed = 30.0f;    // TODO: Calculate speed increment based on content.width vs bounds.width
    }

    // Calculate view area (area without the scrollbars)
    *view = (GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)?
                RAYGUI_CLITERAL(Rectangle){ bounds.x + verticalScrollBarWidth + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth } :
                RAYGUI_CLITERAL(Rectangle){ bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.y + GuiGetStyle(DEFAULT, BORDER_WIDTH), bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth, bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth };

    // Clip view area to the actual content size
    if (view->width > content.width) view->width = content.width;
    if (view->height > content.height) view->height = content.height;

    float horizontalMin = hasHorizontalScrollBar? ((GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(DEFAULT, BORDER_WIDTH) : (((float)GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)-verticalScrollBarWidth : 0) - (float)GuiGetStyle(DEFAULT, BORDER_WIDTH);
    float horizontalMax = hasHorizontalScrollBar? content.width - bounds.width + (float)verticalScrollBarWidth + GuiGetStyle(DEFAULT, BORDER_WIDTH) - (((float)GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (float)verticalScrollBarWidth : 0) : (float)-GuiGetStyle(DEFAULT, BORDER_WIDTH);
    float verticalMin = hasVerticalScrollBar? 0.0f : -1.0f;
    float verticalMax = hasVerticalScrollBar? content.height - bounds.height + (float)horizontalScrollBarWidth + (float)GuiGetStyle(DEFAULT, BORDER_WIDTH) : (float)-GuiGetStyle(DEFAULT, BORDER_WIDTH);

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

#if defined(SUPPORT_SCROLLBAR_KEY_INPUT)
            if (hasHorizontalScrollBar)
            {
                if (IsKeyDown(KEY_RIGHT)) scrollPos.x -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                if (IsKeyDown(KEY_LEFT)) scrollPos.x += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
            }

            if (hasVerticalScrollBar)
            {
                if (IsKeyDown(KEY_DOWN)) scrollPos.y -= GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                if (IsKeyDown(KEY_UP)) scrollPos.y += GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
            }
#endif
            float wheelMove = GetMouseWheelMove();

            // Horizontal and vertical scrolling with mouse wheel
            if (hasHorizontalScrollBar && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT))) scrollPos.x += wheelMove*mouseWheelSpeed;
            else scrollPos.y += wheelMove*mouseWheelSpeed; // Vertical scroll
        }
    }

    // Normalize scroll values
    if (scrollPos.x > -horizontalMin) scrollPos.x = -horizontalMin;
    if (scrollPos.x < -horizontalMax) scrollPos.x = -horizontalMax;
    if (scrollPos.y > -verticalMin) scrollPos.y = -verticalMin;
    if (scrollPos.y < -verticalMax) scrollPos.y = -verticalMax;
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (text != NULL) GuiStatusBar(statusBar, text);  // Draw panel header as status bar

    GuiDrawRectangle(bounds, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));        // Draw background

    // Save size of the scrollbar slider
    const int slider = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);

    // Draw horizontal scrollbar if visible
    if (hasHorizontalScrollBar)
    {
        // Change scrollbar slider size to show the diff in size between the content width and the widget width
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)/(int)content.width)*((int)bounds.width - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - verticalScrollBarWidth)));
        scrollPos.x = (float)-GuiScrollBar(horizontalScrollBar, (int)-scrollPos.x, (int)horizontalMin, (int)horizontalMax);
    }
    else scrollPos.x = 0.0f;

    // Draw vertical scrollbar if visible
    if (hasVerticalScrollBar)
    {
        // Change scrollbar slider size to show the diff in size between the content height and the widget height
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)(((bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth)/(int)content.height)*((int)bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH) - horizontalScrollBarWidth)));
        scrollPos.y = (float)-GuiScrollBar(verticalScrollBar, (int)-scrollPos.y, (int)verticalMin, (int)verticalMax);
    }
    else scrollPos.y = 0.0f;

    // Draw detail corner rectangle if both scroll bars are visible
    if (hasHorizontalScrollBar && hasVerticalScrollBar)
    {
        Rectangle corner = { (GuiGetStyle(LISTVIEW, SCROLLBAR_SIDE) == SCROLLBAR_LEFT_SIDE)? (bounds.x + GuiGetStyle(DEFAULT, BORDER_WIDTH) + 2) : (horizontalScrollBar.x + horizontalScrollBar.width + 2), verticalScrollBar.y + verticalScrollBar.height + 2, (float)horizontalScrollBarWidth - 4, (float)verticalScrollBarWidth - 4 };
        GuiDrawRectangle(corner, 0, BLANK, GetColor(GuiGetStyle(LISTVIEW, TEXT + (state*3))));
    }

    // Draw scrollbar lines depending on current state
    GuiDrawRectangle(bounds, GuiGetStyle(DEFAULT, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + (state*3))), BLANK);

    // Set scrollbar slider size back to the way it was before
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, slider);
    //--------------------------------------------------------------------

    if (scroll != NULL) *scroll = scrollPos;

    return result;
}

// Label control
int GuiLabel(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    //...
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Button control, returns true when clicked
int GuiButton(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), GetColor(GuiGetStyle(BUTTON, BASE + (state*3))));
    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))));

    if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //------------------------------------------------------------------

    return result;      // Button pressed: result = 1
}

// Label button control
int GuiLabelButton(Rectangle bounds, const char *text)
{
    GuiState state = guiState;
    bool pressed = false;

    // NOTE: We force bounds.width to be all text
    float textWidth = (float)GetTextWidth(text);
    if ((bounds.width - 2*GuiGetStyle(LABEL, BORDER_WIDTH) - 2*GuiGetStyle(LABEL, TEXT_PADDING)) < textWidth) bounds.width = textWidth + 2*GuiGetStyle(LABEL, BORDER_WIDTH) + 2*GuiGetStyle(LABEL, TEXT_PADDING) + 2;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check checkbox state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pressed = true;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawText(text, GetTextBounds(LABEL, bounds), GuiGetStyle(LABEL, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return pressed;
}

// Toggle Button control, returns true when active
int GuiToggle(Rectangle bounds, const char *text, bool *active)
{
    int result = 0;
    GuiState state = guiState;

    bool temp = false;
    if (active == NULL) active = &temp;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check toggle button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state = STATE_NORMAL;
                *active = !(*active);
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_NORMAL)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, ((*active)? BORDER_COLOR_PRESSED : (BORDER + state*3)))), GetColor(GuiGetStyle(TOGGLE, ((*active)? BASE_COLOR_PRESSED : (BASE + state*3)))));
        GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TOGGLE, ((*active)? TEXT_COLOR_PRESSED : (TEXT + state*3)))));
    }
    else
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TOGGLE, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, BORDER + state*3)), GetColor(GuiGetStyle(TOGGLE, BASE + state*3)));
        GuiDrawText(text, GetTextBounds(TOGGLE, bounds), GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TOGGLE, TEXT + state*3)));
    }

    if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //--------------------------------------------------------------------

    return result;
}

// Toggle Group control, returns toggled button codepointIndex
int GuiToggleGroup(Rectangle bounds, const char *text, int *active)
{
    #if !defined(RAYGUI_TOGGLEGROUP_MAX_ITEMS)
        #define RAYGUI_TOGGLEGROUP_MAX_ITEMS    32
    #endif

    int result = 0;
    float initBoundsX = bounds.x;

    int temp = 0;
    if (active == NULL) active = &temp;

    bool toggle = false;    // Required for individual toggles

    // Get substrings items from text (items pointers)
    int rows[RAYGUI_TOGGLEGROUP_MAX_ITEMS] = { 0 };
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, rows);

    int prevRow = rows[0];

    for (int i = 0; i < itemCount; i++)
    {
        if (prevRow != rows[i])
        {
            bounds.x = initBoundsX;
            bounds.y += (bounds.height + GuiGetStyle(TOGGLE, GROUP_PADDING));
            prevRow = rows[i];
        }

        if (i == (*active))
        {
            toggle = true;
            GuiToggle(bounds, items[i], &toggle);
        }
        else
        {
            toggle = false;
            GuiToggle(bounds, items[i], &toggle);
            if (toggle) *active = i;
        }

        bounds.x += (bounds.width + GuiGetStyle(TOGGLE, GROUP_PADDING));
    }

    return result;
}

// Toggle Slider control extended, returns true when clicked
int GuiToggleSlider(Rectangle bounds, const char *text, int *active)
{
    int result = 0;
    GuiState state = guiState;

    int temp = 0;
    if (active == NULL) active = &temp;

    //bool toggle = false;    // Required for individual toggles

    // Get substrings items from text (items pointers)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    Rectangle slider = {
        0,      // Calculated later depending on the active toggle
        bounds.y + GuiGetStyle(SLIDER, BORDER_WIDTH) + GuiGetStyle(SLIDER, SLIDER_PADDING),
        (bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - (itemCount + 1)*GuiGetStyle(SLIDER, SLIDER_PADDING))/itemCount,
        bounds.height - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - 2*GuiGetStyle(SLIDER, SLIDER_PADDING) };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                (*active)++;
                result = 1;
            }
            else state = STATE_FOCUSED;
        }

        if ((*active) && (state != STATE_FOCUSED)) state = STATE_PRESSED;
    }

    if (*active >= itemCount) *active = 0;
    slider.x = bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH) + (*active + 1)*GuiGetStyle(SLIDER, SLIDER_PADDING) + (*active)*slider.width;
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SLIDER, BORDER_WIDTH), GetColor(GuiGetStyle(TOGGLE, BORDER + (state*3))),
        GetColor(GuiGetStyle(TOGGLE, BASE_COLOR_NORMAL)));

    // Draw internal slider
    if (state == STATE_NORMAL) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));
    else if (state == STATE_FOCUSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_FOCUSED)));
    else if (state == STATE_PRESSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));

    // Draw text in slider
    if (text != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(text);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = slider.x + slider.width/2 - textBounds.width/2;
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(items[*active], textBounds, GuiGetStyle(TOGGLE, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(TOGGLE, TEXT + (state*3))), guiAlpha));
    }
    //--------------------------------------------------------------------

    return result;
}

// Check Box control, returns 1 when state changed
int GuiCheckBox(Rectangle bounds, const char *text, bool *checked)
{
    int result = 0;
    GuiState state = guiState;

    bool temp = false;
    if (checked == NULL) checked = &temp;

    Rectangle textBounds = { 0 };

    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(CHECKBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        Rectangle totalBounds = {
            (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT)? textBounds.x : bounds.x,
            bounds.y,
            bounds.width + textBounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING),
            bounds.height,
        };

        // Check checkbox state
        if (CheckCollisionPointRec(mousePoint, totalBounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                *checked = !(*checked);
                result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(CHECKBOX, BORDER_WIDTH), GetColor(GuiGetStyle(CHECKBOX, BORDER + (state*3))), BLANK);

    if (*checked)
    {
        Rectangle check = { bounds.x + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.y + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.width - 2*(GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)),
                            bounds.height - 2*(GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)) };
        GuiDrawRectangle(check, 0, BLANK, GetColor(GuiGetStyle(CHECKBOX, TEXT + state*3)));
    }

    GuiDrawText(text, textBounds, (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Combo Box control, returns selected item codepointIndex
int GuiComboBox(Rectangle bounds, const char *text, int *active)
{
    int result = 0;
    GuiState state = guiState;

    int temp = 0;
    if (active == NULL) active = &temp;

    bounds.width -= (GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH) + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING));

    Rectangle selector = { (float)bounds.x + bounds.width + GuiGetStyle(COMBOBOX, COMBO_BUTTON_SPACING),
                           (float)bounds.y, (float)GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH), (float)bounds.height };

    // Get substrings items from text (items pointers, lengths and count)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    if (*active < 0) *active = 0;
    else if (*active > (itemCount - 1)) *active = itemCount - 1;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && (itemCount > 1) && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds) ||
            CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                *active += 1;
                if (*active >= itemCount) *active = 0;      // Cyclic combobox
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // Draw combo box main
    GuiDrawRectangle(bounds, GuiGetStyle(COMBOBOX, BORDER_WIDTH), GetColor(GuiGetStyle(COMBOBOX, BORDER + (state*3))), GetColor(GuiGetStyle(COMBOBOX, BASE + (state*3))));
    GuiDrawText(items[*active], GetTextBounds(COMBOBOX, bounds), GuiGetStyle(COMBOBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(COMBOBOX, TEXT + (state*3))));

    // Draw selector using a custom button
    // NOTE: BORDER_WIDTH and TEXT_ALIGNMENT forced values
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiButton(selector, TextFormat("%i/%i", *active + 1, itemCount));

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlign);
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);
    //--------------------------------------------------------------------

    return result;
}

// Dropdown Box control
// NOTE: Returns mouse click
int GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode)
{
    int result = 0;
    GuiState state = guiState;

    int itemSelected = *active;
    int itemFocused = -1;

    // Get substrings items from text (items pointers, lengths and count)
    int itemCount = 0;
    const char **items = GuiTextSplit(text, ';', &itemCount, NULL);

    Rectangle boundsOpen = bounds;
    boundsOpen.height = (itemCount + 1)*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

    Rectangle itemBounds = bounds;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && (editMode || !guiLocked) && (itemCount > 1) && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        if (editMode)
        {
            state = STATE_PRESSED;

            // Check if mouse has been pressed or released outside limits
            if (!CheckCollisionPointRec(mousePoint, boundsOpen))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
            }

            // Check if already selected item has been pressed again
            if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;

            // Check focused and selected item
            for (int i = 0; i < itemCount; i++)
            {
                // Update item rectangle y position for next item
                itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

                if (CheckCollisionPointRec(mousePoint, itemBounds))
                {
                    itemFocused = i;
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                    {
                        itemSelected = i;
                        result = 1;         // Item selected
                    }
                    break;
                }
            }

            itemBounds = bounds;
        }
        else
        {
            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    result = 1;
                    state = STATE_PRESSED;
                }
                else state = STATE_FOCUSED;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (editMode) GuiPanel(boundsOpen, NULL);

    GuiDrawRectangle(bounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state*3)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state*3)));
    GuiDrawText(items[itemSelected], GetTextBounds(DROPDOWNBOX, bounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + state*3)));

    if (editMode)
    {
        // Draw visible items
        for (int i = 0; i < itemCount; i++)
        {
            // Update item rectangle y position for next item
            itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING));

            if (i == itemSelected)
            {
                GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED)));
                GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED)));
            }
            else if (i == itemFocused)
            {
                GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)));
                GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)));
            }
            else GuiDrawText(items[i], GetTextBounds(DROPDOWNBOX, itemBounds), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)));
        }
    }

    // Draw arrows (using icon if available)
#if defined(RAYGUI_NO_ICONS)
    GuiDrawText("v", RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 2, 10, 10 },
                TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
#else
    GuiDrawText("#120#", RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 6, 10, 10 },
                TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));   // ICON_ARROW_DOWN_FILL
#endif
    //--------------------------------------------------------------------

    *active = itemSelected;

    // TODO: Use result to return more internal states: mouse-press out-of-bounds, mouse-press over selected-item...
    return result;   // Mouse click: result = 1
}

// Text Box control
// NOTE: Returns true on ENTER pressed (useful for data validation)
int GuiTextBox(Rectangle bounds, char *text, int bufferSize, bool editMode)
{
    #if !defined(RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)
        #define RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN  40        // Frames to wait for autocursor movement
    #endif
    #if !defined(RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY)
        #define RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY      1        // Frames delay for autocursor movement
    #endif

    int result = 0;
    GuiState state = guiState;

    bool multiline = false;     // TODO: Consider multiline text input
    int wrapMode = GuiGetStyle(DEFAULT, TEXT_WRAP_MODE);

    Rectangle textBounds = GetTextBounds(TEXTBOX, bounds);
    int textWidth = GetTextWidth(text) - GetTextWidth(text + textBoxCursorIndex);
    int textIndexOffset = 0;    // Text index offset to start drawing in the box

    // Cursor rectangle
    // NOTE: Position X value should be updated
    Rectangle cursor = {
        textBounds.x + textWidth + GuiGetStyle(DEFAULT, TEXT_SPACING),
        textBounds.y + textBounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
        2,
        (float)GuiGetStyle(DEFAULT, TEXT_SIZE)*2
    };

    if (cursor.height >= bounds.height) cursor.height = bounds.height - GuiGetStyle(TEXTBOX, BORDER_WIDTH)*2;
    if (cursor.y < (bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH))) cursor.y = bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH);

    // Mouse cursor rectangle
    // NOTE: Initialized outside of screen
    Rectangle mouseCursor = cursor;
    mouseCursor.x = -1;
    mouseCursor.width = 1;

    // Auto-cursor movement logic
    // NOTE: Cursor moves automatically when key down after some time
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_BACKSPACE) || IsKeyDown(KEY_DELETE)) autoCursorCooldownCounter++;
    else
    {
        autoCursorCooldownCounter = 0;      // GLOBAL: Cursor cooldown counter
        autoCursorDelayCounter = 0;         // GLOBAL: Cursor delay counter
    }

    // Blink-cursor frame counter
    //if (!autoCursorMode) blinkCursorFrameCounter++;
    //else blinkCursorFrameCounter = 0;

    // Update control
    //--------------------------------------------------------------------
    // WARNING: Text editing is only supported under certain conditions:
    if ((state != STATE_DISABLED) &&                // Control not disabled
        !GuiGetStyle(TEXTBOX, TEXT_READONLY) &&     // TextBox not on read-only mode
        !guiLocked &&                               // Gui not locked
        !guiSliderDragging &&                       // No gui slider on dragging
        (wrapMode == TEXT_WRAP_NONE))               // No wrap mode
    {
        Vector2 mousePosition = GetMousePosition();

        if (editMode)
        {
            state = STATE_PRESSED;

            // If text does not fit in the textbox and current cursor position is out of bounds,
            // we add an index offset to text for drawing only what requires depending on cursor
            while (textWidth >= textBounds.width)
            {
                int nextCodepointSize = 0;
                GetCodepointNext(text + textIndexOffset, &nextCodepointSize);

                textIndexOffset += nextCodepointSize;

                textWidth = GetTextWidth(text + textIndexOffset) - GetTextWidth(text + textBoxCursorIndex);
            }

            int textLength = (int)strlen(text);     // Get current text length
            int codepoint = GetCharPressed();       // Get Unicode codepoint
            if (multiline && IsKeyPressed(KEY_ENTER)) codepoint = (int)'\n';

            if (textBoxCursorIndex > textLength) textBoxCursorIndex = textLength;

            // Encode codepoint as UTF-8
            int codepointSize = 0;
            const char *charEncoded = CodepointToUTF8(codepoint, &codepointSize);

            // Add codepoint to text, at current cursor position
            // NOTE: Make sure we do not overflow buffer size
            if (((multiline && (codepoint == (int)'\n')) || (codepoint >= 32)) && ((textLength + codepointSize) < bufferSize))
            {
                // Move forward data from cursor position
                for (int i = (textLength + codepointSize); i > textBoxCursorIndex; i--) text[i] = text[i - codepointSize];

                // Add new codepoint in current cursor position
                for (int i = 0; i < codepointSize; i++) text[textBoxCursorIndex + i] = charEncoded[i];

                textBoxCursorIndex += codepointSize;
                textLength += codepointSize;

                // Make sure text last character is EOL
                text[textLength] = '\0';
            }

            // Move cursor to start
            if ((textLength > 0) && IsKeyPressed(KEY_HOME)) textBoxCursorIndex = 0;

            // Move cursor to end
            if ((textLength > textBoxCursorIndex) && IsKeyPressed(KEY_END)) textBoxCursorIndex = textLength;

            // Delete codepoint from text, after current cursor position
            if ((textLength > textBoxCursorIndex) && (IsKeyPressed(KEY_DELETE) || (IsKeyDown(KEY_DELETE) && (autoCursorCooldownCounter >= RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN))))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_DELETE) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int nextCodepointSize = 0;
                    GetCodepointNext(text + textBoxCursorIndex, &nextCodepointSize);

                    // Move backward text from cursor position
                    for (int i = textBoxCursorIndex; i < textLength; i++) text[i] = text[i + nextCodepointSize];

                    textLength -= codepointSize;

                    // Make sure text last character is EOL
                    text[textLength] = '\0';
                }
            }

            // Delete codepoint from text, before current cursor position
            if ((textLength > 0) && (IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && (autoCursorCooldownCounter >= RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN))))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_BACKSPACE) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int prevCodepointSize = 0;
                    GetCodepointPrevious(text + textBoxCursorIndex, &prevCodepointSize);

                    // Move backward text from cursor position
                    for (int i = (textBoxCursorIndex - prevCodepointSize); i < textLength; i++) text[i] = text[i + prevCodepointSize];

                    // Prevent cursor index from decrementing past 0
                    if (textBoxCursorIndex > 0)
                    {
                        textBoxCursorIndex -= codepointSize;
                        textLength -= codepointSize;
                    }

                    // Make sure text last character is EOL
                    text[textLength] = '\0';
                }
            }

            // Move cursor position with keys
            if (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && (autoCursorCooldownCounter > RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_LEFT) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int prevCodepointSize = 0;
                    GetCodepointPrevious(text + textBoxCursorIndex, &prevCodepointSize);

                    if (textBoxCursorIndex >= prevCodepointSize) textBoxCursorIndex -= prevCodepointSize;
                }
            }
            else if (IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && (autoCursorCooldownCounter > RAYGUI_TEXTBOX_AUTO_CURSOR_COOLDOWN)))
            {
                autoCursorDelayCounter++;

                if (IsKeyPressed(KEY_RIGHT) || (autoCursorDelayCounter%RAYGUI_TEXTBOX_AUTO_CURSOR_DELAY) == 0)      // Delay every movement some frames
                {
                    int nextCodepointSize = 0;
                    GetCodepointNext(text + textBoxCursorIndex, &nextCodepointSize);

                    if ((textBoxCursorIndex + nextCodepointSize) <= textLength) textBoxCursorIndex += nextCodepointSize;
                }
            }

            // Move cursor position with mouse
            if (CheckCollisionPointRec(mousePosition, textBounds))     // Mouse hover text
            {
                float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/(float)guiFont.baseSize;
                int codepoint = 0;
                int codepointSize = 0;
                int codepointIndex = 0;
                float glyphWidth = 0.0f;
                float widthToMouseX = 0;
                int mouseCursorIndex = 0;

                for (int i = textIndexOffset; i < textLength; i++)
                {
                    codepoint = GetCodepointNext(&text[i], &codepointSize);
                    codepointIndex = GetGlyphIndex(guiFont, codepoint);

                    if (guiFont.glyphs[codepointIndex].advanceX == 0) glyphWidth = ((float)guiFont.recs[codepointIndex].width*scaleFactor);
                    else glyphWidth = ((float)guiFont.glyphs[codepointIndex].advanceX*scaleFactor);

                    if (mousePosition.x <= (textBounds.x + (widthToMouseX + glyphWidth/2)))
                    {
                        mouseCursor.x = textBounds.x + widthToMouseX;
                        mouseCursorIndex = i;
                        break;
                    }

                    widthToMouseX += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
                }

                // Check if mouse cursor is at the last position
                int textEndWidth = GetTextWidth(text + textIndexOffset);
                if (GetMousePosition().x >= (textBounds.x + textEndWidth - glyphWidth/2))
                {
                    mouseCursor.x = textBounds.x + textEndWidth;
                    mouseCursorIndex = strlen(text);
                }

                // Place cursor at required index on mouse click
                if ((mouseCursor.x >= 0) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    cursor.x = mouseCursor.x;
                    textBoxCursorIndex = mouseCursorIndex;
                }
            }
            else mouseCursor.x = -1;

            // Recalculate cursor position.y depending on textBoxCursorIndex
            cursor.x = bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(text + textIndexOffset) - GetTextWidth(text + textBoxCursorIndex) + GuiGetStyle(DEFAULT, TEXT_SPACING);
            //if (multiline) cursor.y = GetTextLines()

            // Finish text editing on ENTER or mouse click outside bounds
            if ((!multiline && IsKeyPressed(KEY_ENTER)) ||
                (!CheckCollisionPointRec(mousePosition, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                textBoxCursorIndex = 0;     // GLOBAL: Reset the shared cursor index
                result = 1;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePosition, bounds))
            {
                state = STATE_FOCUSED;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    textBoxCursorIndex = (int)strlen(text);   // GLOBAL: Place cursor index to the end of current text
                    result = 1;
                }
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_PRESSED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)));
    }
    else if (state == STATE_DISABLED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)));
    }
    else GuiDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), BLANK);

    // Draw text considering index offset if required
    // NOTE: Text index offset depends on cursor position
    GuiDrawText(text + textIndexOffset, textBounds, GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TEXTBOX, TEXT + (state*3))));

    // Draw cursor
    if (editMode && !GuiGetStyle(TEXTBOX, TEXT_READONLY))
    {
        //if (autoCursorMode || ((blinkCursorFrameCounter/40)%2 == 0))
        GuiDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));

        // Draw mouse position cursor (if required)
        if (mouseCursor.x >= 0) GuiDrawRectangle(mouseCursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));
    }
    else if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //--------------------------------------------------------------------

    return result;      // Mouse button pressed: result = 1
}

/*
// Text Box control with multiple lines and word-wrap
// NOTE: This text-box is readonly, no editing supported by default
bool GuiTextBoxMulti(Rectangle bounds, char *text, int bufferSize, bool editMode)
{
    bool pressed = false;

    GuiSetStyle(TEXTBOX, TEXT_READONLY, 1);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);   // WARNING: If wrap mode enabled, text editing is not supported
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);

    // TODO: Implement methods to calculate cursor position properly
    pressed = GuiTextBox(bounds, text, bufferSize, editMode);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
    GuiSetStyle(TEXTBOX, TEXT_READONLY, 0);

    return pressed;
}
*/

// Spinner control, returns selected value
int GuiSpinner(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
{
    int result = 1;
    GuiState state = guiState;

    int tempValue = *value;

    Rectangle spinner = { bounds.x + GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING), bounds.y,
                          bounds.width - 2*(GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH) + GuiGetStyle(SPINNER, SPIN_BUTTON_SPACING)), bounds.height };
    Rectangle leftButtonBound = { (float)bounds.x, (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };
    Rectangle rightButtonBound = { (float)bounds.x + bounds.width - GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.y, (float)GuiGetStyle(SPINNER, SPIN_BUTTON_WIDTH), (float)bounds.height };

    Rectangle textBounds = { 0 };
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(SPINNER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SPINNER, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check spinner state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }

#if defined(RAYGUI_NO_ICONS)
    if (GuiButton(leftButtonBound, "<")) tempValue--;
    if (GuiButton(rightButtonBound, ">")) tempValue++;
#else
    if (GuiButton(leftButtonBound, GuiIconText(ICON_ARROW_LEFT_FILL, NULL))) tempValue--;
    if (GuiButton(rightButtonBound, GuiIconText(ICON_ARROW_RIGHT_FILL, NULL))) tempValue++;
#endif

    if (!editMode)
    {
        if (tempValue < minValue) tempValue = minValue;
        if (tempValue > maxValue) tempValue = maxValue;
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    result = GuiValueBox(spinner, NULL, &tempValue, minValue, maxValue, editMode);

    // Draw value selector custom buttons
    // NOTE: BORDER_WIDTH and TEXT_ALIGNMENT forced values
    int tempBorderWidth = GuiGetStyle(BUTTON, BORDER_WIDTH);
    int tempTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, GuiGetStyle(SPINNER, BORDER_WIDTH));
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, tempTextAlign);
    GuiSetStyle(BUTTON, BORDER_WIDTH, tempBorderWidth);

    // Draw text label if provided
    GuiDrawText(text, textBounds, (GuiGetStyle(SPINNER, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    *value = tempValue;
    return result;
}

// Value Box control, updates input text with numbers
// NOTE: Requires static variables: frameCounter
int GuiValueBox(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode)
{
    #if !defined(RAYGUI_VALUEBOX_MAX_CHARS)
        #define RAYGUI_VALUEBOX_MAX_CHARS  32
    #endif

    int result = 0;
    GuiState state = guiState;

    char textValue[RAYGUI_VALUEBOX_MAX_CHARS + 1] = "\0";
    sprintf(textValue, "%i", *value);

    Rectangle textBounds = { 0 };
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(VALUEBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        if (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(VALUEBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        bool valueHasChanged = false;

        if (editMode)
        {
            state = STATE_PRESSED;

            int keyCount = (int)strlen(textValue);

            // Only allow keys in range [48..57]
            if (keyCount < RAYGUI_VALUEBOX_MAX_CHARS)
            {
                if (GetTextWidth(textValue) < bounds.width)
                {
                    int key = GetCharPressed();
                    if ((key >= 48) && (key <= 57))
                    {
                        textValue[keyCount] = (char)key;
                        keyCount++;
                        valueHasChanged = true;
                    }
                }
            }

            // Delete text
            if (keyCount > 0)
            {
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    keyCount--;
                    textValue[keyCount] = '\0';
                    valueHasChanged = true;
                }
            }

            if (valueHasChanged) *value = TextToInteger(textValue);

            // NOTE: We are not clamp values until user input finishes
            //if (*value > maxValue) *value = maxValue;
            //else if (*value < minValue) *value = minValue;

            if (IsKeyPressed(KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) result = 1;
        }
        else
        {
            if (*value > maxValue) *value = maxValue;
            else if (*value < minValue) *value = minValue;

            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                state = STATE_FOCUSED;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    Color baseColor = BLANK;
    if (state == STATE_PRESSED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED));
    else if (state == STATE_DISABLED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_DISABLED));

    GuiDrawRectangle(bounds, GuiGetStyle(VALUEBOX, BORDER_WIDTH), GetColor(GuiGetStyle(VALUEBOX, BORDER + (state*3))), baseColor);
    GuiDrawText(textValue, GetTextBounds(VALUEBOX, bounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(VALUEBOX, TEXT + (state*3))));

    // Draw cursor
    if (editMode)
    {
        // NOTE: ValueBox internal text is always centered
        Rectangle cursor = { bounds.x + GetTextWidth(textValue)/2 + bounds.width/2 + 1, bounds.y + 2*GuiGetStyle(VALUEBOX, BORDER_WIDTH), 4, bounds.height - 4*GuiGetStyle(VALUEBOX, BORDER_WIDTH) };
        GuiDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED)));
    }

    // Draw text label if provided
    GuiDrawText(text, textBounds, (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT)? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Slider control with pro parameters
// NOTE: Other GuiSlider*() controls use this one
int GuiSliderPro(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue, int sliderWidth)
{
    int result = 0;
    float oldValue = *value;
    GuiState state = guiState;

    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;

    int sliderValue = (int)(((*value - minValue)/(maxValue - minValue))*(bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH)));

    Rectangle slider = { bounds.x, bounds.y + GuiGetStyle(SLIDER, BORDER_WIDTH) + GuiGetStyle(SLIDER, SLIDER_PADDING),
                         0, bounds.height - 2*GuiGetStyle(SLIDER, BORDER_WIDTH) - 2*GuiGetStyle(SLIDER, SLIDER_PADDING) };

    if (sliderWidth > 0)        // Slider
    {
        slider.x += (sliderValue - sliderWidth/2);
        slider.width = (float)sliderWidth;
    }
    else if (sliderWidth == 0)  // SliderBar
    {
        slider.x += GuiGetStyle(SLIDER, BORDER_WIDTH);
        slider.width = (float)sliderValue;
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiSliderDragging) // Keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiSliderActive))
                {
                    state = STATE_PRESSED;

                    // Get equivalent value and slider position from mousePosition.x
                    *value = ((maxValue - minValue)*(mousePoint.x - (float)(bounds.x + sliderWidth/2)))/(float)(bounds.width - sliderWidth) + minValue;
                }
            }
            else
            {
                guiSliderDragging = false;
                guiSliderActive = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiSliderDragging = true;
                guiSliderActive = bounds; // Store bounds as an identifier when dragging starts

                if (!CheckCollisionPointRec(mousePoint, slider))
                {
                    // Get equivalent value and slider position from mousePosition.x
                    *value = ((maxValue - minValue)*(mousePoint.x - (float)(bounds.x + sliderWidth/2)))/(float)(bounds.width - sliderWidth) + minValue;

                    if (sliderWidth > 0) slider.x = mousePoint.x - slider.width/2;      // Slider
                    else if (sliderWidth == 0) slider.width = (float)sliderValue;       // SliderBar
                }
            }
            else state = STATE_FOCUSED;
        }

        if (*value > maxValue) *value = maxValue;
        else if (*value < minValue) *value = minValue;
    }

    // Control value change check
    if(oldValue == *value) result = 0;
    else result = 1;

    // Bar limits check
    if (sliderWidth > 0)        // Slider
    {
        if (slider.x <= (bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH))) slider.x = bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH);
        else if ((slider.x + slider.width) >= (bounds.x + bounds.width)) slider.x = bounds.x + bounds.width - slider.width - GuiGetStyle(SLIDER, BORDER_WIDTH);
    }
    else if (sliderWidth == 0)  // SliderBar
    {
        if (slider.width > bounds.width) slider.width = bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH);
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SLIDER, BORDER_WIDTH), GetColor(GuiGetStyle(SLIDER, BORDER + (state*3))), GetColor(GuiGetStyle(SLIDER, (state != STATE_DISABLED)?  BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)));

    // Draw slider internal bar (depends on state)
    if (state == STATE_NORMAL) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BASE_COLOR_PRESSED)));
    else if (state == STATE_FOCUSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_FOCUSED)));
    else if (state == STATE_PRESSED) GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, TEXT_COLOR_PRESSED)));

    // Draw left/right text if provided
    if (textLeft != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textLeft);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x - textBounds.width - GuiGetStyle(SLIDER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))));
    }

    if (textRight != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textRight);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(SLIDER, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(SLIDER, TEXT + (state*3))));
    }
    //--------------------------------------------------------------------

    return result;
}

// Slider control extended, returns selected value and has text
int GuiSlider(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, GuiGetStyle(SLIDER, SLIDER_WIDTH));
}

// Slider Bar control extended, returns selected value
int GuiSliderBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    return GuiSliderPro(bounds, textLeft, textRight, value, minValue, maxValue, 0);
}

// Progress Bar control extended, shows current progress value
int GuiProgressBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue)
{
    int result = 0;
    GuiState state = guiState;

    float temp = (maxValue - minValue)/2.0f;
    if (value == NULL) value = &temp;

    // Progress bar
    Rectangle progress = { bounds.x + GuiGetStyle(PROGRESSBAR, BORDER_WIDTH),
                           bounds.y + GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) + GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING), 0,
                           bounds.height - 2*GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) - 2*GuiGetStyle(PROGRESSBAR, PROGRESS_PADDING) };

    // Update control
    //--------------------------------------------------------------------
    if (*value > maxValue) *value = maxValue;

    // WARNING: Working with floats could lead to rounding issues
    if ((state != STATE_DISABLED)) progress.width = (float)(*value/(maxValue - minValue))*bounds.width - ((*value >= maxValue)? (float)(2*GuiGetStyle(PROGRESSBAR, BORDER_WIDTH)) : 0.0f);
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_DISABLED)
    {
        GuiDrawRectangle(bounds, GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(PROGRESSBAR, BORDER + (state*3))), BLANK);
    }
    else
    {
        if (*value > minValue)
        {
            // Draw progress bar with colored border, more visual
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, (int)progress.width + (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height - 2 }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y + bounds.height - 1, (int)progress.width + (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
        }
        else GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x, bounds.y, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));

        if (*value >= maxValue) GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + progress.width + 1, bounds.y, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED)));
        else
        {
            // Draw borders not yet reached by value
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + (int)progress.width + 1, bounds.y, bounds.width - (int)progress.width - 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + (int)progress.width + 1, bounds.y + bounds.height - 1, bounds.width - (int)progress.width - 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH) }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
            GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - 1, bounds.y + 1, (float)GuiGetStyle(PROGRESSBAR, BORDER_WIDTH), bounds.height - 2 }, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL)));
        }

        // Draw slider internal progress bar (depends on state)
        GuiDrawRectangle(progress, 0, BLANK, GetColor(GuiGetStyle(PROGRESSBAR, BASE_COLOR_PRESSED)));
    }

    // Draw left/right text if provided
    if (textLeft != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textLeft);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x - textBounds.width - GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textLeft, textBounds, TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))));
    }

    if (textRight != NULL)
    {
        Rectangle textBounds = { 0 };
        textBounds.width = (float)GetTextWidth(textRight);
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(PROGRESSBAR, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE)/2;

        GuiDrawText(textRight, textBounds, TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(PROGRESSBAR, TEXT + (state*3))));
    }
    //--------------------------------------------------------------------

    return result;
}

// Status Bar control
int GuiStatusBar(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER + (state*3))), GetColor(GuiGetStyle(STATUSBAR, BASE + (state*3))));
    GuiDrawText(text, GetTextBounds(STATUSBAR, bounds), GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT), GetColor(GuiGetStyle(STATUSBAR, TEXT + (state*3))));
    //--------------------------------------------------------------------

    return result;
}

// Dummy rectangle control, intended for placeholding
int GuiDummyRec(Rectangle bounds, const char *text)
{
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, 0, BLANK, GetColor(GuiGetStyle(DEFAULT, (state != STATE_DISABLED)? BASE_COLOR_NORMAL : BASE_COLOR_DISABLED)));
    GuiDrawText(text, GetTextBounds(DEFAULT, bounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(BUTTON, (state != STATE_DISABLED)? TEXT_COLOR_NORMAL : TEXT_COLOR_DISABLED)));
    //------------------------------------------------------------------

    return result;
}

// List View control
int GuiListView(Rectangle bounds, const char *text, int *scrollIndex, int *active)
{
    int result = 0;
    int itemCount = 0;
    const char **items = NULL;

    if (text != NULL) items = GuiTextSplit(text, ';', &itemCount, NULL);

    result = GuiListViewEx(bounds, items, itemCount, scrollIndex, active, NULL);

    return result;
}

// List View control with extended parameters
int GuiListViewEx(Rectangle bounds, const char **text, int count, int *scrollIndex, int *active, int *focus)
{
    int result = 0;
    GuiState state = guiState;

    int itemFocused = (focus == NULL)? -1 : *focus;
    int itemSelected = (active == NULL)? -1 : *active;

    // Check if we need a scroll bar
    bool useScrollBar = false;
    if ((GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING))*count > bounds.height) useScrollBar = true;

    // Define base item rectangle [0]
    Rectangle itemBounds = { 0 };
    itemBounds.x = bounds.x + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING);
    itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.width = bounds.width - 2*GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) - GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.height = (float)GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT);
    if (useScrollBar) itemBounds.width -= GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);

    // Get items on the list
    int visibleItems = (int)bounds.height/(GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
    if (visibleItems > count) visibleItems = count;

    int startIndex = (scrollIndex == NULL)? 0 : *scrollIndex;
    if ((startIndex < 0) || (startIndex > (count - visibleItems))) startIndex = 0;
    int endIndex = startIndex + visibleItems;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check mouse inside list view
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            state = STATE_FOCUSED;

            // Check focused and selected item
            for (int i = 0; i < visibleItems; i++)
            {
                if (CheckCollisionPointRec(mousePoint, itemBounds))
                {
                    itemFocused = startIndex + i;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if (itemSelected == (startIndex + i)) itemSelected = -1;
                        else itemSelected = startIndex + i;
                    }
                    break;
                }

                // Update item rectangle y position for next item
                itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
            }

            if (useScrollBar)
            {
                int wheelMove = (int)GetMouseWheelMove();
                startIndex -= wheelMove;

                if (startIndex < 0) startIndex = 0;
                else if (startIndex > (count - visibleItems)) startIndex = count - visibleItems;

                endIndex = startIndex + visibleItems;
                if (endIndex > count) endIndex = count;
            }
        }
        else itemFocused = -1;

        // Reset item rectangle y to [0]
        itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(DEFAULT, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + state*3)), GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));     // Draw background

    // Draw visible items
    for (int i = 0; ((i < visibleItems) && (text != NULL)); i++)
    {
        if (state == STATE_DISABLED)
        {
            if ((startIndex + i) == itemSelected) GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_DISABLED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_DISABLED)));

            GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_DISABLED)));
        }
        else
        {
            if (((startIndex + i) == itemSelected) && (active != NULL))
            {
                // Draw item selected
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_PRESSED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_PRESSED)));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_PRESSED)));
            }
            else if (((startIndex + i) == itemFocused)) // && (focus != NULL))  // NOTE: We want items focused, despite not returned!
            {
                // Draw item focused
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_FOCUSED)), GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_FOCUSED)));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_FOCUSED)));
            }
            else
            {
                // Draw item normal
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_NORMAL)));
            }
        }

        // Update item rectangle y position for next item
        itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING));
    }

    if (useScrollBar)
    {
        Rectangle scrollBarBounds = {
            bounds.x + bounds.width - GuiGetStyle(LISTVIEW, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.y + GuiGetStyle(LISTVIEW, BORDER_WIDTH), (float)GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH)
        };

        // Calculate percentage of visible items and apply same percentage to scrollbar
        float percentVisible = (float)(endIndex - startIndex)/count;
        float sliderSize = bounds.height*percentVisible;

        int prevSliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);   // Save default slider size
        int prevScrollSpeed = GuiGetStyle(SCROLLBAR, SCROLL_SPEED); // Save default scroll speed
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, (int)sliderSize);            // Change slider size
        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, count - visibleItems); // Change scroll speed

        startIndex = GuiScrollBar(scrollBarBounds, startIndex, 0, count - visibleItems);

        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, prevScrollSpeed); // Reset scroll speed to default
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, prevSliderSize); // Reset slider size to default
    }
    //--------------------------------------------------------------------

    if (active != NULL) *active = itemSelected;
    if (focus != NULL) *focus = itemFocused;
    if (scrollIndex != NULL) *scrollIndex = startIndex;

    return result;
}

// Color Panel control
int GuiColorPanel(Rectangle bounds, const char *text, Color *color)
{
    int result = 0;
    GuiState state = guiState;
    Vector2 pickerSelector = { 0 };

    const Color colWhite = { 255, 255, 255, 255 };
    const Color colBlack = { 0, 0, 0, 255 };

    Vector3 vcolor = { (float)color->r/255.0f, (float)color->g/255.0f, (float)color->b/255.0f };
    Vector3 hsv = ConvertRGBtoHSV(vcolor);

    pickerSelector.x = bounds.x + (float)hsv.y*bounds.width;            // HSV: Saturation
    pickerSelector.y = bounds.y + (1.0f - (float)hsv.z)*bounds.height;  // HSV: Value

    float hue = -1.0f;
    Vector3 maxHue = { (hue >= 0.0f)? hue : hsv.x, 1.0f, 1.0f };
    Vector3 rgbHue = ConvertHSVtoRGB(maxHue);
    Color maxHueCol = { (unsigned char)(255.0f*rgbHue.x),
                      (unsigned char)(255.0f*rgbHue.y),
                      (unsigned char)(255.0f*rgbHue.z), 255 };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                pickerSelector = mousePoint;

                // Calculate color from picker
                Vector2 colorPick = { pickerSelector.x - bounds.x, pickerSelector.y - bounds.y };

                colorPick.x /= (float)bounds.width;     // Get normalized value on x
                colorPick.y /= (float)bounds.height;    // Get normalized value on y

                hsv.y = colorPick.x;
                hsv.z = 1.0f - colorPick.y;

                Vector3 rgb = ConvertHSVtoRGB(hsv);

                // NOTE: Vector3ToColor() only available on raylib 1.8.1
                *color = RAYGUI_CLITERAL(Color){ (unsigned char)(255.0f*rgb.x),
                                 (unsigned char)(255.0f*rgb.y),
                                 (unsigned char)(255.0f*rgb.z),
                                 (unsigned char)(255.0f*(float)color->a/255.0f) };

            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        DrawRectangleGradientEx(bounds, Fade(colWhite, guiAlpha), Fade(colWhite, guiAlpha), Fade(maxHueCol, guiAlpha), Fade(maxHueCol, guiAlpha));
        DrawRectangleGradientEx(bounds, Fade(colBlack, 0), Fade(colBlack, guiAlpha), Fade(colBlack, guiAlpha), Fade(colBlack, 0));

        // Draw color picker: selector
        Rectangle selector = { pickerSelector.x - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, pickerSelector.y - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE), (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE) };
        GuiDrawRectangle(selector, 0, BLANK, colWhite);
    }
    else
    {
        DrawRectangleGradientEx(bounds, Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), 0.6f), guiAlpha));
    }

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);
    //--------------------------------------------------------------------

    return result;
}

// Color Bar Alpha control
// NOTE: Returns alpha value normalized [0..1]
int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha)
{
    #if !defined(RAYGUI_COLORBARALPHA_CHECKED_SIZE)
        #define RAYGUI_COLORBARALPHA_CHECKED_SIZE   10
    #endif

    int result = 0;
    GuiState state = guiState;
    Rectangle selector = { (float)bounds.x + (*alpha)*bounds.width - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.y - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT), (float)bounds.height + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2 };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiSliderDragging) // Keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiSliderActive))
                {
                    state = STATE_PRESSED;

                    *alpha = (mousePoint.x - bounds.x)/bounds.width;
                    if (*alpha <= 0.0f) *alpha = 0.0f;
                    if (*alpha >= 1.0f) *alpha = 1.0f;
                }
            }
            else
            {
                guiSliderDragging = false;
                guiSliderActive = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds) || CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiSliderDragging = true;
                guiSliderActive = bounds; // Store bounds as an identifier when dragging starts

                *alpha = (mousePoint.x - bounds.x)/bounds.width;
                if (*alpha <= 0.0f) *alpha = 0.0f;
                if (*alpha >= 1.0f) *alpha = 1.0f;
                //selector.x = bounds.x + (int)(((alpha - 0)/(100 - 0))*(bounds.width - 2*GuiGetStyle(SLIDER, BORDER_WIDTH))) - selector.width/2;
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------

    // Draw alpha bar: checked background
    if (state != STATE_DISABLED)
    {
        int checksX = (int)bounds.width/RAYGUI_COLORBARALPHA_CHECKED_SIZE;
        int checksY = (int)bounds.height/RAYGUI_COLORBARALPHA_CHECKED_SIZE;

        for (int x = 0; x < checksX; x++)
        {
            for (int y = 0; y < checksY; y++)
            {
                Rectangle check = { bounds.x + x*RAYGUI_COLORBARALPHA_CHECKED_SIZE, bounds.y + y*RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE, RAYGUI_COLORBARALPHA_CHECKED_SIZE };
                GuiDrawRectangle(check, 0, BLANK, ((x + y)%2)? Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), 0.4f) : Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.4f));
            }
        }

        DrawRectangleGradientEx(bounds, RAYGUI_CLITERAL(Color){ 255, 255, 255, 0 }, RAYGUI_CLITERAL(Color){ 255, 255, 255, 0 }, Fade(RAYGUI_CLITERAL(Color){ 0, 0, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 0, 255 }, guiAlpha));
    }
    else DrawRectangleGradientEx(bounds, Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha));

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);

    // Draw alpha bar: selector
    GuiDrawRectangle(selector, 0, BLANK, GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)));
    //--------------------------------------------------------------------

    return result;
}

// Color Bar Hue control
// Returns hue value normalized [0..1]
// NOTE: Other similar bars (for reference):
//      Color GuiColorBarSat() [WHITE->color]
//      Color GuiColorBarValue() [BLACK->color], HSV/HSL
//      float GuiColorBarLuminance() [BLACK->WHITE]
int GuiColorBarHue(Rectangle bounds, const char *text, float *hue)
{
    int result = 0;
    GuiState state = guiState;
    Rectangle selector = { (float)bounds.x - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW), (float)bounds.y + (*hue)/360.0f*bounds.height - GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT)/2, (float)bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW)*2, (float)GuiGetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT) };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiSliderDragging) // Keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CHECK_BOUNDS_ID(bounds, guiSliderActive))
                {
                    state = STATE_PRESSED;

                    *hue = (mousePoint.y - bounds.y)*360/bounds.height;
                    if (*hue <= 0.0f) *hue = 0.0f;
                    if (*hue >= 359.0f) *hue = 359.0f;
                }
            }
            else
            {
                guiSliderDragging = false;
                guiSliderActive = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds) || CheckCollisionPointRec(mousePoint, selector))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                guiSliderDragging = true;
                guiSliderActive = bounds; // Store bounds as an identifier when dragging starts

                *hue = (mousePoint.y - bounds.y)*360/bounds.height;
                if (*hue <= 0.0f) *hue = 0.0f;
                if (*hue >= 359.0f) *hue = 359.0f;

            }
            else state = STATE_FOCUSED;

            /*if (IsKeyDown(KEY_UP))
            {
                hue -= 2.0f;
                if (hue <= 0.0f) hue = 0.0f;
            }
            else if (IsKeyDown(KEY_DOWN))
            {
                hue += 2.0f;
                if (hue >= 360.0f) hue = 360.0f;
            }*/
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        // Draw hue bar:color bars
        // TODO: Use directly DrawRectangleGradientEx(bounds, color1, color2, color2, color1);
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 255, 0, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + bounds.height/6), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 255, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 0, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 2*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 0, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 3*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 255, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 4*(bounds.height/6)), (int)bounds.width, (int)ceilf(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 0, 0, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 255, 255 }, guiAlpha));
        DrawRectangleGradientV((int)bounds.x, (int)(bounds.y + 5*(bounds.height/6)), (int)bounds.width, (int)(bounds.height/6), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 255, 255 }, guiAlpha), Fade(RAYGUI_CLITERAL(Color){ 255, 0, 0, 255 }, guiAlpha));
    }
    else DrawRectangleGradientV((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height, Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), guiAlpha));

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);

    // Draw hue bar: selector
    GuiDrawRectangle(selector, 0, BLANK, GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)));
    //--------------------------------------------------------------------

    return result;
}

// Color Picker control
// NOTE: It's divided in multiple controls:
//      Color GuiColorPanel(Rectangle bounds, Color color)
//      float GuiColorBarAlpha(Rectangle bounds, float alpha)
//      float GuiColorBarHue(Rectangle bounds, float value)
// NOTE: bounds define GuiColorPanel() size
int GuiColorPicker(Rectangle bounds, const char *text, Color *color)
{
    int result = 0;

    Color temp = { 200, 0, 0, 255 };
    if (color == NULL) color = &temp;

    GuiColorPanel(bounds, NULL, color);

    Rectangle boundsHue = { (float)bounds.x + bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_PADDING), (float)bounds.y, (float)GuiGetStyle(COLORPICKER, HUEBAR_WIDTH), (float)bounds.height };
    //Rectangle boundsAlpha = { bounds.x, bounds.y + bounds.height + GuiGetStyle(COLORPICKER, BARS_PADDING), bounds.width, GuiGetStyle(COLORPICKER, BARS_THICK) };

    Vector3 hsv = ConvertRGBtoHSV(RAYGUI_CLITERAL(Vector3){ (*color).r/255.0f, (*color).g/255.0f, (*color).b/255.0f });

    GuiColorBarHue(boundsHue, NULL, &hsv.x);

    //color.a = (unsigned char)(GuiColorBarAlpha(boundsAlpha, (float)color.a/255.0f)*255.0f);
    Vector3 rgb = ConvertHSVtoRGB(hsv);

    *color = RAYGUI_CLITERAL(Color){ (unsigned char)roundf(rgb.x*255.0f), (unsigned char)roundf(rgb.y*255.0f), (unsigned char)roundf(rgb.z*255.0f), (*color).a };

    return result;
}

// Color Picker control that avoids conversion to RGB and back to HSV on each call, thus avoiding jittering.
// The user can call ConvertHSVtoRGB() to convert *colorHsv value to RGB.
// NOTE: It's divided in multiple controls:
//      int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
//      int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha)
//      float GuiColorBarHue(Rectangle bounds, float value)
// NOTE: bounds define GuiColorPanelHSV() size
int GuiColorPickerHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
{
    int result = 0;

    Vector3 tempHsv = { 0 };

    if (colorHsv == NULL)
    {
        const Vector3 tempColor = { 200.0f/255.0f, 0.0f, 0.0f };
        tempHsv = ConvertRGBtoHSV(tempColor);
        colorHsv = &tempHsv;
    }

    GuiColorPanelHSV(bounds, NULL, colorHsv);

    const Rectangle boundsHue = { (float)bounds.x + bounds.width + GuiGetStyle(COLORPICKER, HUEBAR_PADDING), (float)bounds.y, (float)GuiGetStyle(COLORPICKER, HUEBAR_WIDTH), (float)bounds.height };

    GuiColorBarHue(boundsHue, NULL, &colorHsv->x);

    return result;
}

// Color Panel control, returns HSV color value in *colorHsv.
// Used by GuiColorPickerHSV()
int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv)
{
    int result = 0;
    GuiState state = guiState;
    Vector2 pickerSelector = { 0 };

    const Color colWhite = { 255, 255, 255, 255 };
    const Color colBlack = { 0, 0, 0, 255 };

    pickerSelector.x = bounds.x + (float)colorHsv->y*bounds.width;            // HSV: Saturation
    pickerSelector.y = bounds.y + (1.0f - (float)colorHsv->z)*bounds.height;  // HSV: Value

    float hue = -1.0f;
    Vector3 maxHue = { (hue >= 0.0f)? hue : colorHsv->x, 1.0f, 1.0f };
    Vector3 rgbHue = ConvertHSVtoRGB(maxHue);
    Color maxHueCol = { (unsigned char)(255.0f*rgbHue.x),
                      (unsigned char)(255.0f*rgbHue.y),
                      (unsigned char)(255.0f*rgbHue.z), 255 };

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = STATE_PRESSED;
                pickerSelector = mousePoint;

                // Calculate color from picker
                Vector2 colorPick = { pickerSelector.x - bounds.x, pickerSelector.y - bounds.y };

                colorPick.x /= (float)bounds.width;     // Get normalized value on x
                colorPick.y /= (float)bounds.height;    // Get normalized value on y

                colorHsv->y = colorPick.x;
                colorHsv->z = 1.0f - colorPick.y;
            }
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        DrawRectangleGradientEx(bounds, Fade(colWhite, guiAlpha), Fade(colWhite, guiAlpha), Fade(maxHueCol, guiAlpha), Fade(maxHueCol, guiAlpha));
        DrawRectangleGradientEx(bounds, Fade(colBlack, 0), Fade(colBlack, guiAlpha), Fade(colBlack, guiAlpha), Fade(colBlack, 0));

        // Draw color picker: selector
        Rectangle selector = { pickerSelector.x - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, pickerSelector.y - GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE)/2, (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE), (float)GuiGetStyle(COLORPICKER, COLOR_SELECTOR_SIZE) };
        GuiDrawRectangle(selector, 0, BLANK, colWhite);
    }
    else
    {
        DrawRectangleGradientEx(bounds, Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BASE_COLOR_DISABLED)), 0.1f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(colBlack, 0.6f), guiAlpha), Fade(Fade(GetColor(GuiGetStyle(COLORPICKER, BORDER_COLOR_DISABLED)), 0.6f), guiAlpha));
    }

    GuiDrawRectangle(bounds, GuiGetStyle(COLORPICKER, BORDER_WIDTH), GetColor(GuiGetStyle(COLORPICKER, BORDER + state*3)), BLANK);
    //--------------------------------------------------------------------

    return result;
}

// Message Box control
int GuiMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons)
{
    #if !defined(RAYGUI_MESSAGEBOX_BUTTON_HEIGHT)
        #define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT    24
    #endif
    #if !defined(RAYGUI_MESSAGEBOX_BUTTON_PADDING)
        #define RAYGUI_MESSAGEBOX_BUTTON_PADDING   12
    #endif

    int result = -1;    // Returns clicked button from buttons list, 0 refers to closed window button

    int buttonCount = 0;
    const char **buttonsText = GuiTextSplit(buttons, ';', &buttonCount, NULL);
    Rectangle buttonBounds = { 0 };
    buttonBounds.x = bounds.x + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    buttonBounds.y = bounds.y + bounds.height - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT - RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    buttonBounds.width = (bounds.width - RAYGUI_MESSAGEBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
    buttonBounds.height = RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

    int textWidth = GetTextWidth(message) + 2;

    Rectangle textBounds = { 0 };
    textBounds.x = bounds.x + bounds.width/2 - textWidth/2;
    textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + RAYGUI_MESSAGEBOX_BUTTON_PADDING;
    textBounds.width = (float)textWidth;
    textBounds.height = bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 3*RAYGUI_MESSAGEBOX_BUTTON_PADDING - RAYGUI_MESSAGEBOX_BUTTON_HEIGHT;

    // Draw control
    //--------------------------------------------------------------------
    if (GuiWindowBox(bounds, title)) result = 0;

    int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel(textBounds, message);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevTextAlignment);

    prevTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    for (int i = 0; i < buttonCount; i++)
    {
        if (GuiButton(buttonBounds, buttonsText[i])) result = i + 1;
        buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
    }

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, prevTextAlignment);
    //--------------------------------------------------------------------

    return result;
}

// Text Input Box control, ask for text
int GuiTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool *secretViewActive)
{
    #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT)
        #define RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT      24
    #endif
    #if !defined(RAYGUI_TEXTINPUTBOX_BUTTON_PADDING)
        #define RAYGUI_TEXTINPUTBOX_BUTTON_PADDING     12
    #endif
    #if !defined(RAYGUI_TEXTINPUTBOX_HEIGHT)
        #define RAYGUI_TEXTINPUTBOX_HEIGHT             26
    #endif

    // Used to enable text edit mode
    // WARNING: No more than one GuiTextInputBox() should be open at the same time
    static bool textEditMode = false;

    int result = -1;

    int buttonCount = 0;
    const char **buttonsText = GuiTextSplit(buttons, ';', &buttonCount, NULL);
    Rectangle buttonBounds = { 0 };
    buttonBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    buttonBounds.y = bounds.y + bounds.height - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    buttonBounds.width = (bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*(buttonCount + 1))/buttonCount;
    buttonBounds.height = RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT;

    int messageInputHeight = (int)bounds.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - GuiGetStyle(STATUSBAR, BORDER_WIDTH) - RAYGUI_TEXTINPUTBOX_BUTTON_HEIGHT - 2*RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;

    Rectangle textBounds = { 0 };
    if (message != NULL)
    {
        int textSize = GetTextWidth(message) + 2;

        textBounds.x = bounds.x + bounds.width/2 - textSize/2;
        textBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + messageInputHeight/4 - (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/2;
        textBounds.width = (float)textSize;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    }

    Rectangle textBoxBounds = { 0 };
    textBoxBounds.x = bounds.x + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    textBoxBounds.y = bounds.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_TEXTINPUTBOX_HEIGHT/2;
    if (message == NULL) textBoxBounds.y = bounds.y + 24 + RAYGUI_TEXTINPUTBOX_BUTTON_PADDING;
    else textBoxBounds.y += (messageInputHeight/2 + messageInputHeight/4);
    textBoxBounds.width = bounds.width - RAYGUI_TEXTINPUTBOX_BUTTON_PADDING*2;
    textBoxBounds.height = RAYGUI_TEXTINPUTBOX_HEIGHT;

    // Draw control
    //--------------------------------------------------------------------
    if (GuiWindowBox(bounds, title)) result = 0;

    // Draw message if available
    if (message != NULL)
    {
        int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiLabel(textBounds, message);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevTextAlignment);
    }

    if (secretViewActive != NULL)
    {
        static char stars[] = "****************";
        if (GuiTextBox(RAYGUI_CLITERAL(Rectangle){ textBoxBounds.x, textBoxBounds.y, textBoxBounds.width - 4 - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.height },
            ((*secretViewActive == 1) || textEditMode)? text : stars, textMaxSize, textEditMode)) textEditMode = !textEditMode;

        GuiToggle(RAYGUI_CLITERAL(Rectangle){ textBoxBounds.x + textBoxBounds.width - RAYGUI_TEXTINPUTBOX_HEIGHT, textBoxBounds.y, RAYGUI_TEXTINPUTBOX_HEIGHT, RAYGUI_TEXTINPUTBOX_HEIGHT }, (*secretViewActive == 1)? "#44#" : "#45#", secretViewActive);
    }
    else
    {
        if (GuiTextBox(textBoxBounds, text, textMaxSize, textEditMode)) textEditMode = !textEditMode;
    }

    int prevBtnTextAlignment = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    for (int i = 0; i < buttonCount; i++)
    {
        if (GuiButton(buttonBounds, buttonsText[i])) result = i + 1;
        buttonBounds.x += (buttonBounds.width + RAYGUI_MESSAGEBOX_BUTTON_PADDING);
    }

    if (result >= 0) textEditMode = false;

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, prevBtnTextAlignment);
    //--------------------------------------------------------------------

    return result;      // Result is the pressed button index
}

// Grid control
// NOTE: Returns grid mouse-hover selected cell
// About drawing lines at subpixel spacing, simple put, not easy solution:
// https://stackoverflow.com/questions/4435450/2d-opengl-drawing-lines-that-dont-exactly-fit-pixel-raster
int GuiGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 *mouseCell)
{
    // Grid lines alpha amount
    #if !defined(RAYGUI_GRID_ALPHA)
        #define RAYGUI_GRID_ALPHA    0.15f
    #endif

    int result = 0;
    GuiState state = guiState;

    Vector2 mousePoint = GetMousePosition();
    Vector2 currentMouseCell = { -1, -1 };

    float spaceWidth = spacing/(float)subdivs;
    int linesV = (int)(bounds.width/spaceWidth) + 1;
    int linesH = (int)(bounds.height/spaceWidth) + 1;

    int color = GuiGetStyle(DEFAULT, LINE_COLOR);

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging)
    {
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // NOTE: Cell values must be the upper left of the cell the mouse is in
            currentMouseCell.x = floorf((mousePoint.x - bounds.x)/spacing);
            currentMouseCell.y = floorf((mousePoint.y - bounds.y)/spacing);
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    if (state == STATE_DISABLED) color = GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED);

    if (subdivs > 0)
    {
        // Draw vertical grid lines
        for (int i = 0; i < linesV; i++)
        {
            Rectangle lineV = { bounds.x + spacing*i/subdivs, bounds.y, 1, bounds.height };
            GuiDrawRectangle(lineV, 0, BLANK, ((i%subdivs) == 0)? GuiFade(GetColor(color), RAYGUI_GRID_ALPHA*4) : GuiFade(GetColor(color), RAYGUI_GRID_ALPHA));
        }

        // Draw horizontal grid lines
        for (int i = 0; i < linesH; i++)
        {
            Rectangle lineH = { bounds.x, bounds.y + spacing*i/subdivs, bounds.width, 1 };
            GuiDrawRectangle(lineH, 0, BLANK, ((i%subdivs) == 0)? GuiFade(GetColor(color), RAYGUI_GRID_ALPHA*4) : GuiFade(GetColor(color), RAYGUI_GRID_ALPHA));
        }
    }

    if (mouseCell != NULL) *mouseCell = currentMouseCell;
    return result;
}

//----------------------------------------------------------------------------------
// Tooltip management functions
// NOTE: Tooltips requires some global variables: tooltipPtr
//----------------------------------------------------------------------------------
// Enable gui tooltips (global state)
void GuiEnableTooltip(void) { guiTooltip = true; }

// Disable gui tooltips (global state)
void GuiDisableTooltip(void) { guiTooltip = false; }

// Set tooltip string
void GuiSetTooltip(const char *tooltip) { guiTooltipPtr = tooltip; }


//----------------------------------------------------------------------------------
// Styles loading functions
//----------------------------------------------------------------------------------

// Load raygui style file (.rgs)
// NOTE: By default a binary file is expected, that file could contain a custom font,
// in that case, custom font image atlas is GRAY+ALPHA and pixel data can be compressed (DEFLATE)
void GuiLoadStyle(const char *fileName)
{
    #define MAX_LINE_BUFFER_SIZE    256

    bool tryBinary = false;

    // Try reading the files as text file first
    FILE *rgsFile = fopen(fileName, "rt");

    if (rgsFile != NULL)
    {
        char buffer[MAX_LINE_BUFFER_SIZE] = { 0 };
        fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);

        if (buffer[0] == '#')
        {
            int controlId = 0;
            int propertyId = 0;
            unsigned int propertyValue = 0;

            while (!feof(rgsFile))
            {
                switch (buffer[0])
                {
                    case 'p':
                    {
                        // Style property: p <control_id> <property_id> <property_value> <property_name>

                        sscanf(buffer, "p %d %d 0x%x", &controlId, &propertyId, &propertyValue);
                        GuiSetStyle(controlId, propertyId, (int)propertyValue);

                    } break;
                    case 'f':
                    {
                        // Style font: f <gen_font_size> <charmap_file> <font_file>

                        int fontSize = 0;
                        char charmapFileName[256] = { 0 };
                        char fontFileName[256] = { 0 };
                        sscanf(buffer, "f %d %s %[^\r\n]s", &fontSize, charmapFileName, fontFileName);

                        Font font = { 0 };
                        int *codepoints = NULL;
                        int codepointCount = 0;

                        if (charmapFileName[0] != '0')
                        {
                            // Load text data from file
                            // NOTE: Expected an UTF-8 array of codepoints, no separation
                            char *textData = LoadFileText(TextFormat("%s/%s", GetDirectoryPath(fileName), charmapFileName));
                            codepoints = LoadCodepoints(textData, &codepointCount);
                            UnloadFileText(textData);
                        }

                        if (fontFileName[0] != '\0')
                        {
                            // In case a font is already loaded and it is not default internal font, unload it
                            if (font.texture.id != GetFontDefault().texture.id) UnloadTexture(font.texture);

                            if (codepointCount > 0) font = LoadFontEx(TextFormat("%s/%s", GetDirectoryPath(fileName), fontFileName), fontSize, codepoints, codepointCount);
                            else font = LoadFontEx(TextFormat("%s/%s", GetDirectoryPath(fileName), fontFileName), fontSize, NULL, 0);   // Default to 95 standard codepoints
                        }

                        // If font texture not properly loaded, revert to default font and size/spacing
                        if (font.texture.id == 0)
                        {
                            font = GetFontDefault();
                            GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
                            GuiSetStyle(DEFAULT, TEXT_SPACING, 1);
                        }

                        UnloadCodepoints(codepoints);

                        if ((font.texture.id > 0) && (font.glyphCount > 0)) GuiSetFont(font);

                    } break;
                    default: break;
                }

                fgets(buffer, MAX_LINE_BUFFER_SIZE, rgsFile);
            }
        }
        else tryBinary = true;

        fclose(rgsFile);
    }

    if (tryBinary)
    {
        rgsFile = fopen(fileName, "rb");

        if (rgsFile != NULL)
        {
            fseek(rgsFile, 0, SEEK_END);
            int fileDataSize = ftell(rgsFile);
            fseek(rgsFile, 0, SEEK_SET);

            if (fileDataSize > 0)
            {
                unsigned char *fileData = (unsigned char *)RAYGUI_MALLOC(fileDataSize*sizeof(unsigned char));
                fread(fileData, sizeof(unsigned char), fileDataSize, rgsFile);

                GuiLoadStyleFromMemory(fileData, fileDataSize);

                RAYGUI_FREE(fileData);
            }

            fclose(rgsFile);
        }
    }
}

// Load style default over global style
void GuiLoadStyleDefault(void)
{
    // We set this variable first to avoid cyclic function calls
    // when calling GuiSetStyle() and GuiGetStyle()
    guiStyleLoaded = true;

    // Initialize default LIGHT style property values
    // WARNING: Default value are applied to all controls on set but
    // they can be overwritten later on for every custom control
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x838383ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xc9c9c9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x686868ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x5bb2d9ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xc9effeff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x6c9bbcff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x0492c7ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x97e8ffff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0x368bafff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0xb5c1c2ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0xe6e9e9ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0xaeb7b8ff);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 0);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Initialize default extended property values
    // NOTE: By default, extended property values are initialized to 0
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);                // DEFAULT, shared by all controls
    GuiSetStyle(DEFAULT, TEXT_SPACING, 1);              // DEFAULT, shared by all controls
    GuiSetStyle(DEFAULT, LINE_COLOR, 0x90abb5ff);       // DEFAULT specific property
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xf5f5f5ff); // DEFAULT specific property
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, 15);        // DEFAULT, 15 pixels between lines
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);   // DEFAULT, text aligned vertically to middle of text-bounds

    // Initialize control-specific property values
    // NOTE: Those properties are in default list but require specific values by control type
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
    GuiSetStyle(SLIDER, TEXT_PADDING, 4);
    GuiSetStyle(PROGRESSBAR, TEXT_PADDING, 4);
    GuiSetStyle(CHECKBOX, TEXT_PADDING, 4);
    GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 4);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(VALUEBOX, TEXT_PADDING, 0);
    GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(SPINNER, TEXT_PADDING, 0);
    GuiSetStyle(SPINNER, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(STATUSBAR, TEXT_PADDING, 8);
    GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    // Initialize extended property values
    // NOTE: By default, extended property values are initialized to 0
    GuiSetStyle(TOGGLE, GROUP_PADDING, 2);
    GuiSetStyle(SLIDER, SLIDER_WIDTH, 16);
    GuiSetStyle(SLIDER, SLIDER_PADDING, 1);
    GuiSetStyle(PROGRESSBAR, PROGRESS_PADDING, 1);
    GuiSetStyle(CHECKBOX, CHECK_PADDING, 1);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, 32);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_SPACING, 2);
    GuiSetStyle(DROPDOWNBOX, ARROW_PADDING, 16);
    GuiSetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING, 2);
    GuiSetStyle(SPINNER, SPIN_BUTTON_WIDTH, 24);
    GuiSetStyle(SPINNER, SPIN_BUTTON_SPACING, 2);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    GuiSetStyle(SCROLLBAR, ARROWS_VISIBLE, 0);
    GuiSetStyle(SCROLLBAR, ARROWS_SIZE, 6);
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING, 0);
    GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, 16);
    GuiSetStyle(SCROLLBAR, SCROLL_PADDING, 0);
    GuiSetStyle(SCROLLBAR, SCROLL_SPEED, 12);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 28);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_SPACING, 2);
    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 12);
    GuiSetStyle(LISTVIEW, SCROLLBAR_SIDE, SCROLLBAR_RIGHT_SIDE);
    GuiSetStyle(COLORPICKER, COLOR_SELECTOR_SIZE, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_WIDTH, 16);
    GuiSetStyle(COLORPICKER, HUEBAR_PADDING, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT, 8);
    GuiSetStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW, 2);

    if (guiFont.texture.id != GetFontDefault().texture.id)
    {
        // Unload previous font texture
        UnloadTexture(guiFont.texture);
        RL_FREE(guiFont.recs);
        RL_FREE(guiFont.glyphs);
        guiFont.recs = NULL;
        guiFont.glyphs = NULL;

        // Setup default raylib font
        guiFont = GetFontDefault();

        // NOTE: Default raylib font character 95 is a white square
        Rectangle whiteChar = guiFont.recs[95];

        // NOTE: We set up a 1px padding on char rectangle to avoid pixel bleeding on MSAA filtering
        SetShapesTexture(guiFont.texture, RAYGUI_CLITERAL(Rectangle){ whiteChar.x + 1, whiteChar.y + 1, whiteChar.width - 2, whiteChar.height - 2 });
    }
}

// Get text with icon id prepended
// NOTE: Useful to add icons by name id (enum) instead of
// a number that can change between ricon versions
const char *GuiIconText(int iconId, const char *text)
{
#if defined(RAYGUI_NO_ICONS)
    return NULL;
#else
    static char buffer[1024] = { 0 };
    static char iconBuffer[6] = { 0 };

    if (text != NULL)
    {
        memset(buffer, 0, 1024);
        sprintf(buffer, "#%03i#", iconId);

        for (int i = 5; i < 1024; i++)
        {
            buffer[i] = text[i - 5];
            if (text[i - 5] == '\0') break;
        }

        return buffer;
    }
    else
    {
        sprintf(iconBuffer, "#%03i#", iconId & 0x1ff);

        return iconBuffer;
    }
#endif
}

#if !defined(RAYGUI_NO_ICONS)
// Get full icons data pointer
unsigned int *GuiGetIcons(void) { return guiIconsPtr; }

// Load raygui icons file (.rgi)
// NOTE: In case nameIds are required, they can be requested with loadIconsName,
// they are returned as a guiIconsName[iconCount][RAYGUI_ICON_MAX_NAME_LENGTH],
// WARNING: guiIconsName[]][] memory should be manually freed!
char **GuiLoadIcons(const char *fileName, bool loadIconsName)
{
    // Style File Structure (.rgi)
    // ------------------------------------------------------
    // Offset  | Size    | Type       | Description
    // ------------------------------------------------------
    // 0       | 4       | char       | Signature: "rGI "
    // 4       | 2       | short      | Version: 100
    // 6       | 2       | short      | reserved

    // 8       | 2       | short      | Num icons (N)
    // 10      | 2       | short      | Icons size (Options: 16, 32, 64) (S)

    // Icons name id (32 bytes per name id)
    // foreach (icon)
    // {
    //   12+32*i  | 32   | char       | Icon NameId
    // }

    // Icons data: One bit per pixel, stored as unsigned int array (depends on icon size)
    // S*S pixels/32bit per unsigned int = K unsigned int per icon
    // foreach (icon)
    // {
    //   ...   | K       | unsigned int | Icon Data
    // }

    FILE *rgiFile = fopen(fileName, "rb");

    char **guiIconsName = NULL;

    if (rgiFile != NULL)
    {
        char signature[5] = { 0 };
        short version = 0;
        short reserved = 0;
        short iconCount = 0;
        short iconSize = 0;

        fread(signature, 1, 4, rgiFile);
        fread(&version, sizeof(short), 1, rgiFile);
        fread(&reserved, sizeof(short), 1, rgiFile);
        fread(&iconCount, sizeof(short), 1, rgiFile);
        fread(&iconSize, sizeof(short), 1, rgiFile);

        if ((signature[0] == 'r') &&
            (signature[1] == 'G') &&
            (signature[2] == 'I') &&
            (signature[3] == ' '))
        {
            if (loadIconsName)
            {
                guiIconsName = (char **)RAYGUI_MALLOC(iconCount*sizeof(char **));
                for (int i = 0; i < iconCount; i++)
                {
                    guiIconsName[i] = (char *)RAYGUI_MALLOC(RAYGUI_ICON_MAX_NAME_LENGTH);
                    fread(guiIconsName[i], 1, RAYGUI_ICON_MAX_NAME_LENGTH, rgiFile);
                }
            }
            else fseek(rgiFile, iconCount*RAYGUI_ICON_MAX_NAME_LENGTH, SEEK_CUR);

            // Read icons data directly over internal icons array
            fread(guiIconsPtr, sizeof(unsigned int), iconCount*(iconSize*iconSize/32), rgiFile);
        }

        fclose(rgiFile);
    }

    return guiIconsName;
}

// Draw selected icon using rectangles pixel-by-pixel
void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color)
{
    #define BIT_CHECK(a,b) ((a) & (1u<<(b)))

    for (int i = 0, y = 0; i < RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32; i++)
    {
        for (int k = 0; k < 32; k++)
        {
            if (BIT_CHECK(guiIconsPtr[iconId*RAYGUI_ICON_DATA_ELEMENTS + i], k))
            {
            #if !defined(RAYGUI_STANDALONE)
                GuiDrawRectangle(RAYGUI_CLITERAL(Rectangle){ (float)posX + (k%RAYGUI_ICON_SIZE)*pixelSize, (float)posY + y*pixelSize, (float)pixelSize, (float)pixelSize }, 0, BLANK, color);
            #endif
            }

            if ((k == 15) || (k == 31)) y++;
        }
    }
}

// Set icon drawing size
void GuiSetIconScale(int scale)
{
    if (scale >= 1) guiIconScale = scale;
}

#endif      // !RAYGUI_NO_ICONS

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

// Load style from memory
// WARNING: Binary files only
void GuiLoadStyleFromMemory(const unsigned char *fileData, int dataSize)
{
    unsigned char *fileDataPtr = (unsigned char *)fileData;

    char signature[5] = { 0 };
    short version = 0;
    short reserved = 0;
    int propertyCount = 0;

    memcpy(signature, fileDataPtr, 4);
    memcpy(&version, fileDataPtr + 4, sizeof(short));
    memcpy(&reserved, fileDataPtr + 4 + 2, sizeof(short));
    memcpy(&propertyCount, fileDataPtr + 4 + 2 + 2, sizeof(int));
    fileDataPtr += 12;

    if ((signature[0] == 'r') &&
        (signature[1] == 'G') &&
        (signature[2] == 'S') &&
        (signature[3] == ' '))
    {
        short controlId = 0;
        short propertyId = 0;
        unsigned int propertyValue = 0;

        for (int i = 0; i < propertyCount; i++)
        {
            memcpy(&controlId, fileDataPtr, sizeof(short));
            memcpy(&propertyId, fileDataPtr + 2, sizeof(short));
            memcpy(&propertyValue, fileDataPtr + 2 + 2, sizeof(unsigned int));
            fileDataPtr += 8;

            if (controlId == 0) // DEFAULT control
            {
                // If a DEFAULT property is loaded, it is propagated to all controls
                // NOTE: All DEFAULT properties should be defined first in the file
                GuiSetStyle(0, (int)propertyId, propertyValue);

                if (propertyId < RAYGUI_MAX_PROPS_BASE) for (int i = 1; i < RAYGUI_MAX_CONTROLS; i++) GuiSetStyle(i, (int)propertyId, propertyValue);
            }
            else GuiSetStyle((int)controlId, (int)propertyId, propertyValue);
        }

        // Font loading is highly dependant on raylib API to load font data and image

#if !defined(RAYGUI_STANDALONE)
        // Load custom font if available
        int fontDataSize = 0;
        memcpy(&fontDataSize, fileDataPtr, sizeof(int));
        fileDataPtr += 4;

        if (fontDataSize > 0)
        {
            Font font = { 0 };
            int fontType = 0;   // 0-Normal, 1-SDF

            memcpy(&font.baseSize, fileDataPtr, sizeof(int));
            memcpy(&font.glyphCount, fileDataPtr + 4, sizeof(int));
            memcpy(&fontType, fileDataPtr + 4 + 4, sizeof(int));
            fileDataPtr += 12;

            // Load font white rectangle
            Rectangle fontWhiteRec = { 0 };
            memcpy(&fontWhiteRec, fileDataPtr, sizeof(Rectangle));
            fileDataPtr += 16;

            // Load font image parameters
            int fontImageUncompSize = 0;
            int fontImageCompSize = 0;
            memcpy(&fontImageUncompSize, fileDataPtr, sizeof(int));
            memcpy(&fontImageCompSize, fileDataPtr + 4, sizeof(int));
            fileDataPtr += 8;

            Image imFont = { 0 };
            imFont.mipmaps = 1;
            memcpy(&imFont.width, fileDataPtr, sizeof(int));
            memcpy(&imFont.height, fileDataPtr + 4, sizeof(int));
            memcpy(&imFont.format, fileDataPtr + 4 + 4, sizeof(int));
            fileDataPtr += 12;

            if ((fontImageCompSize > 0) && (fontImageCompSize != fontImageUncompSize))
            {
                // Compressed font atlas image data (DEFLATE), it requires DecompressData()
                int dataUncompSize = 0;
                unsigned char *compData = (unsigned char *)RAYGUI_MALLOC(fontImageCompSize);
                memcpy(compData, fileDataPtr, fontImageCompSize);
                fileDataPtr += fontImageCompSize;

                imFont.data = DecompressData(compData, fontImageCompSize, &dataUncompSize);

                // Security check, dataUncompSize must match the provided fontImageUncompSize
                if (dataUncompSize != fontImageUncompSize) RAYGUI_LOG("WARNING: Uncompressed font atlas image data could be corrupted");

                RAYGUI_FREE(compData);
            }
            else
            {
                // Font atlas image data is not compressed
                imFont.data = (unsigned char *)RAYGUI_MALLOC(fontImageUncompSize);
                memcpy(imFont.data, fileDataPtr, fontImageUncompSize);
                fileDataPtr += fontImageUncompSize;
            }

            if (font.texture.id != GetFontDefault().texture.id) UnloadTexture(font.texture);
            font.texture = LoadTextureFromImage(imFont);

            RAYGUI_FREE(imFont.data);

            // Validate font atlas texture was loaded correctly
            if (font.texture.id != 0)
            {
                // Load font recs data
                int recsDataSize = font.glyphCount*sizeof(Rectangle);
                int recsDataCompressedSize = 0;

                // WARNING: Version 400 adds the compression size parameter
                if (version >= 400)
                {
                    // RGS files version 400 support compressed recs data
                    memcpy(&recsDataCompressedSize, fileDataPtr, sizeof(int));
                    fileDataPtr += sizeof(int);
                }

                if ((recsDataCompressedSize > 0) && (recsDataCompressedSize != recsDataSize))
                {
                    // Recs data is compressed, uncompress it
                    unsigned char *recsDataCompressed = (unsigned char *)RAYGUI_MALLOC(recsDataCompressedSize);

                    memcpy(recsDataCompressed, fileDataPtr, recsDataCompressedSize);
                    fileDataPtr += recsDataCompressedSize;

                    int recsDataUncompSize = 0;
                    font.recs = (Rectangle *)DecompressData(recsDataCompressed, recsDataCompressedSize, &recsDataUncompSize);

                    // Security check, data uncompressed size must match the expected original data size
                    if (recsDataUncompSize != recsDataSize) RAYGUI_LOG("WARNING: Uncompressed font recs data could be corrupted");

                    RAYGUI_FREE(recsDataCompressed);
                }
                else
                {
                    // Recs data is uncompressed
                    font.recs = (Rectangle *)RAYGUI_CALLOC(font.glyphCount, sizeof(Rectangle));
                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.recs[i], fileDataPtr, sizeof(Rectangle));
                        fileDataPtr += sizeof(Rectangle);
                    }
                }

                // Load font glyphs info data
                int glyphsDataSize = font.glyphCount*16;    // 16 bytes data per glyph
                int glyphsDataCompressedSize = 0;

                // WARNING: Version 400 adds the compression size parameter
                if (version >= 400)
                {
                    // RGS files version 400 support compressed glyphs data
                    memcpy(&glyphsDataCompressedSize, fileDataPtr, sizeof(int));
                    fileDataPtr += sizeof(int);
                }

                // Allocate required glyphs space to fill with data
                font.glyphs = (GlyphInfo *)RAYGUI_CALLOC(font.glyphCount, sizeof(GlyphInfo));

                if ((glyphsDataCompressedSize > 0) && (glyphsDataCompressedSize != glyphsDataSize))
                {
                    // Glyphs data is compressed, uncompress it
                    unsigned char *glypsDataCompressed = (unsigned char *)RAYGUI_MALLOC(glyphsDataCompressedSize);

                    memcpy(glypsDataCompressed, fileDataPtr, glyphsDataCompressedSize);
                    fileDataPtr += glyphsDataCompressedSize;

                    int glyphsDataUncompSize = 0;
                    unsigned char *glyphsDataUncomp = DecompressData(glypsDataCompressed, glyphsDataCompressedSize, &glyphsDataUncompSize);

                    // Security check, data uncompressed size must match the expected original data size
                    if (glyphsDataUncompSize != glyphsDataSize) RAYGUI_LOG("WARNING: Uncompressed font glyphs data could be corrupted");

                    unsigned char *glyphsDataUncompPtr = glyphsDataUncomp;

                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.glyphs[i].value, glyphsDataUncompPtr, sizeof(int));
                        memcpy(&font.glyphs[i].offsetX, glyphsDataUncompPtr + 4, sizeof(int));
                        memcpy(&font.glyphs[i].offsetY, glyphsDataUncompPtr + 8, sizeof(int));
                        memcpy(&font.glyphs[i].advanceX, glyphsDataUncompPtr + 12, sizeof(int));
                        glyphsDataUncompPtr += 16;
                    }

                    RAYGUI_FREE(glypsDataCompressed);
                    RAYGUI_FREE(glyphsDataUncomp);
                }
                else
                {
                    // Glyphs data is uncompressed
                    for (int i = 0; i < font.glyphCount; i++)
                    {
                        memcpy(&font.glyphs[i].value, fileDataPtr, sizeof(int));
                        memcpy(&font.glyphs[i].offsetX, fileDataPtr + 4, sizeof(int));
                        memcpy(&font.glyphs[i].offsetY, fileDataPtr + 8, sizeof(int));
                        memcpy(&font.glyphs[i].advanceX, fileDataPtr + 12, sizeof(int));
                        fileDataPtr += 16;
                    }
                }
            }
            else font = GetFontDefault();   // Fallback in case of errors loading font atlas texture

            GuiSetFont(font);

            // Set font texture source rectangle to be used as white texture to draw shapes
            // NOTE: It makes possible to draw shapes and text (full UI) in a single draw call
            if ((fontWhiteRec.x > 0) &&
                (fontWhiteRec.y > 0) &&
                (fontWhiteRec.width > 0) &&
                (fontWhiteRec.height > 0)) SetShapesTexture(font.texture, fontWhiteRec);
        }
#endif
    }
}

// Gui get text width considering icon

int GetTextWidth(const char *text)
{
    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    Vector2 textSize = { 0 };
    int textIconOffset = 0;

    if ((text != NULL) && (text[0] != '\0'))
    {
        if (text[0] == '#')
        {
            for (int i = 1; (i < 5) && (text[i] != '\0'); i++)
            {
                if (text[i] == '#')
                {
                    textIconOffset = i;
                    break;
                }
            }
        }

        text += textIconOffset;

        // Make sure guiFont is set, GuiGetStyle() initializes it lazynessly
        float fontSize = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);

        // Custom MeasureText() implementation
        if ((guiFont.texture.id > 0) && (text != NULL))
        {
            // Get size in bytes of text, considering end of line and line break
            int size = 0;
            for (int i = 0; i < MAX_LINE_BUFFER_SIZE; i++)
            {
                if ((text[i] != '\0') && (text[i] != '\n')) size++;
                else break;
            }

            float scaleFactor = fontSize/(float)guiFont.baseSize;
            textSize.y = (float)guiFont.baseSize*scaleFactor;
            float glyphWidth = 0.0f;

            for (int i = 0, codepointSize = 0; i < size; i += codepointSize)
            {
                int codepoint = GetCodepointNext(&text[i], &codepointSize);
                int codepointIndex = GetGlyphIndex(guiFont, codepoint);

                if (guiFont.glyphs[codepointIndex].advanceX == 0) glyphWidth = ((float)guiFont.recs[codepointIndex].width*scaleFactor);
                else glyphWidth = ((float)guiFont.glyphs[codepointIndex].advanceX*scaleFactor);

                textSize.x += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
            }
        }

        if (textIconOffset > 0) textSize.x += (RAYGUI_ICON_SIZE - ICON_TEXT_PADDING);
    }

    return (int)textSize.x;
}

// Get text bounds considering control bounds
Rectangle GetTextBounds(int control, Rectangle bounds)
{
    Rectangle textBounds = bounds;

    textBounds.x = bounds.x + GuiGetStyle(control, BORDER_WIDTH);
    textBounds.y = bounds.y + GuiGetStyle(control, BORDER_WIDTH) + GuiGetStyle(control, TEXT_PADDING);
    textBounds.width = bounds.width - 2*GuiGetStyle(control, BORDER_WIDTH) - 2*GuiGetStyle(control, TEXT_PADDING);
    textBounds.height = bounds.height - 2*GuiGetStyle(control, BORDER_WIDTH) - 2*GuiGetStyle(control, TEXT_PADDING);    // NOTE: Text is processed line per line!

    // Depending on control, TEXT_PADDING and TEXT_ALIGNMENT properties could affect the text-bounds
    switch (control)
    {
        case COMBOBOX:
        case DROPDOWNBOX:
        case LISTVIEW:
            // TODO: Special cases (no label): COMBOBOX, DROPDOWNBOX, LISTVIEW
        case SLIDER:
        case CHECKBOX:
        case VALUEBOX:
        case SPINNER:
            // TODO: More special cases (label on side): SLIDER, CHECKBOX, VALUEBOX, SPINNER
        default:
        {
            // TODO: WARNING: TEXT_ALIGNMENT is already considered in GuiDrawText()
            if (GuiGetStyle(control, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) textBounds.x -= GuiGetStyle(control, TEXT_PADDING);
            else textBounds.x += GuiGetStyle(control, TEXT_PADDING);
        }
        break;
    }

    return textBounds;
}

// Get text icon if provided and move text cursor
// NOTE: We support up to 999 values for iconId
const char *GetTextIcon(const char *text, int *iconId)
{
#if !defined(RAYGUI_NO_ICONS)
    *iconId = -1;
    if (text[0] == '#')     // Maybe we have an icon!
    {
        char iconValue[4] = { 0 };  // Maximum length for icon value: 3 digits + '\0'

        int pos = 1;
        while ((pos < 4) && (text[pos] >= '0') && (text[pos] <= '9'))
        {
            iconValue[pos - 1] = text[pos];
            pos++;
        }

        if (text[pos] == '#')
        {
            *iconId = TextToInteger(iconValue);

            // Move text pointer after icon
            // WARNING: If only icon provided, it could point to EOL character: '\0'
            if (*iconId >= 0) text += (pos + 1);
        }
    }
#endif

    return text;
}

// Get text divided into lines (by line-breaks '\n')
const char **GetTextLines(const char *text, int *count)
{
    #define RAYGUI_MAX_TEXT_LINES   128

    static const char *lines[RAYGUI_MAX_TEXT_LINES] = { 0 };
    for (int i = 0; i < RAYGUI_MAX_TEXT_LINES; i++) lines[i] = NULL;    // Init NULL pointers to substrings

    int textSize = (int)strlen(text);

    lines[0] = text;
    int len = 0;
    *count = 1;
    //int lineSize = 0;   // Stores current line size, not returned

    for (int i = 0, k = 0; (i < textSize) && (*count < RAYGUI_MAX_TEXT_LINES); i++)
    {
        if (text[i] == '\n')
        {
            //lineSize = len;
            k++;
            lines[k] = &text[i + 1];     // WARNING: next value is valid?
            len = 0;
            *count += 1;
        }
        else len++;
    }

    //lines[*count - 1].size = len;

    return lines;
}

// Get text width to next space for provided string
float GetNextSpaceWidth(const char *text, int *nextSpaceIndex)
{
    float width = 0;
    int codepointByteCount = 0;
    int codepoint = 0;
    int index = 0;
    float glyphWidth = 0;
    float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/guiFont.baseSize;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] != ' ')
        {
            codepoint = GetCodepoint(&text[i], &codepointByteCount);
            index = GetGlyphIndex(guiFont, codepoint);
            glyphWidth = (guiFont.glyphs[index].advanceX == 0)? guiFont.recs[index].width*scaleFactor : guiFont.glyphs[index].advanceX*scaleFactor;
            width += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
        }
        else
        {
            *nextSpaceIndex = i;
            break;
        }
    }

    return width;
}

// Gui draw text using default font
void GuiDrawText(const char *text, Rectangle textBounds, int alignment, Color tint)
{
    #define TEXT_VALIGN_PIXEL_OFFSET(h)  ((int)h%2)     // Vertical alignment for pixel perfect

    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    if ((text == NULL) || (text[0] == '\0')) return;    // Security check

    // PROCEDURE:
    //   - Text is processed line per line
    //   - For every line, horizontal alignment is defined
    //   - For all text, vertical alignment is defined (multiline text only)
    //   - For every line, wordwrap mode is checked (useful for GuitextBox(), read-only)

    // Get text lines (using '\n' as delimiter) to be processed individually
    // WARNING: We can't use GuiTextSplit() function because it can be already used
    // before the GuiDrawText() call and its buffer is static, it would be overriden :(
    int lineCount = 0;
    const char **lines = GetTextLines(text, &lineCount);

    // Text style variables
    //int alignment = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT);
    int alignmentVertical = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL);
    int wrapMode = GuiGetStyle(DEFAULT, TEXT_WRAP_MODE);    // Wrap-mode only available in read-only mode, no for text editing

    // TODO: WARNING: This totalHeight is not valid for vertical alignment in case of word-wrap
    float totalHeight = (float)(lineCount*GuiGetStyle(DEFAULT, TEXT_SIZE) + (lineCount - 1)*GuiGetStyle(DEFAULT, TEXT_SIZE)/2);
    float posOffsetY = 0.0f;

    for (int i = 0; i < lineCount; i++)
    {
        int iconId = 0;
        lines[i] = GetTextIcon(lines[i], &iconId);      // Check text for icon and move cursor

        // Get text position depending on alignment and iconId
        //---------------------------------------------------------------------------------
        Vector2 textBoundsPosition = { textBounds.x, textBounds.y };

        // NOTE: We get text size after icon has been processed
        // WARNING: GetTextWidth() also processes text icon to get width! -> Really needed?
        int textSizeX = GetTextWidth(lines[i]);

        // If text requires an icon, add size to measure
        if (iconId >= 0)
        {
            textSizeX += RAYGUI_ICON_SIZE*guiIconScale;

            // WARNING: If only icon provided, text could be pointing to EOF character: '\0'
#if !defined(RAYGUI_NO_ICONS)
            if ((lines[i] != NULL) && (lines[i][0] != '\0')) textSizeX += ICON_TEXT_PADDING;
#endif
        }

        // Check guiTextAlign global variables
        switch (alignment)
        {
            case TEXT_ALIGN_LEFT: textBoundsPosition.x = textBounds.x; break;
            case TEXT_ALIGN_CENTER: textBoundsPosition.x = textBounds.x +  textBounds.width/2 - textSizeX/2; break;
            case TEXT_ALIGN_RIGHT: textBoundsPosition.x = textBounds.x + textBounds.width - textSizeX; break;
            default: break;
        }

        switch (alignmentVertical)
        {
            // Only valid in case of wordWrap = 0;
            case TEXT_ALIGN_TOP: textBoundsPosition.y = textBounds.y + posOffsetY; break;
            case TEXT_ALIGN_MIDDLE: textBoundsPosition.y = textBounds.y + posOffsetY + textBounds.height/2 - totalHeight/2 + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height); break;
            case TEXT_ALIGN_BOTTOM: textBoundsPosition.y = textBounds.y + posOffsetY + textBounds.height - totalHeight + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height); break;
            default: break;
        }

        // NOTE: Make sure we get pixel-perfect coordinates,
        // In case of decimals we got weird text positioning
        textBoundsPosition.x = (float)((int)textBoundsPosition.x);
        textBoundsPosition.y = (float)((int)textBoundsPosition.y);
        //---------------------------------------------------------------------------------

        // Draw text (with icon if available)
        //---------------------------------------------------------------------------------
#if !defined(RAYGUI_NO_ICONS)
        if (iconId >= 0)
        {
            // NOTE: We consider icon height, probably different than text size
            GuiDrawIcon(iconId, (int)textBoundsPosition.x, (int)(textBounds.y + textBounds.height/2 - RAYGUI_ICON_SIZE*guiIconScale/2 + TEXT_VALIGN_PIXEL_OFFSET(textBounds.height)), guiIconScale, tint);
            textBoundsPosition.x += (RAYGUI_ICON_SIZE*guiIconScale + ICON_TEXT_PADDING);
        }
#endif
        // Get size in bytes of text,
        // considering end of line and line break
        int lineSize = 0;
        for (int c = 0; (lines[i][c] != '\0') && (lines[i][c] != '\n') && (lines[i][c] != '\r'); c++, lineSize++){ }
        float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE)/guiFont.baseSize;

        int textOffsetY = 0;
        float textOffsetX = 0.0f;
        float glyphWidth = 0;
        for (int c = 0, codepointSize = 0; c < lineSize; c += codepointSize)
        {
            int codepoint = GetCodepointNext(&lines[i][c], &codepointSize);
            int index = GetGlyphIndex(guiFont, codepoint);

            // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
            // but we need to draw all of the bad bytes using the '?' symbol moving one byte
            if (codepoint == 0x3f) codepointSize = 1;       // TODO: Review not recognized codepoints size

            // Wrap mode text measuring to space to validate if it can be drawn or
            // a new line is required
            if (wrapMode == TEXT_WRAP_CHAR)
            {
                // Get glyph width to check if it goes out of bounds
                if (guiFont.glyphs[index].advanceX == 0) glyphWidth = ((float)guiFont.recs[index].width*scaleFactor);
                else glyphWidth = (float)guiFont.glyphs[index].advanceX*scaleFactor;

                // Jump to next line if current character reach end of the box limits
                if ((textOffsetX + glyphWidth) > textBounds.width)
                {
                    textOffsetX = 0.0f;
                    textOffsetY += GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);
                }
            }
            else if (wrapMode == TEXT_WRAP_WORD)
            {
                // Get width to next space in line
                int nextSpaceIndex = 0;
                float nextSpaceWidth = GetNextSpaceWidth(lines[i] + c, &nextSpaceIndex);

                if ((textOffsetX + nextSpaceWidth) > textBounds.width)
                {
                    textOffsetX = 0.0f;
                    textOffsetY += GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);
                }

                // TODO: Consider case: (nextSpaceWidth >= textBounds.width)
            }

            if (codepoint == '\n') break;   // WARNING: Lines are already processed manually, no need to keep drawing after this codepoint
            else
            {
                // TODO: There are multiple types of spaces in Unicode,
                // maybe it's a good idea to add support for more: http://jkorpela.fi/chars/spaces.html
                if ((codepoint != ' ') && (codepoint != '\t'))      // Do not draw codepoints with no glyph
                {
                    if (wrapMode == TEXT_WRAP_NONE)
                    {
                        // Draw only required text glyphs fitting the textBounds.width
                        if (textOffsetX <= (textBounds.width - glyphWidth))
                        {
                            DrawTextCodepoint(guiFont, codepoint, RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                        }
                    }
                    else if ((wrapMode == TEXT_WRAP_CHAR) || (wrapMode == TEXT_WRAP_WORD))
                    {
                        // Draw only glyphs inside the bounds
                        if ((textBoundsPosition.y + textOffsetY) <= (textBounds.y + textBounds.height - GuiGetStyle(DEFAULT, TEXT_SIZE)))
                        {
                            DrawTextCodepoint(guiFont, codepoint, RAYGUI_CLITERAL(Vector2){ textBoundsPosition.x + textOffsetX, textBoundsPosition.y + textOffsetY }, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), GuiFade(tint, guiAlpha));
                        }
                    }
                }

                if (guiFont.glyphs[index].advanceX == 0) textOffsetX += ((float)guiFont.recs[index].width*scaleFactor + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
                else textOffsetX += ((float)guiFont.glyphs[index].advanceX*scaleFactor + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
            }
        }

        if (wrapMode == TEXT_WRAP_NONE) posOffsetY += (float)GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);
        else if ((wrapMode == TEXT_WRAP_CHAR) || (wrapMode == TEXT_WRAP_WORD)) posOffsetY += (textOffsetY + (float)GuiGetStyle(DEFAULT, TEXT_LINE_SPACING));
        //---------------------------------------------------------------------------------
    }

#if defined(RAYGUI_DEBUG_TEXT_BOUNDS)
    GuiDrawRectangle(textBounds, 0, WHITE, Fade(BLUE, 0.4f));
#endif
}

// Gui draw rectangle using default raygui plain style with borders
void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color)
{
    if (color.a > 0)
    {
        // Draw rectangle filled with color
        DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, GuiFade(color, guiAlpha));
    }

    if (borderWidth > 0)
    {
        // Draw rectangle border lines with color
        DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x + (int)rec.width - borderWidth, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2*borderWidth, GuiFade(borderColor, guiAlpha));
        DrawRectangle((int)rec.x, (int)rec.y + (int)rec.height - borderWidth, (int)rec.width, borderWidth, GuiFade(borderColor, guiAlpha));
    }

#if defined(RAYGUI_DEBUG_RECS_BOUNDS)
    DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, Fade(RED, 0.4f));
#endif
}

// Draw tooltip using control bounds
void GuiTooltip(Rectangle controlRec)
{
    if (!guiLocked && guiTooltip && (guiTooltipPtr != NULL) && !guiSliderDragging)
    {
        Vector2 textSize = MeasureTextEx(GuiGetFont(), guiTooltipPtr, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING));

        if ((controlRec.x + textSize.x + 16) > GetScreenWidth()) controlRec.x -= (textSize.x + 16 - controlRec.width);

        GuiPanel(RAYGUI_CLITERAL(Rectangle){ controlRec.x, controlRec.y + controlRec.height + 4, textSize.x + 16, GuiGetStyle(DEFAULT, TEXT_SIZE) + 8.f }, NULL);

        int textPadding = GuiGetStyle(LABEL, TEXT_PADDING);
        int textAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_PADDING, 0);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiLabel(RAYGUI_CLITERAL(Rectangle){ controlRec.x, controlRec.y + controlRec.height + 4, textSize.x + 16, GuiGetStyle(DEFAULT, TEXT_SIZE) + 8.f }, guiTooltipPtr);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, textAlignment);
        GuiSetStyle(LABEL, TEXT_PADDING, textPadding);
    }
}

// Split controls text into multiple strings
// Also check for multiple columns (required by GuiToggleGroup())
const char **GuiTextSplit(const char *text, char delimiter, int *count, int *textRow)
{
    // NOTE: Current implementation returns a copy of the provided string with '\0' (string end delimiter)
    // inserted between strings defined by "delimiter" parameter. No memory is dynamically allocated,
    // all used memory is static... it has some limitations:
    //      1. Maximum number of possible split strings is set by RAYGUI_TEXTSPLIT_MAX_ITEMS
    //      2. Maximum size of text to split is RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE
    // NOTE: Those definitions could be externally provided if required

    // TODO: HACK: GuiTextSplit() - Review how textRows are returned to user
    // textRow is an externally provided array of integers that stores row number for every splitted string

    #if !defined(RAYGUI_TEXTSPLIT_MAX_ITEMS)
        #define RAYGUI_TEXTSPLIT_MAX_ITEMS          128
    #endif
    #if !defined(RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE)
        #define RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE     1024
    #endif

    static const char *result[RAYGUI_TEXTSPLIT_MAX_ITEMS] = { NULL };   // String pointers array (points to buffer data)
    static char buffer[RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE] = { 0 };         // Buffer data (text input copy with '\0' added)
    memset(buffer, 0, RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE);

    result[0] = buffer;
    int counter = 1;

    if (textRow != NULL) textRow[0] = 0;

    // Count how many substrings we have on text and point to every one
    for (int i = 0; i < RAYGUI_TEXTSPLIT_MAX_TEXT_SIZE; i++)
    {
        buffer[i] = text[i];
        if (buffer[i] == '\0') break;
        else if ((buffer[i] == delimiter) || (buffer[i] == '\n'))
        {
            result[counter] = buffer + i + 1;

            if (textRow != NULL)
            {
                if (buffer[i] == '\n') textRow[counter] = textRow[counter - 1] + 1;
                else textRow[counter] = textRow[counter - 1];
            }

            buffer[i] = '\0';   // Set an end of string at this point

            counter++;
            if (counter == RAYGUI_TEXTSPLIT_MAX_ITEMS) break;
        }
    }

    *count = counter;

    return result;
}

// Convert color data from RGB to HSV
// NOTE: Color data should be passed normalized
Vector3 ConvertRGBtoHSV(Vector3 rgb)
{
    Vector3 hsv = { 0 };
    float min = 0.0f;
    float max = 0.0f;
    float delta = 0.0f;

    min = (rgb.x < rgb.y)? rgb.x : rgb.y;
    min = (min < rgb.z)? min  : rgb.z;

    max = (rgb.x > rgb.y)? rgb.x : rgb.y;
    max = (max > rgb.z)? max  : rgb.z;

    hsv.z = max;            // Value
    delta = max - min;

    if (delta < 0.00001f)
    {
        hsv.y = 0.0f;
        hsv.x = 0.0f;           // Undefined, maybe NAN?
        return hsv;
    }

    if (max > 0.0f)
    {
        // NOTE: If max is 0, this divide would cause a crash
        hsv.y = (delta/max);    // Saturation
    }
    else
    {
        // NOTE: If max is 0, then r = g = b = 0, s = 0, h is undefined
        hsv.y = 0.0f;
        hsv.x = 0.0f;           // Undefined, maybe NAN?
        return hsv;
    }

    // NOTE: Comparing float values could not work properly
    if (rgb.x >= max) hsv.x = (rgb.y - rgb.z)/delta;    // Between yellow & magenta
    else
    {
        if (rgb.y >= max) hsv.x = 2.0f + (rgb.z - rgb.x)/delta;  // Between cyan & yellow
        else hsv.x = 4.0f + (rgb.x - rgb.y)/delta;      // Between magenta & cyan
    }

    hsv.x *= 60.0f;     // Convert to degrees

    if (hsv.x < 0.0f) hsv.x += 360.0f;

    return hsv;
}

// Convert color data from HSV to RGB
// NOTE: Color data should be passed normalized
Vector3 ConvertHSVtoRGB(Vector3 hsv)
{
    Vector3 rgb = { 0 };
    float hh = 0.0f, p = 0.0f, q = 0.0f, t = 0.0f, ff = 0.0f;
    long i = 0;

    // NOTE: Comparing float values could not work properly
    if (hsv.y <= 0.0f)
    {
        rgb.x = hsv.z;
        rgb.y = hsv.z;
        rgb.z = hsv.z;
        return rgb;
    }

    hh = hsv.x;
    if (hh >= 360.0f) hh = 0.0f;
    hh /= 60.0f;

    i = (long)hh;
    ff = hh - i;
    p = hsv.z*(1.0f - hsv.y);
    q = hsv.z*(1.0f - (hsv.y*ff));
    t = hsv.z*(1.0f - (hsv.y*(1.0f - ff)));

    switch (i)
    {
        case 0:
        {
            rgb.x = hsv.z;
            rgb.y = t;
            rgb.z = p;
        } break;
        case 1:
        {
            rgb.x = q;
            rgb.y = hsv.z;
            rgb.z = p;
        } break;
        case 2:
        {
            rgb.x = p;
            rgb.y = hsv.z;
            rgb.z = t;
        } break;
        case 3:
        {
            rgb.x = p;
            rgb.y = q;
            rgb.z = hsv.z;
        } break;
        case 4:
        {
            rgb.x = t;
            rgb.y = p;
            rgb.z = hsv.z;
        } break;
        case 5:
        default:
        {
            rgb.x = hsv.z;
            rgb.y = p;
            rgb.z = q;
        } break;
    }

    return rgb;
}

// Scroll bar control (used by GuiScrollPanel())
int GuiScrollBar(Rectangle bounds, int value, int minValue, int maxValue)
{
    GuiState state = guiState;

    // Is the scrollbar horizontal or vertical?
    bool isVertical = (bounds.width > bounds.height)? false : true;

    // The size (width or height depending on scrollbar type) of the spinner buttons
    const int spinnerSize = GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE)?
        (isVertical? (int)bounds.width - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH) :
        (int)bounds.height - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH)) : 0;

    // Arrow buttons [<] [>] [∧] [∨]
    Rectangle arrowUpLeft = { 0 };
    Rectangle arrowDownRight = { 0 };

    // Actual area of the scrollbar excluding the arrow buttons
    Rectangle scrollbar = { 0 };

    // Slider bar that moves     --[///]-----
    Rectangle slider = { 0 };

    // Normalize value
    if (value > maxValue) value = maxValue;
    if (value < minValue) value = minValue;

    const int valueRange = maxValue - minValue;
    int sliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);

    // Calculate rectangles for all of the components
    arrowUpLeft = RAYGUI_CLITERAL(Rectangle){
        (float)bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH),
        (float)bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH),
        (float)spinnerSize, (float)spinnerSize };

    if (isVertical)
    {
        arrowDownRight = RAYGUI_CLITERAL(Rectangle){ (float)bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)bounds.y + bounds.height - spinnerSize - GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
        scrollbar = RAYGUI_CLITERAL(Rectangle){ bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), arrowUpLeft.y + arrowUpLeft.height, bounds.width - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)), bounds.height - arrowUpLeft.height - arrowDownRight.height - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH) };

        // Make sure the slider won't get outside of the scrollbar
        sliderSize = (sliderSize >= scrollbar.height)? ((int)scrollbar.height - 2) : sliderSize;
        slider = RAYGUI_CLITERAL(Rectangle){
            bounds.x + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING),
            scrollbar.y + (int)(((float)(value - minValue)/valueRange)*(scrollbar.height - sliderSize)),
            bounds.width - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)),
            (float)sliderSize };
    }
    else    // horizontal
    {
        arrowDownRight = RAYGUI_CLITERAL(Rectangle){ (float)bounds.x + bounds.width - spinnerSize - GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH), (float)spinnerSize, (float)spinnerSize };
        scrollbar = RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x + arrowUpLeft.width, bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING), bounds.width - arrowUpLeft.width - arrowDownRight.width - 2*GuiGetStyle(SCROLLBAR, BORDER_WIDTH), bounds.height - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_PADDING)) };

        // Make sure the slider won't get outside of the scrollbar
        sliderSize = (sliderSize >= scrollbar.width)? ((int)scrollbar.width - 2) : sliderSize;
        slider = RAYGUI_CLITERAL(Rectangle){
            scrollbar.x + (int)(((float)(value - minValue)/valueRange)*(scrollbar.width - sliderSize)),
            bounds.y + GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING),
            (float)sliderSize,
            bounds.height - 2*(GuiGetStyle(SCROLLBAR, BORDER_WIDTH) + GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_PADDING)) };
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        if (guiSliderDragging) // Keep dragging outside of bounds
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
                !CheckCollisionPointRec(mousePoint, arrowUpLeft) &&
                !CheckCollisionPointRec(mousePoint, arrowDownRight))
            {
                if (CHECK_BOUNDS_ID(bounds, guiSliderActive))
                {
                    state = STATE_PRESSED;

                    if (isVertical) value = (int)(((float)(mousePoint.y - scrollbar.y - slider.height/2)*valueRange)/(scrollbar.height - slider.height) + minValue);
                    else value = (int)(((float)(mousePoint.x - scrollbar.x - slider.width/2)*valueRange)/(scrollbar.width - slider.width) + minValue);
                }
            }
            else
            {
                guiSliderDragging = false;
                guiSliderActive = RAYGUI_CLITERAL(Rectangle){ 0, 0, 0, 0 };
            }
        }
        else if (CheckCollisionPointRec(mousePoint, bounds))
        {
            state = STATE_FOCUSED;

            // Handle mouse wheel
            int wheel = (int)GetMouseWheelMove();
            if (wheel != 0) value += wheel;

            // Handle mouse button down
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                guiSliderDragging = true;
                guiSliderActive = bounds; // Store bounds as an identifier when dragging starts

                // Check arrows click
                if (CheckCollisionPointRec(mousePoint, arrowUpLeft)) value -= valueRange/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                else if (CheckCollisionPointRec(mousePoint, arrowDownRight)) value += valueRange/GuiGetStyle(SCROLLBAR, SCROLL_SPEED);
                else if (!CheckCollisionPointRec(mousePoint, slider))
                {
                    // If click on scrollbar position but not on slider, place slider directly on that position
                    if (isVertical) value = (int)(((float)(mousePoint.y - scrollbar.y - slider.height/2)*valueRange)/(scrollbar.height - slider.height) + minValue);
                    else value = (int)(((float)(mousePoint.x - scrollbar.x - slider.width/2)*valueRange)/(scrollbar.width - slider.width) + minValue);
                }

                state = STATE_PRESSED;
            }

            // Keyboard control on mouse hover scrollbar
            /*
            if (isVertical)
            {
                if (IsKeyDown(KEY_DOWN)) value += 5;
                else if (IsKeyDown(KEY_UP)) value -= 5;
            }
            else
            {
                if (IsKeyDown(KEY_RIGHT)) value += 5;
                else if (IsKeyDown(KEY_LEFT)) value -= 5;
            }
            */
        }

        // Normalize value
        if (value > maxValue) value = maxValue;
        if (value < minValue) value = minValue;
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(SCROLLBAR, BORDER_WIDTH), GetColor(GuiGetStyle(LISTVIEW, BORDER + state*3)), GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED)));   // Draw the background

    GuiDrawRectangle(scrollbar, 0, BLANK, GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL)));     // Draw the scrollbar active area background
    GuiDrawRectangle(slider, 0, BLANK, GetColor(GuiGetStyle(SLIDER, BORDER + state*3)));         // Draw the slider bar

    // Draw arrows (using icon if available)
    if (GuiGetStyle(SCROLLBAR, ARROWS_VISIBLE))
    {
#if defined(RAYGUI_NO_ICONS)
        GuiDrawText(isVertical? "^" : "<",
            RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x, arrowUpLeft.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
        GuiDrawText(isVertical? "v" : ">",
            RAYGUI_CLITERAL(Rectangle){ arrowDownRight.x, arrowDownRight.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))));
#else
        GuiDrawText(isVertical? "#121#" : "#118#",
            RAYGUI_CLITERAL(Rectangle){ arrowUpLeft.x, arrowUpLeft.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)));   // ICON_ARROW_UP_FILL / ICON_ARROW_LEFT_FILL
        GuiDrawText(isVertical? "#120#" : "#119#",
            RAYGUI_CLITERAL(Rectangle){ arrowDownRight.x, arrowDownRight.y, isVertical? bounds.width : bounds.height, isVertical? bounds.width : bounds.height },
            TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(SCROLLBAR, TEXT + state*3)));   // ICON_ARROW_DOWN_FILL / ICON_ARROW_RIGHT_FILL
#endif
    }
    //--------------------------------------------------------------------

    return value;
}

// Color fade-in or fade-out, alpha goes from 0.0f to 1.0f
// WARNING: It multiplies current alpha by alpha scale factor
Color GuiFade(Color color, float alpha)
{
    if (alpha < 0.0f) alpha = 0.0f;
    else if (alpha > 1.0f) alpha = 1.0f;

    Color result = { color.r, color.g, color.b, (unsigned char)(color.a*alpha) };

    return result;
}
