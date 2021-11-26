/*
 * HighScore.c
 *
 * Created: 15.11.2021 13:58:00
 *  Author: harishnv
 */ 



#include <stdio.h>
#include <stdlib.h>
#include "OLED.h"
#include "HighScore.h"
#include "EEPROM_driver.h"

//Global variable for updating the score when the game has started
int present_score;
//Global array for reading the data from EEPROM
//int current_HighScore[MAX_HighScore];	

/* 
Description: Updates the value of the current user providing points during the game
Input: Nothing
Output: Nothing
*/
void update_present_score()
{
	present_score+= 1;
}
/* 
Description: Updates the HighScore in EEPROM after the game has ended
Input: Nothing
Output: Nothing
*/
void update_HighScore(){
	if(Highscore_data[2] < present_score)
	{
		Highscore_data[0]=Highscore_data[1];
		Highscore_data[1]=Highscore_data[2];
		Highscore_data[2]=present_score;		
	}
	else if(Highscore_data[1] < present_score)
	{
		Highscore_data[0]=Highscore_data[1];
		Highscore_data[1]=present_score;		
	}
	else if(Highscore_data[0] < present_score)
	{
		Highscore_data[0]=present_score;		
	}
	for (int i = 0; i < MAX_HighScore;i++)
	{
		EEPROM_write(i,Highscore_data[i]);
	}
}
/* 
Description: Reads the HighScore from EEPROM
Input: Nothing
Output: Nothing
*/
void read_HighScore()
{
	for (int i =MAX_HighScore; i>=0;i--)
	{
		current_HighScore[i] = EEPROM_read(i);
	}
}