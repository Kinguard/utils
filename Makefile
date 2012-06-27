TOP := $(shell pwd)

LIB=libutils.a

LIB_SRC=Condition.cpp Mutex.cpp Socket.cpp String.cpp Thread.cpp SimpleHttpClient.cpp

CXXFLAGS=-Wall -std=c++0x -I$(TOP)
LDFLAGS=-pthread
LDLIBS=

OBJS=$(LIB_SRC:%.cpp=%.o)
OBJS_OTHER=$(OBJS)

all: pre $(LIB)

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
