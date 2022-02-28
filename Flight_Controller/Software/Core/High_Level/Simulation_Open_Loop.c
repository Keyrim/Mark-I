/*
 * Simulation_Open_Loop.c
 *
 *  Created on: 28 févr. 2022
 *      Author: Théo Magne
 */
#define	HL_ROLL 	high_level->control[0]
#define HL_PITCH 	high_level->control[1]
#define HL_YAW 		high_level->control[2]
#define HL_POWER 	high_level->control[3]

#include "High_Level.h"
#include "../Motors/Motors.h"

void SIMULATION_OPEN_LOOP_Main(high_level_t * high_level)
{
	float output_motor[4];
	output_motor[orien_control_vector_eMOTOR_FL] = 	 HL_POWER +  HL_ROLL -  HL_PITCH -  HL_YAW;
	output_motor[orien_control_vector_eMOTOR_FR] = 	 HL_POWER -  HL_ROLL -  HL_PITCH +  HL_YAW;
	output_motor[orien_control_vector_eMOTOR_BR] = 	 HL_POWER -  HL_ROLL +  HL_PITCH -  HL_YAW;
	output_motor[orien_control_vector_eMOTOR_BL] = 	 HL_POWER +  HL_ROLL +  HL_PITCH +  HL_YAW;
	MOTOR_Set(output_motor);
}

