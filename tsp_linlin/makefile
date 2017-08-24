test: evalOneLLH.cpp random.o chromosome.o globalSetup.o individual.o ga.o problem.o feature.o mutation.o selection.o crossover.o localsearch.o  population.o 
	g++ evalOneLLH.cpp random.o  globalSetup.o  chromosome.o individual.o ga.o problem.o feature.o mutation.o selection.o crossover.o localsearch.o population.o  -g -O3  -o evaloneLLH
random.o:random.cpp random.hpp
	g++ -c -g -o3 random.cpp
chromosome.o:chromosome.cpp chromosome.hpp
	g++ -c -g  -o3 chromosome.cpp -o chromosome.o
globalSetup.o:globalSetup.hpp globalSetup.cpp
	g++ -c -g -o3 globalSetup.cpp
individual.o:individual.hpp individual.cpp
	g++ -c -g -o3 individual.cpp
population.o: population.hpp population.cpp
	g++ -c -g -o3 population.cpp
ga.o:ga.hpp ga.cpp
	g++ -c -g -o3 ga.cpp
problem.o:problem.hpp problem.cpp
	g++ -c -g -o3 problem.cpp
feature.o:feature.hpp feature.cpp
	g++ -c -g -o3 feature.cpp

mutation.o: mutation.hpp mutation.cpp
	g++ -c -g -o3 mutation.cpp
selection.o: selection.hpp selection.cpp
	g++ -c  -g -o3 selection.cpp
crossover.o: crossover.hpp crossover.cpp
	g++ -c -g -o3 crossover.cpp -o crossover.o
localsearch.o: localsearch.hpp localsearch.cpp
	g++ -c  -g -o3  localsearch.cpp


