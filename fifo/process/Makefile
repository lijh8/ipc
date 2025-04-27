# build shared library with -fPIC, -shared
CXXFLAGS   = -g -std=c++23 # -O3 # -fPIC  # CXXFLAGS for .cpp
LDFLAGS  = # -L../hello # -shared
LDLIBS   = #-pthread
CPPFLAGS = -MMD -MP # -fexec-charset=gbk # -I../hello
CC      = $(CXX)  # link with CXX for .cpp

# target name is basename of one of the source files
all : server client
server : $(patsubst %.cpp,%.o,$(wildcard server.cpp)) # .cpp
client : $(patsubst %.cpp,%.o,$(wildcard client.cpp)) # .cpp
-include *.d
clean : ; -rm -fr *.o *.d server client
.PHONY : clean

CXXFLAGS += -std=c++23 -Wall -Wextra -Wpedantic
CXXFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
LDFLAGS  += -fsanitize=address -fsanitize=undefined -fsanitize=leak
