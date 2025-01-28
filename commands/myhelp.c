#include <stdio.h>

int main(){
    char* help_propmt = "Microshell\n"
                        "Autor: Filip Kojro\n"
                        "Wersja: ???:latest\n"
                        "Dostepne komendy:\n"
                        " - help\n"
                        " - exit\n"
                        " - myls\n"
                        " - mycd\n"
                        " - mycat\n"
                        " - myls\n";
    printf("%s\n", help_propmt);
    return 0;
}