# default architecture
ifndef ARCH
ARCH = x64
endif


# compiler
CXX := g++
CPPFLAGS := -I include/ -Wall -pedantic

OBJDIR := o

ifeq ($(ARCH), x64)
OBJDIR := $(OBJDIR)/x64
else
OBJDIR := $(OBJDIR)/x86
CPPFLAGS += -m32
endif

ifdef _DEBUG
OBJDIR := $(OBJDIR)/Debug
CPPFLAGS += -g
else
CPPFLAGS += -O3
OBJDIR := $(OBJDIR)/Release
endif



all: samples
	
$(OBJDIR)/%.o: source/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

	
clean:
	rm $(OBJS)
	
samples: samp1

samp1: sample/sample.cpp
	$(CXX) $(CPPFLAGS) -o samp1 $< 
