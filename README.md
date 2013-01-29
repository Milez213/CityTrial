Kart Part Park
==============

CPE 476 Final Project - Kart Part Park

Synopsis
--------

Open world, surreal kart game with emphasis on exploration and collecting kart parts.


TODO
----

GameDrawableObject - Bryan
   -implement vitural collide function
      -Takes Actor
   -pass physics Actor Bounding information

GameKartObject - Chris
   -override collide function
      -Collides with upgrades
         -upgrades kart stats (topSpeed, turningRadius, acceleration, etc)
      -collides with scenery
         -nullifies velocity
   -Change velocity based on control stick (See BBPlayerObject in Package for how to do this)

Shaders - Mustafa
   -Implement Phong Shading
      -Allow objects access to normal and texture handlers

Lighting/Camera - Mustafa
   -One Light "Sun"
   -Initialize camera object
      -LookAt KartObject
   

More in https://github.com/Milez213/CityTrial/wiki/What-we-need-for-25%25

* Camera controls (with springs for smooth movement?) using keyboard and joystick
* Work on structure of objects
* kart physics
* ???
* Sound
