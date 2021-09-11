# Makefile
CFLAGS = -g
LDFLAGS =
LDLIBS = -lpthread -lrt
CPPFLAGS = -MMD -MP

all : write read
write    : $(patsubst %c,%o,$(wildcard write.c))
read : $(patsubst %c,%o,$(wildcard read.c))

clean : ; -rm -fr *.o *.d
.PHONE : clean
