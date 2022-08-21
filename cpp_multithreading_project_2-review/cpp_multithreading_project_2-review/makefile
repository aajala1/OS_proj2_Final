# Compile p01.cpp
OBJ = p1.o
OBJ_0 = p0.o

p0: ${OBJ_0} 
	clang++ ${OBJ_0} -lpthread -o p0

p1: ${OBJ} 
	clang++ ${OBJ} -lpthread -o p1

account_thread.o: account_thread.cpp config.h
	clang++ -g account_thread.cpp

m1: m1.cpp utils_m1.h
	clang++ --std=gnu++17 -g m1.cpp  -lpthread -o m1

m1: m2.cpp utils_m2.h
	clang++ --std=gnu++17 -g m2.cpp  -lpthread -o m2


p0.o: p0.cpp config.h 
	clang++ --std=gnu++17 -g m2.cpp -o m2

p1.o: p1.cpp config.h 
	clang++ -c -g p1.cpp

test: test.o
	clang++ -c -g test.o test.cpp

clean:
	rm *.o p1 p0 m1 m2 *.txt

