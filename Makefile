all: ao.vegen ao.llvm rt.vegen rt.llvm options.vegen options.llvm stencil.vegen stencil.llvm mandelbrot.vegen mandelbrot.llvm volume.vegen volume.llvm 

VEGEN_CXX = /home/t-bykang/vegen/build/vegen-clang++
VEGEN_C = /home/t-bykang/vegen/build/vegen-clang

clean:
	rm -rf *.o *.vegen *.llvm

.PHONY: all clean

%.vegen.o: %.cpp
	$(VEGEN_CXX) -O3 -march=native -ffast-math $^ -o $@ -c

%.vegen.ll: %.cpp
	$(VEGEN_CXX) -O3 -S -emit-llvm -ffast-math $^ -o $@ -c

%.llvm.o: %.cpp
	clang++ -O3 -march=native -ffast-math $^ -o $@ -c

%.llvm.ll: %.cpp
	clang++ -O3 -S -emit-llvm -ffast-math $^ -o $@ -c

test-%.o: test-%.cpp
	clang++ -O1 $^ -o $@ -c

%.llvm: %_serial.llvm.o test-%.o
	clang++ $^ -o $@

%.vegen: %_serial.vegen.o test-%.o
	clang++ $^ -o $@

run-ao: ao.vegen ao.llvm
	./ao.vegen 500 500
	./ao.llvm 500 500

run-rt: rt.vegen rt.llvm
	./rt.vegen sponza
	./rt.llvm sponza

run-options: options.vegen options.llvm
	./options.vegen
	./options.llvm

run-stencil: stencil.vegen stencil.llvm
	./stencil.vegen
	./stencil.llvm

run-mandelbrot: mandelbrot.vegen mandelbrot.llvm
	./mandelbrot.vegen
	./mandelbrot.llvm

run-volume: volume.vegen volume.llvm
	./volume.vegen camera.dat density_lowres.vol
	./volume.llvm camera.dat density_lowres.vol
