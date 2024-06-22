#include "maths.h"
#define _USE_MATH_DEFINES
#include <cmath>

radians deg_to_rad(float angle_in_degrees)
{
	return angle_in_degrees * (M_PI/180);
}

void scale(mat4f *arr, vec3 scale_vector, float mult)
{
	arr[0] = (scale_vector.x) * mult;
	arr[5] = (scale_vector.y) * mult;
	arr[10] = (scale_vector.z) * mult;
}

// TODO: Incorrect
void translate(mat4f *arr, vec3 *transform_vector)
{
	arr[3] = transform_vector->x;
	arr[7] = transform_vector->y;
	arr[11] = transform_vector->z;
}

// TODO
/* mat4f rotate(mat4f *arr, float angle_in_rad, vec3 *transform_vector) */
/* { */
/* 	mat4f mat_rot[MAT4] = { */
/* 		cos(angle_in_rad) + transform_vector->x * transform_vector->x * (1 - cos(angle_in_rad)), */
/* 	}; */
/* } */
