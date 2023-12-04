/*******************************************************************************************
*
*   raygui v4.0 - A simple and easy-to-use immediate-mode gui library
*
*   DESCRIPTION:
*       raygui is a tools-dev-focused immediate-mode-gui library based on raylib but also
*       available as a standalone library, as long as input and drawing functions are provided.
*
*   FEATURES:
*       - Immediate-mode gui, minimal retained data
*       - +25 controls provided (basic and advanced)
*       - Styling system for colors, font and metrics
*       - Icons supported, embedded as a 1-bit icons pack
*       - Standalone mode option (custom input/graphics backend)
*       - Multiple support tools provided for raygui development
*
*   POSSIBLE IMPROVEMENTS:
*       - Better standalone mode API for easy plug of custom backends
*       - Externalize required inputs, allow user easier customization
*
*   LIMITATIONS:
*       - No editable multi-line word-wraped text box supported
*       - No auto-layout mechanism, up to the user to define controls position and size
*       - Standalone mode requires library modification and some user work to plug another backend
*
*   NOTES:
*       - WARNING: GuiLoadStyle() and GuiLoadStyle{Custom}() functions, allocate memory for
*         font atlas recs and glyphs, freeing that memory is (usually) up to the user,
*         no unload function is explicitly provided... but note that GuiLoadStyleDefaulf() unloads
*         by default any previously loaded font (texture, recs, glyphs).
*       - Global UI alpha (guiAlpha) is applied inside GuiDrawRectangle() and GuiDrawText() functions
*
*   CONTROLS PROVIDED:
*     # Container/separators Controls
*       - WindowBox     --> StatusBar, Panel
*       - GroupBox      --> Line
*       - Line
*       - Panel         --> StatusBar
*       - ScrollPanel   --> StatusBar
*       - TabBar        --> Button
*
*     # Basic Controls
*       - Label
*       - LabelButton   --> Label
*       - Button
*       - Toggle
*       - ToggleGroup   --> Toggle
*       - ToggleSlider
*       - CheckBox
*       - ComboBox
*       - DropdownBox
*       - TextBox
*       - ValueBox      --> TextBox
*       - Spinner       --> Button, ValueBox
*       - Slider
*       - SliderBar     --> Slider
*       - ProgressBar
*       - StatusBar
*       - DummyRec
*       - Grid
*
*     # Advance Controls
*       - ListView
*       - ColorPicker   --> ColorPanel, ColorBarHue
*       - MessageBox    --> Window, Label, Button
*       - TextInputBox  --> Window, Label, TextBox, Button
*
*     It also provides a set of functions for styling the controls based on its properties (size, color).
*
*
*   RAYGUI STYLE (guiStyle):
*       raygui uses a global data array for all gui style properties (allocated on data segment by default),
*       when a new style is loaded, it is loaded over the global style... but a default gui style could always be
*       recovered with GuiLoadStyleDefault() function, that overwrites the current style to the default one
*
*       The global style array size is fixed and depends on the number of controls and properties:
*
*           static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)];
*
*       guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
*
*       Note that the first set of BASE properties (by default guiStyle[0..15]) belong to the generic style
*       used for all controls, when any of those base values is set, it is automatically populated to all
*       controls, so, specific control values overwriting generic style should be set after base values.
*
*       After the first BASE set we have the EXTENDED properties (by default guiStyle[16..23]), those
*       properties are actually common to all controls and can not be overwritten individually (like BASE ones)
*       Some of those properties are: TEXT_SIZE, TEXT_SPACING, LINE_COLOR, BACKGROUND_COLOR
*
*       Custom control properties can be defined using the EXTENDED properties for each independent control.
*
*       TOOL: rGuiStyler is a visual tool to customize raygui style: github.com/raysan5/rguistyler
*
*
*   RAYGUI ICONS (guiIcons):
*       raygui could use a global array containing icons data (allocated on data segment by default),
*       a custom icons set could be loaded over this array using GuiLoadIcons(), but loaded icons set
*       must be same RAYGUI_ICON_SIZE and no more than RAYGUI_ICON_MAX_ICONS will be loaded
*
*       Every icon is codified in binary form, using 1 bit per pixel, so, every 16x16 icon
*       requires 8 integers (16*16/32) to be stored in memory.
*
*       When the icon is draw, actually one quad per pixel is drawn if the bit for that pixel is set.
*
*       The global icons array size is fixed and depends on the number of icons and size:
*
*           static unsigned int guiIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS];
*
*       guiIcons size is by default: 256*(16*16/32) = 2048*4 = 8192 bytes = 8 KB
*
*       TOOL: rGuiIcons is a visual tool to customize/create raygui icons: github.com/raysan5/rguiicons
*
*   RAYGUI LAYOUT:
*       raygui currently does not provide an auto-layout mechanism like other libraries,
*       layouts must be defined manually on controls drawing, providing the right bounds Rectangle for it.
*
*       TOOL: rGuiLayout is a visual tool to create raygui layouts: github.com/raysan5/rguilayout
*
*   CONFIGURATION:
*       #define RAYGUI_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RAYGUI_STANDALONE
*           Avoid raylib.h header inclusion in this file. Data types defined on raylib are defined
*           internally in the library and input management and drawing functions must be provided by
*           the user (check library implementation for further details).
*
*       #define RAYGUI_NO_ICONS
*           Avoid including embedded ricons data (256 icons, 16x16 pixels, 1-bit per pixel, 2KB)
*
*       #define RAYGUI_CUSTOM_ICONS
*           Includes custom ricons.h header defining a set of custom icons,
*           this file can be generated using rGuiIcons tool
*
*       #define RAYGUI_DEBUG_RECS_BOUNDS
*           Draw control bounds rectangles for debug
*
*       #define RAYGUI_DEBUG_TEXT_BOUNDS
*           Draw text bounds rectangles for debug
*
*   VERSIONS HISTORY:
*       4.0 (12-Sep-2023) ADDED: GuiToggleSlider()
*                         ADDED: GuiColorPickerHSV() and GuiColorPanelHSV()
*                         ADDED: Multiple new icons, mostly compiler related
*                         ADDED: New DEFAULT properties: TEXT_LINE_SPACING, TEXT_ALIGNMENT_VERTICAL, TEXT_WRAP_MODE
*                         ADDED: New enum values: GuiTextAlignment, GuiTextAlignmentVertical, GuiTextWrapMode
*                         ADDED: Support loading styles with custom font charset from external file
*                         REDESIGNED: GuiTextBox(), support mouse cursor positioning
*                         REDESIGNED: GuiDrawText(), support multiline and word-wrap modes (read only)
*                         REDESIGNED: GuiProgressBar() to be more visual, progress affects border color
*                         REDESIGNED: Global alpha consideration moved to GuiDrawRectangle() and GuiDrawText()
*                         REDESIGNED: GuiScrollPanel(), get parameters by reference and return result value
*                         REDESIGNED: GuiToggleGroup(), get parameters by reference and return result value
*                         REDESIGNED: GuiComboBox(), get parameters by reference and return result value
*                         REDESIGNED: GuiCheckBox(), get parameters by reference and return result value
*                         REDESIGNED: GuiSlider(), get parameters by reference and return result value
*                         REDESIGNED: GuiSliderBar(), get parameters by reference and return result value
*                         REDESIGNED: GuiProgressBar(), get parameters by reference and return result value
*                         REDESIGNED: GuiListView(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorPicker(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorPanel(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorBarAlpha(), get parameters by reference and return result value
*                         REDESIGNED: GuiColorBarHue(), get parameters by reference and return result value
*                         REDESIGNED: GuiGrid(), get parameters by reference and return result value
*                         REDESIGNED: GuiGrid(), added extra parameter
*                         REDESIGNED: GuiListViewEx(), change parameters order
*                         REDESIGNED: All controls return result as int value
*                         REVIEWED: GuiScrollPanel() to avoid smallish scroll-bars
*                         REVIEWED: All examples and specially controls_test_suite
*                         RENAMED: gui_file_dialog module to gui_window_file_dialog
*                         UPDATED: All styles to include ISO-8859-15 charset (as much as possible)
*
*       3.6 (10-May-2023) ADDED: New icon: SAND_TIMER
*                         ADDED: GuiLoadStyleFromMemory() (binary only)
*                         REVIEWED: GuiScrollBar() horizontal movement key
*                         REVIEWED: GuiTextBox() crash on cursor movement
*                         REVIEWED: GuiTextBox(), additional inputs support
*                         REVIEWED: GuiLabelButton(), avoid text cut
*                         REVIEWED: GuiTextInputBox(), password input
*                         REVIEWED: Local GetCodepointNext(), aligned with raylib
*                         REDESIGNED: GuiSlider*()/GuiScrollBar() to support out-of-bounds
*
*       3.5 (20-Apr-2023) ADDED: GuiTabBar(), based on GuiToggle()
*                         ADDED: Helper functions to split text in separate lines
*                         ADDED: Multiple new icons, useful for code editing tools
*                         REMOVED: Unneeded icon editing functions
*                         REMOVED: GuiTextBoxMulti(), very limited and broken
*                         REMOVED: MeasureTextEx() dependency, logic directly implemented
*                         REMOVED: DrawTextEx() dependency, logic directly implemented
*                         REVIEWED: GuiScrollBar(), improve mouse-click behaviour
*                         REVIEWED: Library header info, more info, better organized
*                         REDESIGNED: GuiTextBox() to support cursor movement
*                         REDESIGNED: GuiDrawText() to divide drawing by lines
*
*       3.2 (22-May-2022) RENAMED: Some enum values, for unification, avoiding prefixes
*                         REMOVED: GuiScrollBar(), only internal
*                         REDESIGNED: GuiPanel() to support text parameter
*                         REDESIGNED: GuiScrollPanel() to support text parameter
*                         REDESIGNED: GuiColorPicker() to support text parameter
*                         REDESIGNED: GuiColorPanel() to support text parameter
*                         REDESIGNED: GuiColorBarAlpha() to support text parameter
*                         REDESIGNED: GuiColorBarHue() to support text parameter
*                         REDESIGNED: GuiTextInputBox() to support password
*
*       3.1 (12-Jan-2022) REVIEWED: Default style for consistency (aligned with rGuiLayout v2.5 tool)
*                         REVIEWED: GuiLoadStyle() to support compressed font atlas image data and unload previous textures
*                         REVIEWED: External icons usage logic
*                         REVIEWED: GuiLine() for centered alignment when including text
*                         RENAMED: Multiple controls properties definitions to prepend RAYGUI_
*                         RENAMED: RICON_ references to RAYGUI_ICON_ for library consistency
*                         Projects updated and multiple tweaks
*
*       3.0 (04-Nov-2021) Integrated ricons data to avoid external file
*                         REDESIGNED: GuiTextBoxMulti()
*                         REMOVED: GuiImageButton*()
*                         Multiple minor tweaks and bugs corrected
*
*       2.9 (17-Mar-2021) REMOVED: Tooltip API
*       2.8 (03-May-2020) Centralized rectangles drawing to GuiDrawRectangle()
*       2.7 (20-Feb-2020) ADDED: Possible tooltips API
*       2.6 (09-Sep-2019) ADDED: GuiTextInputBox()
*                         REDESIGNED: GuiListView*(), GuiDropdownBox(), GuiSlider*(), GuiProgressBar(), GuiMessageBox()
*                         REVIEWED: GuiTextBox(), GuiSpinner(), GuiValueBox(), GuiLoadStyle()
*                         Replaced property INNER_PADDING by TEXT_PADDING, renamed some properties
*                         ADDED: 8 new custom styles ready to use
*                         Multiple minor tweaks and bugs corrected
*
*       2.5 (28-May-2019) Implemented extended GuiTextBox(), GuiValueBox(), GuiSpinner()
*       2.3 (29-Apr-2019) ADDED: rIcons auxiliar library and support for it, multiple controls reviewed
*                         Refactor all controls drawing mechanism to use control state
*       2.2 (05-Feb-2019) ADDED: GuiScrollBar(), GuiScrollPanel(), reviewed GuiListView(), removed Gui*Ex() controls
*       2.1 (26-Dec-2018) REDESIGNED: GuiCheckBox(), GuiComboBox(), GuiDropdownBox(), GuiToggleGroup() > Use combined text string
*                         REDESIGNED: Style system (breaking change)
*       2.0 (08-Nov-2018) ADDED: Support controls guiLock and custom fonts
*                         REVIEWED: GuiComboBox(), GuiListView()...
*       1.9 (09-Oct-2018) REVIEWED: GuiGrid(), GuiTextBox(), GuiTextBoxMulti(), GuiValueBox()...
*       1.8 (01-May-2018) Lot of rework and redesign to align with rGuiStyler and rGuiLayout
*       1.5 (21-Jun-2017) Working in an improved styles system
*       1.4 (15-Jun-2017) Rewritten all GUI functions (removed useless ones)
*       1.3 (12-Jun-2017) Complete redesign of style system
*       1.1 (01-Jun-2017) Complete review of the library
*       1.0 (07-Jun-2016) Converted to header-only by Ramon Santamaria.
*       0.9 (07-Mar-2016) Reviewed and tested by Albert Martos, Ian Eito, Sergio Martinez and Ramon Santamaria.
*       0.8 (27-Aug-2015) Initial release. Implemented by Kevin Gato, Daniel Nicolás and Ramon Santamaria.
*
*   DEPENDENCIES:
*       raylib 4.6-dev      Inputs reading (keyboard/mouse), shapes drawing, font loading and text drawing
*
*   STANDALONE MODE:
*       By default raygui depends on raylib mostly for the inputs and the drawing functionality but that dependency can be disabled
*       with the config flag RAYGUI_STANDALONE. In that case is up to the user to provide another backend to cover library needs.
*
*       The following functions should be redefined for a custom backend:
*
*           - Vector2 GetMousePosition(void);
*           - float GetMouseWheelMove(void);
*           - bool IsMouseButtonDown(int button);
*           - bool IsMouseButtonPressed(int button);
*           - bool IsMouseButtonReleased(int button);
*           - bool IsKeyDown(int key);
*           - bool IsKeyPressed(int key);
*           - int GetCharPressed(void);         // -- GuiTextBox(), GuiValueBox()
*
*           - void DrawRectangle(int x, int y, int width, int height, Color color); // -- GuiDrawRectangle()
*           - void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4); // -- GuiColorPicker()
*
*           - Font GetFontDefault(void);                            // -- GuiLoadStyleDefault()
*           - Font LoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount); // -- GuiLoadStyle()
*           - Texture2D LoadTextureFromImage(Image image);          // -- GuiLoadStyle(), required to load texture from embedded font atlas image
*           - void SetShapesTexture(Texture2D tex, Rectangle rec);  // -- GuiLoadStyle(), required to set shapes rec to font white rec (optimization)
*           - char *LoadFileText(const char *fileName);             // -- GuiLoadStyle(), required to load charset data
*           - void UnloadFileText(char *text);                      // -- GuiLoadStyle(), required to unload charset data
*           - const char *GetDirectoryPath(const char *filePath);   // -- GuiLoadStyle(), required to find charset/font file from text .rgs
*           - int *LoadCodepoints(const char *text, int *count);    // -- GuiLoadStyle(), required to load required font codepoints list
*           - void UnloadCodepoints(int *codepoints);               // -- GuiLoadStyle(), required to unload codepoints list
*           - unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize); // -- GuiLoadStyle()
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Supervision, review, redesign, update and maintenance
*       Vlad Adrian:        Complete rewrite of GuiTextBox() to support extended features (2019)
*       Sergio Martinez:    Review, testing (2015) and redesign of multiple controls (2018)
*       Adria Arranz:       Testing and implementation of additional controls (2018)
*       Jordi Jorba:        Testing and implementation of additional controls (2018)
*       Albert Martos:      Review and testing of the library (2015)
*       Ian Eito:           Review and testing of the library (2015)
*       Kevin Gato:         Initial implementation of basic components (2014)
*       Daniel Nicolas:     Initial implementation of basic components (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYGUI_H
#define RAYGUI_H

#define RAYGUI_VERSION_MAJOR 4
#define RAYGUI_VERSION_MINOR 0
#define RAYGUI_VERSION_PATCH 0
#define RAYGUI_VERSION  "4.0"

#if !defined(RAYGUI_STANDALONE)
    #include "raylib.h"
#endif

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RAYGUIAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RAYGUIAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

// Function specifiers definition
#ifndef RAYGUIAPI
    #define RAYGUIAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Allow custom memory allocators
#ifndef RAYGUI_MALLOC
    #define RAYGUI_MALLOC(sz)       malloc(sz)
#endif
#ifndef RAYGUI_CALLOC
    #define RAYGUI_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RAYGUI_FREE
    #define RAYGUI_FREE(p)          free(p)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define RAYGUI_SUPPORT_LOG_INFO
#if defined(RAYGUI_SUPPORT_LOG_INFO)
  #define RAYGUI_LOG(...)           printf(__VA_ARGS__)
#else
  #define RAYGUI_LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Some types are required for RAYGUI_STANDALONE usage
//----------------------------------------------------------------------------------
#if defined(RAYGUI_STANDALONE)
    #ifndef __cplusplus
    // Boolean type
        #ifndef true
            typedef enum { false, true } bool;
        #endif
    #endif

    // Vector2 type
    typedef struct Vector2 {
        float x;
        float y;
    } Vector2;

    // Vector3 type                 // -- ConvertHSVtoRGB(), ConvertRGBtoHSV()
    typedef struct Vector3 {
        float x;
        float y;
        float z;
    } Vector3;

    // Color type, RGBA (32bit)
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;

    // Rectangle type
    typedef struct Rectangle {
        float x;
        float y;
        float width;
        float height;
    } Rectangle;

    // TODO: Texture2D type is very coupled to raylib, required by Font type
    // It should be redesigned to be provided by user
    typedef struct Texture2D {
        unsigned int id;        // OpenGL texture id
        int width;              // Texture base width
        int height;             // Texture base height
        int mipmaps;            // Mipmap levels, 1 by default
        int format;             // Data format (PixelFormat type)
    } Texture2D;

    // Image, pixel data stored in CPU memory (RAM)
    typedef struct Image {
        void *data;             // Image raw data
        int width;              // Image base width
        int height;             // Image base height
        int mipmaps;            // Mipmap levels, 1 by default
        int format;             // Data format (PixelFormat type)
    } Image;

    // GlyphInfo, font characters glyphs info
    typedef struct GlyphInfo {
        int value;              // Character value (Unicode)
        int offsetX;            // Character offset X when drawing
        int offsetY;            // Character offset Y when drawing
        int advanceX;           // Character advance position X
        Image image;            // Character image data
    } GlyphInfo;

    // TODO: Font type is very coupled to raylib, mostly required by GuiLoadStyle()
    // It should be redesigned to be provided by user
    typedef struct Font {
        int baseSize;           // Base size (default chars height)
        int glyphCount;         // Number of glyph characters
        int glyphPadding;       // Padding around the glyph characters
        Texture2D texture;      // Texture atlas containing the glyphs
        Rectangle *recs;        // Rectangles in texture for the glyphs
        GlyphInfo *glyphs;      // Glyphs info data
    } Font;
#endif


// Style property
// NOTE: Used when exporting style as code for convenience
typedef struct GuiStyleProp {
    unsigned short controlId;   // Control identifier
    unsigned short propertyId;  // Property identifier
    int propertyValue;          // Property value
} GuiStyleProp;

/*
// Controls text style -NOT USED-
// NOTE: Text style is defined by control
typedef struct GuiTextStyle {
    unsigned int size;
    int charSpacing;
    int lineSpacing;
    int alignmentH;
    int alignmentV;
    int padding;
} GuiTextStyle;
*/

// Gui control state
typedef enum {
    STATE_NORMAL = 0,
    STATE_FOCUSED,
    STATE_PRESSED,
    STATE_DISABLED
} GuiState;

// Gui control text alignment
typedef enum {
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} GuiTextAlignment;

// Gui control text alignment vertical
// NOTE: Text vertical position inside the text bounds
typedef enum {
    TEXT_ALIGN_TOP = 0,
    TEXT_ALIGN_MIDDLE,
    TEXT_ALIGN_BOTTOM
} GuiTextAlignmentVertical;

// Gui control text wrap mode
// NOTE: Useful for multiline text
typedef enum {
    TEXT_WRAP_NONE = 0,
    TEXT_WRAP_CHAR,
    TEXT_WRAP_WORD
} GuiTextWrapMode;

// Gui controls
typedef enum {
    // Default -> populates to all controls when set
    DEFAULT = 0,

    // Basic controls
    LABEL,          // Used also for: LABELBUTTON
    BUTTON,
    TOGGLE,         // Used also for: TOGGLEGROUP
    SLIDER,         // Used also for: SLIDERBAR, TOGGLESLIDER
    PROGRESSBAR,
    CHECKBOX,
    COMBOBOX,
    DROPDOWNBOX,
    TEXTBOX,        // Used also for: TEXTBOXMULTI
    VALUEBOX,
    SPINNER,        // Uses: BUTTON, VALUEBOX
    LISTVIEW,
    COLORPICKER,
    SCROLLBAR,
    STATUSBAR
} GuiControl;

// Gui base properties for every control
// NOTE: RAYGUI_MAX_PROPS_BASE properties (by default 16 properties)
typedef enum {
    BORDER_COLOR_NORMAL = 0,    // Control border color in STATE_NORMAL
    BASE_COLOR_NORMAL,          // Control base color in STATE_NORMAL
    TEXT_COLOR_NORMAL,          // Control text color in STATE_NORMAL
    BORDER_COLOR_FOCUSED,       // Control border color in STATE_FOCUSED
    BASE_COLOR_FOCUSED,         // Control base color in STATE_FOCUSED
    TEXT_COLOR_FOCUSED,         // Control text color in STATE_FOCUSED
    BORDER_COLOR_PRESSED,       // Control border color in STATE_PRESSED
    BASE_COLOR_PRESSED,         // Control base color in STATE_PRESSED
    TEXT_COLOR_PRESSED,         // Control text color in STATE_PRESSED
    BORDER_COLOR_DISABLED,      // Control border color in STATE_DISABLED
    BASE_COLOR_DISABLED,        // Control base color in STATE_DISABLED
    TEXT_COLOR_DISABLED,        // Control text color in STATE_DISABLED
    BORDER_WIDTH,               // Control border size, 0 for no border
    //TEXT_SIZE,                  // Control text size (glyphs max height) -> GLOBAL for all controls
    //TEXT_SPACING,               // Control text spacing between glyphs -> GLOBAL for all controls
    //TEXT_LINE_SPACING           // Control text spacing between lines -> GLOBAL for all controls
    TEXT_PADDING,               // Control text padding, not considering border
    TEXT_ALIGNMENT,             // Control text horizontal alignment inside control text bound (after border and padding)
    //TEXT_WRAP_MODE              // Control text wrap-mode inside text bounds -> GLOBAL for all controls
} GuiControlProperty;

// TODO: Which text styling properties should be global or per-control?
// At this moment TEXT_PADDING and TEXT_ALIGNMENT is configured and saved per control while
// TEXT_SIZE, TEXT_SPACING, TEXT_LINE_SPACING, TEXT_ALIGNMENT_VERTICAL, TEXT_WRAP_MODE are global and
// should be configured by user as needed while defining the UI layout


// Gui extended properties depend on control
// NOTE: RAYGUI_MAX_PROPS_EXTENDED properties (by default, max 8 properties)
//----------------------------------------------------------------------------------
// DEFAULT extended properties
// NOTE: Those properties are common to all controls or global
// WARNING: We only have 8 slots for those properties by default!!! -> New global control: TEXT?
typedef enum {
    TEXT_SIZE = 16,             // Text size (glyphs max height)
    TEXT_SPACING,               // Text spacing between glyphs
    LINE_COLOR,                 // Line control color
    BACKGROUND_COLOR,           // Background color
    TEXT_LINE_SPACING,          // Text spacing between lines
    TEXT_ALIGNMENT_VERTICAL,    // Text vertical alignment inside text bounds (after border and padding)
    TEXT_WRAP_MODE              // Text wrap-mode inside text bounds
    //TEXT_DECORATION             // Text decoration: 0-None, 1-Underline, 2-Line-through, 3-Overline
    //TEXT_DECORATION_THICK       // Text decoration line thikness
} GuiDefaultProperty;

// Other possible text properties:
// TEXT_WEIGHT                  // Normal, Italic, Bold -> Requires specific font change
// TEXT_INDENT	                // Text indentation -> Now using TEXT_PADDING...

// Label
//typedef enum { } GuiLabelProperty;

// Button/Spinner
//typedef enum { } GuiButtonProperty;

// Toggle/ToggleGroup
typedef enum {
    GROUP_PADDING = 16,         // ToggleGroup separation between toggles
} GuiToggleProperty;

// Slider/SliderBar
typedef enum {
    SLIDER_WIDTH = 16,          // Slider size of internal bar
    SLIDER_PADDING              // Slider/SliderBar internal bar padding
} GuiSliderProperty;

// ProgressBar
typedef enum {
    PROGRESS_PADDING = 16,      // ProgressBar internal padding
} GuiProgressBarProperty;

// ScrollBar
typedef enum {
    ARROWS_SIZE = 16,           // ScrollBar arrows size
    ARROWS_VISIBLE,             // ScrollBar arrows visible
    SCROLL_SLIDER_PADDING,      // ScrollBar slider internal padding
    SCROLL_SLIDER_SIZE,         // ScrollBar slider size
    SCROLL_PADDING,             // ScrollBar scroll padding from arrows
    SCROLL_SPEED,               // ScrollBar scrolling speed
} GuiScrollBarProperty;

// CheckBox
typedef enum {
    CHECK_PADDING = 16          // CheckBox internal check padding
} GuiCheckBoxProperty;

// ComboBox
typedef enum {
    COMBO_BUTTON_WIDTH = 16,    // ComboBox right button width
    COMBO_BUTTON_SPACING        // ComboBox button separation
} GuiComboBoxProperty;

// DropdownBox
typedef enum {
    ARROW_PADDING = 16,         // DropdownBox arrow separation from border and items
    DROPDOWN_ITEMS_SPACING      // DropdownBox items separation
} GuiDropdownBoxProperty;

// TextBox/TextBoxMulti/ValueBox/Spinner
typedef enum {
    TEXT_READONLY = 16,         // TextBox in read-only mode: 0-text editable, 1-text no-editable
} GuiTextBoxProperty;

// Spinner
typedef enum {
    SPIN_BUTTON_WIDTH = 16,     // Spinner left/right buttons width
    SPIN_BUTTON_SPACING,        // Spinner buttons separation
} GuiSpinnerProperty;

// ListView
typedef enum {
    LIST_ITEMS_HEIGHT = 16,     // ListView items height
    LIST_ITEMS_SPACING,         // ListView items separation
    SCROLLBAR_WIDTH,            // ListView scrollbar size (usually width)
    SCROLLBAR_SIDE,             // ListView scrollbar side (0-SCROLLBAR_LEFT_SIDE, 1-SCROLLBAR_RIGHT_SIDE)
} GuiListViewProperty;

// ColorPicker
typedef enum {
    COLOR_SELECTOR_SIZE = 16,
    HUEBAR_WIDTH,               // ColorPicker right hue bar width
    HUEBAR_PADDING,             // ColorPicker right hue bar separation from panel
    HUEBAR_SELECTOR_HEIGHT,     // ColorPicker right hue bar selector height
    HUEBAR_SELECTOR_OVERFLOW    // ColorPicker right hue bar selector overflow
} GuiColorPickerProperty;

#define SCROLLBAR_LEFT_SIDE     0
#define SCROLLBAR_RIGHT_SIDE    1

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Global gui state control functions
RAYGUIAPI void GuiEnable(void);                                 // Enable gui controls (global state)
RAYGUIAPI void GuiDisable(void);                                // Disable gui controls (global state)
RAYGUIAPI void GuiLock(void);                                   // Lock gui controls (global state)
RAYGUIAPI void GuiUnlock(void);                                 // Unlock gui controls (global state)
RAYGUIAPI bool GuiIsLocked(void);                               // Check if gui is locked (global state)
RAYGUIAPI void GuiSetAlpha(float alpha);                        // Set gui controls alpha (global state), alpha goes from 0.0f to 1.0f
RAYGUIAPI void GuiSetState(int state);                          // Set gui state (global state)
RAYGUIAPI int GuiGetState(void);                                // Get gui state (global state)

// Font set/get functions
RAYGUIAPI void GuiSetFont(Font font);                           // Set gui custom font (global state)
RAYGUIAPI Font GuiGetFont(void);                                // Get gui custom font (global state)

// Style set/get functions
RAYGUIAPI void GuiSetStyle(int control, int property, int value); // Set one style property
RAYGUIAPI int GuiGetStyle(int control, int property);           // Get one style property

// Styles loading functions
RAYGUIAPI void GuiLoadStyle(const char *fileName);              // Load style file over global style variable (.rgs)
RAYGUIAPI void GuiLoadStyleDefault(void);                       // Load style default over global style

// Tooltips management functions
RAYGUIAPI void GuiEnableTooltip(void);                          // Enable gui tooltips (global state)
RAYGUIAPI void GuiDisableTooltip(void);                         // Disable gui tooltips (global state)
RAYGUIAPI void GuiSetTooltip(const char *tooltip);              // Set tooltip string

// Icons functionality
RAYGUIAPI const char *GuiIconText(int iconId, const char *text); // Get text with icon id prepended (if supported)
#if !defined(RAYGUI_NO_ICONS)
RAYGUIAPI void GuiSetIconScale(int scale);                      // Set default icon drawing size
RAYGUIAPI unsigned int *GuiGetIcons(void);                      // Get raygui icons data pointer
RAYGUIAPI char **GuiLoadIcons(const char *fileName, bool loadIconsName); // Load raygui icons file (.rgi) into internal icons data
RAYGUIAPI void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color); // Draw icon using pixel size at specified position
#endif


// Controls
//----------------------------------------------------------------------------------------------------------
// Container/separator controls, useful for controls organization
RAYGUIAPI int GuiWindowBox(Rectangle bounds, const char *title);                                       // Window Box control, shows a window that can be closed
RAYGUIAPI int GuiGroupBox(Rectangle bounds, const char *text);                                         // Group Box control with text name
RAYGUIAPI int GuiLine(Rectangle bounds, const char *text);                                             // Line separator control, could contain text
RAYGUIAPI int GuiPanel(Rectangle bounds, const char *text);                                            // Panel control, useful to group controls
RAYGUIAPI int GuiTabBar(Rectangle bounds, const char **text, int count, int *active);                  // Tab Bar control, returns TAB to be closed or -1
RAYGUIAPI int GuiScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view); // Scroll Panel control

// Basic controls set
RAYGUIAPI int GuiLabel(Rectangle bounds, const char *text);                                            // Label control, shows text
RAYGUIAPI int GuiButton(Rectangle bounds, const char *text);                                           // Button control, returns true when clicked
RAYGUIAPI int GuiLabelButton(Rectangle bounds, const char *text);                                      // Label button control, show true when clicked
RAYGUIAPI int GuiToggle(Rectangle bounds, const char *text, bool *active);                             // Toggle Button control, returns true when active
RAYGUIAPI int GuiToggleGroup(Rectangle bounds, const char *text, int *active);                         // Toggle Group control, returns active toggle index
RAYGUIAPI int GuiToggleSlider(Rectangle bounds, const char *text, int *active);                        // Toggle Slider control, returns true when clicked
RAYGUIAPI int GuiCheckBox(Rectangle bounds, const char *text, bool *checked);                          // Check Box control, returns true when active
RAYGUIAPI int GuiComboBox(Rectangle bounds, const char *text, int *active);                            // Combo Box control, returns selected item index

RAYGUIAPI int GuiDropdownBox(Rectangle bounds, const char *text, int *active, bool editMode);          // Dropdown Box control, returns selected item
RAYGUIAPI int GuiSpinner(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode); // Spinner control, returns selected value
RAYGUIAPI int GuiValueBox(Rectangle bounds, const char *text, int *value, int minValue, int maxValue, bool editMode); // Value Box control, updates input text with numbers
RAYGUIAPI int GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode);                   // Text Box control, updates input text

RAYGUIAPI int GuiSlider(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Slider control, returns selected value
RAYGUIAPI int GuiSliderBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Slider Bar control, returns selected value
RAYGUIAPI int GuiProgressBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Progress Bar control, shows current progress value
RAYGUIAPI int GuiStatusBar(Rectangle bounds, const char *text);                                        // Status Bar control, shows info text
RAYGUIAPI int GuiDummyRec(Rectangle bounds, const char *text);                                         // Dummy control for placeholders
RAYGUIAPI int GuiGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 *mouseCell); // Grid control, returns mouse cell position

// Advance controls set
RAYGUIAPI int GuiListView(Rectangle bounds, const char *text, int *scrollIndex, int *active);          // List View control, returns selected list item index
RAYGUIAPI int GuiListViewEx(Rectangle bounds, const char **text, int count, int *scrollIndex, int *active, int *focus); // List View with extended parameters
RAYGUIAPI int GuiMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons); // Message Box control, displays a message
RAYGUIAPI int GuiTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool *secretViewActive); // Text Input Box control, ask for text, supports secret
RAYGUIAPI int GuiColorPicker(Rectangle bounds, const char *text, Color *color);                        // Color Picker control (multiple color controls)
RAYGUIAPI int GuiColorPanel(Rectangle bounds, const char *text, Color *color);                         // Color Panel control
RAYGUIAPI int GuiColorBarAlpha(Rectangle bounds, const char *text, float *alpha);                      // Color Bar Alpha control
RAYGUIAPI int GuiColorBarHue(Rectangle bounds, const char *text, float *value);                        // Color Bar Hue control
RAYGUIAPI int GuiColorPickerHSV(Rectangle bounds, const char *text, Vector3 *colorHsv);                // Color Picker control that avoids conversion to RGB on each call (multiple color controls)
RAYGUIAPI int GuiColorPanelHSV(Rectangle bounds, const char *text, Vector3 *colorHsv);                 // Color Panel control that returns HSV color value, used by GuiColorPickerHSV()
//----------------------------------------------------------------------------------------------------------


#if !defined(RAYGUI_NO_ICONS)

#if !defined(RAYGUI_CUSTOM_ICONS)
//----------------------------------------------------------------------------------
// Icons enumeration
//----------------------------------------------------------------------------------
typedef enum {
    ICON_NONE                     = 0,
    ICON_FOLDER_FILE_OPEN         = 1,
    ICON_FILE_SAVE_CLASSIC        = 2,
    ICON_FOLDER_OPEN              = 3,
    ICON_FOLDER_SAVE              = 4,
    ICON_FILE_OPEN                = 5,
    ICON_FILE_SAVE                = 6,
    ICON_FILE_EXPORT              = 7,
    ICON_FILE_ADD                 = 8,
    ICON_FILE_DELETE              = 9,
    ICON_FILETYPE_TEXT            = 10,
    ICON_FILETYPE_AUDIO           = 11,
    ICON_FILETYPE_IMAGE           = 12,
    ICON_FILETYPE_PLAY            = 13,
    ICON_FILETYPE_VIDEO           = 14,
    ICON_FILETYPE_INFO            = 15,
    ICON_FILE_COPY                = 16,
    ICON_FILE_CUT                 = 17,
    ICON_FILE_PASTE               = 18,
    ICON_CURSOR_HAND              = 19,
    ICON_CURSOR_POINTER           = 20,
    ICON_CURSOR_CLASSIC           = 21,
    ICON_PENCIL                   = 22,
    ICON_PENCIL_BIG               = 23,
    ICON_BRUSH_CLASSIC            = 24,
    ICON_BRUSH_PAINTER            = 25,
    ICON_WATER_DROP               = 26,
    ICON_COLOR_PICKER             = 27,
    ICON_RUBBER                   = 28,
    ICON_COLOR_BUCKET             = 29,
    ICON_TEXT_T                   = 30,
    ICON_TEXT_A                   = 31,
    ICON_SCALE                    = 32,
    ICON_RESIZE                   = 33,
    ICON_FILTER_POINT             = 34,
    ICON_FILTER_BILINEAR          = 35,
    ICON_CROP                     = 36,
    ICON_CROP_ALPHA               = 37,
    ICON_SQUARE_TOGGLE            = 38,
    ICON_SYMMETRY                 = 39,
    ICON_SYMMETRY_HORIZONTAL      = 40,
    ICON_SYMMETRY_VERTICAL        = 41,
    ICON_LENS                     = 42,
    ICON_LENS_BIG                 = 43,
    ICON_EYE_ON                   = 44,
    ICON_EYE_OFF                  = 45,
    ICON_FILTER_TOP               = 46,
    ICON_FILTER                   = 47,
    ICON_TARGET_POINT             = 48,
    ICON_TARGET_SMALL             = 49,
    ICON_TARGET_BIG               = 50,
    ICON_TARGET_MOVE              = 51,
    ICON_CURSOR_MOVE              = 52,
    ICON_CURSOR_SCALE             = 53,
    ICON_CURSOR_SCALE_RIGHT       = 54,
    ICON_CURSOR_SCALE_LEFT        = 55,
    ICON_UNDO                     = 56,
    ICON_REDO                     = 57,
    ICON_REREDO                   = 58,
    ICON_MUTATE                   = 59,
    ICON_ROTATE                   = 60,
    ICON_REPEAT                   = 61,
    ICON_SHUFFLE                  = 62,
    ICON_EMPTYBOX                 = 63,
    ICON_TARGET                   = 64,
    ICON_TARGET_SMALL_FILL        = 65,
    ICON_TARGET_BIG_FILL          = 66,
    ICON_TARGET_MOVE_FILL         = 67,
    ICON_CURSOR_MOVE_FILL         = 68,
    ICON_CURSOR_SCALE_FILL        = 69,
    ICON_CURSOR_SCALE_RIGHT_FILL  = 70,
    ICON_CURSOR_SCALE_LEFT_FILL   = 71,
    ICON_UNDO_FILL                = 72,
    ICON_REDO_FILL                = 73,
    ICON_REREDO_FILL              = 74,
    ICON_MUTATE_FILL              = 75,
    ICON_ROTATE_FILL              = 76,
    ICON_REPEAT_FILL              = 77,
    ICON_SHUFFLE_FILL             = 78,
    ICON_EMPTYBOX_SMALL           = 79,
    ICON_BOX                      = 80,
    ICON_BOX_TOP                  = 81,
    ICON_BOX_TOP_RIGHT            = 82,
    ICON_BOX_RIGHT                = 83,
    ICON_BOX_BOTTOM_RIGHT         = 84,
    ICON_BOX_BOTTOM               = 85,
    ICON_BOX_BOTTOM_LEFT          = 86,
    ICON_BOX_LEFT                 = 87,
    ICON_BOX_TOP_LEFT             = 88,
    ICON_BOX_CENTER               = 89,
    ICON_BOX_CIRCLE_MASK          = 90,
    ICON_POT                      = 91,
    ICON_ALPHA_MULTIPLY           = 92,
    ICON_ALPHA_CLEAR              = 93,
    ICON_DITHERING                = 94,
    ICON_MIPMAPS                  = 95,
    ICON_BOX_GRID                 = 96,
    ICON_GRID                     = 97,
    ICON_BOX_CORNERS_SMALL        = 98,
    ICON_BOX_CORNERS_BIG          = 99,
    ICON_FOUR_BOXES               = 100,
    ICON_GRID_FILL                = 101,
    ICON_BOX_MULTISIZE            = 102,
    ICON_ZOOM_SMALL               = 103,
    ICON_ZOOM_MEDIUM              = 104,
    ICON_ZOOM_BIG                 = 105,
    ICON_ZOOM_ALL                 = 106,
    ICON_ZOOM_CENTER              = 107,
    ICON_BOX_DOTS_SMALL           = 108,
    ICON_BOX_DOTS_BIG             = 109,
    ICON_BOX_CONCENTRIC           = 110,
    ICON_BOX_GRID_BIG             = 111,
    ICON_OK_TICK                  = 112,
    ICON_CROSS                    = 113,
    ICON_ARROW_LEFT               = 114,
    ICON_ARROW_RIGHT              = 115,
    ICON_ARROW_DOWN               = 116,
    ICON_ARROW_UP                 = 117,
    ICON_ARROW_LEFT_FILL          = 118,
    ICON_ARROW_RIGHT_FILL         = 119,
    ICON_ARROW_DOWN_FILL          = 120,
    ICON_ARROW_UP_FILL            = 121,
    ICON_AUDIO                    = 122,
    ICON_FX                       = 123,
    ICON_WAVE                     = 124,
    ICON_WAVE_SINUS               = 125,
    ICON_WAVE_SQUARE              = 126,
    ICON_WAVE_TRIANGULAR          = 127,
    ICON_CROSS_SMALL              = 128,
    ICON_PLAYER_PREVIOUS          = 129,
    ICON_PLAYER_PLAY_BACK         = 130,
    ICON_PLAYER_PLAY              = 131,
    ICON_PLAYER_PAUSE             = 132,
    ICON_PLAYER_STOP              = 133,
    ICON_PLAYER_NEXT              = 134,
    ICON_PLAYER_RECORD            = 135,
    ICON_MAGNET                   = 136,
    ICON_LOCK_CLOSE               = 137,
    ICON_LOCK_OPEN                = 138,
    ICON_CLOCK                    = 139,
    ICON_TOOLS                    = 140,
    ICON_GEAR                     = 141,
    ICON_GEAR_BIG                 = 142,
    ICON_BIN                      = 143,
    ICON_HAND_POINTER             = 144,
    ICON_LASER                    = 145,
    ICON_COIN                     = 146,
    ICON_EXPLOSION                = 147,
    ICON_1UP                      = 148,
    ICON_PLAYER                   = 149,
    ICON_PLAYER_JUMP              = 150,
    ICON_KEY                      = 151,
    ICON_DEMON                    = 152,
    ICON_TEXT_POPUP               = 153,
    ICON_GEAR_EX                  = 154,
    ICON_CRACK                    = 155,
    ICON_CRACK_POINTS             = 156,
    ICON_STAR                     = 157,
    ICON_DOOR                     = 158,
    ICON_EXIT                     = 159,
    ICON_MODE_2D                  = 160,
    ICON_MODE_3D                  = 161,
    ICON_CUBE                     = 162,
    ICON_CUBE_FACE_TOP            = 163,
    ICON_CUBE_FACE_LEFT           = 164,
    ICON_CUBE_FACE_FRONT          = 165,
    ICON_CUBE_FACE_BOTTOM         = 166,
    ICON_CUBE_FACE_RIGHT          = 167,
    ICON_CUBE_FACE_BACK           = 168,
    ICON_CAMERA                   = 169,
    ICON_SPECIAL                  = 170,
    ICON_LINK_NET                 = 171,
    ICON_LINK_BOXES               = 172,
    ICON_LINK_MULTI               = 173,
    ICON_LINK                     = 174,
    ICON_LINK_BROKE               = 175,
    ICON_TEXT_NOTES               = 176,
    ICON_NOTEBOOK                 = 177,
    ICON_SUITCASE                 = 178,
    ICON_SUITCASE_ZIP             = 179,
    ICON_MAILBOX                  = 180,
    ICON_MONITOR                  = 181,
    ICON_PRINTER                  = 182,
    ICON_PHOTO_CAMERA             = 183,
    ICON_PHOTO_CAMERA_FLASH       = 184,
    ICON_HOUSE                    = 185,
    ICON_HEART                    = 186,
    ICON_CORNER                   = 187,
    ICON_VERTICAL_BARS            = 188,
    ICON_VERTICAL_BARS_FILL       = 189,
    ICON_LIFE_BARS                = 190,
    ICON_INFO                     = 191,
    ICON_CROSSLINE                = 192,
    ICON_HELP                     = 193,
    ICON_FILETYPE_ALPHA           = 194,
    ICON_FILETYPE_HOME            = 195,
    ICON_LAYERS_VISIBLE           = 196,
    ICON_LAYERS                   = 197,
    ICON_WINDOW                   = 198,
    ICON_HIDPI                    = 199,
    ICON_FILETYPE_BINARY          = 200,
    ICON_HEX                      = 201,
    ICON_SHIELD                   = 202,
    ICON_FILE_NEW                 = 203,
    ICON_FOLDER_ADD               = 204,
    ICON_ALARM                    = 205,
    ICON_CPU                      = 206,
    ICON_ROM                      = 207,
    ICON_STEP_OVER                = 208,
    ICON_STEP_INTO                = 209,
    ICON_STEP_OUT                 = 210,
    ICON_RESTART                  = 211,
    ICON_BREAKPOINT_ON            = 212,
    ICON_BREAKPOINT_OFF           = 213,
    ICON_BURGER_MENU              = 214,
    ICON_CASE_SENSITIVE           = 215,
    ICON_REG_EXP                  = 216,
    ICON_FOLDER                   = 217,
    ICON_FILE                     = 218,
    ICON_SAND_TIMER               = 219,
    ICON_220                      = 220,
    ICON_221                      = 221,
    ICON_222                      = 222,
    ICON_223                      = 223,
    ICON_224                      = 224,
    ICON_225                      = 225,
    ICON_226                      = 226,
    ICON_227                      = 227,
    ICON_228                      = 228,
    ICON_229                      = 229,
    ICON_230                      = 230,
    ICON_231                      = 231,
    ICON_232                      = 232,
    ICON_233                      = 233,
    ICON_234                      = 234,
    ICON_235                      = 235,
    ICON_236                      = 236,
    ICON_237                      = 237,
    ICON_238                      = 238,
    ICON_239                      = 239,
    ICON_240                      = 240,
    ICON_241                      = 241,
    ICON_242                      = 242,
    ICON_243                      = 243,
    ICON_244                      = 244,
    ICON_245                      = 245,
    ICON_246                      = 246,
    ICON_247                      = 247,
    ICON_248                      = 248,
    ICON_249                      = 249,
    ICON_250                      = 250,
    ICON_251                      = 251,
    ICON_252                      = 252,
    ICON_253                      = 253,
    ICON_254                      = 254,
    ICON_255                      = 255,
} GuiIconName;
#endif

#endif

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif // RAYGUI_H

/***********************************************************************************
*
*   RAYGUI IMPLEMENTATION
*
************************************************************************************/


#include <stdio.h>              // Required for: FILE, fopen(), fclose(), fprintf(), feof(), fscanf(), vsprintf() [GuiLoadStyle(), GuiLoadIcons()]
#include <stdlib.h>             // Required for: malloc(), calloc(), free() [GuiLoadStyle(), GuiLoadIcons()]
#include <string.h>             // Required for: strlen() [GuiTextBox(), GuiValueBox()], memset(), memcpy()
#include <stdarg.h>             // Required for: va_list, va_start(), vfprintf(), va_end() [TextFormat()]
#include <math.h>               // Required for: roundf() [GuiColorPicker()]

#ifdef __cplusplus
    #define RAYGUI_CLITERAL(name) name
#else
    #define RAYGUI_CLITERAL(name) (name)
#endif

// Check if two rectangles are equal, used to validate a slider bounds as an id
#ifndef CHECK_BOUNDS_ID
    #define CHECK_BOUNDS_ID(src, dst) ((src.x == dst.x) && (src.y == dst.y) && (src.width == dst.width) && (src.height == dst.height))
#endif

#if !defined(RAYGUI_NO_ICONS) && !defined(RAYGUI_CUSTOM_ICONS)

// Embedded icons, no external file provided
#define RAYGUI_ICON_SIZE               16          // Size of icons in pixels (squared)
#define RAYGUI_ICON_MAX_ICONS         256          // Maximum number of icons
#define RAYGUI_ICON_MAX_NAME_LENGTH    32          // Maximum length of icon name id

// Icons data is defined by bit array (every bit represents one pixel)
// Those arrays are stored as unsigned int data arrays, so,
// every array element defines 32 pixels (bits) of information
// One icon is defined by 8 int, (8 int * 32 bit = 256 bit = 16*16 pixels)
// NOTE: Number of elemens depend on RAYGUI_ICON_SIZE (by default 16x16 pixels)
#define RAYGUI_ICON_DATA_ELEMENTS   (RAYGUI_ICON_SIZE*RAYGUI_ICON_SIZE/32)

//----------------------------------------------------------------------------------
// Icons data for all gui possible icons (allocated on data segment by default)
//
// NOTE 1: Every icon is codified in binary form, using 1 bit per pixel, so,
// every 16x16 icon requires 8 integers (16*16/32) to be stored
//
// NOTE 2: A different icon set could be loaded over this array using GuiLoadIcons(),
// but loaded icons set must be same RAYGUI_ICON_SIZE and no more than RAYGUI_ICON_MAX_ICONS
//
// guiIcons size is by default: 256*(16*16/32) = 2048*4 = 8192 bytes = 8 KB
//----------------------------------------------------------------------------------
static unsigned int guiIcons[RAYGUI_ICON_MAX_ICONS*RAYGUI_ICON_DATA_ELEMENTS] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_NONE
    0x3ff80000, 0x2f082008, 0x2042207e, 0x40027fc2, 0x40024002, 0x40024002, 0x40024002, 0x00007ffe,      // ICON_FOLDER_FILE_OPEN
    0x3ffe0000, 0x44226422, 0x400247e2, 0x5ffa4002, 0x57ea500a, 0x500a500a, 0x40025ffa, 0x00007ffe,      // ICON_FILE_SAVE_CLASSIC
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x41024002, 0x44424282, 0x793e4102, 0x00000100,      // ICON_FOLDER_OPEN
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x41024102, 0x44424102, 0x793e4282, 0x00000000,      // ICON_FOLDER_SAVE
    0x3ff00000, 0x201c2010, 0x20042004, 0x21042004, 0x24442284, 0x21042104, 0x20042104, 0x00003ffc,      // ICON_FILE_OPEN
    0x3ff00000, 0x201c2010, 0x20042004, 0x21042004, 0x21042104, 0x22842444, 0x20042104, 0x00003ffc,      // ICON_FILE_SAVE
    0x3ff00000, 0x201c2010, 0x00042004, 0x20041004, 0x20844784, 0x00841384, 0x20042784, 0x00003ffc,      // ICON_FILE_EXPORT
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x22042204, 0x22042f84, 0x20042204, 0x00003ffc,      // ICON_FILE_ADD
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x25042884, 0x25042204, 0x20042884, 0x00003ffc,      // ICON_FILE_DELETE
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042ff4, 0x20042ff4, 0x20042ff4, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_TEXT
    0x3ff00000, 0x201c2010, 0x27042004, 0x244424c4, 0x26442444, 0x20642664, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_AUDIO
    0x3ff00000, 0x201c2010, 0x26042604, 0x20042004, 0x35442884, 0x2414222c, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_IMAGE
    0x3ff00000, 0x201c2010, 0x20c42004, 0x22442144, 0x22442444, 0x20c42144, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_PLAY
    0x3ff00000, 0x3ffc2ff0, 0x3f3c2ff4, 0x3dbc2eb4, 0x3dbc2bb4, 0x3f3c2eb4, 0x3ffc2ff4, 0x00002ff4,      // ICON_FILETYPE_VIDEO
    0x3ff00000, 0x201c2010, 0x21842184, 0x21842004, 0x21842184, 0x21842184, 0x20042184, 0x00003ffc,      // ICON_FILETYPE_INFO
    0x0ff00000, 0x381c0810, 0x28042804, 0x28042804, 0x28042804, 0x28042804, 0x20102ffc, 0x00003ff0,      // ICON_FILE_COPY
    0x00000000, 0x701c0000, 0x079c1e14, 0x55a000f0, 0x079c00f0, 0x701c1e14, 0x00000000, 0x00000000,      // ICON_FILE_CUT
    0x01c00000, 0x13e41bec, 0x3f841004, 0x204420c4, 0x20442044, 0x20442044, 0x207c2044, 0x00003fc0,      // ICON_FILE_PASTE
    0x00000000, 0x3aa00fe0, 0x2abc2aa0, 0x2aa42aa4, 0x20042aa4, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_CURSOR_HAND
    0x00000000, 0x003c000c, 0x030800c8, 0x30100c10, 0x10202020, 0x04400840, 0x01800280, 0x00000000,      // ICON_CURSOR_POINTER
    0x00000000, 0x00180000, 0x01f00078, 0x03e007f0, 0x07c003e0, 0x04000e40, 0x00000000, 0x00000000,      // ICON_CURSOR_CLASSIC
    0x00000000, 0x04000000, 0x11000a00, 0x04400a80, 0x01100220, 0x00580088, 0x00000038, 0x00000000,      // ICON_PENCIL
    0x04000000, 0x15000a00, 0x50402880, 0x14102820, 0x05040a08, 0x015c028c, 0x007c00bc, 0x00000000,      // ICON_PENCIL_BIG
    0x01c00000, 0x01400140, 0x01400140, 0x0ff80140, 0x0ff80808, 0x0aa80808, 0x0aa80aa8, 0x00000ff8,      // ICON_BRUSH_CLASSIC
    0x1ffc0000, 0x5ffc7ffe, 0x40004000, 0x00807f80, 0x01c001c0, 0x01c001c0, 0x01c001c0, 0x00000080,      // ICON_BRUSH_PAINTER
    0x00000000, 0x00800000, 0x01c00080, 0x03e001c0, 0x07f003e0, 0x036006f0, 0x000001c0, 0x00000000,      // ICON_WATER_DROP
    0x00000000, 0x3e003800, 0x1f803f80, 0x0c201e40, 0x02080c10, 0x00840104, 0x00380044, 0x00000000,      // ICON_COLOR_PICKER
    0x00000000, 0x07800300, 0x1fe00fc0, 0x3f883fd0, 0x0e021f04, 0x02040402, 0x00f00108, 0x00000000,      // ICON_RUBBER
    0x00c00000, 0x02800140, 0x08200440, 0x20081010, 0x2ffe3004, 0x03f807fc, 0x00e001f0, 0x00000040,      // ICON_COLOR_BUCKET
    0x00000000, 0x21843ffc, 0x01800180, 0x01800180, 0x01800180, 0x01800180, 0x03c00180, 0x00000000,      // ICON_TEXT_T
    0x00800000, 0x01400180, 0x06200340, 0x0c100620, 0x1ff80c10, 0x380c1808, 0x70067004, 0x0000f80f,      // ICON_TEXT_A
    0x78000000, 0x50004000, 0x00004800, 0x03c003c0, 0x03c003c0, 0x00100000, 0x0002000a, 0x0000000e,      // ICON_SCALE
    0x75560000, 0x5e004002, 0x54001002, 0x41001202, 0x408200fe, 0x40820082, 0x40820082, 0x00006afe,      // ICON_RESIZE
    0x00000000, 0x3f003f00, 0x3f003f00, 0x3f003f00, 0x00400080, 0x001c0020, 0x001c001c, 0x00000000,      // ICON_FILTER_POINT
    0x6d800000, 0x00004080, 0x40804080, 0x40800000, 0x00406d80, 0x001c0020, 0x001c001c, 0x00000000,      // ICON_FILTER_BILINEAR
    0x40080000, 0x1ffe2008, 0x14081008, 0x11081208, 0x10481088, 0x10081028, 0x10047ff8, 0x00001002,      // ICON_CROP
    0x00100000, 0x3ffc0010, 0x2ab03550, 0x22b02550, 0x20b02150, 0x20302050, 0x2000fff0, 0x00002000,      // ICON_CROP_ALPHA
    0x40000000, 0x1ff82000, 0x04082808, 0x01082208, 0x00482088, 0x00182028, 0x35542008, 0x00000002,      // ICON_SQUARE_TOGGLE
    0x00000000, 0x02800280, 0x06c006c0, 0x0ea00ee0, 0x1e901eb0, 0x3e883e98, 0x7efc7e8c, 0x00000000,      // ICON_SYMMETRY
    0x01000000, 0x05600100, 0x1d480d50, 0x7d423d44, 0x3d447d42, 0x0d501d48, 0x01000560, 0x00000100,      // ICON_SYMMETRY_HORIZONTAL
    0x01800000, 0x04200240, 0x10080810, 0x00001ff8, 0x00007ffe, 0x0ff01ff8, 0x03c007e0, 0x00000180,      // ICON_SYMMETRY_VERTICAL
    0x00000000, 0x010800f0, 0x02040204, 0x02040204, 0x07f00308, 0x1c000e00, 0x30003800, 0x00000000,      // ICON_LENS
    0x00000000, 0x061803f0, 0x08240c0c, 0x08040814, 0x0c0c0804, 0x23f01618, 0x18002400, 0x00000000,      // ICON_LENS_BIG
    0x00000000, 0x00000000, 0x1c7007c0, 0x638e3398, 0x1c703398, 0x000007c0, 0x00000000, 0x00000000,      // ICON_EYE_ON
    0x00000000, 0x10002000, 0x04700fc0, 0x610e3218, 0x1c703098, 0x001007a0, 0x00000008, 0x00000000,      // ICON_EYE_OFF
    0x00000000, 0x00007ffc, 0x40047ffc, 0x10102008, 0x04400820, 0x02800280, 0x02800280, 0x00000100,      // ICON_FILTER_TOP
    0x00000000, 0x40027ffe, 0x10082004, 0x04200810, 0x02400240, 0x02400240, 0x01400240, 0x000000c0,      // ICON_FILTER
    0x00800000, 0x00800080, 0x00000080, 0x3c9e0000, 0x00000000, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_POINT
    0x00800000, 0x00800080, 0x00800080, 0x3f7e01c0, 0x008001c0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_SMALL
    0x00800000, 0x00800080, 0x03e00080, 0x3e3e0220, 0x03e00220, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_BIG
    0x01000000, 0x04400280, 0x01000100, 0x43842008, 0x43849ab2, 0x01002008, 0x04400100, 0x01000280,      // ICON_TARGET_MOVE
    0x01000000, 0x04400280, 0x01000100, 0x41042108, 0x41049ff2, 0x01002108, 0x04400100, 0x01000280,      // ICON_CURSOR_MOVE
    0x781e0000, 0x500a4002, 0x04204812, 0x00000240, 0x02400000, 0x48120420, 0x4002500a, 0x0000781e,      // ICON_CURSOR_SCALE
    0x00000000, 0x20003c00, 0x24002800, 0x01000200, 0x00400080, 0x00140024, 0x003c0004, 0x00000000,      // ICON_CURSOR_SCALE_RIGHT
    0x00000000, 0x0004003c, 0x00240014, 0x00800040, 0x02000100, 0x28002400, 0x3c002000, 0x00000000,      // ICON_CURSOR_SCALE_LEFT
    0x00000000, 0x00100020, 0x10101fc8, 0x10001020, 0x10001000, 0x10001000, 0x00001fc0, 0x00000000,      // ICON_UNDO
    0x00000000, 0x08000400, 0x080813f8, 0x00080408, 0x00080008, 0x00080008, 0x000003f8, 0x00000000,      // ICON_REDO
    0x00000000, 0x3ffc0000, 0x20042004, 0x20002000, 0x20402000, 0x3f902020, 0x00400020, 0x00000000,      // ICON_REREDO
    0x00000000, 0x3ffc0000, 0x20042004, 0x27fc2004, 0x20202000, 0x3fc82010, 0x00200010, 0x00000000,      // ICON_MUTATE
    0x00000000, 0x0ff00000, 0x10081818, 0x11801008, 0x10001180, 0x18101020, 0x00100fc8, 0x00000020,      // ICON_ROTATE
    0x00000000, 0x04000200, 0x240429fc, 0x20042204, 0x20442004, 0x3f942024, 0x00400020, 0x00000000,      // ICON_REPEAT
    0x00000000, 0x20001000, 0x22104c0e, 0x00801120, 0x11200040, 0x4c0e2210, 0x10002000, 0x00000000,      // ICON_SHUFFLE
    0x7ffe0000, 0x50024002, 0x44024802, 0x41024202, 0x40424082, 0x40124022, 0x4002400a, 0x00007ffe,      // ICON_EMPTYBOX
    0x00800000, 0x03e00080, 0x08080490, 0x3c9e0808, 0x08080808, 0x03e00490, 0x00800080, 0x00000000,      // ICON_TARGET
    0x00800000, 0x00800080, 0x00800080, 0x3ffe01c0, 0x008001c0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_SMALL_FILL
    0x00800000, 0x00800080, 0x03e00080, 0x3ffe03e0, 0x03e003e0, 0x00800080, 0x00800080, 0x00000000,      // ICON_TARGET_BIG_FILL
    0x01000000, 0x07c00380, 0x01000100, 0x638c2008, 0x638cfbbe, 0x01002008, 0x07c00100, 0x01000380,      // ICON_TARGET_MOVE_FILL
    0x01000000, 0x07c00380, 0x01000100, 0x610c2108, 0x610cfffe, 0x01002108, 0x07c00100, 0x01000380,      // ICON_CURSOR_MOVE_FILL
    0x781e0000, 0x6006700e, 0x04204812, 0x00000240, 0x02400000, 0x48120420, 0x700e6006, 0x0000781e,      // ICON_CURSOR_SCALE_FILL
    0x00000000, 0x38003c00, 0x24003000, 0x01000200, 0x00400080, 0x000c0024, 0x003c001c, 0x00000000,      // ICON_CURSOR_SCALE_RIGHT_FILL
    0x00000000, 0x001c003c, 0x0024000c, 0x00800040, 0x02000100, 0x30002400, 0x3c003800, 0x00000000,      // ICON_CURSOR_SCALE_LEFT_FILL
    0x00000000, 0x00300020, 0x10301ff8, 0x10001020, 0x10001000, 0x10001000, 0x00001fc0, 0x00000000,      // ICON_UNDO_FILL
    0x00000000, 0x0c000400, 0x0c081ff8, 0x00080408, 0x00080008, 0x00080008, 0x000003f8, 0x00000000,      // ICON_REDO_FILL
    0x00000000, 0x3ffc0000, 0x20042004, 0x20002000, 0x20402000, 0x3ff02060, 0x00400060, 0x00000000,      // ICON_REREDO_FILL
    0x00000000, 0x3ffc0000, 0x20042004, 0x27fc2004, 0x20202000, 0x3ff82030, 0x00200030, 0x00000000,      // ICON_MUTATE_FILL
    0x00000000, 0x0ff00000, 0x10081818, 0x11801008, 0x10001180, 0x18301020, 0x00300ff8, 0x00000020,      // ICON_ROTATE_FILL
    0x00000000, 0x06000200, 0x26042ffc, 0x20042204, 0x20442004, 0x3ff42064, 0x00400060, 0x00000000,      // ICON_REPEAT_FILL
    0x00000000, 0x30001000, 0x32107c0e, 0x00801120, 0x11200040, 0x7c0e3210, 0x10003000, 0x00000000,      // ICON_SHUFFLE_FILL
    0x00000000, 0x30043ffc, 0x24042804, 0x21042204, 0x20442084, 0x20142024, 0x3ffc200c, 0x00000000,      // ICON_EMPTYBOX_SMALL
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX
    0x00000000, 0x23c43ffc, 0x23c423c4, 0x200423c4, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP
    0x00000000, 0x3e043ffc, 0x3e043e04, 0x20043e04, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x3e043e04, 0x3e043e04, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x3e042004, 0x3e043e04, 0x3ffc3e04, 0x00000000,      // ICON_BOX_BOTTOM_RIGHT
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x23c42004, 0x23c423c4, 0x3ffc23c4, 0x00000000,      // ICON_BOX_BOTTOM
    0x00000000, 0x20043ffc, 0x20042004, 0x20042004, 0x207c2004, 0x207c207c, 0x3ffc207c, 0x00000000,      // ICON_BOX_BOTTOM_LEFT
    0x00000000, 0x20043ffc, 0x20042004, 0x207c207c, 0x207c207c, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_LEFT
    0x00000000, 0x207c3ffc, 0x207c207c, 0x2004207c, 0x20042004, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_TOP_LEFT
    0x00000000, 0x20043ffc, 0x20042004, 0x23c423c4, 0x23c423c4, 0x20042004, 0x3ffc2004, 0x00000000,      // ICON_BOX_CENTER
    0x7ffe0000, 0x40024002, 0x47e24182, 0x4ff247e2, 0x47e24ff2, 0x418247e2, 0x40024002, 0x00007ffe,      // ICON_BOX_CIRCLE_MASK
    0x7fff0000, 0x40014001, 0x40014001, 0x49555ddd, 0x4945495d, 0x400149c5, 0x40014001, 0x00007fff,      // ICON_POT
    0x7ffe0000, 0x53327332, 0x44ce4cce, 0x41324332, 0x404e40ce, 0x48125432, 0x4006540e, 0x00007ffe,      // ICON_ALPHA_MULTIPLY
    0x7ffe0000, 0x53327332, 0x44ce4cce, 0x41324332, 0x5c4e40ce, 0x44124432, 0x40065c0e, 0x00007ffe,      // ICON_ALPHA_CLEAR
    0x7ffe0000, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x42fe417e, 0x00007ffe,      // ICON_DITHERING
    0x07fe0000, 0x1ffa0002, 0x7fea000a, 0x402a402a, 0x5b2a512a, 0x5128552a, 0x40205128, 0x00007fe0,      // ICON_MIPMAPS
    0x00000000, 0x1ff80000, 0x12481248, 0x12481ff8, 0x1ff81248, 0x12481248, 0x00001ff8, 0x00000000,      // ICON_BOX_GRID
    0x12480000, 0x7ffe1248, 0x12481248, 0x12487ffe, 0x7ffe1248, 0x12481248, 0x12487ffe, 0x00001248,      // ICON_GRID
    0x00000000, 0x1c380000, 0x1c3817e8, 0x08100810, 0x08100810, 0x17e81c38, 0x00001c38, 0x00000000,      // ICON_BOX_CORNERS_SMALL
    0x700e0000, 0x700e5ffa, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x5ffa700e, 0x0000700e,      // ICON_BOX_CORNERS_BIG
    0x3f7e0000, 0x21422142, 0x21422142, 0x00003f7e, 0x21423f7e, 0x21422142, 0x3f7e2142, 0x00000000,      // ICON_FOUR_BOXES
    0x00000000, 0x3bb80000, 0x3bb83bb8, 0x3bb80000, 0x3bb83bb8, 0x3bb80000, 0x3bb83bb8, 0x00000000,      // ICON_GRID_FILL
    0x7ffe0000, 0x7ffe7ffe, 0x77fe7000, 0x77fe77fe, 0x777e7700, 0x777e777e, 0x777e777e, 0x0000777e,      // ICON_BOX_MULTISIZE
    0x781e0000, 0x40024002, 0x00004002, 0x01800000, 0x00000180, 0x40020000, 0x40024002, 0x0000781e,      // ICON_ZOOM_SMALL
    0x781e0000, 0x40024002, 0x00004002, 0x03c003c0, 0x03c003c0, 0x40020000, 0x40024002, 0x0000781e,      // ICON_ZOOM_MEDIUM
    0x781e0000, 0x40024002, 0x07e04002, 0x07e007e0, 0x07e007e0, 0x400207e0, 0x40024002, 0x0000781e,      // ICON_ZOOM_BIG
    0x781e0000, 0x5ffa4002, 0x1ff85ffa, 0x1ff81ff8, 0x1ff81ff8, 0x5ffa1ff8, 0x40025ffa, 0x0000781e,      // ICON_ZOOM_ALL
    0x00000000, 0x2004381c, 0x00002004, 0x00000000, 0x00000000, 0x20040000, 0x381c2004, 0x00000000,      // ICON_ZOOM_CENTER
    0x00000000, 0x1db80000, 0x10081008, 0x10080000, 0x00001008, 0x10081008, 0x00001db8, 0x00000000,      // ICON_BOX_DOTS_SMALL
    0x35560000, 0x00002002, 0x00002002, 0x00002002, 0x00002002, 0x00002002, 0x35562002, 0x00000000,      // ICON_BOX_DOTS_BIG
    0x7ffe0000, 0x40024002, 0x48124ff2, 0x49924812, 0x48124992, 0x4ff24812, 0x40024002, 0x00007ffe,      // ICON_BOX_CONCENTRIC
    0x00000000, 0x10841ffc, 0x10841084, 0x1ffc1084, 0x10841084, 0x10841084, 0x00001ffc, 0x00000000,      // ICON_BOX_GRID_BIG
    0x00000000, 0x00000000, 0x10000000, 0x04000800, 0x01040200, 0x00500088, 0x00000020, 0x00000000,      // ICON_OK_TICK
    0x00000000, 0x10080000, 0x04200810, 0x01800240, 0x02400180, 0x08100420, 0x00001008, 0x00000000,      // ICON_CROSS
    0x00000000, 0x02000000, 0x00800100, 0x00200040, 0x00200010, 0x00800040, 0x02000100, 0x00000000,      // ICON_ARROW_LEFT
    0x00000000, 0x00400000, 0x01000080, 0x04000200, 0x04000800, 0x01000200, 0x00400080, 0x00000000,      // ICON_ARROW_RIGHT
    0x00000000, 0x00000000, 0x00000000, 0x08081004, 0x02200410, 0x00800140, 0x00000000, 0x00000000,      // ICON_ARROW_DOWN
    0x00000000, 0x00000000, 0x01400080, 0x04100220, 0x10040808, 0x00000000, 0x00000000, 0x00000000,      // ICON_ARROW_UP
    0x00000000, 0x02000000, 0x03800300, 0x03e003c0, 0x03e003f0, 0x038003c0, 0x02000300, 0x00000000,      // ICON_ARROW_LEFT_FILL
    0x00000000, 0x00400000, 0x01c000c0, 0x07c003c0, 0x07c00fc0, 0x01c003c0, 0x004000c0, 0x00000000,      // ICON_ARROW_RIGHT_FILL
    0x00000000, 0x00000000, 0x00000000, 0x0ff81ffc, 0x03e007f0, 0x008001c0, 0x00000000, 0x00000000,      // ICON_ARROW_DOWN_FILL
    0x00000000, 0x00000000, 0x01c00080, 0x07f003e0, 0x1ffc0ff8, 0x00000000, 0x00000000, 0x00000000,      // ICON_ARROW_UP_FILL
    0x00000000, 0x18a008c0, 0x32881290, 0x24822686, 0x26862482, 0x12903288, 0x08c018a0, 0x00000000,      // ICON_AUDIO
    0x00000000, 0x04800780, 0x004000c0, 0x662000f0, 0x08103c30, 0x130a0e18, 0x0000318e, 0x00000000,      // ICON_FX
    0x00000000, 0x00800000, 0x08880888, 0x2aaa0a8a, 0x0a8a2aaa, 0x08880888, 0x00000080, 0x00000000,      // ICON_WAVE
    0x00000000, 0x00600000, 0x01080090, 0x02040108, 0x42044204, 0x24022402, 0x00001800, 0x00000000,      // ICON_WAVE_SINUS
    0x00000000, 0x07f80000, 0x04080408, 0x04080408, 0x04080408, 0x7c0e0408, 0x00000000, 0x00000000,      // ICON_WAVE_SQUARE
    0x00000000, 0x00000000, 0x00a00040, 0x22084110, 0x08021404, 0x00000000, 0x00000000, 0x00000000,      // ICON_WAVE_TRIANGULAR
    0x00000000, 0x00000000, 0x04200000, 0x01800240, 0x02400180, 0x00000420, 0x00000000, 0x00000000,      // ICON_CROSS_SMALL
    0x00000000, 0x18380000, 0x12281428, 0x10a81128, 0x112810a8, 0x14281228, 0x00001838, 0x00000000,      // ICON_PLAYER_PREVIOUS
    0x00000000, 0x18000000, 0x11801600, 0x10181060, 0x10601018, 0x16001180, 0x00001800, 0x00000000,      // ICON_PLAYER_PLAY_BACK
    0x00000000, 0x00180000, 0x01880068, 0x18080608, 0x06081808, 0x00680188, 0x00000018, 0x00000000,      // ICON_PLAYER_PLAY
    0x00000000, 0x1e780000, 0x12481248, 0x12481248, 0x12481248, 0x12481248, 0x00001e78, 0x00000000,      // ICON_PLAYER_PAUSE
    0x00000000, 0x1ff80000, 0x10081008, 0x10081008, 0x10081008, 0x10081008, 0x00001ff8, 0x00000000,      // ICON_PLAYER_STOP
    0x00000000, 0x1c180000, 0x14481428, 0x15081488, 0x14881508, 0x14281448, 0x00001c18, 0x00000000,      // ICON_PLAYER_NEXT
    0x00000000, 0x03c00000, 0x08100420, 0x10081008, 0x10081008, 0x04200810, 0x000003c0, 0x00000000,      // ICON_PLAYER_RECORD
    0x00000000, 0x0c3007e0, 0x13c81818, 0x14281668, 0x14281428, 0x1c381c38, 0x08102244, 0x00000000,      // ICON_MAGNET
    0x07c00000, 0x08200820, 0x3ff80820, 0x23882008, 0x21082388, 0x20082108, 0x1ff02008, 0x00000000,      // ICON_LOCK_CLOSE
    0x07c00000, 0x08000800, 0x3ff80800, 0x23882008, 0x21082388, 0x20082108, 0x1ff02008, 0x00000000,      // ICON_LOCK_OPEN
    0x01c00000, 0x0c180770, 0x3086188c, 0x60832082, 0x60034781, 0x30062002, 0x0c18180c, 0x01c00770,      // ICON_CLOCK
    0x0a200000, 0x1b201b20, 0x04200e20, 0x04200420, 0x04700420, 0x0e700e70, 0x0e700e70, 0x04200e70,      // ICON_TOOLS
    0x01800000, 0x3bdc318c, 0x0ff01ff8, 0x7c3e1e78, 0x1e787c3e, 0x1ff80ff0, 0x318c3bdc, 0x00000180,      // ICON_GEAR
    0x01800000, 0x3ffc318c, 0x1c381ff8, 0x781e1818, 0x1818781e, 0x1ff81c38, 0x318c3ffc, 0x00000180,      // ICON_GEAR_BIG
    0x00000000, 0x08080ff8, 0x08081ffc, 0x0aa80aa8, 0x0aa80aa8, 0x0aa80aa8, 0x08080aa8, 0x00000ff8,      // ICON_BIN
    0x00000000, 0x00000000, 0x20043ffc, 0x08043f84, 0x04040f84, 0x04040784, 0x000007fc, 0x00000000,      // ICON_HAND_POINTER
    0x00000000, 0x24400400, 0x00001480, 0x6efe0e00, 0x00000e00, 0x24401480, 0x00000400, 0x00000000,      // ICON_LASER
    0x00000000, 0x03c00000, 0x08300460, 0x11181118, 0x11181118, 0x04600830, 0x000003c0, 0x00000000,      // ICON_COIN
    0x00000000, 0x10880080, 0x06c00810, 0x366c07e0, 0x07e00240, 0x00001768, 0x04200240, 0x00000000,      // ICON_EXPLOSION
    0x00000000, 0x3d280000, 0x2528252c, 0x3d282528, 0x05280528, 0x05e80528, 0x00000000, 0x00000000,      // ICON_1UP
    0x01800000, 0x03c003c0, 0x018003c0, 0x0ff007e0, 0x0bd00bd0, 0x0a500bd0, 0x02400240, 0x02400240,      // ICON_PLAYER
    0x01800000, 0x03c003c0, 0x118013c0, 0x03c81ff8, 0x07c003c8, 0x04400440, 0x0c080478, 0x00000000,      // ICON_PLAYER_JUMP
    0x3ff80000, 0x30183ff8, 0x30183018, 0x3ff83ff8, 0x03000300, 0x03c003c0, 0x03e00300, 0x000003e0,      // ICON_KEY
    0x3ff80000, 0x3ff83ff8, 0x33983ff8, 0x3ff83398, 0x3ff83ff8, 0x00000540, 0x0fe00aa0, 0x00000fe0,      // ICON_DEMON
    0x00000000, 0x0ff00000, 0x20041008, 0x25442004, 0x10082004, 0x06000bf0, 0x00000300, 0x00000000,      // ICON_TEXT_POPUP
    0x00000000, 0x11440000, 0x07f00be8, 0x1c1c0e38, 0x1c1c0c18, 0x07f00e38, 0x11440be8, 0x00000000,      // ICON_GEAR_EX
    0x00000000, 0x20080000, 0x0c601010, 0x07c00fe0, 0x07c007c0, 0x0c600fe0, 0x20081010, 0x00000000,      // ICON_CRACK
    0x00000000, 0x20080000, 0x0c601010, 0x04400fe0, 0x04405554, 0x0c600fe0, 0x20081010, 0x00000000,      // ICON_CRACK_POINTS
    0x00000000, 0x00800080, 0x01c001c0, 0x1ffc3ffe, 0x03e007f0, 0x07f003e0, 0x0c180770, 0x00000808,      // ICON_STAR
    0x0ff00000, 0x08180810, 0x08100818, 0x0a100810, 0x08180810, 0x08100818, 0x08100810, 0x00001ff8,      // ICON_DOOR
    0x0ff00000, 0x08100810, 0x08100810, 0x10100010, 0x4f902010, 0x10102010, 0x08100010, 0x00000ff0,      // ICON_EXIT
    0x00040000, 0x001f000e, 0x0ef40004, 0x12f41284, 0x0ef41214, 0x10040004, 0x7ffc3004, 0x10003000,      // ICON_MODE_2D
    0x78040000, 0x501f600e, 0x0ef44004, 0x12f41284, 0x0ef41284, 0x10140004, 0x7ffc300c, 0x10003000,      // ICON_MODE_3D
    0x7fe00000, 0x50286030, 0x47fe4804, 0x44224402, 0x44224422, 0x241275e2, 0x0c06140a, 0x000007fe,      // ICON_CUBE
    0x7fe00000, 0x5ff87ff0, 0x47fe4ffc, 0x44224402, 0x44224422, 0x241275e2, 0x0c06140a, 0x000007fe,      // ICON_CUBE_FACE_TOP
    0x7fe00000, 0x50386030, 0x47c2483c, 0x443e443e, 0x443e443e, 0x241e75fe, 0x0c06140e, 0x000007fe,      // ICON_CUBE_FACE_LEFT
    0x7fe00000, 0x50286030, 0x47fe4804, 0x47fe47fe, 0x47fe47fe, 0x27fe77fe, 0x0ffe17fe, 0x000007fe,      // ICON_CUBE_FACE_FRONT
    0x7fe00000, 0x50286030, 0x47fe4804, 0x44224402, 0x44224422, 0x3bf27be2, 0x0bfe1bfa, 0x000007fe,      // ICON_CUBE_FACE_BOTTOM
    0x7fe00000, 0x70286030, 0x7ffe7804, 0x7c227c02, 0x7c227c22, 0x3c127de2, 0x0c061c0a, 0x000007fe,      // ICON_CUBE_FACE_RIGHT
    0x7fe00000, 0x6fe85ff0, 0x781e77e4, 0x7be27be2, 0x7be27be2, 0x24127be2, 0x0c06140a, 0x000007fe,      // ICON_CUBE_FACE_BACK
    0x00000000, 0x2a0233fe, 0x22022602, 0x22022202, 0x2a022602, 0x00a033fe, 0x02080110, 0x00000000,      // ICON_CAMERA
    0x00000000, 0x200c3ffc, 0x000c000c, 0x3ffc000c, 0x30003000, 0x30003000, 0x3ffc3004, 0x00000000,      // ICON_SPECIAL
    0x00000000, 0x0022003e, 0x012201e2, 0x0100013e, 0x01000100, 0x79000100, 0x4f004900, 0x00007800,      // ICON_LINK_NET
    0x00000000, 0x44007c00, 0x45004600, 0x00627cbe, 0x00620022, 0x45007cbe, 0x44004600, 0x00007c00,      // ICON_LINK_BOXES
    0x00000000, 0x0044007c, 0x0010007c, 0x3f100010, 0x3f1021f0, 0x3f100010, 0x3f0021f0, 0x00000000,      // ICON_LINK_MULTI
    0x00000000, 0x0044007c, 0x00440044, 0x0010007c, 0x00100010, 0x44107c10, 0x440047f0, 0x00007c00,      // ICON_LINK
    0x00000000, 0x0044007c, 0x00440044, 0x0000007c, 0x00000010, 0x44007c10, 0x44004550, 0x00007c00,      // ICON_LINK_BROKE
    0x02a00000, 0x22a43ffc, 0x20042004, 0x20042ff4, 0x20042ff4, 0x20042ff4, 0x20042004, 0x00003ffc,      // ICON_TEXT_NOTES
    0x3ffc0000, 0x20042004, 0x245e27c4, 0x27c42444, 0x2004201e, 0x201e2004, 0x20042004, 0x00003ffc,      // ICON_NOTEBOOK
    0x00000000, 0x07e00000, 0x04200420, 0x24243ffc, 0x24242424, 0x24242424, 0x3ffc2424, 0x00000000,      // ICON_SUITCASE
    0x00000000, 0x0fe00000, 0x08200820, 0x40047ffc, 0x7ffc5554, 0x40045554, 0x7ffc4004, 0x00000000,      // ICON_SUITCASE_ZIP
    0x00000000, 0x20043ffc, 0x3ffc2004, 0x13c81008, 0x100813c8, 0x10081008, 0x1ff81008, 0x00000000,      // ICON_MAILBOX
    0x00000000, 0x40027ffe, 0x5ffa5ffa, 0x5ffa5ffa, 0x40025ffa, 0x03c07ffe, 0x1ff81ff8, 0x00000000,      // ICON_MONITOR
    0x0ff00000, 0x6bfe7ffe, 0x7ffe7ffe, 0x68167ffe, 0x08106816, 0x08100810, 0x0ff00810, 0x00000000,      // ICON_PRINTER
    0x3ff80000, 0xfffe2008, 0x870a8002, 0x904a888a, 0x904a904a, 0x870a888a, 0xfffe8002, 0x00000000,      // ICON_PHOTO_CAMERA
    0x0fc00000, 0xfcfe0cd8, 0x8002fffe, 0x84428382, 0x84428442, 0x80028382, 0xfffe8002, 0x00000000,      // ICON_PHOTO_CAMERA_FLASH
    0x00000000, 0x02400180, 0x08100420, 0x20041008, 0x23c42004, 0x22442244, 0x3ffc2244, 0x00000000,      // ICON_HOUSE
    0x00000000, 0x1c700000, 0x3ff83ef8, 0x3ff83ff8, 0x0fe01ff0, 0x038007c0, 0x00000100, 0x00000000,      // ICON_HEART
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x80000000, 0xe000c000,      // ICON_CORNER
    0x00000000, 0x14001c00, 0x15c01400, 0x15401540, 0x155c1540, 0x15541554, 0x1ddc1554, 0x00000000,      // ICON_VERTICAL_BARS
    0x00000000, 0x03000300, 0x1b001b00, 0x1b601b60, 0x1b6c1b60, 0x1b6c1b6c, 0x1b6c1b6c, 0x00000000,      // ICON_VERTICAL_BARS_FILL
    0x00000000, 0x00000000, 0x403e7ffe, 0x7ffe403e, 0x7ffe0000, 0x43fe43fe, 0x00007ffe, 0x00000000,      // ICON_LIFE_BARS
    0x7ffc0000, 0x43844004, 0x43844284, 0x43844004, 0x42844284, 0x42844284, 0x40044384, 0x00007ffc,      // ICON_INFO
    0x40008000, 0x10002000, 0x04000800, 0x01000200, 0x00400080, 0x00100020, 0x00040008, 0x00010002,      // ICON_CROSSLINE
    0x00000000, 0x1ff01ff0, 0x18301830, 0x1f001830, 0x03001f00, 0x00000300, 0x03000300, 0x00000000,      // ICON_HELP
    0x3ff00000, 0x2abc3550, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x2aac3554, 0x00003ffc,      // ICON_FILETYPE_ALPHA
    0x3ff00000, 0x201c2010, 0x22442184, 0x28142424, 0x29942814, 0x2ff42994, 0x20042004, 0x00003ffc,      // ICON_FILETYPE_HOME
    0x07fe0000, 0x04020402, 0x7fe20402, 0x44224422, 0x44224422, 0x402047fe, 0x40204020, 0x00007fe0,      // ICON_LAYERS_VISIBLE
    0x07fe0000, 0x04020402, 0x7c020402, 0x44024402, 0x44024402, 0x402047fe, 0x40204020, 0x00007fe0,      // ICON_LAYERS
    0x00000000, 0x40027ffe, 0x7ffe4002, 0x40024002, 0x40024002, 0x40024002, 0x7ffe4002, 0x00000000,      // ICON_WINDOW
    0x09100000, 0x09f00910, 0x09100910, 0x00000910, 0x24a2779e, 0x27a224a2, 0x709e20a2, 0x00000000,      // ICON_HIDPI
    0x3ff00000, 0x201c2010, 0x2a842e84, 0x2e842a84, 0x2ba42004, 0x2aa42aa4, 0x20042ba4, 0x00003ffc,      // ICON_FILETYPE_BINARY
    0x00000000, 0x00000000, 0x00120012, 0x4a5e4bd2, 0x485233d2, 0x00004bd2, 0x00000000, 0x00000000,      // ICON_HEX
    0x01800000, 0x381c0660, 0x23c42004, 0x23c42044, 0x13c82204, 0x08101008, 0x02400420, 0x00000180,      // ICON_SHIELD
    0x007e0000, 0x20023fc2, 0x40227fe2, 0x400a403a, 0x400a400a, 0x400a400a, 0x4008400e, 0x00007ff8,      // ICON_FILE_NEW
    0x00000000, 0x0042007e, 0x40027fc2, 0x44024002, 0x5f024402, 0x44024402, 0x7ffe4002, 0x00000000,      // ICON_FOLDER_ADD
    0x44220000, 0x12482244, 0xf3cf0000, 0x14280420, 0x48122424, 0x08100810, 0x1ff81008, 0x03c00420,      // ICON_ALARM
    0x0aa00000, 0x1ff80aa0, 0x1068700e, 0x1008706e, 0x1008700e, 0x1008700e, 0x0aa01ff8, 0x00000aa0,      // ICON_CPU
    0x07e00000, 0x04201db8, 0x04a01c38, 0x04a01d38, 0x04a01d38, 0x04a01d38, 0x04201d38, 0x000007e0,      // ICON_ROM
    0x00000000, 0x03c00000, 0x3c382ff0, 0x3c04380c, 0x01800000, 0x03c003c0, 0x00000180, 0x00000000,      // ICON_STEP_OVER
    0x01800000, 0x01800180, 0x01800180, 0x03c007e0, 0x00000180, 0x01800000, 0x03c003c0, 0x00000180,      // ICON_STEP_INTO
    0x01800000, 0x07e003c0, 0x01800180, 0x01800180, 0x00000180, 0x01800000, 0x03c003c0, 0x00000180,      // ICON_STEP_OUT
    0x00000000, 0x0ff003c0, 0x181c1c34, 0x303c301c, 0x30003000, 0x1c301800, 0x03c00ff0, 0x00000000,      // ICON_RESTART
    0x00000000, 0x00000000, 0x07e003c0, 0x0ff00ff0, 0x0ff00ff0, 0x03c007e0, 0x00000000, 0x00000000,      // ICON_BREAKPOINT_ON
    0x00000000, 0x00000000, 0x042003c0, 0x08100810, 0x08100810, 0x03c00420, 0x00000000, 0x00000000,      // ICON_BREAKPOINT_OFF
    0x00000000, 0x00000000, 0x1ff81ff8, 0x1ff80000, 0x00001ff8, 0x1ff81ff8, 0x00000000, 0x00000000,      // ICON_BURGER_MENU
    0x00000000, 0x00000000, 0x00880070, 0x0c880088, 0x1e8810f8, 0x3e881288, 0x00000000, 0x00000000,      // ICON_CASE_SENSITIVE
    0x00000000, 0x02000000, 0x07000a80, 0x07001fc0, 0x02000a80, 0x00300030, 0x00000000, 0x00000000,      // ICON_REG_EXP
    0x00000000, 0x0042007e, 0x40027fc2, 0x40024002, 0x40024002, 0x40024002, 0x7ffe4002, 0x00000000,      // ICON_FOLDER
    0x3ff00000, 0x201c2010, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x20042004, 0x00003ffc,      // ICON_FILE
    0x1ff00000, 0x20082008, 0x17d02fe8, 0x05400ba0, 0x09200540, 0x23881010, 0x2fe827c8, 0x00001ff0,      // ICON_SAND_TIMER
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_220
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_221
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_222
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_223
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_224
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_225
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_226
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_227
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_228
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_229
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_230
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_231
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_232
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_233
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_234
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_235
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_236
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_237
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_238
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_239
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_240
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_241
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_242
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_243
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_244
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_245
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_246
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_247
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_248
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_249
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_250
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_251
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_252
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_253
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_254
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,      // ICON_255
};

// NOTE: We keep a pointer to the icons array, useful to point to other sets if required
static unsigned int *guiIconsPtr = guiIcons;

#endif      // !RAYGUI_NO_ICONS && !RAYGUI_CUSTOM_ICONS

#ifndef RAYGUI_ICON_SIZE
    #define RAYGUI_ICON_SIZE             0
#endif

// WARNING: Those values define the total size of the style data array,
// if changed, previous saved styles could become incompatible
#define RAYGUI_MAX_CONTROLS             16      // Maximum number of controls
#define RAYGUI_MAX_PROPS_BASE           16      // Maximum number of base properties
#define RAYGUI_MAX_PROPS_EXTENDED        8      // Maximum number of extended properties

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Gui control property style color element
typedef enum { BORDER = 0, BASE, TEXT, OTHER } GuiPropertyElement;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static GuiState guiState = STATE_NORMAL;        // Gui global state, if !STATE_NORMAL, forces defined state

static Font guiFont = { 0 };                    // Gui current font (WARNING: highly coupled to raylib)
static bool guiLocked = false;                  // Gui lock state (no inputs processed)
static float guiAlpha = 1.0f;                   // Gui controls transparency

static unsigned int guiIconScale = 1;           // Gui icon default scale (if icons enabled)

static bool guiTooltip = false;                 // Tooltip enabled/disabled
static const char *guiTooltipPtr = NULL;        // Tooltip string pointer (string provided by user)

static bool guiSliderDragging = false;          // Gui slider drag state (no inputs processed except dragged slider)
static Rectangle guiSliderActive = { 0 };       // Gui slider active bounds rectangle, used as an unique identifier

static int textBoxCursorIndex = 0;              // Cursor index, shared by all GuiTextBox*()
//static int blinkCursorFrameCounter = 0;       // Frame counter for cursor blinking
static int autoCursorCooldownCounter = 0;       // Cooldown frame counter for automatic cursor movement on key-down
static int autoCursorDelayCounter = 0;          // Delay frame counter for automatic cursor movement

//----------------------------------------------------------------------------------
// Style data array for all gui style properties (allocated on data segment by default)
//
// NOTE 1: First set of BASE properties are generic to all controls but could be individually
// overwritten per control, first set of EXTENDED properties are generic to all controls and
// can not be overwritten individually but custom EXTENDED properties can be used by control
//
// NOTE 2: A new style set could be loaded over this array using GuiLoadStyle(),
// but default gui style could always be recovered with GuiLoadStyleDefault()
//
// guiStyle size is by default: 16*(16 + 8) = 384*4 = 1536 bytes = 1.5 KB
//----------------------------------------------------------------------------------
static unsigned int guiStyle[RAYGUI_MAX_CONTROLS*(RAYGUI_MAX_PROPS_BASE + RAYGUI_MAX_PROPS_EXTENDED)] = { 0 };

static bool guiStyleLoaded = false;         // Style loaded flag for lazy style initialization

//----------------------------------------------------------------------------------
// Standalone Mode Functions Declaration
//
// NOTE: raygui depend on some raylib input and drawing functions
// To use raygui as standalone library, below functions must be defined by the user
//----------------------------------------------------------------------------------


#define KEY_RIGHT           262
#define KEY_LEFT            263
#define KEY_DOWN            264
#define KEY_UP              265
#define KEY_BACKSPACE       259
#define KEY_ENTER           257

// #define MOUSE_LEFT_BUTTON     0

// Input required functions
//-------------------------------------------------------------------------------
Vector2 GetMousePosition(void);
float GetMouseWheelMove(void);
bool IsMouseButtonDown(int button);
bool IsMouseButtonPressed(int button);
bool IsMouseButtonReleased(int button);

bool IsKeyDown(int key);
bool IsKeyPressed(int key);
int GetCharPressed(void);         // -- GuiTextBox(), GuiValueBox()
//-------------------------------------------------------------------------------

// Drawing required functions
//-------------------------------------------------------------------------------
void DrawRectangle(int x, int y, int width, int height, Color color);        // -- GuiDrawRectangle()
void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4); // -- GuiColorPicker()
//-------------------------------------------------------------------------------

// Text required functions
//-------------------------------------------------------------------------------
Font GetFontDefault(void);                            // -- GuiLoadStyleDefault()
Font LoadFontEx(const char *fileName, int fontSize, int *codepoints, int codepointCount); // -- GuiLoadStyle(), load font

Texture2D LoadTextureFromImage(Image image);          // -- GuiLoadStyle(), required to load texture from embedded font atlas image
void SetShapesTexture(Texture2D tex, Rectangle rec);  // -- GuiLoadStyle(), required to set shapes rec to font white rec (optimization)

char *LoadFileText(const char *fileName);             // -- GuiLoadStyle(), required to load charset data
void UnloadFileText(char *text);                      // -- GuiLoadStyle(), required to unload charset data

const char *GetDirectoryPath(const char *filePath);   // -- GuiLoadStyle(), required to find charset/font file from text .rgs

int *LoadCodepoints(const char *text, int *count);    // -- GuiLoadStyle(), required to load required font codepoints list
void UnloadCodepoints(int *codepoints);               // -- GuiLoadStyle(), required to unload codepoints list

unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize); // -- GuiLoadStyle()
//-------------------------------------------------------------------------------

// raylib functions already implemented in raygui
//-------------------------------------------------------------------------------
// Color GetColor(int hexValue);                // Returns a Color struct from hexadecimal value
int ColorToInt(Color color);                 // Returns hexadecimal value for a Color
bool CheckCollisionPointRec(Vector2 point, Rectangle rec);   // Check if point is inside rectangle
const char *TextFormat(const char *text, ...);               // Formatting of text with variables to 'embed'
const char **TextSplit(const char *text, char delimiter, int *count);    // Split text into multiple strings
int TextToInteger(const char *text);         // Get integer value from text

int GetCodepointNext(const char *text, int *codepointSize);  // Get next codepoint in a UTF-8 encoded text
const char *CodepointToUTF8(int codepoint, int *byteSize);   // Encode codepoint into UTF-8 text (char array size returned as parameter)

void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2);  // Draw rectangle vertical gradient
//-------------------------------------------------------------------------------


//--------------------GuiFade--------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
void GuiLoadStyleFromMemory(const unsigned char *fileData, int dataSize);    // Load style from memory (binary only)

int GetTextWidth(const char *text);                      // Gui get text width using gui font and style
Rectangle GetTextBounds(int control, Rectangle bounds);  // Get text bounds considering control bounds
const char *GetTextIcon(const char *text, int *iconId);  // Get text icon if provided and move text cursor

void GuiDrawText(const char *text, Rectangle textBounds, int alignment, Color tint);     // Gui draw text using default font
void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color);   // Gui draw rectangle using default raygui style

const char **GuiTextSplit(const char *text, char delimiter, int *count, int *textRow);   // Split controls text into multiple strings
Vector3 ConvertHSVtoRGB(Vector3 hsv);                    // Convert color data from HSV to RGB
Vector3 ConvertRGBtoHSV(Vector3 rgb);                    // Convert color data from RGB to HSV

int GuiScrollBar(Rectangle bounds, int value, int minValue, int maxValue);   // Scroll bar control, used by GuiScrollPanel()
void GuiTooltip(Rectangle controlRec);                   // Draw tooltip using control rec position

Color GuiFade(Color color, float alpha);         // Fade color by an alpha factor

