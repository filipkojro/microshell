#include <stdio.h>

int main(){
    char* help_propmt = "Microshell\n"
                        "Autor: Filip Kojro\n"
                        "Wersja: ???:latest\n"
                        "Dostepne komendy:\n"
                        " - help\n"
                        " - exit\n"
                        " - myls\n"
                        " - mycp [SRC] [DST]\n"
                        " - mycd\n"
                        " - mycat\n"
                        " - myls [-l -a]\n"
                        " - microdocker run [COMMAND] [ARGUMENTS]\n"
                        " - httpserver [PORT] [HTTP_FILE_PATH]\n";
    printf("%s\n", help_propmt);
    return 0;
}