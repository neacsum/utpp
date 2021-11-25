# default architecture
ifndef ARCH
ARCH = x64
endif


# compiler
CXX := g++
CPPFLAGS := -I include/ -Wall -pedantic

OBJDIR := o
LIBDIR := lib

ifeq ($(ARCH), x64)
OBJDIR := $(OBJDIR)/x64
LIBDIR := $(LIBDIR)/x64
else
OBJDIR := $(OBJDIR)/x86
LIBDIR := $(LIBDIR)/x86
CPPFLAGS += -m32
endif

ifdef _DEBUG
OBJDIR := $(OBJDIR)/Debug
LIBDIR := $(LIBDIR)/Debug
CPPFLAGS += -g
else
CPPFLAGS += -O3
OBJDIR := $(OBJDIR)/Release
LIBDIR := $(LIBDIR)/Release
endif


LIB = $(LIBDIR)/libutpp.a

OFILES =  deferred_test_reporter.o suites_list.o test.o test_reporter.o test_reporter_stdout.o \
 test_suite.o time_constraint.o time_helpers.o test_reporter_xml.o
 
OBJS = $(addprefix $(OBJDIR)/,$(OFILES))

all: $(LIB) samples

	
$(OBJDIR)/%.o: source/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(LIB): | $(LIBDIR)

$(LIB): $(OBJS)
	ar rcs  $@ $^

$(OBJS) : | $(OBJDIR)

$(LIBDIR) $(OBJDIR):
	mkdir -p $@
	
clean:
	rm $(OBJS)
	
samples: samp1

samp1: sample/sample.cpp $(LIB)
	$(CXX) $(CPPFLAGS) -o samp1 -L $(LIBDIR)/ $< -lutpp 
