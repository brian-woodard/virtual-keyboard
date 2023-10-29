# Make Symbology Interface

#.SILENT:

LDFLAGS = -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXcursor -lGLEW -lGL -lglfw3 -lsoil2 -lfreetype

CPPFLAGS = -g -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I../include -I../utils -I../resources/include -I../resources/include/freetype2/ -I../resources/include/soil2

SRCS =  ../utils/Stopwatch.cpp \
		  ../utils/PrintData.cpp \
		  KeyboardMain.cpp \
		  CShaderUtils.cpp \
		  CText.cpp \
		  Line.cpp \
		  CImage.cpp \
		  Stats.cpp \
		  SimTimer.cpp

all :
	make keyboard
	
	echo ""
	echo "Build complete, moving files to ../bin"
	echo ""
	
	mkdir -p ../bin
	mv Keyboard ../bin

keyboard :
	g++ $(CPPFLAGS)  $(SRCS) $(LDFLAGS) -o Keyboard

clean :
	rm -f ../bin/Keyboard

