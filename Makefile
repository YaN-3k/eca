CPPFLAGS =
CFLAGS   = -g -ansi -pedantic -Wextra -Wall ${CPPFLAGS}
LDFLAGS  =

SRC = eca.c
OBJ = ${SRC:.c=.o}

all: options eca

options:
	@echo eca build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

eca: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f eca ${OBJ}

.PHONY: all options clean
