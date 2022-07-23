#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PROGRAM_SIZE 65535 // 2¹⁶ bytes
#define SBUF_SIZE 8192     // 8KiB

size_t file_get_size(FILE *stream)
{
	size_t res;

	fseek(stream, 0, SEEK_END);
	res = ftell(stream);
	rewind(stream);

	return res;
}

char const * bf_read(unsigned char const *fname)
{
	static char  static_buffer[SBUF_SIZE];
	char 		*bufptr = static_buffer;
	size_t		 nread;
	size_t		 fsize;
	FILE		*f;

	if ((f = fopen(fname, "r")) == NULL)
		return NULL;

	if ((fsize = file_get_size(f)) + 1 > SBUF_SIZE)
		if ((bufptr = malloc(fsize + 1)) == NULL)
			return NULL;

	nread = fread(bufptr, sizeof *bufptr, fsize, f);
	bufptr[nread] = '\0';

	if (nread < fsize && ferror(f))
		perror("bf_read");

	fclose(f);

	return bufptr;
}

void bf_free(char const *buffer)
{
	size_t buf_size = strlen(buffer) + 1;

	if (buf_size > SBUF_SIZE)
		free(buffer);
}

// TODO:
// Having a function to compile the bf data has the advantage of code generation being done ahead of time.
// This makes runtime go much faster since the executor doesn't need to know anything about the original code or its structure to execute it.
void bf_compile(char const *buffer)
{
}

// I think switches are ugly but they're really fast due to them compiling to jump tables.
// It's a fair trade and I think my way of writing them in this fashion with an opcode table make them more readable.
void bf_execute(char *buffer)
{
	char const opcode;
	uint8_t    pc;

	switch (opcode)
	{
		case '>':
			++*buffer;
			break;

		case '<':
			--*buffer;
			break;

		case '+':
			++pc;
			break;

		case '-':
			--pc;
			break;

		case '.':
			putchar(pc);
			break;

		case ',':
			*buffer = getchar();
			break;

		//TODO
		//I still haven't really figured out how to do the jumps
	}
}

int main(int argc, char **argv)
{
	if (argc < 2)
		puts("Insufficient amount of arguments. Please provide a brainfuck file for interpretation.");

	char const *buffer = bf_read(argv[1]);

	return 0;
}
