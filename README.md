# microshell

## Run / build

### (linux / mac)
u need `gcc`, `make`
```sh
git clone https://github.com/filipkojro/microshell.git
cd microshell
make
tar -xzvf alpineimage.tar.gz
./microshell
```
### !!! to run microdocker u need to have root privileges and use whole path to microdocker `sudo {path to microshell}/commands/bin/microdocker` !!!

alpineimage compressed using
```sh
sudo tar -czvf alpineimage.tar.gz alpineimage
```

## losowe informacje
- obsluga ctrl+c
- historia w strzaklach
- propmpt (login + host + path)
- kolory :o
- obsluga bledow programow
- wykonywanie programow z PATH
- zamiana `~` na HOMEDIR i z powrotem
- kilka zaimplementowanych programow:
    - `mycd` (jest "alias" na `cd` bo sie czesto myli)
    - `myhelp`
    - `myls`  + flagi `-a` ,`-l`
    - `mycp [SRC] [DST]` dziala recursive
    - `mycat`
    - `colortest` ...?
    - `sudo ./commands/bin/microdocker run [COMMAND] [ARGUMENTS]` example: [COMMAND] = `python3`; [ARGUMENT] = `/home/main.py`
    - `httpserver [PORT] [HTTP_FILE_PATH]`

microshell based on https://www.youtube.com/watch?v=Utf-A4rODH8
