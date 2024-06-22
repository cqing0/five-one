#include <windows.h>
#include "glew.h"
#include <GL/gl.h>
#include "SDL.h"
#include <stdio.h>
#include "file_loading.h"
#include "maths.h"
#include "vertex_data.h"
#include "timing.h"

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
// To test movement we will be manipulating the cubes' vertices
// in various transforms
// TODO: Remove when finished with
static float y_size = 1.0f;
static float x_size = 1.0f;
void get_input()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	player_dir = 0;

	if (state[SDL_SCANCODE_A]) {
		x_size -= 0.55f * delta_time;
	}
	if (state[SDL_SCANCODE_D]) {
		x_size += 0.55f * delta_time;
	}
	if (state[SDL_SCANCODE_W]) {
		y_size += 0.55f * delta_time;
	}
	if (state[SDL_SCANCODE_S]) {
		y_size -= 0.55f * delta_time;
	}
}

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
	GLuint main_texture_id;
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

	glGenTextures(1, &main_texture_id);
	glBindTexture(GL_TEXTURE_2D, main_texture_id);
	const char *texture_data = LoadBitmap_GL("assets/tex.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *file_fragment_shader_source = LoadShaderData("assets/fragShaderTex.fs");
	glShaderSource(fragment_shader, 1, &file_fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info);
		printf("Error compiling fragment shader %s\n", info);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex), cube_tex, GL_STATIC_DRAW);

	// Positional data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Colour data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shader_program);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool quit = false;
	SDL_Event event;

	// Temporary matrices
	mat4f model[MAT4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4f view[MAT4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4f projection[MAT4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

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

		/* translate(view, &scalar); */
		scale(model, vec3(x_size, y_size, 1.0f), 1.0f);

		// Temporary transforms for testing inputs and transforms
		GLuint model_loc = glGetUniformLocation(shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
		GLuint view_loc = glGetUniformLocation(shader_program, "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);
		GLuint projection_loc = glGetUniformLocation(shader_program, "projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection);

		glBindVertexArray(vertex_array_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		SDL_GL_SwapWindow(window);
	}

	// Free necessary data on shutdown
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
