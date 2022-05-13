/*
 * caculate_angle.h
 *
 *  Created on: May 9, 2022
 *      Author: zhangrun
 */

#ifndef CACULATE_ANGLE_H_
#define CACULATE_ANGLE_H_

#include "stdint.h"
#include "tim.h"
#include "main.h"
#include "usart.h"

#define pi  3.1415926535
#define X_origin  10
#define Z_origin  0
#define ANGLE_origin 0
#define STEP_TIME 50
#define TARGET_SPEND_TIME 3000 //ms

#define receive_head_1 0xFF
#define receive_head_2 0xFE

#define x_offset 9
#define y_offset 15.5
#define x_scale  0.0878378378378378//(160 * 22 / 255 / 120)
#define y_scale  0.1324786324786325//(120 * 30 / 255 / 160)


uint8_t control(double x, double z, double angle);
void caculate_position(double x, double z, double angle);
uint8_t catch_reset();
void catch_release();
void catch();
void output(double *pwm1, double *pwm2, double *pwm3, double *pwm4, double *pwm5,  double *pwm6);
double abs_small(double x, double y, double target);
void wait_for_catch();
void catch_first_color_to_left();
void catch_second_color_to_right();

#endif /* CACULATE_ANGLE_H_ */
