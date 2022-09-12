greedy: greedy.c
	gcc greedy.c -o greedy -lm
run:
	[ ! -f tsp.output ] || mv tsp.output tsp.output.old ; \
	for OUTPUT in $$(ls instances/*.tsp); \
	do printf "INPUT FILE: %s\n" $$OUTPUT >> tsp.output; \
	./greedy<$$OUTPUT>>tsp.output; \
	printf "\n" >> tsp.output; \
	done
clear:
	rm -f greedy
all: greedy.c
	make clear greedy run
