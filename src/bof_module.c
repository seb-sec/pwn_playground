#include <stdio.h>
#include "include/bof_module.h"

void overflow_module() {
	char buf[BUF_SIZE];
	printf("Place input into an 0x%lx sized buffer: ", sizeof(buf));
	gets(buf);
}
