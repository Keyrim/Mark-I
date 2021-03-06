/*
 * Data_logger.h
 *
 *  Created on: Oct 24, 2021
 *      Author: Theo
 */

#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

#include "../Task_Manager/macro_types.h"

/*
 * @brief data id enumeration definition
 */
typedef enum
{
	/* Buttons */
	data_id_eCONFIG_REQUEST = 0,
	data_id_eSTART_TRANSFER,
	data_id_eSTOP_TRANSFER,
	/* High level controls */
	data_id_eHIGH_LEVEL_CONTROL1,
	data_id_eHIGH_LEVEL_CONTROL2,
	data_id_eHIGH_LEVEL_CONTROL3,
	data_id_eHIGH_LEVEL_CONTROL4,
	/* State vector */
	data_id_eSTATE_VELOCITY_ROLL,
	data_id_eSTATE_VELOCITY_PITCH,
	data_id_eSTATE_VELOCITY_YAW,
	/* Angles */
	data_id_eANGLE_ROLL,
	data_id_eANGLE_PITCH,
	data_id_eANGLE_YAW,
	/* Controller */
	data_id_eTARGET_ANGLE_SPEED_ROLL,
	data_id_eTARGET_ANGLE_SPEED_PITCH,
	data_id_eTARGET_ANGLE_SPEED_YAW,
	/* Radio */
	data_id_eRADIO1,
	data_id_eRADIO2,
	data_id_eRADIO3,
	data_id_eRADIO4,
	data_id_eRADIO5,
	data_id_eRADIO6,
	data_id_eRADIO7,
	data_id_eRADIO8,
	data_id_eRADIO9,
	data_id_eRADIO10,
	data_id_eRADIO11,
	data_id_eRADIO12,
	data_id_eRADIO13,
	data_id_eRADIO14,
	/* Controller */
	data_id_ePID_VEL_ROLL,
	data_id_ePID_VEL_PITCH,
	data_id_ePID_VEL_YAW,
	data_id_ePID_P_VEL_ROLL,
	data_id_ePID_P_VEL_PITCH,
	data_id_ePID_P_VEL_YAW,
	data_id_ePID_I_VEL_ROLL,
	data_id_ePID_I_VEL_PITCH,
	data_id_ePID_I_VEL_YAW,
	data_id_ePID_KP_VEL_ROLL,
	data_id_ePID_KP_VEL_PITCH,
	data_id_ePID_KP_VEL_YAW,
	data_id_ePID_KI_VEL_ROLL,
	data_id_ePID_KI_VEL_PITCH,
	data_id_ePID_KI_VEL_YAW,
	/* Motors */
	data_id_eMOTOR_FL,
	data_id_eMOTOR_FR,
	data_id_eMOTOR_BR,
	data_id_eMOTOR_BL,
	/* Gyro */
	data_id_eGYRO_ROLL,
	data_id_eGYRO_PITCH,
	data_id_eGYRO_YAW,
	data_id_eGYRO_RAW_ROLL,
	data_id_eGYRO_RAW_PITCH,
	data_id_eGYRO_RAW_YAW,
	/* Acc */
	data_id_eACC_ROLL,
	data_id_eACC_PITCH,
	data_id_eACC_YAW,
	data_id_eACC_RAW_ROLL,
	data_id_eACC_RAW_PITCH,
	data_id_eACC_RAW_YAW,
	data_id_eCOUNT
}data_id_e;


/*
 * @brief Data format enumeration definition
 * 		  Used to know how to encode the data before transmission
 * 		  Or how to decode a data on reception
 */
typedef enum
{
	/* 0 byte */
	data_format_e0B_BUTTON = 0,
	/* 1 byte */
	data_format_e8B = 20,
	data_format_e8B_PWM,
	data_format_e8B_FLOAT_1D,
	data_format_e8B_FLOAT_0D,
	/* 2 bytes */
	data_format_e16B = 40,
	data_format_e16B_FLOAT_1D,
	data_format_e16B_FLOAT_2D,
	data_format_e16B_FLOAT_3D,
	data_format_e16B_UINT16,
	data_format_e16B_INT16,
}data_format_e;

/*
 * @brief data format enumeration definition
 */
typedef enum
{
	use_format_eNOT_USED = 0,
	use_format_eAS_OUTPUT,
	use_format_eAS_INPUT
}use_format_e;


typedef struct
{
	uint8_t len;
	void * data;
	uint8_t id;
	data_format_e format;
	uint8_t * name ;
	uint8_t len_name;
	use_format_e use_format;
}data_t;


void DATA_LOGGER_Init(void);
void DATA_LOGGER_Main(void);
void DATA_LOGGER_Reception(uint8_t * input_buffer);
data_t * DATA_LOGGER_Get_Data(data_id_e id);
uint8_t DATA_LOGGER_Get_Data_Value(data_id_e id, uint8_t * buffer);
uint8_t DATA_LOGGER_Get_Data_Config(data_id_e id, uint8_t * buffer);

#endif /* DATA_LOGGER_DATA_LOGGER_H_ */
