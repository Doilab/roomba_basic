CC = g++
CC_MINGW32 = c:\Program Files (x86)\CodeBlocks\MinGW\bin\g++
CC_MINGW64 = c:\Program Files\CodeBlocks\MinGW\bin\g++
SRC = roomba_test.cpp serial.cpp
WIN_LIB32 := mingw32_lib/libfreeglut/libfreeglut.a
WIN_LIB64 := mingw32_lib/libfreeglut/x64/freeglut.lib

all:linux

win64:
	$(CC_MINGW64) -o roomba_test_d.exe $(SRC) -lm 


win64GLUT:
	$(CC_MINGW64) -o roomba_test_d.exe $(SRC) -lopengl32 $(WIN_LIB64) -lglu32 -lm 


linux:
	$(CC) -o roomba_test_d.exe $(SRC) -L/usr/lib  -lm 

linux-glut:
	$(CC) -o roomba_test_d.exe $(SRC) -L/usr/lib -lglut -lGLU -lGL -lm 

clean:
	rm *.o *.exe
