#include "data_process.h"

/* <功能码处理函数声明> */
static void LED_on(void);
static void LED_off(void);

/* <功能码处理函数声明> */
DataFrameHandle_t data_frame_handle;

/* <功能码函数指针> */
typedef void(*HandleFunc)();

/* <定义功能码与处理函数的映射关系> */
typedef struct
{
	uint8_t function_code;    /* <功能码> */
	HandleFunc func_process;  /* <功能码处理函数> */
}FrameProceHandle_t;

/* <定义功能码与处理函数的映射关系数组> */
FrameProceHandle_t func_handle_map[] = {
	{0x01, LED_on},
	{0x02, LED_off},
};

/**
  * @brief   数据帧处理函数
  * @note    data_frame：接收到的数据帧
  * @param   无
  * @param   无
  * @param   无 
  * @retval  无 
  */
DataFrameError Data_Frame_Process(DataFrameHandle_t *data_frame)
{
	uint8_t i = 0;
	
	if(data_frame->rx_flag == 1){
		data_frame->rx_flag = 0;    //清除中断标志
		
		if(data_frame->rx_buff[0] == FRAME_HEAD && data_frame->rx_buff[3] == FRAME_END){  //判断帧头帧尾			
			if(data_frame->rx_buff[1] == DEVICE_CODE){  //判断设备码
				
				uint8_t func_code = data_frame->rx_buff[2];  //获取功能码			
				
				/* <查找处理函数> */
				for(i = 0; i<sizeof(func_handle_map)/sizeof(func_handle_map[0]); i++){
					if(func_handle_map[i].function_code == func_code){
						func_handle_map[i].func_process();  //调用处理函数
						break;
					}
				}
				if(i == sizeof(func_handle_map)/sizeof(func_handle_map[0])){
					Debug(DEBUG_UART, "function error\r\n");
					return FUNC_ERR;
				}
			}
			else{
				Debug(DEBUG_UART, "device error\r\n");
				return DEV_ERR;
			}
		}
		else{
			Debug(DEBUG_UART, "head tail error\r\n");
			return HT_ERR;
		}
	}
	return DATA_SUCCESS;
}

/**
  * @brief   串口中断回调函数
  * @note    huart：串口句柄
  * @param   无
  * @param   无
  * @param   无
  * @retval  无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		data_frame_handle.rx_flag = 1;
		HAL_UART_Receive_IT(&huart1, (uint8_t *)data_frame_handle.rx_buff, 4);
	}
}

/**
  * @brief  点亮LED功能码处理函数
  * @note   用户自己编写，用于实现不同功能码对应的功能
  * @param  无  
  * @param  无
  * @param  无
  * @retval 无
  */
static void LED_on(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  熄灭LED功能码处理函数
  * @note   用户自己编写，用于实现不同功能码对应的功能
  * @param  无  
  * @param  无
  * @param  无
  * @retval 无
  */
static void LED_off(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

