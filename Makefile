js/main.js:
	/opt/cheerp/bin/clang++ -target cheerp-wasm main.cpp -o js/main.js -cheerp-wasm-enable=anyref -cheerp-secondary-output-path=js/main.wasm

serve: js/main.js
	python -m http.server 8080

open: js/main.js
	chromium --js-flags="--experimental-wasm-anyref"

all: js/main.js
