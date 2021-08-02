#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define PI 3.141592
#define LOC_VERTEX 0

int win_width = 0, win_height = 0;
float centerx = 0.0f, centery = 0.0f;

float button_x = 0.0f;

unsigned int simpson_timer = 0;//default
unsigned int shirts_timer = 0;
unsigned int wind_timer = 0;
unsigned int car_timer = 0;
unsigned int cake_timer = 0;


// 2D ��ü ���� �κ��� objects.h ���Ϸ� �и�
// ���ο� ��ü �߰� �� prepare_scene() �Լ����� �ش� ��ü�� ���� prepare_***() �Լ��� ������.
// (�ʼ��� �ƴϳ� �ùٸ� �ڵ��� ���Ͽ�) cleanup() �Լ����� �ش� resource�� free ��Ŵ.
#include "objects.h"

unsigned int timestamp = 0;
void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void display(void) {
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_ground();

	button_x = win_width / 2.0f * 0.8f;

	for (int i = 0; i < 3; i++) {//three right button
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_button(i);
	}
	button_x *= -1;

	for (int i = 0; i < 3; i++) {//three right button
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_button(i + 3);
	}


	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_house();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - 1 * win_height / 5.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_cake();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x + 20.f, win_height / 2.0f - win_height / 10.0f - 2 * win_height / 5.0f + 20.0f, 0.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_wind();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x - 10.0f, win_height / 2.0f - win_height / 10.0f - 2 * win_height / 5.0f - 15.0f, 0.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_wind();

	button_x *= -1;

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_shirt();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - 1 * win_height / 5.0f, 0.0f));//y��ǥ�� window�� �������� �����ؼ� �������� ���

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_airplane();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - 2 * win_height / 5.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	draw_car2();


	/*button end*/

	
	/*airplane*/
	//�ձ� �����,����ǥ�� �̿��ؼ� ����
	int airplane_clock = timestamp % 360; // 0 <= airplane_clock <= 719 
	//clock�� angle�̱⵵ ��
	//airplane�� �ɽ� �ڿ� �׸���
	float airplane_r;
	float airplane_x;
	float airplane_y;
	if (button_flag[1] == 0) {
		airplane_r = win_height / 2.0f * cosf(sinf(2.5 * airplane_clock * (float)TO_RADIAN));//�ձ� ��
	}
	else {
		airplane_r = win_height / 3.0f * (1 - sinf(airplane_clock * TO_RADIAN));//������ 
	}


	airplane_x = airplane_r * cosf(airplane_clock * TO_RADIAN);

	airplane_y = airplane_r * sinf(airplane_clock * TO_RADIAN);
	if (button_flag[1] == 1)
		airplane_y += win_height / 5.0f;//ȭ�鿡 �µ��� �����̵�



	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));

	ModelMatrix = glm::rotate(ModelMatrix, airplane_clock * 10.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(sinf(airplane_clock * TO_RADIAN), sinf(airplane_clock * TO_RADIAN), 1.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();



	if (button_flag[5] == 1) {//wind pressed
		if (button_flag[0] == 0) {//���� �԰����� ���� ����
			button_flag[6] = 1;//��� �ɽ��� ���۽�Ŵ
		}
	}



	//simpson

	unsigned int simpson_time;
	int simpson_angle;
	if (button_flag[6] == 0) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 100, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));


		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		draw_simpson();
	}
	else {//flag��������
		if (simpson_timer == 0) {
			simpson_timer = timestamp;//Ŭ���� �ð�
		}

		simpson_time = timestamp - simpson_timer;
		if (simpson_time > 720) {//������ �ð� ����
			button_flag[6] = 0;
			simpson_timer = 0; //������ ����

			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 100, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));


			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

			draw_simpson();//������� �׸��� 
			glClearColor(189 / 255.0f, 236 / 255.0f, 253 / 255.0f, 1.0f);//skyblue

			if (button_flag[5] == 1) {
				button_flag[5] = 0;
			}
		}

		else {
			simpson_angle = simpson_time * 10 % 360;//�ѹ��� 10���� ȸ��

			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 100.0f + simpson_time * 0.5f, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, (float)(simpson_angle * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f / 720.0f * (720 - simpson_time), 2.5f / 720.0f * (720 - simpson_time), 1.0f));



			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

			draw_simpson();
			int r = 189 - (int)(simpson_time * 0.3);
			int g = 236 - (int)(simpson_time * 0.3);
			int b = 253 - (int)(simpson_time * 0.3);//g,b������ ���� ��ο����� �ص� �Ϻ��� �������� ������ ����
			r = r <= 0 ? 0 : r;
			g = g <= 0 ? 0 : g;
			b = b <= 0 ? 0 : b;
			glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);//skyblue

		}


	}

	//shirts

	unsigned int shirts_time;
	unsigned int wind_time;

	float shirts_x, shirts_y;
	float shirts_lotate_angle;
	if (button_flag[0] == 1) {
		if (shirts_timer == 0) {
			shirts_timer = timestamp;//Ŭ���� �ð�
		}

		shirts_time = timestamp - shirts_timer;
		if (shirts_time > 720) {//������ �ð� ����
			
			if (button_flag[5] == 0) {//wind�ƴ�
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 230.0f, 0.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f, 6.0f, 1.0f));

				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

				draw_shirt();
			}
			else {//wind+ ��ư
				if (wind_timer == 0)
					wind_timer = timestamp;
				wind_time = timestamp - wind_timer;
				if (wind_time <= 720) {
					shirts_x = wind_time / 720.0f * (win_width / 3.0f);
					shirts_y = win_height/2.0f+(720-wind_time)/720.0f*(230.0f-win_height/2.0f-win_height/2.5f);
					shirts_lotate_angle = atanf(shirts_y / shirts_x);
					if (shirts_lotate_angle < 0)
						shirts_lotate_angle = 0.0f;
					ModelMatrix = glm::rotate(glm::mat4(1.0f), shirts_lotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
					ModelMatrix = glm::translate(ModelMatrix, glm::vec3(shirts_x, 100.0f * sinf(shirts_x / 30) - 100.0f, 0.0f));
					ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f, 6.0f, 1.0f));



					ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
					glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

					draw_shirt();
				}
			}

		}
		else {

			shirts_x = win_width / 3.0f  - shirts_time / 720.0f * (win_width / 3.0f );
			shirts_y = win_height / 2.0f +shirts_time / 720.0f * (+230.0f - win_height / 2.0f - win_height / 2.5f);
			shirts_lotate_angle = atanf(shirts_y / shirts_x);
	//		printf("%f %f %f\n", shirts_x, shirts_y,shirts_lotate_angle);
			if (shirts_lotate_angle < 0)
				shirts_lotate_angle = 0.0f;

	//		ModelMatrix = glm::rotate(ModelMatrix, (float)(shirts_angle * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::rotate(glm::mat4(1.0f), shirts_lotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(shirts_x, 100.0f*sinf(shirts_x/30)-100.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f,6.0f, 1.0f));

		

			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

			draw_shirt();


		}

	}
	else {
		if(shirts_timer!=0)
			shirts_timer = 0; //������ ����
		if (wind_timer != 0)
			wind_timer = 0;
	}



	//house
	

	int house_clock; // -45 <= house_clock <= 45 ���� ��ġ�� �Ű澲�� ����
	float house_x, house_y;
	float scaling_house_x,scaling_house_y;
	if (button_flag[3] == 1) {//house button

		house_clock = timestamp*3 % (360 * 2); 
		house_x = -win_width / 4.5f;
		house_y = -win_height / 2.5f + 84.0f;
	

		if (house_clock < 360) {//�Ʒ�->��
			house_y = (win_height/2.0f+win_height/2.5f-165.0f)/360.0f*house_clock-win_height/2.5f+42.0f;
			scaling_house_y = 1.0f / 360.0f * (house_clock % 360) + 0.5f;
		}
		else {//��->�Ʒ� 
			house_y = (win_height / 2.0f + win_height / 2.5f - 165.0f) / 360.0f * (720-house_clock) - win_height / 2.5f + 42.0f;
		//	printf("%f\n", house_y);
			scaling_house_y = 1.0f / 360.0f * ((720-house_clock) % 360) + 0.5f;
		}
		
		if (house_y>0&&scaling_house_y < 1.0f)
			scaling_house_y = 1.5f;

		scaling_house_x = 1.0f/ scaling_house_y;


		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));
		
	
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scaling_house_x, scaling_house_y, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f, 6.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();




		house_clock = (house_clock + 360) % (360 * 2);
		house_x =0.0f;

		if (house_clock < 360) {
			house_y = house_clock*(win_height / 2.0f + win_height / 2.5f  - 168 * 2.5f-14*2*1.5f -12*2*0.5f ) / 360.0f  - win_height / 2.5f + (128.0f+40.0f) * 2.5f + 14 * 2 * 1.5f;
			//	printf("%f\n", house_y);
			scaling_house_y = 1.0f / 360.0f * ((360 - house_clock) % 360) + 0.5f;
		}
		else { 


			house_y = (720 - house_clock)*(win_height / 2.0f + win_height / 2.5f  - 168 * 2.5f - 14 * 2 * 1.5f - 12 * 2 * 0.5f) / 360.0f  - win_height / 2.5f + 168.0f * 2.5f + 14 * 2 * 1.5f;

			scaling_house_y = 1.0f / 360.0f * (house_clock % 360) + 0.5f;
		}

		if (house_y <= win_height / 5.0f && scaling_house_y < 1.0f)
			scaling_house_y = 1.5f;
		
		scaling_house_x =1.0f/scaling_house_y;
	//	printf("%f %f\n", house_x, house_y);

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));

		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scaling_house_x, scaling_house_y, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();












		house_clock = (house_clock + 360) % (360 * 2);
		house_x = win_width / 4.5f;
		
		if (house_clock < 360) {//�Ʒ�->��
			house_y = (win_height / 2.0f + win_height / 2.5f - 165.0f) / 360.0f * house_clock - win_height / 2.5f + 42.0f;
			scaling_house_y = 1.0f / 360.0f * (house_clock % 360) + 0.5f;
		}
		else {//��->�Ʒ� 
			house_y = (win_height / 2.0f + win_height / 2.5f - 165.0f) / 360.0f * (720 - house_clock) - win_height / 2.5f + 42.0f;
			//	printf("%f\n", house_y);
			scaling_house_y = 1.0f / 360.0f * ((720 - house_clock) % 360) + 0.5f;
		}

		if (house_y > 0 && scaling_house_y < 1.0f)
			scaling_house_y = 1.5f;

		scaling_house_x =1.0f/ scaling_house_y;

		
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));

		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scaling_house_x, scaling_house_y, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f, 6.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();

	}

	float car_x;
	float car_y;

	float car_r;
	float car_det_x;
	float car_det_y;
	float car_det_r;
	int car_time;
	int car_angle;
	float car_scaling;
	//car 
	//����ǥ�� ����Ͽ� �Ƹ�Ű�޵��� ���� �׸���, �� ������ ������ ����
	if (button_flag[2] == 1) {
		if (car_timer == 0) {
			car_timer = timestamp;//Ŭ���� �ð�
		}

		car_time = timestamp - car_timer;
		car_angle = car_time % (360 * 10);//10����
		car_r = 10.0f + 10.0f * (car_angle * TO_RADIAN);
		car_x = car_r * cosf(car_angle * TO_RADIAN);
		car_y = car_r * sinf(car_angle * TO_RADIAN);

		//thetha�� ���� �̺�
		car_det_r = 10.0f; //���

		car_det_x = car_det_r * cosf(car_angle * TO_RADIAN) - car_r * sinf(car_angle * TO_RADIAN);
		car_det_y = car_det_r + sinf(car_angle * TO_RADIAN) + car_r * cosf(car_angle * TO_RADIAN);


		float car_rotate_angle = atanf(car_det_y / car_det_x);

		if (car_det_x > 0)
			car_rotate_angle += (float)PI;
		car_scaling = (car_angle) * 0.001f + 1.0f;

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car_x, car_y, 0.0f));

		ModelMatrix = glm::rotate(ModelMatrix, car_rotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-car_scaling,car_scaling, 1.0f)); //���⶧���� �¿����


		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car2();


	}
	else if (car_timer != 0)
		car_timer = 0;

	unsigned int cake_time;
	float cake_x, cake_y;
	//cake
	float scale_cake;

	if (button_flag[4] == 1) {
		if (cake_timer == 0) {
			cake_timer = timestamp;//Ŭ���� �ð�
		}
		cake_time = timestamp-cake_timer ;
	
		cake_time %= 100;
//		printf("%d\n", cake_time);
		for (int i = 0; i < 15; i++) {//15���� ����߸���
			
			cake_x = -win_width / 2.0f + win_width / 17.0f * (i + 1);
			cake_y = -(win_height/2.0f+win_height/2.5f ) / 100.0f * (cake_time+1) +win_height/2.0f;
			
			if (i % 2 == 0) {
				cake_x += 100.0f / 100.0f * (cake_time);
			}
			else {
				cake_x -= 100.0f / 100.0f * (cake_time);
			}
			scale_cake = cake_time % 10 / 5.0f;
			cake_time += 3;
			cake_time %= 100;

			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cake_x, cake_y, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale_cake, scale_cake, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_cake();
		}
	}


	

	int donut_clock = timestamp % (360 * 5); // 0<= donut_clock < 1800	
	int jump_idx = timestamp / 360;
	//	float start_x = jump_idx * (win_width / 4.0f)-win_width/2.0f;
	//	float start_x = -win_width / 2.0f;
	float start_y = -win_height / 2.5f + 50.0f;
	int donut_win_width = win_width - 50;//���� ũ�� ����
	float donut_angle;

	float donut_x;
	if (donut_clock < 900) {//��->��
		donut_x = -donut_win_width / 2.0f + donut_win_width / 900.0f * donut_clock;
	}
	else {//��->��
		donut_x = -donut_win_width / 2.0f + donut_win_width / 900.0f * (1800-donut_clock) ;
	}

	float temp_donut_x = (int)(donut_x + donut_win_width / 2.0f) % (donut_win_width / 5);//y��ǥ�� �����ϱ� ���� x
	float donut_y = -100 / (float)((donut_win_width / 10.0f) * (donut_win_width / 10.0f)) * (temp_donut_x - donut_win_width / 10) * (temp_donut_x - donut_win_width / 10) + 100.0f;
	donut_y += -win_height / 2.5f + 50.0f + 25.0f;//��ǥ�� ����+scale�� �þ y ����(1.5��)

	float scale_donut_x;
	if (temp_donut_x <= (donut_win_width / 10)) {
		scale_donut_x = 0.5f + 1.0f * temp_donut_x / ((donut_win_width) / 10.0f);

	}
	else {
		scale_donut_x = 1.0f * (temp_donut_x - (donut_win_width / 10.0f)) / ((donut_win_width) / 10.0f);

		scale_donut_x = 1.5f - scale_donut_x;
	}
	float scale_donut_y = 2.0f - scale_donut_x;

	donut_angle = -donut_clock % 360 * TO_RADIAN;
	if (donut_clock > 900)
		donut_angle *= -1;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(donut_x, donut_y, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale_donut_x, scale_donut_y, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, donut_angle, glm::vec3(0.0f, 0.0f, 1.0f));



	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_donut();







	 
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	bool flag = false;
	if (button_flag[0] == 1 && button_flag[5] == 1)
		flag = true;

	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case '1' ://1
		button_flag[3] = (1 + button_flag[3]) % 2;
		printf("button 1 pressed\n");
	
		break;
	case '2'://2
		button_flag[4] = (1 + button_flag[4]) % 2;
		printf("button 2 pressed\n");
		break;
	case '3'://3
		button_flag[5] = (1 + button_flag[5]) % 2;
		printf("button 3 pressed\n");
		break;
	case '4'://4
		button_flag[0] = (1 + button_flag[0]) % 2;
		printf("button 4 pressed\n");
		break;
	case '5'://5
		button_flag[1] = (1 + button_flag[1]) % 2;
		printf("button 5 pressed\n");
		break;
	case '6'://6
		button_flag[2] = (1 + button_flag[2]) % 2;
		printf("button 6 pressed\n");
		break;
	case 'S'://S
		button_flag[6] = (1 + button_flag[6]) % 2;
		printf("simpson pressed\n");
		break;
	case 's'://s
		button_flag[6] = (1 + button_flag[6]) % 2;
		printf("simpson pressed\n");
		break;
	default:
		break;
	
	}
	if (flag && button_flag[0] * button_flag[5] == 0) {//�ٶ�/���� ������ ���¿��� �ϳ� �����ϸ� �ٸ� �͵� ���� �Ǿ�� ��
		button_flag[0] = 0;
		button_flag[5] = 0;
	}

}

int leftbuttonpressed = 0;
void mouse(int button, int state, int x, int y) {
	float height;
	int change_x, change_y;
	bool flag=false;
	if (button_flag[0] == 1 && button_flag[5] == 1)
		flag = true;
	

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		leftbuttonpressed = 1;
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {

		change_x = x - win_width / 2.0f, change_y = (win_height - y) - win_height / 2.0f;//��ǥ�迡 �°� ��ȭ

		if (button_x < 0)
			button_x *= -1;//����� ����� 

		if (button_x - 50.0f <= change_x && change_x <= button_x + 50.0f) {
			for (int i = 0; i < 3; i++) {
				height = win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f;
				if (height - 50 <= change_y && change_y <= height + 50) {
					button_flag[i] = (1 + button_flag[i]) % 2;
					printf("button %d pressed\n",i+4);
				}

			}
		}
		button_x *= -1;//������ �����
		if (button_x - 50.0f <= change_x && change_x <= button_x + 50.0f) {
			for (int i = 0; i < 3; i++) {
				height = win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f;
				if (height - 50 <= change_y && change_y <= height + 50) {
					button_flag[i + 3] = (1 + button_flag[i + 3]) % 2;
					printf("button %d pressed\n",i+1);
				}

			}
		}

		if (-win_height / 2.5f <= change_y && change_y <= -win_height / 2.5f + 170.0f * 2.5f && -60.0f * 2.5f <= change_x && change_x <= 60.0f * 2.5f) {
			if (button_flag[5] == 0) {//wind�� �������� ���� ��Ȳ
				button_flag[6] = (1 + button_flag[6]) % 2;
				printf("simpson pressed\n");
			}
		}

		if (flag && button_flag[0] * button_flag[5] == 0) {//�ٶ�/���� ������ ���¿��� �ϳ� �����ϸ� �ٸ� �͵� ���� �Ǿ�� ��
			button_flag[0] = 0;
			button_flag[5] = 0;
		}

	}

}

void motion(int x, int y) {


	

}

void reshape(int width, int height) {
	win_width = width, win_height = height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


	update_ground();
	glutPostRedisplay();
}

void cleanup(void) {

	

	glDeleteVertexArrays(1, &VAO_ground);
	glDeleteBuffers(1, &VBO_ground);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);

	glDeleteVertexArrays(1, &VAO_cake);
	glDeleteBuffers(1, &VBO_cake);

	glDeleteVertexArrays(1, &VAO_car2);
	glDeleteBuffers(1, &VBO_car2);

	glDeleteVertexArrays(1, &VAO_shirt);
	glDeleteBuffers(1, &VBO_shirt);


	glDeleteVertexArrays(1, &VAO_simpson);
	glDeleteBuffers(1, &VBO_simpson);

	glDeleteVertexArrays(1, &VAO_donut);
	glDeleteBuffers(1, &VBO_donut);

	glDeleteVertexArrays(1, &VAO_wind);
	glDeleteBuffers(1, &VBO_wind);

	glDeleteVertexArrays(1, &VAO_button);
	glDeleteBuffers(1, &VBO_button);

}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

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

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(189 / 255.0f, 236 / 255.0f, 253 / 255.0f, 1.0f);//skyblue
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {

	prepare_ground();
	prepare_button();
	prepare_donut();
	prepare_cake();
	prepare_car2();
	prepare_wind();
	prepare_shirt();
	prepare_airplane();
	prepare_house();
	prepare_simpson();
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

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);

	// Ű ���� ����
	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
int main(int argc, char* argv[]) {
	char program_name[64] = "HW2_20191571";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC' '1' '2' '3' '4' '5' '6' 's' 'S'"
		"    - Mouse used: L-click"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);//�ʱ� ������ 1200*800
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


