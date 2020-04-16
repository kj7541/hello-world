#ifndef _ANTO_H
#define _ANTO_H
#include "usart.h"
void ANO_Send(u8 *buf,u8 len);
void ANO_Send_Sensor(u16 ax,u16 ay,u16 az,u16 gx,u16 gy,u16 gz);
void ANO_DT_Send_Sensor(u16 a_x,u16 a_y,u16 a_z,u16 g_x,u16 g_y,u16 g_z);
void ANO_Send_Status(float rol,float pit,float yaw);




#endif









