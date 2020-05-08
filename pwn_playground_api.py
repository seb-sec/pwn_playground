#!/bin/env python3
from pwn import *

# starts up the program- should be run from the same directory binary is in
# allows dictionary of environment variables to be given as an argument
# returns objects for the process, ELF and libc
def init_process(env_dict=None):
    PROGNAME = "./pwn_playground"
    p = process(PROGNAME, env=env_dict)
    elf = p.elf
    libc = elf.libc
    return p, elf, libc

# all functions below assume they are being called from the main menu
# if you expect to get code execution during one of these functions, the call
# to skip_menu will wait forever. An optional argument skip_m is included in some functions
# and should be set to False in this scenario- add this arg to more functions if required
# -------------------------------------------------------------------- #
def skip_menu(p):
    p.recvuntil(">\n")

# send a payload to the buffer overflow module
def send_overflow(p, payload, skip_m=True):
    p.sendline('1')
    p.sendline(payload)
    if skip_m:
        skip_menu(p)

# send a payload to the format string module
# returns the response
def send_fmt(p, payload, skip_m=True):
    p.sendline('2')
    p.sendlineafter("string:\n", payload)
    if skip_m:
        res = p.recvline()[:-1]     # cut newline char
        skip_menu(p)
        return res

# helper function to handle heap module menu
def enter_heap_menu(p):
    p.sendline('3')
    skip_menu(p)

# allocate a new heap chunk with given size
# returns the index of the allocated chunk
def alloc_heap_chunk(p, size, skip_m=True):
    enter_heap_menu(p)
    p.sendline('1')
    p.sendlineafter("chunk:\n", str(size))
    if skip_m:
        p.recvuntil("index ")
        idx = int(p.recvline()[:-1])
        skip_menu(p)
        return idx

# writes payload to chunk with given index
def write_heap_chunk(p, idx, payload):
    enter_heap_menu(p)
    p.sendline('2')
    p.sendlineafter("write to:\n", str(idx))
    p.sendlineafter(":\n", payload)
    skip_menu(p)

# reads from chunk at given index and returns the result
def read_heap_chunk(p, idx):
    enter_heap_menu(p)
    p.sendline('3')
    p.sendlineafter("print:\n", str(idx))
    p.recvuntil("contents: ")
    res = p.recvline()[:-1]     # cut newline char
    skip_menu(p)
    return res

# frees the heap chunk at given index
def free_heap_chunk(p, idx, skip_m=True):
    enter_heap_menu(p)
    p.sendline('4')
    p.sendlineafter("free:\n", str(idx))
    if skip_m:
        skip_menu(p)

# helper function to handle the FILE menu
def enter_file_menu(p):
    p.sendline('4')
    skip_menu(p)

# leaks the pointer to the FILE struct in use
def leak_file_fp(p):
    enter_file_menu(p)
    p.sendline('1')
    p.recvuntil('0x')
    res = int(p.recvline(), 16)
    skip_menu(p)
    return res

# all the basic FILE functions below may need to be modified to observe any desired effects
# for example- if using fwrite to perform an arbitrary read after corrupting the FILE struct
# code should be added to file_fwrite() below to return the value of interest

# performs an fwrite operation with the current FILE struct, optionally with supplied payload
def file_fwrite(p, payload='', skip_m=True):
    enter_file_menu(p)
    p.sendline('2')
    p.sendlineafter('input:\n', payload)
    if skip_m:
        skip_menu(p)

# performs an fread operation with the current FILE struct
def file_fread(p, skip_m=True):
    enter_file_menu(p)
    p.sendline('3')
    if skip_m:
        skip_menu(p)

# performs an fclose on the current FILE struct
def file_fclose(p, skip_m=True):
    enter_file_menu(p)
    p.sendline('4')
    if skip_m:
        skip_menu(p)

# writes over the FILE struct with the given payload
def forge_file_fp(p, payload):
    enter_file_menu(p)
    p.sendline('5')
    p.sendlineafter('struct:\n', payload)
    skip_menu(p)

# helper function to handle entering the leak menu
def enter_leak_menu(p):
    p.sendline('5')
    skip_menu(p)

# leaks the address of the menu_loop function in the binary
def leak_text_segment(p):
    enter_leak_menu(p)
    p.sendline('1')
    p.recvuntil('0x')
    res = int(p.recvline(),16)
    skip_menu(p)
    return res

# leaks address of heap chunk at given index- NOTE: this must be allocated
def leak_heap_address(p, idx):
    enter_leak_menu(p)
    p.sendline('2')
    p.sendlineafter("allocated):\n", str(idx))
    p.recvuntil('0x')
    res = int(p.recvline(), 16)
    skip_menu(p)
    return res

# leaks the address of _IO_2_1_stdin_ in libc
def leak_libc_segment(p):
    enter_leak_menu(p)
    p.sendline('3')
    p.recvuntil('0x')
    res = int(p.recvline(), 16)
    skip_menu(p)
    return res

# leaks a stack address: this will be a local variable in the leak_segment function
def leak_stack_segment(p):
    enter_leak_menu(p)
    p.sendline('4')
    p.recvuntil('0x')
    res = int(p.recvline(), 16)
    skip_menu(p)
    return res

# sets the base address of the text segment by using the leak functionality
def set_text_base(p, elf):
    menu_loop_addr = leak_text_segment(p)
    elf.address = menu_loop_addr - elf.symbols['menu_loop']

# sets the base of libc using the leak functionality
def set_libc_base(p, libc):
    stdin_addr = leak_libc_segment(p)
    libc.address = stdin_addr - libc.symbols['_IO_2_1_stdin_']

# perform a read at the given address
def arb_read(p, where):
    p.sendline('6')
    p.sendlineafter("value):\n", str(where))
    p.recvuntil(": 0x")
    res = int(p.recvline(), 16)
    skip_menu(p)
    return res

# perform arbitrary write of value 'what' to address 'where'
def arb_write_val(p, what, where, skip_m=True):
    p.sendline('7')
    payload = "{} {}".format(str(what), str(where))
    p.sendlineafter("values):\n", payload)
    if skip_m:
        skip_menu(p)

# as above but expects a string of appropriate size for the 'what' argument
def arb_write_str(p, str_what, where, skip_m=True):
    if type(str_what) == bytes:
        s = str_what
    elif type(str_what) == str:
        s = str_what.encode()
    else:
        log.error("bytes or a string as first argument")
    s = int(s.hex(), 16)
    arb_write_val(p, s, where, skip_m)

# writes an arbitrary size block of string or bytes starting at addr where
# elf object needed to determine architecture
def arb_write_block(p, elf, block, where, skip_m=True):
    arch_bytes = elf.bytes
    if type(block) == str:
        block = block.encode()
    # split block into list of chunks based on architecture bytes
    chunk_list = [ block[i:i+arch_bytes] for i in range(0, len(block), arch_bytes) ]
    cur_where = where
    for c in chunk_list:
        # rjust used as value is passed to scanf
        c = c.rjust(arch_bytes, b'\x00')
        arb_write_str(p, c, cur_where, skip_m)
        cur_where += arch_bytes

def start_fork_server(p):
    p.sendline('8')
    skip_menu(p)

def call_exit(p):
    p.sendline('9')
