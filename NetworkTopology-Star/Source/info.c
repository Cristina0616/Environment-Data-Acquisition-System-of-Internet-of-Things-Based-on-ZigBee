/*********************************************************************************************
* �ļ���info.c
* ���ߣ�liutong 2016.7.20
* ˵����ͨ�����ڿ���LCD��ʾ����س���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include <ioCC2530.h>            
#include <stdio.h>
#include "info.h"
#include "string.h"
#include "OnBoard.h"
#define HAL_INFOP_IEEE_OSET        0xC                          //mac��ַƫ����
#define halWait(x) halMcuWaitMs(x)

/*********************************************************************************************
* ���ƣ�lcd_dis
* ���ܣ�ͨ�����ڿ���LCD��ʾʵ�����ݺͽڵ��MAC
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void lcd_dis(void){
  for(unsigned char i = 0;i<2;i++){                              //����TYPE,��2��
    Uart_Send_String("{TYPE=06205}");                            //��״��ʵ��
    halWait(250);
    halWait(250);
  }
  
  halWait(250);
  halWait(250);
  char CC2530_MAC[30] = {0};                                     //���MAC
  char devmacaddr[8];
  unsigned char *macaddrptr = (unsigned char *)(P_INFOPAGE+HAL_INFOP_IEEE_OSET);
  for(int i=0;i<8;i++) {
    devmacaddr[i] = macaddrptr[i];                              //��ȡmac��ַ
  }
  strcat(CC2530_MAC,"{MAC=");
  sprintf(&CC2530_MAC[5],"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                          devmacaddr[7],devmacaddr[6],devmacaddr[5],
                          devmacaddr[4],devmacaddr[3],devmacaddr[2],
                          devmacaddr[1],devmacaddr[0]);
  CC2530_MAC[28]='}';
   for(unsigned char i = 0;i<2;i++){                            //����MAC����2��
   Uart_Send_String(CC2530_MAC);  
   halWait(250);
   halWait(250);
  }
}
/*********************************************************************************************
* ���ƣ�halMcuWaitMs
* ���ܣ�������ʱ����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#pragma optimize=none
void halMcuWaitMs(uint16 msec){
  while(msec--)
    halMcuWaitUs(1000);

}
/*********************************************************************************************
* ���ƣ�halMcuWaitUs
* ���ܣ�΢����ʱ����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#pragma optimize=none
void halMcuWaitUs(uint16 usec)
{
    usec>>= 1;
    while(usec--)
    {
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
    }
}
/*uart0��ʼ��
-------------------------------------------------------*/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                 //��ʼ��UART0�˿�
  PERCFG&= ~0x01;                 //ѡ��UART0Ϊ��ѡλ��һ
  P2DIR &= ~0xC0;                 //P0������Ϊ����0
  U0CSR = 0xC0;                   //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U0GCR = 0x0A;                  
  U0BAUD = 0x3B;                  //����������Ϊ38400
  
  U0UCR |= StopBits|Parity;       //����ֹͣλ����żУ��
}
/*���ڷ����ֽں���
-------------------------------------------------------*/
void Uart_Send_char(char ch)
{
  U0DBUF = ch;
  while(UTX0IF == 0);
  UTX0IF = 0;
}

/*���ڷ����ַ�������
-------------------------------------------------------*/
void Uart_Send_String(char *Data)
{  
  while (*Data != '\0')
  {
    Uart_Send_char(*Data++);
  }
}