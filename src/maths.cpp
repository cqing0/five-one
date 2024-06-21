#include "maths.h"

void scale(mat4f *arr, vec3 *scale_vector, float mult)
{
	arr[0] += (scale_vector->x) * mult;
	arr[5] += (scale_vector->y) * mult;
	arr[10] += (scale_vector->z) * mult;
}
