control: control.o
	gcc -o control control.o
control.o: control.c
	gcc -c control.c
write: write.o
	gcc -o write write.o
write.o: write.c
	gcc -c write.c
clean:
	rm *.o