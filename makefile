# compiler options
CPPFLAGS = -I include/ -c -g -Wall -pedantic

OBJDIR = o
OUTDIR = lib/x64/Release
CXX = g++

OFILES =  deferred_test_reporter.o suites_list.o test.o test_reporter.o test_reporter_stdout.o \
 test_suite.o time_constraint.o time_helpers.o xml_test_reporter.o
 
OBJS = $(addprefix $(OBJDIR)/,$(OFILES))

.PHONY: directories

all: directories program

directories: $(OUTDIR) $(OBJDIR)

$(OUTDIR):
	md -p $(OUTDIR)
		
$(OBJDIR):
	md -p $(OBJDIR)
	
$(OBJDIR)/%.o: source/%.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

$(OUTDIR)/libutpp.a: $(OBJS)
	ar rcs  $@ $^

clean:
	rm $(OBJS)
	
samples: samp1

samp1: sample/sample.cpp
	$(CXX) -g -Wall -I include/  -o samp1 -L $(OUTDIR)/ $< -lutpp 
	
