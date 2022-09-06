greedy: greedy.c
	gcc greedy.c -o greedy -lm
run:
	for OUTPUT in $$(ls instances/*.tsp); do ./greedy<$$OUTPUT>$$OUTPUT.out; done
clear:
	rm instances/*.out greedy
all: greedy.c
	make clear greedy run
