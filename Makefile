COMPILER = g++

WINDOWS_FLAGS = -lmingw32

win_main: src/main.cpp
	$(COMPILER) src/main.cpp -o build/five-one.exe $(WINDOWS_FLAGS)
