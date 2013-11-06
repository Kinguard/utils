TOP := $(shell pwd)

CXX = $(CROSS_COMPILE)g++
CC = $(CROSS_COMPILE)gcc
LD = $(CXX)

LIB=libutils.a
LIB_SRC=Condition.cpp Mutex.cpp Socket.cpp String.cpp Thread.cpp SimpleHttpClient.cpp NetUtils.cpp \
	FileUtils.cpp CircularBuffer.cpp Logger.cpp Shm.cpp NetServer.cpp UserGroups.cpp
DESTDIR?=$(HOME)/Apps

CXXFLAGS=-Wall -g -std=c++0x -I$(TOP)
LDFLAGS=-pthread -lrt
LDLIBS=

OBJS=$(LIB_SRC:%.cpp=%.o)
OBJS_OTHER=$(OBJS)

all: pre $(LIB)

install: all
	install -d $(DESTDIR)/lib
	install -d $(DESTDIR)/include/utils
	install -m 0644 $(LIB) $(DESTDIR)/lib
	install -m 0644 *.h $(DESTDIR)/include/utils

$(LIB): $(OBJS)
	@echo "Linking $(LIB)"
	$(AR) rcs $(LIB) $(OBJS) 

DEPDIR=$(TOP)/.deps

%.o : %.cpp
	@echo "Compiling $<"
	@$(COMPILE.cpp) $(CXXFLAGS) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -o $@ $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

-include $(APP_SRC:%.cpp=$(DEPDIR)/%.Po)

pre:
	@@if [ ! -d $(TOP)/.deps ]; then mkdir -p $(TOP)/.deps; fi

-include tests/maketests


clean:
	@echo "Make clean"
	@$(MAKE) test_clean                                                                
	@rm -f *~ *.o $(LIB) $(OBJS)
	@rm -rf .deps

.PHONY: clean pre all
