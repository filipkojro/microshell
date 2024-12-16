all: microshell colortest mycat mycp myls test

# SRCS = $(wildcard commands/*.c)

# test: $(SRCS)
# 	echo $(SRCS)

test: commands/bin | commands/test.c
	gcc -Wall -ansi -o commands/bin/test commands/test.c

microshell: microshell.c
	gcc -Wall -ansi -o microshell microshell.c

colortest: commands/bin | commands/colortest.c
	gcc -Wall -ansi -o commands/bin/colortest commands/colortest.c

mycat: commands/bin | commands/mycp.c
	gcc -Wall -ansi -o commands/bin/mycp commands/mycp.c

mycp: commands/bin | commands/mycat.c
	gcc -Wall -ansi -o commands/bin/mycat commands/mycat.c

myls: commands/bin | commands/myls.c
	gcc -Wall -ansi -o commands/bin/myls commands/myls.c

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
