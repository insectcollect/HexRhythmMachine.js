##HexRhythmMachine.js
[HexRhythmMachine.h](/src/HexRhythmMachine.h) based on [http://the.mohayonao.com/6chars/](http://the.mohayonao.com/6chars/)

###example
	open ./dst/index.html
	
###build
	cp ./src/main.mm ./src/main.cpp
	~/Development/emsdk/emscripten/tag-1.34.1/em++ \
	-O3 \
	-std=c++11 \
	-Wc++11-extensions \
	-s VERBOSE=1 \
	-s ASSERTIONS=1 \
	--memory-init-file 0 \
	-s EXPORTED_FUNCTIONS="['_setup','_next','_randamize']" \
	-I./src \
	-I./src/msp \
	-I./src/utils \
	-I./src/drum \
	./src/main.cpp \
	-o ./dst/HexRhythmMachine.js
	rm -f ./src/main.cpp
	
###licence
MIT LICENSE