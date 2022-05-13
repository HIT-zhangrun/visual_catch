#include "caculate_angle.h"
#include "math.h"

double X = X_origin;
double Z = Z_origin;
double ANGLE = ANGLE_origin;
uint8_t buff[10];
uint8_t receive_flag = 0;

//平缓到达任意一点
uint8_t control(double x, double z, double angle)
{
	if(x > 30 || x < 0)
	{
		return 1;
	}
	if(z > 8 || z < 0)
	{
		return 1;
	}
	if(angle > pi || angle < 0)
	{
		return 1;
	}
	double x_target = x;
	double z_target = z;
	double angle_target = angle;
	double num = TARGET_SPEND_TIME / STEP_TIME;
	double x_error = (x_target - X) / num;
	double z_error = (z_target - Z) / num;
	double angle_error = (angle_target - ANGLE) / num;

	if((x_error == 0) && (z_error == 0) && (angle_error == 0))
	{
		return 0;
	}

	for(uint32_t i = 0; i < num; i++)
	{
		X += x_error;
		Z += z_error;
		ANGLE += angle_error;
		caculate_position(X, Z, ANGLE);
		HAL_Delay(STEP_TIME);
	}

	return 0;
}

//根据坐标计算舵机输出角度
void caculate_position(double x, double z, double angle)
{
	double z_init = 7.5 - x * 0.25;

	double A = z_init - z;
	double B = 10.5;
	double C = 15;
	double D = 17;
	//double pi = 3.1415926535;
	double theta;
	double beta;
	double alpha;
	double pwm_1;
	double pwm_2;
	double pwm_3;
	double pwm_4;

	if (x >= 0 && x < 15)
	{
		alpha = 0;
	}
	if (x >= 15)
	{
		alpha = (90 * (x - 15) / (40 - 15)) * (pi / 180);
	}
	double x_ = x - B * sin(alpha);
	double Y_ = 0 - A - B * cos(alpha);
	theta = acos((pow(x_, 2) + pow(Y_, 2) - pow(C, 2) - pow(D, 2)) / (2 * C * D));

	double M = D * sin(theta);
	double P = x_ * cos(alpha) - Y_ * sin(alpha);
	double Q = x_ * sin(alpha) + Y_ * cos(alpha);
	double a = pow(P, 2) + pow(Q, 2);
	double b = - 2 * M * P;
	double c = pow(M, 2) - pow(Q, 2);
	double delta = pow(b, 2) - 4 * a * c;
	double cos_beta_1 = (- b + sqrt(delta)) / (2 * a);
	double cos_beta_2 = (-b - sqrt(delta)) / (2 * a);

	double beta_1 = acos(cos_beta_1);
	double beta_2 = acos(cos_beta_2);

	//double x_caculate;
	double x_caculate_1 = B * sin(alpha) + C * sin(alpha + beta_1) + D * sin(alpha + beta_1 + theta);
	double x_caculate_2 = B * sin(alpha) + C * sin(alpha + beta_2) + D * sin(alpha + beta_2 + theta);

	if (abs_small(x_caculate_1, x_caculate_2, x) == x_caculate_1)
	{
		beta = beta_1;
		//x_caculate = x_caculate_1;
	}
	else
	{
		beta = beta_2;
		//x_caculate = x_caculate_2;
	}

	pwm_1 = 500 + angle / (pi / 2000);
	pwm_2 = 2500 - (pi / 2 - alpha) / (pi / 2000);
	pwm_3 = 2500 - (pi / 2 - beta) / (pi / 2000);
	pwm_4 = 2420 - theta / (pi / 2000);

	output(&pwm_1, &pwm_2, &pwm_3, &pwm_4, NULL, NULL);
}

uint8_t catch_reset()
{
	if((X == X_origin) && (Z == Z_origin) && (ANGLE == ANGLE_origin))
	{
		//第一次初始化
		HAL_UART_Receive_IT(&huart2, buff, 10);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		catch_release();
		caculate_position(10, 0, 0);
		return 0;
	}

	catch_release();
	control(X_origin, Z_origin, ANGLE_origin);


	return 0;
}

//释放
void catch_release()
{
	double pwm6 = 1200;
	output(NULL, NULL, NULL, NULL, NULL, &pwm6);
}

//抓取
void catch()
{
	double pwm6 = 700;
	output(NULL, NULL, NULL, NULL, NULL, &pwm6);
}

//底层pwm输出接口
void output(double *pwm1, double *pwm2, double *pwm3, double *pwm4, double *pwm5,  double *pwm6)
{
	if(pwm1 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, *pwm1);
	}
	if(pwm2 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, *pwm2);
	}
	if(pwm3 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, *pwm3);
	}
	if(pwm4 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, *pwm4);
	}
	if(pwm5 != NULL)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 1500);
	}
	if(pwm6 != NULL)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, *pwm6);
	}
}

double abs_small(double x, double y, double target)
{
	double x_ = x - target;
	double y_ = y - target;
	if (x_ < 0)
	{
		x_ = -x_;
	}
	if (y_ < 0)
	{
		y_ = -y_;
	}
	if (x_ <= y_)
	{
		return x;
	}

	return y;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//回调函数
{
	if(huart->Instance == USART2)
	{
		for(uint8_t num = 0; num < 7; num++)
		{
			if(buff[num] == receive_head_1 || buff[num] == receive_head_2)
			{
				buff[0] = buff[num];
				buff[1] = buff[num + 1];
				buff[2] = buff[num + 2];
				receive_flag = 1;
				return ;
			}
		}
		HAL_UART_Receive_IT(&huart2, buff, 10);
	}

}

void wait_for_catch()
{
	if(receive_flag)
	{
		receive_flag = 0;
		if(buff[0] == receive_head_1)
		{
			catch_first_color_to_left();
		}
		if(buff[0] == receive_head_2)
		{
			catch_second_color_to_right();
		}
		HAL_UART_Receive_IT(&huart2, buff, 10);
	}
}

void catch_first_color_to_left()
{
	double x_o = (buff[2] * x_scale + x_offset);
	double y_o = (buff[1] * y_scale - y_offset);
	//到达目标上空
	double x = sqrt(pow(x_o, 2) + pow(y_o, 2));
	double angle = atan(x_o / y_o);
	if(angle < 0)
	{
			angle = pi + angle;
	}
	X_TARGET = x;
	Z_TARGET = 5;
	ANGLE_TARGET = angle;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//打开爪子
	catch_release();
	//对准目标
	Z_TARGET = 0;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//夹取目标
	catch();
	//抬起目标
	Z_TARGET = 5;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//转移目标
	X_TARGET = 25;
	ANGLE_TARGET = 0;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//释放目标到指定区域
	catch_release();
	catch_reset();
	HAL_UART_Receive_IT(&huart2, buff, 10);
}

void catch_second_color_to_right()
{
	//到达目标上空
	double x_o = (buff[2] * x_scale + x_offset);
	double y_o = (buff[1] * y_scale - y_offset);
	//到达目标上空
	double x = sqrt(pow(x_o, 2) + pow(y_o, 2));
	double angle = atan(x_o / y_o);
	if(angle < 0)
	{
		angle = pi + angle;
	}
	X_TARGET = x;
	Z_TARGET = 5;
	ANGLE_TARGET = angle;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//打开爪子
	catch_release();
	//对准目标
	Z_TARGET = 0;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//夹取目标
	catch();
	//抬起目标
	Z_TARGET = 5;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//转移目标
	X_TARGET = 25;
	ANGLE_TARGET = pi;
	control(X_TARGET, Z_TARGET, ANGLE_TARGET);
	//释放目标到指定区域
	catch_release();
	catch_reset();
	HAL_UART_Receive_IT(&huart2, buff, 10);
}
