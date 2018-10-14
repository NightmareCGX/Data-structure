#
#   Purpose: This makefile generates the VoteR executable
#
#   Author: Sijie Pan
#


CXXFLAGS = --std=c++11 -xCORE-AVX2 -O3 #0 -g #-check-pointers=rw

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

VOTEREXE = VoteR
MAINOBJS = NvraRecord.o NvraComparator.o VoteR.o

$(VOTEREXE): $(MAINOBJS)
	$(CXX) $(CXXFLAGS) -o $(VOTEREXE) $(MAINOBJS)

NvraRecord.o: NvraRecord.cpp NvraRecord.h Exceptions.h
NvraComparator.o : NvraComparator.cpp NvraComparator.h Comparator.h NvraRecord.h Exceptions.h
VoteR.o: VoteR.cpp NvraRecord.o NvraComparator.o TemplatedArray.h Search.h Sorter.h

.PHONY: clean
clean:
		-rm -f $(VOTEREXE) $(MAINOBJS)
