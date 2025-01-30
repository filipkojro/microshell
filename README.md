# microshell

## Run / build

### Building and running (linux / mac)
u need `gcc`, `make`
```sh
git clone https://github.com/filipkojro/microshell.git
cd microshell
make
tar -xzvf alpineimage.tar.gz
./microshell
```
### !!! to run microdocker inside microshell u need to run microshell using sudo `sudo ./commands/bin/microshell` or root user !!!

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
    - `mycat`
    - `colortest` ...?
    - `sudo ./commands/bin/microdocker run [COMMAND] [ARGUMENTS]` example: [COMMAND] = `python3`; [ARGUMENT] = `/home/main.py`
    - `httpserver [PORT] [HTTP_FILE_PATH]`

microshell based on https://www.youtube.com/watch?v=Utf-A4rODH8