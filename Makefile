flags = -Wall

all: microshell commands/bin colortest mycat mycp myls test

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