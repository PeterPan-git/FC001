#include "main.h"

void I2C1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=I2C1_SCL|I2C1_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	I2C1_SCL_H;
	I2C1_SDA_H;
}
void I2C1_SDA_OUT(void)
{
GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C1_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void I2C1_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C1_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void I2C1_Start(void)
{
	I2C1_SDA_OUT();
	
	I2C1_SDA_H;
	I2C1_SCL_H;
	US_Delay(5);
	I2C1_SDA_L;
	US_Delay(6);
	I2C1_SCL_L;
}
void I2C1_Stop(void)
{
	I2C1_SDA_OUT();

    I2C1_SCL_L;
    I2C1_SDA_L;
    I2C1_SCL_H;
    US_Delay(6);
    I2C1_SDA_H;
    US_Delay(6);
}
void I2C1_ACK(void)
{
	I2C1_SCL_L;
    I2C1_SDA_OUT();
    I2C1_SDA_L;
    US_Delay(2);
    I2C1_SCL_H;
    US_Delay(5);
    I2C1_SCL_L;
}
void I2C1_NACK(void)
{
	I2C1_SCL_L;
    I2C1_SDA_OUT();
    I2C1_SDA_H;
    US_Delay(2);
    I2C1_SCL_H;
    US_Delay(5);
    I2C1_SCL_L;
}
u8 I2C1_Wait_ACK(void)
{
	u8 tempTime=0;

	I2C1_SDA_IN();

	I2C1_SDA_H;
	US_Delay(1);
	I2C1_SCL_H;
	US_Delay(1);

	while(GPIO_ReadInputDataBit(GPIO_I2C1,I2C1_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C1_Stop();
			return 1;
		}	 
	}

	I2C1_SCL_L;
	return 0;
}
void I2C1_Write_Byte(u8 data)
{
	u8 i=0;

	I2C1_SDA_OUT();
	I2C1_SCL_L;//����ʱ�ӿ�ʼ���ݴ���

	for(i=0;i<8;i++)
	{
		if((data&0x80)>0) //0x80  1000 0000
			I2C1_SDA_H;
		else
			I2C1_SDA_L;

		data<<=1;
		I2C1_SCL_H;
		US_Delay(2); //��������
		I2C1_SCL_L;
		US_Delay(2);
	}
}
u8 I2C1_Read_Byte(u8 ack)
{
	u8 i=0,receive=0;

   I2C1_SDA_IN();
   for(i=0;i<8;i++)
   {
   		I2C1_SCL_L;
		US_Delay(2);
		I2C1_SCL_H;
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIO_I2C1,I2C1_SDA))
		   receive++;
		US_Delay(1);	
   }

   	if(ack==0)
	   	I2C1_NACK();
	else
		I2C1_ACK();

	return receive;
}

