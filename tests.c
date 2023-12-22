#include "tests.h"
#include "case_fix.h"




void
test_layout_generation() {

    generate_new_layout();
    ____test_layout_generation____check_sequence();
    ____test_layout_generation____check_answer_options_no_repeat();
    ____test_layout_generation____answer_options_set_must_contain_right_answer();
}


void
____test_layout_generation____check_sequence() {

    char name[] = "sequence checking";
    char description[] = "must have all glyphs";
    bool map[KANA_SIZE];
    memset(&map, 0, sizeof(bool) * KANA_SIZE);
    for (int i = 0; i < KANA_SIZE; i ++) {
        map[model.sequence[i]] = TRUE;
    }
    bool result = TRUE;
    for (int i = 0; i < KANA_SIZE; i ++) {
        result &= map[i];
    }
    print_result(name, description, result);
}


void
____test_layout_generation____check_answer_options_no_repeat() {

    char name[] = "different answer options";
    char description[] = "answer options cannot be repeated in one set";
    bool result = TRUE;
    for (int i = 0; i < KANA_SIZE; i ++) {
        int map[KANA_SIZE];
        memset(&map, 0, sizeof(int) * KANA_SIZE);
        for (int j = 0; j < 4; j ++) {
            map[model.choices[i][j]] = 1;
        }
        int sum = 0;
        for (int x = 0; x < KANA_SIZE; x ++) {
            sum += map[x];
        }
        result &= (4 == sum);
    }
    print_result(name, description, result);
}


void
____test_layout_generation____answer_options_set_must_contain_right_answer() {

    char name[] = "right answer";
    char description[] = "answer options set must contain right answer";
    bool result = TRUE;
    for (int i = 0; i < KANA_SIZE; i ++) {
        int right = model.sequence[i];
        result &= (right == model.choices[i][0] || right == model.choices[i][1] ||
                   right == model.choices[i][2] || right == model.choices[i][3]);
    }
    print_result(name, description, result);
}


void
print_result(const char name[], const char description[], bool result) {

    print_colored(name, FOREGROUND_INTENSITY);
    if (result == TRUE) {
        print_colored(" - OK\n", FOREGROUND_GREEN);
        return;
    }
    print_colored(" - FAILED: ", FOREGROUND_RED);
    print_colored(description, FOREGROUND_INTENSITY);
    printf("\n");
}


void
print_colored(const char message[], WORD color) {

    HANDLE hStdOut=get_std_handle(STD_OUTPUT_HANDLE);
    DWORD num;
    set_console_text_attribute(hStdOut, color);
    write_console(hStdOut,message,strlen(message),&num, NULL);
}


void
tests_run_all() {

    test_layout_generation();
    print_colored("tests finished", FOREGROUND_BLUE);
}
