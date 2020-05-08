#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "include/pwn_playground.h"
#include "include/bof_module.h"
#include "include/fmt_string_module.h"
#include "include/heap_module.h"
#include "include/file_module.h"

void flush(void) {
	char ch;
	while ((ch = getchar()) && ch != EOF && ch != '\n'){}
}

void print_menu() {
	puts("\n--- Menu ---");
	puts("1. Buffer overflow module");
	puts("2. Format string module");
	puts("3. Heap module");
	puts("4. FILE module");
	puts("5. Leak segment address");
	puts("6. Arbitrary read");
	puts("7. Arbitrary write");
	puts("8. Fork server");
	puts("9. Exit");
	puts(">");
}

/* initialisation calls for modules go here */
void init_modules() {
	init_heap_module();
	init_file_module();
}

void leak_segment() {
	int stack_var;
	puts("Enter segment to leak from list:");
	puts("1. .text");
	puts("2. heap");
	puts("3. libc");
	puts("4. stack");
	puts(">");
	int choice;
	scanf("%d", &choice);
	flush();
	switch (choice) {
		case 1:
			printf("Address of menu_loop function: %p\n", menu_loop);
			break;
		case 2:
			/* delegate this to heap module */
			leak_heap();
			break;
		case 3:
			printf("Address of stdin: %p\n", stdin);
			break;
		case 4:
			printf("Address of local stack variable: %p\n", &stack_var);
			break;
		default:
			puts("Invalid option");
	}
}

void arb_r() {
	puts("Enter an address to from (base 10 value):");
	unsigned long addr;
	scanf("%lu", &addr);
	flush();
	printf("Value at addr %p: 0x%lx\n", (void *)addr, *(long *)addr);
}

void arb_w() {
	puts("Enter arbitrary write in form '{value} {address}' (base 10 values):");
	unsigned long where;
	unsigned long what;
	scanf("%lu %lu", &what, &where);
	flush();
	printf("Writing 0x%lx to 0x%lx\n", what, where);
	*(long *)where = what;
}

void fork_handler() {
	while (1) {
		pid_t pid = fork();
		int wstatus;
		if (pid == -1) {
			puts("Fork failed!");
			exit(1);
		}
		/* child process */
		if (pid == 0) {
			puts("Child forked");
			init_modules();
			menu_loop();
		/* parent process */
		} else {
			if (wait(&wstatus) == -1) {
				puts("Parent failed to wait on child");
				exit(1);
			}
			/* if child terminated normally assume parent should also terminate */
			if (WIFEXITED(wstatus)) {
				exit(0);
			}
			/* otherwise child crashed and we re-fork */
			puts("Child crashed, reforking");
		}
	}
}

void menu_loop() {
	while (1) {
		print_menu();
		int choice;
		scanf("%d", &choice);
		flush();
		switch (choice) {
			case 1:
				overflow_module();
				break;
			case 2:
				fmt_string_module();
				break;
			case 3:
				heap_menu();
				break;
			case 4:
				file_menu();
				break;
			case 5:
				leak_segment();
				break;
			case 6:
				arb_r();
				break;
			case 7:
				arb_w();
				break;
			case 8:
				fork_handler();
				break;
			case 9:
				exit(0);
			default:
				puts("Invalid option");
		}
	}
}

int main() {
	init_modules();
	menu_loop();
	return 0;
}
