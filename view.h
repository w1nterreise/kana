#pragma once


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "define.h"




typedef struct {
    WNDCLASSA wndClass;
    HWND hWnd;
    HDC hDc;
    HDC hDcCompat;
    RECT clientRect;
        HWND up;
        HWND down;
        HWND left;
        HWND right;
        HWND label;
        HWND toggle;
} kana_form_t;

kana_form_t form;

typedef struct {
    HBITMAP katakana[KANA_SIZE];
    HBITMAP hiragana[KANA_SIZE];
} bitmaps_t;

bitmaps_t bmp;

int mode;

LRESULT window_process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void get_img_path(char * dst, const char romaji[], const char k[]);
void load_bitmaps();
bool assert_bitmaps_loaded();

void init_window();
void init_controls();
void redraw();
void update();

void cmd_up();
void cmd_down();
void cmd_left();
void cmd_right();
void cmd_common(int pick);
void switch_kana();
