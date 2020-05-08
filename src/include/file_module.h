#pragma once
#include <stdio.h>

FILE *fp;

/* initialise the FILE struct by opening a file */
void init_file_module();

/* leaks address of the FILE *fp */
void leak_fp();

/* performs an fwrite from user input into fp */
void do_fwrite();

/* performs an fread on fp into a buffer on the stack */
void do_fread();

/* calls fclose on fp */
void do_fclose();

/* allows user to overwrite the fp FILE struct with custom input */
void forge_fp();

void print_file_menu();

void file_menu();
