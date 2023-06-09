# STM32-Serial-Interrupt-Data-Frame
该仓库仅上传STM32串口中断数据帧处理

# 使用教程
1. 数据帧
```
数据帧格式 （帧头:0xaa 设备码:0x00 功能码:0x01 帧尾:0x55）
该代码接收到：0xaa 0x00 0x01 0x55 点亮LED 
该代码接收到：0xaa 0x00 0x02 0x55 熄灭LED 
```
2. main.c
```
#include "Debug.h"
#include "data_process.h"

int main(void)
{	
  Debug(huart1, "begin\r\n");	
  HAL_UART_Receive_IT(&huart1, (uint8_t *)data_frame_handle.rx_buff, sizeof(data_frame_handle.rx_buff));
  
  while (1)
  {
    Data_Frame_Process(&data_frame_handle);
    HAL_Delay(1);
  }
}
```
