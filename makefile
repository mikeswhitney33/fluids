TARGETOS = $(shell uname -s)

MAKE_DIR = mkdir -p
CC = g++
INCLUDES = -Iinclude
C = -x c
CPP = -x c++
STB = include/stb_image.cpp
GLAD = deps/glad.c
DEPS = -Ideps



ifeq ($(TARGETOS), Darwin)
	FLAGS = -lglfw -lassimp
else
	FLAGS = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
endif

MAIN_DIR = build/main
TRIANGLE_DIR = build/triangle
CUBE_DIR = build/cube
TERRAIN_DIR = build/terrain

IN_FILE = main.cpp
OUT_FILE = -o $(MAIN_DIR)/main

COMPILE = $(CC) $(DEPS) $(INCLUDES) $(C) $(GLAD) $(CPP) $(STB) $(IN_FILE) $(OUT_FILE) $(FLAGS)

main:
	$(MAKE_DIR) $(MAIN_DIR)
	$(COMPILE)

run_main:
	./$(MAIN_DIR)/main

clean_main:
	rm $(MAIN_DIR)/*

triangle:
	$(MAKE_DIR) $(TRIANGLE_DIR)
	$(eval IN_FILE := src/triangle_scene.cpp)
	$(eval OUT_FILE := -o $(TRIANGLE_DIR)/triangle)
	$(COMPILE)

run_triangle:
	./$(TRIANGLE_DIR)/triangle

clean_triangle:
	rm $(TRIANGLE_DIR)/*

cube:
	$(MAKE_DIR) $(CUBE_DIR)
	$(eval IN_FILE := src/cube_scene.cpp)
	$(eval OUT_FILE := -o $(CUBE_DIR)/cube)
	$(COMPILE)

run_cube:
	./$(CUBE_DIR)/cube

clean_cube:
	rm $(CUBE_DIR)/*

terrain:
	$(MAKE_DIR) $(TERRAIN_DIR)
	$(eval IN_FILE := src/terrain_scene.cpp)
	$(eval OUT_FILE := -o $(TERRAIN_DIR)/terrain)
	$(COMPILE)

run_terrain:
	./$(TERRAIN_DIR)/terrain

clean_terrain:
	rm $(TERRAIN_DIR)/*

clean:
	rm $(MAIN_DIR)/* $(TRIANGLE_DIR)/* $(CUBE_DIR)/*
