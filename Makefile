all: polygonDisplay boundaries
	gcc -o boundaries boundaries.o polygonDisplay.o -lX11

polygonDisplay: polygonDisplay.c polygonDisplay.h
	gcc -c polygonDisplay.c

boundaries: boundaries.c polygonSet.h
	gcc -c boundaries.c

clean:
	rm -f *.o *~ boundaries
