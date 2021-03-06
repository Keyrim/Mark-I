/*
 * Controller.h
 *
 *  Created on: 24 nov. 2021
 *      Author: Théo Magne
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Controller.h"
#include "../Task_Manager/macro_types.h"
#include "../Motors/Motors.h"

typedef enum
{
	controller_state_eDISABLED,
	controller_state_eENABLE_P_GYRO,
	controller_state_eENABLE_P_MS,
}controller_state_e;

/*
 * @brief controller configuration structure definition
 */
typedef struct
{
	float angle_speed_kp[axe_eCOUNT];
	float angle_speed_ki[axe_eCOUNT];
	float angle_kp[axe_eCOUNT];
}controller_config_t;

typedef struct
{
	/* Configuration */
	controller_config_t config;
	/* Angular position regulation */
	float * angle;							/* 	[degree]		Angles */
	float target_angle[axe_eCOUNT];			/* 	[degree]		Angular target */
	float angle_error[axe_eCOUNT];			/* 	[degree]		Angular error */
	float angle_kp[axe_eCOUNT];
	/* Angular speed regulation */
	float * angle_speed;					/*	[degree / s]	Angular speed */
	float target_angle_speed[axe_eCOUNT];	/*	[degree / s]	Angular speed target */
	float angle_speed_error[axe_eCOUNT];	/*	[degree / s]	Angular speed error */
	float angle_speed_P[axe_eCOUNT];
	float angle_speed_I[axe_eCOUNT];
	float angle_speed_kp[axe_eCOUNT];
	float angle_speed_ki[axe_eCOUNT];
	/* Controller state */
	float global_thrust;
	float output_pid[axe_eCOUNT];
	float output_motor[motor_eCOUNT];
	controller_state_e state;
}controller_t;

void CONTROLLER_Init(void);
void CONTROLLER_Process_Gyro(void);
void CONTROLLER_Process_ms(void);
void CONTROLLER_Set_State(controller_state_e new_state);
void CONTROLLER_Set_Thrust(float thrust);
float * CONTROLLER_Get_Angle_Target(void);
float * CONTROLLER_Get_Angle_Speed_Target(void);
float * CONTROLLER_Get_Pid_Output(void);
float * CONTROLLER_Get_Pid_Output_P(void);
float * CONTROLLER_Get_Pid_Output_I(void);
float * CONTROLLER_Get_Pid_KP(void);
float * CONTROLLER_Get_Pid_KI(void);

#endif /* CONTROLLER_H_ */
