all:
	mkdir -p build
	cd build && cmake ..
	cd build && make -j12

run: all
	exec $(shell find build/ -maxdepth 1 -type f -executable)


package: all
	-rm build/*.deb
	cd build && make package

