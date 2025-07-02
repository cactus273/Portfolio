EXE=translate

$(EXE): main.c task1.c task1.h task2.c task2.h task3.c task3.h task4.c task4.h
	gcc -Wall -o $(EXE) main.c task1.c task2.c task3.c task4.c
format:
	clang-format -style=file -i *.c

clean:
	rm -f $(EXE)