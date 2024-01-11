#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb-hsv.h"

#define HEPS 1
#define SVEPS 0.1
#define RGBTOL 1
#define ABS(a) ((a) > 0 ? (a) : (-(a)))

struct rgb {
        uint8_t r, g, b;
};

struct hsv {
        float h, s, v;
};

bool feq(float a, float b, float eps)
{
        return (((a - b) < eps) || ((b - a) < eps));
}

bool hsveq(struct hsv val1, struct hsv val2)
{
        return  feq(val1.h, val2.h, HEPS) && 
                feq(val1.s, val2.s, SVEPS) && 
                feq(val1.v, val2.v, SVEPS);
}

bool rgbeq(struct rgb val1, struct rgb val2)
{
        return  ABS(val1.r - val2.r) <= RGBTOL &&
                ABS(val1.g - val2.g) <= RGBTOL &&
                ABS(val1.b - val2.b) <= RGBTOL;
}

struct rgb_hsv {
        struct rgb rgb_val;
        struct hsv hsv_val;
};

struct rgb_hsv test_rgb_hsv[] = {
        {{19, 81, 35}, {135, 0.765, 0.318}},
        {{16, 80, 3}, {110, 0.962, 0.314}},
        {{0, 0, 0}, {0.0, 0.0, 0.0}},
        {{255, 255, 255}, {0.0, 0.0, 1.0}},
        {{255, 0, 21}, {355.0, 1.0, 1.0}}
};

int main(void)
{
        for (int i = 0; i < sizeof(test_rgb_hsv) / sizeof(struct rgb_hsv); i++) {
                struct rgb rgb_test = {0, 0, 0};
                struct hsv hsv_test = {0.0, 0.0, 0.0};
                rgb_test.r = test_rgb_hsv[i].rgb_val.r;
                rgb_test.g = test_rgb_hsv[i].rgb_val.g;
                rgb_test.b = test_rgb_hsv[i].rgb_val.b;
                RGBtoHSV(rgb_test.r, rgb_test.g, rgb_test.b, 
                        &hsv_test.h, &hsv_test.s, &hsv_test.v);
                if (!hsveq(hsv_test, test_rgb_hsv[i].hsv_val)) {
                        printf("Test #%d failed (hsveq)\n", i);
                        return -1;
                }
                hsv_test.h = test_rgb_hsv[i].hsv_val.h;
                hsv_test.s = test_rgb_hsv[i].hsv_val.s;
                hsv_test.v = test_rgb_hsv[i].hsv_val.v;
                HSVtoRGB(hsv_test.h, hsv_test.s, hsv_test.v,
                        &rgb_test.r, &rgb_test.g, &rgb_test.b);
                if (!rgbeq(rgb_test, test_rgb_hsv[i].rgb_val)) {
                        printf("Test #%d failed (rgbeq)\n", i);
                        return -1;
                }
                // printf("(%f %f %f) converted to (%u %u %u)\n",  
                //         hsv_test.h, hsv_test.s, hsv_test.v,
                //         rgb_test.r, rgb_test.g, rgb_test.b);
        }
        return 0;
}