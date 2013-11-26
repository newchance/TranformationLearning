#Executable
EXE    = TBL

# Compiler, Linker Defines
CC      = g++ 
#CFLAGS  = -Wall -O0 -Wno-deprecated -g -v
#LIBS    = ./lib/libsrilm.a ./lib/lm.a ./lib/misc.a ./lib/dstruct.a 
# ./srilm-lib/mmlm.a  #./srilm-lib/libsrilm.a ./srilm-lib/lm.a ./srilm-lib/misc.a ./srilm-lib/dstruct.a
LDFLAGS = $(LIBS)
#FLAG=-O0 -Wall -D_FILE_OFFSET_BITS=64
# Compile and Assemble C++ Source Files into Object Files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@  -I/usr/local/include/python2.7 -L/usr/local/lib/python2.7
# Source and Object files
SRC    = $(wildcard *.cpp)
OBJ    = $(patsubst %.cpp, %.o, $(SRC))
FLAG=
CFLAGS=-O3 -g 
# Link all Object Files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(FLAG) -o $(EXE) -lpthread  -lpython2.7 -lz

.PHONY: clean
clean:
	 rm -f core *.o

