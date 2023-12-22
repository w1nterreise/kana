#include "view.h"
#include "case_fix.h"




LRESULT
window_process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == WM_DESTROY) post_quit_message(0);

    bool key = message == WM_KEYUP;
    bool cmd = message == WM_COMMAND;
    static bool action[] = {0, 0, 0, 0, 0};

    action[0] = (key && (wParam == 38 || wParam == 87)) || (cmd && (form.up     == (HWND) lParam));
    action[1] = (key && (wParam == 40 || wParam == 83)) || (cmd && (form.down   == (HWND) lParam));
    action[2] = (key && (wParam == 37 || wParam == 65)) || (cmd && (form.left   == (HWND) lParam));
    action[3] = (key && (wParam == 39 || wParam == 68)) || (cmd && (form.right  == (HWND) lParam));
    action[4] = (key && (wParam == 32 || wParam == 32)) || (cmd && (form.toggle == (HWND) lParam));

    static void (*delegate[])() = { &cmd_up, &cmd_down, &cmd_left, &cmd_right, &switch_kana };
    for (int i = 0; i < 5; i ++) { if (action[i] == TRUE) delegate[i](); }
    if (cmd) set_focus(hWnd);

    return def_window_proc_a(hWnd, message, wParam, lParam);
}


void
load_bitmaps() {

    for (int i = 0; i < KANA_SIZE; i ++) {
        size_t len = strlen(kana[i]);
        char path_hiragana[len + 10];
        char path_katakana[len + 10];
        sprintf(path_hiragana, "img\\h_%s.bmp", kana[i]);
        sprintf(path_katakana, "img\\k_%s.bmp", kana[i]);
        bmp.hiragana[i] = (HBITMAP) LoadImage(NULL, path_hiragana, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        bmp.katakana[i] = (HBITMAP) LoadImage(NULL, path_katakana, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
}


bool
assert_bitmaps_loaded() {

    size_t count = 0;
    for (int i = 0; i < KANA_SIZE; i ++) {
        if (bmp.hiragana[i] != NULL) count ++;
        if (bmp.katakana[i] != NULL) count ++;
    }
    return count == (2 * KANA_SIZE);
}


void
redraw() {

    int p = model.sequence[model.pointer];
    select_object(form.hDcCompat, mode == MODE ? bmp.hiragana[p] : bmp.katakana[p]);
    bit_blt(form.hDc, WINDOW_WIDTH / 2 - GLYPH_WIDTH / 2, GLYPH_TOP, GLYPH_WIDTH, GLYPH_HEIGHT, form.hDcCompat, 0, 0, SRCCOPY);
}


void
init_window() {

    memset(&form.wndClass, 0, sizeof(WNDCLASSA));
    form.wndClass.lpszClassName = "kana trainer window";
    form.wndClass.lpfnWndProc = window_process;
    register_class_a(&form.wndClass);

    int screenX = get_system_metrics(SM_CXSCREEN);
    int screenY = get_system_metrics(SM_CYSCREEN);

    int x = (screenX / 2) - (WINDOW_WIDTH / 2);
    int y = (screenY / 2) - (WINDOW_HEIGHT / 2);

    form.hWnd = create_window("kana trainer window",
                             "",
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                             x, y,
                             WINDOW_WIDTH, WINDOW_HEIGHT,
                             NULL, NULL, NULL, NULL
                             );

    form.hDc = get_dc(form.hWnd);
    form.hDcCompat = create_compatible_dc(form.hDc);
    get_client_rect(form.hWnd, &form.clientRect);
    mode = MODE;
}


void
init_controls() {

    form.up     = create_window("button",
                                "",
                                WS_VISIBLE | WS_CHILD | BS_FLAT,
                                WINDOW_WIDTH / 2 - BTN_UP_WIDTH / 2,
                                CONTROLS_TOP,
                                BTN_UP_WIDTH, BTN_UP_HEIGHT,
                                form.hWnd, NULL, NULL, NULL
                                );

    form.down   = create_window("button",
                                "",
                                WS_VISIBLE | WS_CHILD | BS_FLAT,
                                WINDOW_WIDTH / 2 - BTN_DOWN_WIDTH / 2,
                                CONTROLS_TOP + CONTROLS_VERTICAL_DISTANCE + BTN_UP_HEIGHT,
                                BTN_DOWN_WIDTH, BTN_DOWN_HEIGHT,
                                form.hWnd, NULL, NULL, NULL
                                );

    form.left   = create_window("button",
                                "",
                                WS_VISIBLE | WS_CHILD | BS_FLAT,
                                WINDOW_WIDTH / 2 - BTNS_LR_WIDTH - BTNS_LR_DISTANCE_FROM_CENTER_HORIZ,
                                CONTROLS_TOP + BTNS_LR_TOP_FROM_CTRLS_TOP,
                                BTNS_LR_WIDTH, BTNS_LR_HEIGHT,
                                form.hWnd, NULL, NULL, NULL
                                );

    form.right  = create_window("button",
                                "",
                                WS_VISIBLE | WS_CHILD | BS_FLAT,
                                WINDOW_WIDTH / 2 + BTNS_LR_DISTANCE_FROM_CENTER_HORIZ,
                                CONTROLS_TOP + BTNS_LR_TOP_FROM_CTRLS_TOP,
                                BTNS_LR_WIDTH, BTNS_LR_HEIGHT,
                                form.hWnd, NULL, NULL, NULL
                                );

    form.label  = create_window("static",
                                "label",
                                WS_VISIBLE | WS_CHILD | SS_CENTER,
                                WINDOW_WIDTH / 2 - BTNS_LR_DISTANCE_FROM_CENTER_HORIZ - BTNS_LR_WIDTH, 300,
                                LABEL_WIDTH, LABEL_HEIGHT,
                                form.hWnd, NULL, NULL, NULL
                                );

    form.toggle = create_window("button",
                                "k",
                                WS_VISIBLE | WS_CHILD | BS_FLAT,
                                WINDOW_WIDTH / 2 + BTNS_LR_DISTANCE_FROM_CENTER_HORIZ + BTNS_LR_WIDTH - SWITCH_SIZE,
                                300 - SWITCH_SIZE + LABEL_HEIGHT,
                                SWITCH_SIZE, SWITCH_SIZE,
                                form.hWnd, NULL, NULL, NULL
                                );
}


void
update() {

    set_window_text(form.toggle, mode == MODE ? "h" : "k");
    set_window_text(form.up,     kana[model.choices[model.pointer][0]]);
    set_window_text(form.down,   kana[model.choices[model.pointer][1]]);
    set_window_text(form.left,   kana[model.choices[model.pointer][2]]);
    set_window_text(form.right,  kana[model.choices[model.pointer][3]]);

    enable_window(form.up,    model.choice_enable[0]);
    enable_window(form.down,  model.choice_enable[1]);
    enable_window(form.left,  model.choice_enable[2]);
    enable_window(form.right, model.choice_enable[3]);

    char stats[40];
    sprintf(stats, "%d / %d.    faults: %d", model.pointer + 1, KANA_SIZE, model.faults);
    set_window_text(form.label, stats);
    redraw();
}


void
switch_kana() {

    if (model.pointer == KANA_SIZE - 1) {
        generate_new_layout();
    }

    mode *= (-1);
    update();
    play_sound(SND_SWITCH, NULL, SND_FILENAME | SND_ASYNC);
}


void
cmd_common(int pick) {

    update();
    if (pick == 1)   play_sound(SND_OK,     NULL, SND_FILENAME | SND_ASYNC);
    if (pick == -1)  play_sound(SND_FAIL,   NULL, SND_FILENAME | SND_ASYNC);
    if (pick == END) play_sound(SND_END,    NULL, SND_FILENAME | SND_ASYNC);
}


void
cmd_up() {

    cmd_common(pick(0));
}


void
cmd_down() {

    cmd_common(pick(1));
}


void
cmd_left() {

    cmd_common(pick(2));
}


void
cmd_right() {

    cmd_common(pick(3));
}
