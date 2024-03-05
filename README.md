# README

## Struttura del Progetto
- `kcomp.cpp`: Il file principale del compilatore.
- `driver.hpp`: Definisce la classe `driver` e le classi AST
- `driver.cpp`: Implementa le funzionalità di `driver` e delle classi AST
- `parser.yy`: File della grammatica Bison per il parser.
- `scanner.ll`: File Flex/Lex per lo scanner.

## Compilazione con Makefile

Per la compilazione, sono disponibili due Makefile distinti, uno per macOS e uno per Linux. Ovviamente, è necessario scegliere il Makefile appropriato in base al sistema operativo utilizzato.

### Comandi Makefile
Per eseguire i comandi di compilazione, assicurarsi di specificare il Makefile appropriato usando l'opzione `-f` con il comando `make`. Ad esempio, per utilizzare il Makefile per Linux, si digiti:

```bash
make -f Makefile.Linux [comando]
```

#### Comandi Disponibili
- `make -f [Makefile] all`: Compila tutti i componenti necessari per creare l'eseguibile `kcomp`.
- `make -f [Makefile] clean`: Rimuove tutti i file generati, inclusi gli eseguibili.
- `make -f [Makefile] clean_obj`: Rimuove solo i file oggetto


## Utilizzo
Per utilizzare kcomp, eseguire il seguente comando dalla riga di comando:

```
kcomp [opzioni] <file.k>
```

Dove `<file.k>` è il file sorgente Kaleidoscope da compilare. Le opzioni disponibili sono:

- `-p`: Abilita le tracce di debug nel parser.
- `-s`: Abilita le tracce di debug nello scanner.

Esempio:

```bash
kcomp -p -s esempio.k
```

## Test
Per comodità vengono già forniti tutti i programmi di test presenti nella cartella `test_progetto`. Nella cartella è presente un makefile che chiama il compilatore con il compando `../kcomp` e compila il file `.ll` generato e infine fa il linking con i file `.cpp` presenti nella cartella genrando gli eseguibili dei test.

## Note Aggiuntive
- Il compilatore KComp genera l'IR sullo `stderr`.
- Per ottenere l'output del compilatore, è necessario utilizzare il comando `2>` per reindirizzare lo `stderr` su un file `.ll`:
  - `kcomp esempio.k 2> esempio.ll`