#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "model.h"
#include "define.h"




void tests_run_all();
void test_layout_generation();
void ____test_layout_generation____check_sequence();
void ____test_layout_generation____check_answer_options_no_repeat();
void ____test_layout_generation____answer_options_set_must_contain_right_answer();
void print_colored(const char message[], WORD color);
void print_result(const char name[], const char description[], bool result);
