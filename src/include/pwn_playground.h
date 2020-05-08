#pragma once

/* flush user input */
void flush();

void print_menu();

/* runs any required initialisation for modules used. If new modules are made any
 * initialisation calls they need should be made here
 */
void init_modules();

/* leak an address from a chosen segment in memory */
void leak_segment();

/* read a value from a user supplied address */
void arb_r();

/* write a user supplied value to a user supplied address */
void arb_w();

/* performs a fork and sends child process to the main menu loop after initialising modules
 * parent will simply wait on the child
 * if the child terminates 'normally' (determined by WIFEXITED), it is assumed the program should
 * end and exit() is called by the parent
 * if the child terminates another way, the parent re-forks
 */
void fork_handler();

/* main menu loop handling user input. Does not return */
void menu_loop();
