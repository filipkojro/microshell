# microshell

## Run / build

### Building and running (linux / mac)
u need `gcc`, `make`
```sh
git clone https://github.com/filipkojro/microshell.git
cd microshell
make
./microshell
```

## losowe informacje
- obsluga ctrl+c
- historia w strzaklach
- propmpt (login + host + path)
- kolory :o
- obsluga bledow programow
- wykonywanie programow z PATH
- kilka zaimplementowanych programow:
    - mycd (jest "alias" na cd bo sie czesto myli)
    - myhelp
    - myls  + flagi `-a` ,`-l`
    - mycat
    - colortest ...?
- zamiana `~` na HOMEDIR i z powrotem