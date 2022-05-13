/*
 * Orientation.h
 *
 *  Created on: 8 janv. 2022
 *      Author: Théo Magne
 */

#ifndef SYSTEM_ORIENTATION_H_
#define SYSTEM_ORIENTATION_H_

#include "State_Space_Model.h"
#include "Kalman.h"
#include "../Task_Manager/macro_types.h"

/*
 * @brief orientation state vector enumeration definition
 */
typedef enum
{
	orien_state_vector_eVELOCITY_ROLL = 0,
	orien_state_vector_eVELOCITY_PITCH,
	orien_state_vector_eVELOCITY_YAW,
	orien_state_vector_eANGLE_ROLL,
	orien_state_vector_eANGLE_PITCH,
	orien_state_vector_eANGLE_YAW,
	orien_state_vector_eCOUNT
}orientation_state_vector_e;


/*
 * @brief orientation control vector enumeration definition
 */
typedef enum
{
	orien_control_vector_eMOTOR_FL = 0,
	orien_control_vector_eMOTOR_FR,
	orien_control_vector_eMOTOR_BR,
	orien_control_vector_eMOTOR_BL,
	orien_control_vector_eCOUNT
}orientation_control_vector_e;

/*
 * @brief orientation measurement vector enumeration definition
 */
typedef enum
{
	orien_meas_vector_eVELOCITY_ROLL = 0,
	orien_meas_vector_eVELOCITY_PITCH,
	orien_meas_vector_eVELOCITY_YAW,
	orien_meas_vector_eANGLE_ROLL,
	orien_meas_vector_eANGLE_PITCH,
	orien_meas_vector_eANGLE_YAW,
	orien_meas_vector_eCOUNT,
}orientation_measurement_vector_e;

/*
 * @brief motor configuration structure definition
 */
typedef struct
{
	float x;
	float y;
}motor_pos_t;

/*
 * @brief orientation mode enumeration definition
 */
typedef enum
{
	orien_mode_eREAL = 0,
	orien_mode_eSIMULATION
}orien_mode_e;

/*
 * @brief Orientation configuration structure definition
 */
typedef struct
{
	uint8_t prescaler;				/* 						Prescaler relatively to the gyroscope update frequency */
	motor_pos_t motor_pos[4];		/* [m] 					Motors positions */
	float inertia_matrix[9];		/* [kg.m^2] 			Moment of inertia matrix */
	float yaw_moment;				/* [kg.m^2.s^-2]		Constant to translate the yaw moment resulting from the motors */
	float motor_to_newton;			/* [kg.m.s^-2]			Coef to convert the signals sent to the motors into Newton */
	float f[axe_eCOUNT];			/* [kg.m.s^-3.rad^-1]	Moments resulting from the angular speed of the drone on each axis */
	orien_mode_e mode;
}orientation_config_t;

/*
 * @brief Orientation state space model structure inherited from state space model abstract structure
 */
typedef struct
{
	ABSTRACT_STATE_SPACE_MODEL_T(orien_state_vector_eCOUNT,
								 orien_control_vector_eCOUNT,
								 orien_meas_vector_eCOUNT);
	orientation_config_t config;
	orien_mode_e mode;
	float * gyro_data;		/* gyro data array */
	float * acc_data;		/* acc data array */
	float period;		/* update period in seconds */
}orientation_model_t;

/*
 * @brief Orientation kalman structure inherited from kalman abstract structure
 */
typedef struct
{
	ABSTRACT_KALMAN_MODEL_T(orien_state_vector_eCOUNT,
							orien_meas_vector_eCOUNT);
}orientation_kalman_t;

void ORIENTATION_Init(void);
void ORIENTATION_Process_Ms(void);
void ORIENTATION_Process_Gyro_Callback(void);
float * ORIENTATION_Get_State_Vector(void);
void ORIENTATION_Set_Mode(orien_mode_e new_mode);

#endif /* SYSTEM_ORIENTATION_H_ */
