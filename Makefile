all: microshell commands/bin colortest mycat mycp myls myhelp microdocker httpserver test

test: commands/test.c
	gcc -o commands/bin/test commands/test.c

microshell: microshell.c
	gcc -o microshell microshell.c

colortest: commands/colortest.c
	gcc -o commands/bin/colortest commands/colortest.c

mycat: commands/mycp.c
	gcc -o commands/bin/mycp commands/mycp.c

mycp: commands/mycat.c
	gcc -o commands/bin/mycat commands/mycat.c

myls: commands/myls.c
	gcc -o commands/bin/myls commands/myls.c

myhelp: commands/myhelp.c
	gcc -o commands/bin/myhelp commands/myhelp.c

microdocker: commands/microdocker.c
	gcc -o commands/bin/microdocker commands/microdocker.c

httpserver: commands/httpserver.c
	gcc -o commands/bin/httpserver commands/httpserver.c

commands/bin:
	mkdir -p commands/bin

clean:
	rm -f *.o microshell
	rm -rf commands/bin