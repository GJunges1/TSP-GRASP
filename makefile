greedy: greedy.c
	gcc greedy.c -o greedy -lm
stocastic: stocastic.c
	gcc stocastic.c -o stocastic -lm -lsodium
local: local.c
	gcc local.c -o local -lm
grasp: grasp.c
	gcc grasp.c -o grasp -lm -lsodium
run_greedy: greedy
	echo "running route search for all instances...\n"; \
	[ ! -f tsp_output.html ] || mv tsp_output.html tsp_output.old.html ; \
	for OUTPUT in $$(ls instances/*.tsp); \
	do echo -n $$OUTPUT ".."; \
	printf "<details><summary>%s</summary><p><blockquote>\n" $$OUTPUT >> tsp_output.html; \
	./greedy<$$OUTPUT>>tsp_output.html; \
	printf "</blockquote></details><p>\n" >> tsp_output.html; \
	echo "OK\n"; \
	done; \
	echo "all instances have been runned!\ncheck tsp_output.html for results"; \
	microsoft-edge-stable tsp_output.html;

run_grasp: grasp
	echo "running route search for all instances...\n"; \
	[ ! -f tsp_output.html ] || mv tsp_output.html tsp_output.old.html ; \
	for OUTPUT in $$(ls instances/*.tsp); \
	do echo -n $$OUTPUT ".."; \
	printf "<details><summary>%s</summary><p><blockquote>\n" $$OUTPUT >> tsp_output.html; \
	./grasp<$$OUTPUT>>tsp_output.html; \
	printf "</blockquote></details><p>\n" >> tsp_output.html; \
	echo "OK\n"; \
	done; \
	echo "all instances have been runned!\ncheck tsp_output.html for results"; \
	microsoft-edge-stable tsp_output.html;

run_stocastic: stocastic
	echo "running route search for all instances...\n"; \
	[ ! -f tsp_output.html ] || mv tsp_output.html tsp_output.old.html ; \
	for OUTPUT in $$(ls instances/*.tsp); \
	do echo -n $$OUTPUT ".."; \
	printf "<details><summary>%s</summary><p><blockquote>\n" $$OUTPUT >> tsp_output.html; \
	./stocastic<$$OUTPUT>>tsp_output.html; \
	printf "</blockquote></details><p>\n" >> tsp_output.html; \
	echo "OK\n"; \
	done; \
	echo "all instances have been runned!\ncheck tsp_output.html for results"; \
	microsoft-edge-stable tsp_output.html;

run_local: local
	echo "running route search for all instances...\n"; \
	[ ! -f tsp_output.html ] || mv tsp_output.html tsp_output.old.html ; \
	for OUTPUT in $$(ls instances/*.tsp); \
	do echo -n $$OUTPUT ".."; \
	printf "<details><summary>%s</summary><p><blockquote>\n" $$OUTPUT >> tsp_output.html; \
	./local<$$OUTPUT>>tsp_output.html; \
	printf "</blockquote></details><p>\n" >> tsp_output.html; \
	echo "OK\n"; \
	done; \
	echo "all instances have been runned!\ncheck tsp_output.html for results"; \
	microsoft-edge-stable tsp_output.html;

# one: greedy
# 	./greedy<instances/rat99.tsp
clear:
	rm -f greedy stocastic local grasp
# all: greedy.c
# 	make clear greedy run
