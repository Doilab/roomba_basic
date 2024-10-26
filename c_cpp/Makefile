CC = g++
CC_MINGW32 = c:\Program Files (x86)\CodeBlocks\MinGW\bin\g++
CC_MINGW64 = c:\Program Files\CodeBlocks\MinGW\bin\g++
CC_MINGW64VSC = c:\cprog20\mingw64\bin\g++
CC_MINGW64VSCP = ..\..\mingw64\bin\g++
SRC = roomba_test.cpp serial.cpp
WIN_LIB32 := mingw32_lib/libfreeglut/libfreeglut.a
WIN_LIB64 := mingw32_lib/libfreeglut/x64/freeglut.lib
WIN_OPT = -lglu32 -lm -lopengl32 -static -lstdc++ -lgcc  -lwinpthread

all:win64

win64:
#	$(CC_MINGW64VSC) -o roomba_test_d.exe $(SRC) -lm 
#	$(CC_MINGW64VSC) -o roomba_test_d.exe $(SRC)  $(WIN_OPT) 
	$(CC_MINGW64VSCP) -o roomba_test_d.exe $(SRC)  -lm -static


#win64GLUT:
#	$(CC_MINGW64) -o roomba_test_d.exe $(SRC) $(WIN_OPT) $(WIN_LIB64)


linux:
	$(CC) -o roomba_test_d.exe $(SRC) -L/usr/lib  -lm 

linux-glut:
	$(CC) -o roomba_test_d.exe $(SRC) -L/usr/lib -lglut -lGLU -lGL -lm 

clean:
	rm *.o *.exe
