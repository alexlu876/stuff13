all: semcontrol.c story.c
	gcc -o control semcontrol.c
	gcc -o main story.c
clean:
	rm a.out
	rm *~
