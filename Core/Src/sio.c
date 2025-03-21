#include "netif/slipif.h"
#include "lwip/sio.h"

extern UART_HandleTypeDef huart3;



/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

  sd = &huart3;

  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
    ret = HAL_UART_Transmit(&huart3, (uint8_t *)&c, 1, HAL_MAX_DELAY);

    if(ret == HAL_OK) {
		printf("sio_send\n");
		BSP_LED_Toggle(LED_RED);
    }
    else {
    	BSP_LED_Toggle(LED_YELLOW);
    }
}


#define BUFFER_LENGTH 500
uint8_t rxdata[1];
uint8_t rxBuffer[BUFFER_LENGTH]; // Circular buffer
int writeindex = 0;
int readindex = 0;
int getData = 0;

void startUART() {
	HAL_UART_Receive_IT(&huart3, rxdata, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	rxBuffer[writeindex] = rxdata[0];
	writeindex++;
	getData = 1;
	//BSP_LED_Toggle(LED_GREEN);
	if(writeindex >= BUFFER_LENGTH) {
		writeindex = 0;
	}


    if (huart->Instance == USART3)
    {
    	HAL_UART_Receive_IT(&huart3, rxdata, 1);
    }
}
/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */


  if(getData == 1)
  {
	memcpy(data, &rxBuffer[readindex], len);
    recved_bytes = len;

	readindex++;
	if(readindex >= BUFFER_LENGTH) {
		readindex = 0;
	}
	if(readindex == writeindex) {
		getData = 0;
	}
	BSP_LED_Toggle(LED_GREEN);
    //printf("recved_bytes = %d\n", recved_bytes);
    //printf("sio_read = %X\n", data[0]);
  }
  else
  {
    recved_bytes = 0;
  }

  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

  if(getData == 1)
  {
		memcpy(data, &rxBuffer[readindex], len);
	    recved_bytes = len;

		readindex++;
		if(readindex >= BUFFER_LENGTH) {
			readindex = 0;
		}
		if(readindex == writeindex) {
			getData = 0;
		}
    //printf("recved_bytes = %d\n", recved_bytes);
    //printf("sio_read = %X\n", data[0]);
  }
  else
  {
    recved_bytes = 0;
  }

  return recved_bytes;
}
