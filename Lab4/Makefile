#
#   Purpose: This makefile generates the VoteR executable
#
#   Author: Sijie Pan
#


CXXFLAGS = --std=c++11 -xCORE-AVX2 -O3

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

VOTEREXE = VoteR
MAINOBJS = NvraRecord.o VoteR.o

$(VOTEREXE): $(MAINOBJS)
	$(CXX) $(CXXFLAGS) -o $(VOTEREXE) $(MAINOBJS)

NvraRecord.o: NvraRecord.cpp NvraRecord.h Exceptions.h
VoteR.o: VoteR.cpp NvraRecord.o TemplatedArray.h Exceptions.h

.PHONY: clean
clean:
		-rm -f $(VOTEREXE) $(MAINOBJS)
