# The Makefile for the C++ implementation of lda

COMPILER = g++
OBJS = utils.o document.o corpus.o lda.o lda_main.o inference.o estimate.o option.o model.o
SOURCE = $(OBJS:.o=.cc)

FLAGS = -g -Wall  -I/usr/local/Cellar/gsl/1.16/include -std=c++11

# GSL library
LIBS = -lgsl -lgslcblas -L/usr/local/Cellar/gsl/1.16/lib

default: lda

lda: $(OBJS) 
	$(COMPILER) $(FLAGS) $(OBJS) -o lda  $(LIBS)

%.o: %.cc
	$(COMPILER) -c $(FLAGS) -o $@  $< 

.PHONY: clean
clean: 
	rm -f *.o