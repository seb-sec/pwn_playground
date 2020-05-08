#include <stdio.h>
#include "include/fmt_string_module.h"

void fmt_string_module() {
	char buf[FMT_BUF_SIZE];
	puts("Supply your format string:");
	fgets(buf, FMT_BUF_SIZE, stdin);
	printf(buf);
}
