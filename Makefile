flags = -I /opt/homebrew/opt/readline/include -L /opt/homebrew/opt/readline/lib -lreadline -Wall

all: microshell commands/bin colortest mycat mycp myls test

# SRCS = $(wildcard commands/*.c)

# test: $(SRCS)
# 	echo $(SRCS)

files := file1 file2
some_file:
	echo "Look at this variable: " $(files)

test: commands/test.c
	gcc -o commands/bin/test commands/test.c $(flags)

microshell: microshell.c
	gcc -o microshell microshell.c $(flags)

colortest: commands/colortest.c
	gcc -o commands/bin/colortest commands/colortest.c $(flags)

mycat: commands/mycp.c
	gcc -o commands/bin/mycp commands/mycp.c $(flags)

mycp: commands/mycat.c
	gcc -o commands/bin/mycat commands/mycat.c $(flags)

myls: commands/myls.c
	gcc -o commands/bin/myls commands/myls.c $(flags)

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
