all: build

build:
	@echo "Building"
	mkdir -p bin
	clang++ -std=c++17 -g \
		src/main.cpp vendor/**/*.cpp vendor/lzma/*.c \
		-Isrc -Ivendor \
		-lGLEW -lGLU -lglfw3 -ldl -lpthread -lGL -lrenderer \
		-o bin/renderer.out

install: build
	cp bin/* /usr/bin

clean:
	rm -r bin/*
