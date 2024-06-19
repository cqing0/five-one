#include <windows.h>
#include "glew.h"
#include <GL/gl.h>
#include "SDL.h"
#include <stdio.h>
#include "file_loading.h"
#include "shaders.h"

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
GLuint vertex_shader;
GLuint fragment_shader;
GLuint shader_program;

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

	int success;
	char info[512];

	// Load the shaders into an OpenGL program
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char *file_vertex_shader_source = LoadShaderData("assets/vertShader.vs");
	glShaderSource(vertex_shader, 1, &file_vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetProgramInfoLog(vertex_shader, 512, NULL, info);
		printf("Error loading vertex shader %s\n", info);
    }
	else {
		printf("Success loading vertex shader\n");
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetProgramInfoLog(fragment_shader, 512, NULL, info);
		printf("Error loading fragment shader %s\n", info);
    }
	else {
		printf("Success loading fragment shader\n");
	}

	// Create a shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info);
		printf("Error compiling shader program %s\n", info);
	}
	else {
		printf("Success compiling shader program\n");
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Bind data
	// Array data
	glGenVertexArrays(1, &vertex_array_id);
	glGenBuffers(1, &vertex_buffer);
	glBindVertexArray(vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool quit = false;
	SDL_Event event;

	// Main loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
		// Generate output
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vertex_array_id);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(window);
	}

	// Free necessary data on shutdown
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
