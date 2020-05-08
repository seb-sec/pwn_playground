## pwn-playground

Often in ctfs you are restricted in the environment you are given and exploitation techniques at your disposal. Being able to adapt to that environment and still exploit the vulnerable program under the given restrictions is a fun challenge, but I thought it would be beneficial (particularly to beginners) to be able to practice different types of exploitation in an environment without many restrictions, at the same time gaining more knowledge of how the internals of programs work.

This is what this small educational project seeks to provide (at least for exploiting C programs). Users can quickly develop proof of concept exploits that work under different self imposed scenarios- for example, trying to pop a shell with a classic buffer overflow given only a libc leak and an arbitrary write (with xyz protections enabled, with a specific libc version, etc).

Provided is a python file (pwn-playground-api.py) that contains a few useful functions that can be used as an API for interacting with the program using [pwntools](http://docs.pwntools.com/en/stable/). This designed so that users can focus on hacking rather than program interaction.

<br>

tldr the main idea is to allow a user to jump into trying out some new exploit idea/technique without needing to set up a proof of concept source file

<br>

<hr>

### Configuration/Building

A simple Makefile is provided, with comments specifying different CFLAGS the user might be interested in (mostly how to enable/disable specific protections). Modify this file however you please, then run `make` to build the binary `pwn_playground`

<br>

<hr>

### Usage

After modifying the Makefile and compiling the binary, construct a pwntools script that uses the functions in pwn_playground_api.py to interact with the program. Read through this API to get a feeling of what interactions it handles.

See the 'Overview of features' section below for some of the provided features of the program.

Additionally, see exploit-example.py for a simple commented example of how the interaction API can be used.

<br>

<hr>

### Overview of features

Here is a brief overview of the current program features:

1. Buffer overflow module
  * Simple buffer overflow, with gets() called on a buffer of limited size
2. Format string module
  * Simple module that calls printf() on user supplied input
3. Heap module
  * Collection of functions to create, read from, write to, and free heap chunks
4. FILE module
  * Collection of functions to perform different FILE operations on a FILE *fp, as well as ability to corrupt the FILE struct it points to
5. Leak segment address
  * Functionality to leak an address from a number of segments in process memory (.text, heap, libc, stack)
6. Arbitrary read
  * Functionality to read at a user supplied address
7. Arbitrary write
  * Fuctionality to write a user supplied value to an arbitrary address
8. Fork server
  * Performs a fork() and sends child to the main program loop while the parent waits. If the child crashes, the parent re-forks and waits again.

<br>

More information as well as helpful resources for each topic is given [here](https://seb-sec.github.io/2020/05/06/pwn-playground.html)

<br>

### Additional Notes

If you wish to change the version of libc used by the program (e.g. for exploiting different heap protection mechanisms), two options that you might also find useful for traditional ctf challenges are to use `LD_PRELOAD` or `patchelf`.

#### LD_PRELOAD

Run the binary with the following:

```
$ LD_PRELOAD="/path/to/libc" ./pwn_playground
```

Any normal exploit script used must also pass an environment dictionary to the `process()` function

```python
p = process("./pwn_playground", env={"LD_PRELOAD":"/path/to/libc"})
```

If using the init_process function in the provided API, pass it the env dictionary as an argument instead.

#### patchelf

You can point the ELF to use the desired library by using:
```
$ patchelf --add-needed /path/to/libc ./pwn_playground
```

This will take precendence over the default libc.so.6 (see `patchelf --print-needed ./pwn_playground`), but you could replace the default entirely it if you wish with `patchelf --replace-needed libc.so.6 /path/to/libc ./pwn_playground`

