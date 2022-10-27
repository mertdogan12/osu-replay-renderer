all: build

build:
	@echo "Building"
	mkdir -p bin
	g++ -g \
		src/main.cpp vendor/**/*.cpp vendor/lzma/*.c \
		-Isrc -Ivendor \
		-lGLEW -lGLU -lglfw3 -ldl -lpthread -lGL -lrenderer \
		-o bin/renderer.out

build-log:
	@echo "Building with loggin (log.txt)"
	mkdir -p bin
	g++ -g \
		src/main.cpp src/**/*.cpp vendor/**/*.cpp vendor/lzma/*.c \
		-Isrc -Ivendor \
		-lGLEW -lGLU -lglfw3 -ldl -lpthread -lGL -lrenderer \
		-o bin/renderer.out 2> log.txt

install: build
	cp bin/* /usr/bin

clean:
	rm -r bin/*
