# build shared library with -fPIC, -shared
CFLAGS   = -g # -O3 # -fPIC  # CXXFLAGS for .cpp
LDFLAGS  = # -L../hello # -shared
LDLIBS   = #-pthread
CPPFLAGS = -MMD -MP # -fexec-charset=gbk # -I../hello
#CC      = $(CXX)  # link with CXX for .cpp

# target name is basename of one of the source files
all : write read
write : $(patsubst %.cpp,%.o,$(wildcard write.c))  # .cpp
read : $(patsubst %.cpp,%.o,$(wildcard read.c))  # .cpp
-include *.d
clean : ; -rm -fr *.o *.d
.PHONY : clean
