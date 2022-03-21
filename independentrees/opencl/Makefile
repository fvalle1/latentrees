CXX = clang++

LIBS = -L.
FLAGS = -std=c++17 -O3
INCLUDE = -I../pcg-cpp/include
FRAMEWORKS = OpenCL

all: rand.out

%.out: %.o LocalDevices.o
	#$(CXX) $(FLAGS) $(LIBS) -framework $(FRAMEWORKS) $^ -o $@
	$(CXX) $(FLAGS) $(LIBS) $^ -o $@ -lm -pthread -lOpenCL

%.o: %.cpp
	$(CXX) -c $(FLAGS) $(INCLUDE) $< -o $@

.PHONY:
clean:
	rm -rf *.o *.out
