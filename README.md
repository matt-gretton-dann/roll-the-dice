# roll-the-dice

Basic roll dice to get a target model for son

## License

Apache-2.0 - see LICENSE.md

## Compilation

Should be standard C++ - use your favourite compiler.

This was done too quickly for command line processing to have been added -
change the defines at the top of the source to change the behaviour.

`g++ roll-the-dice.cpp -o roll-the-dice -O2 -g`

## Usage

```
./roll-the-dice > rolls.csv
localc rolls.csv &
```

