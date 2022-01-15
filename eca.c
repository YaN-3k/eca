/*
 * Elementary Cellular Automata
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	if(fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else fputc('\n', stderr);
	exit(1);
}

int
main(const int argc, const char *argv[])
{
	int *grid[2], gridsiz, gridp;
	int ruleset[1 << 3], rule, state;
	int genc, i, j, k;
	FILE *file;
	char byte;

	gridp = 0;
	file = NULL;
	memset(ruleset, 0, sizeof ruleset);

	if (argc < 4)
		die("usage: %s rule gridsiz genc [file.pbm]", argv[0]);


	rule = atoi(argv[1]);
	if ((rule == 0 && *argv[1] != '0') || rule < 0 || rule > 255)
		die("%s: invalid rule", argv[1]);

	gridsiz = atoi(argv[2]);
	if ((gridsiz == 0 && *argv[2] != '0') || gridsiz < 0)
		die("%s: invalid grid size", argv[2]);

	genc = atoi(argv[3]);
	if ((genc == 0 && *argv[3] != '0') || genc < 0)
		die("%s: invalid generation count", argv[3]);

	if (argc > 4) {
		file = fopen(argv[4], "w");
		if (!file) die("%:", argv[4]);
		fprintf(file, "P4 %d %d\n", gridsiz, genc);

	}

	for (i = 0; rule > 0; i++) {
		ruleset[i] = rule % 2;
		rule /= 2;
	}

	for (i = 0; i < 2; i++) {
		grid[i] = calloc(gridsiz, sizeof **grid);
		if (!grid[i]) die("calloc:");
	}

	grid[gridp][(gridsiz - 1) / 2] = 1;
	for (i = 0; i < genc; i++) {
		if (file) {
			byte = 0;
			for (j = 0; j < gridsiz; j++) {
				byte |= grid[gridp][j] << (7 - j % 8);
				if (j % 8 == 7) {
					fputc(byte, file);
					byte = 0;
				}
			}
			if (gridsiz % 8) fputc(byte, file);
		} else {
			putchar('|');
			for (j = 0; j < gridsiz; j++)
				putchar(" #"[grid[gridp][j]]);
			puts("|");
		}

		for (j = 0; j < gridsiz; j++) {
			state = 0;
			for (k = 0; k < 3; k++)
				state |= grid[gridp][(j + k) % gridsiz] << (2 - k);
			grid[gridp ^ 1][(j + 1) % gridsiz] = ruleset[state];
		}
		gridp ^= 1;
	}
	return 0;
}
