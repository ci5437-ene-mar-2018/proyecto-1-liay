# to make a PDB starting from foo.psvn  type the command:     make pdb ss=foo
# if $(absname).txt exists it will be used as the input (abstraction commands) to build the abstraction,
#   otherwise the user will enter the abstraction commands interactively.

CC = gcc
CXX = g++
OPT = -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --backward_moves --fwd_history_len=3 --bwd_history_len=0

psvn2c_core.c:
	cp ../../../psvn/src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../../../psvn/src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../../../psvn/src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../../../psvn/bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

priority_queue.hpp:
	cp ../../Global/priority_queue.hpp ./priority_queue.hpp

node.hpp:
	cp ../../Global/node.hpp ./node.hpp


%.succ: %.c ../../../psvn/global/succ.c
	$(CC) $(OPT) ../../../psvn/global/succ.c -include $< -o $@
	rm -f $*.c

%.dist: %.c ../../../psvn/global/dist.cpp
	$(CXX) $(OPT) ../../../psvn/global/dist.cpp -include $< -o $@
	rm -f $*.c

%.distSummary: %.c ../../../psvn/global/distSummary.cpp
	$(CXX) $(OPT) ../../../psvn/global/distSummary.cpp -include $< -o $@
	rm -f $*.c

abstractor:
	$(CXX) $(OPT) ../../../psvn/src/abstractor.cpp ../../../psvn/src/psvn.cpp -o $@

%.pdb: abstractor
	@rm -f `dirname $*`_`basename $*`.{abst,pdb,psvn}
	./abstractor `dirname $*`.psvn `dirname $*`_`basename $*` < `basename $*`.txt
	make `dirname $*`_`basename $*`.distSummary
	echo Calculating `dirname $*`_`basename $*.pdb` ...
	@./`dirname $*`_`basename $*`.distSummary `dirname $*`_`basename $*`.pdb

%.dist_pdb: %.c ../../../psvn/global/dist_pdb.cpp
	$(CXX) $(OPT) ../../../psvn/global/dist_pdb.cpp -include $< -o $@
	rm -f $*.c

%.idaStar: %.c ../../Global/idaStar.cpp priority_queue.hpp node.hpp
	$(CXX) $(OPT) ../../Global/idaStar.cpp -include $< -include towers-of-hanoi-4-18_PDB.cpp -o $@

%.aStar: %.c ../../Global/aStar.cpp priority_queue.hpp node.hpp
	$(CXX) $(OPT) ../../Global/aStar.cpp -include $< -include towers-of-hanoi-4-18_PDB.cpp -o $@

%.bfs: %.c ../../Global/bfs.cpp priority_queue.hpp node.hpp
	$(CXX) $(OPT) -std=c++11 ../../Global/bfs.cpp -include $< -o $@

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.dist_pdb psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c abstractor *.dSYM *.o *~
