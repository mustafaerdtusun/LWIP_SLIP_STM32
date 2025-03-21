Working example of SLIP(Serial Line Internet Protocol) with LWIP Stack. 
UDP, TCP and HTTP WebServer.
Working on STM32H743ZI2 Board directly from ST-Link Connector.

Can work on Linux with commands below:
?

Change Notes:
STM32 CubeMX Code Generator can remove some codes.

LWIP/Target/lwipopts.h
Add these
/* USER CODE BEGIN 1 */
#define SLIPIF_THREAD_PRIO osPriorityNormal
#define DEFAULT_THREAD_PRIO osPriorityNormal
#define CHECKSUM_GEN_IP 1
#define CHECKSUM_GEN_UDP 1
#define CHECKSUM_GEN_TCP 1

#define HTTPD_USE_CUSTOM_FSDATA 0
/* USER CODE END 1 */


LWIP/APP/lwip.c
change
 /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &slipif_init, &tcpip_input);

add
/* USER CODE BEGIN 0 */
#include "netif/slipif.h"
#include "lwip/sio.h"
/* USER CODE END 0 */


Middlewares/Third_Party/LwIP/src/netif/slipf.c
add
netif_set_up(netif);
netif_set_link_up(netif);


core/inc
copy these files
tcpClientRAW.h
tcpServerRAW.h
udpClientRAW.h
udpServerRAW.h


core/src
copy these files
sio.c
tcpClientRAW.c
tcpServerRAW.c
udpClientRAW.c
udpServerRAW.c
