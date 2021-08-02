#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"




// for Phone Shading shaders
#define NUMBER_OF_LIGHT_SUPPORTED 4 
GLint loc_global_ambient_color[2];
loc_light_Parameters loc_light[2][NUMBER_OF_LIGHT_SUPPORTED];//ps,gs
loc_Material_Parameters loc_material[2];//index 0: ps 1:gs
// lights in scene
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

GLuint h_ShaderProgram, h_ShaderProgram_PS, h_ShaderProgram_GS; // handle to shader program
GLint loc_ModelViewProjectionMatrix_PS, loc_ModelViewMatrix_PS, loc_ModelViewMatrixInvTrans_PS;
GLint loc_ModelMatrix_PS;//ps에서만 사용
GLint loc_ModelViewProjectionMatrix_GS, loc_ModelViewMatrix_GS, loc_ModelViewMatrixInvTrans_GS;
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // simple shader
GLint loc_texture, loc_flag_texture_mapping,loc_flag_arrow;

GLint loc_flag_tess;
GLint loc_flag_wave, loc_wave_freq;

GLfloat loc_spider_rotate;
int wave_flag=0, wave_freq=1, tess_flag=0, arrow_flag=0;

int camera_index = 0;//default: 0 

#include "HW5_20191571.h"
// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
void print_mat4(const char* string, glm::mat4 M);

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))


typedef struct {
	glm::vec3 pos; // in this example code, make vup always equal to the v direction.
	glm::vec3 uaxis, vaxis, naxis;
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;


#define NUMBER_OF_CAMERAS 5

CAMERA camera[NUMBER_OF_CAMERAS];

// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
glm::mat4 ViewProjectionMatrix[NUMBER_OF_CAMERAS], ViewMatrix[NUMBER_OF_CAMERAS], ProjectionMatrix[NUMBER_OF_CAMERAS];
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ModelViewMatrix_spider;
glm::mat4 ModelMatrix;

bool move_ben = true, move_spider = true, move_tiger = true, move_wolf = true;
//default는 true
bool camera_translate = true;//false면 rotate
// callbacks


unsigned int timestamp_spider = 0, timestamp_ben = 0, timestamp_tiger = 0, timestamp_wolf = 0;
unsigned int timestamp_scene = 0; // the global clock in the scenes

float tiger_x = 0, tiger_z = 0;
float tiger_rotate = 0;
float wolf_x = 0, wolf_y = 0, wolf_z = 0;
int spider_rotate = 0;
float spider_x, spider_z;
void update_lightWC();
void update_lightMC();
//camera의 uvn을 알때 viewmatrix를 구하는 함수
void set_ViewMatrix_from_camera_frame(int idx) {
	ViewMatrix[idx] = glm::mat4(camera[idx].uaxis[0], camera[idx].vaxis[0], camera[idx].naxis[0], 0.0f,
		camera[idx].uaxis[1], camera[idx].vaxis[1], camera[idx].naxis[1], 0.0f,
		camera[idx].uaxis[2], camera[idx].vaxis[2], camera[idx].naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ViewMatrix[idx] = glm::translate(ViewMatrix[idx], glm::vec3(-camera[idx].pos[0], -camera[idx].pos[1], -camera[idx].pos[2]));
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(h_ShaderProgram);
	ModelViewMatrix = glm::scale(ViewMatrix[camera_index], glm::vec3(0.5f, 0.5f, 0.5f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	glUseProgram(h_ShaderProgram_PS);
  	set_material_floor(0);
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-7.5f, -0.01f, 7.5f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(15.0f, 15.0f, 15.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	
	ModelViewMatrix = ViewMatrix[camera_index]*ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	
	glUniformMatrix4fv(loc_ModelMatrix_PS, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniform1f(loc_spider_rotate, (float)spider_rotate);
	glUniform1i(loc_flag_arrow, arrow_flag);
	glUniform1i(loc_flag_tess, tess_flag);
	glUniform1i(loc_flag_wave, wave_flag);
	glUniform1f(loc_wave_freq, (float)wave_freq);
 	draw_square();
	glUniform1i(loc_flag_tess, 0);
	glUniform1i(loc_flag_arrow, 0);
	glUniform1i(loc_flag_wave, 0);

	//static object
	glUseProgram(h_ShaderProgram);
	//1. ironman

	ModelViewProjectionMatrix = glm::rotate(ViewProjectionMatrix[camera_index],
		2 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(1.25f, 0.685f, 3.0f));

	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(1, 214 / 255.0f, 66 / 255.0f, 65 / 255.0f); //  red

	//2. tank (texture)
	glUseProgram(h_ShaderProgram_PS);
	set_material_tank(0);
	glUniform1i(loc_texture,0);
	ModelViewMatrix = glm::translate(ViewMatrix[camera_index],
		glm::vec3(-1.25f, 0.0f, 1.0f));

	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		70 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		-90 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.2f, 0.2f, 0.2f));


	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	
	glUniform1i(loc_flag_texture_mapping, 1);
	draw_tank();
	glUniform1i(loc_flag_texture_mapping, 0);
	
	
	glUseProgram(h_ShaderProgram);
	//3. bike
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(-1.25f, 0.27f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		30 * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(4, 98 / 255.0f, 217 / 255.0f, 159 / 255.0f);



	//4. bus(phong shading)

	glUseProgram(h_ShaderProgram_PS);//phong
	set_material_bus(0);

	ModelViewMatrix = glm::rotate(ViewMatrix[camera_index],
		2 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(1.25f, 0.0f, 3.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		90 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;//reshape() 때 projectionmatrix 구함


	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_bus();



	//5.optimus
	glUseProgram(h_ShaderProgram);
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(1.25f, 2.0f, -2.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		-120 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.003f, 0.003f, 0.003f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(5, 152 / 255.0f, 160 / 255.0f, 184 / 255.0f);

	/*dynamic*/

	//1.tiger(gouraud shading) //
	glUseProgram(h_ShaderProgram_GS);
	set_material_tiger(1);

	if (move_tiger) {
		if (timestamp_tiger % 75 < 50) {
			if (timestamp_tiger < 50) {
				tiger_z = timestamp_tiger * (13.0f / 50);
			}
			else if (timestamp_tiger < 125) {
				tiger_x = (timestamp_tiger - 75) * (13.0f / 50);
			}
			else if (timestamp_tiger < 200) {
				tiger_z = (200 - timestamp_tiger) * (13.0f / 50);
			}
			else {
				tiger_x = (275 - timestamp_tiger) * (13.0f / 50);
			}
		}
		else {
			tiger_rotate = (timestamp_tiger % 75 - 50) * 90.0f / 25 + timestamp_tiger / 75 * 90;
			tiger_rotate = tiger_rotate;

		}
	}

	//global var이므로 움직이지 않을 경우 기존 값 유지
	ModelViewMatrix = glm::translate(ViewMatrix[camera_index],
		glm::vec3(-6.5f + tiger_x, 0.0f, -6.5f + tiger_z));
	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		tiger_rotate * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		-90 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.008f, 0.008f, 0.008f));

	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	draw_tiger();
	

	glUniform1i(loc_flag_arrow, 1);
	glUseProgram(h_ShaderProgram);
	//2.spider
	if (move_spider) {
		spider_rotate = timestamp_spider;//range 0~365
		

	}
	spider_x = 5.0f * cosf(spider_rotate * TO_RADIAN);
	spider_z = 5.0f * sinf(spider_rotate * TO_RADIAN);



	ModelViewMatrix = glm::translate(ViewMatrix[camera_index],
		glm::vec3(spider_x, 0.0f, spider_z));
	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		-spider_rotate * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.2f, -0.2f, 0.2f));

	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	ModelViewMatrix_spider = ModelViewMatrix;
	draw_spider();
	
	update_lightMC();

	//3.ben

	glUseProgram(h_ShaderProgram);

	float ben_x = 0, ben_z = 0;
	float ben_factor;
	if (timestamp_ben < 100) {
		ben_x = timestamp_ben * -(6.0f / 100);
		ben_z = ben_x;
	}
	else {
		ben_x = (200 - timestamp_ben) * -(6.0f / 100);
		ben_z = ben_x;
	}
	ben_factor = (6.0f - ben_x) * 15.0f / 65 + 0.3f;
	ModelViewMatrix = glm::translate(ViewMatrix[camera_index],
		glm::vec3(ben_x, -ben_z, ben_z));

	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(ben_factor, -ben_factor, ben_factor));

	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;


	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	


	draw_ben();



	//4.wolf(texture)
	glUseProgram(h_ShaderProgram_PS);
	set_material_wolf(0);
	glUniform1i(loc_texture, 1);

	if (timestamp_wolf < 100) {
		wolf_x = -6.0f + timestamp_wolf * 12 / 100.0f;
		wolf_y = (wolf_x + 6.0f) - 12 / 10000.0f * timestamp_wolf * timestamp_wolf;
	}
	else {
		wolf_x = -6.0f + (200 - timestamp_wolf) * 12 / 100.0f;
		wolf_y = -(wolf_x - 6.0f) - 12 / 10000.0f * (timestamp_wolf - 100) * (timestamp_wolf - 100);
	}
	wolf_z = -wolf_x;

	ModelViewMatrix = glm::translate(ViewMatrix[camera_index],
		glm::vec3(wolf_x, wolf_y, wolf_z));

	ModelViewMatrix = glm::rotate(ModelViewMatrix,
		(135 + timestamp_wolf / 100 * 180) * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);


	glUniform1i(loc_flag_texture_mapping, 1);
	draw_wolf();
	glUniform1i(loc_flag_texture_mapping, 0);

	glUseProgram(0);

	glutSwapBuffers();
}

//5번 카메라에서만 작동
enum axes { X_AXIS, Y_AXIS, Z_AXIS };
int flag_translation_axis = 0;
#define CAM_TSPEED 0.05f
//translate하는 함수
void renew_cam_position(int del) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_tmp;

	switch (flag_translation_axis) {
	case X_AXIS:
		camera[4].pos[0] += CAM_TSPEED * del * (camera[4].uaxis[0]);
		camera[4].pos[1] += CAM_TSPEED * del * (camera[4].uaxis[1]);
		camera[4].pos[2] += CAM_TSPEED * del * (camera[4].uaxis[2]);
		break;
	case Y_AXIS:
		camera[4].pos[0] += CAM_TSPEED * del * (camera[4].vaxis[0]);
		camera[4].pos[1] += CAM_TSPEED * del * (camera[4].vaxis[1]);
		camera[4].pos[2] += CAM_TSPEED * del * (camera[4].vaxis[2]);
		break;
	case Z_AXIS:
		camera[4].pos[0] += CAM_TSPEED * del * (-camera[4].naxis[0]);
		camera[4].pos[1] += CAM_TSPEED * del * (-camera[4].naxis[1]);
		camera[4].pos[2] += CAM_TSPEED * del * (-camera[4].naxis[2]);
		break;

	}
	//printf("%f %f %f\n", camera[4].pos[0], camera[4].pos[1], camera[4].pos[2]);

}
#define CAM_RSPEED 0.5f
void renew_cam_orientation_rotation(int angle) {
	// let's get a help from glm
	glm::mat3 RotationMatrix;
	glm::vec3 direction;
	switch (flag_translation_axis) {
	case X_AXIS:

		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0), CAM_RSPEED * TO_RADIAN * angle,
			glm::vec3(camera[4].uaxis[0], camera[4].uaxis[1], camera[4].uaxis[2])));

		direction = RotationMatrix * glm::vec3(camera[4].vaxis[0], camera[4].vaxis[1], camera[4].vaxis[2]);
		camera[4].vaxis[0] = direction.x; camera[4].vaxis[1] = direction.y; camera[4].vaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(camera[4].naxis[0], camera[4].naxis[1], camera[4].naxis[2]);
		camera[4].naxis[0] = direction.x; camera[4].naxis[1] = direction.y; camera[4].naxis[2] = direction.z;



		break;
	case Y_AXIS:

		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0), CAM_RSPEED * TO_RADIAN * angle,
			glm::vec3(camera[4].vaxis[0], camera[4].vaxis[1], camera[4].vaxis[2])));

		direction = RotationMatrix * glm::vec3(camera[4].uaxis[0], camera[4].uaxis[1], camera[4].uaxis[2]);
		camera[4].uaxis[0] = direction.x; camera[4].uaxis[1] = direction.y; camera[4].uaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(camera[4].naxis[0], camera[4].naxis[1], camera[4].naxis[2]);
		camera[4].naxis[0] = direction.x; camera[4].naxis[1] = direction.y; camera[4].naxis[2] = direction.z;


		break;

	case Z_AXIS:
		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0), -CAM_RSPEED * TO_RADIAN * angle,//-z 방향이므로 각도 반대 
			glm::vec3(camera[4].naxis[0], camera[4].naxis[1], camera[4].naxis[2])));

		direction = RotationMatrix * glm::vec3(camera[4].uaxis[0], camera[4].uaxis[1], camera[4].uaxis[2]);
		camera[4].uaxis[0] = direction.x; camera[4].uaxis[1] = direction.y; camera[4].uaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(camera[4].vaxis[0], camera[4].vaxis[1], camera[4].vaxis[2]);
		camera[4].vaxis[0] = direction.x; camera[4].vaxis[1] = direction.y; camera[4].vaxis[2] = direction.z;


		break;

	}


}


#define CAM_FAR_CLIP_DISTANCE_STEP	2.0f
#define CAM_MAX_FAR_CLIP_DISTANCE	100.0f
#define CAM_ZOOM_STEP			0.05f
#define CAM_MAX_ZOOM_IN_FACTOR	0.25f
#define CAM_MAX_ZOOM_OUT_FACTOR	2.50f

void special(int key, int x, int y) {//zoom in-out
	switch (key) {

	case GLUT_KEY_LEFT://zoom in
		if (camera_index != 4)
			return;
		camera[4].zoom_factor -= CAM_ZOOM_STEP;
		if (camera[4].zoom_factor < CAM_MAX_ZOOM_IN_FACTOR)
			camera[4].zoom_factor = CAM_MAX_ZOOM_IN_FACTOR;
		ProjectionMatrix[4] = glm::perspective(camera[4].zoom_factor * camera[4].fov_y * TO_RADIAN, camera[4].aspect_ratio, camera[4].near_clip, camera[4].far_clip);
		ViewProjectionMatrix[4] = ProjectionMatrix[4] * ViewMatrix[4];
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT://zoom out
		if (camera_index != 4)
			return;
		camera[4].zoom_factor += CAM_ZOOM_STEP;
		if (camera[4].zoom_factor > CAM_MAX_ZOOM_OUT_FACTOR)
			camera[4].zoom_factor = CAM_MAX_ZOOM_OUT_FACTOR;
		ProjectionMatrix[4] = glm::perspective(camera[4].zoom_factor * camera[4].fov_y * TO_RADIAN, camera[4].aspect_ratio, camera[4].near_clip, camera[4].far_clip);
		ViewProjectionMatrix[4] = ProjectionMatrix[4] * ViewMatrix[4];
		glutPostRedisplay();
		break;
	}
}

unsigned int leftbutton_pressed = 0, rotation_mode_cow = 1;

int prevx, prevy;


void timer_scene(int value) {

	timestamp_scene = (timestamp_scene + 1) % UINT_MAX;
	cur_frame_tiger = timestamp_scene % N_TIGER_FRAMES;
	cur_frame_ben = timestamp_scene % N_BEN_FRAMES;
	cur_frame_wolf = timestamp_scene % N_WOLF_FRAMES;
	cur_frame_spider = timestamp_scene % N_SPIDER_FRAMES;
	if (move_ben) {
		timestamp_ben = (timestamp_ben + 1) % 200;//위아래
	}
	if (move_spider) {
		timestamp_spider = (timestamp_spider + 1) % 360;//원
	}
	if (move_tiger) {
		timestamp_tiger = (timestamp_tiger + 1) % 300;//사각형
	}
	if (move_wolf) {
		timestamp_wolf = (timestamp_wolf + 1) % 200;//포물선
	}
	glutTimerFunc(100, timer_scene, 0);
	glutPostRedisplay();
}
void mousepress(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		prevx = x, prevy = y;
		leftbutton_pressed = 1;
		glutPostRedisplay();
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		leftbutton_pressed = 0;
		glutPostRedisplay();
	}
}

void motion(int x, int y) {

	if (camera_index != 4)
		return;

	if (camera_translate)
		renew_cam_position(prevy - y);
	else
		renew_cam_orientation_rotation(prevx - x);

	prevx = x; prevy = y;


	set_ViewMatrix_from_camera_frame(4);


	ViewProjectionMatrix[4] = ProjectionMatrix[4] * ViewMatrix[4];
	update_lightWC();
	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &points_VAO);
	glDeleteBuffers(1, &points_VBO);

	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(N_OBJECTS, object_VAO);
	glDeleteBuffers(N_OBJECTS, object_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteVertexArrays(1, &spider_VAO);
	glDeleteBuffers(1, &spider_VBO);

	glDeleteVertexArrays(1, &bus_VAO);
	glDeleteBuffers(1, &bus_VBO);

	glDeleteVertexArrays(1, &ben_VAO);
	glDeleteBuffers(1, &ben_VBO);

	glDeleteVertexArrays(1, &wolf_VAO);
	glDeleteBuffers(1, &wolf_VBO);

	glDeleteTextures(N_TEXTURES_USED, texture_names);
}


void prepare_shader_program(void) {
	int i;
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_PS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_GS[3] = {
	{ GL_VERTEX_SHADER, "Shaders/Gouraud.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Gouraud.frag" },
	{ GL_NONE, NULL }
	};
	h_ShaderProgram = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");

	h_ShaderProgram_PS = LoadShaders(shader_info_PS);
	loc_ModelViewProjectionMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrixInvTrans");
	loc_ModelMatrix_PS= glGetUniformLocation(h_ShaderProgram_PS, "u_ModelMatrix");
	loc_global_ambient_color[0] = glGetUniformLocation(h_ShaderProgram_PS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[0][i].light_on = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[0][i].position = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[0][i].ambient_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[0][i].diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[0][i].specular_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[0][i].spot_direction = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[0][i].spot_exponent = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[0][i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[0][i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_PS, string);
	}

	loc_material[0].ambient_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.ambient_color");
	loc_material[0].diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.diffuse_color");
	loc_material[0].specular_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_color");
	loc_material[0].emissive_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.emissive_color");
	loc_material[0].specular_exponent = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_exponent");

	loc_texture = glGetUniformLocation(h_ShaderProgram_PS, "u_base_texture");

	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_PS, "u_flag_texture_mapping");
	loc_flag_arrow= glGetUniformLocation(h_ShaderProgram_PS, "u_arrow_flag");
	loc_spider_rotate = glGetUniformLocation(h_ShaderProgram_PS, "spider_rotate");
	loc_flag_wave = glGetUniformLocation(h_ShaderProgram_PS, "u_wave_flag");
	loc_wave_freq = glGetUniformLocation(h_ShaderProgram_PS, "u_wave_freq");
	loc_flag_tess = glGetUniformLocation(h_ShaderProgram_PS, "u_tess_flag");
	//gs

	h_ShaderProgram_GS = LoadShaders(shader_info_GS);
	loc_ModelViewProjectionMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrixInvTrans");

	loc_global_ambient_color[1] = glGetUniformLocation(h_ShaderProgram_GS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[1][i].light_on = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[1][i].position = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[1][i].ambient_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[1][i].diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[1][i].specular_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[1][i].spot_direction = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[1][i].spot_exponent = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[1][i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[1][i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_GS, string);
	}

	loc_material[1].ambient_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.ambient_color");
	loc_material[1].diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.diffuse_color");
	loc_material[1].specular_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_color");
	loc_material[1].emissive_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.emissive_color");
	loc_material[1].specular_exponent = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_exponent");

}


void initialize_lights_and_material(void) { // follow OpenGL conventions for initialization
	int i;

	glUseProgram(h_ShaderProgram_PS);

	glUniform4f(loc_global_ambient_color[0], 0.2f, 0.2f, 0.2f, 1.0f);
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light[0][i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light[0][i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light[0][i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light[0][i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light[0][i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light[0][i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light[0][i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light[0][i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light[0][i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light[0][i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light[0][i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}

	glUniform4f(loc_material[0].ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material[0].diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material[0].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material[0].emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material[0].specular_exponent, 0.0f); // [0.0, 128.0]


	glUseProgram(h_ShaderProgram_GS);
	glUniform4f(loc_global_ambient_color[1], 0.2f, 0.2f, 0.2f, 1.0f);
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light[1][i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light[1][i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light[1][i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light[1][i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light[1][i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light[1][i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light[1][i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light[1][i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light[1][i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light[1][i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light[1][i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}

	glUniform4f(loc_material[1].ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material[1].diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material[1].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material[1].emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material[1].specular_exponent, 0.0f); // [0.0, 128.0]
	glUseProgram(0);
}


void print_mat4(const char* string, glm::mat4 M) {
	fprintf(stdout, "\n***** %s ******\n", string);
	for (int i = 0; i < 4; i++)
		fprintf(stdout, "*** COL[%d] (%f, %f, %f, %f)\n", i, M[i].x, M[i].y, M[i].z, M[i].w);
	fprintf(stdout, "**************\n\n");
}

//lookAt()을 사용했을 때 u,v,n을 update하는 함수
void update_uvn(int idx) {
	camera[idx].uaxis.x = ViewMatrix[idx][0][0];
	camera[idx].uaxis.y = ViewMatrix[idx][1][0];
	camera[idx].uaxis.z = ViewMatrix[idx][2][0];

	camera[idx].vaxis.x = ViewMatrix[idx][0][1];
	camera[idx].vaxis.y = ViewMatrix[idx][1][1];
	camera[idx].vaxis.z = ViewMatrix[idx][2][1];

	camera[idx].naxis.x = ViewMatrix[idx][0][2];
	camera[idx].naxis.y = ViewMatrix[idx][1][2];
	camera[idx].naxis.z = ViewMatrix[idx][2][2];
}

#define PRINT_DEBUG_INFO  
void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize the 0th camera.
	camera[0].pos = glm::vec3(15.0f, 15.0f, -15.0f);

	ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	update_uvn(0);

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 1", ViewMatrix[0]);
#endif

	camera[0].fov_y = 20.0f;
	camera[0].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[0].near_clip = 0.1f;
	camera[0].far_clip = 50.0f;
	camera[0].zoom_factor = 1.0f; // will be used for zoomming in and out.

		// initialize the 1th camera.
	camera[1].pos = glm::vec3(30.0f, 20.0f, 0.0f);

	ViewMatrix[1] = glm::lookAt(camera[1].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	update_uvn(1);

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 2", ViewMatrix[1]);
#endif

	camera[1].fov_y = 15.0f;
	camera[1].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[1].near_clip = 0.5f;
	camera[1].far_clip = 80.0f;
	camera[1].zoom_factor = 1.0f; // will be used for zoomming in and out.


		// initialize the 2th camera.
	camera[2].pos = glm::vec3(0.0f, 20.0f, 30.0f);

	ViewMatrix[2] = glm::lookAt(camera[2].pos, glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	update_uvn(2);

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 3", ViewMatrix[2]);
#endif

	camera[2].fov_y = 50.0f;
	camera[2].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[2].near_clip = 0.1f;
	camera[2].far_clip = 70.0f;
	camera[2].zoom_factor = 1.0f; // will be used for zoomming in and out.


		// initialize the 3th camera.
	camera[3].pos = glm::vec3(10.0f, 50.0f, 20.0f);

	ViewMatrix[3] = glm::lookAt(camera[3].pos, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	update_uvn(3);
#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 4", ViewMatrix[3]);
#endif

	camera[3].fov_y = 20.0f;
	camera[3].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[3].near_clip = 0.1f;
	camera[3].far_clip = 70.0f;
	camera[3].zoom_factor = 1.0f; // will be used for zoomming in and out.


		// initialize the 4th camera.
	camera[4].pos = glm::vec3(30.0f, 40.0f, 30.0f);

	ViewMatrix[4] = glm::lookAt(camera[4].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	update_uvn(4);
#ifdef PRINT_DEBUG_INFO
	print_mat4("Cam 5", ViewMatrix[4]);
#endif

	camera[4].fov_y = 15.0f;
	camera[4].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[4].near_clip = 0.1f;
	camera[4].far_clip = 1000.0f;
	camera[4].zoom_factor = 1.0f; // will be used for zoomming in and out.


	initialize_lights_and_material();

	glGenTextures(N_TEXTURES_USED, texture_names);
}
void update_lightWC() {

	glUseProgram(h_ShaderProgram_PS);
	glUniform1i(loc_light[0][1].light_on, light[1].light_on);
	// need to supply position in EC for shading
	glm::vec4 position_EC = ViewMatrix[camera_index] * glm::vec4(light[1].position[0], light[1].position[1],
		light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[0][1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[0][1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[0][1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[0][1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix[camera_index]) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1],
		light[1].spot_direction[2]);
	glUniform3fv(loc_light[0][1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[0][1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[0][1].spot_exponent, light[1].spot_exponent);

	glUseProgram(h_ShaderProgram_GS);
	glUniform1i(loc_light[1][1].light_on, light[1].light_on);
	// need to supply position in EC for shading

	glUniform4fv(loc_light[1][1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[1][1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1][1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1][1].specular_color, 1, light[1].specular_color);

	//위의 계산과 동일
	glUniform3fv(loc_light[1][1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[1][1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1][1].spot_exponent, light[1].spot_exponent);

}

void update_lightMC() {
	glUseProgram(h_ShaderProgram_PS);
	glm::vec4 position_MC = ModelViewMatrix_spider * glm::vec4(light[2].position[0], light[2].position[1],
		light[2].position[2], light[2].position[3]);
	
	glUniform4fv(loc_light[0][2].position, 1, &position_MC[0]);
	glUniform4fv(loc_light[0][2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[0][2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[0][2].specular_color, 1, light[2].specular_color);

	//방향 반대라서 - 붙임
	ModelViewMatrix_spider[1] = -ModelViewMatrix_spider[1];
	glm::vec3 direction_MC = glm::mat3(ModelViewMatrix_spider) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1],
		light[2].spot_direction[2]);
	glUniform3fv(loc_light[0][2].spot_direction, 1, &direction_MC[0]);
	glUniform1f(loc_light[0][2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[0][2].spot_exponent, light[2].spot_exponent);
	
	glUseProgram(h_ShaderProgram_GS);
	glUniform1i(loc_light[1][2].light_on, light[2].light_on);

	glUniform4fv(loc_light[1][2].position, 1, &position_MC[0]);
	glUniform4fv(loc_light[1][2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[1][2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[1][2].specular_color, 1, light[2].specular_color);

	glUniform3fv(loc_light[1][2].spot_direction, 1, &direction_MC[0]);
	glUniform1f(loc_light[1][2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[1][2].spot_exponent, light[2].spot_exponent);

//	print_mat4("update", ModelViewMatrix);

	glUseProgram(0);
}
void set_up_scene_lights(void) {
	// point_light_EC: use light 0
	light[0].light_on = 1;
	light[0].position[0] = 0.0f; light[0].position[1] = 0.0f; 	// point light position in EC
	light[0].position[2] =0.0f; light[0].position[3] = 1.0f;

	light[0].ambient_color[0] = 0.2f; light[0].ambient_color[1] = 0.2f;
	light[0].ambient_color[2] = 0.2f; light[0].ambient_color[3] = 0.2f;

	light[0].diffuse_color[0] = 0.7f; light[0].diffuse_color[1] = 0.7f;
	light[0].diffuse_color[2] = 0.7f; light[0].diffuse_color[3] = 0.7f;

	light[0].specular_color[0] = 0.9f; light[0].specular_color[1] = 0.9f;
	light[0].specular_color[2] = 0.9f; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: use light 1
	light[1].light_on = 1;
	light[1].position[0] = 0.0f; light[1].position[1] = 20.0f; // spot light position in WC
	light[1].position[2] = 0.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.5f; light[1].ambient_color[1] = 0.5f;
	light[1].ambient_color[2] = 0.5f; light[1].ambient_color[3] = 1.0f;

	light[1].diffuse_color[0] = 0.7f; light[1].diffuse_color[1] = 0.7f;
	light[1].diffuse_color[2] = 0.7f; light[1].diffuse_color[3] = 1.0f;

	light[1].specular_color[0] = 0.9f; light[1].specular_color[1] = 0.9f;
	light[1].specular_color[2] = 0.9f; light[1].specular_color[3] = 1.0f;

	light[1].spot_direction[0] = 0.0f; light[1].spot_direction[1] = -1.0f;
	light[1].spot_direction[2] = 0.0f;
	light[1].spot_cutoff_angle = 5.0f;
	light[1].spot_exponent = 30.0f;

	//spot light 2: MC (spider 과 함께 움직임)

	light[2].light_on = 1;
	light[2].position[0] = 3.0f; light[2].position[1] = -5.0f; //modelMatrix에서 y축 반전을 한다 (spider의 초기위치)
	light[2].position[2] = 0.0f; light[2].position[3] = 1.0f;

	light[2].ambient_color[0] = 0.5f; light[2].ambient_color[1] = 0.5f;
	light[2].ambient_color[2] = 0.5f; light[2].ambient_color[3] = 1.0f;

	light[2].diffuse_color[0] = 0.7f; light[2].diffuse_color[1] = 0.7f;
	light[2].diffuse_color[2] = 0.7f; light[2].diffuse_color[3] = 1.0f;

	light[2].specular_color[0] = 0.9f; light[2].specular_color[1] = 0.9f;
	light[2].specular_color[2] = 0.9f; light[2].specular_color[3] = 1.0f;

	light[2].spot_direction[0] = 0.0f; light[2].spot_direction[1] = -1.0f; 
	light[2].spot_direction[2] = 0.0f;
	light[2].spot_cutoff_angle = 15.0f;
	light[2].spot_exponent = 30.0f;


	//3: EC 
	light[3].light_on = 1;
	light[3].position[0] = 0.0f; light[3].position[1] = 0.0f; 	// point light position in EC
	light[3].position[2] = 0.0f; light[3].position[3] = 1.0f;

	light[3].ambient_color[0] = 0.2f; light[3].ambient_color[1] = 0.2f;
	light[3].ambient_color[2] = 0.2f; light[3].ambient_color[3] = 0.2f;

	light[3].diffuse_color[0] = 0.7f; light[3].diffuse_color[1] = 0.7f;
	light[3].diffuse_color[2] = 0.7f; light[3].diffuse_color[3] = 0.7f;

	light[3].specular_color[0] = 0.9f; light[3].specular_color[1] = 0.9f;
	light[3].specular_color[2] = 0.9f; light[3].specular_color[3] = 1.0f;

	light[3].spot_direction[0] = 0.0f; light[3].spot_direction[1] = 0.0f;
	light[3].spot_direction[2] = -1.0f;
	light[3].spot_cutoff_angle = 5.0f;
	light[3].spot_exponent =10.0f;


	glUseProgram(h_ShaderProgram_PS);
	glUniform1i(loc_light[0][0].light_on, light[0].light_on);
	glUniform4fv(loc_light[0][0].position, 1, light[0].position);
	glUniform4fv(loc_light[0][0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[0][0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[0][0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[0][3].light_on, light[3].light_on);
	glUniform4fv(loc_light[0][3].position, 1, light[3].position);
	glUniform4fv(loc_light[0][3].ambient_color, 1, light[3].ambient_color);
	glUniform4fv(loc_light[0][3].diffuse_color, 1, light[3].diffuse_color);
	glUniform4fv(loc_light[0][3].specular_color, 1, light[3].specular_color);

	glUniform3fv(loc_light[0][3].spot_direction, 1, light[3].spot_direction);
	glUniform1f(loc_light[0][3].spot_cutoff_angle, light[3].spot_cutoff_angle);
	glUniform1f(loc_light[0][3].spot_exponent, light[3].spot_exponent);


	glUniform1i(loc_light[0][1].light_on, light[1].light_on);
	// need to supply position in EC for shading
	glm::vec4 position_EC = ViewMatrix [camera_index]* glm::vec4(light[1].position[0], light[1].position[1],
												light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[0][1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[0][1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[0][1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[0][1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix[camera_index]) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1], 
																light[1].spot_direction[2]);
	glUniform3fv(loc_light[0][1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[0][1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[0][1].spot_exponent, light[1].spot_exponent);

	glUniform1i(loc_light[0][2].light_on, light[2].light_on);
	glm::vec4 position_MC = ModelViewMatrix_spider * glm::vec4(light[2].position[0], light[2].position[1],
		light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light[0][2].position, 1, &position_MC[0]);
	glUniform4fv(loc_light[0][2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[0][2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[0][2].specular_color, 1, light[2].specular_color);

	// transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_MC = glm::mat3(ModelViewMatrix_spider) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1],
		light[2].spot_direction[2]);
	glUniform3fv(loc_light[0][2].spot_direction, 1, &direction_MC[0]);
	glUniform1f(loc_light[0][2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[0][2].spot_exponent, light[2].spot_exponent);


	glUseProgram(h_ShaderProgram_GS);
	glUniform1i(loc_light[1][0].light_on, light[0].light_on);
	glUniform4fv(loc_light[1][0].position, 1, light[0].position);
	glUniform4fv(loc_light[1][0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[1][0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[1][0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[1][3].light_on, light[3].light_on);
	glUniform4fv(loc_light[1][3].position, 1, light[3].position);
	glUniform4fv(loc_light[1][3].ambient_color, 1, light[3].ambient_color);
	glUniform4fv(loc_light[1][3].diffuse_color, 1, light[3].diffuse_color);
	glUniform4fv(loc_light[1][3].specular_color, 1, light[3].specular_color);

	glUniform3fv(loc_light[1][3].spot_direction, 1, light[3].spot_direction);
	glUniform1f(loc_light[1][3].spot_cutoff_angle, light[3].spot_cutoff_angle);
	glUniform1f(loc_light[1][3].spot_exponent, light[3].spot_exponent);

	glUniform1i(loc_light[1][1].light_on, light[1].light_on);
	// need to supply position in EC for shading

	glUniform4fv(loc_light[1][1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[1][1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1][1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1][1].specular_color, 1, light[1].specular_color);

	//위의 계산과 동일
	glUniform3fv(loc_light[1][1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[1][1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1][1].spot_exponent, light[1].spot_exponent);

	glUniform1i(loc_light[1][2].light_on, light[2].light_on);

	glUniform4fv(loc_light[1][2].position, 1, &position_MC[0]);
	glUniform4fv(loc_light[1][2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[1][2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[1][2].specular_color, 1, light[2].specular_color);

	glUniform3fv(loc_light[1][2].spot_direction, 1, &direction_MC[0]);
	glUniform1f(loc_light[1][2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[1][2].spot_exponent, light[2].spot_exponent);


	glUseProgram(0);
}


void reshape(int width, int height) {

	glViewport(0, 0, width, height);
	for (int i = 0; i < 5; i++) {
		camera[i].aspect_ratio = (float)width / height;

		ProjectionMatrix[i] = glm::perspective(camera[i].zoom_factor * camera[i].fov_y * TO_RADIAN, camera[i].aspect_ratio,
			camera[i].near_clip, camera[i].far_clip);
		ViewProjectionMatrix[i] = ProjectionMatrix[i] * ViewMatrix[i];
	}
	update_lightWC();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {


	if (key == 27) { // ESC key
		glutLeaveMainLoop(); // incur destuction callback for cleanups.
		return;
	}
	switch (key) {


	case 'b'://ben
		if (move_ben)
			move_ben = false;
		else
			move_ben = true;
		break;
	case 's'://spider
		if (move_spider)
			move_spider = false;
		else
			move_spider = true;
		break;
	case 't'://tiger
		if (move_tiger)
			move_tiger = false;
		else
			move_tiger = true;
		break;
	case 'l'://wolf
		if (move_wolf)
			move_wolf = false;
		else
			move_wolf = true;
		break;
	case 'e':
		light[0].light_on = 1 - light[0].light_on;
		light[3].light_on = light[0].light_on;
		glUseProgram(h_ShaderProgram_PS);
		glUniform1i(loc_light[0][0].light_on, light[0].light_on); 
		glUniform1i(loc_light[0][3].light_on, light[3].light_on); 
		glUseProgram(h_ShaderProgram_GS);
		glUniform1i(loc_light[1][0].light_on, light[0].light_on); 
		glUniform1i(loc_light[1][3].light_on, light[3].light_on);
		if (!light[0].light_on) {
			tess_flag = 0;
		}

		break;

	case 'w':
		light[1].light_on = 1 - light[1].light_on;
		glUseProgram(h_ShaderProgram_PS);
		glUniform1i(loc_light[0][1].light_on, light[1].light_on);
		glUseProgram(h_ShaderProgram_GS);
		glUniform1i(loc_light[1][1].light_on, light[1].light_on);
		if (!light[1].light_on) {
			arrow_flag = 0;
			wave_flag = 0;
		}
		break;
	case 'm':
		light[2].light_on = 1 - light[2].light_on;
		glUseProgram(h_ShaderProgram_PS);
		glUniform1i(loc_light[0][2].light_on, light[2].light_on);
		glUseProgram(h_ShaderProgram_GS);
		glUniform1i(loc_light[1][2].light_on, light[2].light_on);
		break;
	case 'h':
		if (!light[1].light_on)
			break;
		arrow_flag = 1 - arrow_flag;
		if (arrow_flag == 1) {
			tess_flag = 1 - arrow_flag;
			wave_flag = 1 - arrow_flag;
		}
		break;
	case 'i':
		if (!light[0].light_on)
			break;
		tess_flag = 1 - tess_flag;
		if (tess_flag == 1) {
			wave_flag = 0;
			arrow_flag = 0;
		}
		break;
	case 'j':
		if (!light[1].light_on)
			break;
		wave_flag = 1 - wave_flag;
		if (wave_flag == 1) {
			arrow_flag = 1 - wave_flag;
			tess_flag = 1 - wave_flag;
		}
		break;
	case 'f':
		if (!light[1].light_on)
			break;
		if (wave_freq == 1) {
			wave_freq = 3;

		}
		else
			wave_freq = 1;
		break;
	case '1':
		camera_index = 0;
		break;
	case '2':
		camera_index = 1;
		break;
	case '3':
		camera_index = 2;
		break;
	case '4':
		camera_index = 3;
		break;
	case '5':
		camera_index = 4;
		break;
	case '6'://look at ironman (static)
		if (camera_index != 0)
			break;

		ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(1.25f, 0.685f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		update_uvn(0);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		break;
	case '7'://look at optimus(static)
		if (camera_index != 0)
			break;

		ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(1.25f, 2.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		update_uvn(0);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		break;


	case '8'://look at spdier(dynamic)
		if (camera_index != 0)
			break;


		ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(spider_x, 0.0f, spider_z), glm::vec3(0.0f, 1.0f, 0.0f));
		update_uvn(0);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		break;
	case '9'://look at wolf(dynamic)
		if (camera_index != 0)
			break;


		ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(wolf_x, wolf_y, wolf_z), glm::vec3(0.0f, 1.0f, 0.0f));
		update_uvn(0);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		break;

	case '0': //1번 카메라의 vrp를 원점으로 
		if (camera_index != 0)
			break;
		ViewMatrix[0] = glm::lookAt(camera[0].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		update_uvn(0);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		break;
	case 'x':
		if (camera_index != 4)
			break;
		flag_translation_axis = X_AXIS;
		break;
	case 'y':
		if (camera_index != 4)
			break;
		flag_translation_axis = Y_AXIS;
		break;
	case 'z':
		if (camera_index != 4)
			break;
		flag_translation_axis = Z_AXIS;
		break;
	case 'p'://translation
		if (camera_index != 4)
			break;
		camera_translate = true;
		fprintf(stdout, "*** camera translate on ***\n");
		break;
	case 'q'://rotation
		if (camera_index != 4)
			break;
		camera_translate = false;

		fprintf(stdout, "*** camera rotate on ***\n");
		break;

	}
	update_lightWC();
	glutPostRedisplay();
	/*
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	case 'c':
		flag_cull_face = (flag_cull_face + 1) % 3;
		switch (flag_cull_face) {
		case 0:
			glDisable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 1: // cull back faces;
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 2: // cull front faces;
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		}
		break;
	case 'p':
		flag_polygon_mode = 1 - flag_polygon_mode;
		if (flag_polygon_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	}
	*/
}


void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mousepress);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}
void prepare_scene(void) {
	prepare_points();
	prepare_axes();
	prepare_square();
	prepare_bus();
	prepare_tank();
	prepare_bike();
	prepare_ironman();
	prepare_optimus();
	prepare_ben();
	prepare_tiger();
	prepare_wolf();
	prepare_spider();
	set_up_scene_lights();

}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);


	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}


#define N_MESSAGE_LINES 3
void main(int argc, char* argv[]) {
	char program_name[64] = "HW5_20191571";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0'~'9' \n\t'x' 'y' 'z' 'p' 'q' 'b' 's' 't' 'l' \n\t'h' 'i' 'j' 'f' 'w' 'm' 'e'\n\t'ESC'",
		"    - Special key used:  KEY-LEFT/KEY-RIGHT",
		"    - Mouse used: Left Butten Click and Move"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}
