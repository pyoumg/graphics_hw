#pragma once


//ground
GLfloat ground[4][2];
GLfloat ground_color[3]= { 134 / 255.0f, 89 / 255.0f, 45 / 255.0f };//brown
GLuint VBO_ground, VAO_ground;

void prepare_ground(void) {//땅그리기 (좌표계 변환 안함)
	ground[0][0] = -win_width / 2.0f; ground[0][1] = -win_height / 2.5f; //왼쪽 위
	ground[1][0] = win_width / 2.0f; ground[1][1] = -win_height / 2.5f;//오른쪽 위
	ground[2][0] = win_width / 2.0f; ground[2][1] = -win_height /2.0f;//오른쪽 아래
	ground[3][0] = -win_width / 2.0f; ground[3][1] = -win_height / 2.0f;//왼쪽 아래 



	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_ground(void) {
	ground[0][0] = -win_width / 2.0f; ground[0][1] = -win_height / 2.5f; //왼쪽 위
	ground[1][0] = win_width / 2.0f; ground[1][1] = -win_height / 2.5f;//오른쪽 위
	ground[2][0] = win_width / 2.0f; ground[2][1] = -win_height / 2.0f;//오른쪽 아래
	ground[3][0] = -win_width / 2.0f; ground[3][1] = -win_height / 2.0f;//왼쪽 아래 

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_ground(void) {
	glUniform3fv(loc_primitive_color, 1, ground_color);
	glBindVertexArray(VAO_ground);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

//wind

GLfloat wind[8][2] = { {-33.0f,10.0f},{-23.6f,-8.4f},{-13.5f,-17.8f},{0,-21.8f},{16.4f,-18.7f},{23.2f,-3.6f},{14.6f,5.8f},{3.1f,1.6f} };
GLfloat wind_color[3] = { 0 / 255.0f,0 / 255.0f, 0 / 255.0f };//black
GLuint VBO_wind, VAO_wind;
void prepare_wind(void) {//버튼 그리기 

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_wind);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_wind);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wind), wind, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_wind);
	glBindVertexArray(VAO_wind);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_wind);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void draw_wind(void) {
	glUniform3fv(loc_primitive_color, 1, wind_color);

	glBindVertexArray(VAO_wind);
	glDrawArrays(GL_LINE_STRIP, 0, 8);
	glBindVertexArray(0);
}
//button

int button_flag[7] = { 0, };
//0~5 button 6 simpson 
GLfloat button[4][2] = { {-50.0,50.0},{50.0 ,50.0} ,{50.0,-50.0},{-50.0,-50.0 } };
GLfloat button_color[2][3] = { 
	{147 / 255.0f, 207 / 255.0f, 173 / 255.0f},//default
	{76 / 255.0f, 110 / 255.0f, 90 / 255.0f}
};//default,floating(or click)

GLuint VBO_button, VAO_button;

void prepare_button(void) {//버튼 그리기 

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_button);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_button);
	glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_button);
	glBindVertexArray(VAO_button);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_button);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void draw_button(int idx) {
	glUniform3fv(loc_primitive_color, 1, button_color[button_flag[idx]]);

	glBindVertexArray(VAO_button);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

//donut
#define DONUT_OUT 0
#define DONUT_CREAM 1
#define DONUT_BLUE 2
#define DONUT_GREEN 3

GLfloat donut_out[34][2];//16각형의 바깥+안(안이 비어있는 구조)
GLfloat donut_cream[18][2];
GLfloat donut_blue[4][2] = { {-26.0f,21.2f}, {-29.0f,25.1f} ,{-17.7f,34.3f},{-15.0f,30.0f} };
GLfloat donut_green[4][2] = { {27.6f,25.8f},{23.7f,25.6f},{32.5f,15.9f} ,{36.1f,19.7f} };

GLfloat donut_color[5][3] = {
	{ 233 / 255.0f, 164 / 255.0f, 86 / 255.0f },  // out
	{ 252 / 255.0f, 138 / 255.0f, 161 / 255.0f },  // cream
	{ 27 / 255.0f, 199 / 255.0f, 242 / 255.0f },  // blue
	{ 20 / 255.0f, 240 / 255.0f, 100 / 255.0f }  // green
};
GLuint VBO_donut, VAO_donut;

void prepare_donut() {
	
	for (int i = 0; i < 17; i++) {
		donut_out[i * 2][0] = 50 * cosf(360 / 16.0f *i* TO_RADIAN);
		donut_out[i * 2][1] = 50 * sinf(360 / 16.0f *i* TO_RADIAN);

		donut_out[i * 2+1][0] = 20 * cosf(360 / 16.0f *i* TO_RADIAN);
		donut_out[i * 2+1][1] = 20 * sinf(360 / 16.0f *i* TO_RADIAN);

	}
	

	for (int i = 0; i < 8; i++) {
		donut_cream[i*2][0] = donut_out[i * 2][0];
		donut_cream[i*2][1] = donut_out[i * 2][1];
		donut_cream[i * 2 + 1][0] = donut_out[i * 2+1][0];
		donut_cream[i * 2 + 1][1] = donut_out[i * 2+1][1];
	}


	



	GLsizeiptr buffer_size = sizeof(donut_out) + sizeof(donut_cream) + sizeof(donut_blue)+ sizeof(donut_green) ;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_donut);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_donut);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(donut_out), donut_out);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(donut_out), sizeof(donut_cream), donut_cream);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(donut_out)+ sizeof(donut_cream), sizeof(donut_blue), donut_blue);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(donut_out) + sizeof(donut_cream) + sizeof(donut_blue), sizeof(donut_green), donut_green);
	


	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_donut);
	glBindVertexArray(VAO_donut);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_donut);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_donut() { // Draw donut in its MC.
	glBindVertexArray(VAO_donut);

	glUniform3fv(loc_primitive_color, 1, donut_color[DONUT_OUT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 34);
	
	glUniform3fv(loc_primitive_color, 1, donut_color[DONUT_CREAM]);
	glDrawArrays(GL_TRIANGLE_STRIP, 34, 18);
	glUniform3fv(loc_primitive_color, 1, donut_color[DONUT_BLUE]);
	glDrawArrays(GL_TRIANGLE_FAN, 52, 4);
	glUniform3fv(loc_primitive_color, 1, donut_color[DONUT_GREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 56, 4);

	


	glBindVertexArray(0);
}




#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 }, { -20.0, 15.0 }, { -20.0, 20.0 }, { 0.0, 23.0 }, { 20.0, 20.0 }, { 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 }, { -11.0, -12.0 }, { -12.0, -7.0 }, { 0.0, -10.0 }, { 12.0, -7.0 }, { 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 }, { -6.0, 0.0 }, { -6.0, 22.0 }, { 6.0, 22.0 }, { 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 }, { -7.0, 24.0 }, { -7.0, 21.0 }, { 7.0, 21.0 }, { 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 }, { -18.0, 3.0 }, { -16.0, 10.0 }, { -18.0, 20.0 }, { -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 }, { 18.0, 3.0 }, { 16.0, 10.0 }, { 18.0, 20.0 }, { 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

#define AIRPLANE_ROTATION_RADIUS 200.0f
float airplane_s_factor = 1.0f;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2) + sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}



GLfloat d_out[16][2] = { {} };

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}


//shirt
#define SHIRT_LEFT_BODY 0
#define SHIRT_RIGHT_BODY 1
#define SHIRT_LEFT_COLLAR 2
#define SHIRT_RIGHT_COLLAR 3
#define SHIRT_FRONT_POCKET 4
#define SHIRT_BUTTON1 5
#define SHIRT_BUTTON2 6
#define SHIRT_BUTTON3 7
#define SHIRT_BUTTON4 8
GLfloat left_body[6][2] = { { 0.0, -9.0 },{ -8.0, -9.0 },{ -11.0, 8.0 },{ -6.0, 10.0 },{ -3.0, 7.0 },{ 0.0, 9.0 } };
GLfloat right_body[6][2] = { { 0.0, -9.0 },{ 0.0, 9.0 },{ 3.0, 7.0 },{ 6.0, 10.0 },{ 11.0, 8.0 },{ 8.0, -9.0 } };
GLfloat left_collar[4][2] = { { 0.0, 9.0 },{ -3.0, 7.0 },{ -6.0, 10.0 },{ -4.0, 11.0 } };
GLfloat right_collar[4][2] = { { 0.0, 9.0 },{ 4.0, 11.0 },{ 6.0, 10.0 },{ 3.0, 7.0 } };
GLfloat front_pocket[6][2] = { { 5.0, 0.0 },{ 4.0, 1.0 },{ 4.0, 3.0 },{ 7.0, 3.0 },{ 7.0, 1.0 },{ 6.0, 0.0 } };
GLfloat button1[3][2] = { { -1.0, 6.0 },{ 1.0, 6.0 },{ 0.0, 5.0 } };
GLfloat button2[3][2] = { { -1.0, 3.0 },{ 1.0, 3.0 },{ 0.0, 2.0 } };
GLfloat button3[3][2] = { { -1.0, 0.0 },{ 1.0, 0.0 },{ 0.0, -1.0 } };
GLfloat button4[3][2] = { { -1.0, -3.0 },{ 1.0, -3.0 },{ 0.0, -4.0 } };

GLfloat shirt_color[9][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f },
	{ 206 / 255.0f, 173 / 255.0f, 184 / 255.0f }
};

GLuint VBO_shirt, VAO_shirt;
void prepare_shirt() {
	GLsizeiptr buffer_size = sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2) + sizeof(button3) + sizeof(button4);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(left_body), left_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body), sizeof(right_body), right_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body), sizeof(left_collar), left_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar), sizeof(right_collar), right_collar);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar),
		sizeof(front_pocket), front_pocket);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket), sizeof(button1), button1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1), sizeof(button2), button2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2), sizeof(button3), button3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(left_body) + sizeof(right_body) + sizeof(left_collar) + sizeof(right_collar)
		+ sizeof(front_pocket) + sizeof(button1) + sizeof(button2) + sizeof(button3), sizeof(button4), button4);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_shirt);
	glBindVertexArray(VAO_shirt);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_shirt() {
	glBindVertexArray(VAO_shirt);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_LEFT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_RIGHT_COLLAR]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_FRONT_POCKET]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 6);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON1]);
	glDrawArrays(GL_TRIANGLE_FAN, 26, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON2]);
	glDrawArrays(GL_TRIANGLE_FAN, 29, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON3]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, shirt_color[SHIRT_BUTTON4]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);
	glBindVertexArray(0);
}


//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel) + sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}

//draw cocktail
#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

GLfloat neck[6][2] = { { -6.0, -12.0 },{ -6.0, -11.0 },{ -1.0, 0.0 },{ 1.0, 0.0 },{ 6.0, -11.0 },{ 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 },{ -9.0, 4.0 },{ -12.0, 7.0 },{ 12.0, 7.0 },{ 9.0, 4.0 },{ 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 },{ -12.0, 10.0 },{ 12.0, 10.0 },{ 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 },{ 12.0, 12.0 },{ 14.0, 12.0 },{ 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 },{ 10.0, 14.0 },{ 10.0, 16.0 },{ 12.0, 18.0 },{ 14.0, 18.0 },{ 16.0, 16.0 },{ 16.0, 14.0 },{ 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw)
		+ sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

//draw car2
#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

GLfloat car2_body[8][2] = { { -18.0, -7.0 },{ -18.0, 0.0 },{ -13.0, 0.0 },{ -10.0, 8.0 },{ 10.0, 8.0 },{ 13.0, 0.0 },{ 18.0, 0.0 },{ 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 },{ -8.0, 6.0 },{ -2.0, 6.0 },{ -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 },{ 0.0, 6.0 },{ 8.0, 6.0 },{ 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 },{ -13.0, -8.0 },{ -13.0, -7.0 },{ -11.0, -4.0 },{ -7.0, -4.0 },{ -5.0, -7.0 },{ -5.0, -8.0 },{ -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 },{ 5.0, -8.0 },{ 5.0, -7.0 },{ 7.0, -4.0 },{ 11.0, -4.0 },{ 13.0, -7.0 },{ 13.0, -8.0 },{ 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 },{ -17.0, -2.0 },{ -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 },{ -17.0, -5.0 },{ -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1) + sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}

// cake
#define CAKE_FIRE 0
#define CAKE_CANDLE 1
#define CAKE_BODY 2
#define CAKE_BOTTOM 3
#define CAKE_DECORATE 4

GLfloat cake_fire[4][2] = { { -0.5, 14.0 },{ -0.5, 13.0 },{ 0.5, 13.0 },{ 0.5, 14.0 } };
GLfloat cake_candle[4][2] = { { -1.0, 8.0 } ,{ -1.0, 13.0 },{ 1.0, 13.0 },{ 1.0, 8.0 } };
GLfloat cake_body[4][2] = { { 8.0, 5.0 },{ -8.0, 5.0 } ,{ -8.0, 8.0 },{ 8.0, 8.0 } };
GLfloat cake_bottom[4][2] = { { -10.0, 1.0 },{ -10.0, 5.0 },{ 10.0, 5.0 },{ 10.0, 1.0 } };
GLfloat cake_decorate[4][2] = { { -10.0, 0.0 },{ -10.0, 1.0 },{ 10.0, 1.0 },{ 10.0, 0.0 } };

GLfloat cake_color[5][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f }
};

GLuint VBO_cake, VAO_cake;

void prepare_cake() {
	int size = sizeof(cake_fire);
	GLsizeiptr buffer_size = sizeof(cake_fire) * 5;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, cake_fire);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, cake_candle);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, cake_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, cake_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, cake_decorate);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cake);
	glBindVertexArray(VAO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cake() {
	glBindVertexArray(VAO_cake);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_FIRE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_CANDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_DECORATE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindVertexArray(0);
}


//simpson


#define SIMPSON_YELLOW 0

#define SIMPSON_PANTS 1
#define SIMPSON_SHOE 2
#define SIMPSON_WHITE 3
#define SIMPSON_BLACK 4

#define SIMPSON_MOUSE 5

GLfloat simpson_head[8][2] = { { 11.3,128.0 },{20.9, 120.5 },{ 20.9, 95.7 },{ 8.4, 80.3 },{ -11.3,128.0 },{-20.9, 120.5 },{ -20.9, 95.7 },{ -8.4, 80.3 } };
GLfloat simpson_body[18][2] = { { 8.4,80.3 } ,{ 17.7,78.4},{ 25.4,71.3 },{ 32.5,62.2 } ,
	{22.1,49.46 } ,{ 26.0,23.2 } ,{ 19.5,12.3},{ 5.0,12.3 } ,{ 0.0,12.3},
	{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},//x축 반전 할거라서 임시로 저장
	{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0}
};
GLfloat simpson_arm_right[7][2] = { { 32.5, 62.2 },{ 46.2, 47.1 } ,{ 61.4, 43.2 },{ 60.0, 28.7 },
	{ 47.0,27.5},{ 40.3,36.4 } ,{ 22.1,49.4 }
};
GLfloat simpson_arm_left[7][2] = { { -32.5, 62.2 },{ -46.2, 47.1 } ,{ -61.4, 43.2 },{ -60.0, 28.7 },
	{ -47.0,27.5},{ -40.3,36.4 } ,{ -22.1,49.4 }
};

GLfloat simpson_eye_right[7][2] = { {6.8,117.5},{ 11.7,114.8 },{12.7,110.0},{10.0,105.0 },{ 4.1,105.0 },{1.23,110.2 },{ 2.1,114.8 } };
GLfloat simpson_eye_left[7][2] = { {-6.8,117.5},{ -11.7,114.8 },{-12.7,110.0},{-10.0,105.0 },{- 4.1,105.0 },{-1.23,110.2 },{ -2.1,114.8 } };
GLfloat simpson_eye_black_right[1][2] = { {7.1,110.2} };//두번 그림
GLfloat simpson_eye_black_left[1][2] = { {-7.1,110.2} };//두번 그림
GLfloat simpson_ear_right[4][2] = { {20.9,106.8} ,{20.9,102.3},{24.2,104.0},{23.3,107.4} };//두번그림
GLfloat simpson_ear_left[4][2] = { {-20.9,106.8} ,{-20.9,102.3},{-24.2,104.0},{-23.3,107.4} };//두번그림
GLfloat simpson_mouse[6][2] = { {8.4,80.3},{20.9,90.7},{20.9,102.3},{-20.9,102.3},{-20.9,90.7},{-8.4,80.3} };
GLfloat simpson_teeth[6][2] = { {0.0,94.0},{7.5,95.0},{5.0,90.0},{0,88.8},{-5.0,90.0},{-7.5,95.0} };
GLfloat simpson_nose[4][2]= { {0.0,103.7},{-3.5,103.5},{-3.8,100.4},{0,99.3} };

GLfloat simpson_hair_right[3][2] = { {1.8,127.4},{-1.7,131.4},{-8.3,127.3}};//두번그림
GLfloat simpson_hair_left[3][2] = { {-1.8,127.4},{1.7,131.4},{8.3,127.3} };//두번그림

GLfloat simpson_pants_right[5][2] = { { 19.5,12.3 },{28.4, -23.8 },{ 10.5,-23.8 },{ 0.0, 2.6 },{0.0,12.3} };
GLfloat simpson_pants_left[5][2] = { { -19.5,12.3 },{-28.4, -23.8 },{ -10.5,-23.8 },{ 0.0, 2.6 },{0.0,12.3} };
GLfloat simpson_shoe_right[5][2] = { {10.8,-23.8},{ 28.4,-23.8 },{28.1,-31.1},{40.0,-40.0 },{ 12.0, -40.0 } };
GLfloat simpson_shoe_left[5][2] = { {-10.8,-23.8},{-28.4,-23.8 },{-28.1,-31.1},{-40.0,-40.0 },{-12.0, -40.0 } };



GLfloat simpson_color[6][3] = {
	{ 254 / 255.0f, 212 / 255.0f, 30 / 255.0f },//yellow
{ 102 / 255.0f, 174 / 255.0f, 224 / 255.0f },//pants
{ 104 / 255.0f, 109 / 255.0f, 103 / 255.0f },//shoes
{ 255 / 255.0f, 255/ 255.0f, 255 / 255.0f },//white
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },//black
{ 212 / 255.0f, 176 / 255.0f, 126 / 255.0f },//mouse
};

GLuint VBO_simpson, VAO_simpson;

void prepare_simpson() {

	for (int i = 0; i < 9; i++) {
		simpson_body[i + 9][0] = -simpson_body[i][0];
		simpson_body[i + 9][1] = simpson_body[i][1];
	}

	GLsizeiptr buffer_size = sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_black_right) + +sizeof(simpson_eye_left) + sizeof(simpson_eye_black_left) 
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left)
		+ sizeof(simpson_mouse) + sizeof(simpson_teeth) + sizeof(simpson_nose) + sizeof(simpson_hair_right) + sizeof(simpson_hair_left)
		+ sizeof(simpson_pants_right) + sizeof(simpson_pants_left) + sizeof(simpson_shoe_right) + sizeof(simpson_shoe_left);//총 19개
	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_simpson);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_simpson);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(simpson_head), simpson_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head), sizeof(simpson_body), simpson_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body), sizeof(simpson_arm_right), simpson_arm_right);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right), sizeof(simpson_arm_left), simpson_arm_left);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left),
		sizeof(simpson_eye_right), simpson_eye_right);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right), sizeof(simpson_eye_left), simpson_eye_left);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left), sizeof(simpson_eye_black_right), simpson_eye_black_right);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left)+ sizeof(simpson_eye_black_right), sizeof(simpson_eye_black_left), simpson_eye_black_left);

	
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right)+ sizeof(simpson_eye_black_left), 
		sizeof(simpson_ear_right), simpson_ear_right);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)+ sizeof(simpson_ear_right),
		sizeof(simpson_ear_left), simpson_ear_left);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left) 
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left),
		sizeof(simpson_mouse), simpson_mouse);

	glBufferSubData(GL_ARRAY_BUFFER, 
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left)+ sizeof(simpson_mouse),
		sizeof(simpson_teeth), simpson_teeth);

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse)+ sizeof(simpson_teeth),
		sizeof(simpson_nose), simpson_nose);

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth)+ sizeof(simpson_nose),
		sizeof(simpson_hair_right), simpson_hair_right);
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth) 
		+ sizeof(simpson_nose)+ sizeof(simpson_hair_right),
		sizeof(simpson_hair_left), simpson_hair_left);


	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth)
		+ sizeof(simpson_nose) + sizeof(simpson_hair_right)+ sizeof(simpson_hair_left),
		sizeof(simpson_pants_right), simpson_pants_right);

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth)
		+ sizeof(simpson_nose) + sizeof(simpson_hair_right) + sizeof(simpson_hair_left)+ sizeof(simpson_pants_right),
		sizeof(simpson_pants_left), simpson_pants_left);

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth)
		+ sizeof(simpson_nose) + sizeof(simpson_hair_right) + sizeof(simpson_hair_left) + sizeof(simpson_pants_right)+ sizeof(simpson_pants_left),
		sizeof(simpson_shoe_right), simpson_shoe_right);

	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(simpson_head) + sizeof(simpson_body) + sizeof(simpson_arm_right) + sizeof(simpson_arm_left)
		+ sizeof(simpson_eye_right) + sizeof(simpson_eye_left) + sizeof(simpson_eye_black_right) + sizeof(simpson_eye_black_left)
		+ sizeof(simpson_ear_right) + sizeof(simpson_ear_left) + sizeof(simpson_mouse) + sizeof(simpson_teeth)
		+ sizeof(simpson_nose) + sizeof(simpson_hair_right) + sizeof(simpson_hair_left) + sizeof(simpson_pants_right) 
		+ sizeof(simpson_pants_left)+ sizeof(simpson_shoe_right),
		sizeof(simpson_shoe_left), simpson_shoe_left);




	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_simpson);
	glBindVertexArray(VAO_simpson);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_simpson);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_simpson() {
	glBindVertexArray(VAO_simpson);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_YELLOW]);//head
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_WHITE]);//body
	glDrawArrays(GL_TRIANGLE_FAN, 8, 18);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_YELLOW]);//arm right
	glDrawArrays(GL_TRIANGLE_FAN, 26, 7);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_YELLOW]);//arm left
	glDrawArrays(GL_TRIANGLE_FAN, 33, 7);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_WHITE]);//eye right
	glDrawArrays(GL_TRIANGLE_FAN, 40, 7);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_WHITE]);//eye left
	glDrawArrays(GL_TRIANGLE_FAN, 47, 7);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_BLACK]);//eyeblack (point로 그림)
	glPointSize(7.0);
	glDrawArrays(GL_POINTS, 54, 1);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_BLACK]);//eyeblack (point로 그림)
	glDrawArrays(GL_POINTS, 55, 1);
	glPointSize(1.0);//원래대로



	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_YELLOW]);//ear right
	glDrawArrays(GL_TRIANGLE_FAN, 56, 4);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_YELLOW]);//ear left
	glDrawArrays(GL_TRIANGLE_FAN, 60, 4);
	


	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_MOUSE]);//mouse
	glDrawArrays(GL_TRIANGLE_FAN, 64, 6);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_WHITE]);//teeth
	glDrawArrays(GL_TRIANGLE_FAN, 70, 6);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_BLACK]);//nose
	glDrawArrays(GL_LINE_STRIP, 76, 4);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_BLACK]);//hair
	glDrawArrays(GL_LINE_STRIP, 80, 3);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_BLACK]);//hair
	glDrawArrays(GL_LINE_STRIP, 83, 3);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_PANTS]);//pants right
	glDrawArrays(GL_TRIANGLE_FAN, 86, 5);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_PANTS]);//pants left
	glDrawArrays(GL_TRIANGLE_FAN, 91, 5);

	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_SHOE]);//shoes right
	glDrawArrays(GL_TRIANGLE_FAN, 96, 5);
	glUniform3fv(loc_primitive_color, 1, simpson_color[SIMPSON_SHOE]);//shoes left
	glDrawArrays(GL_TRIANGLE_FAN, 101, 5);
	glBindVertexArray(0);
}
