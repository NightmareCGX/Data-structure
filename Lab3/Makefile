#
#   Purpose: This makefile generates the VoteR executable
#
#   Author: Sijie Pan
#


CXXFLAGS = --std=c++11 -xCORE-AVX2 -O3

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

VOTEREXE = VoteR
MAINOBJS = NvraRecord.o NvraArray.o VoteR.o

$(VOTEREXE): $(MAINOBJS)
	$(CXX) $(CXXFLAGS) -o $(VOTEREXE) $(MAINOBJS)

NvraRecord.o: NvraRecord.cpp NvraRecord.h
NvraArray.o: NvraArray.cpp NvraArray.h NvraRecord.o
VoteR.o: VoteR.cpp NvraArray.o NvraRecord.o

.PHONY: clean
clean:
		-rm -f $(VOTEREXE) $(MAINOBJS)
