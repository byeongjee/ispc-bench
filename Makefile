%.vegen.o: %.cpp
	vegen-clang++ -O3 -march=native -ffast-math $^ -o $@ -c

%.llvm.o: %.cpp
	clang++ -O3 -march=native -ffast-math $^ -o $@ -c

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

run-volume: volume.vegen volume.llvm
	./volume.vegen camera.dat density_lowres.vol
	./volume.llvm camera.dat density_lowres.vol

clean:
	rm -rf *.o *.vegen *.llvm
