COMPILER = clang
SOURCE_LIBS = -Ilib/
BIN = -IBin/
OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a -std=c17
OSX_OUT = -o "bin/build_osx"
CFILES = src/*.c

build_osx: 
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)

run:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)
	bin/./build_osx