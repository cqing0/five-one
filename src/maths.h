#define MAT4 16
typedef float mat4f;
typedef float radians;

struct vec3 {
	float x, y, z;

	vec3() = default;

	vec3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
};

radians deg_to_rad(float angle_in_degrees);

// TODO: Refactor so we don't need mult? 
void scale(mat4f *arr, vec3 scale_vector, float mult);
void translate(mat4f *arr, vec3 *transform_vector);
mat4f rotate(mat4f *arr, float angle_in_rad, vec3 *transform_vector);
