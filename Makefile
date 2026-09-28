SRCS = main.cpp res_init_nodes.cc res_init_tiles.cc

all: program

program: $(SRCS)
	clang++ $(SRCS) -o program

clean:
	rm -f program

