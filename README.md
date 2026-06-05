# TryndLang
C++ implementation of the Java tree-walk interpreter for Lox from [Crafting Interpreters](https://craftinginterpreters.com/).  
*(Yes, it's named after my [league main](https://wiki.leagueoflegends.com/en-us/Tryndamere), very uninspiring name.)*

The syntax can be found [here](https://craftinginterpreters.com/the-lox-language.html). All is the same other than `fun` is actually `func` in this implementation. Everything in chapters 4-10 are implemented, meaning the last 3 are not (classes, inheritance, resolving/binding).

## Build

```bash
git clone https://github.com/mattman4/TryndLang.git
cd TryndLang
mkdir build && cd build
cmake ..
make
```

## Usage
```bash
./trynd [<file>]
```
- `<file>` replace with any file e.g. `../examples/test.trynd`
- If no file is provided, the interpreter runs interactively where you can run code 1 line at a time.