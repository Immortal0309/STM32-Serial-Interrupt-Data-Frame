#ifndef _DATA_PROCESS_
#define _DATA_PROCESS_

#include "main.h"
#include "usart.h"
#include "Debug.h"

#define DEBUG_UART             huart1  /*<Debug串口句柄>*/

#define FRAME_HEAD             0xaa    /*<帧头>*/
#define FRAME_END              0x55    /*<帧尾>*/
#define FUNC_LED_OPEN_CODE     0x01    /*<功能帧>*/
#define FUNC_LED_CLOSE_CODE    0x02    /*<功能帧>*/
#define FUNC_LED_TOGGLE_CODE   0x03    /*<功能帧>*/
#define DEVICE_CODE            0x00    /*<设备帧>*/

/* <功能码函数指针> */
typedef void(*HandleFunc)();

/* <定义功能码与处理函数的映射关系> */
typedef struct
{
	uint8_t function_code;    /* <功能码> */
	HandleFunc func_process;  /* <功能码处理函数> */
}FrameProceHandle_t;

typedef struct
{
	uint8_t rx_buff[4];    /*<0:帧头-1:设备吗-2:功能码-3:帧尾。例如(0xaa 0x01 0x00 0x55)>*/
	uint8_t rx_flag : 1;   /*<接收完成标志：0为未完成，1为完成>*/
}DataFrameHandle_t;

extern DataFrameHandle_t data_frame_handle;

typedef enum
{
	HT_ERR        = 0,      /*<表示帧头帧尾错误>*/
	DEV_ERR       = 1,      /*<表示设备错误>*/
	FUNC_ERR      = 2,      /*<表示功能错误>*/
	DATA_SUCCESS  = 3       /*<表示成功>*/
}DataFrameError;

DataFrameError Data_Frame_Process(DataFrameHandle_t *data_frame_handle);

#endif
