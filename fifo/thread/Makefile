# build shared library with -fPIC, -shared
CXXFLAGS   = -g -std=c++23 # -O3 # -fPIC  # CXXFLAGS for .cpp
LDFLAGS  = # -L../hello # -shared
LDLIBS   = #-pthread
CPPFLAGS = -MMD -MP # -fexec-charset=gbk # -I../hello
CC      = $(CXX)  # link with CXX for .cpp

# target name is basename of one of the source files
all: main_to_sub sub_to_main
main_to_sub : $(patsubst %.cpp,%.o,$(wildcard main_to_sub.cpp)) # .cpp
sub_to_main : $(patsubst %.cpp,%.o,$(wildcard sub_to_main.cpp)) # .cpp
-include *.d
clean : ; -rm -fr *.o *.d main_to_sub sub_to_main
.PHONY : clean

CXXFLAGS += -std=c++23 -Wall -Wextra -Wpedantic
CXXFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
LDFLAGS  += -fsanitize=address -fsanitize=undefined -fsanitize=leak
