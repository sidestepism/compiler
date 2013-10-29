CC := gcc
CFLAGS := -Wall -g
SYNTREE_OBJS := util.o list.o tokenizer.o syntree.o char_buf.o parse.o env.o code_gen.o

all: cogen

cogen: $(SYNTREE_OBJS) syntree_demo.o
	$(CC) -o $@ $^

clean:
	rm -f $(SYNTREE_OBJS) syntree_demo.o

