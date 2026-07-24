run:
	cmake -B build
	cmake --build build
	ctest --test-dir build -VV

reformat:
	find \
		include/* \
		source/* \
		tests/* \
		-iname '*.h' -o \
		-iname '*.c' -o \
		-iname '*.vert' -o \
		-iname '*.frag' | xargs clang-format -i
