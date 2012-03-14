Phong's lighting implementation in OpenGL
=========================================

I applied Phong's lighting equation so it can be compared with the OpenGL implementation. 
In order to invert the normals press I, and in order to switch between implementations press L.

What is the lighting equation?
-----------------------------

Though the physical parameters are the same in real world and OpenGL, but
OpenGL has more parameters to tune the lighting so that people who use it have
more freedom to get what they want (rather than what real objects are).
For example: in addition to having global ambient light, OpenGL also allows
“ambient” component for every light. Further, material can also have an
“emission” component (such that an object can be lit even when there is no other
light source). The ambient (ka), diffuse (kd) and specular (ks) constants are
actually arrays of 4 constants each (for the RGBA channels). Instead of one color
per light, OpenGL defines different colors, for the ambient, diffuse, and specular
components. You can probably model diffuse light like fluorescence light, or a
specular light like laser rays using these modeling parameters.
The OpenGL equation for lighting is:
Vertex color = (material emission) + (global ambient)*(material ambient) +
Σ (1/( k1+ k2d + k3d 2)) * [(light ambient)*(material ambient) + (max{L.N, 0})*(light
diffuse)*(material diffuse) + (max{H.N,0})n *(light specular)*(material specular) ]
H=(L+V)/|L+V|, where L is the unit vector towards the light source, and V is the
unit vector towards the eye. This is called the half-vector between L and V.
N is the normal vector. n defines the shininess. The “max” functions will make
sure that the light is not shining in the back-side of the face. If this value goes
beyond 1, it is clamped to 1 (lighting has been saturated at that point).

