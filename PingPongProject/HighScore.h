/*
 * HighScore.h
 *
 * Created: 15.11.2021 13:58:12
 *  Author: harishnv
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

//MAX HighScore to be stored and displayed from EEPROM
#define MAX_HighScore 3

//Intialising the array
int Highscore_data[MAX_HighScore] ;
int current_HighScore[MAX_HighScore];

void update_HighScore();
void read_HighScore();
void update_present_score();

#endif /* HIGHSCORE_H_ */