run:
	gcc -Wall -Wextra -Wpedantic -std=c11 main.c
	./a.out "source/cp1251.txt" "cp1251" "utf-8.txt"
	./a.out "source/iso-8859-5.txt" "iso-8859-5" "utf-8.txt"
	./a.out "source/koi8.txt" "koi8-r" "utf-8.txt"

verify:
	comm -23 utf-8.txt expected_result.txt

test:
	gcc -Wall -Wextra -Wpedantic -std=c11 main.c
	./a.out "input.txt" "test" "output.txt"
