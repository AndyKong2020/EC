#include "SysConfig.h"

#ifndef _CUSTOMIZED_UARTS_H_
#define _CUSTOMIZED_UARTS_H_

extern s16 color_identify_data[3]; 	//openmvɫ��ʶ��xyz�����*10000

extern s16 follow_line_data[2]; 		//openmvѲ�߽Ƕ�*100���ؾ�*10000

extern s16 detour_pole_data[2]; 		//openmv�Ƹ������*10000

extern s16 K210_Identify_data[4];		//K210ʶ������
/*��һ��λ����Ϊ�洢��Ŀ����״����ɫ
	�ڶ���λ����ΪĿǰʶ�����״����ɫ
	��״��0��Ч��1Բ��2�����Σ�3������
	��ɫ��0��Ч��1�죬2��*/

extern s16 K210_learn_state;			//K210ѧϰ״̬
/*LΪK210�Ӿ�ѧϰ״̬������һλ
	0δ��ʼ��1����ѧϰ����2��ɣ�3��ʧ��*/
	
extern s16 QR_data[3]; 							//��ά������

void UART1_Receive_Prepare(u8 data);
void UART1_data_handle(void);				

void UART2_Receive_Prepare(u8 data);
void UART2_data_handle(void);					

void UART3_Receive_Prepare(u8 data);
void UART3_data_handle(void);					//�Զ��崮�����ݴ���

extern u8 U1_re_temp[26];							//�Զ���uart2���ջ���

extern u8 U2_re_temp[26];							//�Զ���uart2���ջ���

extern u8 U3_re_temp[26];							//�Զ���uart2���ջ���

#endif
