CC = g++ 
CFLAGS = -c -O2
DBFLAGS = -O2
OBJS = random.o chromosome.o individual.o globalSetup.o ga.o population.o nsgapopulation.o\
	selection.o crossover.o localsearch.o

GAtbx    : $(OBJS) userDefinables.cpp
	$(CC) $(DBFLAGS) -o GAtbx userDefinables.cpp $(OBJS)
random.o : random.cpp
	$(CC) $(CFLAGS) random.cpp
chromosome.o : chromosome.cpp
	$(CC) $(CFLAGS) chromosome.cpp
globalSetup.o : globalSetup.cpp
	$(CC) $(CFLAGS) globalSetup.cpp
individual.o : individual.cpp
	$(CC) $(CFLAGS) individual.cpp
selection.o : selection.cpp
	$(CC) $(CFLAGS) selection.cpp
population.o : population.cpp
	$(CC) $(CFLAGS) population.cpp
ga.o : ga.cpp
	$(CC) $(CFLAGS) ga.cpp
nsgapopulation.o : nsgapopulation.cpp
	$(CC) $(CFLAGS) nsgapopulation.cpp
crossover.o : crossover.cpp
	$(CC) $(CFLAGS) crossover.cpp
localsearch.o : localsearch.cpp
	$(CC) $(CFLAGS) localsearch.cpp

