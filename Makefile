all: microshell commands

microshell: microshell.c
	gcc -Wall -ansi -o microshell microshell.c

commands: commands/bin | commands/test.c
	gcc -Wall -ansi -o commands/bin/test commands/test.c

commands/bin:
	mkdir -p commands/bin

clean:
	rm -f *.o microshell
	rm -rf commands/bin