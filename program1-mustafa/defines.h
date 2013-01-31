#ifndef DEFINES_H
#define DEFINES_H

#define PI 3.141592653589793
#define TWOPI 6.2831853071795862
#define TO_RADIANS(x) (x * 0.017453292519943295)
#define TO_DEGREES(x) (x * 57.29577951308232)

#define POS_LOCATION  0
#define NORM_LOCATION 1

#define MS_2_S (1.0f/1000.0f)

#define print3f(v, name) \
    printf("%s: %f %f %f\n", name, v.x, v.y, v.z);


#endif // DEFINES_H
