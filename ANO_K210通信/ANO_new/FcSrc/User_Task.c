#include "User_Task.h"
#include "Drv_RcIn.h"
#include "LX_FC_Fun.h"
#include "Drv_Uart.h"
#include <string.h>
#include <math.h>
#include "ANO_LX.h"
#include "ANO_DT_LX.h"
#include "customized_uarts.h"
#include "Ano_Scheduler.h"

s16 yaw_1,position;

s16 xa,ya;

u16 i=0;

s16 a,b;

void control(s16 yawdps,s16 pitch,s16 roll,s16 height);
////////////////////////////////////////////////////////////////////////////////////////////////////////
///·���滮
/////////////////////////////////////////////





u16 target1, target2;           //��������Ŀ������1-12

float path1[10][2] = { 0 };                 //ǰ��Ŀ���1��������
float path2[10][2] = { 0 };                 //ǰ��Ŀ���2��������
float back_path[10][2] = { 0 };         //ǰ��ԭ��������꣬ע���Ǹ�����

u16 path1_targets_count = 0;        //ǰ��Ŀ���1��������
u16 path2_targets_count = 0;        //ǰ��Ŀ���2��������
u16 back_path_targets_count = 0;//ǰ��ԭ�㶨������

const float points[13][2] = {
	  0.0,  0.0,
	 50.0,275.0,
	200.0,125.0,
	275.0,200.0,
	350.0,-25.0,
	350.0,275.0,
	275.0, 50.0,
	125.0, 50.0,
	125.0,200.0,
	 50.0,125.0,
	200.0,-25.0,
	200.0,275.0,
	350.0,125.0
};



float distance(float x1, float y1, float x2, float y2)
{
	return (float)(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

void FindPath(const float (*str)[2], u16 target1, u16 target2, float path1[10][2], float path2[10][2], float back_path[10][2], u16* path1_targets_count, u16* path2_targets_count, u16* back_path_targets_count)
{
	u16 adjoin[20] = {0};
	u16 adjoin_count = 0;
	float max_adjoin_dis = 151;
	const u16 points_count = sizeof(points)/ sizeof(points[0]);
	u16 recent_point = 0;
	u16 closest_point;
	u16 i;

	float t1, t2;
	*path1_targets_count = 0;
	*path2_targets_count = 0;

	while (target1 != recent_point)
	{
		adjoin_count = 0;
		for (i = 0; i < points_count; i++)
		{			
			if (distance(points[i][0], points[i][1], points[recent_point][0], points[recent_point][1]) < max_adjoin_dis)
			{
				adjoin[adjoin_count] = i;
				adjoin_count++;
			}
		}		

		closest_point = adjoin[0];

		for (i = 0; i < adjoin_count; i++)
		{
			t1 = distance(points[adjoin[i]][0], points[adjoin[i]][1], points[target1][0], points[target1][1]);
			t2 = distance(points[closest_point][0], points[closest_point][1], points[target1][0], points[target1][1]);
			
			if ( t1< t2)
			{
				closest_point = adjoin[i];
			}
		}

		recent_point = closest_point;
		

		path1[*path1_targets_count][0] = points[closest_point][0];
		path1[*path1_targets_count][1] = points[closest_point][1];
		(*path1_targets_count)++;
	}

	while (target2 != recent_point)
	{
		adjoin_count = 0;
		for (i = 0; i < points_count; i++)
		{
			if (distance(points[i][0], points[i][1], points[recent_point][0], points[recent_point][1]) < max_adjoin_dis)
			{
				adjoin[adjoin_count] = i;
				adjoin_count++;
			}
		}

		closest_point = adjoin[0];

		for (i = 0; i < adjoin_count; i++)
		{
			t1 = distance(points[adjoin[i]][0], points[adjoin[i]][1], points[target2][0], points[target2][1]);
			t2 = distance(points[closest_point][0], points[closest_point][1], points[target2][0], points[target2][1]);

			if (t1 < t2)
			{
				closest_point = adjoin[i];
			}
		}

		recent_point = closest_point;


		path2[*path2_targets_count][0] = points[closest_point][0];
		path2[*path2_targets_count][1] = points[closest_point][1];
		(*path2_targets_count)++;
	}

	while (0 != recent_point)
	{
		adjoin_count = 0;
		for (i = 0; i < points_count; i++)
		{
			if (distance(points[i][0], points[i][1], points[recent_point][0], points[recent_point][1]) < max_adjoin_dis)
			{
				adjoin[adjoin_count] = i;
				adjoin_count++;
			}
		}

		closest_point = adjoin[0];

		for (i = 0; i < adjoin_count; i++)
		{
			t1 = distance(points[adjoin[i]][0], points[adjoin[i]][1], 0, 0);
			t2 = distance(points[closest_point][0], points[closest_point][1], 0, 0);

			if (t1 < t2)
			{
				closest_point = adjoin[i];
			}
		}

		recent_point = closest_point;

		back_path[*back_path_targets_count][0] = points[closest_point][0];
		back_path[*back_path_targets_count][1] = points[closest_point][1];
		(*back_path_targets_count)++;
	}
}



void missionstep(void)
{
//	u16 target1, target2;           //��������Ŀ������1-12

//float path1[10][2] = { 0 };                 //ǰ��Ŀ���1��������
//float path2[10][2] = { 0 };                 //ǰ��Ŀ���2��������
//float back_path[10][2] = { 0 };         //ǰ��ԭ��������꣬ע���Ǹ�����

//u16 path1_targets_count = 0;        //ǰ��Ŀ���1��������
//u16 path2_targets_count = 0;        //ǰ��Ŀ���2��������
//u16 back_path_targets_count = 0;//ǰ��ԭ�㶨������
//	mission_step += 1;
		s16 control_x_speed,control_y_speed;

		////���������
		static u8 i=0,j=0,k=0;

		target1 = 8;
		target2 = 6;

		u8 speed=15;//ƽ���ٶ�

		u8 length;
		s16 start_range;

		u8 range_changed = 0;

		u8 range_flag = 0;

		u8 stable_time;

		u8 recite=0;

		u8 no_shape=0;
		
		FindPath(points, target1, target2, path1, path2, back_path, &path1_targets_count, &path2_targets_count,&back_path_targets_count);
		
		if(i<=path1_targets_count-1)
		{
				switch(i)
				{
						case 0:
						{
					
								if(range_changed == 0)
								{
									start_range = sum_range;
									range_changed =1;
								}
					
				
								length = (int)sqrt(path1[0][0]*path1[0][0]+path1[0][1]*path1[0][1]);
					
								control_x_speed = path1[0][1]*speed/length;
								control_y_speed = -path1[0][0]*speed/length;
						
								if(control_x_speed>30)
									control_x_speed=30;
								if(control_x_speed<-30)
									control_x_speed=-30;
								if(control_y_speed>30)
									control_y_speed=30;
								if(control_y_speed<-30)
									control_y_speed=-30;
					
								if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
								{
										recite++;
										if(recite>20)
												no_shape = 1;
								}
								else
								{
										recite=0;
										no_shape=0;
								}
					
					
								if(range_flag==1&&no_shape==0)
										control(0,pitch_speed(),roll_speed(),0);
								else if(range_flag==1&&no_shape==1)
									  control(0,0,0,-10);
								else if(sum_range-start_range < length)													
									  control(0,control_x_speed,control_y_speed,0);							
								else if(sum_range-start_range >= length)
									  range_flag =1;
					
					
								if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
								{
										stable_time++; 
										if(stable_time>100)
										{
												stable_time=0;
												control(0,0,0,0);
												i += 1;
												range_flag = 0;
												range_changed = 0;							
										}
								}
								else
										stable_time=0;
						
						
								}
								break;
								case 1:
								{
						
										if(range_changed == 0)
										{
												start_range = sum_range;
												range_changed =1;
										}
						
					
										length = (int)sqrt((path1[i][0]-path1[i-1][0])*(path1[i][0]-path1[i-1][0])+(path1[i][1]-path1[i-1][1])*(path1[i][1]-path1[i-1][1]));
										
										control_x_speed = (path1[i][1]-path1[i-1][1])*speed/length;

										control_y_speed = -(path1[i][0]-path1[i-1][0])*speed/length;
							
										if(control_x_speed>30)
											control_x_speed=30;
										if(control_x_speed<-30)
											control_x_speed=-30;
										if(control_y_speed>30)
											control_y_speed=30;
										if(control_y_speed<-30)
											control_y_speed=-30;
						
						if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
						{
							recite++;
							if(recite>20)
								no_shape = 1;
						}
						else
						{
							recite=0;
							no_shape=0;
						}
						
						
						if(range_flag==1&&no_shape==0)
							
							control(0,pitch_speed(),roll_speed(),0);
						
						else if(range_flag==1&&no_shape==1)
							control(0,0,0,-10);
						else if(sum_range-start_range < length)
						{					
							control(0,control_x_speed,control_y_speed,0);
						}
						else if(sum_range-start_range >= length)
							range_flag =1;
						
						
						
						
						
						if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
						{
								stable_time++; 
								if(stable_time>100)
								{
									stable_time=0;
									control(0,0,0,0);
									i += 1;
									range_flag = 0;
									range_changed = 0;							
								}
						}
							else
								stable_time=0;
							
							
						}
						break;
						case 2:
						{
							
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path1[i][0]-path1[i-1][0])*(path1[i][0]-path1[i-1][0])+(path1[i][1]-path1[i-1][1])*(path1[i][1]-path1[i-1][1]));
							
							control_x_speed = (path1[i][1]-path1[i-1][1])*speed/length;

							control_y_speed = -(path1[i][0]-path1[i-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
						}
						break;
						case 3:
						{
								
								if(range_changed == 0)
								{
									start_range = sum_range;
									range_changed =1;
								}
								
							
								length = (int)sqrt((path1[i][0]-path1[i-1][0])*(path1[i][0]-path1[i-1][0])+(path1[i][1]-path1[i-1][1])*(path1[i][1]-path1[i-1][1]));
								
								control_x_speed = (path1[i][1]-path1[i-1][1])*speed/length;

								control_y_speed = -(path1[i][0]-path1[i-1][0])*speed/length;
									
								if(control_x_speed>30)
									control_x_speed=30;
								if(control_x_speed<-30)
									control_x_speed=-30;
								if(control_y_speed>30)
									control_y_speed=30;
								if(control_y_speed<-30)
									control_y_speed=-30;
								
								if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
								{
									recite++;
									if(recite>20)
										no_shape = 1;
								}
								else
								{
									recite=0;
									no_shape=0;
								}
								
								
								if(range_flag==1&&no_shape==0)
									
									control(0,pitch_speed(),roll_speed(),0);
								
								else if(range_flag==1&&no_shape==1)
									control(0,0,0,-10);
								else if(sum_range-start_range < length)
								{					
									control(0,control_x_speed,control_y_speed,0);
								}
								else if(sum_range-start_range >= length)
									range_flag =1;
								
								
								
								
								
								if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
								{
										stable_time++; 
										if(stable_time>100)
										{
											stable_time=0;
											control(0,0,0,0);
											i += 1;
											range_flag = 0;
											range_changed = 0;							
										}
								}
									else
										stable_time=0;
									
									
						}
						break;
						case 4:
						{
									
									if(range_changed == 0)
									{
										start_range = sum_range;
										range_changed =1;
									}
									
								
									length = (int)sqrt((path1[i][0]-path1[i-1][0])*(path1[i][0]-path1[i-1][0])+(path1[i][1]-path1[i-1][1])*(path1[i][1]-path1[i-1][1]));
									
									control_x_speed = (path1[i][1]-path1[i-1][1])*speed/length;

									control_y_speed = -(path1[i][0]-path1[i-1][0])*speed/length;
										
									if(control_x_speed>30)
										control_x_speed=30;
									if(control_x_speed<-30)
										control_x_speed=-30;
									if(control_y_speed>30)
										control_y_speed=30;
									if(control_y_speed<-30)
										control_y_speed=-30;
									
									if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
									{
										recite++;
										if(recite>20)
											no_shape = 1;
									}
									else
									{
										recite=0;
										no_shape=0;
									}
									
									
									if(range_flag==1&&no_shape==0)
										
										control(0,pitch_speed(),roll_speed(),0);
									
									else if(range_flag==1&&no_shape==1)
										control(0,0,0,-10);
									else if(sum_range-start_range < length)
									{					
										control(0,control_x_speed,control_y_speed,0);
									}
									else if(sum_range-start_range >= length)
										range_flag =1;
									
									
									
									
									
									if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
									{
											stable_time++; 
											if(stable_time>100)
											{
												stable_time=0;
												control(0,0,0,0);
												i += 1;
												range_flag = 0;
												range_changed = 0;							
											}
									}
										else
											stable_time=0;
										
										
							}
							break;
				}
			}
			
			
			
			else if(i==path1_targets_count)		
			{
				
				if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
				{
					recite++;
					if(recite>20)
						no_shape = 1;
				}
				else
				{
					recite=0;
					no_shape=0;
				}
				
				
				if(no_shape==0)
				{	
					control(0,pitch_speed(),roll_speed(),height_speed(80));//�½�
					//��������Ͷ��
				}
				else
					control(0,0,0,-10);
				
				if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0&&height_laser<86)
				{
						stable_time++; 
						if(stable_time>250)
						{
							stable_time=0;
							///�ն��
							
							///
							control(0,0,0,0);
							i += 1;
							range_flag = 0;
							range_changed = 0;							
						}
				}
				else
						stable_time=0;
			}
			else if(i==path1_targets_count+1)
			{
				if(j<=path2_targets_count-1)
				{
					switch(j)
					{
						case 0:
						{
							
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt ( (path2[0][0]-path1[path1_targets_count-1][0]) * (path2[0][0]-path1[path1_targets_count-1][0])+(path2[0][1]-path1[path1_targets_count-1][1])* (path2[0][1]-path1[path1_targets_count-1][1]) );
							
							control_x_speed = (path2[0][1]-path1[path1_targets_count-1][1])*speed/length;

							control_y_speed = -(path2[0][0]-path1[path1_targets_count-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
						
							case 1:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
							case 2:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
							case 3:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
						}
					}
			else if(j==path2_targets_count)		
			{
				
				if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
				{
					recite++;
					if(recite>20)
						no_shape = 1;
				}
				else
				{
					recite=0;
					no_shape=0;
				}
				
				
				if(no_shape==0)
				{	
					control(0,pitch_speed(),roll_speed(),height_speed(80));//�½�
					//��������Ͷ��
				}
				else
					control(0,0,0,-10);
				
				if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0&&height_laser<86)
				{
						stable_time++; 
						if(stable_time>250)
						{
							stable_time=0;
							///�ն��
							
							///
							control(0,0,0,0);
							i += 1;
							range_flag = 0;
							range_changed = 0;							
						}
				}
				else
						stable_time=0;
			}
			else if(j==path2_targets_count+1)
			{
				if(k<=back_path_targets_count-1)
				{
					switch(k)
					{
						case 0:
						{
							
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt ( (back_path[0][0]-path2[path2_targets_count-1][0]) * (back_path[0][0]-path2[path2_targets_count-1][0])+(back_path[0][1]-path2[path2_targets_count-1][1])* (back_path[0][1]-path2[path2_targets_count-1][1]) );
							
							control_x_speed = (back_path[0][1]-path2[path2_targets_count-1][1])*speed/length;

							control_y_speed = -(back_path[0][0]-path2[path2_targets_count-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
						
							case 1:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
							case 2:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
							case 3:
						{
						
							if(range_changed == 0)
							{
								start_range = sum_range;
								range_changed =1;
							}
							
						
							length = (int)sqrt((path2[j][0]-path2[j-1][0])*(path2[j][0]-path2[j-1][0])+(path2[j][1]-path2[j-1][1])*(path2[j][1]-path2[j-1][1]));
							
							control_x_speed = (path2[j][1]-path2[j-1][1])*speed/length;

							control_y_speed = -(path2[j][0]-path2[j-1][0])*speed/length;
								
							if(control_x_speed>30)
								control_x_speed=30;
							if(control_x_speed<-30)
								control_x_speed=-30;
							if(control_y_speed>30)
								control_y_speed=30;
							if(control_y_speed<-30)
								control_y_speed=-30;
							
							if(color_identify_data[0]==5000&&color_identify_data[1]==5000)
							{
								recite++;
								if(recite>20)
									no_shape = 1;
							}
							else
							{
								recite=0;
								no_shape=0;
							}
							
							
							if(range_flag==1&&no_shape==0)
								
								control(0,pitch_speed(),roll_speed(),0);
							
							else if(range_flag==1&&no_shape==1)
								control(0,0,0,-10);
							else if(sum_range-start_range < length)
							{					
								control(0,control_x_speed,control_y_speed,0);
							}
							else if(sum_range-start_range >= length)
								range_flag =1;
							
							
							
							
							
							if(xa<3&&xa>-3&&ya<3&&ya>-3&&no_shape==0)
							{
									stable_time++; 
									if(stable_time>100)
									{
										stable_time=0;
										control(0,0,0,0);
										i += 1;
										range_flag = 0;
										range_changed = 0;							
									}
							}
								else
									stable_time=0;
								
								
							}
							break;
						}
					}
			else if(k==back_path_targets_count)		
			{
				
				mission_step += 1;
			}
							
								
								
								
								
								
						}
					}
				}		



////---------------------------------------����---------------------------------------����
void control(s16 yawdps,s16 pitch,s16 roll,s16 height)
{
	rt_tar.st_data.yaw_dps = yawdps;  //����ת�����ٶȣ���ÿ�룬��ʱ��Ϊ��
	rt_tar.st_data.vel_x = pitch;    //ͷ���ٶȣ�����ÿ��
	rt_tar.st_data.vel_y = roll;    //�����ٶȣ�����ÿ��
	rt_tar.st_data.vel_z = height;	//�����ٶȣ�����ÿ��
  dt.fun[0x41].WTS = 1;	          //ʹ��
}


/////////////////////�߶�
s16 height_speed(u8 target_height)
{
//	static u8 flag=0;
	s16 z_speed=(target_height-height_laser)/2;
	if(z_speed>40)
		z_speed=40;
	if(z_speed<-15)
		z_speed=-15;
	return z_speed;
}

////////////////////�����ٶ�
s16 pitch_speed(void)
{
	static s32 SyEk=0;
	
	SyEk = SyEk+color_identify_data[1]-5000;
			
	if(SyEk>8000)
				SyEk=8000;
			
			if(SyEk<-8000)
				SyEk=-8000;
		
			ya = (color_identify_data[1]-5000)/200;
		
			if(ya>25)
				ya=25;
			if(ya<-25)
				ya=-25;
			return ya;
}

////////////////////////��ת�ٶ�
s16 roll_speed(void)
{
	static s32 SxEk=0;
	
	SxEk=SxEk+5000-color_identify_data[0];
	
	
			if(SxEk>8000)
				SxEk=8000;
		
			if(SxEk<-8000)
				SxEk=-8000;

			xa = (5000-color_identify_data[0])/200;
			if(xa>25)
				xa=25;
			if(xa<-25)
				xa=-25;
			return xa;
}


//void find_target_test(void)
//{
//	static s32 SxEk=0,SyEk=0;
////	s16 last_biasx,last_biasy;
//	if (rc_in.no_signal == 0)
//  {
//        //�жϵ�6ͨ������λ�� 1700<CH_6<2200
//		if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 1300 && rc_in.rc_ch.st_data.ch_[ch_6_aux2] < 1700)
//		{ 
//			
//			SxEk=SxEk+5000-color_identify_data[0];
//			SyEk=SyEk+color_identify_data[1]-5000;
//			if(SyEk>10000)
//				SyEk=10000;
//			if(SxEk>10000)
//				SxEk=10000;
//			if(SyEk<-10000)
//				SyEk=-10000;
//			if(SxEk<-10000)
//				SxEk=-10000;
//			//a=5000-color_identify_data[0]-last_biasx;
//			//b=color_identify_data[1]-5000-last_biasy;
//			xa = (5000-color_identify_data[0])/200+SxEk/4000;
//			ya = (color_identify_data[1]-5000)/200+SyEk/4000;
//			if(xa>25)
//				xa=25;
//			if(xa<-25)
//				xa=-25;
//			if(ya>25)
//				ya=25;
//			if(ya<-25)
//				ya=-25;
////			last_biasx=5000-color_identify_data[0];
////			last_biasy=color_identify_data[1]-5000;
//			control(0,ya,xa,0);
//			if(xa>-3&&xa<3&&ya>-3&&ya<3)
//				i++;
//			else
//				i=0;
//		}
//    else if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 1700 && rc_in.rc_ch.st_data.ch_[ch_6_aux2] < 2200)
//    {	
//			xa = (5000-color_identify_data[0])/200;
//			ya = (color_identify_data[1]-5000)/200;
//			if(xa>25)
//				xa=25;
//			if(xa<-25)
//				xa=-25;
//			if(ya>25)
//				ya=25;
//			if(ya<-25)
//				ya=-25;
//			control(0,ya,xa,-10);
//		}
//		else
//		{
//			SxEk=0,SyEk=0;
//			control(0,0,0,0);
//		}
//	}
//}


//void find_target(void)
//{	
//			static s32 SxEk=0,SyEk=0;
////			s16 last_biasx,last_biasy;
//			
//			SxEk=SxEk+5000-color_identify_data[0];
//			SyEk=SyEk+color_identify_data[1]-5000;
//			if(SyEk>8000)
//				SyEk=8000;
//			if(SxEk>8000)
//				SxEk=8000;
//			if(SyEk<-8000)
//				SyEk=-8000;
//			if(SxEk<-8000)
//				SxEk=-8000;
////			last_biasx=5000-color_identify_data[0];
////			last_biasy=color_identify_data[1]-5000;
//			xa = (5000-color_identify_data[0])/200+SxEk/4000;
//			ya = (color_identify_data[1]-5000)/200+SyEk/4000;
//			if(xa>25)
//				xa=25;
//			if(xa<-25)
//				xa=-25;
//			if(ya>25)
//				ya=25;
//			if(ya<-25)
//				ya=-25;
////			last_biasx=5000-color_identify_data[0];
////			last_biasy=color_identify_data[1]-5000;
//			control(0,ya,xa,0);
//			
//}

u8 mission_step;
u8 cnt_1=0;
u8 one_key_mission_f = 0;
void mission_start(void)
{


    
    //�ж���ң���źŲ�ִ��
  if (rc_in.no_signal == 0)
  {
        
		if(rc_in.rc_ch.st_data.ch_[ch_6_aux2]>1700 && rc_in.rc_ch.st_data.ch_[ch_6_aux2]<2200)
		{
			//��û��ִ��
			if(one_key_mission_f ==0)
			{
				//����Ѿ�ִ��
				one_key_mission_f = 1;
				//��ʼ����
				mission_step = 1;
			}
		}
		else
		{
			//��λ��ǣ��Ա��ٴ�ִ��
			one_key_mission_f = 0;	
				control(0,0,0,0); 
		}
		//
		if(one_key_mission_f==1)
		{
			static u16 time_dly_cnt_ms;
			//
			switch(mission_step)
			{
				case 0:
				{
					//reset
					time_dly_cnt_ms = 0;
				}
				break;
				case 1:
				{
					//����
					mission_step += FC_Unlock();
				}
				break;
				case 2:
				{
					//��2��
					if(time_dly_cnt_ms<2000)
					{
						time_dly_cnt_ms+=20;//ms
					}
					else
					{
						time_dly_cnt_ms = 0;
						mission_step += 1;
					}
				}
				break;
				case 3:
				{
					if(cnt_1<120)
					{
						control(0,0,0,height_speed(145));
						cnt_1++;
					}
					else
					{
						
					  control(0,pitch_speed(),roll_speed(),height_speed(145));
					}
					if(xa<4&&xa>-4&&ya<4&&ya>-4&&height_laser>138)
					{
						i++; 
						if(i>1000)
						{
							i=0;
							control(0,0,0,0);
							mission_step += 1;
							cnt_1=0;
						}
					}
					else
						i=0;
				}
				
				break;
				case 4:
				{
					missionstep(); 
				}
				break;
				case 5:
				{
					
					  control(0,pitch_speed(),roll_speed(),height_speed(0));
					
					
				}
				break;				
				default:break;
			}
		}
		else
		{
			mission_step = 0;
			
		}
	}
    ////////////////////////////////////////////////////////////////////////
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////ͨ��
////////////////////////////////////////////////////////

void add_one_number(char *send_buf,u8 num)  //���һ�����֣�0-255
{
	char three_bytes[4] = {0};
	three_bytes[0] = num/100+48;
	three_bytes[1] = num%100/10+48;
	three_bytes[2] = num%10+48;
	strcat(send_buf,three_bytes);
}

void add_string(char *send_buf,char *string)	//���һ���ַ���
{
	strcat(send_buf,string);
}

void send_attitude_angle(u8 *original_data) //����2������̬����
{
	char send_buf[50] = {0};		//���ݰ�����
	
	s16 rol,pit = 0;
	rol = ( (s16)( original_data[1] << 8 | original_data[0] ) )*314/180 + 15000 ;
	pit = ( (s16)( original_data[3] << 8 | original_data[2] ) )*314/180 + 15000 ; //������ȡ
		
	add_string(send_buf,"$");								//��ʼ����
	add_string(send_buf,"attitude_angle");	//���ݱ�ʶָ��
	add_one_number(send_buf,rol / 256);
	add_one_number(send_buf,rol % 256);
	add_one_number(send_buf,pit / 256);
	add_one_number(send_buf,pit % 256);			//���ݰ�����
	
	DrvUart2SendBuf(send_buf,27);					//�������ݰ�
}

/*-----------------------------�����Զ���ÿ������ָ�������-------------------------------------------------------------------------*/
void UART1_data_handle()
{
	u16 data1,data2,data3,data4;
	data1 = (U1_re_temp[ 1]*100+U1_re_temp[ 2]*10+U1_re_temp[ 3])*256+U1_re_temp[ 4]*100+U1_re_temp[ 5]*10+U1_re_temp[ 6];
	data2 = (U1_re_temp[ 7]*100+U1_re_temp[ 8]*10+U1_re_temp[ 9])*256+U1_re_temp[10]*100+U1_re_temp[11]*10+U1_re_temp[12];
	data3 = (U1_re_temp[13]*100+U1_re_temp[14]*10+U1_re_temp[15])*256+U1_re_temp[16]*100+U1_re_temp[17]*10+U1_re_temp[18];
	data4 = (U1_re_temp[19]*100+U1_re_temp[20]*10+U1_re_temp[21])*256+U1_re_temp[22]*100+U1_re_temp[23]*10+U1_re_temp[24];
	
	//���ݱ�ʶ,D�Ƹ�
	switch(U1_re_temp[0])
	{
		case 'D':
			if(U1_re_temp[25] == 2)
			{
				detour_pole_data[0] = data1;
				detour_pole_data[1] = data2;
			}
			break;
				
	}
}

void UART2_data_handle()	//���ݴ���
{
	u16 data1,data2,data3,data4;
	data1 = (U2_re_temp[ 1]*100+U2_re_temp[ 2]*10+U2_re_temp[ 3])*256+U2_re_temp[ 4]*100+U2_re_temp[ 5]*10+U2_re_temp[ 6];
	data2 = (U2_re_temp[ 7]*100+U2_re_temp[ 8]*10+U2_re_temp[ 9])*256+U2_re_temp[10]*100+U2_re_temp[11]*10+U2_re_temp[12];
	data3 = (U2_re_temp[13]*100+U2_re_temp[14]*10+U2_re_temp[15])*256+U2_re_temp[16]*100+U2_re_temp[17]*10+U2_re_temp[18];
	data4 = (U2_re_temp[19]*100+U2_re_temp[20]*10+U2_re_temp[21])*256+U2_re_temp[22]*100+U2_re_temp[23]*10+U2_re_temp[24];
	
	//���ݱ�ʶ,A����̬�ǣ�Cɫ�飬FѲ��
	switch(U2_re_temp[0])
	{
		case 'A':
			send_attitude_angle(fc_att.byte_data);
			break;
		
		case 'C':
			if(U2_re_temp[25] == 3)
			{
				color_identify_data[0] = data1;
				color_identify_data[1] = data2;
				color_identify_data[2] = data3;
			}
			break;
		
		case 'F':
			if(U2_re_temp[25] == 2)
			{
				follow_line_data[0] = data1;
				follow_line_data[1] = data2;
			}
			break;
				
	}
}

void UART3_data_handle()
{
	u16 data1,data2,data3,data4;
	data1 = (U3_re_temp[ 1]*100+U3_re_temp[ 2]*10+U3_re_temp[ 3])*256+U3_re_temp[ 4]*100+U3_re_temp[ 5]*10+U3_re_temp[ 6];
	data2 = (U3_re_temp[ 7]*100+U3_re_temp[ 8]*10+U3_re_temp[ 9])*256+U3_re_temp[10]*100+U3_re_temp[11]*10+U3_re_temp[12];
	data3 = (U3_re_temp[13]*100+U3_re_temp[14]*10+U3_re_temp[15])*256+U3_re_temp[16]*100+U3_re_temp[17]*10+U3_re_temp[18];
	data4 = (U3_re_temp[19]*100+U3_re_temp[20]*10+U3_re_temp[21])*256+U3_re_temp[22]*100+U3_re_temp[23]*10+U3_re_temp[24];
	
	/*K210���ݱ�ʶ
	Dʶ����ɫ����״����
	��ʶ���һ��λ����Ϊ�洢��Ŀ����״����ɫ
				�ڶ���λ����Ϊ�洢��Ŀ����״����ɫ
	��״��0��Ч��1Բ��2�����Σ�3������
	��ɫ��0��Ч��1�죬2��
	
	LΪK210�Ӿ�ѧϰ״̬������һλ
	0δ��ʼ��1����ѧϰ����2��ɣ�3��ʧ��
	
	Q��ά������*/
	switch(U3_re_temp[0])
	{
		case 'D':
			if(U3_re_temp[25] == 4)
			{
				K210_Identify_data[0] = data1;
				K210_Identify_data[1] = data2;
				K210_Identify_data[2] = data3;
				K210_Identify_data[3] = data4;
			}
			break;
		
		case 'L':
			if(U3_re_temp[25] == 1)
			{
				K210_learn_state = data1;
			}
			break;
			
		case 'Q':
			if(U3_re_temp[25] == 3)
			{
				QR_data[0] = data1;
				QR_data[1] = data2;
				QR_data[2] = data3;
			}
			break;
				
	}
}

/*--------------------------------------------------------------------------------------------------------------*/
	
