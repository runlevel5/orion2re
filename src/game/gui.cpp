#include "deps.h"

namespace gui {

// Suggested name: GUI_Prepare_Text
char** __cdecl GUI_Prepare_Text(const char* text, int font_id, int width, int* out_height, int* out_line_count) {
    GUI_Set_Font(font_id);
    int line_height = GUI_Get_Line_Height();
    int line_count = GUI_Get_Line_Count(text, width);

    if (out_line_count) {
        *out_line_count = line_count;
    }

    if (out_height) {
        *out_height = line_count * line_height;
    }

    if (line_count == 0) {
        return nullptr;
    }

    char** lines = (char**)MOX::Check_Allocate(line_count * sizeof(char*));
    for (int i = 0; i < line_count; ++i) {
        int offset = GUI_Get_Line_Offset(text, width, i);
        int length = GUI_Get_Line_Length(text, width, i);
        lines[i] = (char*)MOX::Check_Allocate(length + 1);
        _strncpy(lines[i], text + offset, length);
        lines[i][length] = '\0';
    }

    return lines;
}

}