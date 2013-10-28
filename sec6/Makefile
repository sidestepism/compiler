CC := clang
CFLAGS := -Wall -g
SYNTREE_OBJS := util.o list.o tokenizer.o syntree.o char_buf.o parse.o code_gen.o

all: syntree_demo

syntree_demo: $(SYNTREE_OBJS) syntree_demo.o
	$(CC) -o $@ $^

clean:
	rm -f $(SYNTREE_OBJS) syntree_demo.o

