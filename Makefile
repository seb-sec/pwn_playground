# some different CFLAGS that may be of interest- add them to the EXTRA_FLAGS variable
#
# -fno-stack-protector 	: Disable stack canaries
# -fstack-protector 	: Enable stack canaries
#
#  -fPIE -pie 			: Enable Position Independent Execution (PIE) (off by default)
#
#  -z now			: Enable RELocation Read-Only (RELRO) (off by default)
#
#  -z execstack			: Enable executable stack segment (off by default)
#
#  -m32				: Compile 32-bit binary
#
#  -g				: Compile with debug symbols
#
#  -static			: Statically link libc- disables PIE
#
#  -static-pie			: Statically link libraries with PIE, must be specified with -fPIC
EXTRA_FLAGS			=
CC				= gcc
CFLAGS 				= -I$(PWD)/src -I$(PWD)/src/include $(EXTRA_FLAGS)

SRC_FILES = src/pwn_playground.c src/bof_module.c src/file_module.c src/fmt_string_module.c \
		src/heap_module.c

pwn_playground:	$(SRC_FILES)
	$(CC) $(CFLAGS) $? -o $@

.PHONY: clean
clean:
	rm pwn_playground
