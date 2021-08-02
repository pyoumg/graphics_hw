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


// 2D 물체 정의 부분은 objects.h 파일로 분리
// 새로운 물체 추가 시 prepare_scene() 함수에서 해당 물체에 대한 prepare_***() 함수를 수행함.
// (필수는 아니나 올바른 코딩을 위하여) cleanup() 함수에서 해당 resource를 free 시킴.
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
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_button(i);
	}
	button_x *= -1;

	for (int i = 0; i < 3; i++) {//three right button
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산

		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_button(i + 3);
	}


	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_house();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - 1 * win_height / 5.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산
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

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_shirt();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button_x, win_height / 2.0f - win_height / 10.0f - 1 * win_height / 5.0f, 0.0f));//y좌표는 window의 맨위에서 시작해서 뺄셈으로 계산

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
	//둥근 별모양,극좌표를 이용해서 구현
	int airplane_clock = timestamp % 360; // 0 <= airplane_clock <= 719 
	//clock이 angle이기도 함
	//airplane을 심슨 뒤에 그리기
	float airplane_r;
	float airplane_x;
	float airplane_y;
	if (button_flag[1] == 0) {
		airplane_r = win_height / 2.0f * cosf(sinf(2.5 * airplane_clock * (float)TO_RADIAN));//둥근 별
	}
	else {
		airplane_r = win_height / 3.0f * (1 - sinf(airplane_clock * TO_RADIAN));//심장형 
	}


	airplane_x = airplane_r * cosf(airplane_clock * TO_RADIAN);

	airplane_y = airplane_r * sinf(airplane_clock * TO_RADIAN);
	if (button_flag[1] == 1)
		airplane_y += win_height / 5.0f;//화면에 맞도록 평행이동



	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));

	ModelMatrix = glm::rotate(ModelMatrix, airplane_clock * 10.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(sinf(airplane_clock * TO_RADIAN), sinf(airplane_clock * TO_RADIAN), 1.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();



	if (button_flag[5] == 1) {//wind pressed
		if (button_flag[0] == 0) {//옷을 입고있지 않은 상태
			button_flag[6] = 1;//대신 심슨을 동작시킴
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
	else {//flag켜져있음
		if (simpson_timer == 0) {
			simpson_timer = timestamp;//클릭한 시간
		}

		simpson_time = timestamp - simpson_timer;
		if (simpson_time > 720) {//정해진 시간 지남
			button_flag[6] = 0;
			simpson_timer = 0; //움직임 종료

			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 100, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f, 2.5f, 1.0f));


			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

			draw_simpson();//원래대로 그리기 
			glClearColor(189 / 255.0f, 236 / 255.0f, 253 / 255.0f, 1.0f);//skyblue

			if (button_flag[5] == 1) {
				button_flag[5] = 0;
			}
		}

		else {
			simpson_angle = simpson_time * 10 % 360;//한번에 10도씩 회전

			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 100.0f + simpson_time * 0.5f, 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, (float)(simpson_angle * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.5f / 720.0f * (720 - simpson_time), 2.5f / 720.0f * (720 - simpson_time), 1.0f));



			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

			draw_simpson();
			int r = 189 - (int)(simpson_time * 0.3);
			int g = 236 - (int)(simpson_time * 0.3);
			int b = 253 - (int)(simpson_time * 0.3);//g,b때문에 점점 어두워지긴 해도 완벽한 검은색은 나오지 않음
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
			shirts_timer = timestamp;//클릭한 시간
		}

		shirts_time = timestamp - shirts_timer;
		if (shirts_time > 720) {//정해진 시간 지남
			
			if (button_flag[5] == 0) {//wind아님
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -win_height / 2.5f + 230.0f, 0.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.0f, 6.0f, 1.0f));

				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

				draw_shirt();
			}
			else {//wind+ 버튼
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
			shirts_timer = 0; //움직임 종료
		if (wind_timer != 0)
			wind_timer = 0;
	}



	//house
	

	int house_clock; // -45 <= house_clock <= 45 시작 위치는 신경쓰지 않음
	float house_x, house_y;
	float scaling_house_x,scaling_house_y;
	if (button_flag[3] == 1) {//house button

		house_clock = timestamp*3 % (360 * 2); 
		house_x = -win_width / 4.5f;
		house_y = -win_height / 2.5f + 84.0f;
	

		if (house_clock < 360) {//아래->위
			house_y = (win_height/2.0f+win_height/2.5f-165.0f)/360.0f*house_clock-win_height/2.5f+42.0f;
			scaling_house_y = 1.0f / 360.0f * (house_clock % 360) + 0.5f;
		}
		else {//위->아래 
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
		
		if (house_clock < 360) {//아래->위
			house_y = (win_height / 2.0f + win_height / 2.5f - 165.0f) / 360.0f * house_clock - win_height / 2.5f + 42.0f;
			scaling_house_y = 1.0f / 360.0f * (house_clock % 360) + 0.5f;
		}
		else {//위->아래 
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
	//극좌표를 사용하여 아르키메데스 나선 그리기, 차 방향은 접선의 기울기
	if (button_flag[2] == 1) {
		if (car_timer == 0) {
			car_timer = timestamp;//클릭한 시간
		}

		car_time = timestamp - car_timer;
		car_angle = car_time % (360 * 10);//10바퀴
		car_r = 10.0f + 10.0f * (car_angle * TO_RADIAN);
		car_x = car_r * cosf(car_angle * TO_RADIAN);
		car_y = car_r * sinf(car_angle * TO_RADIAN);

		//thetha에 대해 미분
		car_det_r = 10.0f; //상수

		car_det_x = car_det_r * cosf(car_angle * TO_RADIAN) - car_r * sinf(car_angle * TO_RADIAN);
		car_det_y = car_det_r + sinf(car_angle * TO_RADIAN) + car_r * cosf(car_angle * TO_RADIAN);


		float car_rotate_angle = atanf(car_det_y / car_det_x);

		if (car_det_x > 0)
			car_rotate_angle += (float)PI;
		car_scaling = (car_angle) * 0.001f + 1.0f;

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car_x, car_y, 0.0f));

		ModelMatrix = glm::rotate(ModelMatrix, car_rotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-car_scaling,car_scaling, 1.0f)); //방향때문에 좌우반전


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
			cake_timer = timestamp;//클릭한 시간
		}
		cake_time = timestamp-cake_timer ;
	
		cake_time %= 100;
//		printf("%d\n", cake_time);
		for (int i = 0; i < 15; i++) {//15개를 떨어뜨리기
			
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
	int donut_win_width = win_width - 50;//도넛 크기 빼기
	float donut_angle;

	float donut_x;
	if (donut_clock < 900) {//왼->오
		donut_x = -donut_win_width / 2.0f + donut_win_width / 900.0f * donut_clock;
	}
	else {//오->왼
		donut_x = -donut_win_width / 2.0f + donut_win_width / 900.0f * (1800-donut_clock) ;
	}

	float temp_donut_x = (int)(donut_x + donut_win_width / 2.0f) % (donut_win_width / 5);//y좌표에 대입하기 위한 x
	float donut_y = -100 / (float)((donut_win_width / 10.0f) * (donut_win_width / 10.0f)) * (temp_donut_x - donut_win_width / 10) * (temp_donut_x - donut_win_width / 10) + 100.0f;
	donut_y += -win_height / 2.5f + 50.0f + 25.0f;//좌표계 적용+scale로 늘어난 y 적용(1.5배)

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
	if (flag && button_flag[0] * button_flag[5] == 0) {//바람/셔츠 눌러진 상태에서 하나 해제하면 다른 것도 해제 되어야 함
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

		change_x = x - win_width / 2.0f, change_y = (win_height - y) - win_height / 2.0f;//좌표계에 맞게 변화

		if (button_x < 0)
			button_x *= -1;//양수로 만들기 

		if (button_x - 50.0f <= change_x && change_x <= button_x + 50.0f) {
			for (int i = 0; i < 3; i++) {
				height = win_height / 2.0f - win_height / 10.0f - i * win_height / 5.0f;
				if (height - 50 <= change_y && change_y <= height + 50) {
					button_flag[i] = (1 + button_flag[i]) % 2;
					printf("button %d pressed\n",i+4);
				}

			}
		}
		button_x *= -1;//음수로 만들기
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
			if (button_flag[5] == 0) {//wind는 눌러지지 않은 상황
				button_flag[6] = (1 + button_flag[6]) % 2;
				printf("simpson pressed\n");
			}
		}

		if (flag && button_flag[0] * button_flag[5] == 0) {//바람/셔츠 눌러진 상태에서 하나 해제하면 다른 것도 해제 되어야 함
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

	// 키 설명 쓰기
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
	glutInitWindowSize(1200, 800);//초기 사이즈 1200*800
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


