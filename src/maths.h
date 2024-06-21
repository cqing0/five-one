struct vec3 {
	float x, y, z;
};

typedef float mat4f;

// TODO: Refactor so we don't need mult? 
void scale(mat4f *arr, vec3 *scale_vector, float mult);

void translate(mat4f *arr, vec3 *translate_vector);
