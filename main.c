//黑线贴3厘米宽，否则小车摆动很厉害
#include <reg52.h>//51头文件
#include <QX_A11.h>//QX_A11智能小车配置文件
#include <intrins.h>
unsigned char	pwm_val_left,pwm_val_right;	//中间变量，用户请勿修改。
unsigned char 	pwm_left,pwm_right;			//定义PWM输出高电平的时间的变量。用户操作PWM的变量。
#define		PWM_DUTY		200			//定义PWM的周期，数值为定时器0溢出周期，假如定时器溢出时间为100us，则PWM周期为20ms。
#define		PWM_HIGH_MIN	70			//限制PWM输出的最小占空比。用户请勿修改。
#define		PWM_HIGH_MAX	PWM_DUTY	//限制PWM输出的最大占空比。用户请勿修改。

void Timer0_Init(void); //定时器0初始化
void LoadPWM(void);//装入PWM输出值 
void forward(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车前进 
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车左转  
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed);//QX_A11智能小车右转
void stop(void);//QX_A11智能小车停车
int x,y;
void Delay400ms();

void Tracking()
{
    char d1 = left_led1, d2 = right_led2,d3 = right_led1;
	//为0 没有识别到黑线 为1识别到黑线
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

/*主函数*/     
void main(void)
{
	Timer0_Init();//定时0初始化
    x = 0,y = 0;
	while(1)
	{
		Tracking();
	}	
}


/*********************************************
QX_A11智能小车前进
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void forward(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	left_motor_go; //左电机前进
	right_motor_go; //右电机前进
}
/*小车左转*/
/*********************************************
QX_A11智能小车左转
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void left_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	left_motor_back; //左电机后退
	right_motor_go; //右电机前进	
}

/*********************************************
QX_A11智能小车右转
入口参数：LeftSpeed，RightSpeed
出口参数: 无
说明：LeftSpeed，RightSpeed分别设置左右车轮转速
**********************************************/
void right_run(unsigned char LeftSpeed,unsigned char RightSpeed)
{
	pwm_left = LeftSpeed,pwm_right =  RightSpeed;//设置速度
	right_motor_back;//右电机后退
	left_motor_go;    //左电机前进
}
/*********************************************
QX_A11智能小车停车
入口参数：无
出口参数: 无
说明：QX_A11智能小车停车
**********************************************/
void stop(void)
{
	left_motor_stops;
	right_motor_stops;
}
/*********************************************
QX_A11智能小车PWM输出
入口参数：无
出口参数: 无
说明：装载PWM输出,如果设置全局变量pwm_left,pwm_right分别配置左右输出高电平时间
**********************************************/
void LoadPWM(void)
{
	if(pwm_left > PWM_HIGH_MAX)		pwm_left = PWM_HIGH_MAX;	//如果左输出写入大于最大占空比数据，则强制为最大占空比。
	if(pwm_left < PWM_HIGH_MIN)		pwm_left = PWM_HIGH_MIN;	//如果左输出写入小于最小占空比数据，则强制为最小占空比。
	if(pwm_right > PWM_HIGH_MAX)	pwm_right = PWM_HIGH_MAX;	//如果右输出写入大于最大占空比数据，则强制为最大占空比。
	if(pwm_right < PWM_HIGH_MIN)	pwm_right = PWM_HIGH_MIN;	//如果右输出写入小于最小占空比数据，则强制为最小占空比。
	if(pwm_val_left<=pwm_left)		Left_moto_pwm = 1;  //装载左PWM输出高电平时间
	else Left_moto_pwm = 0; 						    //装载左PWM输出低电平时间
	if(pwm_val_left>=PWM_DUTY)		pwm_val_left = 0;	//如果左对比值大于等于最大占空比数据，则为零

	if(pwm_val_right<=pwm_right)	Right_moto_pwm = 1; //装载右PWM输出高电平时间
	else Right_moto_pwm = 0; 							//装载右PWM输出低电平时间
	if(pwm_val_right>=PWM_DUTY)		pwm_val_right = 0;	//如果右对比值大于等于最大占空比数据，则为零
}
/********************* Timer0初始化************************/
void Timer0_Init(void)
{
	TMOD |= 0x02;//定时器0，8位自动重装模块
	TH0 = 164;
	TL0 = 164;//11.0592M晶振，12T溢出时间约等于100微秒
	TR0 = 1;//启动定时器0
	ET0 = 1;//允许定时器0中断
	EA	= 1;//总中断允许	
}
 
/********************* Timer0中断函数************************/
void timer0_int (void) interrupt 1
{
	 pwm_val_left++;
	 pwm_val_right++;
	 LoadPWM();//装载PWM输出
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
