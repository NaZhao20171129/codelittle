mainquery:mainQuery.cpp textQuery.o QueryBase.o
	g++ mainQuery.cpp textQuery.o QueryBase.o -o   mainquery

textQuery.o:textQuery.cpp textQuery.h
	g++  -c  textQuery.cpp  


QueryBase.o:QueryBase.cpp QueryBase.h  
	g++ -c  QueryBase.cpp


clean:
	rm ./*.o
