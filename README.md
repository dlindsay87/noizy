# What's that Noiz?
Noizy is an experiment with sound generation using the SDL2 library.
You use the top two alphabetic rows of your keyboard to generate sound waves.
The combined waves are displayed in the window.
Select knobs by clicking them and adjust their values with your scroll wheel. Toggle spin boxes by clicking their arrows.

**External Dependencies**
- GCC 14.2.0
- SDL 2.30.0
- OpenGL 4.6
- GLEW 2.2.0
- GLM 0.9.9.8

**The Latest**

Volume, octave, and wave select implemented. Made referencing the modded values more intuitive.

**Known Issues**

Destructors of class vector members seem to be called before class can call clean up in its destructor. Gotta investigate to prevent memory loss or something. Got some incomplete classes. Need to work on inheritance for interface items.