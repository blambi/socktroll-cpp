TARGET += socktroll
SRCS += main.cpp network.cpp
CC := gcc
CPP := g++
LD := g++
override CFLAGS += -Wall -pedantic -ggdb -std=c++98 -g #-DDEBUG
LDFLAGS += -g
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
