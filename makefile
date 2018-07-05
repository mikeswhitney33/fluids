COMPILE = g++ -std=c++11 -c -Ideps -Iinclude deps/glad.c
LINKS = -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
CC = g++
MAKE_DIR = mkdir -p
MOVE_OBJ = mv *.o

MAIN_DIR = build/main

main:
	$(MAKE_DIR) $(MAIN_DIR)
	$(COMPILE) main.cpp
	$(MOVE_OBJ) $(MAIN_DIR)
	$(CC) -Iinclude -Ideps deps/glad.c $(MAIN_DIR)/main.o -o $(MAIN_DIR)/main $(LINKS)
	rm $(MAIN_DIR)/*.o

clean:
	rm $(MAIN_DIR)/*

run_main:
	./$(MAIN_DIR)/main
