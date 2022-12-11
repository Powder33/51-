/*********************************************************************************
* ����дʱ�䡿�� 2020��12��12��
* ����    �ߡ��� �������:03
* ����    ������ 1.0
* ����    վ���� http://www.QXMBOT.com/ 
* ���Ա����̡��� http://qxmcu.taobao.com/ (ֱ����)
* ��ʵ��ƽ̨���� QX-A11�ǿ�������С��  STC89C52
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio4
*	��Ȩ���У�����ؾ���
*	Copyright(C) QXMBOT
*	All rights reserved
***********************************************************************************
* ������˵��������ο�������ͼ��˵����
* �������ܡ���QX-A11�ǿ�������С������ѭ��		   			            			    
* ��ע�����������С��ײ���ϰ����С�����Ӷ�ת
				1����������Ȼ��ǿ�ҵ������½�������
				2����������ǰ������ں�ѭ��̽ͷʹ��������ѭ������
				3���˳���ֻ���ο���ʵ������Ч������ݲ�ͬʵ�鳡�ؽ��в�ͬ����
**********************************************************************************/
//������3���׿�����С���ڶ�������
#include <reg52.h>//51ͷ�ļ�
#include <QX_A11.h>//QX_A11����С�������ļ�
#include <intrins.h>
unsigned char	pwm_val_left,pwm_val_right;	//�м�������û������޸ġ�
unsigned char 	pwm_left,pwm_right;			//����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����
#define		PWM_DUTY		200			//����PWM�����ڣ���ֵΪ��ʱ��0������ڣ����綨ʱ�����ʱ��Ϊ100us����PWM����Ϊ20ms��
#define		PWM_HIGH_MIN	70			//����PWM�������Сռ�ձȡ��û������޸ġ�
#define		PWM_HIGH_MAX	PWM_DUTY	//����PWM��������ռ�ձȡ��û������޸ġ�

void Timer0_Init(void); //��ʱ��0��ʼ��
void LoadPWM(void);//װ��PWM���ֵ 
void forward(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С��ǰ�� 
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С����ת  
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11����С����ת
void stop(void);//QX_A11����С��ͣ��
int x,y;
void Delay400ms();

void Tracking()
{
    char d1 = left_led1, d2 = right_led2,d3 = right_led1;
	//Ϊ0 û��ʶ�𵽺��� Ϊ1ʶ�𵽺���
	if (d1 == 0 && d3 == 0) {
        forward(170, 130);
    }
    if (d1 == 1 && d3 == 0) {
        if (d1 == 1 && d2 == 0) {
            left_run(80, 160);
        }
        if (d2 == 1 && d1 == 0) {
            forward(170, 130);
        }
        if (d1 == 1 && d2 == 0 && d3 == 1) {
            right_run(160, 80);
            Delay400ms();
        }
        if (d2 == 1 && d1 == 1) {
            if (x <= 1) {
                forward(170, 130);
                Delay400ms();
                x++;
            }
            else {
                left_run(80, 160);
                Delay400ms();
            }
        }
    }
    if (d1 == 0 && d3 == 1) {
        if (d2 == 0 && d3 == 1) {
            right_run(160, 80);
        }
        if (d2 == 1 && d3 == 0) {
            forward(170, 130);
    }
        if (d2 == 1 && d3 == 1) {
            if (y <= 1) {
                forward(170, 130);
            }
            else {
                right_run(160, 80);
            }
        }
    }
    if (d1 == 1 && d2 == 1 && d3 == 1) {
        stop();
    }
}

/*������*/     
void main(void)
{
	Timer0_Init();//��ʱ0��ʼ��
    x = 0,y = 0;
	while(1)
	{
		Tracking();
	}	
}


/*********************************************
QX_A11����С��ǰ��
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void forward(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	left_motor_go; //����ǰ��
	right_motor_go; //�ҵ��ǰ��
}
/*С����ת*/
/*********************************************
QX_A11����С����ת
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	left_motor_back; //��������
	right_motor_go; //�ҵ��ǰ��	
}

/*********************************************
QX_A11����С����ת
��ڲ�����LeftSpeed��RightSpeed
���ڲ���: ��
˵����LeftSpeed��RightSpeed�ֱ��������ҳ���ת��
**********************************************/
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//�����ٶ�
	right_motor_back;//�ҵ������
	left_motor_go;    //����ǰ��
}
/*********************************************
QX_A11����С��ͣ��
��ڲ�������
���ڲ���: ��
˵����QX_A11����С��ͣ��
**********************************************/
void stop(void)
{
	left_motor_stops;
	right_motor_stops;
}
/*********************************************
QX_A11����С��PWM���
��ڲ�������
���ڲ���: ��
˵����װ��PWM���,�������ȫ�ֱ���pwm_left,pwm_right�ֱ�������������ߵ�ƽʱ��
**********************************************/
void LoadPWM(void)
{
	if(pwm_left > PWM_HIGH_MAX)		pwm_left = PWM_HIGH_MAX;	//��������д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
	if(pwm_left < PWM_HIGH_MIN)		pwm_left = PWM_HIGH_MIN;	//��������д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
	if(pwm_right > PWM_HIGH_MAX)	pwm_right = PWM_HIGH_MAX;	//��������д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
	if(pwm_right < PWM_HIGH_MIN)	pwm_right = PWM_HIGH_MIN;	//��������д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
	if(pwm_val_left<=pwm_left)		Left_moto_pwm = 1;  //װ����PWM����ߵ�ƽʱ��
	else Left_moto_pwm = 0; 						    //װ����PWM����͵�ƽʱ��
	if(pwm_val_left>=PWM_DUTY)		pwm_val_left = 0;	//�����Ա�ֵ���ڵ������ռ�ձ����ݣ���Ϊ��

	if(pwm_val_right<=pwm_right)	Right_moto_pwm = 1; //װ����PWM����ߵ�ƽʱ��
	else Right_moto_pwm = 0; 							//װ����PWM����͵�ƽʱ��
	if(pwm_val_right>=PWM_DUTY)		pwm_val_right = 0;	//����ҶԱ�ֵ���ڵ������ռ�ձ����ݣ���Ϊ��
}
/********************* Timer0��ʼ��************************/
void Timer0_Init(void)
{
	TMOD |= 0x02;//��ʱ��0��8λ�Զ���װģ��
	TH0 = 164;
	TL0 = 164;//11.0592M����12T���ʱ��Լ����100΢��
	TR0 = 1;//������ʱ��0
	ET0 = 1;//����ʱ��0�ж�
	EA	= 1;//���ж�����	
}
 
/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt 1
{
	 pwm_val_left++;
	 pwm_val_right++;
	 LoadPWM();//װ��PWM���
}	

void Delay400ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 3;
	j = 206;
	k = 43;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}