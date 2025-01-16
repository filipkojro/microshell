all: microshell commands/bin colortest mycat mycp myls test

# SRCS = $(wildcard commands/*.c)

# test: $(SRCS)
# 	echo $(SRCS)

files := file1 file2
some_file:
	echo "Look at this variable: " $(files)

test: commands/test.c
	gcc -Wall -o commands/bin/test commands/test.c -I /opt/homebrew/opt/readline/include -L /opt/homebrew/opt/readline/lib -lreadline

microshell: microshell.c
	gcc -Wall -o microshell microshell.c

colortest: commands/colortest.c
	gcc -Wall -o commands/bin/colortest commands/colortest.c

mycat: commands/mycp.c
	gcc -Wall -o commands/bin/mycp commands/mycp.c

mycp: commands/mycat.c
	gcc -Wall -o commands/bin/mycat commands/mycat.c

myls: commands/myls.c
	gcc -Wall -o commands/bin/myls commands/myls.c

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
