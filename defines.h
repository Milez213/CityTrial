#ifndef DEFINES_H
#define DEFINES_H

#define PI 3.141592653589793
#define TWOPI 6.2831853071795862
#define TO_RADIANS(x) ((x) * 0.017453292519943295)
#define TO_DEGREES(x) ((x) * 57.29577951308232)

#define POS_LOCATION  0
#define NORM_LOCATION 1

#define MS_2_S (1.0f/1000.0f)

#define print3f(v, name) \
    printf("%s: %f %f %f\n", name, v.x, v.y, v.z);


// get components of Hex color
#define H2R(c)   (((c) >> 16) & 0xFF)/255.0
#define H2G(c)   (((c) >> 8) & 0xFF)/255.0
#define H2B(c)   ((c) & 0xFF)/255.0
#define H2_3f(c) H2R(c), H2G(c), H2B(c)


#endif // DEFINES_H
