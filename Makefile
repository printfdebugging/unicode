run:
	cmake -B build
	cmake --build build
	ctest --test-dir build -VV
