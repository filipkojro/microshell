all: microshell test mycat mycp

# SRCS = $(wildcard commands/*.c)

# test: $(SRCS)
# 	echo $(SRCS)

microshell: microshell.c
	gcc -Wall -ansi -o microshell microshell.c

test: commands/bin | commands/test.c
	gcc -Wall -ansi -o commands/bin/test commands/test.c

mycat: commands/bin | commands/mycp.c
	gcc -Wall -ansi -o commands/bin/mycp commands/mycp.c

mycp: commands/bin | commands/mycat.c
	gcc -Wall -ansi -o commands/bin/mycat commands/mycat.c

commands/bin:
	mkdir -p commands/bin

clean:
	rm -f *.o microshell
	rm -rf commands/bin



# SRC=$(wildcard *.c)
# EXEC=$(SRC:.c=)

# all: $(EXEC)

# %: %.c
#     gcc -o $@ $<
