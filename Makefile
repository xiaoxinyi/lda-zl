# The Makefile for the C++ implementation of lda

COMPILER = g++
OBJS = utils.o document.o corpus.o lda.o inference.o estimate.o option.o model.o
# OBJS_INFER = utils.o document.o corpus.o lda.o inference.o estimate.o option.o model.o infer_main.o 
# OBJE_TRAIN = utils.o document.o corpus.o lda.o inference.o estimate.o option.o model.o lda_main.o
SOURCE = $(OBJS:.o=.cc)

FLAGS = -g -Wall  -I/usr/local/Cellar/gsl/1.16/include -std=c++11

# GSL library
LIBS = -lgsl -lgslcblas -L/usr/local/Cellar/gsl/1.16/lib

default: lda infer-lda

lda: $(OBJS) 
	$(COMPILER) $(FLAGS) $(OBJS)  lda_main.cc -o lda  $(LIBS)


infer-lda: $(OBJS) 
	$(COMPILER) $(FLAGS) $(OBJS) infer_main.cc -o infer-lda $(LIBS)

%.o: %.cc
	$(COMPILER) -c $(FLAGS) -o $@  $< 

.PHONY: clean
clean: 
	rm -f *.o