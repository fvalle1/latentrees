CXX=clang++
LINKER_FLAGS =-framework Metal -framework MetalKit -framework Cocoa -framework CoreFoundation -fobjc-link-runtime
COMPILER_FLAGS = -x objective-c++
INCLUDE_FLAGS = -I/Users/filippovalle/Developer/pcg-cpp/include
CFLAGS = -O3
TARGETS = rand.out
TOCLEAN = *.o *.out

all: $(TARGETS)

%.out: %.cpp mtlpp.o
	$(CXX) $(CFLAGS) -std=c++14 $(INCLUDE_FLAGS) $(LINKER_FLAGS) $^  -o $@

%.o: %.c
	$(CXX) -std=c++14 $(INCLUDE_FLAGS) $(COMPILER_FLAGS) -c $< -o $@

%.o: %.mm
	$(CXX) -std=c++14 $(CFLAGS) -c $< -o $@


.PHONY:
clean:
	rm -rf *.o *.out $(TARGETS)
