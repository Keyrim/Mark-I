/*
 * Orientation.c
 *
 *  Created on: 17 janv. 2022
 *      Author: Théo Magne
 */


#include "Orientation.h"
#include "../Sensors/Mpu.h"


static orientation_model_t orientation;
static orientation_kalman_t kalman;

/* Private functions prototype declaration */
static void compute_b_matrix(void);

/* --------------- Default Configuration --------------- */
static const orientation_config_t default_orientation_config =
{
		.prescaler = 1,
		.inertia_matrix =
		{
				15, 	0.4f, 	0.2f,
				0.4f,	20, 	0,
				0.2f, 	0, 		30
		},
		.motor_pos =
		{	/*					x			y	*/
			(motor_config_t){0.0778f,	 0.0897f},
			(motor_config_t){0.0778f, 	-0.0897f},
			(motor_config_t){-0.1549f, 	-0.0897f},
			(motor_config_t){-0.1549f,	 0.0897f}
		},
		.yaw_moment = 0.01f,
		.motor_to_newton = 1.0f,
		.f =
		{
				0.2f,
				0.2f,
				0.5f,
		}
};

/* --------------- State Space Model matrices --------------- */
/* F Matrix definition */
static arm_matrix_instance_f32 F = {0};
static float F_array[orien_state_vector_eCOUNT * orien_state_vector_eCOUNT] =
{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
};

/* B Matrix definition TODO compute the true value (constant at least) */
static arm_matrix_instance_f32 B = {0};
static float B_array[orien_state_vector_eCOUNT * orien_control_vector_eCOUNT] =
{
	0, 	0, 	0, 	0,
	0, 	0,	0,	0,
	0,	0,	0,	0,
};

/* Observation Matrix definition */
static arm_matrix_instance_f32 H = {0};
static float H_array[orien_meas_vector_eCOUNT * orien_state_vector_eCOUNT] =
{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
};

/* --------------- Kalman Model matrices --------------- */
/* Measurement covariance matrix */
static arm_matrix_instance_f32 R ;
static float R_array[orien_meas_vector_eCOUNT*orien_meas_vector_eCOUNT] =
{
		10, 0, 0,
		0, 10, 0,
		0, 0, 10
};
/* system model covariance matrix */
static arm_matrix_instance_f32 Q ;
static float Q_array[orien_state_vector_eCOUNT*orien_state_vector_eCOUNT] =
{
		100, 0, 0,
		0, 100, 0,
		0, 0, 100
};
/* P Matrix definition */
static arm_matrix_instance_f32 P_predict ;
static float P_array[orien_state_vector_eCOUNT*orien_state_vector_eCOUNT] =
{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
};

void ORIENTATION_Init(void)
{
	/* ------------------------------------ LOAD THE CONFIGURATION ------------------------------------ */
	orientation.config = default_orientation_config;
	float gyro_period = MPU_Get_Period();
	orientation.period = (float)orientation.config.prescaler * gyro_period;
	/* -------------- INIT THE STATE SPACE MODEL MATRIX ACCORDING TO THE CONFIGURATION ---------------- */
	arm_mat_init_f32(&F, orien_state_vector_eCOUNT, orien_state_vector_eCOUNT, F_array);
	arm_mat_init_f32(&B, orien_state_vector_eCOUNT, orien_control_vector_eCOUNT, B_array);
	arm_mat_init_f32(&H, orien_meas_vector_eCOUNT, orien_state_vector_eCOUNT, H_array);
	compute_b_matrix();
	STATE_SPACE_MODEL_Init((State_Space_Model_t*)&orientation, &F, &B, &H);
	/* --------------- INIT THE KALMAN FILTER MATRIX ACCORDING TO THE CONFIGURATION ------------------- */
	arm_mat_init_f32(&R, orien_meas_vector_eCOUNT, orien_meas_vector_eCOUNT, R_array);
	arm_mat_init_f32(&Q, orien_state_vector_eCOUNT, orien_state_vector_eCOUNT, Q_array);
	arm_mat_init_f32(&P_predict, orien_state_vector_eCOUNT, orien_state_vector_eCOUNT, P_array);
	KALMAN_Init((kalman_t*)&kalman, (State_Space_Model_t*)&orientation, &P_predict, &Q, &R);
	/* "Link" the gyroscope to the model by changing the measurement vector ptr */
	orientation.z.pData = MPU_Get_Gyro_Ptr();
}

void ORIENTATION_Update(void)
{
	KALMAN_Update((kalman_t*)&kalman);
	KALMAN_Predict((kalman_t*)&kalman);
}

static void compute_b_matrix(void)
{
	/* To compute the B matrix, we use and intermediate matrix called M to express
	 * the moments resulting from the motors on each axis */
	arm_matrix_instance_f32 M = { 0 };
	float m_array[orien_state_vector_eCOUNT * orien_control_vector_eCOUNT] = { 0 };
	arm_mat_init_f32(&M, orien_state_vector_eCOUNT, orien_control_vector_eCOUNT, m_array);
	for(uint8_t m = 0; m < orien_control_vector_eCOUNT; m++)
	{
		m_array[m] = orientation.config.motor_to_newton * orientation.config.motor_pos[m].y;
		m_array[m + 4] = -orientation.config.motor_to_newton * orientation.config.motor_pos[m].x;
	}
	m_array[8] = -orientation.config.yaw_moment;
	m_array[9] = orientation.config.yaw_moment;
	m_array[10] = -orientation.config.yaw_moment;
	m_array[11] = orientation.config.yaw_moment;
	/* Create the Inertia matrix and compute it's inverse */
	arm_matrix_instance_f32 inertia = { 0 };
	arm_mat_init_f32(&inertia, orien_state_vector_eCOUNT, orien_state_vector_eCOUNT, orientation.config.inertia_matrix);
	arm_matrix_instance_f32 inertia_inverse = { 0 };
	float inertia_inverse_array[orien_state_vector_eCOUNT * orien_state_vector_eCOUNT] = { 0 };
	arm_mat_init_f32(&inertia_inverse, orien_state_vector_eCOUNT, orien_state_vector_eCOUNT, inertia_inverse_array);
	arm_mat_inverse_f32(&inertia, &inertia_inverse);
	/* Then by multiplying the inverse of the moment of inertia by this matrix M, We obtain the B matrix */
	arm_mat_mult_f32(&inertia_inverse, &M, orientation.B);
}
