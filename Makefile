all: build

build:
	@echo "Building"
	mkdir -p bin
	g++ -g src/main.cpp vendor/**/*.cpp vendor/**/*.c \
		-Isrc -Ivendor -Ivendor/lzma \
		-lGLEW -lGLU -lglfw3 -ldl -lpthread -lGL -lrenderer \
		-o bin/renderer.out

install: build
	cp bin/* /usr/bin

clean:
	rm -r bin/*
