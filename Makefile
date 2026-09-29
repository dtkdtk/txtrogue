SRCS = main.cpp res_init_nodes.cc res_init_tiles.cc

all: program

program: $(SRCS)
	clang++ $(SRCS) -o program -std=c++20

sympalette:
	clang++ tools/sympalette.cpp -o sympalette -std=c++20

clean:
	rm -f program

