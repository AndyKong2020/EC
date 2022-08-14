#include "customized_uarts.h"
#include "SysConfig.h"

s16 color_identify_data[3] = {0}; 	//ɫ��ʶ��xyz�����*10000

s16 follow_line_data[2] = {0}; 			//Ѳ�߽Ƕ�*100���ؾ�*10000

s16 detour_pole_data[2] = {0}; 			//�Ƹ������*10000

s16 K210_Identify_data[4] = {0};					//K210ʶ������(��ͷ�ļ�)

s16 K210_learn_state = 0;						//K210ѧϰ״̬

s16 QR_data[3] = {0}; 							//��ά������


u8 U1_re_temp[26] = {0};		//�Զ���uart1���ջ���
				
u8 U2_re_temp[26] = {0};		//�Զ���uart2���ջ���

u8 U3_re_temp[26] = {0};		//�Զ���uart3���ջ���


void UART1_Receive_Prepare(u8 data) 	//����1���մ���
{
	static u8 _data_len = 0;
	static u8 rxstate = 0;


	if (rxstate == 0 && data == 'O')
	{
		rxstate = 1;
	}
	
	else if (rxstate == 1)		//�ж�֡ͷ�Ƿ���openmv��
	{
		if(data == 'P') rxstate = 2;
		else rxstate = 0;		
	}
//���ݱ�ʶ,D�Ƹ�	
	else if (rxstate == 2)		
	{
		if(data == 'D' )
		{
			rxstate = 3;
			U1_re_temp[0] = data;
			_data_len = 0;
		}
		else rxstate = 0;
	}
	//���յ�һ�����ݸ�λ��һλ
	else if (rxstate == 3)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 4;
				U1_re_temp[1] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
			
	//���յ�һ�����ݸ�λ�ڶ�λ
	else if (rxstate == 4)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 5;
				U2_re_temp[2] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ�һ�����ݸ�λ����λ
	else if (rxstate == 5)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 6;
				U1_re_temp[3] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ��һλ
	else if (rxstate == 6)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 7;
				U1_re_temp[4] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ�ڶ�λ
	else if (rxstate == 7)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 8;
				U1_re_temp[5] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յ�һ�����ݵ�λ����λ
	else if (rxstate == 8)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 9;
				U1_re_temp[6] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յڶ������ݸ�λ��һλ
	else if (rxstate == 9)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 10;
				U1_re_temp[7] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���ս��յڶ������ݸ�λ�ڶ�λ
	else if (rxstate == 10)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 11;
				U1_re_temp[8] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���ս��յڶ������ݸ�λ����λ
	else if (rxstate == 11)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 12;
				U1_re_temp[9] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ��һλ
	else if (rxstate == 12)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 13;
				U1_re_temp[10] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ�ڶ�λ
	else if (rxstate == 13)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 14;
				U1_re_temp[11] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յڶ������ݵ�λ����λ
	else if (rxstate == 14)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 15;
				U1_re_temp[12] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}

	//���յ��������ݸ�λ��һλ
	else if (rxstate == 15)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 16;
				U1_re_temp[13] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��������ݸ�λ�ڶ�λ
	else if (rxstate == 16)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 17;
				U1_re_temp[14] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յ��������ݸ�λ����λ
	else if (rxstate == 17)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 18;
				U1_re_temp[15] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��������ݵ�λ��һλ
	else if (rxstate == 18)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 19;
				U1_re_temp[16] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��������ݵ�λ�ڶ�λ
	else if (rxstate == 19)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 20;
				U1_re_temp[17] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յ��������ݵ�λ����λ
	else if (rxstate == 20)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 21;
				U1_re_temp[18] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ��һλ
	else if (rxstate == 21)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 22;
				U1_re_temp[19] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ�ڶ�λ
	else if (rxstate == 22)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 23;
				U1_re_temp[20] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յ��ĸ����ݸ�λ����λ
	else if (rxstate == 23)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 24;
				U1_re_temp[21] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ��һλ
	else if (rxstate == 24)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 25;
				U1_re_temp[22] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ�ڶ�λ
	else if (rxstate == 25)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 26;
				U1_re_temp[23] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}	
	//���յ��ĸ����ݵ�λ����λ
	else if (rxstate == 26)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 0;
				U1_re_temp[24] = data - 48;
				_data_len++;
				U1_re_temp[25] = _data_len;				
				UART1_data_handle(); 
			}
			else rxstate = 0;
		}
		else
		{	
			U1_re_temp[25] = _data_len;
			rxstate = 0;
			UART1_data_handle(); 
		}
	}
}

void UART2_Receive_Prepare(u8 data) 	//����2���մ���
{
	static u8 _data_len = 0;
	static u8 rxstate = 0;

	
	if (rxstate == 0 && data == 'O')
	{
		rxstate = 1;
	}
	
	else if (rxstate == 1)		//�ж�֡ͷ�Ƿ���openmv��
	{
		if(data == 'P') rxstate = 2;
		else rxstate = 0;		
	}
//���ݱ�ʶ,A����̬�ǣ�Cɫ�飬FѲ�ߣ�D�Ƹ�	
	else if (rxstate == 2)		
	{
		if(data == 'A' || data == 'C' || data == 'D' || data == 'F' )
		{
			rxstate = 3;
			U2_re_temp[0] = data;
			_data_len = 0;
		}
		else rxstate = 0;
	}
	//���յ�һ�����ݸ�λ��һλ
	else if (rxstate == 3)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 4;
				U2_re_temp[1] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
			
	//���յ�һ�����ݸ�λ�ڶ�λ
	else if (rxstate == 4)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 5;
				U2_re_temp[2] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ�һ�����ݸ�λ����λ
	else if (rxstate == 5)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 6;
				U2_re_temp[3] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ��һλ
	else if (rxstate == 6)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 7;
				U2_re_temp[4] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ�ڶ�λ
	else if (rxstate == 7)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 8;
				U2_re_temp[5] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յ�һ�����ݵ�λ����λ
	else if (rxstate == 8)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 9;
				U2_re_temp[6] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յڶ������ݸ�λ��һλ
	else if (rxstate == 9)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 10;
				U2_re_temp[7] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���ս��յڶ������ݸ�λ�ڶ�λ
	else if (rxstate == 10)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 11;
				U2_re_temp[8] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���ս��յڶ������ݸ�λ����λ
	else if (rxstate == 11)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 12;
				U2_re_temp[9] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ��һλ
	else if (rxstate == 12)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 13;
				U2_re_temp[10] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ�ڶ�λ
	else if (rxstate == 13)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 14;
				U2_re_temp[11] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յڶ������ݵ�λ����λ
	else if (rxstate == 14)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 15;
				U2_re_temp[12] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}

	//���յ��������ݸ�λ��һλ
	else if (rxstate == 15)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 16;
				U2_re_temp[13] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��������ݸ�λ�ڶ�λ
	else if (rxstate == 16)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 17;
				U2_re_temp[14] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յ��������ݸ�λ����λ
	else if (rxstate == 17)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 18;
				U2_re_temp[15] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��������ݵ�λ��һλ
	else if (rxstate == 18)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 19;
				U2_re_temp[16] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��������ݵ�λ�ڶ�λ
	else if (rxstate == 19)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 20;
				U2_re_temp[17] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յ��������ݵ�λ����λ
	else if (rxstate == 20)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 21;
				U2_re_temp[18] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ��һλ
	else if (rxstate == 21)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 22;
				U2_re_temp[19] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ�ڶ�λ
	else if (rxstate == 22)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 23;
				U2_re_temp[20] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յ��ĸ����ݸ�λ����λ
	else if (rxstate == 23)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 24;
				U2_re_temp[21] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ��һλ
	else if (rxstate == 24)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 25;
				U2_re_temp[22] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ�ڶ�λ
	else if (rxstate == 25)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 26;
				U2_re_temp[23] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}	
	//���յ��ĸ����ݵ�λ����λ
	else if (rxstate == 26)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 0;
				U2_re_temp[24] = data - 48;
				_data_len++;
				U2_re_temp[25] = _data_len;				
				UART2_data_handle(); 
			}
			else rxstate = 0;
		}
		else
		{	
			U2_re_temp[25] = _data_len;
			rxstate = 0;
			UART2_data_handle(); 
		}
	}
}

void UART3_Receive_Prepare(u8 data) 	//����3���մ���
{
	static u8 _data_len = 0;
	static u8 rxstate = 0;

	
	if (rxstate == 0 && data == 'K')
	{
		rxstate = 1;
	}
	
	else if (rxstate == 1)		//�ж�֡ͷ�Ƿ���K210��
	{
		if(data == 'R') rxstate = 2;
		else rxstate = 0;		
	}
/*���ݱ�ʶ
	Dʶ����ɫ����״����
	��ʶ���һ��λ����Ϊ�洢��Ŀ����״����ɫ
				�ڶ���λ����Ϊ�洢��Ŀ����״����ɫ
	��״��0��Ч��1Բ��2�����Σ�3������
	��ɫ��0��Ч��1�죬2��
	
	LΪK210�Ӿ�ѧϰ״̬������һλ
	0δ��ʼ��1����ѧϰ����2��ɣ�3��ʧ��
	
	Q��ά������*/
	else if (rxstate == 2)		
	{
		if(data == 'D' || data == 'L' || data == 'Q')
		{
			rxstate = 3;
			U3_re_temp[0] = data;
			_data_len = 0;
		}
		else rxstate = 0;
	}
	//���յ�һ�����ݸ�λ��һλ
	else if (rxstate == 3)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 4;
				U3_re_temp[1] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
			
	//���յ�һ�����ݸ�λ�ڶ�λ
	else if (rxstate == 4)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 5;
				U3_re_temp[2] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ�һ�����ݸ�λ����λ
	else if (rxstate == 5)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 6;
				U3_re_temp[3] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ��һλ
	else if (rxstate == 6)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 7;
				U3_re_temp[4] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ�һ�����ݵ�λ�ڶ�λ
	else if (rxstate == 7)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 8;
				U3_re_temp[5] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յ�һ�����ݵ�λ����λ
	else if (rxstate == 8)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 9;
				U3_re_temp[6] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յڶ������ݸ�λ��һλ
	else if (rxstate == 9)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 10;
				U3_re_temp[7] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���ս��յڶ������ݸ�λ�ڶ�λ
	else if (rxstate == 10)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 11;
				U3_re_temp[8] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���ս��յڶ������ݸ�λ����λ
	else if (rxstate == 11)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 12;
				U3_re_temp[9] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ��һλ
	else if (rxstate == 12)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 13;
				U3_re_temp[10] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յڶ������ݵ�λ�ڶ�λ
	else if (rxstate == 13)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 14;
				U3_re_temp[11] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յڶ������ݵ�λ����λ
	else if (rxstate == 14)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 15;
				U3_re_temp[12] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}

	//���յ��������ݸ�λ��һλ
	else if (rxstate == 15)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 16;
				U3_re_temp[13] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��������ݸ�λ�ڶ�λ
	else if (rxstate == 16)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 17;
				U3_re_temp[14] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յ��������ݸ�λ����λ
	else if (rxstate == 17)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 18;
				U3_re_temp[15] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��������ݵ�λ��һλ
	else if (rxstate == 18)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 19;
				U3_re_temp[16] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��������ݵ�λ�ڶ�λ
	else if (rxstate == 19)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 20;
				U3_re_temp[17] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յ��������ݵ�λ����λ
	else if (rxstate == 20)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 21;
				U3_re_temp[18] = data - 48;
				_data_len++;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ��һλ
	else if (rxstate == 21)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 22;
				U3_re_temp[19] = data - 48;
				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��ĸ����ݸ�λ�ڶ�λ
	else if (rxstate == 22)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 23;
				U3_re_temp[20] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յ��ĸ����ݸ�λ����λ
	else if (rxstate == 23)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 24;
				U3_re_temp[21] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ��һλ
	else if (rxstate == 24)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 25;
				U3_re_temp[22] = data - 48;				
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
	//���յ��ĸ����ݵ�λ�ڶ�λ
	else if (rxstate == 25)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 26;
				U3_re_temp[23] = data - 48;
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}	
	//���յ��ĸ����ݵ�λ����λ
	else if (rxstate == 26)
	{
		if(data != '$')
		{
			if(data > 47 && data < 58)
			{				
				rxstate = 0;
				U3_re_temp[24] = data - 48;
				_data_len++;
				U3_re_temp[25] = _data_len;				
				UART3_data_handle(); 
			}
			else rxstate = 0;
		}
		else
		{	
			U3_re_temp[25] = _data_len;
			rxstate = 0;
			UART3_data_handle(); 
		}
	}
}
