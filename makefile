TARGETOS = $(shell uname -s)

MAKE_DIR = mkdir -p
CC = g++
INCLUDES = -Iinclude
C = -x c
CPP = -x c++

ifeq ($(TARGETOS), Darwin)
	DEPS = -Imac_deps
	GLAD = mac_deps/glad.c
	FLAGS = -lglfw -lassimp
else
	DEPS = -Ilinux_deps
	GLAD = linux_deps/glad.c
	FLAGS = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
endif

MAIN_DIR = build/main
TRIANGLE_DIR = build/triangle

main:
	$(MAKE_DIR) $(MAIN_DIR)
	$(CC) $(DEPS) $(INCLUDES) $(C) $(GLAD) $(CPP) main.cpp -o $(MAIN_DIR)/main $(FLAGS)

run_main:
	./$(MAIN_DIR)/main

clean_main:
	rm $(MAIN_DIR)/*

triangle:
	$(MAKE_DIR) $(TRIANGLE_DIR)
	$(CC) $(DEPS) $(INCLUDES) $(C) $(GLAD) $(CPP) src/triangle_scene.cpp -o $(TRIANGLE_DIR)/triangle $(FLAGS)

run_triangle:
	./$(TRIANGLE_DIR)/triangle

clean_triangle:
	rm $(TRIANGLE_DIR)/*

clean:
	rm $(MAIN_DIR)/* $(TRIANGLE_DIR)/*
