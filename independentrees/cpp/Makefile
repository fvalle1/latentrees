CXX=clang++
CFLAGS=-std=c++17 -O3
LDFLAGS=-pthread
OBJ=generator.o model.o uniformModel.o gausModel.o nbinModel.o
INCLUDE=-I../pcg-cpp/include -Imodels/interfaces -Imodels/include

lat.out: $(OBJ)
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) main.cpp $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) -c $< -o $@

%.o: models/%.cpp
	$(CXX) -I. $(INCLUDE) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o lat.out