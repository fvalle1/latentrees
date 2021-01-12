CXX = clang++

LIBS = -L.
FLAGS = -std=c++17 -O3
INCLUDE = -I/users/filippovalle/Developer/pcg-cpp/include
FRAMEWORKS = OpenCL

all: test.out rand.out

%.out: %.o LocalDevices.o
	clang++ $(FLAGS) $(LIBS) -framework $(FRAMEWORKS) $^ -o $@

%.o: %.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) $< -o $@

.PHONY:
clean:
	rm -rf *.o *.out
