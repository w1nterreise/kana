#include <stdio.h>
#include <stdlib.h>
#include "view.h"
#include "case_fix.h"




#ifdef TEST
#include "tests.h"
int
main() {

    tests_run_all();
    return 0;
}
#else


void
hide_console() {

    HWND hide_console;
    alloc_console();
    hide_console = find_window_a("ConsoleWindowClass", NULL);
    show_window(hide_console, 0);
}


int
main() {

    hide_console();
    load_bitmaps();

    if (!assert_bitmaps_loaded()) {
        printf("failed to load images!");
        return -1;
    }

    init_window();
    init_controls();
    show_window(form.hWnd, SW_SHOWNORMAL);
    generate_new_layout();
    update();

    MSG msg;
    while(get_message(&msg, NULL, 0, 0))
        dispatch_message(&msg);

    return 0;
}


#endif
