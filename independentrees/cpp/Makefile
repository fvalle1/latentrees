CC = clang
CXX=clang++
CFLAGS=-std=c++17 -O3 -Xclang -fopenmp
LDFLAGS=-pthread -lomp
OBJ=generator.o monitorThread.o model.o uniformModel.o gausModel.o nbinModel.o gammaModel.o
HEADERS = include/*.h models/include/*.h models/interfaces/*.h
INCLUDE=-I../pcg-cpp/include -Imodels/interfaces -Imodels/include -Iinclude/

lat.out: main.cpp $(OBJ) $(HEADERS)
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $< $(OBJ) -o $@

%.o: src/%.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) -c $^ -o $@

%.o: models/%.cpp
	$(CXX) -I. $(INCLUDE) $(CFLAGS) -c $^ -o $@

.PHONY:
clean:
	rm -f *.o lat.out