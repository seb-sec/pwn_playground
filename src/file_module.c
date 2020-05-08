#include <stdio.h>
#include <stdlib.h>
#include "include/file_module.h"
#include "include/pwn_playground.h"

void init_file_module() {
	fp = fopen("./flag.txt", "rw");
	if (fp == NULL) {
		puts("file_module: Failed to open file!");
		exit(1);
	}
}

void leak_fp() {
	printf("FILE *fp = %p\n", fp);
}

void do_fwrite() {
	puts("Performing fwrite()");
	char buf[0x30];
	puts("Enter input:");
	fgets(buf, 0x30, stdin);
	fwrite(buf, 0x30, 1, fp);
}

void do_fread() {
	puts("Performing fread()");
	char buf[0x30];
	fread(buf, 0x30, 1, fp);
}

void do_fclose() {
	puts("Closing FILE *fp");
	int ret = fclose(fp);
	if (ret != 0) {
		puts("Warning: Unsuccessful fclose()");
	}
}

void forge_fp() {
	puts("Provide your custom FILE struct:");
	gets((char *) fp);
	puts("FILE *fp overwritten");
}

void print_file_menu() {
	puts("\n--- FILE module ---");
	puts("1. Leak FILE *fp");
	puts("2. do fwrite");
	puts("3. do fread");
	puts("4. do fclose");
	puts("5. forge FILE *fp");
	puts("6. Exit FILE module");
	puts(">");
}

void file_menu() {
	print_file_menu();
	int choice;
	scanf("%d", &choice);
	flush();
	switch (choice) {
		case 1:
			leak_fp();
			break;
		case 2:
			do_fwrite();
			break;
		case 3:
			do_fread();
			break;
		case 4:
			do_fclose();
			break;
		case 5:
			forge_fp();
			break;
		case 6:
			return;
		default:
			puts("Invalid option");
	}
}
