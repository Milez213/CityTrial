#ifndef GAME_MATERIALS_H
#define GAME_MATERIALS_H


#define NUM_MATERIALS 5
#define UNSET_MATERIAL -1
#define MAGIC_MATERIAL -2

PhongMaterial g_materials[NUM_MATERIALS] = {
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0x9d5900)), // diff
                   vec3(1, 0, 0),       // spec
                   20.0},               // shine

                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0xe4000c)), // diff
                   vec3(1, 1, 1),       // spec
                   20.0},               // shine

                  {vec3(0.1, 0.1, 0.1),
                   vec3(H2_3f(0xfff852)), //Hex color to rgb
                   vec3(1, 1, 1),
                   5.0},

                  {vec3(0.3, 0.3, 0.3),
                   vec3(0, 0, 1),
                   vec3(0.1, 0.1, 0.7),
                   30.0},
                  {vec3(.1, .8, .1),  // for drawing light
                   vec3(0.1, 0.9, .1),
                   vec3(3),
                   20.0},
};





#endif /* materials.h */
