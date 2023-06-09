#ifndef _DATA_PROCESS_
#define _DATA_PROCESS_

#include "main.h"
#include "usart.h"
#include "Debug.h"

#define DEBUG_UART             huart1  /*<Debug���ھ��>*/

#define FRAME_HEAD             0xaa    /*<֡ͷ>*/
#define FRAME_END              0x55    /*<֡β>*/
#define FUNC_LED_OPEN_CODE     0x01    /*<����֡>*/
#define FUNC_LED_CLOSE_CODE    0x02    /*<����֡>*/
#define FUNC_LED_TOGGLE_CODE   0x03    /*<����֡>*/
#define DEVICE_CODE            0x00    /*<�豸֡>*/

typedef struct
{
	uint8_t rx_buff[4];    /*<0:֡ͷ-1:�豸��-2:������-3:֡β������(0xaa 0x01 0x00 0x55)>*/
	uint8_t rx_flag : 1;   /*<������ɱ�־��0Ϊδ��ɣ�1Ϊ���>*/
}DataFrameHandle_t;

extern DataFrameHandle_t data_frame_handle;

typedef enum
{
	HT_ERR        = 0,      /*<��ʾ֡ͷ֡β����>*/
	DEV_ERR       = 1,      /*<��ʾ�豸����>*/
	FUNC_ERR      = 2,      /*<��ʾ���ܴ���>*/
	DATA_SUCCESS  = 3       /*<��ʾ�ɹ�>*/
}DataFrameError;

DataFrameError Data_Frame_Process(DataFrameHandle_t *data_frame_handle);

#endif
