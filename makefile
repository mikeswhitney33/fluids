COMPILE = g++ -std=c++11 -c -Ideps -Iinclude deps/glad.c
LINKS = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
CC = g++
MAKE_DIR = mkdir -p
MOVE_OBJ = mv *.o
INCLUDES = -Iinclude -Ideps deps/glad.c

MAIN_DIR = build/main
TRIANGLE_DIR = build/triangle

main:
	$(MAKE_DIR) $(MAIN_DIR)
	$(COMPILE) main.cpp
	$(MOVE_OBJ) $(MAIN_DIR)
	$(CC) -Iinclude -Ideps deps/glad.c $(MAIN_DIR)/main.o -o $(MAIN_DIR)/main $(LINKS)
	rm $(MAIN_DIR)/*.o

run_main:
	./$(MAIN_DIR)/main

triangle:
	$(MAKE_DIR) $(TRIANGLE_DIR)
	$(COMPILE) src/triangle_scene.cpp
	$(MOVE_OBJ) $(TRIANGLE_DIR)
	$(CC) $(INCLUDES) $(TRIANGLE_DIR)/triangle_scene.o -o $(TRIANGLE_DIR)/triangle $(LINKS)
	rm $(TRIANGLE_DIR)/*.o

run_triangle:
	./$(TRIANGLE_DIR)/triangle


clean:
	rm $(MAIN_DIR)/* $(TRIANGLE_DIR)/*
