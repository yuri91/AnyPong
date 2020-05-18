# AnyPong

Simple example of using the experimental anyref feature in Cheerp.

This is a simple pong-like game using Three.js.

The Three.js API needed by the game is declared in `three.h`, and the game logic
is in `main.cpp`.

The highlight of the example is that all the logic is compiled to Wasm, and uses
the "anyref" feature to interact with the Three.js library and the DOM.

## Compile and run

compile:
```
make
```

serve locally (do this in a separate tab):
```
make serve
```

open in the browser (special flag needed at the moment, included in the script):
```
make open
```

##  Acknowledgements

The code is heavily inspired by this Three.js tutorial: http://embed.plnkr.co/zg1iEjXtv0krpPSgkWe3/
