#include <stdint.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MAX3(a, b, c) MAX(MAX(a, b), c)
#define MIN3(a, b, c) MIN(MIN(a, b), c)

void HSVtoRGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
	float hex = h / 60.0;
	uint8_t primary = (uint8_t) hex;
	float secondary = hex - primary;

	float x = (1.0 - s) * v;
	float y = (1.0 - (s * secondary)) * v;
	float z = (1.0 - (s * (1.0 - secondary))) * v;

	switch (primary) {
		case 0:
			*r = (v * 255.0) + 0.5;
			*g = (z * 255.0) + 0.5;
			*b = (x * 255.0) + 0.5;
			break;
	
		case 1:
			// 1: R = b, G = v, B = a
			*r = (y * 255.0) + 0.5;
			*g = (v * 255.0) + 0.5;
			*b = (x * 255.0) + 0.5;
			break;
	
		case 2:
			// 2: R = a, G = v, B = c
			*r = (x * 255.0) + 0.5;
			*g = (v * 255.0) + 0.5;
			*b = (z * 255.0) + 0.5;
			break;
		case 3:
			// 3: R = a, G = b, B = v 
			*r = (x * 255.0) + 0.5;
			*g = (y * 255.0) + 0.5;
			*b = (v * 255.0) + 0.5;
			break;
		case 4:
			// 4: R = c, G = a, B = v
			*r = (z * 255.0) + 0.5;
			*g = (x * 255.0) + 0.5;
			*b = (v * 255.0) + 0.5;
			break;
		case 5:
			// 5: R = v, G = a, B = b
			*r = (v * 255.0) + 0.5;
			*g = (x * 255.0) + 0.5;
			*b = (y * 255.0) + 0.5;
			break;
	}
}

void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
	uint8_t max = MAX3(r, g, b);
	float maxf = max / 255.0;
	uint8_t min = MIN3(r, g, b);
	float minf = min / 255.0;

	*s = (maxf < 0.0001) ? 0 : (maxf - minf) / maxf;
	*v = maxf;

	if ((*s * 100.0) < 0.1) {
		*h = 0;
		return;
	}
	else if (r == max) {
		if (g == min) {
			// H = 5 + B' 
			*h = 5 + ((maxf - (b / 255.0)) / (maxf - minf));
		}
		else {
			// H = 1 - G'
			*h = 1 - ((maxf - (g / 255.0)) / (maxf - minf));
		}
	}
	else if (g == max) {
		if(b == min) {
		// H = R' + 1
			*h = ((maxf - (r / 255.0)) / (maxf - minf)) + 1;
		}
		else {
		// H = 3 - B'
			*h = 3 - ((maxf - (b / 255.0)) / (maxf - minf));
		}
	}
	else if (b == max && r == min) {
		// H = 3 + G'
		*h = 3 + ((maxf - (g / 255.0)) / (maxf - minf));
	}
	else {
		/* H = 5 - R' */
		*h = 5 - ((maxf - (r / 255.0)) / (maxf - minf));
	}

	*h = MIN(*h * 60, 360);
}