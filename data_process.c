#include "data_process.h"

/* <�����봦��������> */
static void LED_on(void);
static void LED_off(void);

/* <�����봦��������> */
DataFrameHandle_t data_frame_handle;

/* <�����뺯��ָ��> */
typedef void(*HandleFunc)();

/* <���幦�����봦������ӳ���ϵ> */
typedef struct
{
	uint8_t function_code;    /* <������> */
	HandleFunc func_process;  /* <�����봦����> */
}FrameProceHandle_t;

/* <���幦�����봦������ӳ���ϵ����> */
FrameProceHandle_t func_handle_map[] = {
	{0x01, LED_on},
	{0x02, LED_off},
};

/**
  * @brief   ����֡������
  * @note    data_frame�����յ�������֡
  * @param   ��
  * @param   ��
  * @param   �� 
  * @retval  �� 
  */
DataFrameError Data_Frame_Process(DataFrameHandle_t *data_frame)
{
	uint8_t i = 0;
	
	if(data_frame->rx_flag == 1){
		data_frame->rx_flag = 0;    //����жϱ�־
		
		if(data_frame->rx_buff[0] == FRAME_HEAD && data_frame->rx_buff[3] == FRAME_END){  //�ж�֡ͷ֡β			
			if(data_frame->rx_buff[1] == DEVICE_CODE){  //�ж��豸��
				
				uint8_t func_code = data_frame->rx_buff[2];  //��ȡ������			
				
				/* <���Ҵ�����> */
				for(i = 0; i<sizeof(func_handle_map)/sizeof(func_handle_map[0]); i++){
					if(func_handle_map[i].function_code == func_code){
						func_handle_map[i].func_process();  //���ô�����
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
  * @brief   �����жϻص�����
  * @note    huart�����ھ��
  * @param   ��
  * @param   ��
  * @param   ��
  * @retval  ��
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		data_frame_handle.rx_flag = 1;
		HAL_UART_Receive_IT(&huart1, (uint8_t *)data_frame_handle.rx_buff, 4);
	}
}

/**
  * @brief  ����LED�����봦����
  * @note   �û��Լ���д������ʵ�ֲ�ͬ�������Ӧ�Ĺ���
  * @param  ��  
  * @param  ��
  * @param  ��
  * @retval ��
  */
static void LED_on(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  Ϩ��LED�����봦����
  * @note   �û��Լ���д������ʵ�ֲ�ͬ�������Ӧ�Ĺ���
  * @param  ��  
  * @param  ��
  * @param  ��
  * @retval ��
  */
static void LED_off(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

