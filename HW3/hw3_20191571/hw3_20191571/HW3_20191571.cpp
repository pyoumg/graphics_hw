#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Begin of shader setup
#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables
int camera_index = 0;//default: 0
void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}
// End of shader setup

// Begin of geometry setup
#include "HW3_20191571.h"
// End of geometry setup

// Begin of Callback function definitions

// include glm/*.hpp only if necessary
// #include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

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
glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when it is ready.

bool move_ben = true, move_spider = true, move_tiger = true, move_wolf = true;
//default�� true
bool camera_translate = true;//false�� rotate



unsigned int timestamp_spider = 0, timestamp_ben = 0, timestamp_tiger = 0, timestamp_wolf = 0;
unsigned int timestamp_scene = 0; // the global clock in the scenes

void print_mat4(const char* string, glm::mat4 M) {
	fprintf(stdout, "\n***** %s ******\n", string);
	for (int i = 0; i < 4; i++)
		fprintf(stdout, "*** COL[%d] (%f, %f, %f, %f)\n", i, M[i].x, M[i].y, M[i].z, M[i].w);
	fprintf(stdout, "**************\n\n");
}

//lookAt()�� ������� �� u,v,n�� update�ϴ� �Լ�
void update_uvn(int idx) {
	camera[idx].uaxis.x = ViewMatrix[idx][0][0];
	camera[idx].uaxis.y = ViewMatrix[idx][0][1];
	camera[idx].uaxis.z = ViewMatrix[idx][0][2];

	camera[idx].vaxis.x = ViewMatrix[idx][1][0];
	camera[idx].vaxis.y = ViewMatrix[idx][1][1];
	camera[idx].vaxis.z = ViewMatrix[idx][1][2];

	camera[idx].naxis.x = ViewMatrix[idx][2][0];
	camera[idx].naxis.y = ViewMatrix[idx][2][1];
	camera[idx].naxis.z = ViewMatrix[idx][2][2];
}

void set_ViewMatrix_from_camera_frame(int idx) {
	ViewMatrix[idx] = glm::mat4(camera[idx].uaxis[0], camera[idx].vaxis[0], camera[idx].naxis[0], 0.0f,
		camera[idx].uaxis[1], camera[idx].vaxis[1], camera[idx].naxis[1], 0.0f,
		camera[idx].uaxis[2], camera[idx].vaxis[2], camera[idx].naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ViewMatrix[idx] = glm::translate(ViewMatrix[idx], glm::vec3(-camera[idx].pos[0], -camera[idx].pos[1], -camera[idx].pos[2]));
}
float tiger_x = 0, tiger_z = 0;
float tiger_rotate = 0;
float wolf_x = 0, wolf_y = 0, wolf_z = 0;
int spider_rotate = 0;
float spider_x, spider_z;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[camera_index], glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw the WC axes.
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-7.5f, -0.01f, -7.5f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(15.0f, 15.0f, 15.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_object(OBJECT_SQUARE16, 255 / 255.0f, 202 / 255.0f, 40 / 255.0f); //  draw the floor.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//static object

	//1. ironman

	ModelViewProjectionMatrix = glm::rotate(ViewProjectionMatrix[camera_index],
		2 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(1.25f, 0.685f, 3.0f));

	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_IRONMAN, 214 / 255.0f, 66 / 255.0f, 65 / 255.0f); //  red

	//2. tank
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(-1.25f, 0.0f, 1.0f));

	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		70 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		-90 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_TANK, 137 / 255.0f, 108 / 255.0f, 204 / 255.0f);

	//3. bike
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(-1.25f, 0.27f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		30 * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_BIKE, 98 / 255.0f, 217 / 255.0f, 159 / 255.0f);

	//4. bus
	ModelViewProjectionMatrix = glm::rotate(ViewProjectionMatrix[camera_index],
		2 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(1.25f, 0.0f, 3.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		90 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_BUS, 108 / 255.0f, 130 / 255.0f, 204 / 255.0f);

	//5.optimus

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(1.25f, 2.0f, -2.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		-120 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.003f, 0.003f, 0.003f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_OPTIMUS, 152 / 255.0f, 160 / 255.0f, 184 / 255.0f);

	/*dynamic*/

	//1.tiger


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
	//global var�̹Ƿ� �������� ���� ��� ���� �� ����
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(-6.5f + tiger_x, 0.0f, -6.5f + tiger_z));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		tiger_rotate * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		-90 * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.008f, 0.008f, 0.008f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_tiger();

	//2.spider
	if (move_spider)
		spider_rotate = timestamp_spider;//range 0~365
	spider_x = 5.0f * cosf(spider_rotate * TO_RADIAN);
	spider_z = 5.0f * sinf(spider_rotate * TO_RADIAN);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(spider_x, 0.0f, spider_z));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		-spider_rotate * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.2f, -0.2f, 0.2f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_spider();

	//3.ben
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
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(ben_x, -ben_z, ben_z));

	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		45 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(ben_factor, -ben_factor, ben_factor));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_ben();


	//4.wolf

	if (timestamp_wolf < 100) {
		wolf_x = -6.0f + timestamp_wolf * 12 / 100.0f;
		wolf_y = (wolf_x + 6.0f) - 12 / 10000.0f * timestamp_wolf * timestamp_wolf;
	}
	else {
		wolf_x = -6.0f + (200 - timestamp_wolf) * 12 / 100.0f;
		wolf_y = -(wolf_x - 6.0f) - 12 / 10000.0f * (timestamp_wolf - 100) * (timestamp_wolf - 100);
	}
	wolf_z = -wolf_x;

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index],
		glm::vec3(wolf_x, wolf_y, wolf_z));

	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		(135 + timestamp_wolf / 100 * 180) * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wolf();
	glutSwapBuffers();
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
		timestamp_ben = (timestamp_ben + 1) % 200;//���Ʒ�
	}
	if (move_spider) {
		timestamp_spider = (timestamp_spider + 1) % 360;//��
	}
	if (move_tiger) {
		timestamp_tiger = (timestamp_tiger + 1) % 300;//�簢��
	}
	if (move_wolf) {
		timestamp_wolf = (timestamp_wolf + 1) % 200;//������
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

//5�� ī�޶󿡼��� �۵�
enum axes { X_AXIS, Y_AXIS, Z_AXIS };
int flag_translation_axis=0;
#define CAM_TSPEED 0.05f
//translate�ϴ� �Լ�
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
		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0), -CAM_RSPEED * TO_RADIAN * angle,//-z �����̹Ƿ� ���� �ݴ� 
			glm::vec3(camera[4].naxis[0], camera[4].naxis[1], camera[4].naxis[2])));

		direction = RotationMatrix * glm::vec3(camera[4].uaxis[0], camera[4].uaxis[1], camera[4].uaxis[2]);
		camera[4].uaxis[0] = direction.x; camera[4].uaxis[1] = direction.y; camera[4].uaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(camera[4].vaxis[0], camera[4].vaxis[1], camera[4].vaxis[2]);
		camera[4].vaxis[0] = direction.x; camera[4].vaxis[1] = direction.y; camera[4].vaxis[2] = direction.z;


		break;

	}
	

}
void motion(int x, int y) {

	if (camera_index!=4) 
		return;


	if(camera_translate)
		renew_cam_position(prevy - y);
	else 
		renew_cam_orientation_rotation(prevx - x);

	prevx = x; prevy = y;


	set_ViewMatrix_from_camera_frame(4);
	ViewProjectionMatrix[4] = ProjectionMatrix[4] * ViewMatrix[4];

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
	case 'w'://wolf
		if (move_wolf)
			move_wolf = false;
		else
			move_wolf = true;
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

	case '0': //1�� ī�޶��� vrp�� �������� 
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
	glutPostRedisplay();
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


void reshape(int width, int height) {

	for (int i = 0; i < 5; i++) {
		camera[i].aspect_ratio = (float)width / height;

		ProjectionMatrix[i] = glm::perspective(camera[i].zoom_factor * camera[i].fov_y * TO_RADIAN, camera[i].aspect_ratio,
			camera[i].near_clip, camera[i].far_clip);
		ViewProjectionMatrix[i] = ProjectionMatrix[i] * ViewMatrix[i];
	}
	glViewport(0, 0, width, height);
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

	glDeleteVertexArrays(1, &ben_VAO);
	glDeleteBuffers(1, &ben_VBO);

	glDeleteVertexArrays(1, &wolf_VAO);
	glDeleteBuffers(1, &wolf_VBO);
}
// End of callback function definitions

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

#define PRINT_DEBUG_INFO  
void initialize_OpenGL(void) {


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
	camera[1].near_clip = 0.1f;
	camera[1].far_clip = 40.0f;
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
	camera[3].far_clip = 100.0f;
	camera[3].zoom_factor = 1.0f; // will be used for zoomming in and out.

	
		// initialize the 4th camera.
	camera[4].pos = glm::vec3(0.0f, 50.0f, 0.0f);

	ViewMatrix[4] = glm::lookAt(camera[4].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	update_uvn(4);
#ifdef PRINT_DEBUG_INFO
	print_mat4("Cam 5", ViewMatrix[4]);
#endif

	camera[4].fov_y = 15.0f;
	camera[4].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[4].near_clip = 0.1f;
	camera[4].far_clip = 100.0f;
	camera[4].zoom_factor = 1.0f; // will be used for zoomming in and out.
	

	glClearColor(220 / 255.0f, 226 / 255.0f, 245 / 255.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
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
}

void initialize_renderer(void) {
	initialize_OpenGL();
	register_callbacks();
	prepare_shader_program();
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

void print_message(const char* m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);


	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 3
void main(int argc, char* argv[]) {
	char program_name[64] = "HW3_20191571";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' \n\t'x' 'y' 'z' 'p' 'q' 'b' 's' 't' 'w' 'ESC'",
		"    - Special key used:  KEY-LEFT/KEY-RIGHT",
		"    - Mouse used: Left Butten Click and Move"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}
