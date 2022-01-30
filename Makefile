EXEC   = clg
CFLAGS = -Wall -Wextra -std=c11 -pedantic
SRC    = $(wildcard src/*.c)
LIBS   = -lm

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LIBS)
