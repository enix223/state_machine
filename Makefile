.PHONY: macos-preset clean test

all: statemachine

macos-preset:
	cmake --preset macos

statemachine: macos-preset
	cmake --build --preset macos

clean: macos-preset
	cmake --build --preset macos --target clean

test: statemachine
	cd out/build/macos/test && ctest
