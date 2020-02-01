







#pragma once


#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h"
#endif

#include <float.h>                  
#include <stdarg.h>                 
#include <stddef.h>                 
#include <string.h>                 



#define IMGUI_VERSION               "1.65"
#define IMGUI_VERSION_NUM           16501
#define IMGUI_CHECKVERSION()        ImGui::DebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert))



#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API              IMGUI_API
#endif


#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)                               
#endif
#if defined(__clang__) || defined(__GNUC__)
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1))) 
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))         
#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))           

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif


struct ImDrawChannel;               
struct ImDrawCmd;                   
struct ImDrawData;                  
struct ImDrawList;                  
struct ImDrawListSharedData;        
struct ImDrawVert;                  
struct ImFont;                      
struct ImFontAtlas;                 
struct ImFontConfig;                
struct ImColor;                     
#ifndef ImTextureID
typedef void* ImTextureID;          
#endif
struct ImGuiContext;                
struct ImGuiIO;                     
struct ImGuiInputTextCallbackData;  
struct ImGuiListClipper;            
struct ImGuiOnceUponAFrame;         
struct ImGuiPayload;                
struct ImGuiSizeCallbackData;       
struct ImGuiStorage;                
struct ImGuiStyle;                  
struct ImGuiTextFilter;             
struct ImGuiTextBuffer;             



typedef unsigned int ImGuiID;       
typedef unsigned short ImWchar;     
typedef int ImGuiCol;               
typedef int ImGuiCond;              
typedef int ImGuiDataType;          
typedef int ImGuiDir;               
typedef int ImGuiKey;               
typedef int ImGuiNavInput;          
typedef int ImGuiMouseCursor;       
typedef int ImGuiStyleVar;          
typedef int ImDrawCornerFlags;      
typedef int ImDrawListFlags;        
typedef int ImFontAtlasFlags;       
typedef int ImGuiBackendFlags;      
typedef int ImGuiColorEditFlags;    
typedef int ImGuiColumnsFlags;      
typedef int ImGuiConfigFlags;       
typedef int ImGuiComboFlags;        
typedef int ImGuiDragDropFlags;     
typedef int ImGuiFocusedFlags;      
typedef int ImGuiHoveredFlags;      
typedef int ImGuiInputTextFlags;    
typedef int ImGuiSelectableFlags;   
typedef int ImGuiTreeNodeFlags;     
typedef int ImGuiWindowFlags;       
typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData *data);
typedef void (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);


typedef signed int          ImS32;  
typedef unsigned int        ImU32;  
#if defined(_MSC_VER) && !defined(__clang__)
typedef signed   __int64    ImS64;  
typedef unsigned __int64    ImU64;  
#elif (defined(__clang__) || defined(__GNUC__)) && (__cplusplus < 201100)
#include <stdint.h>
typedef int64_t             ImS64;  
typedef uint64_t            ImU64;  
#else
typedef signed   long long  ImS64;  
typedef unsigned long long  ImU64;  
#endif


struct ImVec2
{
    float     x, y;
    ImVec2()  { x = y = 0.0f; }
    ImVec2(float _x, float _y) { x = _x; y = _y; }
    float operator[] (size_t i) const { IM_ASSERT(i <= 1); return (&x)[i]; }    
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA     
#endif
};


struct ImVec4
{
    float     x, y, z, w;
    ImVec4()  { x = y = z = w = 0.0f; }
    ImVec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA     
#endif
};



namespace ImGui
{
    
    
    
    IMGUI_API ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL);
    IMGUI_API void          DestroyContext(ImGuiContext* ctx = NULL);   
    IMGUI_API ImGuiContext* GetCurrentContext();
    IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);
    IMGUI_API bool          DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert);

    
    IMGUI_API ImGuiIO&      GetIO();                                    
    IMGUI_API ImGuiStyle&   GetStyle();                                 
    IMGUI_API void          NewFrame();                                 
    IMGUI_API void          EndFrame();                                 
    IMGUI_API void          Render();                                   
    IMGUI_API ImDrawData*   GetDrawData();                              

    
    IMGUI_API void          ShowDemoWindow(bool* p_open = NULL);        
    IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);     
    IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);    
    IMGUI_API bool          ShowStyleSelector(const char* label);       
    IMGUI_API void          ShowFontSelector(const char* label);        
    IMGUI_API void          ShowUserGuide();                            
    IMGUI_API const char*   GetVersion();                               

    
    IMGUI_API void          StyleColorsDark(ImGuiStyle* dst = NULL);    
    IMGUI_API void          StyleColorsClassic(ImGuiStyle* dst = NULL); 
    IMGUI_API void          StyleColorsLight(ImGuiStyle* dst = NULL);   

    
    
    
    
    
    
    IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
    IMGUI_API void          End();
    IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags = 0); 
    IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API void          EndChild();

    
    IMGUI_API bool          IsWindowAppearing();
    IMGUI_API bool          IsWindowCollapsed();
    IMGUI_API bool          IsWindowFocused(ImGuiFocusedFlags flags=0); 
    IMGUI_API bool          IsWindowHovered(ImGuiHoveredFlags flags=0); 
    IMGUI_API ImDrawList*   GetWindowDrawList();                        
    IMGUI_API ImVec2        GetWindowPos();                             
    IMGUI_API ImVec2        GetWindowSize();                            
    IMGUI_API float         GetWindowWidth();                           
    IMGUI_API float         GetWindowHeight();                          
    IMGUI_API ImVec2        GetContentRegionMax();                      
    IMGUI_API ImVec2        GetContentRegionAvail();                    
    IMGUI_API float         GetContentRegionAvailWidth();               
    IMGUI_API ImVec2        GetWindowContentRegionMin();                
    IMGUI_API ImVec2        GetWindowContentRegionMax();                
    IMGUI_API float         GetWindowContentRegionWidth();              

    IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0,0)); 
    IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);                  
    IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); 
    IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);                               
    IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                 
    IMGUI_API void          SetNextWindowFocus();                                                       
    IMGUI_API void          SetNextWindowBgAlpha(float alpha);                                          
    IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);                        
    IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);                      
    IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                     
    IMGUI_API void          SetWindowFocus();                                                           
    IMGUI_API void          SetWindowFontScale(float scale);                                            
    IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);      
    IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);    
    IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);   
    IMGUI_API void          SetWindowFocus(const char* name);                                           

    
    IMGUI_API float         GetScrollX();                                                   
    IMGUI_API float         GetScrollY();                                                   
    IMGUI_API float         GetScrollMaxX();                                                
    IMGUI_API float         GetScrollMaxY();                                                
    IMGUI_API void          SetScrollX(float scroll_x);                                     
    IMGUI_API void          SetScrollY(float scroll_y);                                     
    IMGUI_API void          SetScrollHere(float center_y_ratio = 0.5f);                     
    IMGUI_API void          SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f);    

    
    IMGUI_API void          PushFont(ImFont* font);                                         
    IMGUI_API void          PopFont();
    IMGUI_API void          PushStyleColor(ImGuiCol idx, ImU32 col);
    IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
    IMGUI_API void          PopStyleColor(int count = 1);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
    IMGUI_API void          PopStyleVar(int count = 1);
    IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);                                
    IMGUI_API ImFont*       GetFont();                                                      
    IMGUI_API float         GetFontSize();                                                  
    IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                       
    IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);              
    IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                 
    IMGUI_API ImU32         GetColorU32(ImU32 col);                                         

    
    IMGUI_API void          PushItemWidth(float item_width);                                
    IMGUI_API void          PopItemWidth();
    IMGUI_API float         CalcItemWidth();                                                
    IMGUI_API void          PushTextWrapPos(float wrap_pos_x = 0.0f);                       
    IMGUI_API void          PopTextWrapPos();
    IMGUI_API void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              
    IMGUI_API void          PopAllowKeyboardFocus();
    IMGUI_API void          PushButtonRepeat(bool repeat);                                  
    IMGUI_API void          PopButtonRepeat();

    
    IMGUI_API void          Separator();                                                    
    IMGUI_API void          SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);          
    IMGUI_API void          NewLine();                                                      
    IMGUI_API void          Spacing();                                                      
    IMGUI_API void          Dummy(const ImVec2& size);                                      
    IMGUI_API void          Indent(float indent_w = 0.0f);                                  
    IMGUI_API void          Unindent(float indent_w = 0.0f);                                
    IMGUI_API void          BeginGroup();                                                   
    IMGUI_API void          EndGroup();
    IMGUI_API ImVec2        GetCursorPos();                                                 
    IMGUI_API float         GetCursorPosX();                                                
    IMGUI_API float         GetCursorPosY();                                                
    IMGUI_API void          SetCursorPos(const ImVec2& local_pos);                          
    IMGUI_API void          SetCursorPosX(float x);                                         
    IMGUI_API void          SetCursorPosY(float y);                                         
    IMGUI_API ImVec2        GetCursorStartPos();                                            
    IMGUI_API ImVec2        GetCursorScreenPos();                                           
    IMGUI_API void          SetCursorScreenPos(const ImVec2& screen_pos);                   
    IMGUI_API void          AlignTextToFramePadding();                                      
    IMGUI_API float         GetTextLineHeight();                                            
    IMGUI_API float         GetTextLineHeightWithSpacing();                                 
    IMGUI_API float         GetFrameHeight();                                               
    IMGUI_API float         GetFrameHeightWithSpacing();                                    

    
    
    
    
    
    
    IMGUI_API void          PushID(const char* str_id);                                     
    IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);
    IMGUI_API void          PushID(const void* ptr_id);
    IMGUI_API void          PushID(int int_id);
    IMGUI_API void          PopID();
    IMGUI_API ImGuiID       GetID(const char* str_id);                                      
    IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
    IMGUI_API ImGuiID       GetID(const void* ptr_id);

    
    IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL);                
    IMGUI_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); 
    IMGUI_API void          TextV(const char* fmt, va_list args)                            IM_FMTLIST(1);
    IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); 
    IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
    IMGUI_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); 
    IMGUI_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
    IMGUI_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); 
    IMGUI_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
    IMGUI_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); 
    IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
    IMGUI_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); 
    IMGUI_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);

    
    
    IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec2(0,0));    
    IMGUI_API bool          SmallButton(const char* label);                                 
    IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& size);        
    IMGUI_API bool          ArrowButton(const char* str_id, ImGuiDir dir);                  
    IMGUI_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), const ImVec4& tint_col = ImVec4(1,1,1,1), const ImVec4& border_col = ImVec4(0,0,0,0));
    IMGUI_API bool          ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0),  const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1));    
    IMGUI_API bool          Checkbox(const char* label, bool* v);
    IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    IMGUI_API bool          RadioButton(const char* label, bool active);                    
    IMGUI_API bool          RadioButton(const char* label, int* v, int v_button);           
    IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1,0), const char* overlay = NULL);
    IMGUI_API void          Bullet();                                                       

    
    
    
    IMGUI_API bool          BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API void          EndCombo(); 
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      
    IMGUI_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    
    
    
    
    IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);     
    IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, float power = 1.0f);
    IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");                                       
    IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL);
    IMGUI_API bool          DragScalar(const char* label, ImGuiDataType data_type, void* v, float v_speed, const void* v_min = NULL, const void* v_max = NULL, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          DragScalarN(const char* label, ImGuiDataType data_type, void* v, int components, float v_speed, const void* v_min = NULL, const void* v_max = NULL, const char* format = NULL, float power = 1.0f);

    
    
    IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);     
    IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f);
    IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderScalar(const char* label, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          SliderScalarN(const char* label, ImGuiDataType data_type, void* v, int components, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);

    
    
    IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0,0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputDouble(const char* label, double* v, double step = 0.0f, double step_fast = 0.0f, const char* format = "%.6f", ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputScalar(const char* label, ImGuiDataType data_type, void* v, const void* step = NULL, const void* step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool          InputScalarN(const char* label, ImGuiDataType data_type, void* v, int components, const void* step = NULL, const void* step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags extra_flags = 0);

    
    
    IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0));  
    IMGUI_API void          SetColorEditOptions(ImGuiColorEditFlags flags);                     

    
    
    IMGUI_API bool          TreeNode(const char* label);
    IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   
    IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   
    IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
    IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API void          TreePush(const char* str_id);                                       
    IMGUI_API void          TreePush(const void* ptr_id = NULL);                                
    IMGUI_API void          TreePop();                                                          
    IMGUI_API void          TreeAdvanceToLabelPos();                                            
    IMGUI_API float         GetTreeNodeToLabelSpacing();                                        
    IMGUI_API void          SetNextTreeNodeOpen(bool is_open, ImGuiCond cond = 0);              
    IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);  
    IMGUI_API bool          CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0); 

    
    IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));  
    IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));       

    
    IMGUI_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0,0)); 
    IMGUI_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1); 
    IMGUI_API void          ListBoxFooter();                                                    

    
    IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
    IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

    
    IMGUI_API void          Value(const char* prefix, bool b);
    IMGUI_API void          Value(const char* prefix, int v);
    IMGUI_API void          Value(const char* prefix, unsigned int v);
    IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);

    
    IMGUI_API bool          BeginMainMenuBar();                                                 
    IMGUI_API void          EndMainMenuBar();                                                   
    IMGUI_API bool          BeginMenuBar();                                                     
    IMGUI_API void          EndMenuBar();                                                       
    IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                  
    IMGUI_API void          EndMenu();                                                          
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              

    
    IMGUI_API void          BeginTooltip();                                                     
    IMGUI_API void          EndTooltip();
    IMGUI_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     
    IMGUI_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

    
    IMGUI_API void          OpenPopup(const char* str_id);                                      
    IMGUI_API bool          BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);                                             
    IMGUI_API bool          BeginPopupContextItem(const char* str_id = NULL, int mouse_button = 1);                                 
    IMGUI_API bool          BeginPopupContextWindow(const char* str_id = NULL, int mouse_button = 1, bool also_over_items = true);  
    IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, int mouse_button = 1);                                 
    IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);                     
    IMGUI_API void          EndPopup();                                                                                             
    IMGUI_API bool          OpenPopupOnItemClick(const char* str_id = NULL, int mouse_button = 1);                                  
    IMGUI_API bool          IsPopupOpen(const char* str_id);                                    
    IMGUI_API void          CloseCurrentPopup();                                                

    
    
    IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool border = true);
    IMGUI_API void          NextColumn();                                                       
    IMGUI_API int           GetColumnIndex();                                                   
    IMGUI_API float         GetColumnWidth(int column_index = -1);                              
    IMGUI_API void          SetColumnWidth(int column_index, float width);                      
    IMGUI_API float         GetColumnOffset(int column_index = -1);                             
    IMGUI_API void          SetColumnOffset(int column_index, float offset_x);                  
    IMGUI_API int           GetColumnsCount();

    
    IMGUI_API void          LogToTTY(int max_depth = -1);                                       
    IMGUI_API void          LogToFile(int max_depth = -1, const char* filename = NULL);         
    IMGUI_API void          LogToClipboard(int max_depth = -1);                                 
    IMGUI_API void          LogFinish();                                                        
    IMGUI_API void          LogButtons();                                                       
    IMGUI_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        

    
    
    IMGUI_API bool          BeginDragDropSource(ImGuiDragDropFlags flags = 0);                                      
    IMGUI_API bool          SetDragDropPayload(const char* type, const void* data, size_t size, ImGuiCond cond = 0);
    IMGUI_API void          EndDragDropSource();                                                                    
    IMGUI_API bool          BeginDragDropTarget();                                                                  
    IMGUI_API const ImGuiPayload* AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);            
    IMGUI_API void          EndDragDropTarget();                                                                    

    
    IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    IMGUI_API void          PopClipRect();

    
    
    IMGUI_API void          SetItemDefaultFocus();                                              
    IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                               

    
    
    IMGUI_API bool          IsItemHovered(ImGuiHoveredFlags flags = 0);                         
    IMGUI_API bool          IsItemActive();                                                     
    IMGUI_API bool          IsItemFocused();                                                    
    IMGUI_API bool          IsItemClicked(int mouse_button = 0);                                
    IMGUI_API bool          IsItemVisible();                                                    
    IMGUI_API bool          IsItemEdited();                                                     
    IMGUI_API bool          IsItemDeactivated();                                                
    IMGUI_API bool          IsItemDeactivatedAfterEdit();                                       
    IMGUI_API bool          IsAnyItemHovered();
    IMGUI_API bool          IsAnyItemActive();
    IMGUI_API bool          IsAnyItemFocused();
    IMGUI_API ImVec2        GetItemRectMin();                                                   
    IMGUI_API ImVec2        GetItemRectMax();                                                   
    IMGUI_API ImVec2        GetItemRectSize();                                                  
    IMGUI_API void          SetItemAllowOverlap();                                              
    IMGUI_API bool          IsRectVisible(const ImVec2& size);                                  
    IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      
    IMGUI_API double        GetTime();
    IMGUI_API int           GetFrameCount();
    IMGUI_API ImDrawList*   GetOverlayDrawList();                                               
    IMGUI_API ImDrawListSharedData* GetDrawListSharedData();                                    
    IMGUI_API const char*   GetStyleColorName(ImGuiCol idx);
    IMGUI_API void          SetStateStorage(ImGuiStorage* storage);                             
    IMGUI_API ImGuiStorage* GetStateStorage();
    IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
    IMGUI_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);    

    IMGUI_API bool          BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags flags = 0); 
    IMGUI_API void          EndChildFrame();                                                    

    IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
    IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
    IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    
    IMGUI_API int           GetKeyIndex(ImGuiKey imgui_key);                                    
    IMGUI_API bool          IsKeyDown(int user_key_index);                                      
    IMGUI_API bool          IsKeyPressed(int user_key_index, bool repeat = true);               
    IMGUI_API bool          IsKeyReleased(int user_key_index);                                  
    IMGUI_API int           GetKeyPressedAmount(int key_index, float repeat_delay, float rate); 
    IMGUI_API bool          IsMouseDown(int button);                                            
    IMGUI_API bool          IsAnyMouseDown();                                                   
    IMGUI_API bool          IsMouseClicked(int button, bool repeat = false);                    
    IMGUI_API bool          IsMouseDoubleClicked(int button);                                   
    IMGUI_API bool          IsMouseReleased(int button);                                        
    IMGUI_API bool          IsMouseDragging(int button = 0, float lock_threshold = -1.0f);      
    IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);  
    IMGUI_API bool          IsMousePosValid(const ImVec2* mouse_pos = NULL);                    
    IMGUI_API ImVec2        GetMousePos();                                                      
    IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                 
    IMGUI_API ImVec2        GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);    
    IMGUI_API void          ResetMouseDragDelta(int button = 0);                                
    IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                
    IMGUI_API void          SetMouseCursor(ImGuiMouseCursor type);                              
    IMGUI_API void          CaptureKeyboardFromApp(bool capture = true);                        
    IMGUI_API void          CaptureMouseFromApp(bool capture = true);                           

    
    IMGUI_API const char*   GetClipboardText();
    IMGUI_API void          SetClipboardText(const char* text);

    
    
    
    IMGUI_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  
    IMGUI_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size=0); 
    IMGUI_API void          SaveIniSettingsToDisk(const char* ini_filename);
    IMGUI_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               

    
    
    
    IMGUI_API void          SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void(*free_func)(void* ptr, void* user_data), void* user_data = NULL);
    IMGUI_API void*         MemAlloc(size_t size);
    IMGUI_API void          MemFree(void* ptr);

} 


enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None                   = 0,
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   
    ImGuiWindowFlags_NoResize               = 1 << 1,   
    ImGuiWindowFlags_NoMove                 = 1 << 2,   
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   
    ImGuiWindowFlags_NoInputs               = 1 << 9,   
    ImGuiWindowFlags_MenuBar                = 1 << 10,  
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  
    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  
    ImGuiWindowFlags_NoNavInputs            = 1 << 18,  
    ImGuiWindowFlags_NoNavFocus             = 1 << 19,  
    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    
    ImGuiWindowFlags_NavFlattened           = 1 << 23,  
    ImGuiWindowFlags_ChildWindow            = 1 << 24,  
    ImGuiWindowFlags_Tooltip                = 1 << 25,  
    ImGuiWindowFlags_Popup                  = 1 << 26,  
    ImGuiWindowFlags_Modal                  = 1 << 27,  
    ImGuiWindowFlags_ChildMenu              = 1 << 28   

    
    
    
};


enum ImGuiInputTextFlags_
{
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,   
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,   
    ImGuiInputTextFlags_CharsUppercase      = 1 << 2,   
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 3,   
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 4,   
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 5,   
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 6,   
    ImGuiInputTextFlags_CallbackHistory     = 1 << 7,   
    ImGuiInputTextFlags_CallbackAlways      = 1 << 8,   
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 9,   
    ImGuiInputTextFlags_AllowTabInput       = 1 << 10,  
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 12,  
    ImGuiInputTextFlags_AlwaysInsertMode    = 1 << 13,  
    ImGuiInputTextFlags_ReadOnly            = 1 << 14,  
    ImGuiInputTextFlags_Password            = 1 << 15,  
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,  
    ImGuiInputTextFlags_CharsScientific     = 1 << 17,  
    ImGuiInputTextFlags_CallbackResize      = 1 << 18,  
    
    ImGuiInputTextFlags_Multiline           = 1 << 20   
};


enum ImGuiTreeNodeFlags_
{
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,   
    ImGuiTreeNodeFlags_Framed               = 1 << 1,   
    ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,   
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,   
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  
    
    
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  
    ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiTreeNodeFlags_AllowOverlapMode = ImGuiTreeNodeFlags_AllowItemOverlap
#endif
};


enum ImGuiSelectableFlags_
{
    ImGuiSelectableFlags_None               = 0,
    ImGuiSelectableFlags_DontClosePopups    = 1 << 0,   
    ImGuiSelectableFlags_SpanAllColumns     = 1 << 1,   
    ImGuiSelectableFlags_AllowDoubleClick   = 1 << 2,   
    ImGuiSelectableFlags_Disabled           = 1 << 3    
};


enum ImGuiComboFlags_
{
    ImGuiComboFlags_None                    = 0,
    ImGuiComboFlags_PopupAlignLeft          = 1 << 0,   
    ImGuiComboFlags_HeightSmall             = 1 << 1,   
    ImGuiComboFlags_HeightRegular           = 1 << 2,   
    ImGuiComboFlags_HeightLarge             = 1 << 3,   
    ImGuiComboFlags_HeightLargest           = 1 << 4,   
    ImGuiComboFlags_NoArrowButton           = 1 << 5,   
    ImGuiComboFlags_NoPreview               = 1 << 6,   
    ImGuiComboFlags_HeightMask_             = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest
};


enum ImGuiFocusedFlags_
{
    ImGuiFocusedFlags_None                          = 0,
    ImGuiFocusedFlags_ChildWindows                  = 1 << 0,   
    ImGuiFocusedFlags_RootWindow                    = 1 << 1,   
    ImGuiFocusedFlags_AnyWindow                     = 1 << 2,   
    ImGuiFocusedFlags_RootAndChildWindows           = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows
};




enum ImGuiHoveredFlags_
{
    ImGuiHoveredFlags_None                          = 0,        
    ImGuiHoveredFlags_ChildWindows                  = 1 << 0,   
    ImGuiHoveredFlags_RootWindow                    = 1 << 1,   
    ImGuiHoveredFlags_AnyWindow                     = 1 << 2,   
    ImGuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 3,   
    
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 5,   
    ImGuiHoveredFlags_AllowWhenOverlapped           = 1 << 6,   
    ImGuiHoveredFlags_AllowWhenDisabled             = 1 << 7,   
    ImGuiHoveredFlags_RectOnly                      = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows           = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows
};


enum ImGuiDragDropFlags_
{
    ImGuiDragDropFlags_None                         = 0,
    
    ImGuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   
    ImGuiDragDropFlags_SourceNoDisableHover         = 1 << 1,   
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   
    ImGuiDragDropFlags_SourceAllowNullID            = 1 << 3,   
    ImGuiDragDropFlags_SourceExtern                 = 1 << 4,   
    ImGuiDragDropFlags_SourceAutoExpirePayload      = 1 << 5,   
    
    ImGuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  
    ImGuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  
    ImGuiDragDropFlags_AcceptPeekOnly               = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect  
};


#define IMGUI_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    
#define IMGUI_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    


enum ImGuiDataType_
{
    ImGuiDataType_S32,      
    ImGuiDataType_U32,      
    ImGuiDataType_S64,      
    ImGuiDataType_U64,      
    ImGuiDataType_Float,    
    ImGuiDataType_Double,   
    ImGuiDataType_COUNT
};


enum ImGuiDir_
{
    ImGuiDir_None    = -1,
    ImGuiDir_Left    = 0,
    ImGuiDir_Right   = 1,
    ImGuiDir_Up      = 2,
    ImGuiDir_Down    = 3,
    ImGuiDir_COUNT
};


enum ImGuiKey_
{
    ImGuiKey_Tab,
    ImGuiKey_LeftArrow,
    ImGuiKey_RightArrow,
    ImGuiKey_UpArrow,
    ImGuiKey_DownArrow,
    ImGuiKey_PageUp,
    ImGuiKey_PageDown,
    ImGuiKey_Home,
    ImGuiKey_End,
    ImGuiKey_Insert,
    ImGuiKey_Delete,
    ImGuiKey_Backspace,
    ImGuiKey_Space,
    ImGuiKey_Enter,
    ImGuiKey_Escape,
    ImGuiKey_A,         
    ImGuiKey_C,         
    ImGuiKey_V,         
    ImGuiKey_X,         
    ImGuiKey_Y,         
    ImGuiKey_Z,         
    ImGuiKey_COUNT
};





enum ImGuiNavInput_
{
    
    ImGuiNavInput_Activate,      
    ImGuiNavInput_Cancel,        
    ImGuiNavInput_Input,         
    ImGuiNavInput_Menu,          
    ImGuiNavInput_DpadLeft,      
    ImGuiNavInput_DpadRight,     
    ImGuiNavInput_DpadUp,        
    ImGuiNavInput_DpadDown,      
    ImGuiNavInput_LStickLeft,    
    ImGuiNavInput_LStickRight,   
    ImGuiNavInput_LStickUp,      
    ImGuiNavInput_LStickDown,    
    ImGuiNavInput_FocusPrev,     
    ImGuiNavInput_FocusNext,     
    ImGuiNavInput_TweakSlow,     
    ImGuiNavInput_TweakFast,     

    
    
    ImGuiNavInput_KeyMenu_,      
    ImGuiNavInput_KeyLeft_,      
    ImGuiNavInput_KeyRight_,     
    ImGuiNavInput_KeyUp_,        
    ImGuiNavInput_KeyDown_,      
    ImGuiNavInput_COUNT,
    ImGuiNavInput_InternalStart_ = ImGuiNavInput_KeyMenu_
};


enum ImGuiConfigFlags_
{
    ImGuiConfigFlags_NavEnableKeyboard      = 1 << 0,   
    ImGuiConfigFlags_NavEnableGamepad       = 1 << 1,   
    ImGuiConfigFlags_NavEnableSetMousePos   = 1 << 2,   
    ImGuiConfigFlags_NavNoCaptureKeyboard   = 1 << 3,   
    ImGuiConfigFlags_NoMouse                = 1 << 4,   
    ImGuiConfigFlags_NoMouseCursorChange    = 1 << 5,   

    
    ImGuiConfigFlags_IsSRGB                 = 1 << 20,  
    ImGuiConfigFlags_IsTouchScreen          = 1 << 21   
};


enum ImGuiBackendFlags_
{
    ImGuiBackendFlags_HasGamepad            = 1 << 0,   
    ImGuiBackendFlags_HasMouseCursors       = 1 << 1,   
    ImGuiBackendFlags_HasSetMousePos        = 1 << 2    
};


enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              
    ImGuiCol_ChildBg,               
    ImGuiCol_PopupBg,               
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,          
    ImGuiCol_NavWindowingHighlight, 
    ImGuiCol_NavWindowingDimBg,     
    ImGuiCol_ModalWindowDimBg,      
    ImGuiCol_COUNT

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiCol_ChildWindowBg = ImGuiCol_ChildBg, ImGuiCol_Column = ImGuiCol_Separator, ImGuiCol_ColumnHovered = ImGuiCol_SeparatorHovered, ImGuiCol_ColumnActive = ImGuiCol_SeparatorActive
    , ImGuiCol_ModalWindowDarkening = ImGuiCol_ModalWindowDimBg
    
    
#endif
};




enum ImGuiStyleVar_
{
    
    ImGuiStyleVar_Alpha,               
    ImGuiStyleVar_WindowPadding,       
    ImGuiStyleVar_WindowRounding,      
    ImGuiStyleVar_WindowBorderSize,    
    ImGuiStyleVar_WindowMinSize,       
    ImGuiStyleVar_WindowTitleAlign,    
    ImGuiStyleVar_ChildRounding,       
    ImGuiStyleVar_ChildBorderSize,     
    ImGuiStyleVar_PopupRounding,       
    ImGuiStyleVar_PopupBorderSize,     
    ImGuiStyleVar_FramePadding,        
    ImGuiStyleVar_FrameRounding,       
    ImGuiStyleVar_FrameBorderSize,     
    ImGuiStyleVar_ItemSpacing,         
    ImGuiStyleVar_ItemInnerSpacing,    
    ImGuiStyleVar_IndentSpacing,       
    ImGuiStyleVar_ScrollbarSize,       
    ImGuiStyleVar_ScrollbarRounding,   
    ImGuiStyleVar_GrabMinSize,         
    ImGuiStyleVar_GrabRounding,        
    ImGuiStyleVar_ButtonTextAlign,     
    ImGuiStyleVar_COUNT

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiStyleVar_Count_ = ImGuiStyleVar_COUNT, ImGuiStyleVar_ChildWindowRounding = ImGuiStyleVar_ChildRounding
#endif
};


enum ImGuiColorEditFlags_
{
    ImGuiColorEditFlags_None            = 0,
    ImGuiColorEditFlags_NoAlpha         = 1 << 1,   
    ImGuiColorEditFlags_NoPicker        = 1 << 2,   
    ImGuiColorEditFlags_NoOptions       = 1 << 3,   
    ImGuiColorEditFlags_NoSmallPreview  = 1 << 4,   
    ImGuiColorEditFlags_NoInputs        = 1 << 5,   
    ImGuiColorEditFlags_NoTooltip       = 1 << 6,   
    ImGuiColorEditFlags_NoLabel         = 1 << 7,   
    ImGuiColorEditFlags_NoSidePreview   = 1 << 8,   
    ImGuiColorEditFlags_NoDragDrop      = 1 << 9,   

    
    ImGuiColorEditFlags_AlphaBar        = 1 << 16,  
    ImGuiColorEditFlags_AlphaPreview    = 1 << 17,  
    ImGuiColorEditFlags_AlphaPreviewHalf= 1 << 18,  
    ImGuiColorEditFlags_HDR             = 1 << 19,  
    ImGuiColorEditFlags_RGB             = 1 << 20,  
    ImGuiColorEditFlags_HSV             = 1 << 21,  
    ImGuiColorEditFlags_HEX             = 1 << 22,  
    ImGuiColorEditFlags_Uint8           = 1 << 23,  
    ImGuiColorEditFlags_Float           = 1 << 24,  
    ImGuiColorEditFlags_PickerHueBar    = 1 << 25,  
    ImGuiColorEditFlags_PickerHueWheel  = 1 << 26,  

    
    ImGuiColorEditFlags__InputsMask     = ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_HSV|ImGuiColorEditFlags_HEX,
    ImGuiColorEditFlags__DataTypeMask   = ImGuiColorEditFlags_Uint8|ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags__PickerMask     = ImGuiColorEditFlags_PickerHueWheel|ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags__OptionsDefault = ImGuiColorEditFlags_Uint8|ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_PickerHueBar    
};



enum ImGuiMouseCursor_
{
    ImGuiMouseCursor_None = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,         
    ImGuiMouseCursor_ResizeAll,         
    ImGuiMouseCursor_ResizeNS,          
    ImGuiMouseCursor_ResizeEW,          
    ImGuiMouseCursor_ResizeNESW,        
    ImGuiMouseCursor_ResizeNWSE,        
    ImGuiMouseCursor_Hand,              
    ImGuiMouseCursor_COUNT

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiMouseCursor_Count_ = ImGuiMouseCursor_COUNT
#endif
};



enum ImGuiCond_
{
    ImGuiCond_Always        = 1 << 0,   
    ImGuiCond_Once          = 1 << 1,   
    ImGuiCond_FirstUseEver  = 1 << 2,   
    ImGuiCond_Appearing     = 1 << 3    

    
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiSetCond_Always = ImGuiCond_Always, ImGuiSetCond_Once = ImGuiCond_Once, ImGuiSetCond_FirstUseEver = ImGuiCond_FirstUseEver, ImGuiSetCond_Appearing = ImGuiCond_Appearing
#endif
};



struct ImGuiStyle
{
    float       Alpha;                      
    ImVec2      WindowPadding;              
    float       WindowRounding;             
    float       WindowBorderSize;           
    ImVec2      WindowMinSize;              
    ImVec2      WindowTitleAlign;           
    float       ChildRounding;              
    float       ChildBorderSize;            
    float       PopupRounding;              
    float       PopupBorderSize;            
    ImVec2      FramePadding;               
    float       FrameRounding;              
    float       FrameBorderSize;            
    ImVec2      ItemSpacing;                
    ImVec2      ItemInnerSpacing;           
    ImVec2      TouchExtraPadding;          
    float       IndentSpacing;              
    float       ColumnsMinSpacing;          
    float       ScrollbarSize;              
    float       ScrollbarRounding;          
    float       GrabMinSize;                
    float       GrabRounding;               
    ImVec2      ButtonTextAlign;            
    ImVec2      DisplayWindowPadding;       
    ImVec2      DisplaySafeAreaPadding;     
    float       MouseCursorScale;           
    bool        AntiAliasedLines;           
    bool        AntiAliasedFill;            
    float       CurveTessellationTol;       
    ImVec4      Colors[ImGuiCol_COUNT];

    IMGUI_API ImGuiStyle();
    IMGUI_API void ScaleAllSizes(float scale_factor);
};



struct ImGuiIO
{
    
    
    

    ImGuiConfigFlags   ConfigFlags;         
    ImGuiBackendFlags  BackendFlags;        
    ImVec2        DisplaySize;              
    float         DeltaTime;                
    float         IniSavingRate;            
    const char*   IniFilename;              
    const char*   LogFilename;              
    float         MouseDoubleClickTime;     
    float         MouseDoubleClickMaxDist;  
    float         MouseDragThreshold;       
    int           KeyMap[ImGuiKey_COUNT];   
    float         KeyRepeatDelay;           
    float         KeyRepeatRate;            
    void*         UserData;                 

    ImFontAtlas*  Fonts;                    
    float         FontGlobalScale;          
    bool          FontAllowUserScaling;     
    ImFont*       FontDefault;              
    ImVec2        DisplayFramebufferScale;  
    ImVec2        DisplayVisibleMin;        
    ImVec2        DisplayVisibleMax;        

    
    bool          MouseDrawCursor;              
    bool          ConfigMacOSXBehaviors;        
    bool          ConfigInputTextCursorBlink;   
    bool          ConfigResizeWindowsFromEdges; 

    
    
    

    
    
    const char* (*GetClipboardTextFn)(void* user_data);
    void        (*SetClipboardTextFn)(void* user_data, const char* text);
    void*       ClipboardUserData;

    
    
    void        (*ImeSetInputScreenPosFn)(int x, int y);
    void*       ImeWindowHandle;            

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    
    
    void        (*RenderDrawListsFn)(ImDrawData* data);
#else
    
    void*       RenderDrawListsFnUnused;
#endif

    
    
    

    ImVec2      MousePos;                       
    bool        MouseDown[5];                   
    float       MouseWheel;                     
    float       MouseWheelH;                    
    bool        KeyCtrl;                        
    bool        KeyShift;                       
    bool        KeyAlt;                         
    bool        KeySuper;                       
    bool        KeysDown[512];                  
    ImWchar     InputCharacters[16+1];          
    float       NavInputs[ImGuiNavInput_COUNT]; 

    
    IMGUI_API void AddInputCharacter(ImWchar c);                        
    IMGUI_API void AddInputCharactersUTF8(const char* utf8_chars);      
    inline void    ClearInputCharacters() { InputCharacters[0] = 0; }   

    
    
    

    bool        WantCaptureMouse;           
    bool        WantCaptureKeyboard;        
    bool        WantTextInput;              
    bool        WantSetMousePos;            
    bool        WantSaveIniSettings;        
    bool        NavActive;                  
    bool        NavVisible;                 
    float       Framerate;                  
    int         MetricsRenderVertices;      
    int         MetricsRenderIndices;       
    int         MetricsRenderWindows;       
    int         MetricsActiveWindows;       
    int         MetricsActiveAllocations;   
    ImVec2      MouseDelta;                 

    
    
    

    ImVec2      MousePosPrev;               
    ImVec2      MouseClickedPos[5];         
    double      MouseClickedTime[5];        
    bool        MouseClicked[5];            
    bool        MouseDoubleClicked[5];      
    bool        MouseReleased[5];           
    bool        MouseDownOwned[5];          
    float       MouseDownDuration[5];       
    float       MouseDownDurationPrev[5];   
    ImVec2      MouseDragMaxDistanceAbs[5]; 
    float       MouseDragMaxDistanceSqr[5]; 
    float       KeysDownDuration[512];      
    float       KeysDownDurationPrev[512];  
    float       NavInputsDownDuration[ImGuiNavInput_COUNT];
    float       NavInputsDownDurationPrev[ImGuiNavInput_COUNT];

    IMGUI_API   ImGuiIO();
};





#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
namespace ImGui
{
    
    static inline bool  IsItemDeactivatedAfterChange()        { return IsItemDeactivatedAfterEdit(); }
    
    IMGUI_API bool      InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags extra_flags = 0); 
    IMGUI_API bool      InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool      InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags extra_flags = 0);
    IMGUI_API bool      InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags extra_flags = 0);
    
    static inline bool  IsAnyWindowFocused()                  { return IsWindowFocused(ImGuiFocusedFlags_AnyWindow); }
    static inline bool  IsAnyWindowHovered()                  { return IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }
    static inline ImVec2 CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = 0.f) { (void)on_edge; (void)outward; IM_ASSERT(0); return pos; }
    
    static inline void  ShowTestWindow()                      { return ShowDemoWindow(); }
    static inline bool  IsRootWindowFocused()                 { return IsWindowFocused(ImGuiFocusedFlags_RootWindow); }
    static inline bool  IsRootWindowOrAnyChildFocused()       { return IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows); }
    static inline void  SetNextWindowContentWidth(float w)    { SetNextWindowContentSize(ImVec2(w, 0.0f)); }
    static inline float GetItemsLineHeightWithSpacing()       { return GetFrameHeightWithSpacing(); }
    
    IMGUI_API bool      Begin(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha_override = -1.0f, ImGuiWindowFlags flags = 0); 
    static inline bool  IsRootWindowOrAnyChildHovered()       { return IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows); }
    static inline void  AlignFirstTextHeightToWidgets()       { AlignTextToFramePadding(); }
    static inline void  SetNextWindowPosCenter(ImGuiCond c=0) { ImGuiIO& io = GetIO(); SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), c, ImVec2(0.5f, 0.5f)); }
    
    static inline bool  IsItemHoveredRect()                   { return IsItemHovered(ImGuiHoveredFlags_RectOnly); }
    static inline bool  IsPosHoveringAnyWindow(const ImVec2&) { IM_ASSERT(0); return false; } 
    static inline bool  IsMouseHoveringAnyWindow()            { return IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }
    static inline bool  IsMouseHoveringWindow()               { return IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem); }
}
#endif







template<typename T>
class ImVector
{
public:
    int                         Size;
    int                         Capacity;
    T*                          Data;

    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    inline ImVector()           { Size = Capacity = 0; Data = NULL; }
    inline ~ImVector()          { if (Data) ImGui::MemFree(Data); }
    inline ImVector(const ImVector<T>& src)                     { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline ImVector& operator=(const ImVector<T>& src)          { clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(value_type)); return *this; }

    inline bool                 empty() const                   { return Size == 0; }
    inline int                  size() const                    { return Size; }
    inline int                  capacity() const                { return Capacity; }
    inline value_type&          operator[](int i)               { IM_ASSERT(i < Size); return Data[i]; }
    inline const value_type&    operator[](int i) const         { IM_ASSERT(i < Size); return Data[i]; }

    inline void                 clear()                         { if (Data) { Size = Capacity = 0; ImGui::MemFree(Data); Data = NULL; } }
    inline iterator             begin()                         { return Data; }
    inline const_iterator       begin() const                   { return Data; }
    inline iterator             end()                           { return Data + Size; }
    inline const_iterator       end() const                     { return Data + Size; }
    inline value_type&          front()                         { IM_ASSERT(Size > 0); return Data[0]; }
    inline const value_type&    front() const                   { IM_ASSERT(Size > 0); return Data[0]; }
    inline value_type&          back()                          { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const value_type&    back() const                    { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void                 swap(ImVector<value_type>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; value_type* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const            { int new_capacity = Capacity ? (Capacity + Capacity/2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size,const value_type& v){ if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         reserve(int new_capacity)
    {
        if (new_capacity <= Capacity)
            return;
        value_type* new_data = (value_type*)ImGui::MemAlloc((size_t)new_capacity * sizeof(value_type));
        if (Data)
        {
            memcpy(new_data, Data, (size_t)Size * sizeof(value_type));
            ImGui::MemFree(Data);
        }
        Data = new_data;
        Capacity = new_capacity;
    }

    
    inline void         push_back(const value_type& v)                  { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back()                                      { IM_ASSERT(Size > 0); Size--; }
    inline void         push_front(const value_type& v)                 { if (Size == 0) push_back(v); else insert(Data, v); }
    inline iterator     erase(const_iterator it)                        { IM_ASSERT(it >= Data && it < Data+Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type)); Size--; return Data + off; }
    inline iterator     erase(const_iterator it, const_iterator it_last){ IM_ASSERT(it >= Data && it < Data+Size && it_last > it && it_last <= Data+Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - count) * sizeof(value_type)); Size -= (int)count; return Data + off; }
    inline iterator     erase_unsorted(const_iterator it)               { IM_ASSERT(it >= Data && it < Data+Size);  const ptrdiff_t off = it - Data; if (it < Data+Size-1) memcpy(Data + off, Data + Size - 1, sizeof(value_type)); Size--; return Data + off; }
    inline iterator     insert(const_iterator it, const value_type& v)  { IM_ASSERT(it >= Data && it <= Data+Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const value_type& v) const             { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline int          index_from_pointer(const_iterator it) const     { IM_ASSERT(it >= Data && it <= Data+Size); const ptrdiff_t off = it - Data; return (int)off; }
};




struct ImNewDummy {};
inline void* operator new(size_t, ImNewDummy, void* ptr) { return ptr; }
inline void  operator delete(void*, ImNewDummy, void*)   {} 
#define IM_PLACEMENT_NEW(_PTR)              new(ImNewDummy(), _PTR)
#define IM_NEW(_TYPE)                       new(ImNewDummy(), ImGui::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void IM_DELETE(T* p)   { if (p) { p->~T(); ImGui::MemFree(p); } }



struct ImGuiOnceUponAFrame
{
    ImGuiOnceUponAFrame() { RefFrame = -1; }
    mutable int RefFrame;
    operator bool() const { int current_frame = ImGui::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};


#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS    
#define IMGUI_ONCE_UPON_A_FRAME     static ImGuiOnceUponAFrame imgui_oaf; if (imgui_oaf)
#endif


struct ImGuiTextFilter
{
    IMGUI_API           ImGuiTextFilter(const char* default_filter = "");
    IMGUI_API bool      Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);    
    IMGUI_API bool      PassFilter(const char* text, const char* text_end = NULL) const;
    IMGUI_API void      Build();
    void                Clear()          { InputBuf[0] = 0; Build(); }
    bool                IsActive() const { return !Filters.empty(); }

    
    struct TextRange
    {
        const char* b;
        const char* e;

        TextRange() { b = e = NULL; }
        TextRange(const char* _b, const char* _e) { b = _b; e = _e; }
        const char*     begin() const   { return b; }
        const char*     end () const    { return e; }
        bool            empty() const   { return b == e; }
        IMGUI_API void  split(char separator, ImVector<TextRange>* out) const;
    };
    char                InputBuf[256];
    ImVector<TextRange> Filters;
    int                 CountGrep;
};


struct ImGuiTextBuffer
{
    ImVector<char>      Buf;

    ImGuiTextBuffer()   { Buf.push_back(0); }
    inline char         operator[](int i) { return Buf.Data[i]; }
    const char*         begin() const { return &Buf.front(); }
    const char*         end() const { return &Buf.back(); }      
    int                 size() const { return Buf.Size - 1; }
    bool                empty() { return Buf.Size <= 1; }
    void                clear() { Buf.clear(); Buf.push_back(0); }
    void                reserve(int capacity) { Buf.reserve(capacity); }
    const char*         c_str() const { return Buf.Data; }
    IMGUI_API void      appendf(const char* fmt, ...) IM_FMTARGS(2);
    IMGUI_API void      appendfv(const char* fmt, va_list args) IM_FMTLIST(2);
};









struct ImGuiStorage
{
    struct Pair
    {
        ImGuiID key;
        union { int val_i; float val_f; void* val_p; };
        Pair(ImGuiID _key, int _val_i)   { key = _key; val_i = _val_i; }
        Pair(ImGuiID _key, float _val_f) { key = _key; val_f = _val_f; }
        Pair(ImGuiID _key, void* _val_p) { key = _key; val_p = _val_p; }
    };
    ImVector<Pair>      Data;

    
    
    
    void                Clear() { Data.clear(); }
    IMGUI_API int       GetInt(ImGuiID key, int default_val = 0) const;
    IMGUI_API void      SetInt(ImGuiID key, int val);
    IMGUI_API bool      GetBool(ImGuiID key, bool default_val = false) const;
    IMGUI_API void      SetBool(ImGuiID key, bool val);
    IMGUI_API float     GetFloat(ImGuiID key, float default_val = 0.0f) const;
    IMGUI_API void      SetFloat(ImGuiID key, float val);
    IMGUI_API void*     GetVoidPtr(ImGuiID key) const; 
    IMGUI_API void      SetVoidPtr(ImGuiID key, void* val);

    
    
    
    
    IMGUI_API int*      GetIntRef(ImGuiID key, int default_val = 0);
    IMGUI_API bool*     GetBoolRef(ImGuiID key, bool default_val = false);
    IMGUI_API float*    GetFloatRef(ImGuiID key, float default_val = 0.0f);
    IMGUI_API void**    GetVoidPtrRef(ImGuiID key, void* default_val = NULL);

    
    IMGUI_API void      SetAllInt(int val);

    
    IMGUI_API void      BuildSortByKey();
};






struct ImGuiInputTextCallbackData
{
    ImGuiInputTextFlags EventFlag;      
    ImGuiInputTextFlags Flags;          
    void*               UserData;       

    
    
    
    ImWchar             EventChar;      
    ImGuiKey            EventKey;       
    char*               Buf;            
    int                 BufTextLen;     
    int                 BufSize;        
    bool                BufDirty;       
    int                 CursorPos;      
    int                 SelectionStart; 
    int                 SelectionEnd;   

    
    
    ImGuiInputTextCallbackData();
    IMGUI_API void      DeleteChars(int pos, int bytes_count);
    IMGUI_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    bool                HasSelection() const { return SelectionStart != SelectionEnd; }
};

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
typedef ImGuiInputTextCallback      ImGuiTextEditCallback;      
typedef ImGuiInputTextCallbackData  ImGuiTextEditCallbackData;
#endif



struct ImGuiSizeCallbackData
{
    void*   UserData;       
    ImVec2  Pos;            
    ImVec2  CurrentSize;    
    ImVec2  DesiredSize;    
};


struct ImGuiPayload
{
    
    void*           Data;               
    int             DataSize;           

    
    ImGuiID         SourceId;           
    ImGuiID         SourceParentId;     
    int             DataFrameCount;     
    char            DataType[32+1];     
    bool            Preview;            
    bool            Delivery;           

    ImGuiPayload()  { Clear(); }
    void Clear()    { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
    bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
    bool IsPreview() const                  { return Preview; }
    bool IsDelivery() const                 { return Delivery; }
};


#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT    16
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    0
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#else
#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#endif
#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255,255,255,255)  
#define IM_COL32_BLACK       IM_COL32(0,0,0,255)        
#define IM_COL32_BLACK_TRANS IM_COL32(0,0,0,0)          





struct ImColor
{
    ImVec4              Value;

    ImColor()                                                       { Value.x = Value.y = Value.z = Value.w = 0.0f; }
    ImColor(int r, int g, int b, int a = 255)                       { float sc = 1.0f/255.0f; Value.x = (float)r * sc; Value.y = (float)g * sc; Value.z = (float)b * sc; Value.w = (float)a * sc; }
    ImColor(ImU32 rgba)                                             { float sc = 1.0f/255.0f; Value.x = (float)((rgba>>IM_COL32_R_SHIFT)&0xFF) * sc; Value.y = (float)((rgba>>IM_COL32_G_SHIFT)&0xFF) * sc; Value.z = (float)((rgba>>IM_COL32_B_SHIFT)&0xFF) * sc; Value.w = (float)((rgba>>IM_COL32_A_SHIFT)&0xFF) * sc; }
    ImColor(float r, float g, float b, float a = 1.0f)              { Value.x = r; Value.y = g; Value.z = b; Value.w = a; }
    ImColor(const ImVec4& col)                                      { Value = col; }
    inline operator ImU32() const                                   { return ImGui::ColorConvertFloat4ToU32(Value); }
    inline operator ImVec4() const                                  { return Value; }

    
    inline void    SetHSV(float h, float s, float v, float a = 1.0f){ ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }
    static ImColor HSV(float h, float s, float v, float a = 1.0f)   { float r,g,b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b); return ImColor(r,g,b,a); }
};














struct ImGuiListClipper
{
    float   StartPosY;
    float   ItemsHeight;
    int     ItemsCount, StepNo, DisplayStart, DisplayEnd;

    
    
    
    ImGuiListClipper(int items_count = -1, float items_height = -1.0f)  { Begin(items_count, items_height); } 
    ~ImGuiListClipper()                                                 { IM_ASSERT(ItemsCount == -1); }      

    IMGUI_API bool Step();                                              
    IMGUI_API void Begin(int items_count, float items_height = -1.0f);  
    IMGUI_API void End();                                               
};










typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);


struct ImDrawCmd
{
    unsigned int    ElemCount;              
    ImVec4          ClipRect;               
    ImTextureID     TextureId;              
    ImDrawCallback  UserCallback;           
    void*           UserCallbackData;       

    ImDrawCmd() { ElemCount = 0; ClipRect.x = ClipRect.y = ClipRect.z = ClipRect.w = 0.0f; TextureId = NULL; UserCallback = NULL; UserCallbackData = NULL; }
};


#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;
#endif


#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
    ImVec2  pos;
    ImVec2  uv;
    ImU32   col;
};
#else




IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif



struct ImDrawChannel
{
    ImVector<ImDrawCmd>     CmdBuffer;
    ImVector<ImDrawIdx>     IdxBuffer;
};

enum ImDrawCornerFlags_
{
    ImDrawCornerFlags_TopLeft   = 1 << 0, 
    ImDrawCornerFlags_TopRight  = 1 << 1, 
    ImDrawCornerFlags_BotLeft   = 1 << 2, 
    ImDrawCornerFlags_BotRight  = 1 << 3, 
    ImDrawCornerFlags_Top       = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,   
    ImDrawCornerFlags_Bot       = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,   
    ImDrawCornerFlags_Left      = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,    
    ImDrawCornerFlags_Right     = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight,  
    ImDrawCornerFlags_All       = 0xF     
};

enum ImDrawListFlags_
{
    ImDrawListFlags_AntiAliasedLines = 1 << 0,
    ImDrawListFlags_AntiAliasedFill  = 1 << 1
};







struct ImDrawList
{
    
    ImVector<ImDrawCmd>     CmdBuffer;          
    ImVector<ImDrawIdx>     IdxBuffer;          
    ImVector<ImDrawVert>    VtxBuffer;          
    ImDrawListFlags         Flags;              

    
    const ImDrawListSharedData* _Data;          
    const char*             _OwnerName;         
    unsigned int            _VtxCurrentIdx;     
    ImDrawVert*             _VtxWritePtr;       
    ImDrawIdx*              _IdxWritePtr;       
    ImVector<ImVec4>        _ClipRectStack;     
    ImVector<ImTextureID>   _TextureIdStack;    
    ImVector<ImVec2>        _Path;              
    int                     _ChannelsCurrent;   
    int                     _ChannelsCount;     
    ImVector<ImDrawChannel> _Channels;          

    
    ImDrawList(const ImDrawListSharedData* shared_data) { _Data = shared_data; _OwnerName = NULL; Clear(); }
    ~ImDrawList() { ClearFreeMemory(); }
    IMGUI_API void  PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false);  
    IMGUI_API void  PushClipRectFullScreen();
    IMGUI_API void  PopClipRect();
    IMGUI_API void  PushTextureID(ImTextureID texture_id);
    IMGUI_API void  PopTextureID();
    inline ImVec2   GetClipRectMin() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.x, cr.y); }
    inline ImVec2   GetClipRectMax() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.z, cr.w); }

    
    IMGUI_API void  AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All, float thickness = 1.0f);   
    IMGUI_API void  AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All);                     
    IMGUI_API void  AddRectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    IMGUI_API void  AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
    IMGUI_API void  AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
    IMGUI_API void  AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
    IMGUI_API void  AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
    IMGUI_API void  AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
    IMGUI_API void  AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
    IMGUI_API void  AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a = ImVec2(0,0), const ImVec2& uv_b = ImVec2(1,1), ImU32 col = 0xFFFFFFFF);
    IMGUI_API void  AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a = ImVec2(0,0), const ImVec2& uv_b = ImVec2(1,0), const ImVec2& uv_c = ImVec2(1,1), const ImVec2& uv_d = ImVec2(0,1), ImU32 col = 0xFFFFFFFF);
    IMGUI_API void  AddImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners = ImDrawCornerFlags_All);
    IMGUI_API void  AddPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness);
    IMGUI_API void  AddConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col); 
    IMGUI_API void  AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);

    
    inline    void  PathClear()                                                 { _Path.resize(0); }
    inline    void  PathLineTo(const ImVec2& pos)                               { _Path.push_back(pos); }
    inline    void  PathLineToMergeDuplicate(const ImVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path[_Path.Size-1], &pos, 8) != 0) _Path.push_back(pos); }
    inline    void  PathFillConvex(ImU32 col)                                   { AddConvexPolyFilled(_Path.Data, _Path.Size, col); PathClear(); }  
    inline    void  PathStroke(ImU32 col, bool closed, float thickness = 1.0f)  { AddPolyline(_Path.Data, _Path.Size, col, closed, thickness); PathClear(); }
    IMGUI_API void  PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments = 10);
    IMGUI_API void  PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12);                                            
    IMGUI_API void  PathBezierCurveTo(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, int num_segments = 0);
    IMGUI_API void  PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All);

    
    
    
    IMGUI_API void  ChannelsSplit(int channels_count);
    IMGUI_API void  ChannelsMerge();
    IMGUI_API void  ChannelsSetCurrent(int channel_index);

    
    IMGUI_API void  AddCallback(ImDrawCallback callback, void* callback_data);  
    IMGUI_API void  AddDrawCmd();                                               
    IMGUI_API ImDrawList* CloneOutput() const;                                  

    
    
    IMGUI_API void  Clear();
    IMGUI_API void  ClearFreeMemory();
    IMGUI_API void  PrimReserve(int idx_count, int vtx_count);
    IMGUI_API void  PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);      
    IMGUI_API void  PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
    IMGUI_API void  PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
    inline    void  PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col){ _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
    inline    void  PrimWriteIdx(ImDrawIdx idx)                                 { *_IdxWritePtr = idx; _IdxWritePtr++; }
    inline    void  PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)     { PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); }
    IMGUI_API void  UpdateClipRect();
    IMGUI_API void  UpdateTextureID();
};



struct ImDrawData
{
    bool            Valid;                  
    ImDrawList**    CmdLists;               
    int             CmdListsCount;          
    int             TotalIdxCount;          
    int             TotalVtxCount;          
    ImVec2          DisplayPos;             
    ImVec2          DisplaySize;            

    
    ImDrawData()    { Valid = false; Clear(); }
    ~ImDrawData()   { Clear(); }
    void Clear()    { Valid = false; CmdLists = NULL; CmdListsCount = TotalVtxCount = TotalIdxCount = 0; DisplayPos = DisplaySize = ImVec2(0.f, 0.f); } 
    IMGUI_API void  DeIndexAllBuffers();                
    IMGUI_API void  ScaleClipRects(const ImVec2& sc);   
};

struct ImFontConfig
{
    void*           FontData;               
    int             FontDataSize;           
    bool            FontDataOwnedByAtlas;   
    int             FontNo;                 
    float           SizePixels;             
    int             OversampleH;            
    int             OversampleV;            
    bool            PixelSnapH;             
    ImVec2          GlyphExtraSpacing;      
    ImVec2          GlyphOffset;            
    const ImWchar*  GlyphRanges;            
    float           GlyphMinAdvanceX;       
    float           GlyphMaxAdvanceX;       
    bool            MergeMode;              
    unsigned int    RasterizerFlags;        
    float           RasterizerMultiply;     

    
    char            Name[40];               
    ImFont*         DstFont;

    IMGUI_API ImFontConfig();
};

struct ImFontGlyph
{
    ImWchar         Codepoint;          
    float           AdvanceX;           
    float           X0, Y0, X1, Y1;     
    float           U0, V0, U1, V1;     
};

enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1    
};


















struct ImFontAtlas
{
    IMGUI_API ImFontAtlas();
    IMGUI_API ~ImFontAtlas();
    IMGUI_API ImFont*           AddFont(const ImFontConfig* font_cfg);
    IMGUI_API ImFont*           AddFontDefault(const ImFontConfig* font_cfg = NULL);
    IMGUI_API ImFont*           AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    IMGUI_API ImFont*           AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); 
    IMGUI_API ImFont*           AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); 
    IMGUI_API ImFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);              
    IMGUI_API void              ClearInputData();           
    IMGUI_API void              ClearTexData();             
    IMGUI_API void              ClearFonts();               
    IMGUI_API void              Clear();                    

    
    
    
    
    
    IMGUI_API bool              Build();                    
    IMGUI_API bool              IsBuilt()                   { return Fonts.Size > 0 && (TexPixelsAlpha8 != NULL || TexPixelsRGBA32 != NULL); }
    IMGUI_API void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  
    IMGUI_API void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  
    void                        SetTexID(ImTextureID id)    { TexID = id; }

    
    
    

    
    
    
    IMGUI_API const ImWchar*    GetGlyphRangesDefault();                
    IMGUI_API const ImWchar*    GetGlyphRangesKorean();                 
    IMGUI_API const ImWchar*    GetGlyphRangesJapanese();               
    IMGUI_API const ImWchar*    GetGlyphRangesChineseFull();            
    IMGUI_API const ImWchar*    GetGlyphRangesChineseSimplifiedCommon();
    IMGUI_API const ImWchar*    GetGlyphRangesCyrillic();               
    IMGUI_API const ImWchar*    GetGlyphRangesThai();                   

    
    struct GlyphRangesBuilder
    {
        ImVector<unsigned char> UsedChars;  
        GlyphRangesBuilder()                { UsedChars.resize(0x10000 / 8); memset(UsedChars.Data, 0, 0x10000 / 8); }
        bool           GetBit(int n) const  { return (UsedChars[n >> 3] & (1 << (n & 7))) != 0; }
        void           SetBit(int n)        { UsedChars[n >> 3] |= 1 << (n & 7); }  
        void           AddChar(ImWchar c)   { SetBit(c); }                          
        IMGUI_API void AddText(const char* text, const char* text_end = NULL);      
        IMGUI_API void AddRanges(const ImWchar* ranges);                            
        IMGUI_API void BuildRanges(ImVector<ImWchar>* out_ranges);                  
    };

    
    
    

    
    
    struct CustomRect
    {
        unsigned int    ID;             
        unsigned short  Width, Height;  
        unsigned short  X, Y;           
        float           GlyphAdvanceX;  
        ImVec2          GlyphOffset;    
        ImFont*         Font;           
        CustomRect()            { ID = 0xFFFFFFFF; Width = Height = 0; X = Y = 0xFFFF; GlyphAdvanceX = 0.0f; GlyphOffset = ImVec2(0,0); Font = NULL; }
        bool IsPacked() const   { return X != 0xFFFF; }
    };

    IMGUI_API int       AddCustomRectRegular(unsigned int id, int width, int height);                                                                   
    IMGUI_API int       AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0,0));   
    const CustomRect*   GetCustomRectByIndex(int index) const { if (index < 0) return NULL; return &CustomRects[index]; }

    
    IMGUI_API void      CalcCustomRectUV(const CustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max);
    IMGUI_API bool      GetMouseCursorTexData(ImGuiMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]);

    
    
    

    bool                        Locked;             
    ImFontAtlasFlags            Flags;              
    ImTextureID                 TexID;              
    int                         TexDesiredWidth;    
    int                         TexGlyphPadding;    

    
    
    unsigned char*              TexPixelsAlpha8;    
    unsigned int*               TexPixelsRGBA32;    
    int                         TexWidth;           
    int                         TexHeight;          
    ImVec2                      TexUvScale;         
    ImVec2                      TexUvWhitePixel;    
    ImVector<ImFont*>           Fonts;              
    ImVector<CustomRect>        CustomRects;        
    ImVector<ImFontConfig>      ConfigData;         
    int                         CustomRectIds[1];   
};



struct ImFont
{
    
    float                       FontSize;           
    float                       Scale;              
    ImVec2                      DisplayOffset;      
    ImVector<ImFontGlyph>       Glyphs;             
    ImVector<float>             IndexAdvanceX;      
    ImVector<unsigned short>    IndexLookup;        
    const ImFontGlyph*          FallbackGlyph;      
    float                       FallbackAdvanceX;   
    ImWchar                     FallbackChar;       

    
    short                       ConfigDataCount;    
    ImFontConfig*               ConfigData;         
    ImFontAtlas*                ContainerAtlas;     
    float                       Ascent, Descent;    
    bool                        DirtyLookupTables;
    int                         MetricsTotalSurface;

    
    IMGUI_API ImFont();
    IMGUI_API ~ImFont();
    IMGUI_API void              ClearOutputData();
    IMGUI_API void              BuildLookupTable();
    IMGUI_API const ImFontGlyph*FindGlyph(ImWchar c) const;
    IMGUI_API const ImFontGlyph*FindGlyphNoFallback(ImWchar c) const;
    IMGUI_API void              SetFallbackChar(ImWchar c);
    float                       GetCharAdvance(ImWchar c) const     { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
    bool                        IsLoaded() const                    { return ContainerAtlas != NULL; }
    const char*                 GetDebugName() const                { return ConfigData ? ConfigData->Name : "<unknown>"; }

    
    
    IMGUI_API ImVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; 
    IMGUI_API const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    IMGUI_API void              RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const;
    IMGUI_API void              RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    
    IMGUI_API void              GrowIndex(int new_size);
    IMGUI_API void              AddGlyph(ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    IMGUI_API void              AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true); 

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    typedef ImFontGlyph Glyph; 
#endif
};

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif


#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif
