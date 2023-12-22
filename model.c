#include "model.h"




void
generate_new_layout() {

    model.pointer = 0;
    model.faults = 0;
    choices_unlock_all();
    generate_new_sequence();
    generate_answers();
}


void
generate_new_sequence() {

    for (int i = 0; i < KANA_SIZE; i ++) {
        model.sequence[i] = i;
    }
    shuffle(model.sequence, KANA_SIZE);
}


void
generate_answers() {

    for (int i = 0; i < KANA_SIZE; i ++) {
        int right = model.sequence[i];
        int selection[KANA_SIZE - 1];
        for (int j = 0, p = 0; j < KANA_SIZE - 1; j ++, p ++) {
            if (model.sequence[j] == right) p ++;
            selection[j] = model.sequence[p];
        }
        shuffle(selection, KANA_SIZE - 1);
        model.choices[i][0] = right;
        model.choices[i][1] = selection[1];
        model.choices[i][2] = selection[2];
        model.choices[i][3] = selection[3];
        shuffle(model.choices[i], 4);
    }
}


void
print_sequence() {

        for (int i = 0; i < KANA_SIZE; i ++) {
        printf("%d - %d %d %d %d\n",
               model.sequence[i],
               model.choices[i][0],
               model.choices[i][1],
               model.choices[i][2],
               model.choices[i][3]);
    }
}


int
pick(int option) {

    if (model.choices[model.pointer][option] == model.sequence[model.pointer]) {
        model.pointer ++;
        if (model.pointer == KANA_SIZE) {
            choises_lock_all();
            model.pointer = KANA_SIZE - 1;
            return END;
        }
        choices_unlock_all();
        return 1;
    }
    if (model.choice_enable[option]) {
        model.choice_enable[option] = FALSE;
        model.faults ++;
        return -1;
    }
    return 0;
}


void
shuffle(int array[], size_t n) {

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i ++) {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


void
choices_unlock_all() {

    for (int i = 0; i < 4; i ++) {
        model.choice_enable[i] = TRUE;
    }
}


void
choises_lock_all() {

    memset(model.choice_enable, 0, sizeof(bool) * 4);
}


const char * kana[] = {"a",   "i",   "u",   "e",   "o",
                       "ka",  "ki",  "ku",  "ke",  "ko",
                       "sa",  "shi", "su",  "se",  "so",
                       "ta",  "chi", "tsu", "te",  "to",
                       "na",  "ni",  "nu",  "ne",  "no",
                       "ha",  "hi",  "fu",  "he",  "ho",
                       "ma",  "mi",  "mu",  "me",  "mo",
                       "ya",         "yu",         "yo",
                       "ra",  "ri",  "ru",  "re",  "ro",
                       "wa",         "wo",          "n",
                       "ga",  "gi",  "gu",  "ge",  "go",
                       "za",  "ji",  "zu",  "ze",  "zo",
                       "da",  "dji", "dzu", "de",  "do",
                       "ba",  "bi",  "bu",  "be",  "bo",
                       "pa",  "pi",  "pu",  "pe",  "po",
                       "kya",        "kyu",       "kyo",
                       "sha",        "shu",       "sho",
                       "cha",        "chu",       "cho",
                       "nya",        "nyu",       "nyo",
                       "hya",        "hyu",       "hyo",
                       "mya",        "myu",       "myo",
                       "rya",        "ryu",       "ryo",
                       "gya",        "gyu",       "gyo",
                       "ja",         "ju",        "jo",
                       "dja",        "dju",       "djo",
                       "bya",        "byu",       "byo",
                       "pya",        "pyu",       "pyo"};
