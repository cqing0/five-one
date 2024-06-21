#include <windows.h>
#include "glew.h"
#include <GL/gl.h>
#include "SDL.h"
#include <stdio.h>
#include "file_loading.h"
#include "maths.h"
#include "vertex_data.h"

#define SDL_main main

// TODO: Rescope these globals?
SDL_Window *window = NULL;
SDL_GLContext context = NULL;
static int width = 1280;
static int height = 720;

// OpenGL vert data
GLuint vertex_array_id;
GLuint vertex_buffer;

// Get user input;  TODO: rescope globals
static int player_dir = 0;
void get_input()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	player_dir = 0;

	if (state[SDL_SCANCODE_A]) player_dir = -1;
	if (state[SDL_SCANCODE_D]) player_dir = 1;
}

// Timings; TODO: Rescope globals
static float delta_time = 0.0f;
static int ticks = 0;

void update_game()
{
	// 60 fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks + 16)) {
	}

	delta_time = (SDL_GetTicks() - ticks) / 1000.0f;

	if (delta_time > 0.05f) delta_time = 0.05f;

	ticks = SDL_GetTicks();
}

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

	window = SDL_CreateWindow("Game", 50, 50, width, height, SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

	// Shaders
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader_program;

	if (context == NULL) {
        printf("error creating context: %s\n", SDL_GetError());
	}

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("error: %d\n", glGetError());
    }

	int success;

	// Load the shaders into an OpenGL program
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char *file_vertex_shader_source = LoadShaderData("assets/vertShader.vs");
	printf("%s\n", file_vertex_shader_source);
	glShaderSource(vertex_shader, 1, &file_vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	char info[4096];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetShaderInfoLog(vertex_shader, 4096, NULL, info);
		printf("Error compiling vertex shader %s 0x%x\n", info, glGetError());
    }
	else {
		printf("Success loading vertex shader\n");
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *file_fragment_shader_source = LoadShaderData("assets/fragShader.fs");
	glShaderSource(fragment_shader, 1, &file_fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(right_angle_tri), right_angle_tri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool quit = false;
	SDL_Event event;

	// A temporary transform matrix
	mat4f transform[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// Try scaling
	vec3 scalar = {0.1f, 0.5f, 1.0f};

	// Main loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
		// Get input
		get_input();

		// Update game information
		update_game();

		// Generate output
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// Temporary transforms for testing inputs and transforms
		GLuint transform_loc = glGetUniformLocation(shader_program, "transform");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, transform);
		scale(transform, &scalar, player_dir * delta_time);

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
