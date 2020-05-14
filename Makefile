js/main.js:
	/opt/cheerp/bin/clang++ -target cheerp-wasm main.cpp -o js/main.js -cheerp-wasm-enable=anyref -cheerp-secondary-output-path=js/main.wasm

serve: js/main.js
	python -m http.server 8080

open: js/main.js
	chromium --js-flags="--experimental-wasm-anyref" --user-data-dir=/tmp http://localhost:8080

all: js/main.js
