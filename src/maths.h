#include <cmath>
#define MAT4 16
typedef float mat4f;
typedef float radians;

#define DEG_TO_RAD acos(-1.0f) / 180

// We don't need to denote this matrix's n*m as
// we will always be using 4x4
struct Matrix {
	float m0, m4, m8, m12;
	float m1, m5, m9, m13;
	float m2, m6, m10, m14;
	float m3, m7, m11, m15;
};

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
void rotate(mat4f *arr, float rad_angle, vec3 *in);
void translate(mat4f *arr, vec3 *in);
void perspective(mat4f *arr, float left, float right, float bottom, float top, float n_plane, float f_plane);
void n_perspective(mat4f *arr, float fov_y, float aspect, float nplane, float fplane);
void orthographic(mat4f *arr, float left, float right, float bottom, float top, float n_plane, float f_plane);
