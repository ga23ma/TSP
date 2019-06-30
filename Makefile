src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

solver: $(obj)
	$(CXX) $(DEBUG) -o $@ $^

solver.o: solver.cpp
	$(CXX) $(DEBUG) -c -o solver.o solver.cpp

main.o: main.cpp
	$(CXX) $(DEBUG) -c -o main.o main.cpp
	
.PHONY: clean
clean:
	rm -f $(obj) solver

debug: DEBUG = -g
debug: solver
