TARGET += socktroll
SRCS += main.cpp network.cpp ui.cpp protocol.cpp
CC := gcc
CPP := g++
LD := g++
override CFLAGS += -Wall -pedantic -ggdb -std=c++98 -g \
-D_XOPEN_SOURCE_EXTENDED -DWIDEC_CURSES `pkg-config glibmm-2.4 --cflags`#-DDEBUG
LDFLAGS += -g -lncursesw `pkg-config glibmm-2.4 --libs`
OBJS += $(SRCS:.cpp=.o)

all:$(TARGET)

debug:
	@$(MAKE) CFLAGS=-DDEBUG all

$(TARGET):$(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	$(CPP) -c $(CFLAGS) $< 

clean:
	rm -f $(TARGET) *.o

devclean:clean
	rm -f *~
