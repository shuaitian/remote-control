MUDUO_DIRECTORY ?= ..
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRCS = main.cpp\
	ChatServer.cpp\
	MessageDispatcher.cpp\
	UserDomain.cpp\
	Utils.cpp\
	UserDao.cpp\
	FriendDao.cpp\
	ChatMessage.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))	

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L/usr/lib/mysql/ -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lmysqlclient -lrt -I$(MUDUO_INCLUDE)



all: $(OBJS)
	g++  -o $@ $^ $(LDFLAGS)
%.o:%.cpp
	g++  -o $@ $^ -c $(LDFLAGS)
clean:
	rm -f *.o
	rm -f all
