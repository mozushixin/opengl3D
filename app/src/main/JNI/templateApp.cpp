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

#include "templateApp.h"
//加载着色器程序
#define OBJ_FILE ( char * )"model.obj"//坐标设置

#define VERTEX_SHADER ( char * )"vertex.glsl"

#define FRAGMENT_SHADER ( char * )"fragment.glsl"

#define DEBUG_SHADERS 1

OBJ *obj = NULL;

OBJMESH *objmesh = NULL;

PROGRAM *program = NULL;


TEMPLATEAPP templateApp = { templateAppInit,
							templateAppDraw,
							templateAppToucheBegan,
							templateAppToucheMoved };
//回调函数
void program_draw_callback( void *ptr ) {

	PROGRAM *curr_program = ( PROGRAM * )ptr;

	unsigned int i = 0;

	while( i != curr_program->uniform_count ) {

		if( !strcmp( curr_program->uniform_array[ i ].name, "MODELVIEWPROJECTIONMATRIX" ) ) {

			glUniformMatrix4fv( curr_program->uniform_array[ i ].location,
								1,
								GL_FALSE,
								( float * )GFX_get_modelview_projection_matrix() ); 
		}


		++i;
	}
}
//初始化opengl

void templateAppInit( int width, int height ) {

	atexit( templateAppExit );

	GFX_start();

	glViewport( 0.0f, 0.0f, width, height );

	GFX_set_matrix_mode( PROJECTION_MATRIX );
	GFX_load_identity();
	GFX_set_perspective( 45.0f,
						 ( float )width / ( float )height,
						 0.1f,
						 100.0f,
						 0.0f );
//初始化着色程序,返回值是加载成功后的index
	program = PROGRAM_create( ( char * )"default",
							  VERTEX_SHADER,
							  FRAGMENT_SHADER,
							  1,
							  DEBUG_SHADERS,
							  NULL,
							  program_draw_callback );

	obj = OBJ_load( OBJ_FILE, 1 );   


	unsigned char *vertex_array = NULL,
				  *vertex_start = NULL;

	unsigned int i	    = 0,
				 index  = 0,
				 stride = 0,
				 size   = 0;

	objmesh = &obj->objmesh[ 0 ];

	size = objmesh->n_objvertexdata * sizeof( vec3 ) * sizeof( vec3 );

	vertex_array = ( unsigned char * ) malloc( size );
	vertex_start = vertex_array;

	while( i != objmesh->n_objvertexdata ) { 

		index = objmesh->objvertexdata[ i ].vertex_index;

		memcpy( vertex_array,
				&obj->indexed_vertex[ index ],
				sizeof( vec3 ) );

		vertex_array += sizeof( vec3 );


		memcpy( vertex_array,
				&obj->indexed_normal[ index ],
				sizeof( vec3 ) );

		vertex_array += sizeof( vec3 );

		++i;
	}
//生成纹理函数，返回值就是纹理ID，指定后面要渲染的纹理对象
	glGenBuffers( 1, &objmesh->vbo ); 
	glBindBuffer( GL_ARRAY_BUFFER, objmesh->vbo );


	glBufferData( GL_ARRAY_BUFFER,
				  size,
				  vertex_start,
				  GL_STATIC_DRAW );

	free( vertex_start );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );


	glGenBuffers( 1, &objmesh->objtrianglelist[ 0 ].vbo );
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 
				  objmesh->objtrianglelist[ 0 ].vbo );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				  objmesh->objtrianglelist[ 0 ].n_indice_array *
				  sizeof( unsigned short ),
				  objmesh->objtrianglelist[ 0 ].indice_array,
				  GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


	unsigned char attribute;
				  stride = sizeof( vec3 )+
						   sizeof( vec3 );

	glGenVertexArraysOES( 1, &objmesh->vao );

	glBindVertexArrayOES( objmesh->vao );


	glBindBuffer( GL_ARRAY_BUFFER, objmesh->vbo );

	attribute = PROGRAM_get_vertex_attrib_location( program, ( char * )"POSITION" );

	glEnableVertexAttribArray( attribute );

	glVertexAttribPointer( attribute,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   stride,
						   ( void * )NULL );

	attribute = PROGRAM_get_vertex_attrib_location( program, ( char * )"NORMAL" );

	glEnableVertexAttribArray( attribute );

	glVertexAttribPointer( attribute,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   stride,
						   BUFFER_OFFSET( sizeof( vec3 ) ) );
						   
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				  objmesh->objtrianglelist[ 0 ].vbo );					   

	glBindVertexArrayOES( 0 );
}

//绘图函数
void templateAppDraw( void ) {

	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );


	GFX_set_matrix_mode( MODELVIEW_MATRIX );
	GFX_load_identity(); {
	
		vec3 e = { 0.0f, -4.0f, 0.0f },
			 c = { 0.0f,  0.0f, 0.0f },
			 u = { 0.0f,  0.0f, 1.0f };

		GFX_look_at( &e, &c, &u ); 
	}

	glBindVertexArrayOES( objmesh->vao );

	PROGRAM_draw( program );

	glDrawElements( GL_TRIANGLES,
					objmesh->objtrianglelist[ 0 ].n_indice_array,
					GL_UNSIGNED_SHORT,
					( void * )NULL );
}


void templateAppToucheBegan( float x, float y, unsigned int tap_count ) {
}


void templateAppToucheMoved( float x, float y, unsigned int tap_count ) {
}


void templateAppExit( void ) {

	SHADER_free( program->vertex_shader );
	
	SHADER_free( program->fragment_shader );
	
	PROGRAM_free( program );
	
	OBJ_free( obj );
}
