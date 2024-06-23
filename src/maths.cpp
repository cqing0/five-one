#include "maths.h"
#define _USE_MATH_DEFINES
#include <cmath>

void scale(mat4f *arr, vec3 scale_vector, float mult)
{
	arr[0] = (scale_vector.x) * mult;
	arr[5] = (scale_vector.y) * mult;
	arr[10] = (scale_vector.z) * mult;
}

// TODO: Incorrect
void translate(mat4f *arr, vec3 *in)
{
	arr[12] = in->x;
	arr[13] = in->y;
	arr[14] = in->z;

	/* arr[3] = in->x; */
	/* arr[7] = in->y; */
	/* arr[11] = in->z; */
}

void rotate(mat4f *arr, float rad_angle, vec3 *in)
{
	arr[0] = cos(rad_angle) + in->x * in->x * (1 - cos(rad_angle));
	arr[1] = in->x * in->y * (1 - cos(rad_angle)) - in->z * sin(rad_angle);
	arr[2] = in->x * in->z * (1 - cos(rad_angle)) + in->y * sin(rad_angle);
	
	arr[4] = in->x * in->y * (1 - cos(rad_angle)) - in->z * sin(rad_angle);
	arr[5] = cos(rad_angle) + in->y * in->y * (1 - cos(rad_angle));
	arr[6] = in->y * in->z * (1 - cos(rad_angle)) - in->x * sin(rad_angle);

	arr[8] = in->z * in->x * (1 - cos(rad_angle)) - in->y * sin(rad_angle);
	arr[9] = in->z * in->y * (1 - cos(rad_angle)) + in->x * sin(rad_angle);
	arr[10] = cos(rad_angle) + in->z * in->z * (1 - cos(rad_angle));
}

void perspective(mat4f *arr, float left, float right, float bottom, float top, float n_plane, float f_plane)
{
	float rl = (right - left);
	float tb = (top - bottom);
	float fn = (f_plane - n_plane);

	arr[0] = (n_plane * 2.0f) / rl;
	arr[5] = (n_plane * 2.0f) / tb;
	arr[8] = (right + left) / rl;
	arr[9] = (top + bottom) / fn;
	arr[10] = -(f_plane + n_plane) / fn;
	arr[11] = -1.0f;
	arr[14] = -(f_plane * n_plane * 2.0f) / fn;
}

void orthographic(mat4f *arr, float left, float right, float bottom, float top, float n_plane, float f_plane)
{
	float rl = (right - left);
	float tb = (top - bottom);
	float fn = (f_plane - n_plane);

	arr[0] = 2.0f / rl;
	arr[5] = 2.0f / tb;
	arr[10] = -2.0f / fn;
	arr[12] = -(left + right) / rl;
	arr[13] = -(top + bottom) / tb;
	arr[14] = -(f_plane + n_plane);
	arr[15] = 1.0f;
}
