/*

Book:      	Game and Graphics Programming for iOS and Android with OpenGL(R) ES 2.0
Author:    	Romain Marucchi-Foino
ISBN-10: 	1119975913
ISBN-13: 	978-1119975915
Publisher: 	John Wiley & Sons	

Copyright (C) 2011 Romain Marucchi-Foino

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of
this software. Permission is granted to anyone who either own or purchase a copy of
the book specified above, to use this software for any purpose, including commercial
applications subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that
you wrote the original software. If you use this software in a product, an acknowledgment
in the product would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented
as being the original software.

3. This notice may not be removed or altered from any source distribution.

*/
//顶点着色器(世界坐标顶点设置)
uniform mediump mat4 MODELVIEWPROJECTIONMATRIX;

attribute mediump vec3 POSITION;

attribute lowp vec3 NORMAL;

varying lowp vec3 normal;

void main( void ) { 

   normal = ( NORMAL + 1.0 ) * 0.5;

   gl_Position = MODELVIEWPROJECTIONMATRIX * vec4( POSITION, 1.0 );
}
