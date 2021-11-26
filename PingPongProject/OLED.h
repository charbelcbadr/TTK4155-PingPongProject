/*
 * OLED.h
 *
 * Created: 27.09.2021 17:36:13
 *  Author: charbelb
 */ 


#ifndef OLED_H_
#define OLED_H_

typedef enum FONTSTYLE {SMALL,MEDIUM,LARGE} FONTSTYLE;
void write_command(uint8_t command);
void data_command(uint8_t command);
void init_oled();
void reset_oled();
void oled_clear_line(int line);
void oled_goto_line(int line);
void oled_goto_column(int column);
void oled_goto_pos(int line,int column);
void oled_print(char* character,FONTSTYLE font);
void OLED_timer(int value, int line, int column);
void print_digit(int digit, int line, int column);
char* convertIntegerToChar(int N);



#endif /* OLED_H_ */