main: src/main.cpp
	cl /Zi src/main.cpp /I"src\\include" /Fe"build\\five-one.exe" /Fo"build\\" /link /LIBPATH:"src\\lib" opengl32.lib SDL2main.lib SDL2.lib glew32.lib
	
