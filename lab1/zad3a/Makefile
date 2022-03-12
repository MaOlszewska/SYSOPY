CC = gcc -Wall -std=c11 -ggdb
DIR := ${CURDIR}


dynamic:
		make -C ../zad1/ shared
		$(CC) -c ../zad2/main.c -o main.o -ldl -D DYNAMIC

clean:
		make -C ../zad1/ clean
		@rm -rf *.o *.a *.so raport3a.txt main

tests:
		make test-static
		make test-shared
		make test_dynamic

test-static:
		make -C ../zad1/ static
		$(CC) -c ../zad2/main.c
		$(CC) main.o ../zad1/lib_counting.a -o main

		echo "\n---STATIC LIBRARY--- \n" >> raport3a.txt

		@echo "SMALL TESTS\n\n\n" >> raport3a.txt

		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt  remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small.txt remove: 0 count_files: small1.txt remove: 1 >> raport3a.txt

		@echo "\n\n\nMEDIUM TESTS \n\n\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt remove: 0 count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium1.txt medium3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt

		@echo "\n\n\nLARGE TESTS \n\n\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt medium2.txt large3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt

test-shared:
		make -C ../zad1/ shared
		$(CC) -c ../zad2/main.c

		echo "\n---SHARED LIBRARY---\n" >> raport3a.txt

		@echo "SMALL TESTS\n\n\n" >> raport3a.txt

		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt  remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small.txt remove: 0 count_files: small1.txt remove: 1 >> raport3a.txt

		@echo "\n\n\nMEDIUM TESTS \n\n\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt remove: 0 count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium1.txt medium3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt

		@echo "\n\n\nLARGE TESTS \n\n\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt medium2.txt large3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt

test_dynamic:
		make dynamic
		$(CC) -c ../zad2/main.c


		echo "\n---DYNAMIC LIBRARY---\n" >> raport3a.txt

		@echo "SMALL TESTS\n\n\n" >> raport3a.txt

		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small2.txt small3.txt count_files: small4.txt count_files: small1.txt small2.txt small3.txt small4.txt  remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: small.txt remove: 0 count_files: small1.txt remove: 1 >> raport3a.txt

		@echo "\n\n\nMEDIUM TESTS \n\n\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium3.txt remove: 0 count_files: medium1.txt medium2.txt medium3.txt medium1.txt medium2.txt medium1.txt medium3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt count_files: medium1.txt medium2.txt medium3.txt >> raport3a.txt

		@echo "\n\n\nLARGE TESTS \n\n\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt medium2.txt large3.txt >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 0 count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt
		@echo "\n------------------------\n" >> raport3a.txt
		./main count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt count_files: large1.txt large2.txt large3.txt large1.txt large2.txt large1.txt large2.txt large3.txt remove: 1 >> raport3a.txt



clean-library:
		rm -f *.so
		rm -f *.a