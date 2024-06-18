#include <windows.h>
#include "glew.h"
#include <GL/gl.h>
#include "SDL.h"
#include <stdio.h>
#include "file_loading.h"

#define SDL_main main

// TODO: Rescope these globals?
SDL_Window *window = NULL;
SDL_GLContext context = NULL;

// OpenGL vert data
GLuint vertex_array_id;
GLuint vertex_buffer;
static GLfloat vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

// Shaders
char *vertex_shader;
char *fragment_shader;

void Init_SDL_GL()
{
    // Set OpenGL profiles
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Set OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // RGBA: 8 bit
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // Doublebuffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

void Init_GL_Objects()
{
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
	}

	Init_SDL_GL();

	window = SDL_CreateWindow("Game", 50, 50, 600, 600, SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);
	if (context == NULL) {
        printf("error creating context: %s\n", SDL_GetError());
	}

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("error: %d\n", glGetError());
    }

	Init_GL_Objects();

	bool quit = false;
	SDL_Event event;

	// Main loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
		// Generate output
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT );

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);
	}

	// Free necessary data on shutdown
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
