


























#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include <ctype.h>      
#include <stdio.h>      
#if defined(_MSC_VER) && _MSC_VER <= 1500 
#include <stddef.h>     
#else
#include <stdint.h>     
#endif

#define IMGUI_DEBUG_NAV_SCORING     0
#define IMGUI_DEBUG_NAV_RECTS       0


#ifdef _MSC_VER
#pragma warning (disable: 4127) 
#pragma warning (disable: 4996) 
#endif


#ifdef __clang__
#pragma clang diagnostic ignored "-Wunknown-pragmas"        
#pragma clang diagnostic ignored "-Wold-style-cast"         
#pragma clang diagnostic ignored "-Wfloat-equal"            
#pragma clang diagnostic ignored "-Wformat-nonliteral"      
#pragma clang diagnostic ignored "-Wexit-time-destructors"  
#pragma clang diagnostic ignored "-Wglobal-constructors"    
#pragma clang diagnostic ignored "-Wsign-conversion"        
#pragma clang diagnostic ignored "-Wformat-pedantic"        
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast" 
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      
#pragma GCC diagnostic ignored "-Wformat"                   
#pragma GCC diagnostic ignored "-Wdouble-promotion"         
#pragma GCC diagnostic ignored "-Wconversion"               
#pragma GCC diagnostic ignored "-Wformat-nonliteral"        
#pragma GCC diagnostic ignored "-Wstrict-overflow"          
#if __GNUC__ >= 8
#pragma GCC diagnostic ignored "-Wclass-memaccess"          
#endif
#endif


static const float NAV_WINDOWING_HIGHLIGHT_DELAY   = 0.20f; 
static const float NAV_WINDOWING_LIST_APPEAR_DELAY = 0.15f; 





static void             SetCurrentWindow(ImGuiWindow* window);
static void             SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond);
static void             SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond);
static void             SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond);
static void             FindHoveredWindow();
static ImGuiWindow*     CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags);
static void             CheckStacksSize(ImGuiWindow* window, bool write);
static ImVec2           CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window, bool snap_on_edges);

static void             AddDrawListToDrawData(ImVector<ImDrawList*>* out_list, ImDrawList* draw_list);
static void             AddWindowToDrawData(ImVector<ImDrawList*>* out_list, ImGuiWindow* window);
static void             AddWindowToSortedBuffer(ImVector<ImGuiWindow*>* out_sorted_windows, ImGuiWindow* window);

static ImRect           GetViewportRect();


static void*            SettingsHandlerWindow_ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name);
static void             SettingsHandlerWindow_ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line);
static void             SettingsHandlerWindow_WriteAll(ImGuiContext* imgui_ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf);


static const char*      GetClipboardTextFn_DefaultImpl(void* user_data);
static void             SetClipboardTextFn_DefaultImpl(void* user_data, const char* text);
static void             ImeSetInputScreenPosFn_DefaultImpl(int x, int y);

namespace ImGui
{
static bool             BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);


static void             NavUpdate();
static void             NavUpdateWindowing();
static void             NavUpdateWindowingList();
static void             NavUpdateMoveResult();
static float            NavUpdatePageUpPageDown(int allowed_dir_flags);
static inline void      NavUpdateAnyRequestFlag();
static void             NavProcessItem(ImGuiWindow* window, const ImRect& nav_bb, const ImGuiID id);
static ImVec2           NavCalcPreferredRefPos();
static void             NavSaveLastChildNavWindow(ImGuiWindow* nav_window);
static ImGuiWindow*     NavRestoreLastChildNavWindow(ImGuiWindow* window);


static void             UpdateMouseInputs();
static void             UpdateMouseWheel();
static void             UpdateManualResize(ImGuiWindow* window, const ImVec2& size_auto_fit, int* border_held, int resize_grip_count, ImU32 resize_grip_col[4]);
}











#ifndef GImGui
ImGuiContext*   GImGui = NULL;
#endif




#ifndef IMGUI_DISABLE_DEFAULT_ALLOCATORS
static void*   MallocWrapper(size_t size, void* user_data)    { (void)user_data; return malloc(size); }
static void    FreeWrapper(void* ptr, void* user_data)        { (void)user_data; free(ptr); }
#else
static void*   MallocWrapper(size_t size, void* user_data)    { (void)user_data; (void)size; IM_ASSERT(0); return NULL; }
static void    FreeWrapper(void* ptr, void* user_data)        { (void)user_data; (void)ptr; IM_ASSERT(0); }
#endif

static void*  (*GImAllocatorAllocFunc)(size_t size, void* user_data) = MallocWrapper;
static void   (*GImAllocatorFreeFunc)(void* ptr, void* user_data) = FreeWrapper;
static void*    GImAllocatorUserData = NULL;





ImGuiStyle::ImGuiStyle()
{
    Alpha                   = 1.0f;             
    WindowPadding           = ImVec2(8,8);      
    WindowRounding          = 7.0f;             
    WindowBorderSize        = 1.0f;             
    WindowMinSize           = ImVec2(32,32);    
    WindowTitleAlign        = ImVec2(0.0f,0.5f);
    ChildRounding           = 0.0f;             
    ChildBorderSize         = 1.0f;             
    PopupRounding           = 0.0f;             
    PopupBorderSize         = 1.0f;             
    FramePadding            = ImVec2(4,3);      
    FrameRounding           = 0.0f;             
    FrameBorderSize         = 0.0f;             
    ItemSpacing             = ImVec2(8,4);      
    ItemInnerSpacing        = ImVec2(4,4);      
    TouchExtraPadding       = ImVec2(0,0);      
    IndentSpacing           = 21.0f;            
    ColumnsMinSpacing       = 6.0f;             
    ScrollbarSize           = 16.0f;            
    ScrollbarRounding       = 9.0f;             
    GrabMinSize             = 10.0f;            
    GrabRounding            = 0.0f;             
    ButtonTextAlign         = ImVec2(0.5f,0.5f);
    DisplayWindowPadding    = ImVec2(20,20);    
    DisplaySafeAreaPadding  = ImVec2(3,3);      
    MouseCursorScale        = 1.0f;             
    AntiAliasedLines        = true;             
    AntiAliasedFill         = true;             
    CurveTessellationTol    = 1.25f;            

    
    ImGui::StyleColorsDark(this);
}



void ImGuiStyle::ScaleAllSizes(float scale_factor)
{
    WindowPadding = ImFloor(WindowPadding * scale_factor);
    WindowRounding = ImFloor(WindowRounding * scale_factor);
    WindowMinSize = ImFloor(WindowMinSize * scale_factor);
    ChildRounding = ImFloor(ChildRounding * scale_factor);
    PopupRounding = ImFloor(PopupRounding * scale_factor);
    FramePadding = ImFloor(FramePadding * scale_factor);
    FrameRounding = ImFloor(FrameRounding * scale_factor);
    ItemSpacing = ImFloor(ItemSpacing * scale_factor);
    ItemInnerSpacing = ImFloor(ItemInnerSpacing * scale_factor);
    TouchExtraPadding = ImFloor(TouchExtraPadding * scale_factor);
    IndentSpacing = ImFloor(IndentSpacing * scale_factor);
    ColumnsMinSpacing = ImFloor(ColumnsMinSpacing * scale_factor);
    ScrollbarSize = ImFloor(ScrollbarSize * scale_factor);
    ScrollbarRounding = ImFloor(ScrollbarRounding * scale_factor);
    GrabMinSize = ImFloor(GrabMinSize * scale_factor);
    GrabRounding = ImFloor(GrabRounding * scale_factor);
    DisplayWindowPadding = ImFloor(DisplayWindowPadding * scale_factor);
    DisplaySafeAreaPadding = ImFloor(DisplaySafeAreaPadding * scale_factor);
    MouseCursorScale = ImFloor(MouseCursorScale * scale_factor);
}

ImGuiIO::ImGuiIO()
{
    
    memset(this, 0, sizeof(*this));

    
    ConfigFlags = 0x00;
    BackendFlags = 0x00;
    DisplaySize = ImVec2(-1.0f, -1.0f);
    DeltaTime = 1.0f/60.0f;
    IniSavingRate = 5.0f;
    IniFilename = "imgui.ini";
    LogFilename = "imgui_log.txt";
    MouseDoubleClickTime = 0.30f;
    MouseDoubleClickMaxDist = 6.0f;
    for (int i = 0; i < ImGuiKey_COUNT; i++)
        KeyMap[i] = -1;
    KeyRepeatDelay = 0.250f;
    KeyRepeatRate = 0.050f;
    UserData = NULL;

    Fonts = NULL;
    FontGlobalScale = 1.0f;
    FontDefault = NULL;
    FontAllowUserScaling = false;
    DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    DisplayVisibleMin = DisplayVisibleMax = ImVec2(0.0f, 0.0f);

    
#ifdef __APPLE__
    ConfigMacOSXBehaviors = true;  
#else
    ConfigMacOSXBehaviors = false;
#endif
    ConfigInputTextCursorBlink = true;
    ConfigResizeWindowsFromEdges = false;

    
    GetClipboardTextFn = GetClipboardTextFn_DefaultImpl;   
    SetClipboardTextFn = SetClipboardTextFn_DefaultImpl;
    ClipboardUserData = NULL;
    ImeSetInputScreenPosFn = ImeSetInputScreenPosFn_DefaultImpl;
    ImeWindowHandle = NULL;

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    RenderDrawListsFn = NULL;
#endif

    
    MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    MousePosPrev = ImVec2(-FLT_MAX, -FLT_MAX);
    MouseDragThreshold = 6.0f;
    for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++) MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++) KeysDownDuration[i]  = KeysDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < IM_ARRAYSIZE(NavInputsDownDuration); i++) NavInputsDownDuration[i] = -1.0f;
}




void ImGuiIO::AddInputCharacter(ImWchar c)
{
    const int n = ImStrlenW(InputCharacters);
    if (n + 1 < IM_ARRAYSIZE(InputCharacters))
    {
        InputCharacters[n] = c;
        InputCharacters[n+1] = '\0';
    }
}

void ImGuiIO::AddInputCharactersUTF8(const char* utf8_chars)
{
    
    const int wchars_buf_len = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar);
    ImWchar wchars[wchars_buf_len];
    ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL);
    for (int i = 0; i < wchars_buf_len && wchars[i] != 0; i++)
        AddInputCharacter(wchars[i]);
}





ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p)
{
    ImVec2 ap = p - a;
    ImVec2 ab_dir = b - a;
    float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    if (dot < 0.0f)
        return a;
    float ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
    if (dot > ab_len_sqr)
        return b;
    return a + ab_dir * dot / ab_len_sqr;
}

bool ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
    bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
    bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
    bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;
    return ((b1 == b2) && (b2 == b3));
}

void ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w)
{
    ImVec2 v0 = b - a;
    ImVec2 v1 = c - a;
    ImVec2 v2 = p - a;
    const float denom = v0.x * v1.y - v1.x * v0.y;
    out_v = (v2.x * v1.y - v1.x * v2.y) / denom;
    out_w = (v0.x * v2.y - v2.x * v0.y) / denom;
    out_u = 1.0f - out_v - out_w;
}

ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
    ImVec2 proj_ab = ImLineClosestPoint(a, b, p);
    ImVec2 proj_bc = ImLineClosestPoint(b, c, p);
    ImVec2 proj_ca = ImLineClosestPoint(c, a, p);
    float dist2_ab = ImLengthSqr(p - proj_ab);
    float dist2_bc = ImLengthSqr(p - proj_bc);
    float dist2_ca = ImLengthSqr(p - proj_ca);
    float m = ImMin(dist2_ab, ImMin(dist2_bc, dist2_ca));
    if (m == dist2_ab)
        return proj_ab;
    if (m == dist2_bc)
        return proj_bc;
    return proj_ca;
}

int ImStricmp(const char* str1, const char* str2)
{
    int d;
    while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; }
    return d;
}

int ImStrnicmp(const char* str1, const char* str2, size_t count)
{
    int d = 0;
    while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; }
    return d;
}

void ImStrncpy(char* dst, const char* src, size_t count)
{
    if (count < 1) return;
    strncpy(dst, src, count);
    dst[count-1] = 0;
}

char* ImStrdup(const char *str)
{
    size_t len = strlen(str) + 1;
    void* buf = ImGui::MemAlloc(len);
    return (char*)memcpy(buf, (const void*)str, len);
}

const char* ImStrchrRange(const char* str, const char* str_end, char c)
{
    for ( ; str < str_end; str++)
        if (*str == c)
            return str;
    return NULL;
}

int ImStrlenW(const ImWchar* str)
{
    int n = 0;
    while (*str++) n++;
    return n;
}

const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin) 
{
    while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n')
        buf_mid_line--;
    return buf_mid_line;
}

const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
    if (!needle_end)
        needle_end = needle + strlen(needle);

    const char un0 = (char)toupper(*needle);
    while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end))
    {
        if (toupper(*haystack) == un0)
        {
            const char* b = needle + 1;
            for (const char* a = haystack + 1; b < needle_end; a++, b++)
                if (toupper(*a) != toupper(*b))
                    break;
            if (b == needle_end)
                return haystack;
        }
        haystack++;
    }
    return NULL;
}


void ImStrTrimBlanks(char* buf)
{
    char* p = buf;
    while (p[0] == ' ' || p[0] == '\t')     
        p++;
    char* p_start = p;
    while (*p != 0)                         
        p++;
    while (p > p_start && (p[-1] == ' ' || p[-1] == '\t'))  
        p--;
    if (p_start != buf)                     
        memmove(buf, p_start, p - p_start);
    buf[p - p_start] = 0;                   
}




#ifndef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS

#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf _vsnprintf
#endif

int ImFormatString(char* buf, size_t buf_size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int w = vsnprintf(buf, buf_size, fmt, args);
    va_end(args);
    if (buf == NULL)
        return w;
    if (w == -1 || w >= (int)buf_size)
        w = (int)buf_size - 1;
    buf[w] = 0;
    return w;
}

int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
{
    int w = vsnprintf(buf, buf_size, fmt, args);
    if (buf == NULL)
        return w;
    if (w == -1 || w >= (int)buf_size)
        w = (int)buf_size - 1;
    buf[w] = 0;
    return w;
}
#endif 



ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
    static ImU32 crc32_lut[256] = { 0 };
    if (!crc32_lut[1])
    {
        const ImU32 polynomial = 0xEDB88320;
        for (ImU32 i = 0; i < 256; i++)
        {
            ImU32 crc = i;
            for (ImU32 j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
            crc32_lut[i] = crc;
        }
    }

    seed = ~seed;
    ImU32 crc = seed;
    const unsigned char* current = (const unsigned char*)data;

    if (data_size > 0)
    {
        
        while (data_size--)
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *current++];
    }
    else
    {
        
        while (unsigned char c = *current++)
        {
            
            
            
            
            if (c == '#' && current[0] == '#' && current[1] == '#')
                crc = seed;
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
        }
    }
    return ~crc;
}

FILE* ImFileOpen(const char* filename, const char* mode)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__GNUC__)
    
    const int filename_wsize = ImTextCountCharsFromUtf8(filename, NULL) + 1;
    const int mode_wsize = ImTextCountCharsFromUtf8(mode, NULL) + 1;
    ImVector<ImWchar> buf;
    buf.resize(filename_wsize + mode_wsize);
    ImTextStrFromUtf8(&buf[0], filename_wsize, filename, NULL);
    ImTextStrFromUtf8(&buf[filename_wsize], mode_wsize, mode, NULL);
    return _wfopen((wchar_t*)&buf[0], (wchar_t*)&buf[filename_wsize]);
#else
    return fopen(filename, mode);
#endif
}



void* ImFileLoadToMemory(const char* filename, const char* file_open_mode, size_t* out_file_size, int padding_bytes)
{
    IM_ASSERT(filename && file_open_mode);
    if (out_file_size)
        *out_file_size = 0;

    FILE* f;
    if ((f = ImFileOpen(filename, file_open_mode)) == NULL)
        return NULL;

    long file_size_signed;
    if (fseek(f, 0, SEEK_END) || (file_size_signed = ftell(f)) == -1 || fseek(f, 0, SEEK_SET))
    {
        fclose(f);
        return NULL;
    }

    size_t file_size = (size_t)file_size_signed;
    void* file_data = ImGui::MemAlloc(file_size + padding_bytes);
    if (file_data == NULL)
    {
        fclose(f);
        return NULL;
    }
    if (fread(file_data, 1, file_size, f) != file_size)
    {
        fclose(f);
        ImGui::MemFree(file_data);
        return NULL;
    }
    if (padding_bytes > 0)
        memset((void*)(((char*)file_data) + file_size), 0, (size_t)padding_bytes);

    fclose(f);
    if (out_file_size)
        *out_file_size = file_size;

    return file_data;
}








int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
    unsigned int c = (unsigned int)-1;
    const unsigned char* str = (const unsigned char*)in_text;
    if (!(*str & 0x80))
    {
        c = (unsigned int)(*str++);
        *out_char = c;
        return 1;
    }
    if ((*str & 0xe0) == 0xc0)
    {
        *out_char = 0xFFFD; 
        if (in_text_end && in_text_end - (const char*)str < 2) return 1;
        if (*str < 0xc2) return 2;
        c = (unsigned int)((*str++ & 0x1f) << 6);
        if ((*str & 0xc0) != 0x80) return 2;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 2;
    }
    if ((*str & 0xf0) == 0xe0)
    {
        *out_char = 0xFFFD; 
        if (in_text_end && in_text_end - (const char*)str < 3) return 1;
        if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
        if (*str == 0xed && str[1] > 0x9f) return 3; 
        c = (unsigned int)((*str++ & 0x0f) << 12);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 3;
    }
    if ((*str & 0xf8) == 0xf0)
    {
        *out_char = 0xFFFD; 
        if (in_text_end && in_text_end - (const char*)str < 4) return 1;
        if (*str > 0xf4) return 4;
        if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
        if (*str == 0xf4 && str[1] > 0x8f) return 4; 
        c = (unsigned int)((*str++ & 0x07) << 18);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 12);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (*str++ & 0x3f);
        
        if ((c & 0xFFFFF800) == 0xD800) return 4;
        *out_char = c;
        return 4;
    }
    *out_char = 0;
    return 0;
}

int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
    ImWchar* buf_out = buf;
    ImWchar* buf_end = buf + buf_size;
    while (buf_out < buf_end-1 && (!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0)
            break;
        if (c < 0x10000)    
            *buf_out++ = (ImWchar)c;
    }
    *buf_out = 0;
    if (in_text_remaining)
        *in_text_remaining = in_text;
    return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
    int char_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0)
            break;
        if (c < 0x10000)
            char_count++;
    }
    return char_count;
}


static inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
    if (c < 0x80)
    {
        buf[0] = (char)c;
        return 1;
    }
    if (c < 0x800)
    {
        if (buf_size < 2) return 0;
        buf[0] = (char)(0xc0 + (c >> 6));
        buf[1] = (char)(0x80 + (c & 0x3f));
        return 2;
    }
    if (c >= 0xdc00 && c < 0xe000)
    {
        return 0;
    }
    if (c >= 0xd800 && c < 0xdc00)
    {
        if (buf_size < 4) return 0;
        buf[0] = (char)(0xf0 + (c >> 18));
        buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
        buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[3] = (char)(0x80 + ((c ) & 0x3f));
        return 4;
    }
    
    {
        if (buf_size < 3) return 0;
        buf[0] = (char)(0xe0 + (c >> 12));
        buf[1] = (char)(0x80 + ((c>> 6) & 0x3f));
        buf[2] = (char)(0x80 + ((c ) & 0x3f));
        return 3;
    }
}


int ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end)
{
    unsigned int dummy = 0;
    return ImTextCharFromUtf8(&dummy, in_text, in_text_end);
}

static inline int ImTextCountUtf8BytesFromChar(unsigned int c)
{
    if (c < 0x80) return 1;
    if (c < 0x800) return 2;
    if (c >= 0xdc00 && c < 0xe000) return 0;
    if (c >= 0xd800 && c < 0xdc00) return 4;
    return 3;
}

int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
    char* buf_out = buf;
    const char* buf_end = buf + buf_size;
    while (buf_out < buf_end-1 && (!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            *buf_out++ = (char)c;
        else
            buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end-buf_out-1), c);
    }
    *buf_out = 0;
    return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
    int bytes_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            bytes_count++;
        else
            bytes_count += ImTextCountUtf8BytesFromChar(c);
    }
    return bytes_count;
}






ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in)
{
    float s = 1.0f/255.0f;
    return ImVec4(
        ((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_A_SHIFT) & 0xFF) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4& in)
{
    ImU32 out;
    out  = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
    return out;
}



void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        ImSwap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        ImSwap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}



void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        
        out_r = out_g = out_b = v;
        return;
    }

    h = ImFmod(h, 1.0f) / (60.0f/360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul)
{
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = style.Colors[idx];
    c.w *= style.Alpha * alpha_mul;
    return ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(const ImVec4& col)
{
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = col;
    c.w *= style.Alpha;
    return ColorConvertFloat4ToU32(c);
}

const ImVec4& ImGui::GetStyleColorVec4(ImGuiCol idx)
{
    ImGuiStyle& style = GImGui->Style;
    return style.Colors[idx];
}

ImU32 ImGui::GetColorU32(ImU32 col)
{
    float style_alpha = GImGui->Style.Alpha;
    if (style_alpha >= 1.0f)
        return col;
    ImU32 a = (col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT;
    a = (ImU32)(a * style_alpha); 
    return (col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
}







static ImVector<ImGuiStorage::Pair>::iterator LowerBound(ImVector<ImGuiStorage::Pair>& data, ImGuiID key)
{
    ImVector<ImGuiStorage::Pair>::iterator first = data.begin();
    ImVector<ImGuiStorage::Pair>::iterator last = data.end();
    size_t count = (size_t)(last - first);
    while (count > 0)
    {
        size_t count2 = count >> 1;
        ImVector<ImGuiStorage::Pair>::iterator mid = first + count2;
        if (mid->key < key)
        {
            first = ++mid;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
    }
    return first;
}


void ImGuiStorage::BuildSortByKey()
{
    struct StaticFunc
    {
        static int IMGUI_CDECL PairCompareByID(const void* lhs, const void* rhs)
        {
            
            if (((const Pair*)lhs)->key > ((const Pair*)rhs)->key) return +1;
            if (((const Pair*)lhs)->key < ((const Pair*)rhs)->key) return -1;
            return 0;
        }
    };
    if (Data.Size > 1)
        ImQsort(Data.Data, (size_t)Data.Size, sizeof(Pair), StaticFunc::PairCompareByID);
}

int ImGuiStorage::GetInt(ImGuiID key, int default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return default_val;
    return it->val_i;
}

bool ImGuiStorage::GetBool(ImGuiID key, bool default_val) const
{
    return GetInt(key, default_val ? 1 : 0) != 0;
}

float ImGuiStorage::GetFloat(ImGuiID key, float default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return default_val;
    return it->val_f;
}

void* ImGuiStorage::GetVoidPtr(ImGuiID key) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return NULL;
    return it->val_p;
}


int* ImGuiStorage::GetIntRef(ImGuiID key, int default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_i;
}

bool* ImGuiStorage::GetBoolRef(ImGuiID key, bool default_val)
{
    return (bool*)GetIntRef(key, default_val ? 1 : 0);
}

float* ImGuiStorage::GetFloatRef(ImGuiID key, float default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_f;
}

void** ImGuiStorage::GetVoidPtrRef(ImGuiID key, void* default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_p;
}


void ImGuiStorage::SetInt(ImGuiID key, int val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_i = val;
}

void ImGuiStorage::SetBool(ImGuiID key, bool val)
{
    SetInt(key, val ? 1 : 0);
}

void ImGuiStorage::SetFloat(ImGuiID key, float val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_f = val;
}

void ImGuiStorage::SetVoidPtr(ImGuiID key, void* val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_p = val;
}

void ImGuiStorage::SetAllInt(int v)
{
    for (int i = 0; i < Data.Size; i++)
        Data[i].val_i = v;
}






ImGuiTextFilter::ImGuiTextFilter(const char* default_filter)
{
    if (default_filter)
    {
        ImStrncpy(InputBuf, default_filter, IM_ARRAYSIZE(InputBuf));
        Build();
    }
    else
    {
        InputBuf[0] = 0;
        CountGrep = 0;
    }
}

bool ImGuiTextFilter::Draw(const char* label, float width)
{
    if (width != 0.0f)
        ImGui::PushItemWidth(width);
    bool value_changed = ImGui::InputText(label, InputBuf, IM_ARRAYSIZE(InputBuf));
    if (width != 0.0f)
        ImGui::PopItemWidth();
    if (value_changed)
        Build();
    return value_changed;
}

void ImGuiTextFilter::TextRange::split(char separator, ImVector<TextRange>* out) const
{
    out->resize(0);
    const char* wb = b;
    const char* we = wb;
    while (we < e)
    {
        if (*we == separator)
        {
            out->push_back(TextRange(wb, we));
            wb = we + 1;
        }
        we++;
    }
    if (wb != we)
        out->push_back(TextRange(wb, we));
}

void ImGuiTextFilter::Build()
{
    Filters.resize(0);
    TextRange input_range(InputBuf, InputBuf+strlen(InputBuf));
    input_range.split(',', &Filters);

    CountGrep = 0;
    for (int i = 0; i != Filters.Size; i++)
    {
        TextRange& f = Filters[i];
        while (f.b < f.e && ImCharIsBlankA(f.b[0]))
            f.b++;
        while (f.e > f.b && ImCharIsBlankA(f.e[-1]))
            f.e--;
        if (f.empty())
            continue;
        if (Filters[i].b[0] != '-')
            CountGrep += 1;
    }
}

bool ImGuiTextFilter::PassFilter(const char* text, const char* text_end) const
{
    if (Filters.empty())
        return true;

    if (text == NULL)
        text = "";

    for (int i = 0; i != Filters.Size; i++)
    {
        const TextRange& f = Filters[i];
        if (f.empty())
            continue;
        if (f.b[0] == '-')
        {
            
            if (ImStristr(text, text_end, f.begin()+1, f.end()) != NULL)
                return false;
        }
        else
        {
            
            if (ImStristr(text, text_end, f.begin(), f.end()) != NULL)
                return true;
        }
    }

    
    if (CountGrep == 0)
        return true;

    return false;
}







#ifndef va_copy
#if defined(__GNUC__) || defined(__clang__)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#else
#define va_copy(dest, src) (dest = src)
#endif
#endif


void ImGuiTextBuffer::appendfv(const char* fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int len = ImFormatStringV(NULL, 0, fmt, args);         
    if (len <= 0)
    {
        va_end(args_copy);
        return;
    }

    const int write_off = Buf.Size;
    const int needed_sz = write_off + len;
    if (write_off + len >= Buf.Capacity)
    {
        int double_capacity = Buf.Capacity * 2;
        Buf.reserve(needed_sz > double_capacity ? needed_sz : double_capacity);
    }

    Buf.resize(needed_sz);
    ImFormatStringV(&Buf[write_off - 1], (size_t)len + 1, fmt, args_copy);
    va_end(args_copy);
}

void ImGuiTextBuffer::appendf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendfv(fmt, args);
    va_end(args);
}






static void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height)
{
    
    
    
    ImGui::SetCursorPosY(pos_y);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;      
    window->DC.PrevLineSize.y = (line_height - GImGui->Style.ItemSpacing.y);    
    if (window->DC.ColumnsSet)
        window->DC.ColumnsSet->LineMinY = window->DC.CursorPos.y;           
}




void ImGuiListClipper::Begin(int count, float items_height)
{
    StartPosY = ImGui::GetCursorPosY();
    ItemsHeight = items_height;
    ItemsCount = count;
    StepNo = 0;
    DisplayEnd = DisplayStart = -1;
    if (ItemsHeight > 0.0f)
    {
        ImGui::CalcListClipping(ItemsCount, ItemsHeight, &DisplayStart, &DisplayEnd); 
        if (DisplayStart > 0)
            SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight, ItemsHeight); 
        StepNo = 2;
    }
}

void ImGuiListClipper::End()
{
    if (ItemsCount < 0)
        return;
    
    if (ItemsCount < INT_MAX)
        SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight, ItemsHeight); 
    ItemsCount = -1;
    StepNo = 3;
}

bool ImGuiListClipper::Step()
{
    if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems)
    {
        ItemsCount = -1;
        return false;
    }
    if (StepNo == 0) 
    {
        DisplayStart = 0;
        DisplayEnd = 1;
        StartPosY = ImGui::GetCursorPosY();
        StepNo = 1;
        return true;
    }
    if (StepNo == 1) 
    {
        if (ItemsCount == 1) { ItemsCount = -1; return false; }
        float items_height = ImGui::GetCursorPosY() - StartPosY;
        IM_ASSERT(items_height > 0.0f);   
        Begin(ItemsCount-1, items_height);
        DisplayStart++;
        DisplayEnd++;
        StepNo = 3;
        return true;
    }
    if (StepNo == 2) 
    {
        IM_ASSERT(DisplayStart >= 0 && DisplayEnd >= 0);
        StepNo = 3;
        return true;
    }
    if (StepNo == 3) 
        End();
    return false;
}







const char* ImGui::FindRenderedTextEnd(const char* text, const char* text_end)
{
    const char* text_display_end = text;
    if (!text_end)
        text_end = (const char*)-1;

    while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
        text_display_end++;
    return text_display_end;
}



void ImGui::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    
    const char* text_display_end;
    if (hide_text_after_hash)
    {
        text_display_end = FindRenderedTextEnd(text, text_end);
    }
    else
    {
        if (!text_end)
            text_end = text + strlen(text); 
        text_display_end = text_end;
    }

    if (text != text_display_end)
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_display_end);
    }
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (!text_end)
        text_end = text + strlen(text); 

    if (text != text_end)
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_end, wrap_width);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_end);
    }
}



void ImGui::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    
    const char* text_display_end = FindRenderedTextEnd(text, text_end);
    const int text_len = (int)(text_display_end - text);
    if (text_len == 0)
        return;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    
    ImVec2 pos = pos_min;
    const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

    const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
    const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
    bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
    if (clip_rect) 
        need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

    
    if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
    if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

    
    if (need_clipping)
    {
        ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
    }
    else
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
    }
    if (g.LogEnabled)
        LogRenderedText(&pos, text, text_display_end);
}


void ImGui::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
    const float border_size = g.Style.FrameBorderSize;
    if (border && border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min+ImVec2(1,1), p_max+ImVec2(1,1), GetColorU32(ImGuiCol_BorderShadow), rounding, ImDrawCornerFlags_All, border_size);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, ImDrawCornerFlags_All, border_size);
    }
}

void ImGui::RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const float border_size = g.Style.FrameBorderSize;
    if (border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min+ImVec2(1,1), p_max+ImVec2(1,1), GetColorU32(ImGuiCol_BorderShadow), rounding, ImDrawCornerFlags_All, border_size);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, ImDrawCornerFlags_All, border_size);
    }
}


void ImGui::RenderArrow(ImVec2 p_min, ImGuiDir dir, float scale)
{
    ImGuiContext& g = *GImGui;

    const float h = g.FontSize * 1.00f;
    float r = h * 0.40f * scale;
    ImVec2 center = p_min + ImVec2(h * 0.50f, h * 0.50f * scale);

    ImVec2 a, b, c;
    switch (dir)
    {
    case ImGuiDir_Up:
    case ImGuiDir_Down:
        if (dir == ImGuiDir_Up) r = -r;
        a = ImVec2(+0.000f,+0.750f) * r;
        b = ImVec2(-0.866f,-0.750f) * r;
        c = ImVec2(+0.866f,-0.750f) * r;
        break;
    case ImGuiDir_Left:
    case ImGuiDir_Right:
        if (dir == ImGuiDir_Left) r = -r;
        a = ImVec2(+0.750f,+0.000f) * r;
        b = ImVec2(-0.750f,+0.866f) * r;
        c = ImVec2(-0.750f,-0.866f) * r;
        break;
    case ImGuiDir_None:
    case ImGuiDir_COUNT:
        IM_ASSERT(0);
        break;
    }

    g.CurrentWindow->DrawList->AddTriangleFilled(center + a, center + b, center + c, GetColorU32(ImGuiCol_Text));
}

void ImGui::RenderBullet(ImVec2 pos)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddCircleFilled(pos, g.FontSize*0.20f, GetColorU32(ImGuiCol_Text), 8);
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col, float sz)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    float thickness = ImMax(sz / 5.0f, 1.0f);
    sz -= thickness*0.5f;
    pos += ImVec2(thickness*0.25f, thickness*0.25f);

    float third = sz / 3.0f;
    float bx = pos.x + third;
    float by = pos.y + sz - third*0.5f;
    window->DrawList->PathLineTo(ImVec2(bx - third, by - third));
    window->DrawList->PathLineTo(ImVec2(bx, by));
    window->DrawList->PathLineTo(ImVec2(bx + third*2, by - third*2));
    window->DrawList->PathStroke(col, false, thickness);
}

void ImGui::RenderNavHighlight(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags)
{
    ImGuiContext& g = *GImGui;
    if (id != g.NavId)
        return;
    if (g.NavDisableHighlight && !(flags & ImGuiNavHighlightFlags_AlwaysDraw))
        return;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->DC.NavHideHighlightOneFrame)
        return;

    float rounding = (flags & ImGuiNavHighlightFlags_NoRounding) ? 0.0f : g.Style.FrameRounding;
    ImRect display_rect = bb;
    display_rect.ClipWith(window->ClipRect);
    if (flags & ImGuiNavHighlightFlags_TypeDefault)
    {
        const float THICKNESS = 2.0f;
        const float DISTANCE = 3.0f + THICKNESS * 0.5f;
        display_rect.Expand(ImVec2(DISTANCE,DISTANCE));
        bool fully_visible = window->ClipRect.Contains(display_rect);
        if (!fully_visible)
            window->DrawList->PushClipRect(display_rect.Min, display_rect.Max);
        window->DrawList->AddRect(display_rect.Min + ImVec2(THICKNESS*0.5f,THICKNESS*0.5f), display_rect.Max - ImVec2(THICKNESS*0.5f,THICKNESS*0.5f), GetColorU32(ImGuiCol_NavHighlight), rounding, ImDrawCornerFlags_All, THICKNESS);
        if (!fully_visible)
            window->DrawList->PopClipRect();
    }
    if (flags & ImGuiNavHighlightFlags_TypeThin)
    {
        window->DrawList->AddRect(display_rect.Min, display_rect.Max, GetColorU32(ImGuiCol_NavHighlight), rounding, ~0, 1.0f);
    }
}






ImGuiWindow::ImGuiWindow(ImGuiContext* context, const char* name)
    : DrawListInst(&context->DrawListSharedData)
{
    Name = ImStrdup(name);
    ID = ImHash(name, 0);
    IDStack.push_back(ID);
    Flags = 0;
    Pos = ImVec2(0.0f, 0.0f);
    Size = SizeFull = ImVec2(0.0f, 0.0f);
    SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    WindowPadding = ImVec2(0.0f, 0.0f);
    WindowRounding = 0.0f;
    WindowBorderSize = 0.0f;
    MoveId = GetID("#MOVE");
    ChildId = 0;
    Scroll = ImVec2(0.0f, 0.0f);
    ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
    ScrollTargetCenterRatio = ImVec2(0.5f, 0.5f);
    ScrollbarSizes = ImVec2(0.0f, 0.0f);
    ScrollbarX = ScrollbarY = false;
    Active = WasActive = false;
    WriteAccessed = false;
    Collapsed = false;
    WantCollapseToggle = false;
    SkipItems = false;
    Appearing = false;
    Hidden = false;
    HasCloseButton = false;
    BeginCount = 0;
    BeginOrderWithinParent = -1;
    BeginOrderWithinContext = -1;
    PopupId = 0;
    AutoFitFramesX = AutoFitFramesY = -1;
    AutoFitOnlyGrows = false;
    AutoFitChildAxises = 0x00;
    AutoPosLastDirection = ImGuiDir_None;
    HiddenFramesRegular = HiddenFramesForResize = 0;
    SetWindowPosAllowFlags = SetWindowSizeAllowFlags = SetWindowCollapsedAllowFlags = ImGuiCond_Always | ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing;
    SetWindowPosVal = SetWindowPosPivot = ImVec2(FLT_MAX, FLT_MAX);

    LastFrameActive = -1;
    ItemWidthDefault = 0.0f;
    FontWindowScale = 1.0f;
    SettingsIdx = -1;

    DrawList = &DrawListInst;
    DrawList->_OwnerName = Name;
    ParentWindow = NULL;
    RootWindow = NULL;
    RootWindowForTitleBarHighlight = NULL;
    RootWindowForNav = NULL;

    NavLastIds[0] = NavLastIds[1] = 0;
    NavRectRel[0] = NavRectRel[1] = ImRect();
    NavLastChildNavWindow = NULL;

    FocusIdxAllCounter = FocusIdxTabCounter = -1;
    FocusIdxAllRequestCurrent = FocusIdxTabRequestCurrent = INT_MAX;
    FocusIdxAllRequestNext = FocusIdxTabRequestNext = INT_MAX;
}

ImGuiWindow::~ImGuiWindow()
{
    IM_ASSERT(DrawList == &DrawListInst);
    IM_DELETE(Name);
    for (int i = 0; i != ColumnsStorage.Size; i++)
        ColumnsStorage[i].~ImGuiColumnsSet();
}

ImGuiID ImGuiWindow::GetID(const char* str, const char* str_end)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
    ImGui::KeepAliveID(id);
    return id;
}

ImGuiID ImGuiWindow::GetID(const void* ptr)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id = ImHash(&ptr, sizeof(void*), seed);
    ImGui::KeepAliveID(id);
    return id;
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const char* str, const char* str_end)
{
    ImGuiID seed = IDStack.back();
    return ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const void* ptr)
{
    ImGuiID seed = IDStack.back();
    return ImHash(&ptr, sizeof(void*), seed);
}


ImGuiID ImGuiWindow::GetIDFromRectangle(const ImRect& r_abs)
{
    ImGuiID seed = IDStack.back();
    const int r_rel[4] = { (int)(r_abs.Min.x - Pos.x), (int)(r_abs.Min.y - Pos.y), (int)(r_abs.Max.x - Pos.x), (int)(r_abs.Max.y - Pos.y) };
    ImGuiID id = ImHash(&r_rel, sizeof(r_rel), seed);
    ImGui::KeepAliveID(id);
    return id;
}

static void SetCurrentWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow = window;
    if (window)
        g.FontSize = g.DrawListSharedData.FontSize = window->CalcFontSize();
}

void ImGui::SetNavID(ImGuiID id, int nav_layer)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.NavWindow);
    IM_ASSERT(nav_layer == 0 || nav_layer == 1);
    g.NavId = id;
    g.NavWindow->NavLastIds[nav_layer] = id;
}

void ImGui::SetNavIDWithRectRel(ImGuiID id, int nav_layer, const ImRect& rect_rel)
{
    ImGuiContext& g = *GImGui;
    SetNavID(id, nav_layer);
    g.NavWindow->NavRectRel[nav_layer] = rect_rel;
    g.NavMousePosDirty = true;
    g.NavDisableHighlight = false;
    g.NavDisableMouseHover = true;
}

void ImGui::SetActiveID(ImGuiID id, ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    g.ActiveIdIsJustActivated = (g.ActiveId != id);
    if (g.ActiveIdIsJustActivated)
    {
        g.ActiveIdTimer = 0.0f;
        g.ActiveIdHasBeenEdited = false;
        if (id != 0)
        {
            g.LastActiveId = id;
            g.LastActiveIdTimer = 0.0f;
        }
    }
    g.ActiveId = id;
    g.ActiveIdAllowNavDirFlags = 0;
    g.ActiveIdAllowOverlap = false;
    g.ActiveIdWindow = window;
    if (id)
    {
        g.ActiveIdIsAlive = id;
        g.ActiveIdSource = (g.NavActivateId == id || g.NavInputId == id || g.NavJustTabbedId == id || g.NavJustMovedToId == id) ? ImGuiInputSource_Nav : ImGuiInputSource_Mouse;
    }
}

void ImGui::SetFocusID(ImGuiID id, ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(id != 0);

    
    const int nav_layer = window->DC.NavLayerCurrent;
    if (g.NavWindow != window)
        g.NavInitRequest = false;
    g.NavId = id;
    g.NavWindow = window;
    g.NavLayer = nav_layer;
    window->NavLastIds[nav_layer] = id;
    if (window->DC.LastItemId == id)
        window->NavRectRel[nav_layer] = ImRect(window->DC.LastItemRect.Min - window->Pos, window->DC.LastItemRect.Max - window->Pos);

    if (g.ActiveIdSource == ImGuiInputSource_Nav)
        g.NavDisableMouseHover = true;
    else
        g.NavDisableHighlight = true;
}

void ImGui::ClearActiveID()
{
    SetActiveID(0, NULL);
}

void ImGui::SetHoveredID(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    g.HoveredId = id;
    g.HoveredIdAllowOverlap = false;
    if (id != 0 && g.HoveredIdPreviousFrame != id)
        g.HoveredIdTimer = 0.0f;
}

ImGuiID ImGui::GetHoveredID()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredId ? g.HoveredId : g.HoveredIdPreviousFrame;
}

void ImGui::KeepAliveID(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    if (g.ActiveId == id)
        g.ActiveIdIsAlive = id;
    if (g.ActiveIdPreviousFrame == id)
        g.ActiveIdPreviousFrameIsAlive = true;
}

void ImGui::MarkItemEdited(ImGuiID id)
{
    
    
    (void)id; 
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.ActiveId == id || g.ActiveId == 0 || g.DragDropActive);
    
    g.ActiveIdHasBeenEdited = true;
    g.CurrentWindow->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_Edited;
}

static inline bool IsWindowContentHoverable(ImGuiWindow* window, ImGuiHoveredFlags flags)
{
    
    
    ImGuiContext& g = *GImGui;
    if (g.NavWindow)
        if (ImGuiWindow* focused_root_window = g.NavWindow->RootWindow)
            if (focused_root_window->WasActive && focused_root_window != window->RootWindow)
            {
                
                
                if (focused_root_window->Flags & ImGuiWindowFlags_Modal)
                    return false;
                if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiHoveredFlags_AllowWhenBlockedByPopup))
                    return false;
            }

    return true;
}


void ImGui::ItemSize(const ImVec2& size, float text_offset_y)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    
    const float line_height = ImMax(window->DC.CurrentLineSize.y, size.y);
    const float text_base_offset = ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
    
    window->DC.CursorPosPrevLine = ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
    window->DC.CursorPos = ImVec2((float)(int)(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x), (float)(int)(window->DC.CursorPos.y + line_height + g.Style.ItemSpacing.y));
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y - g.Style.ItemSpacing.y);
    

    window->DC.PrevLineSize.y = line_height;
    window->DC.PrevLineTextBaseOffset = text_base_offset;
    window->DC.CurrentLineSize.y = window->DC.CurrentLineTextBaseOffset = 0.0f;

    
    if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
        SameLine();
}

void ImGui::ItemSize(const ImRect& bb, float text_offset_y)
{
    ItemSize(bb.GetSize(), text_offset_y);
}




bool ImGui::ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb_arg)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (id != 0)
    {
        
        
        
        
        
        window->DC.NavLayerActiveMaskNext |= window->DC.NavLayerCurrentMask;
        if (g.NavId == id || g.NavAnyRequest)
            if (g.NavWindow->RootWindowForNav == window->RootWindowForNav)
                if (window == g.NavWindow || ((window->Flags | g.NavWindow->Flags) & ImGuiWindowFlags_NavFlattened))
                    NavProcessItem(window, nav_bb_arg ? *nav_bb_arg : bb, id);
    }

    window->DC.LastItemId = id;
    window->DC.LastItemRect = bb;
    window->DC.LastItemStatusFlags = 0;

    
    const bool is_clipped = IsClippedEx(bb, id, false);
    if (is_clipped)
        return false;
    

    
    if (IsMouseHoveringRect(bb.Min, bb.Max))
        window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_HoveredRect;
    return true;
}




bool ImGui::IsItemHovered(ImGuiHoveredFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (g.NavDisableMouseHover && !g.NavDisableHighlight)
        return IsItemFocused();

    
    if (!(window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect))
        return false;
    IM_ASSERT((flags & (ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows)) == 0);   

    
    
    
    
    
    if (g.HoveredRootWindow != window->RootWindow && !(flags & ImGuiHoveredFlags_AllowWhenOverlapped))
        return false;

    
    if (!(flags & ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
        if (g.ActiveId != 0 && g.ActiveId != window->DC.LastItemId && !g.ActiveIdAllowOverlap && g.ActiveId != window->MoveId)
            return false;

    
    if (!IsWindowContentHoverable(window, flags))
        return false;

    
    if ((window->DC.ItemFlags & ImGuiItemFlags_Disabled) && !(flags & ImGuiHoveredFlags_AllowWhenDisabled))
        return false;

    
    if (window->DC.LastItemId == window->MoveId && window->WriteAccessed)
        return false;
    return true;
}


bool ImGui::ItemHoverable(const ImRect& bb, ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId != 0 && g.HoveredId != id && !g.HoveredIdAllowOverlap)
        return false;

    ImGuiWindow* window = g.CurrentWindow;
    if (g.HoveredWindow != window)
        return false;
    if (g.ActiveId != 0 && g.ActiveId != id && !g.ActiveIdAllowOverlap)
        return false;
    if (!IsMouseHoveringRect(bb.Min, bb.Max))
        return false;
    if (g.NavDisableMouseHover || !IsWindowContentHoverable(window, ImGuiHoveredFlags_None))
        return false;
    if (window->DC.ItemFlags & ImGuiItemFlags_Disabled)
        return false;

    SetHoveredID(id);
    return true;
}

bool ImGui::IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (!bb.Overlaps(window->ClipRect))
        if (id == 0 || id != g.ActiveId)
            if (clip_even_when_logged || !g.LogEnabled)
                return true;
    return false;
}

bool ImGui::FocusableItemRegister(ImGuiWindow* window, ImGuiID id, bool tab_stop)
{
    ImGuiContext& g = *GImGui;

    const bool allow_keyboard_focus = (window->DC.ItemFlags & (ImGuiItemFlags_AllowKeyboardFocus | ImGuiItemFlags_Disabled)) == ImGuiItemFlags_AllowKeyboardFocus;
    window->FocusIdxAllCounter++;
    if (allow_keyboard_focus)
        window->FocusIdxTabCounter++;

    
    
    if (tab_stop && (g.ActiveId == id) && window->FocusIdxAllRequestNext == INT_MAX && window->FocusIdxTabRequestNext == INT_MAX && !g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_Tab))
        window->FocusIdxTabRequestNext = window->FocusIdxTabCounter + (g.IO.KeyShift ? (allow_keyboard_focus ? -1 : 0) : +1); 

    if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent)
        return true;
    if (allow_keyboard_focus && window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent)
    {
        g.NavJustTabbedId = id;
        return true;
    }

    return false;
}

void ImGui::FocusableItemUnregister(ImGuiWindow* window)
{
    window->FocusIdxAllCounter--;
    window->FocusIdxTabCounter--;
}

ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y)
{
    ImGuiContext& g = *GImGui;
    ImVec2 content_max;
    if (size.x < 0.0f || size.y < 0.0f)
        content_max = g.CurrentWindow->Pos + GetContentRegionMax();
    if (size.x <= 0.0f)
        size.x = (size.x == 0.0f) ? default_x : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) + size.x;
    if (size.y <= 0.0f)
        size.y = (size.y == 0.0f) ? default_y : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) + size.y;
    return size;
}

float ImGui::CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x)
{
    if (wrap_pos_x < 0.0f)
        return 0.0f;

    ImGuiWindow* window = GetCurrentWindowRead();
    if (wrap_pos_x == 0.0f)
        wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
    else if (wrap_pos_x > 0.0f)
        wrap_pos_x += window->Pos.x - window->Scroll.x; 

    return ImMax(wrap_pos_x - pos.x, 1.0f);
}

void* ImGui::MemAlloc(size_t size)
{
    if (ImGuiContext* ctx = GImGui)
        ctx->IO.MetricsActiveAllocations++;
    return GImAllocatorAllocFunc(size, GImAllocatorUserData);
}

void ImGui::MemFree(void* ptr)
{
    if (ptr) 
        if (ImGuiContext* ctx = GImGui)
            ctx->IO.MetricsActiveAllocations--;
    return GImAllocatorFreeFunc(ptr, GImAllocatorUserData);
}

const char* ImGui::GetClipboardText()
{
    return GImGui->IO.GetClipboardTextFn ? GImGui->IO.GetClipboardTextFn(GImGui->IO.ClipboardUserData) : "";
}

void ImGui::SetClipboardText(const char* text)
{
    if (GImGui->IO.SetClipboardTextFn)
        GImGui->IO.SetClipboardTextFn(GImGui->IO.ClipboardUserData, text);
}

const char* ImGui::GetVersion()
{
    return IMGUI_VERSION;
}



ImGuiContext* ImGui::GetCurrentContext()
{
    return GImGui;
}

void ImGui::SetCurrentContext(ImGuiContext* ctx)
{
#ifdef IMGUI_SET_CURRENT_CONTEXT_FUNC
    IMGUI_SET_CURRENT_CONTEXT_FUNC(ctx); 
#else
    GImGui = ctx;
#endif
}



bool ImGui::DebugCheckVersionAndDataLayout(const char* version, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_vert)
{
    bool error = false;
    if (strcmp(version, IMGUI_VERSION)!=0) { error = true; IM_ASSERT(strcmp(version,IMGUI_VERSION)==0 && "Mismatched version string!");  }
    if (sz_io    != sizeof(ImGuiIO))       { error = true; IM_ASSERT(sz_io    == sizeof(ImGuiIO)      && "Mismatched struct layout!"); }
    if (sz_style != sizeof(ImGuiStyle))    { error = true; IM_ASSERT(sz_style == sizeof(ImGuiStyle)   && "Mismatched struct layout!"); }
    if (sz_vec2  != sizeof(ImVec2))        { error = true; IM_ASSERT(sz_vec2  == sizeof(ImVec2)       && "Mismatched struct layout!"); }
    if (sz_vec4  != sizeof(ImVec4))        { error = true; IM_ASSERT(sz_vec4  == sizeof(ImVec4)       && "Mismatched struct layout!"); }
    if (sz_vert  != sizeof(ImDrawVert))    { error = true; IM_ASSERT(sz_vert  == sizeof(ImDrawVert)   && "Mismatched struct layout!"); }
    return !error;
}

void ImGui::SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void(*free_func)(void* ptr, void* user_data), void* user_data)
{
    GImAllocatorAllocFunc = alloc_func;
    GImAllocatorFreeFunc = free_func;
    GImAllocatorUserData = user_data;
}

ImGuiContext* ImGui::CreateContext(ImFontAtlas* shared_font_atlas)
{
    ImGuiContext* ctx = IM_NEW(ImGuiContext)(shared_font_atlas);
    if (GImGui == NULL)
        SetCurrentContext(ctx);
    Initialize(ctx);
    return ctx;
}

void ImGui::DestroyContext(ImGuiContext* ctx)
{
    if (ctx == NULL)
        ctx = GImGui;
    Shutdown(ctx);
    if (GImGui == ctx)
        SetCurrentContext(NULL);
    IM_DELETE(ctx);
}

ImGuiIO& ImGui::GetIO()
{
    IM_ASSERT(GImGui != NULL && "No current context. Did you call ImGui::CreateContext() or ImGui::SetCurrentContext()?");
    return GImGui->IO;
}

ImGuiStyle& ImGui::GetStyle()
{
    IM_ASSERT(GImGui != NULL && "No current context. Did you call ImGui::CreateContext() or ImGui::SetCurrentContext()?");
    return GImGui->Style;
}


ImDrawData* ImGui::GetDrawData()
{
    ImGuiContext& g = *GImGui;
    return g.DrawData.Valid ? &g.DrawData : NULL;
}

double ImGui::GetTime()
{
    return GImGui->Time;
}

int ImGui::GetFrameCount()
{
    return GImGui->FrameCount;
}

ImDrawList* ImGui::GetOverlayDrawList()
{
    return &GImGui->OverlayDrawList;
}

ImDrawListSharedData* ImGui::GetDrawListSharedData()
{
    return &GImGui->DrawListSharedData;
}

void ImGui::StartMouseMovingWindow(ImGuiWindow* window)
{
    
    ImGuiContext& g = *GImGui;
    FocusWindow(window);
    SetActiveID(window->MoveId, window);
    g.NavDisableHighlight = true;
    g.ActiveIdClickOffset = g.IO.MousePos - window->RootWindow->Pos;
    if (!(window->Flags & ImGuiWindowFlags_NoMove) && !(window->RootWindow->Flags & ImGuiWindowFlags_NoMove))
        g.MovingWindow = window;
}



void ImGui::UpdateMouseMovingWindow()
{
    ImGuiContext& g = *GImGui;
    if (g.MovingWindow != NULL)
    {
        
        
        KeepAliveID(g.ActiveId);
        IM_ASSERT(g.MovingWindow && g.MovingWindow->RootWindow);
        ImGuiWindow* moving_window = g.MovingWindow->RootWindow;
        if (g.IO.MouseDown[0] && IsMousePosValid(&g.IO.MousePos))
        {
            ImVec2 pos = g.IO.MousePos - g.ActiveIdClickOffset;
            if (moving_window->Pos.x != pos.x || moving_window->Pos.y != pos.y)
            {
                MarkIniSettingsDirty(moving_window);
                SetWindowPos(moving_window, pos, ImGuiCond_Always);
            }
            FocusWindow(g.MovingWindow);
        }
        else
        {
            ClearActiveID();
            g.MovingWindow = NULL;
        }
    }
    else
    {
        
        if (g.ActiveIdWindow && g.ActiveIdWindow->MoveId == g.ActiveId)
        {
            KeepAliveID(g.ActiveId);
            if (!g.IO.MouseDown[0])
                ClearActiveID();
        }
    }
}

static bool IsWindowActiveAndVisible(ImGuiWindow* window)
{
    return (window->Active) && (!window->Hidden);
}

static void ImGui::UpdateMouseInputs()
{
    ImGuiContext& g = *GImGui;

    
    if (IsMousePosValid(&g.IO.MousePos) && IsMousePosValid(&g.IO.MousePosPrev))
        g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
    else
        g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
    if (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f)
        g.NavDisableMouseHover = false;

    g.IO.MousePosPrev = g.IO.MousePos;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
    {
        g.IO.MouseClicked[i] = g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
        g.IO.MouseReleased[i] = !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
        g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
        g.IO.MouseDownDuration[i] = g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f ? 0.0f : g.IO.MouseDownDuration[i] + g.IO.DeltaTime) : -1.0f;
        g.IO.MouseDoubleClicked[i] = false;
        if (g.IO.MouseClicked[i])
        {
            if ((float)(g.Time - g.IO.MouseClickedTime[i]) < g.IO.MouseDoubleClickTime)
            {
                ImVec2 delta_from_click_pos = IsMousePosValid(&g.IO.MousePos) ? (g.IO.MousePos - g.IO.MouseClickedPos[i]) : ImVec2(0.0f, 0.0f);
                if (ImLengthSqr(delta_from_click_pos) < g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
                    g.IO.MouseDoubleClicked[i] = true;
                g.IO.MouseClickedTime[i] = -FLT_MAX;    
            }
            else
            {
                g.IO.MouseClickedTime[i] = g.Time;
            }
            g.IO.MouseClickedPos[i] = g.IO.MousePos;
            g.IO.MouseDragMaxDistanceAbs[i] = ImVec2(0.0f, 0.0f);
            g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
        }
        else if (g.IO.MouseDown[i])
        {
            
            ImVec2 delta_from_click_pos = IsMousePosValid(&g.IO.MousePos) ? (g.IO.MousePos - g.IO.MouseClickedPos[i]) : ImVec2(0.0f, 0.0f);
            g.IO.MouseDragMaxDistanceSqr[i] = ImMax(g.IO.MouseDragMaxDistanceSqr[i], ImLengthSqr(delta_from_click_pos));
            g.IO.MouseDragMaxDistanceAbs[i].x = ImMax(g.IO.MouseDragMaxDistanceAbs[i].x, delta_from_click_pos.x < 0.0f ? -delta_from_click_pos.x : delta_from_click_pos.x);
            g.IO.MouseDragMaxDistanceAbs[i].y = ImMax(g.IO.MouseDragMaxDistanceAbs[i].y, delta_from_click_pos.y < 0.0f ? -delta_from_click_pos.y : delta_from_click_pos.y);
        }
        if (g.IO.MouseClicked[i]) 
            g.NavDisableMouseHover = false;
    }
}

void ImGui::UpdateMouseWheel()
{
    ImGuiContext& g = *GImGui;
    if (!g.HoveredWindow || g.HoveredWindow->Collapsed)
        return;
    if (g.IO.MouseWheel == 0.0f && g.IO.MouseWheelH == 0.0f)
        return;

    
    ImGuiWindow* window = g.HoveredWindow;
    ImGuiWindow* scroll_window = window;
    while ((scroll_window->Flags & ImGuiWindowFlags_ChildWindow) && (scroll_window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(scroll_window->Flags & ImGuiWindowFlags_NoScrollbar) && !(scroll_window->Flags & ImGuiWindowFlags_NoInputs) && scroll_window->ParentWindow)
        scroll_window = scroll_window->ParentWindow;
    const bool scroll_allowed = !(scroll_window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(scroll_window->Flags & ImGuiWindowFlags_NoInputs);

    if (g.IO.MouseWheel != 0.0f)
    {
        if (g.IO.KeyCtrl && g.IO.FontAllowUserScaling)
        {
            
            const float new_font_scale = ImClamp(window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
            const float scale = new_font_scale / window->FontWindowScale;
            window->FontWindowScale = new_font_scale;

            const ImVec2 offset = window->Size * (1.0f - scale) * (g.IO.MousePos - window->Pos) / window->Size;
            window->Pos += offset;
            window->Size *= scale;
            window->SizeFull *= scale;
        }
        else if (!g.IO.KeyCtrl && scroll_allowed)
        {
            
            float scroll_amount = 5 * scroll_window->CalcFontSize();
            scroll_amount = (float)(int)ImMin(scroll_amount, (scroll_window->ContentsRegionRect.GetHeight() + scroll_window->WindowPadding.y * 2.0f) * 0.67f);
            SetWindowScrollY(scroll_window, scroll_window->Scroll.y - g.IO.MouseWheel * scroll_amount);
        }
    }
    if (g.IO.MouseWheelH != 0.0f && scroll_allowed && !g.IO.KeyCtrl)
    {
        
        float scroll_amount = scroll_window->CalcFontSize();
        SetWindowScrollX(scroll_window, scroll_window->Scroll.x - g.IO.MouseWheelH * scroll_amount);
    }
}


void ImGui::UpdateHoveredWindowAndCaptureFlags()
{
    ImGuiContext& g = *GImGui;

    
    
    
    
    FindHoveredWindow();

    
    ImGuiWindow* modal_window = GetFrontMostPopupModal();
    if (modal_window)
        if (g.HoveredRootWindow && !IsWindowChildOf(g.HoveredRootWindow, modal_window))
            g.HoveredRootWindow = g.HoveredWindow = NULL;

    
    if (g.IO.ConfigFlags & ImGuiConfigFlags_NoMouse)
        g.HoveredWindow = g.HoveredRootWindow = NULL;

    
    int mouse_earliest_button_down = -1;
    bool mouse_any_down = false;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
    {
        if (g.IO.MouseClicked[i])
            g.IO.MouseDownOwned[i] = (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
        mouse_any_down |= g.IO.MouseDown[i];
        if (g.IO.MouseDown[i])
            if (mouse_earliest_button_down == -1 || g.IO.MouseClickedTime[i] < g.IO.MouseClickedTime[mouse_earliest_button_down])
                mouse_earliest_button_down = i;
    }
    const bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) || g.IO.MouseDownOwned[mouse_earliest_button_down];

    
    
    const bool mouse_dragging_extern_payload = g.DragDropActive && (g.DragDropSourceFlags & ImGuiDragDropFlags_SourceExtern) != 0;
    if (!mouse_avail_to_imgui && !mouse_dragging_extern_payload)
        g.HoveredWindow = g.HoveredRootWindow = NULL;

    
    if (g.WantCaptureMouseNextFrame != -1)
        g.IO.WantCaptureMouse = (g.WantCaptureMouseNextFrame != 0);
    else
        g.IO.WantCaptureMouse = (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) || (!g.OpenPopupStack.empty());

    
    if (g.WantCaptureKeyboardNextFrame != -1)
        g.IO.WantCaptureKeyboard = (g.WantCaptureKeyboardNextFrame != 0);
    else
        g.IO.WantCaptureKeyboard = (g.ActiveId != 0) || (modal_window != NULL);
    if (g.IO.NavActive && (g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) && !(g.IO.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard))
        g.IO.WantCaptureKeyboard = true;

    
    g.IO.WantTextInput = (g.WantTextInputNextFrame != -1) ? (g.WantTextInputNextFrame != 0) : false;
}

void ImGui::NewFrame()
{
    IM_ASSERT(GImGui != NULL && "No current context. Did you call ImGui::CreateContext() or ImGui::SetCurrentContext()?");
    ImGuiContext& g = *GImGui;

    
    
    IM_ASSERT(g.Initialized);
    IM_ASSERT(g.IO.DeltaTime >= 0.0f                                    && "Need a positive DeltaTime (zero is tolerated but will cause some timing issues)");
    IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f  && "Invalid DisplaySize value");
    IM_ASSERT(g.IO.Fonts->Fonts.Size > 0                                && "Font Atlas not built. Did you call io.Fonts->GetTexDataAsRGBA32() / GetTexDataAsAlpha8() ?");
    IM_ASSERT(g.IO.Fonts->Fonts[0]->IsLoaded()                          && "Font Atlas not built. Did you call io.Fonts->GetTexDataAsRGBA32() / GetTexDataAsAlpha8() ?");
    IM_ASSERT(g.Style.CurveTessellationTol > 0.0f                       && "Invalid style setting");
    IM_ASSERT(g.Style.Alpha >= 0.0f && g.Style.Alpha <= 1.0f            && "Invalid style setting. Alpha cannot be negative (allows us to avoid a few clamps in color computations)");
    IM_ASSERT((g.FrameCount == 0 || g.FrameCountEnded == g.FrameCount)  && "Forgot to call Render() or EndFrame() at the end of the previous frame?");
    for (int n = 0; n < ImGuiKey_COUNT; n++)
        IM_ASSERT(g.IO.KeyMap[n] >= -1 && g.IO.KeyMap[n] < IM_ARRAYSIZE(g.IO.KeysDown) && "io.KeyMap[] contains an out of bound value (need to be 0..512, or -1 for unmapped key)");

    
    if (g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)
        IM_ASSERT(g.IO.KeyMap[ImGuiKey_Space] != -1 && "ImGuiKey_Space is not mapped, required for keyboard navigation.");

    
    if (g.IO.ConfigResizeWindowsFromEdges && !(g.IO.BackendFlags & ImGuiBackendFlags_HasMouseCursors))
        g.IO.ConfigResizeWindowsFromEdges = false;

    
    if (!g.SettingsLoaded)
    {
        IM_ASSERT(g.SettingsWindows.empty());
        if (g.IO.IniFilename)
            LoadIniSettingsFromDisk(g.IO.IniFilename);
        g.SettingsLoaded = true;
    }

    
    if (g.SettingsDirtyTimer > 0.0f)
    {
        g.SettingsDirtyTimer -= g.IO.DeltaTime;
        if (g.SettingsDirtyTimer <= 0.0f)
        {
            if (g.IO.IniFilename != NULL)
                SaveIniSettingsToDisk(g.IO.IniFilename);
            else
                g.IO.WantSaveIniSettings = true;  
            g.SettingsDirtyTimer = 0.0f;
        }
    }

    g.Time += g.IO.DeltaTime;
    g.FrameScopeActive = true;
    g.FrameCount += 1;
    g.TooltipOverrideCount = 0;
    g.WindowsActiveCount = 0;

    
    g.IO.Fonts->Locked = true;
    SetCurrentFont(GetDefaultFont());
    IM_ASSERT(g.Font->IsLoaded());
    g.DrawListSharedData.ClipRectFullscreen = ImVec4(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
    g.DrawListSharedData.CurveTessellationTol = g.Style.CurveTessellationTol;

    g.OverlayDrawList.Clear();
    g.OverlayDrawList.PushTextureID(g.IO.Fonts->TexID);
    g.OverlayDrawList.PushClipRectFullScreen();
    g.OverlayDrawList.Flags = (g.Style.AntiAliasedLines ? ImDrawListFlags_AntiAliasedLines : 0) | (g.Style.AntiAliasedFill ? ImDrawListFlags_AntiAliasedFill : 0);

    
    g.DrawData.Clear();

    
    if (g.DragDropActive && g.DragDropPayload.SourceId == g.ActiveId)
        KeepAliveID(g.DragDropPayload.SourceId);

    
    if (!g.HoveredIdPreviousFrame)
        g.HoveredIdTimer = 0.0f;
    if (g.HoveredId)
        g.HoveredIdTimer += g.IO.DeltaTime;
    g.HoveredIdPreviousFrame = g.HoveredId;
    g.HoveredId = 0;
    g.HoveredIdAllowOverlap = false;
    if (g.ActiveIdIsAlive != g.ActiveId && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0)
        ClearActiveID();
    if (g.ActiveId)
        g.ActiveIdTimer += g.IO.DeltaTime;
    g.LastActiveIdTimer += g.IO.DeltaTime;
    g.ActiveIdPreviousFrame = g.ActiveId;
    g.ActiveIdPreviousFrameWindow = g.ActiveIdWindow;
    g.ActiveIdPreviousFrameHasBeenEdited = g.ActiveIdHasBeenEdited;
    g.ActiveIdIsAlive = 0;
    g.ActiveIdPreviousFrameIsAlive = false;
    g.ActiveIdIsJustActivated = false;
    if (g.ScalarAsInputTextId && g.ActiveId != g.ScalarAsInputTextId)
        g.ScalarAsInputTextId = 0;

    
    g.DragDropAcceptIdPrev = g.DragDropAcceptIdCurr;
    g.DragDropAcceptIdCurr = 0;
    g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
    g.DragDropWithinSourceOrTarget = false;

    
    memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration, sizeof(g.IO.KeysDownDuration));
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
        g.IO.KeysDownDuration[i] = g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f ? 0.0f : g.IO.KeysDownDuration[i] + g.IO.DeltaTime) : -1.0f;

    
    NavUpdate();

    
    UpdateMouseInputs();

    
    g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
    g.IO.Framerate = (g.FramerateSecPerFrameAccum > 0.0f) ? (1.0f / (g.FramerateSecPerFrameAccum / (float)IM_ARRAYSIZE(g.FramerateSecPerFrame))) : FLT_MAX;

    
    UpdateMouseMovingWindow();
    UpdateHoveredWindowAndCaptureFlags();

    
    if (GetFrontMostPopupModal() != NULL || (g.NavWindowingTarget != NULL && g.NavWindowingHighlightAlpha > 0.0f))
        g.DimBgRatio = ImMin(g.DimBgRatio + g.IO.DeltaTime * 6.0f, 1.0f);
    else
        g.DimBgRatio = ImMax(g.DimBgRatio - g.IO.DeltaTime * 10.0f, 0.0f);

    g.MouseCursor = ImGuiMouseCursor_Arrow;
    g.WantCaptureMouseNextFrame = g.WantCaptureKeyboardNextFrame = g.WantTextInputNextFrame = -1;
    g.PlatformImePos = ImVec2(1.0f, 1.0f); 

    
    UpdateMouseWheel();

    
    if (g.ActiveId == 0 && g.NavWindow != NULL && g.NavWindow->Active && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs) && !g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_Tab, false))
    {
        if (g.NavId != 0 && g.NavIdTabCounter != INT_MAX)
            g.NavWindow->FocusIdxTabRequestNext = g.NavIdTabCounter + 1 + (g.IO.KeyShift ? -1 : 1);
        else
            g.NavWindow->FocusIdxTabRequestNext = g.IO.KeyShift ? -1 : 0;
    }
    g.NavIdTabCounter = INT_MAX;

    
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        window->WasActive = window->Active;
        window->Active = false;
        window->WriteAccessed = false;
    }

    
    if (g.NavWindow && !g.NavWindow->WasActive)
        FocusFrontMostActiveWindowIgnoringOne(NULL);

    
    
    g.CurrentWindowStack.resize(0);
    g.CurrentPopupStack.resize(0);
    ClosePopupsOverWindow(g.NavWindow);

    
    
    SetNextWindowSize(ImVec2(400,400), ImGuiCond_FirstUseEver);
    Begin("Debug##Default");
}

void ImGui::Initialize(ImGuiContext* context)
{
    ImGuiContext& g = *context;
    IM_ASSERT(!g.Initialized && !g.SettingsLoaded);

    
    ImGuiSettingsHandler ini_handler;
    ini_handler.TypeName = "Window";
    ini_handler.TypeHash = ImHash("Window", 0, 0);
    ini_handler.ReadOpenFn = SettingsHandlerWindow_ReadOpen;
    ini_handler.ReadLineFn = SettingsHandlerWindow_ReadLine;
    ini_handler.WriteAllFn = SettingsHandlerWindow_WriteAll;
    g.SettingsHandlers.push_front(ini_handler);

    g.Initialized = true;
}


void ImGui::Shutdown(ImGuiContext* context)
{
    
    ImGuiContext& g = *context;
    if (g.IO.Fonts && g.FontAtlasOwnedByContext)
        IM_DELETE(g.IO.Fonts);
    g.IO.Fonts = NULL;

    
    if (!g.Initialized)
        return;

    
    if (g.SettingsLoaded && g.IO.IniFilename != NULL)
        SaveIniSettingsToDisk(g.IO.IniFilename);

    
    for (int i = 0; i < g.Windows.Size; i++)
        IM_DELETE(g.Windows[i]);
    g.Windows.clear();
    g.WindowsSortBuffer.clear();
    g.CurrentWindow = NULL;
    g.CurrentWindowStack.clear();
    g.WindowsById.Clear();
    g.NavWindow = NULL;
    g.HoveredWindow = NULL;
    g.HoveredRootWindow = NULL;
    g.ActiveIdWindow = g.ActiveIdPreviousFrameWindow = NULL;
    g.MovingWindow = NULL;
    g.ColorModifiers.clear();
    g.StyleModifiers.clear();
    g.FontStack.clear();
    g.OpenPopupStack.clear();
    g.CurrentPopupStack.clear();
    g.DrawDataBuilder.ClearFreeMemory();
    g.OverlayDrawList.ClearFreeMemory();
    g.PrivateClipboard.clear();
    g.InputTextState.TextW.clear();
    g.InputTextState.InitialText.clear();
    g.InputTextState.TempBuffer.clear();

    for (int i = 0; i < g.SettingsWindows.Size; i++)
        IM_DELETE(g.SettingsWindows[i].Name);
    g.SettingsWindows.clear();
    g.SettingsHandlers.clear();

    if (g.LogFile && g.LogFile != stdout)
    {
        fclose(g.LogFile);
        g.LogFile = NULL;
    }
    g.LogClipboard.clear();

    g.Initialized = false;
}


static int IMGUI_CDECL ChildWindowComparer(const void* lhs, const void* rhs)
{
    const ImGuiWindow* const a = *(const ImGuiWindow* const *)lhs;
    const ImGuiWindow* const b = *(const ImGuiWindow* const *)rhs;
    if (int d = (a->Flags & ImGuiWindowFlags_Popup) - (b->Flags & ImGuiWindowFlags_Popup))
        return d;
    if (int d = (a->Flags & ImGuiWindowFlags_Tooltip) - (b->Flags & ImGuiWindowFlags_Tooltip))
        return d;
    return (a->BeginOrderWithinParent - b->BeginOrderWithinParent);
}

static void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>* out_sorted_windows, ImGuiWindow* window)
{
    out_sorted_windows->push_back(window);
    if (window->Active)
    {
        int count = window->DC.ChildWindows.Size;
        if (count > 1)
            ImQsort(window->DC.ChildWindows.begin(), (size_t)count, sizeof(ImGuiWindow*), ChildWindowComparer);
        for (int i = 0; i < count; i++)
        {
            ImGuiWindow* child = window->DC.ChildWindows[i];
            if (child->Active)
                AddWindowToSortedBuffer(out_sorted_windows, child);
        }
    }
}

static void AddDrawListToDrawData(ImVector<ImDrawList*>* out_list, ImDrawList* draw_list)
{
    if (draw_list->CmdBuffer.empty())
        return;

    
    ImDrawCmd& last_cmd = draw_list->CmdBuffer.back();
    if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL)
    {
        draw_list->CmdBuffer.pop_back();
        if (draw_list->CmdBuffer.empty())
            return;
    }

    
    IM_ASSERT(draw_list->VtxBuffer.Size == 0 || draw_list->_VtxWritePtr == draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
    IM_ASSERT(draw_list->IdxBuffer.Size == 0 || draw_list->_IdxWritePtr == draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
    IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

    
    
    
    
    
    
    
    
    if (sizeof(ImDrawIdx) == 2)
        IM_ASSERT(draw_list->_VtxCurrentIdx < (1 << 16) && "Too many vertices in ImDrawList using 16-bit indices. Read comment above");

    out_list->push_back(draw_list);
}

static void AddWindowToDrawData(ImVector<ImDrawList*>* out_render_list, ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    g.IO.MetricsRenderWindows++;
    AddDrawListToDrawData(out_render_list, window->DrawList);
    for (int i = 0; i < window->DC.ChildWindows.Size; i++)
    {
        ImGuiWindow* child = window->DC.ChildWindows[i];
        if (IsWindowActiveAndVisible(child)) 
            AddWindowToDrawData(out_render_list, child);
    }
}

static void AddWindowToDrawDataSelectLayer(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    if (window->Flags & ImGuiWindowFlags_Tooltip)
        AddWindowToDrawData(&g.DrawDataBuilder.Layers[1], window);
    else
        AddWindowToDrawData(&g.DrawDataBuilder.Layers[0], window);
}

void ImDrawDataBuilder::FlattenIntoSingleLayer()
{
    int n = Layers[0].Size;
    int size = n;
    for (int i = 1; i < IM_ARRAYSIZE(Layers); i++)
        size += Layers[i].Size;
    Layers[0].resize(size);
    for (int layer_n = 1; layer_n < IM_ARRAYSIZE(Layers); layer_n++)
    {
        ImVector<ImDrawList*>& layer = Layers[layer_n];
        if (layer.empty())
            continue;
        memcpy(&Layers[0][n], &layer[0], layer.Size * sizeof(ImDrawList*));
        n += layer.Size;
        layer.resize(0);
    }
}

static void SetupDrawData(ImVector<ImDrawList*>* draw_lists, ImDrawData* draw_data)
{
    ImGuiIO& io = ImGui::GetIO();
    draw_data->Valid = true;
    draw_data->CmdLists = (draw_lists->Size > 0) ? draw_lists->Data : NULL;
    draw_data->CmdListsCount = draw_lists->Size;
    draw_data->TotalVtxCount = draw_data->TotalIdxCount = 0;
    draw_data->DisplayPos = ImVec2(0.0f, 0.0f);
    draw_data->DisplaySize = io.DisplaySize;
    for (int n = 0; n < draw_lists->Size; n++)
    {
        draw_data->TotalVtxCount += draw_lists->Data[n]->VtxBuffer.Size;
        draw_data->TotalIdxCount += draw_lists->Data[n]->IdxBuffer.Size;
    }
}


void ImGui::PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::PopClipRect()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->PopClipRect();
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}


void ImGui::EndFrame()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);
    if (g.FrameCountEnded == g.FrameCount)          
        return;
    IM_ASSERT(g.FrameScopeActive && "Forgot to call ImGui::NewFrame()");

    
    if (g.IO.ImeSetInputScreenPosFn && ImLengthSqr(g.PlatformImeLastPos - g.PlatformImePos) > 0.0001f)
    {
        g.IO.ImeSetInputScreenPosFn((int)g.PlatformImePos.x, (int)g.PlatformImePos.y);
        g.PlatformImeLastPos = g.PlatformImePos;
    }

    
    IM_ASSERT(g.CurrentWindowStack.Size == 1);    
    if (g.CurrentWindow && !g.CurrentWindow->WriteAccessed)
        g.CurrentWindow->Active = false;
    End();

    
    if (g.NavWindowingTarget)
        NavUpdateWindowingList();

    
    if (g.DragDropActive)
    {
        bool is_delivered = g.DragDropPayload.Delivery;
        bool is_elapsed = (g.DragDropPayload.DataFrameCount + 1 < g.FrameCount) && ((g.DragDropSourceFlags & ImGuiDragDropFlags_SourceAutoExpirePayload) || !IsMouseDown(g.DragDropMouseButton));
        if (is_delivered || is_elapsed)
            ClearDragDrop();
    }

    
    if (g.DragDropActive && g.DragDropSourceFrameCount < g.FrameCount)
    {
        g.DragDropWithinSourceOrTarget = true;
        SetTooltip("...");
        g.DragDropWithinSourceOrTarget = false;
    }

    
    if (g.ActiveId == 0 && g.HoveredId == 0)
    {
        if (!g.NavWindow || !g.NavWindow->Appearing) 
        {
            
            if (g.IO.MouseClicked[0])
            {
                if (g.HoveredRootWindow != NULL)
                    StartMouseMovingWindow(g.HoveredWindow);
                else if (g.NavWindow != NULL && GetFrontMostPopupModal() == NULL)
                    FocusWindow(NULL);  
            }

            
            
            if (g.IO.MouseClicked[1])
            {
                
                
                ImGuiWindow* modal = GetFrontMostPopupModal();
                bool hovered_window_above_modal = false;
                if (modal == NULL)
                    hovered_window_above_modal = true;
                for (int i = g.Windows.Size - 1; i >= 0 && hovered_window_above_modal == false; i--)
                {
                    ImGuiWindow* window = g.Windows[i];
                    if (window == modal)
                        break;
                    if (window == g.HoveredWindow)
                        hovered_window_above_modal = true;
                }
                ClosePopupsOverWindow(hovered_window_above_modal ? g.HoveredWindow : modal);
            }
        }
    }

    
    
    g.WindowsSortBuffer.resize(0);
    g.WindowsSortBuffer.reserve(g.Windows.Size);
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        if (window->Active && (window->Flags & ImGuiWindowFlags_ChildWindow))       
            continue;
        AddWindowToSortedBuffer(&g.WindowsSortBuffer, window);
    }

    IM_ASSERT(g.Windows.Size == g.WindowsSortBuffer.Size);  
    g.Windows.swap(g.WindowsSortBuffer);
    g.IO.MetricsActiveWindows = g.WindowsActiveCount;

    
    g.IO.Fonts->Locked = false;

    
    g.IO.MouseWheel = g.IO.MouseWheelH = 0.0f;
    memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
    memset(g.IO.NavInputs, 0, sizeof(g.IO.NavInputs));

    g.FrameScopeActive = false;
    g.FrameCountEnded = g.FrameCount;
}

void ImGui::Render()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);

    if (g.FrameCountEnded != g.FrameCount)
        ImGui::EndFrame();
    g.FrameCountRendered = g.FrameCount;

    
    g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = g.IO.MetricsRenderWindows = 0;
    g.DrawDataBuilder.Clear();
    ImGuiWindow* windows_to_render_front_most[2];
    windows_to_render_front_most[0] = (g.NavWindowingTarget && !(g.NavWindowingTarget->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus)) ? g.NavWindowingTarget->RootWindow : NULL;
    windows_to_render_front_most[1] = g.NavWindowingTarget ? g.NavWindowingList : NULL;
    for (int n = 0; n != g.Windows.Size; n++)
    {
        ImGuiWindow* window = g.Windows[n];
        if (IsWindowActiveAndVisible(window) && (window->Flags & ImGuiWindowFlags_ChildWindow) == 0 && window != windows_to_render_front_most[0] && window != windows_to_render_front_most[1])
            AddWindowToDrawDataSelectLayer(window);
    }
    for (int n = 0; n < IM_ARRAYSIZE(windows_to_render_front_most); n++)
        if (windows_to_render_front_most[n] && IsWindowActiveAndVisible(windows_to_render_front_most[n])) 
            AddWindowToDrawDataSelectLayer(windows_to_render_front_most[n]);
    g.DrawDataBuilder.FlattenIntoSingleLayer();

    
    if (g.IO.MouseDrawCursor)
        RenderMouseCursor(&g.OverlayDrawList, g.IO.MousePos, g.Style.MouseCursorScale, g.MouseCursor);

    if (!g.OverlayDrawList.VtxBuffer.empty())
        AddDrawListToDrawData(&g.DrawDataBuilder.Layers[0], &g.OverlayDrawList);

    
    SetupDrawData(&g.DrawDataBuilder.Layers[0], &g.DrawData);
    g.IO.MetricsRenderVertices = g.DrawData.TotalVtxCount;
    g.IO.MetricsRenderIndices = g.DrawData.TotalIdxCount;

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    if (g.DrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
        g.IO.RenderDrawListsFn(&g.DrawData);
#endif
}



ImVec2 ImGui::CalcTextSize(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
    ImGuiContext& g = *GImGui;

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = FindRenderedTextEnd(text, text_end);      
    else
        text_display_end = text_end;

    ImFont* font = g.Font;
    const float font_size = g.FontSize;
    if (text == text_display_end)
        return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

    
    const float font_scale = font_size / font->FontSize;
    const float character_spacing_x = 1.0f * font_scale;
    if (text_size.x > 0.0f)
        text_size.x -= character_spacing_x;
    text_size.x = (float)(int)(text_size.x + 0.95f);

    return text_size;
}




void ImGui::CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (g.LogEnabled)
    {
        
        *out_items_display_start = 0;
        *out_items_display_end = items_count;
        return;
    }
    if (window->SkipItems)
    {
        *out_items_display_start = *out_items_display_end = 0;
        return;
    }

    
    ImRect unclipped_rect = window->ClipRect;
    if (g.NavMoveRequest)
        unclipped_rect.Add(g.NavScoringRectScreen);

    const ImVec2 pos = window->DC.CursorPos;
    int start = (int)((unclipped_rect.Min.y - pos.y) / items_height);
    int end = (int)((unclipped_rect.Max.y - pos.y) / items_height);

    
    if (g.NavMoveRequest && g.NavMoveClipDir == ImGuiDir_Up)
        start--;
    if (g.NavMoveRequest && g.NavMoveClipDir == ImGuiDir_Down)
        end++;

    start = ImClamp(start, 0, items_count);
    end = ImClamp(end + 1, start, items_count);
    *out_items_display_start = start;
    *out_items_display_end = end;
}





static void FindHoveredWindow()
{
    ImGuiContext& g = *GImGui;

    ImGuiWindow* hovered_window = NULL;
    if (g.MovingWindow && !(g.MovingWindow->Flags & ImGuiWindowFlags_NoInputs))
        hovered_window = g.MovingWindow;

    for (int i = g.Windows.Size - 1; i >= 0 && hovered_window == NULL; i--)
    {
        ImGuiWindow* window = g.Windows[i];
        if (!window->Active || window->Hidden)
            continue;
        if (window->Flags & ImGuiWindowFlags_NoInputs)
            continue;

        
        ImRect bb(window->OuterRectClipped.Min - g.Style.TouchExtraPadding, window->OuterRectClipped.Max + g.Style.TouchExtraPadding);
        if (bb.Contains(g.IO.MousePos))
        {
            if (hovered_window == NULL)
                hovered_window = window;
            if (hovered_window)
                break;
        }
    }

    g.HoveredWindow = hovered_window;
    g.HoveredRootWindow = g.HoveredWindow ? g.HoveredWindow->RootWindow : NULL;

}




bool ImGui::IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
    ImGuiContext& g = *GImGui;

    
    ImRect rect_clipped(r_min, r_max);
    if (clip)
        rect_clipped.ClipWith(g.CurrentWindow->ClipRect);

    
    const ImRect rect_for_touch(rect_clipped.Min - g.Style.TouchExtraPadding, rect_clipped.Max + g.Style.TouchExtraPadding);
    if (!rect_for_touch.Contains(g.IO.MousePos))
        return false;
    return true;
}

int ImGui::GetKeyIndex(ImGuiKey imgui_key)
{
    IM_ASSERT(imgui_key >= 0 && imgui_key < ImGuiKey_COUNT);
    return GImGui->IO.KeyMap[imgui_key];
}


bool ImGui::IsKeyDown(int user_key_index)
{
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(GImGui->IO.KeysDown));
    return GImGui->IO.KeysDown[user_key_index];
}

int ImGui::CalcTypematicPressedRepeatAmount(float t, float t_prev, float repeat_delay, float repeat_rate)
{
    if (t == 0.0f)
        return 1;
    if (t <= repeat_delay || repeat_rate <= 0.0f)
        return 0;
    const int count = (int)((t - repeat_delay) / repeat_rate) - (int)((t_prev - repeat_delay) / repeat_rate);
    return (count > 0) ? count : 0;
}

int ImGui::GetKeyPressedAmount(int key_index, float repeat_delay, float repeat_rate)
{
    ImGuiContext& g = *GImGui;
    if (key_index < 0) return false;
    IM_ASSERT(key_index >= 0 && key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    const float t = g.IO.KeysDownDuration[key_index];
    return CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, repeat_delay, repeat_rate);
}

bool ImGui::IsKeyPressed(int user_key_index, bool repeat)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    const float t = g.IO.KeysDownDuration[user_key_index];
    if (t == 0.0f)
        return true;
    if (repeat && t > g.IO.KeyRepeatDelay)
        return GetKeyPressedAmount(user_key_index, g.IO.KeyRepeatDelay, g.IO.KeyRepeatRate) > 0;
    return false;
}

bool ImGui::IsKeyReleased(int user_key_index)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    return g.IO.KeysDownDurationPrev[user_key_index] >= 0.0f && !g.IO.KeysDown[user_key_index];
}

bool ImGui::IsMouseDown(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDown[button];
}

bool ImGui::IsAnyMouseDown()
{
    ImGuiContext& g = *GImGui;
    for (int n = 0; n < IM_ARRAYSIZE(g.IO.MouseDown); n++)
        if (g.IO.MouseDown[n])
            return true;
    return false;
}

bool ImGui::IsMouseClicked(int button, bool repeat)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    const float t = g.IO.MouseDownDuration[button];
    if (t == 0.0f)
        return true;

    if (repeat && t > g.IO.KeyRepeatDelay)
    {
        float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
        if ((ImFmod(t - delay, rate) > rate*0.5f) != (ImFmod(t - delay - g.IO.DeltaTime, rate) > rate*0.5f))
            return true;
    }

    return false;
}

bool ImGui::IsMouseReleased(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseReleased[button];
}

bool ImGui::IsMouseDoubleClicked(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDoubleClicked[button];
}

bool ImGui::IsMouseDragging(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (!g.IO.MouseDown[button])
        return false;
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    return g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold;
}

ImVec2 ImGui::GetMousePos()
{
    return GImGui->IO.MousePos;
}


ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup()
{
    ImGuiContext& g = *GImGui;
    if (g.CurrentPopupStack.Size > 0)
        return g.OpenPopupStack[g.CurrentPopupStack.Size-1].OpenMousePos;
    return g.IO.MousePos;
}


bool ImGui::IsMousePosValid(const ImVec2* mouse_pos)
{
    if (mouse_pos == NULL)
        mouse_pos = &GImGui->IO.MousePos;
    const float MOUSE_INVALID = -256000.0f;
    return mouse_pos->x >= MOUSE_INVALID && mouse_pos->y >= MOUSE_INVALID;
}


ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    if (g.IO.MouseDown[button])
        if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
            return g.IO.MousePos - g.IO.MouseClickedPos[button];     
    return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    
    g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor()
{
    return GImGui->MouseCursor;
}

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
    GImGui->MouseCursor = cursor_type;
}

void ImGui::CaptureKeyboardFromApp(bool capture)
{
    GImGui->WantCaptureKeyboardNextFrame = capture ? 1 : 0;
}

void ImGui::CaptureMouseFromApp(bool capture)
{
    GImGui->WantCaptureMouseNextFrame = capture ? 1 : 0;
}

bool ImGui::IsItemActive()
{
    ImGuiContext& g = *GImGui;
    if (g.ActiveId)
    {
        ImGuiWindow* window = g.CurrentWindow;
        return g.ActiveId == window->DC.LastItemId;
    }
    return false;
}

bool ImGui::IsItemDeactivated()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    return (g.ActiveIdPreviousFrame == window->DC.LastItemId && g.ActiveIdPreviousFrame != 0 && g.ActiveId != window->DC.LastItemId);
}

bool ImGui::IsItemDeactivatedAfterEdit()
{
    ImGuiContext& g = *GImGui;
    return IsItemDeactivated() && (g.ActiveIdPreviousFrameHasBeenEdited || (g.ActiveId == 0 && g.ActiveIdHasBeenEdited));
}

bool ImGui::IsItemFocused()
{
    ImGuiContext& g = *GImGui;
    return g.NavId && !g.NavDisableHighlight && g.NavId == g.CurrentWindow->DC.LastItemId;
}

bool ImGui::IsItemClicked(int mouse_button)
{
    return IsMouseClicked(mouse_button) && IsItemHovered(ImGuiHoveredFlags_None);
}

bool ImGui::IsAnyItemHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredId != 0 || g.HoveredIdPreviousFrame != 0;
}

bool ImGui::IsAnyItemActive()
{
    ImGuiContext& g = *GImGui;
    return g.ActiveId != 0;
}

bool ImGui::IsAnyItemFocused()
{
    ImGuiContext& g = *GImGui;
    return g.NavId != 0 && !g.NavDisableHighlight;
}

bool ImGui::IsItemVisible()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(window->DC.LastItemRect);
}

bool ImGui::IsItemEdited()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_Edited) != 0;
}


void ImGui::SetItemAllowOverlap()
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId == g.CurrentWindow->DC.LastItemId)
        g.HoveredIdAllowOverlap = true;
    if (g.ActiveId == g.CurrentWindow->DC.LastItemId)
        g.ActiveIdAllowOverlap = true;
}

ImVec2 ImGui::GetItemRectMin()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.Min;
}

ImVec2 ImGui::GetItemRectMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.Max;
}

ImVec2 ImGui::GetItemRectSize()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.GetSize();
}

static ImRect GetViewportRect()
{
    ImGuiContext& g = *GImGui;
    if (g.IO.DisplayVisibleMin.x != g.IO.DisplayVisibleMax.x && g.IO.DisplayVisibleMin.y != g.IO.DisplayVisibleMax.y)
        return ImRect(g.IO.DisplayVisibleMin, g.IO.DisplayVisibleMax);
    return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

static bool ImGui::BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    flags |= ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  

    
    const ImVec2 content_avail = GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); 
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);
    SetNextWindowSize(size);

    
    char title[256];
    if (name)
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%s", parent_window->Name, name);
    else
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%08X", parent_window->Name, id);

    const float backup_border_size = g.Style.ChildBorderSize;
    if (!border)
        g.Style.ChildBorderSize = 0.0f;
    bool ret = Begin(title, NULL, flags);
    g.Style.ChildBorderSize = backup_border_size;

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;
    child_window->AutoFitChildAxises = auto_fit_axises;

    
    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayerActiveMask != 0 || child_window->DC.NavHasScroll))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(id+1, child_window); 
        g.ActiveIdSource = ImGuiInputSource_Nav;
    }
    return ret;
}

bool ImGui::BeginChild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    return BeginChildEx(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool ImGui::BeginChild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    IM_ASSERT(id != 0);
    return BeginChildEx(NULL, id, size_arg, border, extra_flags);
}

void ImGui::EndChild()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   
    if (window->BeginCount > 1)
    {
        End();
    }
    else
    {
        ImVec2 sz = window->Size;
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) 
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
            sz.y = ImMax(4.0f, sz.y);
        End();

        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        if ((window->DC.NavLayerActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
        {
            ItemAdd(bb, window->ChildId);
            RenderNavHighlight(bb, window->ChildId);

            
            if (window->DC.NavLayerActiveMask == 0 && window == g.NavWindow)
                RenderNavHighlight(ImRect(bb.Min - ImVec2(2,2), bb.Max + ImVec2(2,2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
        }
        else
        {
            
            ItemAdd(bb, 0);
        }
    }
}


bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
    PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
    PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
    PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    bool ret = BeginChild(id, size, true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
    PopStyleVar(3);
    PopStyleColor();
    return ret;
}

void ImGui::EndChildFrame()
{
    EndChild();
}


static void CheckStacksSize(ImGuiWindow* window, bool write)
{
    
    ImGuiContext& g = *GImGui;
    int* p_backup = &window->DC.StackSizesBackup[0];
    { int current = window->IDStack.Size;       if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushID/PopID or TreeNode/TreePop Mismatch!");   p_backup++; }    
    { int current = window->DC.GroupStack.Size; if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginGroup/EndGroup Mismatch!");                p_backup++; }    
    { int current = g.CurrentPopupStack.Size;   if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginMenu/EndMenu or BeginPopup/EndPopup Mismatch"); p_backup++;}
    
    { int current = g.ColorModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup >= current && "PushStyleColor/PopStyleColor Mismatch!");       p_backup++; }    
    { int current = g.StyleModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup >= current && "PushStyleVar/PopStyleVar Mismatch!");           p_backup++; }    
    { int current = g.FontStack.Size;           if (write) *p_backup = current; else IM_ASSERT(*p_backup >= current && "PushFont/PopFont Mismatch!");                   p_backup++; }    
    IM_ASSERT(p_backup == window->DC.StackSizesBackup + IM_ARRAYSIZE(window->DC.StackSizesBackup));
}

static void SetWindowConditionAllowFlags(ImGuiWindow* window, ImGuiCond flags, bool enabled)
{
    window->SetWindowPosAllowFlags       = enabled ? (window->SetWindowPosAllowFlags       | flags) : (window->SetWindowPosAllowFlags       & ~flags);
    window->SetWindowSizeAllowFlags      = enabled ? (window->SetWindowSizeAllowFlags      | flags) : (window->SetWindowSizeAllowFlags      & ~flags);
    window->SetWindowCollapsedAllowFlags = enabled ? (window->SetWindowCollapsedAllowFlags | flags) : (window->SetWindowCollapsedAllowFlags & ~flags);
}

ImGuiWindow* ImGui::FindWindowByName(const char* name)
{
    ImGuiContext& g = *GImGui;
    ImGuiID id = ImHash(name, 0);
    return (ImGuiWindow*)g.WindowsById.GetVoidPtr(id);
}

static ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;

    
    ImGuiWindow* window = IM_NEW(ImGuiWindow)(&g, name);
    window->Flags = flags;
    g.WindowsById.SetVoidPtr(window->ID, window);

    
    window->Pos = ImVec2(60, 60);

    
    if (!(flags & ImGuiWindowFlags_NoSavedSettings))
        if (ImGuiWindowSettings* settings = ImGui::FindWindowSettings(window->ID))
        {
            
            window->SettingsIdx = g.SettingsWindows.index_from_pointer(settings);
            SetWindowConditionAllowFlags(window, ImGuiCond_FirstUseEver, false);
            window->Pos = ImFloor(settings->Pos);
            window->Collapsed = settings->Collapsed;
            if (ImLengthSqr(settings->Size) > 0.00001f)
                size = ImFloor(settings->Size);
        }
    window->Size = window->SizeFull = window->SizeFullAtLastBegin = size;
    window->DC.CursorMaxPos = window->Pos; 

    if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0)
    {
        window->AutoFitFramesX = window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = false;
    }
    else
    {
        if (window->Size.x <= 0.0f)
            window->AutoFitFramesX = 2;
        if (window->Size.y <= 0.0f)
            window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
    }

    if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
        g.Windows.insert(g.Windows.begin(), window); 
    else
        g.Windows.push_back(window);
    return window;
}

static ImVec2 CalcSizeAfterConstraint(ImGuiWindow* window, ImVec2 new_size)
{
    ImGuiContext& g = *GImGui;
    if (g.NextWindowData.SizeConstraintCond != 0)
    {
        
        ImRect cr = g.NextWindowData.SizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.NextWindowData.SizeCallback)
        {
            ImGuiSizeCallbackData data;
            data.UserData = g.NextWindowData.SizeCallbackUserData;
            data.Pos = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.NextWindowData.SizeCallback(&data);
            new_size = data.DesiredSize;
        }
    }

    
    if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
    {
        new_size = ImMax(new_size, g.Style.WindowMinSize);
        new_size.y = ImMax(new_size.y, window->TitleBarHeight() + window->MenuBarHeight() + ImMax(0.0f, g.Style.WindowRounding - 1.0f)); 
    }
    return new_size;
}

static ImVec2 CalcSizeContents(ImGuiWindow* window)
{
    ImVec2 sz;
    sz.x = (float)(int)((window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : (window->DC.CursorMaxPos.x - window->Pos.x + window->Scroll.x));
    sz.y = (float)(int)((window->SizeContentsExplicit.y != 0.0f) ? window->SizeContentsExplicit.y : (window->DC.CursorMaxPos.y - window->Pos.y + window->Scroll.y));
    return sz + window->WindowPadding;
}

static ImVec2 CalcSizeAutoFit(ImGuiWindow* window, const ImVec2& size_contents)
{
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    if (window->Flags & ImGuiWindowFlags_Tooltip)
    {
        
        return size_contents;
    }
    else
    {
        
        const bool is_popup = (window->Flags & ImGuiWindowFlags_Popup) != 0;
        const bool is_menu = (window->Flags & ImGuiWindowFlags_ChildMenu) != 0;
        ImVec2 size_min = style.WindowMinSize;
        if (is_popup || is_menu) 
            size_min = ImMin(size_min, ImVec2(4.0f, 4.0f));
        ImVec2 size_auto_fit = ImClamp(size_contents, size_min, ImMax(size_min, g.IO.DisplaySize - style.DisplaySafeAreaPadding * 2.0f));
        ImVec2 size_auto_fit_after_constraint = CalcSizeAfterConstraint(window, size_auto_fit);
        if (size_auto_fit_after_constraint.x < size_contents.x && !(window->Flags & ImGuiWindowFlags_NoScrollbar) && (window->Flags & ImGuiWindowFlags_HorizontalScrollbar))
            size_auto_fit.y += style.ScrollbarSize;
        if (size_auto_fit_after_constraint.y < size_contents.y && !(window->Flags & ImGuiWindowFlags_NoScrollbar))
            size_auto_fit.x += style.ScrollbarSize;
        return size_auto_fit;
    }
}

ImVec2 ImGui::CalcWindowExpectedSize(ImGuiWindow* window)
{
    ImVec2 size_contents = CalcSizeContents(window);
    return CalcSizeAfterConstraint(window, CalcSizeAutoFit(window, size_contents));
}

static float GetScrollMaxX(ImGuiWindow* window)
{
    return ImMax(0.0f, window->SizeContents.x - (window->SizeFull.x - window->ScrollbarSizes.x));
}

static float GetScrollMaxY(ImGuiWindow* window)
{
    return ImMax(0.0f, window->SizeContents.y - (window->SizeFull.y - window->ScrollbarSizes.y));
}

static ImVec2 CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window, bool snap_on_edges)
{
    ImGuiContext& g = *GImGui;
    ImVec2 scroll = window->Scroll;
    if (window->ScrollTarget.x < FLT_MAX)
    {
        float cr_x = window->ScrollTargetCenterRatio.x;
        scroll.x = window->ScrollTarget.x - cr_x * (window->SizeFull.x - window->ScrollbarSizes.x);
    }
    if (window->ScrollTarget.y < FLT_MAX)
    {
        
        float cr_y = window->ScrollTargetCenterRatio.y;
        float target_y = window->ScrollTarget.y;
        if (snap_on_edges && cr_y <= 0.0f && target_y <= window->WindowPadding.y)
            target_y = 0.0f;
        if (snap_on_edges && cr_y >= 1.0f && target_y >= window->SizeContents.y - window->WindowPadding.y + g.Style.ItemSpacing.y)
            target_y = window->SizeContents.y;
        scroll.y = target_y - (1.0f - cr_y) * (window->TitleBarHeight() + window->MenuBarHeight()) - cr_y * (window->SizeFull.y - window->ScrollbarSizes.y);
    }
    scroll = ImMax(scroll, ImVec2(0.0f, 0.0f));
    if (!window->Collapsed && !window->SkipItems)
    {
        scroll.x = ImMin(scroll.x, GetScrollMaxX(window));
        scroll.y = ImMin(scroll.y, GetScrollMaxY(window));
    }
    return scroll;
}

static ImGuiCol GetWindowBgColorIdxFromFlags(ImGuiWindowFlags flags)
{
    if (flags & (ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_Popup))
        return ImGuiCol_PopupBg;
    if (flags & ImGuiWindowFlags_ChildWindow)
        return ImGuiCol_ChildBg;
    return ImGuiCol_WindowBg;
}

static void CalcResizePosSizeFromAnyCorner(ImGuiWindow* window, const ImVec2& corner_target, const ImVec2& corner_norm, ImVec2* out_pos, ImVec2* out_size)
{
    ImVec2 pos_min = ImLerp(corner_target, window->Pos, corner_norm);                
    ImVec2 pos_max = ImLerp(window->Pos + window->Size, corner_target, corner_norm); 
    ImVec2 size_expected = pos_max - pos_min;
    ImVec2 size_constrained = CalcSizeAfterConstraint(window, size_expected);
    *out_pos = pos_min;
    if (corner_norm.x == 0.0f)
        out_pos->x -= (size_constrained.x - size_expected.x);
    if (corner_norm.y == 0.0f)
        out_pos->y -= (size_constrained.y - size_expected.y);
    *out_size = size_constrained;
}

struct ImGuiResizeGripDef
{
    ImVec2  CornerPos;
    ImVec2  InnerDir;
    int     AngleMin12, AngleMax12;
};

const ImGuiResizeGripDef resize_grip_def[4] =
{
    { ImVec2(1,1), ImVec2(-1,-1), 0, 3 }, 
    { ImVec2(0,1), ImVec2(+1,-1), 3, 6 }, 
    { ImVec2(0,0), ImVec2(+1,+1), 6, 9 }, 
    { ImVec2(1,0), ImVec2(-1,+1), 9,12 }, 
};

static ImRect GetResizeBorderRect(ImGuiWindow* window, int border_n, float perp_padding, float thickness)
{
    ImRect rect = window->Rect();
    if (thickness == 0.0f) rect.Max -= ImVec2(1,1);
    if (border_n == 0) return ImRect(rect.Min.x + perp_padding, rect.Min.y,                rect.Max.x - perp_padding, rect.Min.y + thickness);
    if (border_n == 1) return ImRect(rect.Max.x - thickness,    rect.Min.y + perp_padding, rect.Max.x,                rect.Max.y - perp_padding);
    if (border_n == 2) return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness,    rect.Max.x - perp_padding, rect.Max.y);
    if (border_n == 3) return ImRect(rect.Min.x,                rect.Min.y + perp_padding, rect.Min.x + thickness,    rect.Max.y - perp_padding);
    IM_ASSERT(0);
    return ImRect();
}


static void ImGui::UpdateManualResize(ImGuiWindow* window, const ImVec2& size_auto_fit, int* border_held, int resize_grip_count, ImU32 resize_grip_col[4])
{
    ImGuiContext& g = *GImGui;
    ImGuiWindowFlags flags = window->Flags;
    if ((flags & ImGuiWindowFlags_NoResize) || (flags & ImGuiWindowFlags_AlwaysAutoResize) || window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
        return;

    const int resize_border_count = g.IO.ConfigResizeWindowsFromEdges ? 4 : 0;
    const float grip_draw_size = (float)(int)ImMax(g.FontSize * 1.35f, window->WindowRounding + 1.0f + g.FontSize * 0.2f);
    const float grip_hover_size = (float)(int)(grip_draw_size * 0.75f);

    ImVec2 pos_target(FLT_MAX, FLT_MAX);
    ImVec2 size_target(FLT_MAX, FLT_MAX);

    
    PushID("#RESIZE");
    for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
    {
        const ImGuiResizeGripDef& grip = resize_grip_def[resize_grip_n];
        const ImVec2 corner = ImLerp(window->Pos, window->Pos + window->Size, grip.CornerPos);

        
        ImRect resize_rect(corner, corner + grip.InnerDir * grip_hover_size);
        if (resize_rect.Min.x > resize_rect.Max.x) ImSwap(resize_rect.Min.x, resize_rect.Max.x);
        if (resize_rect.Min.y > resize_rect.Max.y) ImSwap(resize_rect.Min.y, resize_rect.Max.y);
        bool hovered, held;
        ButtonBehavior(resize_rect, window->GetID((void*)(intptr_t)resize_grip_n), &hovered, &held, ImGuiButtonFlags_FlattenChildren | ImGuiButtonFlags_NoNavFocus);
        if (hovered || held)
            g.MouseCursor = (resize_grip_n & 1) ? ImGuiMouseCursor_ResizeNESW : ImGuiMouseCursor_ResizeNWSE;

        if (held && g.IO.MouseDoubleClicked[0] && resize_grip_n == 0)
        {
            
            size_target = CalcSizeAfterConstraint(window, size_auto_fit);
            ClearActiveID();
        }
        else if (held)
        {
            
            
            ImVec2 corner_target = g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize() * grip.CornerPos; 
            CalcResizePosSizeFromAnyCorner(window, corner_target, grip.CornerPos, &pos_target, &size_target);
        }
        if (resize_grip_n == 0 || held || hovered)
            resize_grip_col[resize_grip_n] = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);
    }
    for (int border_n = 0; border_n < resize_border_count; border_n++)
    {
        const float BORDER_SIZE = 5.0f;          
        const float BORDER_APPEAR_TIMER = 0.05f; 
        bool hovered, held;
        ImRect border_rect = GetResizeBorderRect(window, border_n, grip_hover_size, BORDER_SIZE);
        ButtonBehavior(border_rect, window->GetID((void*)(intptr_t)(border_n + 4)), &hovered, &held, ImGuiButtonFlags_FlattenChildren);
        if ((hovered && g.HoveredIdTimer > BORDER_APPEAR_TIMER) || held)
        {
            g.MouseCursor = (border_n & 1) ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
            if (held) *border_held = border_n;
        }
        if (held)
        {
            ImVec2 border_target = window->Pos;
            ImVec2 border_posn;
            if (border_n == 0) { border_posn = ImVec2(0, 0); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y); }
            if (border_n == 1) { border_posn = ImVec2(1, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x + BORDER_SIZE); }
            if (border_n == 2) { border_posn = ImVec2(0, 1); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y + BORDER_SIZE); }
            if (border_n == 3) { border_posn = ImVec2(0, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x); }
            CalcResizePosSizeFromAnyCorner(window, border_target, border_posn, &pos_target, &size_target);
        }
    }
    PopID();

    
    if (g.NavWindowingTarget && g.NavWindowingTarget->RootWindow == window)
    {
        ImVec2 nav_resize_delta;
        if (g.NavInputSource == ImGuiInputSource_NavKeyboard && g.IO.KeyShift)
            nav_resize_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard, ImGuiInputReadMode_Down);
        if (g.NavInputSource == ImGuiInputSource_NavGamepad)
            nav_resize_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_PadDPad, ImGuiInputReadMode_Down);
        if (nav_resize_delta.x != 0.0f || nav_resize_delta.y != 0.0f)
        {
            const float NAV_RESIZE_SPEED = 600.0f;
            nav_resize_delta *= ImFloor(NAV_RESIZE_SPEED * g.IO.DeltaTime * ImMin(g.IO.DisplayFramebufferScale.x, g.IO.DisplayFramebufferScale.y));
            g.NavWindowingToggleLayer = false;
            g.NavDisableMouseHover = true;
            resize_grip_col[0] = GetColorU32(ImGuiCol_ResizeGripActive);
            
            size_target = CalcSizeAfterConstraint(window, window->SizeFull + nav_resize_delta);
        }
    }

    
    if (size_target.x != FLT_MAX)
    {
        window->SizeFull = size_target;
        MarkIniSettingsDirty(window);
    }
    if (pos_target.x != FLT_MAX)
    {
        window->Pos = ImFloor(pos_target);
        MarkIniSettingsDirty(window);
    }

    window->Size = window->SizeFull;
}

void ImGui::UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window)
{
    window->ParentWindow = parent_window;
    window->RootWindow = window->RootWindowForTitleBarHighlight = window->RootWindowForNav = window;
    if (parent_window && (flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Tooltip))
        window->RootWindow = parent_window->RootWindow;
    if (parent_window && !(flags & ImGuiWindowFlags_Modal) && (flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)))
        window->RootWindowForTitleBarHighlight = parent_window->RootWindowForTitleBarHighlight;
    while (window->RootWindowForNav->Flags & ImGuiWindowFlags_NavFlattened)
        window->RootWindowForNav = window->RootWindowForNav->ParentWindow;
}








bool ImGui::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    IM_ASSERT(name != NULL);                        
    IM_ASSERT(g.FrameScopeActive);                  
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);   

    
    ImGuiWindow* window = FindWindowByName(name);
    const bool window_just_created = (window == NULL);
    if (window_just_created)
    {
        ImVec2 size_on_first_use = (g.NextWindowData.SizeCond != 0) ? g.NextWindowData.SizeVal : ImVec2(0.0f, 0.0f); 
        window = CreateNewWindow(name, size_on_first_use, flags);
    }

    
    if (flags & ImGuiWindowFlags_NoInputs)
        flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    if (flags & ImGuiWindowFlags_NavFlattened)
        IM_ASSERT(flags & ImGuiWindowFlags_ChildWindow);

    const int current_frame = g.FrameCount;
    const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
    if (first_begin_of_the_frame)
        window->Flags = (ImGuiWindowFlags)flags;
    else
        flags = window->Flags;

    
    ImGuiWindow* parent_window_in_stack = g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back();
    ImGuiWindow* parent_window = first_begin_of_the_frame ? ((flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)) ? parent_window_in_stack : NULL) : window->ParentWindow;
    IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));
    window->HasCloseButton = (p_open != NULL);

    
    bool window_just_activated_by_user = (window->LastFrameActive < current_frame - 1);   
    const bool window_just_appearing_after_hidden_for_resize = (window->HiddenFramesForResize > 0);
    if (flags & ImGuiWindowFlags_Popup)
    {
        ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
        window_just_activated_by_user |= (window->PopupId != popup_ref.PopupId); 
        window_just_activated_by_user |= (window != popup_ref.Window);
    }
    window->Appearing = (window_just_activated_by_user || window_just_appearing_after_hidden_for_resize);
    if (window->Appearing)
        SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, true);

    
    g.CurrentWindowStack.push_back(window);
    SetCurrentWindow(window);
    CheckStacksSize(window, true);
    if (flags & ImGuiWindowFlags_Popup)
    {
        ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
        popup_ref.Window = window;
        g.CurrentPopupStack.push_back(popup_ref);
        window->PopupId = popup_ref.PopupId;
    }

    if (window_just_appearing_after_hidden_for_resize && !(flags & ImGuiWindowFlags_ChildWindow))
        window->NavLastIds[0] = 0;

    
    bool window_pos_set_by_api = false;
    bool window_size_x_set_by_api = false, window_size_y_set_by_api = false;
    if (g.NextWindowData.PosCond)
    {
        window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.NextWindowData.PosCond) != 0;
        if (window_pos_set_by_api && ImLengthSqr(g.NextWindowData.PosPivotVal) > 0.00001f)
        {
            
            
            window->SetWindowPosVal = g.NextWindowData.PosVal;
            window->SetWindowPosPivot = g.NextWindowData.PosPivotVal;
            window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
        }
        else
        {
            SetWindowPos(window, g.NextWindowData.PosVal, g.NextWindowData.PosCond);
        }
    }
    if (g.NextWindowData.SizeCond)
    {
        window_size_x_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.x > 0.0f);
        window_size_y_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.y > 0.0f);
        SetWindowSize(window, g.NextWindowData.SizeVal, g.NextWindowData.SizeCond);
    }
    if (g.NextWindowData.ContentSizeCond)
    {
        
        window->SizeContentsExplicit = g.NextWindowData.ContentSizeVal;
        if (window->SizeContentsExplicit.y != 0.0f)
            window->SizeContentsExplicit.y += window->TitleBarHeight() + window->MenuBarHeight();
    }
    else if (first_begin_of_the_frame)
    {
        window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    }
    if (g.NextWindowData.CollapsedCond)
        SetWindowCollapsed(window, g.NextWindowData.CollapsedVal, g.NextWindowData.CollapsedCond);
    if (g.NextWindowData.FocusCond)
        FocusWindow(window);
    if (window->Appearing)
        SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, false);

    
    if (first_begin_of_the_frame)
    {
        
        const bool window_is_child_tooltip = (flags & ImGuiWindowFlags_ChildWindow) && (flags & ImGuiWindowFlags_Tooltip); 
        UpdateWindowParentAndRootLinks(window, flags, parent_window);

        window->Active = true;
        window->BeginOrderWithinParent = 0;
        window->BeginOrderWithinContext = g.WindowsActiveCount++;
        window->BeginCount = 0;
        window->ClipRect = ImVec4(-FLT_MAX,-FLT_MAX,+FLT_MAX,+FLT_MAX);
        window->LastFrameActive = current_frame;
        window->IDStack.resize(1);

        

        
        window->SizeContents = CalcSizeContents(window);
        if (window->HiddenFramesRegular > 0)
            window->HiddenFramesRegular--;
        if (window->HiddenFramesForResize > 0)
            window->HiddenFramesForResize--;

        
        if (window_just_created && (!window_size_x_set_by_api || !window_size_y_set_by_api))
            window->HiddenFramesForResize = 1;

        
        
        if (window_just_activated_by_user && (flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0)
        {
            window->HiddenFramesForResize = 1;
            if (flags & ImGuiWindowFlags_AlwaysAutoResize)
            {
                if (!window_size_x_set_by_api)
                    window->Size.x = window->SizeFull.x = 0.f;
                if (!window_size_y_set_by_api)
                    window->Size.y = window->SizeFull.y = 0.f;
                window->SizeContents = ImVec2(0.f, 0.f);
            }
        }

        SetCurrentWindow(window);

        
        window->WindowBorderSize = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildBorderSize : ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupBorderSize : style.WindowBorderSize;
        window->WindowPadding = style.WindowPadding;
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_Popup)) && window->WindowBorderSize == 0.0f)
            window->WindowPadding = ImVec2(0.0f, (flags & ImGuiWindowFlags_MenuBar) ? style.WindowPadding.y : 0.0f);
        window->DC.MenuBarOffset.x = ImMax(ImMax(window->WindowPadding.x, style.ItemSpacing.x), g.NextWindowData.MenuBarOffsetMinVal.x);
        window->DC.MenuBarOffset.y = g.NextWindowData.MenuBarOffsetMinVal.y;

        
        
        if (!(flags & ImGuiWindowFlags_NoTitleBar) && !(flags & ImGuiWindowFlags_NoCollapse))
        {
            
            ImRect title_bar_rect = window->TitleBarRect();
            if (g.HoveredWindow == window && g.HoveredId == 0 && g.HoveredIdPreviousFrame == 0 && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) && g.IO.MouseDoubleClicked[0])
                window->WantCollapseToggle = true;
            if (window->WantCollapseToggle)
            {
                window->Collapsed = !window->Collapsed;
                MarkIniSettingsDirty(window);
                FocusWindow(window);
            }
        }
        else
        {
            window->Collapsed = false;
        }
        window->WantCollapseToggle = false;

        

        
        const ImVec2 size_auto_fit = CalcSizeAutoFit(window, window->SizeContents);
        ImVec2 size_full_modified(FLT_MAX, FLT_MAX);
        if ((flags & ImGuiWindowFlags_AlwaysAutoResize) && !window->Collapsed)
        {
            
            if (!window_size_x_set_by_api)
                window->SizeFull.x = size_full_modified.x = size_auto_fit.x;
            if (!window_size_y_set_by_api)
                window->SizeFull.y = size_full_modified.y = size_auto_fit.y;
        }
        else if (window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
        {
            
            
            if (!window_size_x_set_by_api && window->AutoFitFramesX > 0)
                window->SizeFull.x = size_full_modified.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
            if (!window_size_y_set_by_api && window->AutoFitFramesY > 0)
                window->SizeFull.y = size_full_modified.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
            if (!window->Collapsed)
                MarkIniSettingsDirty(window);
        }

        
        window->SizeFull = CalcSizeAfterConstraint(window, window->SizeFull);
        window->Size = window->Collapsed && !(flags & ImGuiWindowFlags_ChildWindow) ? window->TitleBarRect().GetSize() : window->SizeFull;

        

        
        if (!window->Collapsed)
        {
            
            float size_x_for_scrollbars = size_full_modified.x != FLT_MAX ? window->SizeFull.x : window->SizeFullAtLastBegin.x;
            float size_y_for_scrollbars = size_full_modified.y != FLT_MAX ? window->SizeFull.y : window->SizeFullAtLastBegin.y;
            window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) || ((window->SizeContents.y > size_y_for_scrollbars) && !(flags & ImGuiWindowFlags_NoScrollbar));
            window->ScrollbarX = (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) || ((window->SizeContents.x > size_x_for_scrollbars - (window->ScrollbarY ? style.ScrollbarSize : 0.0f)) && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
            if (window->ScrollbarX && !window->ScrollbarY)
                window->ScrollbarY = (window->SizeContents.y > size_y_for_scrollbars - style.ScrollbarSize) && !(flags & ImGuiWindowFlags_NoScrollbar);
            window->ScrollbarSizes = ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
        }

        

        
        if (window_just_activated_by_user)
        {
            window->AutoPosLastDirection = ImGuiDir_None;
            if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
                window->Pos = g.CurrentPopupStack.back().OpenPopupPos;
        }

        
        if (flags & ImGuiWindowFlags_ChildWindow)
        {
            window->BeginOrderWithinParent = parent_window->DC.ChildWindows.Size;
            parent_window->DC.ChildWindows.push_back(window);
            if (!(flags & ImGuiWindowFlags_Popup) && !window_pos_set_by_api && !window_is_child_tooltip)
                window->Pos = parent_window->DC.CursorPos;
        }

        const bool window_pos_with_pivot = (window->SetWindowPosVal.x != FLT_MAX && window->HiddenFramesForResize == 0);
        if (window_pos_with_pivot)
            SetWindowPos(window, ImMax(style.DisplaySafeAreaPadding, window->SetWindowPosVal - window->SizeFull * window->SetWindowPosPivot), 0); 
        else if ((flags & ImGuiWindowFlags_ChildMenu) != 0)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_just_appearing_after_hidden_for_resize)
            window->Pos = FindBestWindowPosForPopup(window);
        else if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api && !window_is_child_tooltip)
            window->Pos = FindBestWindowPosForPopup(window);

        
        if (!(flags & ImGuiWindowFlags_ChildWindow))
        {
            if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f) 
            {
                ImVec2 padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
                window->Pos = ImMax(window->Pos + window->Size, padding) - window->Size;
                window->Pos = ImMin(window->Pos, g.IO.DisplaySize - padding);
            }
        }
        window->Pos = ImFloor(window->Pos);

        
        window->WindowRounding = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildRounding : ((flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupRounding : style.WindowRounding;

        
        window->FocusIdxAllRequestCurrent = (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1) ? INT_MAX : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter+1)) % (window->FocusIdxAllCounter+1);
        window->FocusIdxTabRequestCurrent = (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1) ? INT_MAX : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter+1)) % (window->FocusIdxTabCounter+1);
        window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
        window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;

        
        window->Scroll = CalcNextScrollFromScrollTargetAndClamp(window, true);
        window->ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);

        
        bool want_focus = false;
        if (window_just_activated_by_user && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
            if (!(flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Tooltip)) || (flags & ImGuiWindowFlags_Popup))
                want_focus = true;

        
        int border_held = -1;
        ImU32 resize_grip_col[4] = { 0 };
        const int resize_grip_count = g.IO.ConfigResizeWindowsFromEdges ? 2 : 1; 
        const float grip_draw_size = (float)(int)ImMax(g.FontSize * 1.35f, window->WindowRounding + 1.0f + g.FontSize * 0.2f);
        if (!window->Collapsed)
            UpdateManualResize(window, size_auto_fit, &border_held, resize_grip_count, &resize_grip_col[0]);

        
        if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) && !(flags & ImGuiWindowFlags_AlwaysAutoResize))
            window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
        else
            window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);

        

        
        window->DrawList->Clear();
        window->DrawList->Flags = (g.Style.AntiAliasedLines ? ImDrawListFlags_AntiAliasedLines : 0) | (g.Style.AntiAliasedFill ? ImDrawListFlags_AntiAliasedFill : 0);
        window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
        ImRect viewport_rect(GetViewportRect());
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup) && !window_is_child_tooltip)
            PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
        else
            PushClipRect(viewport_rect.Min, viewport_rect.Max, true);

        
        const bool dim_bg_for_modal = (flags & ImGuiWindowFlags_Modal) && window == GetFrontMostPopupModal() && window->HiddenFramesForResize <= 0;
        const bool dim_bg_for_window_list = g.NavWindowingTargetAnim && (window == g.NavWindowingTargetAnim->RootWindow);
        if (dim_bg_for_modal || dim_bg_for_window_list)
        {
            const ImU32 dim_bg_col = GetColorU32(dim_bg_for_modal ? ImGuiCol_ModalWindowDimBg : ImGuiCol_NavWindowingDimBg, g.DimBgRatio);
            window->DrawList->AddRectFilled(viewport_rect.Min, viewport_rect.Max, dim_bg_col);
        }

        
        if (dim_bg_for_window_list && window == g.NavWindowingTargetAnim)
        {
            ImRect bb = window->Rect();
            bb.Expand(g.FontSize);
            if (!bb.Contains(viewport_rect)) 
                window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(ImGuiCol_NavWindowingHighlight, g.NavWindowingHighlightAlpha * 0.25f), g.Style.WindowRounding);
        }

        
        const float window_rounding = window->WindowRounding;
        const float window_border_size = window->WindowBorderSize;
        const ImGuiWindow* window_to_highlight = g.NavWindowingTarget ? g.NavWindowingTarget : g.NavWindow;
        const bool title_bar_is_highlight = want_focus || (window_to_highlight && window->RootWindowForTitleBarHighlight == window_to_highlight->RootWindowForTitleBarHighlight);
        const ImRect title_bar_rect = window->TitleBarRect();
        if (window->Collapsed)
        {
            
            float backup_border_size = style.FrameBorderSize;
            g.Style.FrameBorderSize = window->WindowBorderSize;
            ImU32 title_bar_col = GetColorU32((title_bar_is_highlight && !g.NavDisableHighlight) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBgCollapsed);
            RenderFrame(title_bar_rect.Min, title_bar_rect.Max, title_bar_col, true, window_rounding);
            g.Style.FrameBorderSize = backup_border_size;
        }
        else
        {
            
            ImU32 bg_col = GetColorU32(GetWindowBgColorIdxFromFlags(flags));
            if (g.NextWindowData.BgAlphaCond != 0)
            {
                bg_col = (bg_col & ~IM_COL32_A_MASK) | (IM_F32_TO_INT8_SAT(g.NextWindowData.BgAlphaVal) << IM_COL32_A_SHIFT);
                g.NextWindowData.BgAlphaCond = 0;
            }
            window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()), window->Pos + window->Size, bg_col, window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Bot);

            
            ImU32 title_bar_col = GetColorU32(window->Collapsed ? ImGuiCol_TitleBgCollapsed : title_bar_is_highlight ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg);
            if (!(flags & ImGuiWindowFlags_NoTitleBar))
                window->DrawList->AddRectFilled(title_bar_rect.Min, title_bar_rect.Max, title_bar_col, window_rounding, ImDrawCornerFlags_Top);

            
            if (flags & ImGuiWindowFlags_MenuBar)
            {
                ImRect menu_bar_rect = window->MenuBarRect();
                menu_bar_rect.ClipWith(window->Rect());  
                window->DrawList->AddRectFilled(menu_bar_rect.Min, menu_bar_rect.Max, GetColorU32(ImGuiCol_MenuBarBg), (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f, ImDrawCornerFlags_Top);
                if (style.FrameBorderSize > 0.0f && menu_bar_rect.Max.y < window->Pos.y + window->Size.y)
                    window->DrawList->AddLine(menu_bar_rect.GetBL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);
            }

            
            if (window->ScrollbarX)
                Scrollbar(ImGuiLayoutType_Horizontal);
            if (window->ScrollbarY)
                Scrollbar(ImGuiLayoutType_Vertical);

            
            if (!(flags & ImGuiWindowFlags_NoResize))
            {
                for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
                {
                    const ImGuiResizeGripDef& grip = resize_grip_def[resize_grip_n];
                    const ImVec2 corner = ImLerp(window->Pos, window->Pos + window->Size, grip.CornerPos);
                    window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(window_border_size, grip_draw_size) : ImVec2(grip_draw_size, window_border_size)));
                    window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(grip_draw_size, window_border_size) : ImVec2(window_border_size, grip_draw_size)));
                    window->DrawList->PathArcToFast(ImVec2(corner.x + grip.InnerDir.x * (window_rounding + window_border_size), corner.y + grip.InnerDir.y * (window_rounding + window_border_size)), window_rounding, grip.AngleMin12, grip.AngleMax12);
                    window->DrawList->PathFillConvex(resize_grip_col[resize_grip_n]);
                }
            }

            
            if (window_border_size > 0.0f)
                window->DrawList->AddRect(window->Pos, window->Pos + window->Size, GetColorU32(ImGuiCol_Border), window_rounding, ImDrawCornerFlags_All, window_border_size);
            if (border_held != -1)
            {
                ImRect border = GetResizeBorderRect(window, border_held, grip_draw_size, 0.0f);
                window->DrawList->AddLine(border.Min, border.Max, GetColorU32(ImGuiCol_SeparatorActive), ImMax(1.0f, window_border_size));
            }
            if (style.FrameBorderSize > 0 && !(flags & ImGuiWindowFlags_NoTitleBar))
                window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(style.WindowBorderSize, -1), title_bar_rect.GetBR() + ImVec2(-style.WindowBorderSize, -1), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);
        }

        
        if (g.NavWindowingTargetAnim == window)
        {
            float rounding = ImMax(window->WindowRounding, g.Style.WindowRounding);
            ImRect bb = window->Rect();
            bb.Expand(g.FontSize);
            if (bb.Contains(viewport_rect)) 
            {
                bb.Expand(-g.FontSize - 1.0f);
                rounding = window->WindowRounding;
            }
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_NavWindowingHighlight, g.NavWindowingHighlightAlpha), rounding, ~0, 3.0f);
        }

        
        window->SizeFullAtLastBegin = window->SizeFull;

        
        
        window->ContentsRegionRect.Min.x = window->Pos.x - window->Scroll.x + window->WindowPadding.x;
        window->ContentsRegionRect.Min.y = window->Pos.y - window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
        window->ContentsRegionRect.Max.x = window->Pos.x - window->Scroll.x - window->WindowPadding.x + (window->SizeContentsExplicit.x != 0.0f ? window->SizeContentsExplicit.x : (window->Size.x - window->ScrollbarSizes.x));
        window->ContentsRegionRect.Max.y = window->Pos.y - window->Scroll.y - window->WindowPadding.y + (window->SizeContentsExplicit.y != 0.0f ? window->SizeContentsExplicit.y : (window->Size.y - window->ScrollbarSizes.y));

        
        
        window->DC.Indent.x = 0.0f + window->WindowPadding.x - window->Scroll.x;
        window->DC.GroupOffset.x = 0.0f;
        window->DC.ColumnsOffset.x = 0.0f;
        window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.Indent.x + window->DC.ColumnsOffset.x, window->TitleBarHeight() + window->MenuBarHeight() + window->WindowPadding.y - window->Scroll.y);
        window->DC.CursorPos = window->DC.CursorStartPos;
        window->DC.CursorPosPrevLine = window->DC.CursorPos;
        window->DC.CursorMaxPos = window->DC.CursorStartPos;
        window->DC.CurrentLineSize = window->DC.PrevLineSize = ImVec2(0.0f, 0.0f);
        window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;
        window->DC.NavHideHighlightOneFrame = false;
        window->DC.NavHasScroll = (GetScrollMaxY() > 0.0f);
        window->DC.NavLayerActiveMask = window->DC.NavLayerActiveMaskNext;
        window->DC.NavLayerActiveMaskNext = 0x00;
        window->DC.MenuBarAppending = false;
        window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
        window->DC.ChildWindows.resize(0);
        window->DC.LayoutType = ImGuiLayoutType_Vertical;
        window->DC.ParentLayoutType = parent_window ? parent_window->DC.LayoutType : ImGuiLayoutType_Vertical;
        window->DC.ItemFlags = parent_window ? parent_window->DC.ItemFlags : ImGuiItemFlags_Default_;
        window->DC.ItemWidth = window->ItemWidthDefault;
        window->DC.TextWrapPos = -1.0f; 
        window->DC.ItemFlagsStack.resize(0);
        window->DC.ItemWidthStack.resize(0);
        window->DC.TextWrapPosStack.resize(0);
        window->DC.ColumnsSet = NULL;
        window->DC.TreeDepth = 0;
        window->DC.TreeDepthMayJumpToParentOnPop = 0x00;
        window->DC.StateStorage = &window->StateStorage;
        window->DC.GroupStack.resize(0);
        window->MenuColumns.Update(3, style.ItemSpacing.x, window_just_activated_by_user);

        if ((flags & ImGuiWindowFlags_ChildWindow) && (window->DC.ItemFlags != parent_window->DC.ItemFlags))
        {
            window->DC.ItemFlags = parent_window->DC.ItemFlags;
            window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);
        }

        if (window->AutoFitFramesX > 0)
            window->AutoFitFramesX--;
        if (window->AutoFitFramesY > 0)
            window->AutoFitFramesY--;

        
        if (want_focus)
        {
            FocusWindow(window);
            NavInitWindow(window, false);
        }

        
        if (!(flags & ImGuiWindowFlags_NoTitleBar))
        {
            
            const ImGuiItemFlags item_flags_backup = window->DC.ItemFlags;
            window->DC.ItemFlags |= ImGuiItemFlags_NoNavDefaultFocus;
            window->DC.NavLayerCurrent++;
            window->DC.NavLayerCurrentMask <<= 1;

            
            if (!(flags & ImGuiWindowFlags_NoCollapse))
                if (CollapseButton(window->GetID("#COLLAPSE"), window->Pos))
                    window->WantCollapseToggle = true; 

            
            if (p_open != NULL)
            {
                const float pad = style.FramePadding.y;
                const float rad = g.FontSize * 0.5f;
                if (CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-pad - rad, pad + rad), rad + 1))
                    *p_open = false;
            }

            window->DC.NavLayerCurrent--;
            window->DC.NavLayerCurrentMask >>= 1;
            window->DC.ItemFlags = item_flags_backup;

            
            ImVec2 text_size = CalcTextSize(name, NULL, true);
            ImRect text_r = title_bar_rect;
            float pad_left = (flags & ImGuiWindowFlags_NoCollapse) ? style.FramePadding.x : (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x);
            float pad_right = (p_open == NULL)                     ? style.FramePadding.x : (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x);
            if (style.WindowTitleAlign.x > 0.0f)
                pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
            text_r.Min.x += pad_left;
            text_r.Max.x -= pad_right;
            ImRect clip_rect = text_r;
            clip_rect.Max.x = window->Pos.x + window->Size.x - (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x); 
            RenderTextClipped(text_r.Min, text_r.Max, name, NULL, &text_size, style.WindowTitleAlign, &clip_rect);
        }

        
        window->OuterRectClipped = window->Rect();
        window->OuterRectClipped.ClipWith(window->ClipRect);

        
        
        
        

        
        
        
        window->InnerMainRect.Min.x = title_bar_rect.Min.x + window->WindowBorderSize;
        window->InnerMainRect.Min.y = title_bar_rect.Max.y + window->MenuBarHeight() + (((flags & ImGuiWindowFlags_MenuBar) || !(flags & ImGuiWindowFlags_NoTitleBar)) ? style.FrameBorderSize : window->WindowBorderSize);
        window->InnerMainRect.Max.x = window->Pos.x + window->Size.x - window->ScrollbarSizes.x - window->WindowBorderSize;
        window->InnerMainRect.Max.y = window->Pos.y + window->Size.y - window->ScrollbarSizes.y - window->WindowBorderSize;
        

        
        
        window->InnerClipRect.Min.x = ImFloor(0.5f + window->InnerMainRect.Min.x + ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - window->WindowBorderSize)));
        window->InnerClipRect.Min.y = ImFloor(0.5f + window->InnerMainRect.Min.y);
        window->InnerClipRect.Max.x = ImFloor(0.5f + window->InnerMainRect.Max.x - ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - window->WindowBorderSize)));
        window->InnerClipRect.Max.y = ImFloor(0.5f + window->InnerMainRect.Max.y);

        
        window->DC.LastItemId = window->MoveId;
        window->DC.LastItemStatusFlags = IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max, false) ? ImGuiItemStatusFlags_HoveredRect : 0;
        window->DC.LastItemRect = title_bar_rect;
    }

    PushClipRect(window->InnerClipRect.Min, window->InnerClipRect.Max, true);

    
    if (first_begin_of_the_frame)
        window->WriteAccessed = false;

    window->BeginCount++;
    g.NextWindowData.Clear();

    if (flags & ImGuiWindowFlags_ChildWindow)
    {
        
        
        IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);

        if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
            if (window->OuterRectClipped.Min.x >= window->OuterRectClipped.Max.x || window->OuterRectClipped.Min.y >= window->OuterRectClipped.Max.y)
                window->HiddenFramesRegular = 1;

        
        if (parent_window && (parent_window->Collapsed || parent_window->Hidden))
            window->HiddenFramesRegular = 1;
    }

    
    if (style.Alpha <= 0.0f)
        window->HiddenFramesRegular = 1;

    
    window->Hidden = (window->HiddenFramesRegular > 0) || (window->HiddenFramesForResize);

    
    window->SkipItems = (window->Collapsed || !window->Active || window->Hidden) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && window->HiddenFramesForResize <= 0;

    return !window->SkipItems;
}


#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
bool ImGui::Begin(const char* name, bool* p_open, const ImVec2& size_first_use, float bg_alpha_override, ImGuiWindowFlags flags)
{
    
    if (size_first_use.x != 0.0f || size_first_use.y != 0.0f)
        ImGui::SetNextWindowSize(size_first_use, ImGuiCond_FirstUseEver);

    
    if (bg_alpha_override >= 0.0f)
        ImGui::SetNextWindowBgAlpha(bg_alpha_override);

    return ImGui::Begin(name, p_open, flags);
}
#endif 

void ImGui::End()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (window->DC.ColumnsSet != NULL)
        EndColumns();
    PopClipRect();   

    
    if (!(window->Flags & ImGuiWindowFlags_ChildWindow))    
        LogFinish();

    
    g.CurrentWindowStack.pop_back();
    if (window->Flags & ImGuiWindowFlags_Popup)
        g.CurrentPopupStack.pop_back();
    CheckStacksSize(window, false);
    SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
}

void ImGui::BringWindowToFront(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* current_front_window = g.Windows.back();
    if (current_front_window == window || current_front_window->RootWindow == window)
        return;
    for (int i = g.Windows.Size - 2; i >= 0; i--) 
        if (g.Windows[i] == window)
        {
            g.Windows.erase(g.Windows.Data + i);
            g.Windows.push_back(window);
            break;
        }
}

void ImGui::BringWindowToBack(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    if (g.Windows[0] == window)
        return;
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i] == window)
        {
            memmove(&g.Windows[1], &g.Windows[0], (size_t)i * sizeof(ImGuiWindow*));
            g.Windows[0] = window;
            break;
        }
}


void ImGui::FocusWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;

    if (g.NavWindow != window)
    {
        g.NavWindow = window;
        if (window && g.NavDisableMouseHover)
            g.NavMousePosDirty = true;
        g.NavInitRequest = false;
        g.NavId = window ? window->NavLastIds[0] : 0; 
        g.NavIdIsAlive = false;
        g.NavLayer = 0;
        
    }

    
    if (!window)
        return;

    
    if (window->RootWindow)
        window = window->RootWindow;

    
    if (window->Flags & ImGuiWindowFlags_Popup) 
        if (g.ActiveId != 0 && g.ActiveIdWindow && g.ActiveIdWindow->RootWindow != window)
            ClearActiveID();

    
    if (!(window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus))
        BringWindowToFront(window);
}

void ImGui::FocusFrontMostActiveWindowIgnoringOne(ImGuiWindow* ignore_window)
{
    ImGuiContext& g = *GImGui;
    for (int i = g.Windows.Size - 1; i >= 0; i--)
        if (g.Windows[i] != ignore_window && g.Windows[i]->WasActive && !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow))
        {
            ImGuiWindow* focus_window = NavRestoreLastChildNavWindow(g.Windows[i]);
            FocusWindow(focus_window);
            return;
        }
}

void ImGui::PushItemWidth(float item_width)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault : item_width);
    window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

void ImGui::PushMultiItemsWidths(int components, float w_full)
{
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GImGui->Style;
    if (w_full <= 0.0f)
        w_full = CalcItemWidth();
    const float w_item_one  = ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components-1)) / (float)components));
    const float w_item_last = ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components-1)));
    window->DC.ItemWidthStack.push_back(w_item_last);
    for (int i = 0; i < components-1; i++)
        window->DC.ItemWidthStack.push_back(w_item_one);
    window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemWidthStack.pop_back();
    window->DC.ItemWidth = window->DC.ItemWidthStack.empty() ? window->ItemWidthDefault : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    float w = window->DC.ItemWidth;
    if (w < 0.0f)
    {
        
        float width_to_right_edge = GetContentRegionAvail().x;
        w = ImMax(1.0f, width_to_right_edge + w);
    }
    w = (float)(int)w;
    return w;
}

void ImGui::SetCurrentFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(font && font->IsLoaded());    
    IM_ASSERT(font->Scale > 0.0f);
    g.Font = font;
    g.FontBaseSize = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
    g.FontSize = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;

    ImFontAtlas* atlas = g.Font->ContainerAtlas;
    g.DrawListSharedData.TexUvWhitePixel = atlas->TexUvWhitePixel;
    g.DrawListSharedData.Font = g.Font;
    g.DrawListSharedData.FontSize = g.FontSize;
}

void ImGui::PushFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    if (!font)
        font = GetDefaultFont();
    SetCurrentFont(font);
    g.FontStack.push_back(font);
    g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void  ImGui::PopFont()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DrawList->PopTextureID();
    g.FontStack.pop_back();
    SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void ImGui::PushItemFlag(ImGuiItemFlags option, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (enabled)
        window->DC.ItemFlags |= option;
    else
        window->DC.ItemFlags &= ~option;
    window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);
}

void ImGui::PopItemFlag()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemFlagsStack.pop_back();
    window->DC.ItemFlags = window->DC.ItemFlagsStack.empty() ? ImGuiItemFlags_Default_ : window->DC.ItemFlagsStack.back();
}

void ImGui::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
    PushItemFlag(ImGuiItemFlags_AllowKeyboardFocus, allow_keyboard_focus);
}

void ImGui::PopAllowKeyboardFocus()
{
    PopItemFlag();
}

void ImGui::PushButtonRepeat(bool repeat)
{
    PushItemFlag(ImGuiItemFlags_ButtonRepeat, repeat);
}

void ImGui::PopButtonRepeat()
{
    PopItemFlag();
}

void ImGui::PushTextWrapPos(float wrap_pos_x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.TextWrapPos = wrap_pos_x;
    window->DC.TextWrapPosStack.push_back(wrap_pos_x);
}

void ImGui::PopTextWrapPos()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.TextWrapPosStack.pop_back();
    window->DC.TextWrapPos = window->DC.TextWrapPosStack.empty() ? -1.0f : window->DC.TextWrapPosStack.back();
}


void ImGui::PushStyleColor(ImGuiCol idx, ImU32 col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorModifiers.push_back(backup);
    g.Style.Colors[idx] = ColorConvertU32ToFloat4(col);
}

void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorModifiers.push_back(backup);
    g.Style.Colors[idx] = col;
}

void ImGui::PopStyleColor(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0)
    {
        ImGuiColorMod& backup = g.ColorModifiers.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorModifiers.pop_back();
        count--;
    }
}

struct ImGuiStyleVarInfo
{
    ImGuiDataType   Type;
    ImU32           Count;
    ImU32           Offset;
    void*           GetVarPtr(ImGuiStyle* style) const { return (void*)((unsigned char*)style + Offset); }
};

static const ImGuiStyleVarInfo GStyleVarInfo[] =
{
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, Alpha) },              
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowPadding) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowRounding) },     
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowBorderSize) },   
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowMinSize) },      
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowTitleAlign) },   
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildRounding) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildBorderSize) },    
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupRounding) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupBorderSize) },    
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, FramePadding) },       
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameRounding) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameBorderSize) },    
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemSpacing) },        
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing) },   
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, IndentSpacing) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarSize) },      
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarRounding) },  
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabMinSize) },        
    { ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabRounding) },       
    { ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ButtonTextAlign) },    
};

static const ImGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx)
{
    IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_COUNT);
    IM_ASSERT(IM_ARRAYSIZE(GStyleVarInfo) == ImGuiStyleVar_COUNT);
    return &GStyleVarInfo[idx];
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, float val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
    {
        ImGuiContext& g = *GImGui;
        float* pvar = (float*)var_info->GetVarPtr(&g.Style);
        g.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0); 
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
    {
        ImGuiContext& g = *GImGui;
        ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&g.Style);
        g.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0); 
}

void ImGui::PopStyleVar(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0)
    {
        
        ImGuiStyleMod& backup = g.StyleModifiers.back();
        const ImGuiStyleVarInfo* info = GetStyleVarInfo(backup.VarIdx);
        void* data = info->GetVarPtr(&g.Style);
        if (info->Type == ImGuiDataType_Float && info->Count == 1)      { ((float*)data)[0] = backup.BackupFloat[0]; }
        else if (info->Type == ImGuiDataType_Float && info->Count == 2) { ((float*)data)[0] = backup.BackupFloat[0]; ((float*)data)[1] = backup.BackupFloat[1]; }
        g.StyleModifiers.pop_back();
        count--;
    }
}

const char* ImGui::GetStyleColorName(ImGuiCol idx)
{
    
    switch (idx)
    {
    case ImGuiCol_Text: return "Text";
    case ImGuiCol_TextDisabled: return "TextDisabled";
    case ImGuiCol_WindowBg: return "WindowBg";
    case ImGuiCol_ChildBg: return "ChildBg";
    case ImGuiCol_PopupBg: return "PopupBg";
    case ImGuiCol_Border: return "Border";
    case ImGuiCol_BorderShadow: return "BorderShadow";
    case ImGuiCol_FrameBg: return "FrameBg";
    case ImGuiCol_FrameBgHovered: return "FrameBgHovered";
    case ImGuiCol_FrameBgActive: return "FrameBgActive";
    case ImGuiCol_TitleBg: return "TitleBg";
    case ImGuiCol_TitleBgActive: return "TitleBgActive";
    case ImGuiCol_TitleBgCollapsed: return "TitleBgCollapsed";
    case ImGuiCol_MenuBarBg: return "MenuBarBg";
    case ImGuiCol_ScrollbarBg: return "ScrollbarBg";
    case ImGuiCol_ScrollbarGrab: return "ScrollbarGrab";
    case ImGuiCol_ScrollbarGrabHovered: return "ScrollbarGrabHovered";
    case ImGuiCol_ScrollbarGrabActive: return "ScrollbarGrabActive";
    case ImGuiCol_CheckMark: return "CheckMark";
    case ImGuiCol_SliderGrab: return "SliderGrab";
    case ImGuiCol_SliderGrabActive: return "SliderGrabActive";
    case ImGuiCol_Button: return "Button";
    case ImGuiCol_ButtonHovered: return "ButtonHovered";
    case ImGuiCol_ButtonActive: return "ButtonActive";
    case ImGuiCol_Header: return "Header";
    case ImGuiCol_HeaderHovered: return "HeaderHovered";
    case ImGuiCol_HeaderActive: return "HeaderActive";
    case ImGuiCol_Separator: return "Separator";
    case ImGuiCol_SeparatorHovered: return "SeparatorHovered";
    case ImGuiCol_SeparatorActive: return "SeparatorActive";
    case ImGuiCol_ResizeGrip: return "ResizeGrip";
    case ImGuiCol_ResizeGripHovered: return "ResizeGripHovered";
    case ImGuiCol_ResizeGripActive: return "ResizeGripActive";
    case ImGuiCol_PlotLines: return "PlotLines";
    case ImGuiCol_PlotLinesHovered: return "PlotLinesHovered";
    case ImGuiCol_PlotHistogram: return "PlotHistogram";
    case ImGuiCol_PlotHistogramHovered: return "PlotHistogramHovered";
    case ImGuiCol_TextSelectedBg: return "TextSelectedBg";
    case ImGuiCol_DragDropTarget: return "DragDropTarget";
    case ImGuiCol_NavHighlight: return "NavHighlight";
    case ImGuiCol_NavWindowingHighlight: return "NavWindowingHighlight";
    case ImGuiCol_NavWindowingDimBg: return "NavWindowingDimBg";
    case ImGuiCol_ModalWindowDimBg: return "ModalWindowDimBg";
    }
    IM_ASSERT(0);
    return "Unknown";
}

bool ImGui::IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent)
{
    if (window->RootWindow == potential_parent)
        return true;
    while (window != NULL)
    {
        if (window == potential_parent)
            return true;
        window = window->ParentWindow;
    }
    return false;
}

bool ImGui::IsWindowHovered(ImGuiHoveredFlags flags)
{
    IM_ASSERT((flags & ImGuiHoveredFlags_AllowWhenOverlapped) == 0);   
    ImGuiContext& g = *GImGui;

    if (flags & ImGuiHoveredFlags_AnyWindow)
    {
        if (g.HoveredWindow == NULL)
            return false;
    }
    else
    {
        switch (flags & (ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows))
        {
        case ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows:
            if (g.HoveredRootWindow != g.CurrentWindow->RootWindow)
                return false;
            break;
        case ImGuiHoveredFlags_RootWindow:
            if (g.HoveredWindow != g.CurrentWindow->RootWindow)
                return false;
            break;
        case ImGuiHoveredFlags_ChildWindows:
            if (g.HoveredWindow == NULL || !IsWindowChildOf(g.HoveredWindow, g.CurrentWindow))
                return false;
            break;
        default:
            if (g.HoveredWindow != g.CurrentWindow)
                return false;
            break;
        }
    }

    if (!IsWindowContentHoverable(g.HoveredRootWindow, flags))
        return false;
    if (!(flags & ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
        if (g.ActiveId != 0 && !g.ActiveIdAllowOverlap && g.ActiveId != g.HoveredWindow->MoveId)
            return false;
    return true;
}

bool ImGui::IsWindowFocused(ImGuiFocusedFlags flags)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.CurrentWindow);     

    if (flags & ImGuiFocusedFlags_AnyWindow)
        return g.NavWindow != NULL;

    switch (flags & (ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows))
    {
    case ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows:
        return g.NavWindow && g.NavWindow->RootWindow == g.CurrentWindow->RootWindow;
    case ImGuiFocusedFlags_RootWindow:
        return g.NavWindow == g.CurrentWindow->RootWindow;
    case ImGuiFocusedFlags_ChildWindows:
        return g.NavWindow && IsWindowChildOf(g.NavWindow, g.CurrentWindow);
    default:
        return g.NavWindow == g.CurrentWindow;
    }
}


bool ImGui::IsWindowNavFocusable(ImGuiWindow* window)
{
    return window->Active && window == window->RootWindow && !(window->Flags & ImGuiWindowFlags_NoNavFocus);
}

float ImGui::GetWindowWidth()
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    return window->Size.x;
}

float ImGui::GetWindowHeight()
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    return window->Size.y;
}

ImVec2 ImGui::GetWindowPos()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    return window->Pos;
}

void ImGui::SetWindowScrollX(ImGuiWindow* window, float new_scroll_x)
{
    window->DC.CursorMaxPos.x += window->Scroll.x; 
    window->Scroll.x = new_scroll_x;
    window->DC.CursorMaxPos.x -= window->Scroll.x;
}

void ImGui::SetWindowScrollY(ImGuiWindow* window, float new_scroll_y)
{
    window->DC.CursorMaxPos.y += window->Scroll.y; 
    window->Scroll.y = new_scroll_y;
    window->DC.CursorMaxPos.y -= window->Scroll.y;
}

static void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond)
{
    
    if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
        return;

    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); 
    window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
    window->SetWindowPosVal = ImVec2(FLT_MAX, FLT_MAX);

    
    const ImVec2 old_pos = window->Pos;
    window->Pos = ImFloor(pos);
    window->DC.CursorPos += (window->Pos - old_pos);    
    window->DC.CursorMaxPos += (window->Pos - old_pos); 
}

void ImGui::SetWindowPos(const ImVec2& pos, ImGuiCond cond)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    SetWindowPos(window, pos, cond);
}

void ImGui::SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name))
        SetWindowPos(window, pos, cond);
}

ImVec2 ImGui::GetWindowSize()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Size;
}

static void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond)
{
    
    if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
        return;

    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); 
    window->SetWindowSizeAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);

    
    if (size.x > 0.0f)
    {
        window->AutoFitFramesX = 0;
        window->SizeFull.x = size.x;
    }
    else
    {
        window->AutoFitFramesX = 2;
        window->AutoFitOnlyGrows = false;
    }
    if (size.y > 0.0f)
    {
        window->AutoFitFramesY = 0;
        window->SizeFull.y = size.y;
    }
    else
    {
        window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = false;
    }
}

void ImGui::SetWindowSize(const ImVec2& size, ImGuiCond cond)
{
    SetWindowSize(GImGui->CurrentWindow, size, cond);
}

void ImGui::SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name))
        SetWindowSize(window, size, cond);
}

static void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond)
{
    
    if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
        return;
    window->SetWindowCollapsedAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);

    
    window->Collapsed = collapsed;
}

void ImGui::SetWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    SetWindowCollapsed(GImGui->CurrentWindow, collapsed, cond);
}

bool ImGui::IsWindowCollapsed()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Collapsed;
}

bool ImGui::IsWindowAppearing()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Appearing;
}

void ImGui::SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name))
        SetWindowCollapsed(window, collapsed, cond);
}

void ImGui::SetWindowFocus()
{
    FocusWindow(GImGui->CurrentWindow);
}

void ImGui::SetWindowFocus(const char* name)
{
    if (name)
    {
        if (ImGuiWindow* window = FindWindowByName(name))
            FocusWindow(window);
    }
    else
    {
        FocusWindow(NULL);
    }
}

void ImGui::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); 
    g.NextWindowData.PosVal = pos;
    g.NextWindowData.PosPivotVal = pivot;
    g.NextWindowData.PosCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); 
    g.NextWindowData.SizeVal = size;
    g.NextWindowData.SizeCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback, void* custom_callback_user_data)
{
    ImGuiContext& g = *GImGui;
    g.NextWindowData.SizeConstraintCond = ImGuiCond_Always;
    g.NextWindowData.SizeConstraintRect = ImRect(size_min, size_max);
    g.NextWindowData.SizeCallback = custom_callback;
    g.NextWindowData.SizeCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2& size)
{
    ImGuiContext& g = *GImGui;
    g.NextWindowData.ContentSizeVal = size;  
    g.NextWindowData.ContentSizeCond = ImGuiCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond)); 
    g.NextWindowData.CollapsedVal = collapsed;
    g.NextWindowData.CollapsedCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowFocus()
{
    ImGuiContext& g = *GImGui;
    g.NextWindowData.FocusCond = ImGuiCond_Always;   
}

void ImGui::SetNextWindowBgAlpha(float alpha)
{
    ImGuiContext& g = *GImGui;
    g.NextWindowData.BgAlphaVal = alpha;
    g.NextWindowData.BgAlphaCond = ImGuiCond_Always; 
}


ImVec2 ImGui::GetContentRegionMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImVec2 mx = window->ContentsRegionRect.Max - window->Pos;
    if (window->DC.ColumnsSet)
        mx.x = GetColumnOffset(window->DC.ColumnsSet->Current + 1) - window->WindowPadding.x;
    return mx;
}

ImVec2 ImGui::GetContentRegionAvail()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return GetContentRegionMax() - (window->DC.CursorPos - window->Pos);
}

float ImGui::GetContentRegionAvailWidth()
{
    return GetContentRegionAvail().x;
}


ImVec2 ImGui::GetWindowContentRegionMin()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.Min - window->Pos;
}

ImVec2 ImGui::GetWindowContentRegionMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.Max - window->Pos;
}

float ImGui::GetWindowContentRegionWidth()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.GetWidth();
}

float ImGui::GetTextLineHeight()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize;
}

float ImGui::GetTextLineHeightWithSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.ItemSpacing.y;
}

float ImGui::GetFrameHeight()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.y * 2.0f;
}

float ImGui::GetFrameHeightWithSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

ImDrawList* ImGui::GetWindowDrawList()
{
    ImGuiWindow* window = GetCurrentWindow();
    return window->DrawList;
}

ImFont* ImGui::GetFont()
{
    return GImGui->Font;
}

float ImGui::GetFontSize()
{
    return GImGui->FontSize;
}

ImVec2 ImGui::GetFontTexUvWhitePixel()
{
    return GImGui->DrawListSharedData.TexUvWhitePixel;
}

void ImGui::SetWindowFontScale(float scale)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->FontWindowScale = scale;
    g.FontSize = g.DrawListSharedData.FontSize = window->CalcFontSize();
}



ImVec2 ImGui::GetCursorPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
}

float ImGui::GetCursorPosX()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float ImGui::GetCursorPosY()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void ImGui::SetCursorPos(const ImVec2& local_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

void ImGui::SetCursorPosX(float x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
}

ImVec2 ImGui::GetCursorStartPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorStartPos - window->Pos;
}

ImVec2 ImGui::GetCursorScreenPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos;
}

void ImGui::SetCursorScreenPos(const ImVec2& screen_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = screen_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

float ImGui::GetScrollX()
{
    return GImGui->CurrentWindow->Scroll.x;
}

float ImGui::GetScrollY()
{
    return GImGui->CurrentWindow->Scroll.y;
}

float ImGui::GetScrollMaxX()
{
    return GetScrollMaxX(GImGui->CurrentWindow);
}

float ImGui::GetScrollMaxY()
{
    return GetScrollMaxY(GImGui->CurrentWindow);
}

void ImGui::SetScrollX(float scroll_x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->ScrollTarget.x = scroll_x;
    window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->ScrollTarget.y = scroll_y + window->TitleBarHeight() + window->MenuBarHeight(); 
    window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
    
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
    window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
    window->ScrollTargetCenterRatio.y = center_y_ratio;
}


void ImGui::SetScrollHere(float center_y_ratio)
{
    ImGuiWindow* window = GetCurrentWindow();
    float target_y = window->DC.CursorPosPrevLine.y - window->Pos.y; 
    target_y += (window->DC.PrevLineSize.y * center_y_ratio) + (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) * 2.0f); 
    SetScrollFromPosY(target_y, center_y_ratio);
}

void ImGui::ActivateItem(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    g.NavNextActivateId = id;
}

void ImGui::SetKeyboardFocusHere(int offset)
{
    IM_ASSERT(offset >= -1);    
    ImGuiWindow* window = GetCurrentWindow();
    window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
    window->FocusIdxTabRequestNext = INT_MAX;
}

void ImGui::SetItemDefaultFocus()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (!window->Appearing)
        return;
    if (g.NavWindow == window->RootWindowForNav && (g.NavInitRequest || g.NavInitResultId != 0) && g.NavLayer == g.NavWindow->DC.NavLayerCurrent)
    {
        g.NavInitRequest = false;
        g.NavInitResultId = g.NavWindow->DC.LastItemId;
        g.NavInitResultRectRel = ImRect(g.NavWindow->DC.LastItemRect.Min - g.NavWindow->Pos, g.NavWindow->DC.LastItemRect.Max - g.NavWindow->Pos);
        NavUpdateAnyRequestFlag();
        if (!IsItemVisible())
            SetScrollHere();
    }
}

void ImGui::SetStateStorage(ImGuiStorage* tree)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.StateStorage = tree ? tree : &window->StateStorage;
}

ImGuiStorage* ImGui::GetStateStorage()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.StateStorage;
}

void ImGui::PushID(const char* str_id)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetIDNoKeepAlive(str_id));
}

void ImGui::PushID(const char* str_id_begin, const char* str_id_end)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetIDNoKeepAlive(str_id_begin, str_id_end));
}

void ImGui::PushID(const void* ptr_id)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetIDNoKeepAlive(ptr_id));
}

void ImGui::PushID(int int_id)
{
    const void* ptr_id = (void*)(intptr_t)int_id;
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetIDNoKeepAlive(ptr_id));
}

void ImGui::PopID()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.pop_back();
}

ImGuiID ImGui::GetID(const char* str_id)
{
    return GImGui->CurrentWindow->GetID(str_id);
}

ImGuiID ImGui::GetID(const char* str_id_begin, const char* str_id_end)
{
    return GImGui->CurrentWindow->GetID(str_id_begin, str_id_end);
}

ImGuiID ImGui::GetID(const void* ptr_id)
{
    return GImGui->CurrentWindow->GetID(ptr_id);
}

bool ImGui::IsRectVisible(const ImVec2& size)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}


void ImGui::BeginGroup()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
    ImGuiGroupData& group_data = window->DC.GroupStack.back();
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndent = window->DC.Indent;
    group_data.BackupGroupOffset = window->DC.GroupOffset;
    group_data.BackupCurrentLineSize = window->DC.CurrentLineSize;
    group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
    group_data.BackupLogLinePosY = window->DC.LogLinePosY;
    group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
    group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
    group_data.AdvanceCursor = true;

    window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
    window->DC.Indent = window->DC.GroupOffset;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrentLineSize = ImVec2(0.0f, 0.0f);
    window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f; 
}

void ImGui::EndGroup()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(!window->DC.GroupStack.empty());    

    ImGuiGroupData& group_data = window->DC.GroupStack.back();

    ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
    group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.Indent = group_data.BackupIndent;
    window->DC.GroupOffset = group_data.BackupGroupOffset;
    window->DC.CurrentLineSize = group_data.BackupCurrentLineSize;
    window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
    window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f; 

    if (group_data.AdvanceCursor)
    {
        window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);      
        ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
        ItemAdd(group_bb, 0);
    }

    
    
    
    if ((group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId) 
        window->DC.LastItemId = g.ActiveId;
    else if (!group_data.BackupActiveIdPreviousFrameIsAlive && g.ActiveIdPreviousFrameIsAlive) 
        window->DC.LastItemId = g.ActiveIdPreviousFrame;
    window->DC.LastItemRect = group_bb;

    window->DC.GroupStack.pop_back();

    
}






void ImGui::SameLine(float pos_x, float spacing_w)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    if (pos_x != 0.0f)
    {
        if (spacing_w < 0.0f) spacing_w = 0.0f;
        window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffset.x + window->DC.ColumnsOffset.x;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    else
    {
        if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
        window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrentLineSize = window->DC.PrevLineSize;
    window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::Indent(float indent_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.Indent.x += (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x;
}

void ImGui::Unindent(float indent_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.Indent.x -= (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x;
}





void ImGui::BeginTooltip()
{
    ImGuiContext& g = *GImGui;
    if (g.DragDropWithinSourceOrTarget)
    {
        
        
        
        
        ImVec2 tooltip_pos = g.IO.MousePos + ImVec2(16 * g.Style.MouseCursorScale, 8 * g.Style.MouseCursorScale);
        SetNextWindowPos(tooltip_pos);
        SetNextWindowBgAlpha(g.Style.Colors[ImGuiCol_PopupBg].w * 0.60f);
        
        BeginTooltipEx(0, true);
    }
    else
    {
        BeginTooltipEx(0, false);
    }
}


void ImGui::BeginTooltipEx(ImGuiWindowFlags extra_flags, bool override_previous_tooltip)
{
    ImGuiContext& g = *GImGui;
    char window_name[16];
    ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip_%02d", g.TooltipOverrideCount);
    if (override_previous_tooltip)
        if (ImGuiWindow* window = FindWindowByName(window_name))
            if (window->Active)
            {
                
                window->Hidden = true;
                window->HiddenFramesRegular = 1;
                ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip_%02d", ++g.TooltipOverrideCount);
            }
    ImGuiWindowFlags flags = ImGuiWindowFlags_Tooltip|ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoNav;
    Begin(window_name, NULL, flags | extra_flags);
}

void ImGui::EndTooltip()
{
    IM_ASSERT(GetCurrentWindowRead()->Flags & ImGuiWindowFlags_Tooltip);   
    End();
}

void ImGui::SetTooltipV(const char* fmt, va_list args)
{
    ImGuiContext& g = *GImGui;
    if (g.DragDropWithinSourceOrTarget)
        BeginTooltip();
    else
        BeginTooltipEx(0, true);
    TextV(fmt, args);
    EndTooltip();
}

void ImGui::SetTooltip(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SetTooltipV(fmt, args);
    va_end(args);
}





bool ImGui::IsPopupOpen(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == id;
}

bool ImGui::IsPopupOpen(const char* str_id)
{
    ImGuiContext& g = *GImGui;
    return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == g.CurrentWindow->GetID(str_id);
}

ImGuiWindow* ImGui::GetFrontMostPopupModal()
{
    ImGuiContext& g = *GImGui;
    for (int n = g.OpenPopupStack.Size-1; n >= 0; n--)
        if (ImGuiWindow* popup = g.OpenPopupStack.Data[n].Window)
            if (popup->Flags & ImGuiWindowFlags_Modal)
                return popup;
    return NULL;
}

void ImGui::OpenPopup(const char* str_id)
{
    ImGuiContext& g = *GImGui;
    OpenPopupEx(g.CurrentWindow->GetID(str_id));
}





void ImGui::OpenPopupEx(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;
    int current_stack_size = g.CurrentPopupStack.Size;
    ImGuiPopupRef popup_ref; 
    popup_ref.PopupId = id;
    popup_ref.Window = NULL;
    popup_ref.ParentWindow = parent_window;
    popup_ref.OpenFrameCount = g.FrameCount;
    popup_ref.OpenParentId = parent_window->IDStack.back();
    popup_ref.OpenMousePos = g.IO.MousePos;
    popup_ref.OpenPopupPos = NavCalcPreferredRefPos();

    
    if (g.OpenPopupStack.Size < current_stack_size + 1)
    {
        g.OpenPopupStack.push_back(popup_ref);
    }
    else
    {
        
        
        
        if (g.OpenPopupStack[current_stack_size].PopupId == id && g.OpenPopupStack[current_stack_size].OpenFrameCount == g.FrameCount - 1)
        {
            g.OpenPopupStack[current_stack_size].OpenFrameCount = popup_ref.OpenFrameCount;
        }
        else
        {
            
            g.OpenPopupStack.resize(current_stack_size + 1);
            g.OpenPopupStack[current_stack_size] = popup_ref;
        }

        
        
        
        
    }
}

bool ImGui::OpenPopupOnItemClick(const char* str_id, int mouse_button)
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    if (IsMouseReleased(mouse_button) && IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        ImGuiID id = str_id ? window->GetID(str_id) : window->DC.LastItemId; 
        IM_ASSERT(id != 0);                                                  
        OpenPopupEx(id);
        return true;
    }
    return false;
}

void ImGui::ClosePopupsOverWindow(ImGuiWindow* ref_window)
{
    ImGuiContext& g = *GImGui;
    if (g.OpenPopupStack.empty())
        return;

    
    
    int n = 0;
    if (ref_window)
    {
        for (n = 0; n < g.OpenPopupStack.Size; n++)
        {
            ImGuiPopupRef& popup = g.OpenPopupStack[n];
            if (!popup.Window)
                continue;
            IM_ASSERT((popup.Window->Flags & ImGuiWindowFlags_Popup) != 0);
            if (popup.Window->Flags & ImGuiWindowFlags_ChildWindow)
                continue;

            
            bool has_focus = false;
            for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
                has_focus = (g.OpenPopupStack[m].Window && g.OpenPopupStack[m].Window->RootWindow == ref_window->RootWindow);
            if (!has_focus)
                break;
        }
    }
    if (n < g.OpenPopupStack.Size) 
        ClosePopupToLevel(n);
}

void ImGui::ClosePopupToLevel(int remaining)
{
    IM_ASSERT(remaining >= 0);
    ImGuiContext& g = *GImGui;
    ImGuiWindow* focus_window = (remaining > 0) ? g.OpenPopupStack[remaining-1].Window : g.OpenPopupStack[0].ParentWindow;
    if (g.NavLayer == 0)
        focus_window = NavRestoreLastChildNavWindow(focus_window);
    FocusWindow(focus_window);
    focus_window->DC.NavHideHighlightOneFrame = true;
    g.OpenPopupStack.resize(remaining);
}

void ImGui::ClosePopup(ImGuiID id)
{
    if (!IsPopupOpen(id))
        return;
    ImGuiContext& g = *GImGui;
    ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}


void ImGui::CloseCurrentPopup()
{
    ImGuiContext& g = *GImGui;
    int popup_idx = g.CurrentPopupStack.Size - 1;
    if (popup_idx < 0 || popup_idx >= g.OpenPopupStack.Size || g.CurrentPopupStack[popup_idx].PopupId != g.OpenPopupStack[popup_idx].PopupId)
        return;
    while (popup_idx > 0 && g.OpenPopupStack[popup_idx].Window && (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
        popup_idx--;
    ClosePopupToLevel(popup_idx);
}

bool ImGui::BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g = *GImGui;
    if (!IsPopupOpen(id))
    {
        g.NextWindowData.Clear(); 
        return false;
    }

    char name[20];
    if (extra_flags & ImGuiWindowFlags_ChildMenu)
        ImFormatString(name, IM_ARRAYSIZE(name), "##Menu_%02d", g.CurrentPopupStack.Size);    
    else
        ImFormatString(name, IM_ARRAYSIZE(name), "##Popup_%08x", id); 

    bool is_open = Begin(name, NULL, extra_flags | ImGuiWindowFlags_Popup);
    if (!is_open) 
        EndPopup();

    return is_open;
}

bool ImGui::BeginPopup(const char* str_id, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    if (g.OpenPopupStack.Size <= g.CurrentPopupStack.Size) 
    {
        g.NextWindowData.Clear(); 
        return false;
    }
    return BeginPopupEx(g.CurrentWindow->GetID(str_id), flags|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings);
}

bool ImGui::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImGuiID id = window->GetID(name);
    if (!IsPopupOpen(id))
    {
        g.NextWindowData.Clear(); 
        return false;
    }

    
    
    if (g.NextWindowData.PosCond == 0)
        SetNextWindowPos(g.IO.DisplaySize * 0.5f, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    bool is_open = Begin(name, p_open, flags | ImGuiWindowFlags_Popup | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    if (!is_open || (p_open && !*p_open)) 
    {
        EndPopup();
        if (is_open)
            ClosePopup(id);
        return false;
    }
    return is_open;
}

void ImGui::EndPopup()
{
    ImGuiContext& g = *GImGui; (void)g;
    IM_ASSERT(g.CurrentWindow->Flags & ImGuiWindowFlags_Popup);  
    IM_ASSERT(g.CurrentPopupStack.Size > 0);

    
    NavMoveRequestTryWrapping(g.CurrentWindow, ImGuiNavMoveFlags_LoopY);

    End();
}




bool ImGui::BeginPopupContextItem(const char* str_id, int mouse_button)
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImGuiID id = str_id ? window->GetID(str_id) : window->DC.LastItemId; 
    IM_ASSERT(id != 0);                                                  
    if (IsMouseReleased(mouse_button) && IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        OpenPopupEx(id);
    return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings);
}

bool ImGui::BeginPopupContextWindow(const char* str_id, int mouse_button, bool also_over_items)
{
    if (!str_id)
        str_id = "window_context";
    ImGuiID id = GImGui->CurrentWindow->GetID(str_id);
    if (IsMouseReleased(mouse_button) && IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        if (also_over_items || !IsAnyItemHovered())
            OpenPopupEx(id);
    return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings);
}

bool ImGui::BeginPopupContextVoid(const char* str_id, int mouse_button)
{
    if (!str_id)
        str_id = "void_context";
    ImGuiID id = GImGui->CurrentWindow->GetID(str_id);
    if (IsMouseReleased(mouse_button) && !IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        OpenPopupEx(id);
    return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings);
}

ImRect ImGui::GetWindowAllowedExtentRect(ImGuiWindow*)
{
    ImVec2 padding = GImGui->Style.DisplaySafeAreaPadding;
    ImRect r_screen = GetViewportRect();
    r_screen.Expand(ImVec2((r_screen.GetWidth() > padding.x * 2) ? -padding.x : 0.0f, (r_screen.GetHeight() > padding.y * 2) ? -padding.y : 0.0f));
    return r_screen;
}



ImVec2 ImGui::FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy)
{
    ImVec2 base_pos_clamped = ImClamp(ref_pos, r_outer.Min, r_outer.Max - size);
    
    

    
    if (policy == ImGuiPopupPositionPolicy_ComboBox)
    {
        const ImGuiDir dir_prefered_order[ImGuiDir_COUNT] = { ImGuiDir_Down, ImGuiDir_Right, ImGuiDir_Left, ImGuiDir_Up };
        for (int n = (*last_dir != ImGuiDir_None) ? -1 : 0; n < ImGuiDir_COUNT; n++)
        {
            const ImGuiDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
            if (n != -1 && dir == *last_dir) 
                continue;
            ImVec2 pos;
            if (dir == ImGuiDir_Down)  pos = ImVec2(r_avoid.Min.x, r_avoid.Max.y);          
            if (dir == ImGuiDir_Right) pos = ImVec2(r_avoid.Min.x, r_avoid.Min.y - size.y); 
            if (dir == ImGuiDir_Left)  pos = ImVec2(r_avoid.Max.x - size.x, r_avoid.Max.y); 
            if (dir == ImGuiDir_Up)    pos = ImVec2(r_avoid.Max.x - size.x, r_avoid.Min.y - size.y); 
            if (!r_outer.Contains(ImRect(pos, pos + size)))
                continue;
            *last_dir = dir;
            return pos;
        }
    }

    
    const ImGuiDir dir_prefered_order[ImGuiDir_COUNT] = { ImGuiDir_Right, ImGuiDir_Down, ImGuiDir_Up, ImGuiDir_Left };
    for (int n = (*last_dir != ImGuiDir_None) ? -1 : 0; n < ImGuiDir_COUNT; n++)
    {
        const ImGuiDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
        if (n != -1 && dir == *last_dir) 
            continue;
        float avail_w = (dir == ImGuiDir_Left ? r_avoid.Min.x : r_outer.Max.x) - (dir == ImGuiDir_Right ? r_avoid.Max.x : r_outer.Min.x);
        float avail_h = (dir == ImGuiDir_Up ? r_avoid.Min.y : r_outer.Max.y) - (dir == ImGuiDir_Down ? r_avoid.Max.y : r_outer.Min.y);
        if (avail_w < size.x || avail_h < size.y)
            continue;
        ImVec2 pos;
        pos.x = (dir == ImGuiDir_Left) ? r_avoid.Min.x - size.x : (dir == ImGuiDir_Right) ? r_avoid.Max.x : base_pos_clamped.x;
        pos.y = (dir == ImGuiDir_Up)   ? r_avoid.Min.y - size.y : (dir == ImGuiDir_Down)  ? r_avoid.Max.y : base_pos_clamped.y;
        *last_dir = dir;
        return pos;
    }

    
    *last_dir = ImGuiDir_None;
    ImVec2 pos = ref_pos;
    pos.x = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
    pos.y = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
    return pos;
}

ImVec2 ImGui::FindBestWindowPosForPopup(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;

    ImRect r_outer = GetWindowAllowedExtentRect(window);
    if (window->Flags & ImGuiWindowFlags_ChildMenu)
    {
        
        
        IM_ASSERT(g.CurrentWindow == window);
        ImGuiWindow* parent_window = g.CurrentWindowStack[g.CurrentWindowStack.Size - 2];
        float horizontal_overlap = g.Style.ItemSpacing.x;       
        ImRect r_avoid;
        if (parent_window->DC.MenuBarAppending)
            r_avoid = ImRect(-FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight(), FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight() + parent_window->MenuBarHeight());
        else
            r_avoid = ImRect(parent_window->Pos.x + horizontal_overlap, -FLT_MAX, parent_window->Pos.x + parent_window->Size.x - horizontal_overlap - parent_window->ScrollbarSizes.x, FLT_MAX);
        return FindBestWindowPosForPopupEx(window->Pos, window->Size, &window->AutoPosLastDirection, r_outer, r_avoid);
    }
    if (window->Flags & ImGuiWindowFlags_Popup)
    {
        ImRect r_avoid = ImRect(window->Pos.x - 1, window->Pos.y - 1, window->Pos.x + 1, window->Pos.y + 1);
        return FindBestWindowPosForPopupEx(window->Pos, window->Size, &window->AutoPosLastDirection, r_outer, r_avoid);
    }
    if (window->Flags & ImGuiWindowFlags_Tooltip)
    {
        
        float sc = g.Style.MouseCursorScale;
        ImVec2 ref_pos = NavCalcPreferredRefPos();
        ImRect r_avoid;
        if (!g.NavDisableHighlight && g.NavDisableMouseHover && !(g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos))
            r_avoid = ImRect(ref_pos.x - 16, ref_pos.y - 8, ref_pos.x + 16, ref_pos.y + 8);
        else
            r_avoid = ImRect(ref_pos.x - 16, ref_pos.y - 8, ref_pos.x + 24 * sc, ref_pos.y + 24 * sc); 
        ImVec2 pos = FindBestWindowPosForPopupEx(ref_pos, window->Size, &window->AutoPosLastDirection, r_outer, r_avoid);
        if (window->AutoPosLastDirection == ImGuiDir_None)
            pos = ref_pos + ImVec2(2, 2); 
        return pos;
    }
    IM_ASSERT(0);
    return window->Pos;
}





ImGuiDir ImGetDirQuadrantFromDelta(float dx, float dy)
{
    if (ImFabs(dx) > ImFabs(dy))
        return (dx > 0.0f) ? ImGuiDir_Right : ImGuiDir_Left;
    return (dy > 0.0f) ? ImGuiDir_Down : ImGuiDir_Up;
}

static float inline NavScoreItemDistInterval(float a0, float a1, float b0, float b1)
{
    if (a1 < b0)
        return a1 - b0;
    if (b1 < a0)
        return a0 - b1;
    return 0.0f;
}

static void inline NavClampRectToVisibleAreaForMoveDir(ImGuiDir move_dir, ImRect& r, const ImRect& clip_rect)
{
    if (move_dir == ImGuiDir_Left || move_dir == ImGuiDir_Right)
    {
        r.Min.y = ImClamp(r.Min.y, clip_rect.Min.y, clip_rect.Max.y);
        r.Max.y = ImClamp(r.Max.y, clip_rect.Min.y, clip_rect.Max.y);
    }
    else
    {
        r.Min.x = ImClamp(r.Min.x, clip_rect.Min.x, clip_rect.Max.x);
        r.Max.x = ImClamp(r.Max.x, clip_rect.Min.x, clip_rect.Max.x);
    }
}


static bool NavScoreItem(ImGuiNavMoveResult* result, ImRect cand)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (g.NavLayer != window->DC.NavLayerCurrent)
        return false;

    const ImRect& curr = g.NavScoringRectScreen; 
    g.NavScoringCount++;

    
    if (window->ParentWindow == g.NavWindow)
    {
        IM_ASSERT((window->Flags | g.NavWindow->Flags) & ImGuiWindowFlags_NavFlattened);
        if (!window->ClipRect.Contains(cand))
            return false;
        cand.ClipWithFull(window->ClipRect); 
    }

    
    
    NavClampRectToVisibleAreaForMoveDir(g.NavMoveClipDir, cand, window->ClipRect);

    
    
    float dbx = NavScoreItemDistInterval(cand.Min.x, cand.Max.x, curr.Min.x, curr.Max.x);
    float dby = NavScoreItemDistInterval(ImLerp(cand.Min.y, cand.Max.y, 0.2f), ImLerp(cand.Min.y, cand.Max.y, 0.8f), ImLerp(curr.Min.y, curr.Max.y, 0.2f), ImLerp(curr.Min.y, curr.Max.y, 0.8f)); 
    if (dby != 0.0f && dbx != 0.0f)
       dbx = (dbx/1000.0f) + ((dbx > 0.0f) ? +1.0f : -1.0f);
    float dist_box = ImFabs(dbx) + ImFabs(dby);

    
    float dcx = (cand.Min.x + cand.Max.x) - (curr.Min.x + curr.Max.x);
    float dcy = (cand.Min.y + cand.Max.y) - (curr.Min.y + curr.Max.y);
    float dist_center = ImFabs(dcx) + ImFabs(dcy); 

    
    ImGuiDir quadrant;
    float dax = 0.0f, day = 0.0f, dist_axial = 0.0f;
    if (dbx != 0.0f || dby != 0.0f)
    {
        
        dax = dbx;
        day = dby;
        dist_axial = dist_box;
        quadrant = ImGetDirQuadrantFromDelta(dbx, dby);
    }
    else if (dcx != 0.0f || dcy != 0.0f)
    {
        
        dax = dcx;
        day = dcy;
        dist_axial = dist_center;
        quadrant = ImGetDirQuadrantFromDelta(dcx, dcy);
    }
    else
    {
        
        quadrant = (window->DC.LastItemId < g.NavId) ? ImGuiDir_Left : ImGuiDir_Right;
    }

#if IMGUI_DEBUG_NAV_SCORING
    char buf[128];
    if (ImGui::IsMouseHoveringRect(cand.Min, cand.Max))
    {
        ImFormatString(buf, IM_ARRAYSIZE(buf), "dbox (%.2f,%.2f->%.4f)\ndcen (%.2f,%.2f->%.4f)\nd (%.2f,%.2f->%.4f)\nnav %c, quadrant %c", dbx, dby, dist_box, dcx, dcy, dist_center, dax, day, dist_axial, "WENS"[g.NavMoveDir], "WENS"[quadrant]);
        ImDrawList* draw_list = ImGui::GetOverlayDrawList();
        draw_list->AddRect(curr.Min, curr.Max, IM_COL32(255,200,0,100));
        draw_list->AddRect(cand.Min, cand.Max, IM_COL32(255,255,0,200));
        draw_list->AddRectFilled(cand.Max-ImVec2(4,4), cand.Max+ImGui::CalcTextSize(buf)+ImVec2(4,4), IM_COL32(40,0,0,150));
        draw_list->AddText(g.IO.FontDefault, 13.0f, cand.Max, ~0U, buf);
    }
    else if (g.IO.KeyCtrl) 
    {
        if (IsKeyPressedMap(ImGuiKey_C)) { g.NavMoveDirLast = (ImGuiDir)((g.NavMoveDirLast + 1) & 3); g.IO.KeysDownDuration[g.IO.KeyMap[ImGuiKey_C]] = 0.01f; }
        if (quadrant == g.NavMoveDir)
        {
            ImFormatString(buf, IM_ARRAYSIZE(buf), "%.0f/%.0f", dist_box, dist_center);
            ImDrawList* draw_list = ImGui::GetOverlayDrawList();
            draw_list->AddRectFilled(cand.Min, cand.Max, IM_COL32(255, 0, 0, 200));
            draw_list->AddText(g.IO.FontDefault, 13.0f, cand.Min, IM_COL32(255, 255, 255, 255), buf);
        }
    }
 #endif

    
    bool new_best = false;
    if (quadrant == g.NavMoveDir)
    {
        
        if (dist_box < result->DistBox)
        {
            result->DistBox = dist_box;
            result->DistCenter = dist_center;
            return true;
        }
        if (dist_box == result->DistBox)
        {
            
            if (dist_center < result->DistCenter)
            {
                result->DistCenter = dist_center;
                new_best = true;
            }
            else if (dist_center == result->DistCenter)
            {
                
                
                
                if (((g.NavMoveDir == ImGuiDir_Up || g.NavMoveDir == ImGuiDir_Down) ? dby : dbx) < 0.0f) 
                    new_best = true;
            }
        }
    }

    
    
    
    
    
    if (result->DistBox == FLT_MAX && dist_axial < result->DistAxial)  
        if (g.NavLayer == 1 && !(g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu))
            if ((g.NavMoveDir == ImGuiDir_Left && dax < 0.0f) || (g.NavMoveDir == ImGuiDir_Right && dax > 0.0f) || (g.NavMoveDir == ImGuiDir_Up && day < 0.0f) || (g.NavMoveDir == ImGuiDir_Down && day > 0.0f))
            {
                result->DistAxial = dist_axial;
                new_best = true;
            }

    return new_best;
}


static void ImGui::NavProcessItem(ImGuiWindow* window, const ImRect& nav_bb, const ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    
    

    const ImGuiItemFlags item_flags = window->DC.ItemFlags;
    const ImRect nav_bb_rel(nav_bb.Min - window->Pos, nav_bb.Max - window->Pos);

    
    if (g.NavInitRequest && g.NavLayer == window->DC.NavLayerCurrent)
    {
        
        if (!(item_flags & ImGuiItemFlags_NoNavDefaultFocus) || g.NavInitResultId == 0)
        {
            g.NavInitResultId = id;
            g.NavInitResultRectRel = nav_bb_rel;
        }
        if (!(item_flags & ImGuiItemFlags_NoNavDefaultFocus))
        {
            g.NavInitRequest = false; 
            NavUpdateAnyRequestFlag();
        }
    }

    
    
    if ((g.NavId != id || (g.NavMoveRequestFlags & ImGuiNavMoveFlags_AllowCurrentNavId)) && !(item_flags & ImGuiItemFlags_NoNav))
    {
        ImGuiNavMoveResult* result = (window == g.NavWindow) ? &g.NavMoveResultLocal : &g.NavMoveResultOther;
#if IMGUI_DEBUG_NAV_SCORING
        
        if (!g.NavMoveRequest)
            g.NavMoveDir = g.NavMoveDirLast;
        bool new_best = NavScoreItem(result, nav_bb) && g.NavMoveRequest;
#else
        bool new_best = g.NavMoveRequest && NavScoreItem(result, nav_bb);
#endif
        if (new_best)
        {
            result->ID = id;
            result->Window = window;
            result->RectRel = nav_bb_rel;
        }

        const float VISIBLE_RATIO = 0.70f;
        if ((g.NavMoveRequestFlags & ImGuiNavMoveFlags_AlsoScoreVisibleSet) && window->ClipRect.Overlaps(nav_bb))
            if (ImClamp(nav_bb.Max.y, window->ClipRect.Min.y, window->ClipRect.Max.y) - ImClamp(nav_bb.Min.y, window->ClipRect.Min.y, window->ClipRect.Max.y) >= (nav_bb.Max.y - nav_bb.Min.y) * VISIBLE_RATIO)
                if (NavScoreItem(&g.NavMoveResultLocalVisibleSet, nav_bb))
                {
                    result = &g.NavMoveResultLocalVisibleSet;
                    result->ID = id;
                    result->Window = window;
                    result->RectRel = nav_bb_rel;
                }
    }

    
    if (g.NavId == id)
    {
        g.NavWindow = window;                                           
        g.NavLayer = window->DC.NavLayerCurrent;
        g.NavIdIsAlive = true;
        g.NavIdTabCounter = window->FocusIdxTabCounter;
        window->NavRectRel[window->DC.NavLayerCurrent] = nav_bb_rel;    
    }
}

bool ImGui::NavMoveRequestButNoResultYet()
{
    ImGuiContext& g = *GImGui;
    return g.NavMoveRequest && g.NavMoveResultLocal.ID == 0 && g.NavMoveResultOther.ID == 0;
}

void ImGui::NavMoveRequestCancel()
{
    ImGuiContext& g = *GImGui;
    g.NavMoveRequest = false;
    NavUpdateAnyRequestFlag();
}

void ImGui::NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, const ImRect& bb_rel, ImGuiNavMoveFlags move_flags)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.NavMoveRequestForward == ImGuiNavForward_None);
    ImGui::NavMoveRequestCancel();
    g.NavMoveDir = move_dir;
    g.NavMoveClipDir = clip_dir;
    g.NavMoveRequestForward = ImGuiNavForward_ForwardQueued;
    g.NavMoveRequestFlags = move_flags;
    g.NavWindow->NavRectRel[g.NavLayer] = bb_rel;
}

void ImGui::NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags)
{
    ImGuiContext& g = *GImGui;
    if (g.NavWindow != window || !NavMoveRequestButNoResultYet() || g.NavMoveRequestForward != ImGuiNavForward_None || g.NavLayer != 0)
        return;
    IM_ASSERT(move_flags != 0); 
    ImRect bb_rel = window->NavRectRel[0];

    ImGuiDir clip_dir = g.NavMoveDir;
    if (g.NavMoveDir == ImGuiDir_Left && (move_flags & (ImGuiNavMoveFlags_WrapX | ImGuiNavMoveFlags_LoopX)))
    {
        bb_rel.Min.x = bb_rel.Max.x = ImMax(window->SizeFull.x, window->SizeContents.x) - window->Scroll.x;
        if (move_flags & ImGuiNavMoveFlags_WrapX) { bb_rel.TranslateY(-bb_rel.GetHeight()); clip_dir = ImGuiDir_Up; }
        NavMoveRequestForward(g.NavMoveDir, clip_dir, bb_rel, move_flags);
    }
    if (g.NavMoveDir == ImGuiDir_Right && (move_flags & (ImGuiNavMoveFlags_WrapX | ImGuiNavMoveFlags_LoopX)))
    {
        bb_rel.Min.x = bb_rel.Max.x = -window->Scroll.x;
        if (move_flags & ImGuiNavMoveFlags_WrapX) { bb_rel.TranslateY(+bb_rel.GetHeight()); clip_dir = ImGuiDir_Down; }
        NavMoveRequestForward(g.NavMoveDir, clip_dir, bb_rel, move_flags);
    }
    if (g.NavMoveDir == ImGuiDir_Up && (move_flags & (ImGuiNavMoveFlags_WrapY | ImGuiNavMoveFlags_LoopY)))
    {
        bb_rel.Min.y = bb_rel.Max.y = ImMax(window->SizeFull.y, window->SizeContents.y) - window->Scroll.y;
        if (move_flags & ImGuiNavMoveFlags_WrapY) { bb_rel.TranslateX(-bb_rel.GetWidth()); clip_dir = ImGuiDir_Left; }
        NavMoveRequestForward(g.NavMoveDir, clip_dir, bb_rel, move_flags);
    }
    if (g.NavMoveDir == ImGuiDir_Down && (move_flags & (ImGuiNavMoveFlags_WrapY | ImGuiNavMoveFlags_LoopY)))
    {
        bb_rel.Min.y = bb_rel.Max.y = -window->Scroll.y;
        if (move_flags & ImGuiNavMoveFlags_WrapY) { bb_rel.TranslateX(+bb_rel.GetWidth()); clip_dir = ImGuiDir_Right; }
        NavMoveRequestForward(g.NavMoveDir, clip_dir, bb_rel, move_flags);
    }
}

static void ImGui::NavSaveLastChildNavWindow(ImGuiWindow* nav_window)
{
    ImGuiWindow* parent_window = nav_window;
    while (parent_window && (parent_window->Flags & ImGuiWindowFlags_ChildWindow) != 0 && (parent_window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) == 0)
        parent_window = parent_window->ParentWindow;
    if (parent_window && parent_window != nav_window)
        parent_window->NavLastChildNavWindow = nav_window;
}


static ImGuiWindow* ImGui::NavRestoreLastChildNavWindow(ImGuiWindow* window)
{
    return window->NavLastChildNavWindow ? window->NavLastChildNavWindow : window;
}

static void NavRestoreLayer(int layer)
{
    ImGuiContext& g = *GImGui;
    g.NavLayer = layer;
    if (layer == 0)
        g.NavWindow = ImGui::NavRestoreLastChildNavWindow(g.NavWindow);
    if (layer == 0 && g.NavWindow->NavLastIds[0] != 0)
        ImGui::SetNavIDWithRectRel(g.NavWindow->NavLastIds[0], layer, g.NavWindow->NavRectRel[0]);
    else
        ImGui::NavInitWindow(g.NavWindow, true);
}

static inline void ImGui::NavUpdateAnyRequestFlag()
{
    ImGuiContext& g = *GImGui;
    g.NavAnyRequest = g.NavMoveRequest || g.NavInitRequest || (IMGUI_DEBUG_NAV_SCORING && g.NavWindow != NULL);
    if (g.NavAnyRequest)
        IM_ASSERT(g.NavWindow != NULL);
}


void ImGui::NavInitWindow(ImGuiWindow* window, bool force_reinit)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(window == g.NavWindow);
    bool init_for_nav = false;
    if (!(window->Flags & ImGuiWindowFlags_NoNavInputs))
        if (!(window->Flags & ImGuiWindowFlags_ChildWindow) || (window->Flags & ImGuiWindowFlags_Popup) || (window->NavLastIds[0] == 0) || force_reinit)
            init_for_nav = true;
    if (init_for_nav)
    {
        SetNavID(0, g.NavLayer);
        g.NavInitRequest = true;
        g.NavInitRequestFromMove = false;
        g.NavInitResultId = 0;
        g.NavInitResultRectRel = ImRect();
        NavUpdateAnyRequestFlag();
    }
    else
    {
        g.NavId = window->NavLastIds[0];
    }
}

static ImVec2 ImGui::NavCalcPreferredRefPos()
{
    ImGuiContext& g = *GImGui;
    if (g.NavDisableHighlight || !g.NavDisableMouseHover || !g.NavWindow)
        return ImFloor(g.IO.MousePos);

    
    const ImRect& rect_rel = g.NavWindow->NavRectRel[g.NavLayer];
    ImVec2 pos = g.NavWindow->Pos + ImVec2(rect_rel.Min.x + ImMin(g.Style.FramePadding.x*4, rect_rel.GetWidth()), rect_rel.Max.y - ImMin(g.Style.FramePadding.y, rect_rel.GetHeight()));
    ImRect visible_rect = GetViewportRect();
    return ImFloor(ImClamp(pos, visible_rect.Min, visible_rect.Max));   
}

float ImGui::GetNavInputAmount(ImGuiNavInput n, ImGuiInputReadMode mode)
{
    ImGuiContext& g = *GImGui;
    if (mode == ImGuiInputReadMode_Down)
        return g.IO.NavInputs[n];                         

    const float t = g.IO.NavInputsDownDuration[n];
    if (t < 0.0f && mode == ImGuiInputReadMode_Released)  
        return (g.IO.NavInputsDownDurationPrev[n] >= 0.0f ? 1.0f : 0.0f);
    if (t < 0.0f)
        return 0.0f;
    if (mode == ImGuiInputReadMode_Pressed)               
        return (t == 0.0f) ? 1.0f : 0.0f;
    if (mode == ImGuiInputReadMode_Repeat)
        return (float)CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, g.IO.KeyRepeatDelay * 0.80f, g.IO.KeyRepeatRate * 0.80f);
    if (mode == ImGuiInputReadMode_RepeatSlow)
        return (float)CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, g.IO.KeyRepeatDelay * 1.00f, g.IO.KeyRepeatRate * 2.00f);
    if (mode == ImGuiInputReadMode_RepeatFast)
        return (float)CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, g.IO.KeyRepeatDelay * 0.80f, g.IO.KeyRepeatRate * 0.30f);
    return 0.0f;
}

ImVec2 ImGui::GetNavInputAmount2d(ImGuiNavDirSourceFlags dir_sources, ImGuiInputReadMode mode, float slow_factor, float fast_factor)
{
    ImVec2 delta(0.0f, 0.0f);
    if (dir_sources & ImGuiNavDirSourceFlags_Keyboard)
        delta += ImVec2(GetNavInputAmount(ImGuiNavInput_KeyRight_, mode)   - GetNavInputAmount(ImGuiNavInput_KeyLeft_,   mode), GetNavInputAmount(ImGuiNavInput_KeyDown_,   mode) - GetNavInputAmount(ImGuiNavInput_KeyUp_,   mode));
    if (dir_sources & ImGuiNavDirSourceFlags_PadDPad)
        delta += ImVec2(GetNavInputAmount(ImGuiNavInput_DpadRight, mode)   - GetNavInputAmount(ImGuiNavInput_DpadLeft,   mode), GetNavInputAmount(ImGuiNavInput_DpadDown,   mode) - GetNavInputAmount(ImGuiNavInput_DpadUp,   mode));
    if (dir_sources & ImGuiNavDirSourceFlags_PadLStick)
        delta += ImVec2(GetNavInputAmount(ImGuiNavInput_LStickRight, mode) - GetNavInputAmount(ImGuiNavInput_LStickLeft, mode), GetNavInputAmount(ImGuiNavInput_LStickDown, mode) - GetNavInputAmount(ImGuiNavInput_LStickUp, mode));
    if (slow_factor != 0.0f && IsNavInputDown(ImGuiNavInput_TweakSlow))
        delta *= slow_factor;
    if (fast_factor != 0.0f && IsNavInputDown(ImGuiNavInput_TweakFast))
        delta *= fast_factor;
    return delta;
}



static void NavScrollToBringItemIntoView(ImGuiWindow* window, const ImRect& item_rect)
{
    ImRect window_rect(window->InnerMainRect.Min - ImVec2(1, 1), window->InnerMainRect.Max + ImVec2(1, 1));
    
    if (window_rect.Contains(item_rect))
        return;

    ImGuiContext& g = *GImGui;
    if (window->ScrollbarX && item_rect.Min.x < window_rect.Min.x)
    {
        window->ScrollTarget.x = item_rect.Min.x - window->Pos.x + window->Scroll.x - g.Style.ItemSpacing.x;
        window->ScrollTargetCenterRatio.x = 0.0f;
    }
    else if (window->ScrollbarX && item_rect.Max.x >= window_rect.Max.x)
    {
        window->ScrollTarget.x = item_rect.Max.x - window->Pos.x + window->Scroll.x + g.Style.ItemSpacing.x;
        window->ScrollTargetCenterRatio.x = 1.0f;
    }
    if (item_rect.Min.y < window_rect.Min.y)
    {
        window->ScrollTarget.y = item_rect.Min.y - window->Pos.y + window->Scroll.y - g.Style.ItemSpacing.y;
        window->ScrollTargetCenterRatio.y = 0.0f;
    }
    else if (item_rect.Max.y >= window_rect.Max.y)
    {
        window->ScrollTarget.y = item_rect.Max.y - window->Pos.y + window->Scroll.y + g.Style.ItemSpacing.y;
        window->ScrollTargetCenterRatio.y = 1.0f;
    }
}

static void ImGui::NavUpdate()
{
    ImGuiContext& g = *GImGui;
    g.IO.WantSetMousePos = false;
#if 0
    if (g.NavScoringCount > 0) printf("[%05d] NavScoringCount %d for '%s' layer %d (Init:%d, Move:%d)\n", g.FrameCount, g.NavScoringCount, g.NavWindow ? g.NavWindow->Name : "NULL", g.NavLayer, g.NavInitRequest || g.NavInitResultId != 0, g.NavMoveRequest);
#endif

    
    bool nav_keyboard_active = (g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) != 0;
    bool nav_gamepad_active = (g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) != 0 && (g.IO.BackendFlags & ImGuiBackendFlags_HasGamepad) != 0;
    if (nav_gamepad_active)
        if (g.IO.NavInputs[ImGuiNavInput_Activate] > 0.0f || g.IO.NavInputs[ImGuiNavInput_Input] > 0.0f || g.IO.NavInputs[ImGuiNavInput_Cancel] > 0.0f || g.IO.NavInputs[ImGuiNavInput_Menu] > 0.0f)
            g.NavInputSource = ImGuiInputSource_NavGamepad;

    
    if (nav_keyboard_active)
    {
        #define NAV_MAP_KEY(_KEY, _NAV_INPUT) if (IsKeyDown(g.IO.KeyMap[_KEY])) { g.IO.NavInputs[_NAV_INPUT] = 1.0f; g.NavInputSource = ImGuiInputSource_NavKeyboard; }
        NAV_MAP_KEY(ImGuiKey_Space,     ImGuiNavInput_Activate );
        NAV_MAP_KEY(ImGuiKey_Enter,     ImGuiNavInput_Input    );
        NAV_MAP_KEY(ImGuiKey_Escape,    ImGuiNavInput_Cancel   );
        NAV_MAP_KEY(ImGuiKey_LeftArrow, ImGuiNavInput_KeyLeft_ );
        NAV_MAP_KEY(ImGuiKey_RightArrow,ImGuiNavInput_KeyRight_);
        NAV_MAP_KEY(ImGuiKey_UpArrow,   ImGuiNavInput_KeyUp_   );
        NAV_MAP_KEY(ImGuiKey_DownArrow, ImGuiNavInput_KeyDown_ );
        if (g.IO.KeyCtrl)   g.IO.NavInputs[ImGuiNavInput_TweakSlow] = 1.0f;
        if (g.IO.KeyShift)  g.IO.NavInputs[ImGuiNavInput_TweakFast] = 1.0f;
        if (g.IO.KeyAlt)    g.IO.NavInputs[ImGuiNavInput_KeyMenu_]  = 1.0f;
        #undef NAV_MAP_KEY
    }
    memcpy(g.IO.NavInputsDownDurationPrev, g.IO.NavInputsDownDuration, sizeof(g.IO.NavInputsDownDuration));
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.NavInputs); i++)
        g.IO.NavInputsDownDuration[i] = (g.IO.NavInputs[i] > 0.0f) ? (g.IO.NavInputsDownDuration[i] < 0.0f ? 0.0f : g.IO.NavInputsDownDuration[i] + g.IO.DeltaTime) : -1.0f;

    
    if (g.NavInitResultId != 0 && (!g.NavDisableHighlight || g.NavInitRequestFromMove))
    {
        
        IM_ASSERT(g.NavWindow);
        if (g.NavInitRequestFromMove)
            SetNavIDWithRectRel(g.NavInitResultId, g.NavLayer, g.NavInitResultRectRel);
        else
            SetNavID(g.NavInitResultId, g.NavLayer);
        g.NavWindow->NavRectRel[g.NavLayer] = g.NavInitResultRectRel;
    }
    g.NavInitRequest = false;
    g.NavInitRequestFromMove = false;
    g.NavInitResultId = 0;
    g.NavJustMovedToId = 0;

    
    if (g.NavMoveRequest && (g.NavMoveResultLocal.ID != 0 || g.NavMoveResultOther.ID != 0))
        NavUpdateMoveResult();

    
    if (g.NavMoveRequestForward == ImGuiNavForward_ForwardActive)
    {
        IM_ASSERT(g.NavMoveRequest);
        if (g.NavMoveResultLocal.ID == 0 && g.NavMoveResultOther.ID == 0)
            g.NavDisableHighlight = false;
        g.NavMoveRequestForward = ImGuiNavForward_None;
    }

    
    if (g.NavMousePosDirty && g.NavIdIsAlive)
    {
        
        if ((g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos) && (g.IO.BackendFlags & ImGuiBackendFlags_HasSetMousePos))
        {
            if (!g.NavDisableHighlight && g.NavDisableMouseHover && g.NavWindow)
            {
                g.IO.MousePos = g.IO.MousePosPrev = NavCalcPreferredRefPos();
                g.IO.WantSetMousePos = true;
            }
        }
        g.NavMousePosDirty = false;
    }
    g.NavIdIsAlive = false;
    g.NavJustTabbedId = 0;
    IM_ASSERT(g.NavLayer == 0 || g.NavLayer == 1);

    
    if (g.NavWindow)
        NavSaveLastChildNavWindow(g.NavWindow);
    if (g.NavWindow && g.NavWindow->NavLastChildNavWindow != NULL && g.NavLayer == 0)
        g.NavWindow->NavLastChildNavWindow = NULL;

    
    NavUpdateWindowing();

    
    g.IO.NavActive = (nav_keyboard_active || nav_gamepad_active) && g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs);
    g.IO.NavVisible = (g.IO.NavActive && g.NavId != 0 && !g.NavDisableHighlight) || (g.NavWindowingTarget != NULL) || g.NavInitRequest;

    
    if (IsNavInputPressed(ImGuiNavInput_Cancel, ImGuiInputReadMode_Pressed))
    {
        if (g.ActiveId != 0)
        {
            ClearActiveID();
        }
        else if (g.NavWindow && (g.NavWindow->Flags & ImGuiWindowFlags_ChildWindow) && !(g.NavWindow->Flags & ImGuiWindowFlags_Popup) && g.NavWindow->ParentWindow)
        {
            
            ImGuiWindow* child_window = g.NavWindow;
            ImGuiWindow* parent_window = g.NavWindow->ParentWindow;
            IM_ASSERT(child_window->ChildId != 0);
            FocusWindow(parent_window);
            SetNavID(child_window->ChildId, 0);
            g.NavIdIsAlive = false;
            if (g.NavDisableMouseHover)
                g.NavMousePosDirty = true;
        }
        else if (g.OpenPopupStack.Size > 0)
        {
            
            if (!(g.OpenPopupStack.back().Window->Flags & ImGuiWindowFlags_Modal))
                ClosePopupToLevel(g.OpenPopupStack.Size - 1);
        }
        else if (g.NavLayer != 0)
        {
            
            NavRestoreLayer(0);
        }
        else
        {
            
            if (g.NavWindow && ((g.NavWindow->Flags & ImGuiWindowFlags_Popup) || !(g.NavWindow->Flags & ImGuiWindowFlags_ChildWindow)))
                g.NavWindow->NavLastIds[0] = 0;
            g.NavId = 0;
        }
    }

    
    g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId = g.NavInputId = 0;
    if (g.NavId != 0 && !g.NavDisableHighlight && !g.NavWindowingTarget && g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs))
    {
        bool activate_down = IsNavInputDown(ImGuiNavInput_Activate);
        bool activate_pressed = activate_down && IsNavInputPressed(ImGuiNavInput_Activate, ImGuiInputReadMode_Pressed);
        if (g.ActiveId == 0 && activate_pressed)
            g.NavActivateId = g.NavId;
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && activate_down)
            g.NavActivateDownId = g.NavId;
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && activate_pressed)
            g.NavActivatePressedId = g.NavId;
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && IsNavInputPressed(ImGuiNavInput_Input, ImGuiInputReadMode_Pressed))
            g.NavInputId = g.NavId;
    }
    if (g.NavWindow && (g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs))
        g.NavDisableHighlight = true;
    if (g.NavActivateId != 0)
        IM_ASSERT(g.NavActivateDownId == g.NavActivateId);
    g.NavMoveRequest = false;

    
    if (g.NavNextActivateId != 0)
        g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId = g.NavInputId = g.NavNextActivateId;
    g.NavNextActivateId = 0;

    
    const int allowed_dir_flags = (g.ActiveId == 0) ? ~0 : g.ActiveIdAllowNavDirFlags;
    if (g.NavMoveRequestForward == ImGuiNavForward_None)
    {
        g.NavMoveDir = ImGuiDir_None;
        g.NavMoveRequestFlags = 0;
        if (g.NavWindow && !g.NavWindowingTarget && allowed_dir_flags && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs))
        {
            if ((allowed_dir_flags & (1<<ImGuiDir_Left))  && IsNavInputPressedAnyOfTwo(ImGuiNavInput_DpadLeft, ImGuiNavInput_KeyLeft_, ImGuiInputReadMode_Repeat)) g.NavMoveDir = ImGuiDir_Left;
            if ((allowed_dir_flags & (1<<ImGuiDir_Right)) && IsNavInputPressedAnyOfTwo(ImGuiNavInput_DpadRight,ImGuiNavInput_KeyRight_,ImGuiInputReadMode_Repeat)) g.NavMoveDir = ImGuiDir_Right;
            if ((allowed_dir_flags & (1<<ImGuiDir_Up))    && IsNavInputPressedAnyOfTwo(ImGuiNavInput_DpadUp,   ImGuiNavInput_KeyUp_,   ImGuiInputReadMode_Repeat)) g.NavMoveDir = ImGuiDir_Up;
            if ((allowed_dir_flags & (1<<ImGuiDir_Down))  && IsNavInputPressedAnyOfTwo(ImGuiNavInput_DpadDown, ImGuiNavInput_KeyDown_, ImGuiInputReadMode_Repeat)) g.NavMoveDir = ImGuiDir_Down;
        }
        g.NavMoveClipDir = g.NavMoveDir;
    }
    else
    {
        
        
        IM_ASSERT(g.NavMoveDir != ImGuiDir_None && g.NavMoveClipDir != ImGuiDir_None);
        IM_ASSERT(g.NavMoveRequestForward == ImGuiNavForward_ForwardQueued);
        g.NavMoveRequestForward = ImGuiNavForward_ForwardActive;
    }

    
    float nav_scoring_rect_offset_y = 0.0f;
    if (nav_keyboard_active)
        nav_scoring_rect_offset_y = NavUpdatePageUpPageDown(allowed_dir_flags);

    
    if (g.NavMoveDir != ImGuiDir_None)
    {
        g.NavMoveRequest = true;
        g.NavMoveDirLast = g.NavMoveDir;
    }
    if (g.NavMoveRequest && g.NavId == 0)
    {
        g.NavInitRequest = g.NavInitRequestFromMove = true;
        g.NavInitResultId = 0;
        g.NavDisableHighlight = false;
    }
    NavUpdateAnyRequestFlag();

    
    if (g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs) && !g.NavWindowingTarget)
    {
        
        ImGuiWindow* window = g.NavWindow;
        const float scroll_speed = ImFloor(window->CalcFontSize() * 100 * g.IO.DeltaTime + 0.5f); 
        if (window->DC.NavLayerActiveMask == 0x00 && window->DC.NavHasScroll && g.NavMoveRequest)
        {
            if (g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right)
                SetWindowScrollX(window, ImFloor(window->Scroll.x + ((g.NavMoveDir == ImGuiDir_Left) ? -1.0f : +1.0f) * scroll_speed));
            if (g.NavMoveDir == ImGuiDir_Up || g.NavMoveDir == ImGuiDir_Down)
                SetWindowScrollY(window, ImFloor(window->Scroll.y + ((g.NavMoveDir == ImGuiDir_Up) ? -1.0f : +1.0f) * scroll_speed));
        }

        
        
        ImVec2 scroll_dir = GetNavInputAmount2d(ImGuiNavDirSourceFlags_PadLStick, ImGuiInputReadMode_Down, 1.0f/10.0f, 10.0f);
        if (scroll_dir.x != 0.0f && window->ScrollbarX)
        {
            SetWindowScrollX(window, ImFloor(window->Scroll.x + scroll_dir.x * scroll_speed));
            g.NavMoveFromClampedRefRect = true;
        }
        if (scroll_dir.y != 0.0f)
        {
            SetWindowScrollY(window, ImFloor(window->Scroll.y + scroll_dir.y * scroll_speed));
            g.NavMoveFromClampedRefRect = true;
        }
    }

    
    g.NavMoveResultLocal.Clear();
    g.NavMoveResultLocalVisibleSet.Clear();
    g.NavMoveResultOther.Clear();

    
    if (g.NavMoveRequest && g.NavMoveFromClampedRefRect && g.NavLayer == 0)
    {
        ImGuiWindow* window = g.NavWindow;
        ImRect window_rect_rel(window->InnerMainRect.Min - window->Pos - ImVec2(1,1), window->InnerMainRect.Max - window->Pos + ImVec2(1,1));
        if (!window_rect_rel.Contains(window->NavRectRel[g.NavLayer]))
        {
            float pad = window->CalcFontSize() * 0.5f;
            window_rect_rel.Expand(ImVec2(-ImMin(window_rect_rel.GetWidth(), pad), -ImMin(window_rect_rel.GetHeight(), pad))); 
            window->NavRectRel[g.NavLayer].ClipWith(window_rect_rel);
            g.NavId = 0;
        }
        g.NavMoveFromClampedRefRect = false;
    }

    
    ImRect nav_rect_rel = (g.NavWindow && !g.NavWindow->NavRectRel[g.NavLayer].IsInverted()) ? g.NavWindow->NavRectRel[g.NavLayer] : ImRect(0,0,0,0);
    g.NavScoringRectScreen = g.NavWindow ? ImRect(g.NavWindow->Pos + nav_rect_rel.Min, g.NavWindow->Pos + nav_rect_rel.Max) : GetViewportRect();
    g.NavScoringRectScreen.TranslateY(nav_scoring_rect_offset_y);
    g.NavScoringRectScreen.Min.x = ImMin(g.NavScoringRectScreen.Min.x + 1.0f, g.NavScoringRectScreen.Max.x);
    g.NavScoringRectScreen.Max.x = g.NavScoringRectScreen.Min.x;
    IM_ASSERT(!g.NavScoringRectScreen.IsInverted()); 
    
    g.NavScoringCount = 0;
#if IMGUI_DEBUG_NAV_RECTS
    if (g.NavWindow) { for (int layer = 0; layer < 2; layer++) GetOverlayDrawList()->AddRect(g.NavWindow->Pos + g.NavWindow->NavRectRel[layer].Min, g.NavWindow->Pos + g.NavWindow->NavRectRel[layer].Max, IM_COL32(255,200,0,255)); } 
    if (g.NavWindow) { ImU32 col = (g.NavWindow->HiddenFrames == 0) ? IM_COL32(255,0,255,255) : IM_COL32(255,0,0,255); ImVec2 p = NavCalcPreferredRefPos(); char buf[32]; ImFormatString(buf, 32, "%d", g.NavLayer); g.OverlayDrawList.AddCircleFilled(p, 3.0f, col); g.OverlayDrawList.AddText(NULL, 13.0f, p + ImVec2(8,-4), col, buf); }
#endif
}

static void ImGui::NavUpdateMoveResult()
{
    
    ImGuiContext& g = *GImGui;
    ImGuiNavMoveResult* result = (g.NavMoveResultLocal.ID != 0) ? &g.NavMoveResultLocal : &g.NavMoveResultOther;

    
    if (g.NavMoveRequestFlags & ImGuiNavMoveFlags_AlsoScoreVisibleSet)
        if (g.NavMoveResultLocalVisibleSet.ID != 0 && g.NavMoveResultLocalVisibleSet.ID != g.NavId)
            result = &g.NavMoveResultLocalVisibleSet;

    
    if (result != &g.NavMoveResultOther && g.NavMoveResultOther.ID != 0 && g.NavMoveResultOther.Window->ParentWindow == g.NavWindow)
        if ((g.NavMoveResultOther.DistBox < result->DistBox) || (g.NavMoveResultOther.DistBox == result->DistBox && g.NavMoveResultOther.DistCenter < result->DistCenter))
            result = &g.NavMoveResultOther;
    IM_ASSERT(g.NavWindow && result->Window);

    
    if (g.NavLayer == 0)
    {
        ImRect rect_abs = ImRect(result->RectRel.Min + result->Window->Pos, result->RectRel.Max + result->Window->Pos);
        NavScrollToBringItemIntoView(result->Window, rect_abs);

        
        ImVec2 next_scroll = CalcNextScrollFromScrollTargetAndClamp(result->Window, false);
        ImVec2 delta_scroll = result->Window->Scroll - next_scroll;
        result->RectRel.Translate(delta_scroll);

        
        if (result->Window->Flags & ImGuiWindowFlags_ChildWindow)
            NavScrollToBringItemIntoView(result->Window->ParentWindow, ImRect(rect_abs.Min + delta_scroll, rect_abs.Max + delta_scroll));
    }

    
    ClearActiveID();
    g.NavWindow = result->Window;
    SetNavIDWithRectRel(result->ID, g.NavLayer, result->RectRel);
    g.NavJustMovedToId = result->ID;
    g.NavMoveFromClampedRefRect = false;
}

static float ImGui::NavUpdatePageUpPageDown(int allowed_dir_flags)
{
    ImGuiContext& g = *GImGui;
    if (g.NavMoveDir == ImGuiDir_None && g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs) && !g.NavWindowingTarget && g.NavLayer == 0)
    {
        ImGuiWindow* window = g.NavWindow;
        bool page_up_held = IsKeyDown(g.IO.KeyMap[ImGuiKey_PageUp]) && (allowed_dir_flags & (1 << ImGuiDir_Up));
        bool page_down_held = IsKeyDown(g.IO.KeyMap[ImGuiKey_PageDown]) && (allowed_dir_flags & (1 << ImGuiDir_Down));
        if ((page_up_held && !page_down_held) || (page_down_held && !page_up_held))
        {
            if (window->DC.NavLayerActiveMask == 0x00 && window->DC.NavHasScroll)
            {
                
                if (IsKeyPressed(g.IO.KeyMap[ImGuiKey_PageUp], true))
                    SetWindowScrollY(window, window->Scroll.y - window->InnerClipRect.GetHeight());
                else if (IsKeyPressed(g.IO.KeyMap[ImGuiKey_PageDown], true))
                    SetWindowScrollY(window, window->Scroll.y + window->InnerClipRect.GetHeight());
            }
            else
            {
                const ImRect& nav_rect_rel = window->NavRectRel[g.NavLayer];
                const float page_offset_y = ImMax(0.0f, window->InnerClipRect.GetHeight() - window->CalcFontSize() * 1.0f + nav_rect_rel.GetHeight());
                float nav_scoring_rect_offset_y = 0.0f;
                if (IsKeyPressed(g.IO.KeyMap[ImGuiKey_PageUp], true))
                {
                    nav_scoring_rect_offset_y = -page_offset_y;
                    g.NavMoveDir = ImGuiDir_Down; 
                    g.NavMoveClipDir = ImGuiDir_Up;
                    g.NavMoveRequestFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_AlsoScoreVisibleSet;
                }
                else if (IsKeyPressed(g.IO.KeyMap[ImGuiKey_PageDown], true))
                {
                    nav_scoring_rect_offset_y = +page_offset_y;
                    g.NavMoveDir = ImGuiDir_Up; 
                    g.NavMoveClipDir = ImGuiDir_Down;
                    g.NavMoveRequestFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_AlsoScoreVisibleSet;
                }
                return nav_scoring_rect_offset_y;
            }
        }
    }
    return 0.0f;
}

static int FindWindowIndex(ImGuiWindow* window) 
{
    ImGuiContext& g = *GImGui;
    for (int i = g.Windows.Size-1; i >= 0; i--)
        if (g.Windows[i] == window)
            return i;
    return -1;
}

static ImGuiWindow* FindWindowNavFocusable(int i_start, int i_stop, int dir) 
{
    ImGuiContext& g = *GImGui;
    for (int i = i_start; i >= 0 && i < g.Windows.Size && i != i_stop; i += dir)
        if (ImGui::IsWindowNavFocusable(g.Windows[i]))
            return g.Windows[i];
    return NULL;
}

static void NavUpdateWindowingHighlightWindow(int focus_change_dir)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.NavWindowingTarget);
    if (g.NavWindowingTarget->Flags & ImGuiWindowFlags_Modal)
        return;

    const int i_current = FindWindowIndex(g.NavWindowingTarget);
    ImGuiWindow* window_target = FindWindowNavFocusable(i_current + focus_change_dir, -INT_MAX, focus_change_dir);
    if (!window_target)
        window_target = FindWindowNavFocusable((focus_change_dir < 0) ? (g.Windows.Size - 1) : 0, i_current, focus_change_dir);
    if (window_target) 
        g.NavWindowingTarget = g.NavWindowingTargetAnim = window_target;
    g.NavWindowingToggleLayer = false;
}


static void ImGui::NavUpdateWindowing()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* apply_focus_window = NULL;
    bool apply_toggle_layer = false;

    ImGuiWindow* modal_window = GetFrontMostPopupModal();
    if (modal_window != NULL)
    {
        g.NavWindowingTarget = NULL;
        return;
    }

    
    if (g.NavWindowingTargetAnim && g.NavWindowingTarget == NULL)
    {
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha - g.IO.DeltaTime * 10.0f, 0.0f);
        if (g.DimBgRatio <= 0.0f && g.NavWindowingHighlightAlpha <= 0.0f)
            g.NavWindowingTargetAnim = NULL;
    }

    
    bool start_windowing_with_gamepad = !g.NavWindowingTarget && IsNavInputPressed(ImGuiNavInput_Menu, ImGuiInputReadMode_Pressed);
    bool start_windowing_with_keyboard = !g.NavWindowingTarget && g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_Tab) && (g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard);
    if (start_windowing_with_gamepad || start_windowing_with_keyboard)
        if (ImGuiWindow* window = g.NavWindow ? g.NavWindow : FindWindowNavFocusable(g.Windows.Size - 1, -INT_MAX, -1))
        {
            g.NavWindowingTarget = g.NavWindowingTargetAnim = window;
            g.NavWindowingTimer = g.NavWindowingHighlightAlpha = 0.0f;
            g.NavWindowingToggleLayer = start_windowing_with_keyboard ? false : true;
            g.NavInputSource = start_windowing_with_keyboard ? ImGuiInputSource_NavKeyboard : ImGuiInputSource_NavGamepad;
        }

    
    g.NavWindowingTimer += g.IO.DeltaTime;
    if (g.NavWindowingTarget && g.NavInputSource == ImGuiInputSource_NavGamepad)
    {
        
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha, ImSaturate((g.NavWindowingTimer - NAV_WINDOWING_HIGHLIGHT_DELAY) / 0.05f));

        
        const int focus_change_dir = (int)IsNavInputPressed(ImGuiNavInput_FocusPrev, ImGuiInputReadMode_RepeatSlow) - (int)IsNavInputPressed(ImGuiNavInput_FocusNext, ImGuiInputReadMode_RepeatSlow);
        if (focus_change_dir != 0)
        {
            NavUpdateWindowingHighlightWindow(focus_change_dir);
            g.NavWindowingHighlightAlpha = 1.0f;
        }

        
        if (!IsNavInputDown(ImGuiNavInput_Menu))
        {
            g.NavWindowingToggleLayer &= (g.NavWindowingHighlightAlpha < 1.0f); 
            if (g.NavWindowingToggleLayer && g.NavWindow)
                apply_toggle_layer = true;
            else if (!g.NavWindowingToggleLayer)
                apply_focus_window = g.NavWindowingTarget;
            g.NavWindowingTarget = NULL;
        }
    }

    
    if (g.NavWindowingTarget && g.NavInputSource == ImGuiInputSource_NavKeyboard)
    {
        
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha, ImSaturate((g.NavWindowingTimer - NAV_WINDOWING_HIGHLIGHT_DELAY) / 0.05f)); 
        if (IsKeyPressedMap(ImGuiKey_Tab, true))
            NavUpdateWindowingHighlightWindow(g.IO.KeyShift ? +1 : -1);
        if (!g.IO.KeyCtrl)
            apply_focus_window = g.NavWindowingTarget;
    }

    
    
    if ((g.ActiveId == 0 || g.ActiveIdAllowOverlap) && IsNavInputPressed(ImGuiNavInput_KeyMenu_, ImGuiInputReadMode_Released))
        if (IsMousePosValid(&g.IO.MousePos) == IsMousePosValid(&g.IO.MousePosPrev))
            apply_toggle_layer = true;

    
    if (g.NavWindowingTarget && !(g.NavWindowingTarget->Flags & ImGuiWindowFlags_NoMove))
    {
        ImVec2 move_delta;
        if (g.NavInputSource == ImGuiInputSource_NavKeyboard && !g.IO.KeyShift)
            move_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard, ImGuiInputReadMode_Down);
        if (g.NavInputSource == ImGuiInputSource_NavGamepad)
            move_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_PadLStick, ImGuiInputReadMode_Down);
        if (move_delta.x != 0.0f || move_delta.y != 0.0f)
        {
            const float NAV_MOVE_SPEED = 800.0f;
            const float move_speed = ImFloor(NAV_MOVE_SPEED * g.IO.DeltaTime * ImMin(g.IO.DisplayFramebufferScale.x, g.IO.DisplayFramebufferScale.y)); 
            g.NavWindowingTarget->RootWindow->Pos += move_delta * move_speed;
            g.NavDisableMouseHover = true;
            MarkIniSettingsDirty(g.NavWindowingTarget);
        }
    }

    
    if (apply_focus_window && (g.NavWindow == NULL || apply_focus_window != g.NavWindow->RootWindow))
    {
        g.NavDisableHighlight = false;
        g.NavDisableMouseHover = true;
        apply_focus_window = NavRestoreLastChildNavWindow(apply_focus_window);
        ClosePopupsOverWindow(apply_focus_window);
        FocusWindow(apply_focus_window);
        if (apply_focus_window->NavLastIds[0] == 0)
            NavInitWindow(apply_focus_window, false);

        
        if (apply_focus_window->DC.NavLayerActiveMask == (1 << 1))
            g.NavLayer = 1;
    }
    if (apply_focus_window)
        g.NavWindowingTarget = NULL;

    
    if (apply_toggle_layer && g.NavWindow)
    {
        
        ImGuiWindow* new_nav_window = g.NavWindow;
        while ((new_nav_window->DC.NavLayerActiveMask & (1 << 1)) == 0 && (new_nav_window->Flags & ImGuiWindowFlags_ChildWindow) != 0 && (new_nav_window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) == 0)
            new_nav_window = new_nav_window->ParentWindow;
        if (new_nav_window != g.NavWindow)
        {
            ImGuiWindow* old_nav_window = g.NavWindow;
            FocusWindow(new_nav_window);
            new_nav_window->NavLastChildNavWindow = old_nav_window;
        }
        g.NavDisableHighlight = false;
        g.NavDisableMouseHover = true;
        NavRestoreLayer((g.NavWindow->DC.NavLayerActiveMask & (1 << 1)) ? (g.NavLayer ^ 1) : 0);
    }
}


static const char* GetFallbackWindowNameForWindowingList(ImGuiWindow* window)
{
    if (window->Flags & ImGuiWindowFlags_Popup)
        return "(Popup)";
    if ((window->Flags & ImGuiWindowFlags_MenuBar) && strcmp(window->Name, "##MainMenuBar") == 0)
        return "(Main menu bar)";
    return "(Untitled)";
}


void ImGui::NavUpdateWindowingList()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.NavWindowingTarget != NULL);

    if (g.NavWindowingTimer < NAV_WINDOWING_LIST_APPEAR_DELAY)
        return;

    if (g.NavWindowingList == NULL)
        g.NavWindowingList = FindWindowByName("###NavWindowingList");
    SetNextWindowSizeConstraints(ImVec2(g.IO.DisplaySize.x * 0.20f, g.IO.DisplaySize.y * 0.20f), ImVec2(FLT_MAX, FLT_MAX));
    SetNextWindowPos(g.IO.DisplaySize * 0.5f, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    PushStyleVar(ImGuiStyleVar_WindowPadding, g.Style.WindowPadding * 2.0f);
    Begin("###NavWindowingList", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    for (int n = g.Windows.Size - 1; n >= 0; n--)
    {
        ImGuiWindow* window = g.Windows[n];
        if (!IsWindowNavFocusable(window))
            continue;
        const char* label = window->Name;
        if (label == FindRenderedTextEnd(label))
            label = GetFallbackWindowNameForWindowingList(window);
        Selectable(label, g.NavWindowingTarget == window);
    }
    End();
    PopStyleVar();
}






void ImGui::NextColumn()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems || window->DC.ColumnsSet == NULL)
        return;

    ImGuiContext& g = *GImGui;
    PopItemWidth();
    PopClipRect();

    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
    if (++columns->Current < columns->Count)
    {
        
        window->DC.ColumnsOffset.x = GetColumnOffset(columns->Current) - window->DC.Indent.x + g.Style.ItemSpacing.x;
        window->DrawList->ChannelsSetCurrent(columns->Current);
    }
    else
    {
        window->DC.ColumnsOffset.x = 0.0f;
        window->DrawList->ChannelsSetCurrent(0);
        columns->Current = 0;
        columns->LineMinY = columns->LineMaxY;
    }
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
    window->DC.CursorPos.y = columns->LineMinY;
    window->DC.CurrentLineSize = ImVec2(0.0f, 0.0f);
    window->DC.CurrentLineTextBaseOffset = 0.0f;

    PushColumnClipRect();
    PushItemWidth(GetColumnWidth() * 0.65f);  
}

int ImGui::GetColumnIndex()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.ColumnsSet ? window->DC.ColumnsSet->Current : 0;
}

int ImGui::GetColumnsCount()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.ColumnsSet ? window->DC.ColumnsSet->Count : 1;
}

static float OffsetNormToPixels(const ImGuiColumnsSet* columns, float offset_norm)
{
    return offset_norm * (columns->MaxX - columns->MinX);
}

static float PixelsToOffsetNorm(const ImGuiColumnsSet* columns, float offset)
{
    return offset / (columns->MaxX - columns->MinX);
}

static inline float GetColumnsRectHalfWidth() { return 4.0f; }

static float GetDraggedColumnOffset(ImGuiColumnsSet* columns, int column_index)
{
    
    
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT(column_index > 0); 
    IM_ASSERT(g.ActiveId == columns->ID + ImGuiID(column_index));

    float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + GetColumnsRectHalfWidth() - window->Pos.x;
    x = ImMax(x, ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing);
    if ((columns->Flags & ImGuiColumnsFlags_NoPreserveWidths))
        x = ImMin(x, ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

    return x;
}

float ImGui::GetColumnOffset(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    IM_ASSERT(column_index < columns->Columns.Size);

    const float t = columns->Columns[column_index].OffsetNorm;
    const float x_offset = ImLerp(columns->MinX, columns->MaxX, t);
    return x_offset;
}

static float GetColumnWidthEx(ImGuiColumnsSet* columns, int column_index, bool before_resize = false)
{
    if (column_index < 0)
        column_index = columns->Current;

    float offset_norm;
    if (before_resize)
        offset_norm = columns->Columns[column_index + 1].OffsetNormBeforeResize - columns->Columns[column_index].OffsetNormBeforeResize;
    else
        offset_norm = columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm;
    return OffsetNormToPixels(columns, offset_norm);
}

float ImGui::GetColumnWidth(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    return OffsetNormToPixels(columns, columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm);
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    IM_ASSERT(column_index < columns->Columns.Size);

    const bool preserve_width = !(columns->Flags & ImGuiColumnsFlags_NoPreserveWidths) && (column_index < columns->Count-1);
    const float width = preserve_width ? GetColumnWidthEx(columns, column_index, columns->IsBeingResized) : 0.0f;

    if (!(columns->Flags & ImGuiColumnsFlags_NoForceWithinWindow))
        offset = ImMin(offset, columns->MaxX - g.Style.ColumnsMinSpacing * (columns->Count - column_index));
    columns->Columns[column_index].OffsetNorm = PixelsToOffsetNorm(columns, offset - columns->MinX);

    if (preserve_width)
        SetColumnOffset(column_index + 1, offset + ImMax(g.Style.ColumnsMinSpacing, width));
}

void ImGui::SetColumnWidth(int column_index, float width)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    SetColumnOffset(column_index + 1, GetColumnOffset(column_index) + width);
}

void ImGui::PushColumnClipRect(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    if (column_index < 0)
        column_index = columns->Current;

    PushClipRect(columns->Columns[column_index].ClipRect.Min, columns->Columns[column_index].ClipRect.Max, false);
}

static ImGuiColumnsSet* FindOrAddColumnsSet(ImGuiWindow* window, ImGuiID id)
{
    for (int n = 0; n < window->ColumnsStorage.Size; n++)
        if (window->ColumnsStorage[n].ID == id)
            return &window->ColumnsStorage[n];

    window->ColumnsStorage.push_back(ImGuiColumnsSet());
    ImGuiColumnsSet* columns = &window->ColumnsStorage.back();
    columns->ID = id;
    return columns;
}

void ImGui::BeginColumns(const char* str_id, int columns_count, ImGuiColumnsFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    IM_ASSERT(columns_count > 1);
    IM_ASSERT(window->DC.ColumnsSet == NULL); 

    
    
    PushID(0x11223347 + (str_id ? 0 : columns_count));
    ImGuiID id = window->GetID(str_id ? str_id : "columns");
    PopID();

    
    ImGuiColumnsSet* columns = FindOrAddColumnsSet(window, id);
    IM_ASSERT(columns->ID == id);
    columns->Current = 0;
    columns->Count = columns_count;
    columns->Flags = flags;
    window->DC.ColumnsSet = columns;

    
    const float content_region_width = (window->SizeContentsExplicit.x != 0.0f) ? (window->SizeContentsExplicit.x) : (window->InnerClipRect.Max.x - window->Pos.x);
    columns->MinX = window->DC.Indent.x - g.Style.ItemSpacing.x; 
    columns->MaxX = ImMax(content_region_width - window->Scroll.x, columns->MinX + 1.0f);
    columns->StartPosY = window->DC.CursorPos.y;
    columns->StartMaxPosX = window->DC.CursorMaxPos.x;
    columns->LineMinY = columns->LineMaxY = window->DC.CursorPos.y;
    window->DC.ColumnsOffset.x = 0.0f;
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);

    
    if (columns->Columns.Size != 0 && columns->Columns.Size != columns_count + 1)
        columns->Columns.resize(0);

    
    columns->IsFirstFrame = (columns->Columns.Size == 0);
    if (columns->Columns.Size == 0)
    {
        columns->Columns.reserve(columns_count + 1);
        for (int n = 0; n < columns_count + 1; n++)
        {
            ImGuiColumnData column;
            column.OffsetNorm = n / (float)columns_count;
            columns->Columns.push_back(column);
        }
    }

    for (int n = 0; n < columns_count; n++)
    {
        
        ImGuiColumnData* column = &columns->Columns[n];
        float clip_x1 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n) - 1.0f);
        float clip_x2 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
        column->ClipRect = ImRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
        column->ClipRect.ClipWith(window->ClipRect);
    }

    window->DrawList->ChannelsSplit(columns->Count);
    PushColumnClipRect();
    PushItemWidth(GetColumnWidth() * 0.65f);
}

void ImGui::EndColumns()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    ImGuiColumnsSet* columns = window->DC.ColumnsSet;
    IM_ASSERT(columns != NULL);

    PopItemWidth();
    PopClipRect();
    window->DrawList->ChannelsMerge();

    columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
    window->DC.CursorPos.y = columns->LineMaxY;
    if (!(columns->Flags & ImGuiColumnsFlags_GrowParentContentsSize))
        window->DC.CursorMaxPos.x = columns->StartMaxPosX;  

    
    bool is_being_resized = false;
    if (!(columns->Flags & ImGuiColumnsFlags_NoBorder) && !window->SkipItems)
    {
        const float y1 = columns->StartPosY;
        const float y2 = window->DC.CursorPos.y;
        int dragging_column = -1;
        for (int n = 1; n < columns->Count; n++)
        {
            float x = window->Pos.x + GetColumnOffset(n);
            const ImGuiID column_id = columns->ID + ImGuiID(n);
            const float column_hw = GetColumnsRectHalfWidth(); 
            const ImRect column_rect(ImVec2(x - column_hw, y1), ImVec2(x + column_hw, y2));
            KeepAliveID(column_id);
            if (IsClippedEx(column_rect, column_id, false))
                continue;

            bool hovered = false, held = false;
            if (!(columns->Flags & ImGuiColumnsFlags_NoResize))
            {
                ButtonBehavior(column_rect, column_id, &hovered, &held);
                if (hovered || held)
                    g.MouseCursor = ImGuiMouseCursor_ResizeEW;
                if (held && !(columns->Columns[n].Flags & ImGuiColumnsFlags_NoResize))
                    dragging_column = n;
            }

            
            const ImU32 col = GetColorU32(held ? ImGuiCol_SeparatorActive : hovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
            const float xi = (float)(int)x;
            window->DrawList->AddLine(ImVec2(xi, ImMax(y1 + 1.0f, window->ClipRect.Min.y)), ImVec2(xi, ImMin(y2, window->ClipRect.Max.y)), col);
        }

        
        if (dragging_column != -1)
        {
            if (!columns->IsBeingResized)
                for (int n = 0; n < columns->Count + 1; n++)
                    columns->Columns[n].OffsetNormBeforeResize = columns->Columns[n].OffsetNorm;
            columns->IsBeingResized = is_being_resized = true;
            float x = GetDraggedColumnOffset(columns, dragging_column);
            SetColumnOffset(dragging_column, x);
        }
    }
    columns->IsBeingResized = is_being_resized;

    window->DC.ColumnsSet = NULL;
    window->DC.ColumnsOffset.x = 0.0f;
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
}


void ImGui::Columns(int columns_count, const char* id, bool border)
{
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(columns_count >= 1);

    ImGuiColumnsFlags flags = (border ? 0 : ImGuiColumnsFlags_NoBorder);
    
    if (window->DC.ColumnsSet != NULL && window->DC.ColumnsSet->Count == columns_count && window->DC.ColumnsSet->Flags == flags)
        return;

    if (window->DC.ColumnsSet != NULL)
        EndColumns();

    if (columns_count != 1)
        BeginColumns(id, columns_count, flags);
}





void ImGui::ClearDragDrop()
{
    ImGuiContext& g = *GImGui;
    g.DragDropActive = false;
    g.DragDropPayload.Clear();
    g.DragDropAcceptFlags = 0;
    g.DragDropAcceptIdCurr = g.DragDropAcceptIdPrev = 0;
    g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
    g.DragDropAcceptFrameCount = -1;

    g.DragDropPayloadBufHeap.clear();
    memset(&g.DragDropPayloadBufLocal, 0, sizeof(g.DragDropPayloadBufLocal));
}



bool ImGui::BeginDragDropSource(ImGuiDragDropFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    bool source_drag_active = false;
    ImGuiID source_id = 0;
    ImGuiID source_parent_id = 0;
    int mouse_button = 0;
    if (!(flags & ImGuiDragDropFlags_SourceExtern))
    {
        source_id = window->DC.LastItemId;
        if (source_id != 0 && g.ActiveId != source_id) 
            return false;
        if (g.IO.MouseDown[mouse_button] == false)
            return false;

        if (source_id == 0)
        {
            
            
            if (!(flags & ImGuiDragDropFlags_SourceAllowNullID))
            {
                IM_ASSERT(0);
                return false;
            }

            
            
            
            
            bool is_hovered = (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) != 0;
            if (!is_hovered && (g.ActiveId == 0 || g.ActiveIdWindow != window))
                return false;
            source_id = window->DC.LastItemId = window->GetIDFromRectangle(window->DC.LastItemRect);
            if (is_hovered)
                SetHoveredID(source_id);
            if (is_hovered && g.IO.MouseClicked[mouse_button])
            {
                SetActiveID(source_id, window);
                FocusWindow(window);
            }
            if (g.ActiveId == source_id) 
                g.ActiveIdAllowOverlap = is_hovered;
        }
        else
        {
            g.ActiveIdAllowOverlap = false;
        }
        if (g.ActiveId != source_id)
            return false;
        source_parent_id = window->IDStack.back();
        source_drag_active = IsMouseDragging(mouse_button);
    }
    else
    {
        window = NULL;
        source_id = ImHash("#SourceExtern", 0);
        source_drag_active = true;
    }

    if (source_drag_active)
    {
        if (!g.DragDropActive)
        {
            IM_ASSERT(source_id != 0);
            ClearDragDrop();
            ImGuiPayload& payload = g.DragDropPayload;
            payload.SourceId = source_id;
            payload.SourceParentId = source_parent_id;
            g.DragDropActive = true;
            g.DragDropSourceFlags = flags;
            g.DragDropMouseButton = mouse_button;
        }
        g.DragDropSourceFrameCount = g.FrameCount;
        g.DragDropWithinSourceOrTarget = true;

        if (!(flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
        {
            
            
            BeginTooltip();
            if (g.DragDropActive && g.DragDropAcceptIdPrev && (g.DragDropAcceptFlags & ImGuiDragDropFlags_AcceptNoPreviewTooltip))
            {
                ImGuiWindow* tooltip_window = g.CurrentWindow;
                tooltip_window->SkipItems = true;
                tooltip_window->HiddenFramesRegular = 1;
            }
        }

        if (!(flags & ImGuiDragDropFlags_SourceNoDisableHover) && !(flags & ImGuiDragDropFlags_SourceExtern))
            window->DC.LastItemStatusFlags &= ~ImGuiItemStatusFlags_HoveredRect;

        return true;
    }
    return false;
}

void ImGui::EndDragDropSource()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.DragDropActive);
    IM_ASSERT(g.DragDropWithinSourceOrTarget && "Not after a BeginDragDropSource()?");

    if (!(g.DragDropSourceFlags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
        EndTooltip();

    
    if (g.DragDropPayload.DataFrameCount == -1)
        ClearDragDrop();
    g.DragDropWithinSourceOrTarget = false;
}


bool ImGui::SetDragDropPayload(const char* type, const void* data, size_t data_size, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    ImGuiPayload& payload = g.DragDropPayload;
    if (cond == 0)
        cond = ImGuiCond_Always;

    IM_ASSERT(type != NULL);
    IM_ASSERT(strlen(type) < IM_ARRAYSIZE(payload.DataType) && "Payload type can be at most 32 characters long");
    IM_ASSERT((data != NULL && data_size > 0) || (data == NULL && data_size == 0));
    IM_ASSERT(cond == ImGuiCond_Always || cond == ImGuiCond_Once);
    IM_ASSERT(payload.SourceId != 0);                               

    if (cond == ImGuiCond_Always || payload.DataFrameCount == -1)
    {
        
        ImStrncpy(payload.DataType, type, IM_ARRAYSIZE(payload.DataType));
        g.DragDropPayloadBufHeap.resize(0);
        if (data_size > sizeof(g.DragDropPayloadBufLocal))
        {
            
            g.DragDropPayloadBufHeap.resize((int)data_size);
            payload.Data = g.DragDropPayloadBufHeap.Data;
            memcpy(payload.Data, data, data_size);
        }
        else if (data_size > 0)
        {
            
            memset(&g.DragDropPayloadBufLocal, 0, sizeof(g.DragDropPayloadBufLocal));
            payload.Data = g.DragDropPayloadBufLocal;
            memcpy(payload.Data, data, data_size);
        }
        else
        {
            payload.Data = NULL;
        }
        payload.DataSize = (int)data_size;
    }
    payload.DataFrameCount = g.FrameCount;

    return (g.DragDropAcceptFrameCount == g.FrameCount) || (g.DragDropAcceptFrameCount == g.FrameCount - 1);
}

bool ImGui::BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    if (!g.DragDropActive)
        return false;

    ImGuiWindow* window = g.CurrentWindow;
    if (g.HoveredWindow == NULL || window->RootWindow != g.HoveredWindow->RootWindow)
        return false;
    IM_ASSERT(id != 0);
    if (!IsMouseHoveringRect(bb.Min, bb.Max) || (id == g.DragDropPayload.SourceId))
        return false;
    if (window->SkipItems)
        return false;

    IM_ASSERT(g.DragDropWithinSourceOrTarget == false);
    g.DragDropTargetRect = bb;
    g.DragDropTargetId = id;
    g.DragDropWithinSourceOrTarget = true;
    return true;
}





bool ImGui::BeginDragDropTarget()
{
    ImGuiContext& g = *GImGui;
    if (!g.DragDropActive)
        return false;

    ImGuiWindow* window = g.CurrentWindow;
    if (!(window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect))
        return false;
    if (g.HoveredWindow == NULL || window->RootWindow != g.HoveredWindow->RootWindow)
        return false;

    const ImRect& display_rect = (window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HasDisplayRect) ? window->DC.LastItemDisplayRect : window->DC.LastItemRect;
    ImGuiID id = window->DC.LastItemId;
    if (id == 0)
        id = window->GetIDFromRectangle(display_rect);
    if (g.DragDropPayload.SourceId == id)
        return false;

    IM_ASSERT(g.DragDropWithinSourceOrTarget == false);
    g.DragDropTargetRect = display_rect;
    g.DragDropTargetId = id;
    g.DragDropWithinSourceOrTarget = true;
    return true;
}

bool ImGui::IsDragDropPayloadBeingAccepted()
{
    ImGuiContext& g = *GImGui;
    return g.DragDropActive && g.DragDropAcceptIdPrev != 0;
}

const ImGuiPayload* ImGui::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiPayload& payload = g.DragDropPayload;
    IM_ASSERT(g.DragDropActive);                        
    IM_ASSERT(payload.DataFrameCount != -1);            
    if (type != NULL && !payload.IsDataType(type))
        return NULL;

    
    
    const bool was_accepted_previously = (g.DragDropAcceptIdPrev == g.DragDropTargetId);
    ImRect r = g.DragDropTargetRect;
    float r_surface = r.GetWidth() * r.GetHeight();
    if (r_surface < g.DragDropAcceptIdCurrRectSurface)
    {
        g.DragDropAcceptFlags = flags;
        g.DragDropAcceptIdCurr = g.DragDropTargetId;
        g.DragDropAcceptIdCurrRectSurface = r_surface;
    }

    
    payload.Preview = was_accepted_previously;
    flags |= (g.DragDropSourceFlags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect); 
    if (!(flags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect) && payload.Preview)
    {
        
        r.Expand(3.5f);
        bool push_clip_rect = !window->ClipRect.Contains(r);
        if (push_clip_rect) window->DrawList->PushClipRect(r.Min-ImVec2(1,1), r.Max+ImVec2(1,1));
        window->DrawList->AddRect(r.Min, r.Max, GetColorU32(ImGuiCol_DragDropTarget), 0.0f, ~0, 2.0f);
        if (push_clip_rect) window->DrawList->PopClipRect();
    }

    g.DragDropAcceptFrameCount = g.FrameCount;
    payload.Delivery = was_accepted_previously && !IsMouseDown(g.DragDropMouseButton); 
    if (!payload.Delivery && !(flags & ImGuiDragDropFlags_AcceptBeforeDelivery))
        return NULL;

    return &payload;
}


void ImGui::EndDragDropTarget()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.DragDropActive);
    IM_ASSERT(g.DragDropWithinSourceOrTarget);
    g.DragDropWithinSourceOrTarget = false;
}






void ImGui::LogText(const char* fmt, ...)
{
    ImGuiContext& g = *GImGui;
    if (!g.LogEnabled)
        return;

    va_list args;
    va_start(args, fmt);
    if (g.LogFile)
        vfprintf(g.LogFile, fmt, args);
    else
        g.LogClipboard.appendfv(fmt, args);
    va_end(args);
}



void ImGui::LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (!text_end)
        text_end = FindRenderedTextEnd(text, text_end);

    const bool log_new_line = ref_pos && (ref_pos->y > window->DC.LogLinePosY + 1);
    if (ref_pos)
        window->DC.LogLinePosY = ref_pos->y;

    const char* text_remaining = text;
    if (g.LogStartDepth > window->DC.TreeDepth)  
        g.LogStartDepth = window->DC.TreeDepth;
    const int tree_depth = (window->DC.TreeDepth - g.LogStartDepth);
    for (;;)
    {
        
        const char* line_end = text_remaining;
        while (line_end < text_end)
            if (*line_end == '\n')
                break;
            else
                line_end++;
        if (line_end >= text_end)
            line_end = NULL;

        const bool is_first_line = (text == text_remaining);
        bool is_last_line = false;
        if (line_end == NULL)
        {
            is_last_line = true;
            line_end = text_end;
        }
        if (line_end != NULL && !(is_last_line && (line_end - text_remaining)==0))
        {
            const int char_count = (int)(line_end - text_remaining);
            if (log_new_line || !is_first_line)
                LogText(IM_NEWLINE "%*s%.*s", tree_depth*4, "", char_count, text_remaining);
            else
                LogText(" %.*s", char_count, text_remaining);
        }

        if (is_last_line)
            break;
        text_remaining = line_end + 1;
    }
}


void ImGui::LogToTTY(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.LogFile == NULL);
    g.LogFile = stdout;
    g.LogEnabled = true;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}


void ImGui::LogToFile(int max_depth, const char* filename)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    if (!filename)
    {
        filename = g.IO.LogFilename;
        if (!filename)
            return;
    }

    IM_ASSERT(g.LogFile == NULL);
    g.LogFile = ImFileOpen(filename, "ab");
    if (!g.LogFile)
    {
        IM_ASSERT(g.LogFile != NULL); 
        return;
    }
    g.LogEnabled = true;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}


void ImGui::LogToClipboard(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.LogFile == NULL);
    g.LogFile = NULL;
    g.LogEnabled = true;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}

void ImGui::LogFinish()
{
    ImGuiContext& g = *GImGui;
    if (!g.LogEnabled)
        return;

    LogText(IM_NEWLINE);
    if (g.LogFile != NULL)
    {
        if (g.LogFile == stdout)
            fflush(g.LogFile);
        else
            fclose(g.LogFile);
        g.LogFile = NULL;
    }
    if (g.LogClipboard.size() > 1)
    {
        SetClipboardText(g.LogClipboard.begin());
        g.LogClipboard.clear();
    }
    g.LogEnabled = false;
}


void ImGui::LogButtons()
{
    ImGuiContext& g = *GImGui;

    PushID("LogButtons");
    const bool log_to_tty = Button("Log To TTY"); SameLine();
    const bool log_to_file = Button("Log To File"); SameLine();
    const bool log_to_clipboard = Button("Log To Clipboard"); SameLine();
    PushItemWidth(80.0f);
    PushAllowKeyboardFocus(false);
    SliderInt("Depth", &g.LogAutoExpandMaxDepth, 0, 9, NULL);
    PopAllowKeyboardFocus();
    PopItemWidth();
    PopID();

    
    if (log_to_tty)
        LogToTTY(g.LogAutoExpandMaxDepth);
    if (log_to_file)
        LogToFile(g.LogAutoExpandMaxDepth, g.IO.LogFilename);
    if (log_to_clipboard)
        LogToClipboard(g.LogAutoExpandMaxDepth);
}





void ImGui::MarkIniSettingsDirty()
{
    ImGuiContext& g = *GImGui;
    if (g.SettingsDirtyTimer <= 0.0f)
        g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

void ImGui::MarkIniSettingsDirty(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    if (!(window->Flags & ImGuiWindowFlags_NoSavedSettings))
        if (g.SettingsDirtyTimer <= 0.0f)
            g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

static ImGuiWindowSettings* CreateNewWindowSettings(const char* name)
{
    ImGuiContext& g = *GImGui;
    g.SettingsWindows.push_back(ImGuiWindowSettings());
    ImGuiWindowSettings* settings = &g.SettingsWindows.back();
    settings->Name = ImStrdup(name);
    settings->ID = ImHash(name, 0);
    return settings;
}

ImGuiWindowSettings* ImGui::FindWindowSettings(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    for (int i = 0; i != g.SettingsWindows.Size; i++)
        if (g.SettingsWindows[i].ID == id)
            return &g.SettingsWindows[i];
    return NULL;
}

void ImGui::LoadIniSettingsFromDisk(const char* ini_filename)
{
    size_t file_data_size = 0;
    char* file_data = (char*)ImFileLoadToMemory(ini_filename, "rb", &file_data_size);
    if (!file_data)
        return;
    LoadIniSettingsFromMemory(file_data, (size_t)file_data_size);
    ImGui::MemFree(file_data);
}

ImGuiSettingsHandler* ImGui::FindSettingsHandler(const char* type_name)
{
    ImGuiContext& g = *GImGui;
    const ImGuiID type_hash = ImHash(type_name, 0, 0);
    for (int handler_n = 0; handler_n < g.SettingsHandlers.Size; handler_n++)
        if (g.SettingsHandlers[handler_n].TypeHash == type_hash)
            return &g.SettingsHandlers[handler_n];
    return NULL;
}


void ImGui::LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);
    IM_ASSERT(g.SettingsLoaded == false && g.FrameCount == 0);

    
    
    if (ini_size == 0)
        ini_size = strlen(ini_data);
    char* buf = (char*)ImGui::MemAlloc(ini_size + 1);
    char* buf_end = buf + ini_size;
    memcpy(buf, ini_data, ini_size);
    buf[ini_size] = 0;

    void* entry_data = NULL;
    ImGuiSettingsHandler* entry_handler = NULL;

    char* line_end = NULL;
    for (char* line = buf; line < buf_end; line = line_end + 1)
    {
        
        while (*line == '\n' || *line == '\r')
            line++;
        line_end = line;
        while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
            line_end++;
        line_end[0] = 0;
        if (line[0] == ';')
            continue;
        if (line[0] == '[' && line_end > line && line_end[-1] == ']')
        {
            
            line_end[-1] = 0;
            const char* name_end = line_end - 1;
            const char* type_start = line + 1;
            char* type_end = (char*)(intptr_t)ImStrchrRange(type_start, name_end, ']');
            const char* name_start = type_end ? ImStrchrRange(type_end + 1, name_end, '[') : NULL;
            if (!type_end || !name_start)
            {
                name_start = type_start; 
                type_start = "Window";
            }
            else
            {
                *type_end = 0; 
                name_start++;  
            }
            entry_handler = FindSettingsHandler(type_start);
            entry_data = entry_handler ? entry_handler->ReadOpenFn(&g, entry_handler, name_start) : NULL;
        }
        else if (entry_handler != NULL && entry_data != NULL)
        {
            
            entry_handler->ReadLineFn(&g, entry_handler, entry_data, line);
        }
    }
    ImGui::MemFree(buf);
    g.SettingsLoaded = true;
}

void ImGui::SaveIniSettingsToDisk(const char* ini_filename)
{
    ImGuiContext& g = *GImGui;
    g.SettingsDirtyTimer = 0.0f;
    if (!ini_filename)
        return;

    size_t ini_data_size = 0;
    const char* ini_data = SaveIniSettingsToMemory(&ini_data_size);
    FILE* f = ImFileOpen(ini_filename, "wt");
    if (!f)
        return;
    fwrite(ini_data, sizeof(char), ini_data_size, f);
    fclose(f);
}


const char* ImGui::SaveIniSettingsToMemory(size_t* out_size)
{
    ImGuiContext& g = *GImGui;
    g.SettingsDirtyTimer = 0.0f;
    g.SettingsIniData.Buf.resize(0);
    g.SettingsIniData.Buf.push_back(0);
    for (int handler_n = 0; handler_n < g.SettingsHandlers.Size; handler_n++)
    {
        ImGuiSettingsHandler* handler = &g.SettingsHandlers[handler_n];
        handler->WriteAllFn(&g, handler, &g.SettingsIniData);
    }
    if (out_size)
        *out_size = (size_t)g.SettingsIniData.size();
    return g.SettingsIniData.c_str();
}

static void* SettingsHandlerWindow_ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name)
{
    ImGuiWindowSettings* settings = ImGui::FindWindowSettings(ImHash(name, 0));
    if (!settings)
        settings = CreateNewWindowSettings(name);
    return (void*)settings;
}

static void SettingsHandlerWindow_ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line)
{
    ImGuiWindowSettings* settings = (ImGuiWindowSettings*)entry;
    float x, y;
    int i;
    if (sscanf(line, "Pos=%f,%f", &x, &y) == 2)         settings->Pos = ImVec2(x, y);
    else if (sscanf(line, "Size=%f,%f", &x, &y) == 2)   settings->Size = ImMax(ImVec2(x, y), GImGui->Style.WindowMinSize);
    else if (sscanf(line, "Collapsed=%d", &i) == 1)     settings->Collapsed = (i != 0);
}

static void SettingsHandlerWindow_WriteAll(ImGuiContext* imgui_ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
    
    
    ImGuiContext& g = *imgui_ctx;
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
            continue;

        ImGuiWindowSettings* settings = (window->SettingsIdx != -1) ? &g.SettingsWindows[window->SettingsIdx] : ImGui::FindWindowSettings(window->ID);
        if (!settings)
        {
            settings = CreateNewWindowSettings(window->Name);
            window->SettingsIdx = g.SettingsWindows.index_from_pointer(settings);
        }
        IM_ASSERT(settings->ID == window->ID);
        settings->Pos = window->Pos;
        settings->Size = window->SizeFull;
        settings->Collapsed = window->Collapsed;
    }

    
    buf->reserve(buf->size() + g.SettingsWindows.Size * 96); 
    for (int i = 0; i != g.SettingsWindows.Size; i++)
    {
        const ImGuiWindowSettings* settings = &g.SettingsWindows[i];
        if (settings->Pos.x == FLT_MAX)
            continue;
        const char* name = settings->Name;
        if (const char* p = strstr(name, "###"))  
            name = p;
        buf->appendf("[%s][%s]\n", handler->TypeName, name);
        buf->appendf("Pos=%d,%d\n", (int)settings->Pos.x, (int)settings->Pos.y);
        buf->appendf("Size=%d,%d\n", (int)settings->Size.x, (int)settings->Size.y);
        buf->appendf("Collapsed=%d\n", settings->Collapsed);
        buf->appendf("\n");
    }
}





#if defined(_WIN32) && !defined(_WINDOWS_) && (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS) || !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS))
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef __MINGW32__
#include <Windows.h>
#else
#include <windows.h>
#endif
#endif


#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#endif

static const char* GetClipboardTextFn_DefaultImpl(void*)
{
    static ImVector<char> buf_local;
    buf_local.clear();
    if (!::OpenClipboard(NULL))
        return NULL;
    HANDLE wbuf_handle = ::GetClipboardData(CF_UNICODETEXT);
    if (wbuf_handle == NULL)
    {
        ::CloseClipboard();
        return NULL;
    }
    if (ImWchar* wbuf_global = (ImWchar*)::GlobalLock(wbuf_handle))
    {
        int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
        buf_local.resize(buf_len);
        ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
    }
    ::GlobalUnlock(wbuf_handle);
    ::CloseClipboard();
    return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    if (!::OpenClipboard(NULL))
        return;
    const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
    HGLOBAL wbuf_handle = ::GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
    if (wbuf_handle == NULL)
    {
        ::CloseClipboard();
        return;
    }
    ImWchar* wbuf_global = (ImWchar*)::GlobalLock(wbuf_handle);
    ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
    ::GlobalUnlock(wbuf_handle);
    ::EmptyClipboard();
    if (::SetClipboardData(CF_UNICODETEXT, wbuf_handle) == NULL)
        ::GlobalFree(wbuf_handle);
    ::CloseClipboard();
}

#else


static const char* GetClipboardTextFn_DefaultImpl(void*)
{
    ImGuiContext& g = *GImGui;
    return g.PrivateClipboard.empty() ? NULL : g.PrivateClipboard.begin();
}


static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    ImGuiContext& g = *GImGui;
    g.PrivateClipboard.clear();
    const char* text_end = text + strlen(text);
    g.PrivateClipboard.resize((int)(text_end - text) + 1);
    memcpy(&g.PrivateClipboard[0], text, (size_t)(text_end - text));
    g.PrivateClipboard[(int)(text_end - text)] = 0;
}

#endif


#if defined(_WIN32) && !defined(__GNUC__) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y)
{
    
    if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
        if (HIMC himc = ::ImmGetContext(hwnd))
        {
            COMPOSITIONFORM cf;
            cf.ptCurrentPos.x = x;
            cf.ptCurrentPos.y = y;
            cf.dwStyle = CFS_FORCE_POSITION;
            ::ImmSetCompositionWindow(himc, &cf);
            ::ImmReleaseContext(hwnd, himc);
        }
}

#else

static void ImeSetInputScreenPosFn_DefaultImpl(int, int) {}

#endif





void ImGui::ShowMetricsWindow(bool* p_open)
{
    if (!ImGui::Begin("ImGui Metrics", p_open))
    {
        ImGui::End();
        return;
    }
    static bool show_draw_cmd_clip_rects = true;
    static bool show_window_begin_order = false;
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
    ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
    ImGui::Text("%d allocations", io.MetricsActiveAllocations);
    ImGui::Checkbox("Show clipping rectangles when hovering draw commands", &show_draw_cmd_clip_rects);
    ImGui::Checkbox("Ctrl shows window begin order", &show_window_begin_order);

    ImGui::Separator();

    struct Funcs
    {
        static void NodeDrawList(ImGuiWindow* window, ImDrawList* draw_list, const char* label)
        {
            bool node_open = ImGui::TreeNode(draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label, draw_list->_OwnerName ? draw_list->_OwnerName : "", draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size, draw_list->CmdBuffer.Size);
            if (draw_list == ImGui::GetWindowDrawList())
            {
                ImGui::SameLine();
                ImGui::TextColored(ImColor(255,100,100), "CURRENTLY APPENDING"); 
                if (node_open) ImGui::TreePop();
                return;
            }

            ImDrawList* overlay_draw_list = GetOverlayDrawList(); 
            if (window && IsItemHovered())
                overlay_draw_list->AddRect(window->Pos, window->Pos + window->Size, IM_COL32(255, 255, 0, 255));
            if (!node_open)
                return;

            int elem_offset = 0;
            for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.begin(); pcmd < draw_list->CmdBuffer.end(); elem_offset += pcmd->ElemCount, pcmd++)
            {
                if (pcmd->UserCallback == NULL && pcmd->ElemCount == 0)
                    continue;
                if (pcmd->UserCallback)
                {
                    ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
                    continue;
                }
                ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
                bool pcmd_node_open = ImGui::TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()), "Draw %4d %s vtx, tex 0x%p, clip_rect (%4.0f,%4.0f)-(%4.0f,%4.0f)", pcmd->ElemCount, draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed", pcmd->TextureId, pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);
                if (show_draw_cmd_clip_rects && ImGui::IsItemHovered())
                {
                    ImRect clip_rect = pcmd->ClipRect;
                    ImRect vtxs_rect;
                    for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount; i++)
                        vtxs_rect.Add(draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
                    clip_rect.Floor(); overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max, IM_COL32(255,255,0,255));
                    vtxs_rect.Floor(); overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max, IM_COL32(255,0,255,255));
                }
                if (!pcmd_node_open)
                    continue;

                
                ImGuiListClipper clipper(pcmd->ElemCount/3); 
                while (clipper.Step())
                    for (int prim = clipper.DisplayStart, vtx_i = elem_offset + clipper.DisplayStart*3; prim < clipper.DisplayEnd; prim++)
                    {
                        char buf[300];
                        char *buf_p = buf, *buf_end = buf + IM_ARRAYSIZE(buf);
                        ImVec2 triangles_pos[3];
                        for (int n = 0; n < 3; n++, vtx_i++)
                        {
                            ImDrawVert& v = draw_list->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
                            triangles_pos[n] = v.pos;
                            buf_p += ImFormatString(buf_p, (int)(buf_end - buf_p), "%s %04d: pos (%8.2f,%8.2f), uv (%.6f,%.6f), col %08X\n", (n == 0) ? "vtx" : "   ", vtx_i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
                        }
                        ImGui::Selectable(buf, false);
                        if (ImGui::IsItemHovered())
                        {
                            ImDrawListFlags backup_flags = overlay_draw_list->Flags;
                            overlay_draw_list->Flags &= ~ImDrawListFlags_AntiAliasedLines; 
                            overlay_draw_list->AddPolyline(triangles_pos, 3, IM_COL32(255,255,0,255), true, 1.0f);
                            overlay_draw_list->Flags = backup_flags;
                        }
                    }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        static void NodeWindows(ImVector<ImGuiWindow*>& windows, const char* label)
        {
            if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size))
                return;
            for (int i = 0; i < windows.Size; i++)
                Funcs::NodeWindow(windows[i], "Window");
            ImGui::TreePop();
        }

        static void NodeWindow(ImGuiWindow* window, const char* label)
        {
            if (!ImGui::TreeNode(window, "%s '%s', %d @ 0x%p", label, window->Name, window->Active || window->WasActive, window))
                return;
            ImGuiWindowFlags flags = window->Flags;
            NodeDrawList(window, window->DrawList, "DrawList");
            ImGui::BulletText("Pos: (%.1f,%.1f), Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)", window->Pos.x, window->Pos.y, window->Size.x, window->Size.y, window->SizeContents.x, window->SizeContents.y);
            ImGui::BulletText("Flags: 0x%08X (%s%s%s%s%s%s%s%s..)", flags,
                (flags & ImGuiWindowFlags_ChildWindow) ? "Child " : "", (flags & ImGuiWindowFlags_Tooltip)   ? "Tooltip "   : "", (flags & ImGuiWindowFlags_Popup) ? "Popup " : "",
                (flags & ImGuiWindowFlags_Modal)       ? "Modal " : "", (flags & ImGuiWindowFlags_ChildMenu) ? "ChildMenu " : "", (flags & ImGuiWindowFlags_NoSavedSettings) ? "NoSavedSettings " : "",
                (flags & ImGuiWindowFlags_NoInputs)    ? "NoInputs":"", (flags & ImGuiWindowFlags_AlwaysAutoResize) ? "AlwaysAutoResize" : "");
            ImGui::BulletText("Scroll: (%.2f/%.2f,%.2f/%.2f)", window->Scroll.x, GetScrollMaxX(window), window->Scroll.y, GetScrollMaxY(window));
            ImGui::BulletText("Active: %d/%d, WriteAccessed: %d, BeginOrderWithinContext: %d", window->Active, window->WasActive, window->WriteAccessed, (window->Active || window->WasActive) ? window->BeginOrderWithinContext : -1);
            ImGui::BulletText("Appearing: %d, Hidden: %d (Reg %d Resize %d), SkipItems: %d", window->Appearing, window->Hidden, window->HiddenFramesRegular, window->HiddenFramesForResize, window->SkipItems);
            ImGui::BulletText("NavLastIds: 0x%08X,0x%08X, NavLayerActiveMask: %X", window->NavLastIds[0], window->NavLastIds[1], window->DC.NavLayerActiveMask);
            ImGui::BulletText("NavLastChildNavWindow: %s", window->NavLastChildNavWindow ? window->NavLastChildNavWindow->Name : "NULL");
            if (!window->NavRectRel[0].IsInverted())
                ImGui::BulletText("NavRectRel[0]: (%.1f,%.1f)(%.1f,%.1f)", window->NavRectRel[0].Min.x, window->NavRectRel[0].Min.y, window->NavRectRel[0].Max.x, window->NavRectRel[0].Max.y);
            else
                ImGui::BulletText("NavRectRel[0]: <None>");
            if (window->RootWindow != window) NodeWindow(window->RootWindow, "RootWindow");
            if (window->ParentWindow != NULL) NodeWindow(window->ParentWindow, "ParentWindow");
            if (window->DC.ChildWindows.Size > 0) NodeWindows(window->DC.ChildWindows, "ChildWindows");
            if (window->ColumnsStorage.Size > 0 && ImGui::TreeNode("Columns", "Columns sets (%d)", window->ColumnsStorage.Size))
            {
                for (int n = 0; n < window->ColumnsStorage.Size; n++)
                {
                    const ImGuiColumnsSet* columns = &window->ColumnsStorage[n];
                    if (ImGui::TreeNode((void*)(uintptr_t)columns->ID, "Columns Id: 0x%08X, Count: %d, Flags: 0x%04X", columns->ID, columns->Count, columns->Flags))
                    {
                        ImGui::BulletText("Width: %.1f (MinX: %.1f, MaxX: %.1f)", columns->MaxX - columns->MinX, columns->MinX, columns->MaxX);
                        for (int column_n = 0; column_n < columns->Columns.Size; column_n++)
                            ImGui::BulletText("Column %02d: OffsetNorm %.3f (= %.1f px)", column_n, columns->Columns[column_n].OffsetNorm, OffsetNormToPixels(columns, columns->Columns[column_n].OffsetNorm));
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::BulletText("Storage: %d bytes", window->StateStorage.Data.Size * (int)sizeof(ImGuiStorage::Pair));
            ImGui::TreePop();
        }
    };

    
    ImGuiContext& g = *GImGui;
    Funcs::NodeWindows(g.Windows, "Windows");
    if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)", g.DrawDataBuilder.Layers[0].Size))
    {
        for (int i = 0; i < g.DrawDataBuilder.Layers[0].Size; i++)
            Funcs::NodeDrawList(NULL, g.DrawDataBuilder.Layers[0][i], "DrawList");
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Popups", "Popups (%d)", g.OpenPopupStack.Size))
    {
        for (int i = 0; i < g.OpenPopupStack.Size; i++)
        {
            ImGuiWindow* window = g.OpenPopupStack[i].Window;
            ImGui::BulletText("PopupID: %08x, Window: '%s'%s%s", g.OpenPopupStack[i].PopupId, window ? window->Name : "NULL", window && (window->Flags & ImGuiWindowFlags_ChildWindow) ? " ChildWindow" : "", window && (window->Flags & ImGuiWindowFlags_ChildMenu) ? " ChildMenu" : "");
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Internal state"))
    {
        const char* input_source_names[] = { "None", "Mouse", "Nav", "NavKeyboard", "NavGamepad" }; IM_ASSERT(IM_ARRAYSIZE(input_source_names) == ImGuiInputSource_COUNT);
        ImGui::Text("HoveredWindow: '%s'", g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
        ImGui::Text("HoveredRootWindow: '%s'", g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
        ImGui::Text("HoveredId: 0x%08X/0x%08X (%.2f sec), AllowOverlap: %d", g.HoveredId, g.HoveredIdPreviousFrame, g.HoveredIdTimer, g.HoveredIdAllowOverlap); 
        ImGui::Text("ActiveId: 0x%08X/0x%08X (%.2f sec), AllowOverlap: %d, Source: %s", g.ActiveId, g.ActiveIdPreviousFrame, g.ActiveIdTimer, g.ActiveIdAllowOverlap, input_source_names[g.ActiveIdSource]);
        ImGui::Text("ActiveIdWindow: '%s'", g.ActiveIdWindow ? g.ActiveIdWindow->Name : "NULL");
        ImGui::Text("MovingWindow: '%s'", g.MovingWindow ? g.MovingWindow->Name : "NULL");
        ImGui::Text("NavWindow: '%s'", g.NavWindow ? g.NavWindow->Name : "NULL");
        ImGui::Text("NavId: 0x%08X, NavLayer: %d", g.NavId, g.NavLayer);
        ImGui::Text("NavInputSource: %s", input_source_names[g.NavInputSource]);
        ImGui::Text("NavActive: %d, NavVisible: %d", g.IO.NavActive, g.IO.NavVisible);
        ImGui::Text("NavActivateId: 0x%08X, NavInputId: 0x%08X", g.NavActivateId, g.NavInputId);
        ImGui::Text("NavDisableHighlight: %d, NavDisableMouseHover: %d", g.NavDisableHighlight, g.NavDisableMouseHover);
        ImGui::Text("NavWindowingTarget: '%s'", g.NavWindowingTarget ? g.NavWindowingTarget->Name : "NULL");
        ImGui::Text("DragDrop: %d, SourceId = 0x%08X, Payload \"%s\" (%d bytes)", g.DragDropActive, g.DragDropPayload.SourceId, g.DragDropPayload.DataType, g.DragDropPayload.DataSize);
        ImGui::TreePop();
    }


    if (g.IO.KeyCtrl && show_window_begin_order)
    {
        for (int n = 0; n < g.Windows.Size; n++)
        {
            ImGuiWindow* window = g.Windows[n];
            if ((window->Flags & ImGuiWindowFlags_ChildWindow) || !window->WasActive)
                continue;
            char buf[32];
            ImFormatString(buf, IM_ARRAYSIZE(buf), "%d", window->BeginOrderWithinContext);
            float font_size = ImGui::GetFontSize() * 2;
            ImDrawList* overlay_draw_list = GetOverlayDrawList();
            overlay_draw_list->AddRectFilled(window->Pos, window->Pos + ImVec2(font_size, font_size), IM_COL32(200, 100, 100, 255));
            overlay_draw_list->AddText(NULL, font_size, window->Pos, IM_COL32(255, 255, 255, 255), buf);
        }
    }
    ImGui::End();
}





#ifdef IMGUI_INCLUDE_IMGUI_USER_INL
#include "imgui_user.inl"
#endif


