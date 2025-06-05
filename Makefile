TARGET = task_manager

build:
	mkdir -p build
	cd build && cmake ..

run:
	cd build && cmake --build . && ./$(TARGET)

clean:
	rm -rf build