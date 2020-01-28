src = $(shell find -not -path '*/\.*' -type f -name '*.c')
obj = $(src:.c=.o)

blackjack.o: $(obj)
	g++ -g -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) blackjack.o
