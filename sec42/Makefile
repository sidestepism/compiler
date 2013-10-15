CC=cc
CFLAGS=-g
OBJS=tokenizer.o tokenizer_main.o
tokenizer : $(OBJS)
	gcc -g -o $@ $(OBJS)
clean :
	rm -f $(OBJS)
test :
	./minicalc tokenizer_test/00.txt > tokenizer_test/00.res && diff tokenizer_test/00.res tokenizer_test/00.ans
	./minicalc tokenizer_test/01.txt > tokenizer_test/01.res && diff tokenizer_test/01.res tokenizer_test/01.ans
	./minicalc tokenizer_test/02.txt > tokenizer_test/02.res && diff tokenizer_test/02.res tokenizer_test/02.ans
	./minicalc tokenizer_test/03.txt > tokenizer_test/03.res && diff tokenizer_test/03.res tokenizer_test/03.ans
	./minicalc tokenizer_test/04.txt > tokenizer_test/04.res && diff tokenizer_test/04.res tokenizer_test/04.ans
