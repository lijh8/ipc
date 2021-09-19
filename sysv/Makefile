# build shared library with -fPIC, -shared
CFLAGS   = -g -std=c99 # -O3 # -fPIC  # CXXFLAGS for .cpp
LDFLAGS  = # -L../hello # -shared
LDLIBS   = #-pthread
CPPFLAGS = -MMD -MP # -fexec-charset=gbk # -I../hello
#CC      = $(CXX)  # link with CXX for .cpp

# target name is basename of one of the source files
all : write read
write : $(patsubst %.c,%.o,$(wildcard write.c)) # .cpp
read : $(patsubst %.c,%.o,$(wildcard read.c)) # .cpp
-include *.d
clean : ; -rm -fr *.o *.d
.PHONY : clean
