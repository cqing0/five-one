#include "maths.h"

void scale(mat4f *arr, vec3 *scale_vector, float mult)
{
	arr[0] += (scale_vector->x) * mult;
	arr[5] += (scale_vector->y) * mult;
	arr[10] += (scale_vector->z) * mult;
}

// TODO: Incorrect
void translate(mat4f *arr, vec3 *translate_vector)
{
	arr[3] = translate_vector->x;
	arr[7] = translate_vector->y;
	arr[11] = translate_vector->z;
}
