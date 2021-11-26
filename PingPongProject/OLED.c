/*
 * OLED.c
 *
 * Created: 27.09.2021 17:35:33
 *  Author: charbelb
 */ 
#define F_CPU 4.9152e6
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1
#define OLED_COMMAND_BASE 0x1000
#define OLED_DATA_BASE 0x1200

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "OLED.h"
#include "fonts.h"
#include "USART.h"

//Declaring the start storage location for the command and data to be stored
volatile char* oled_ext_command= (char *) OLED_COMMAND_BASE;
volatile char* oled_ext_data= (char *) OLED_DATA_BASE;
int current_line=0;

/* 
Description: Writes the command into the storage
Input: Command for the memory to be written
Output: Nothing
*/
void write_command(uint8_t command) 
{	
	oled_ext_command[0]=command;
}
/* 
Description: Writes the data command into the storage
Input: Command for the memory to be written
Output: Nothing
*/
void data_command(uint8_t command) 
{
	oled_ext_data[0]=command;
}
/* 
Description: Initialization of the OLED
Input: Nothing
Output: Nothing
*/
void init_oled()
{
	write_command(0xae);        // display off
	write_command(0xa1);        //segment remap
	write_command(0xda);        //common pads hardware: alternative
	write_command(0x12);
	write_command(0xc8);        //common  output scan direction:com63~com0
	write_command(0xa8);        //multiplex ration mode:63
	write_command(0x3f);
	write_command(0xd5);        //display  divide ratio/osc. freq. mode
	write_command(0x80);
	write_command(0x81);        //contrast control
	write_command(0x50);
	write_command(0xd9);        //set pre-charge period
	write_command(0x21);
	write_command(0x20);        //Set Memory Addressing Mode
	write_command(0x00);
	
	write_command(0x21);      // setting the column limits  
	write_command(0x00);
	write_command(127);
		
	write_command(0x22);       //setting the page limits
	write_command(0xB0);
	write_command(0xB7);
		
	write_command(0xdb);        //VCOM deselect level mode
	write_command(0x30);
	write_command(0xad);        //master configuration
	write_command(0x00);
	write_command(0xa4);        //out follows RAM content
	write_command(0xa6);        //set normal display
	write_command(0xaf);        // display on
	//write_command(0xa5);		//Turn on all  the pixels
	reset_oled();
}
/* 
Description: Reseting the whole OLED to display blank
Input: Nothing
Output: Nothing
*/
void reset_oled()
{
	for(int line=0;line<8;line++)
	{
		oled_clear_line(line);
	}
	oled_goto_line(0);	
}

/* 
Description: Clearing the whole line mentioned in OLED
Input: Input of the line to be processsed
Output: Nothing
*/
void oled_clear_line(int line)
{
	oled_goto_line(line);
	for(int column=0;column<128;column++)
	{
		data_command(0);
	}
}
/* 
Description: Going to the specified line in OLED
Input:  Input of the line to be processsed
Output: Nothing
*/
void oled_goto_line(int line)
{
	current_line=line;
	write_command(0x20);
	write_command(0x02);
	//setting the page number
	write_command(0xB0+line); 
	write_command(0x21);      
	write_command(0);
	//resetting the column number 
	write_command(127);		
	write_command(0x20);
	//going back to horizontal addressing
	write_command(0x00);	
}
/* 
Description: Going to the specified Column in OLED
Input:  Input of the Column to be processsed
Output: Nothing
*/
void oled_goto_column(int column)
{	
	write_command(0x21);
	write_command(column);
	//resetting the column number
	write_command(127);			
	write_command(0x20);
	//going back to horizontal addressing
	write_command(0x00);		
}
/* 
Description: Going to the specified line and Column in OLED
Input:  Input of the position to be processsed
Output: Nothing
*/
void oled_goto_pos(int line,int column)
{
	oled_goto_line(line);
	oled_goto_column(column);
}
/* 
Description: Prints the specified data on the OLED depending on the FONT type
Input:  Input of the data and FONT is received
Output: Nothing
*/
void oled_print(char* character,FONTSTYLE font)
{
	int i=0;	
	if(font==SMALL)
	{
		while(character[i] != '\0')
		{
			unsigned char* tempArray= font4[character[i]-32];
			for (int j=0; j<4 ;j++)
			{
				data_command(pgm_read_byte(&tempArray[j]));
			}
			i++;
		}
	}
	else if(font==MEDIUM)
	{
		while(character[i] != '\0')
		{
			unsigned char* tempArray= font5[character[i]-32];
			for (int j=0; j<5 ;j++)
			{
				data_command(pgm_read_byte(&tempArray[j]));
			}
			i++;	
		}
	}
	else {
		while(character[i] != '\0')
		{
			unsigned char* tempArray= font8[character[i]-32];
			for (int j=0; j<8 ;j++)
			{
				data_command(pgm_read_byte(&tempArray[j]));
			}
			i++;
		}
	}
}
/* 
Description: Generates a timer for the value mentioned at the position on the OLED
Input:  Input of the value, line, Column to be mentioned
Output: Nothing
*/
void OLED_timer(int value, int line, int column)
{
	int current_value;
	for(current_value=value; current_value>=0;current_value--)
	{
		print_digit(current_value,line,column);	
	}	
}
/* 
Description: Prints each of the digit on the OLED by converting to a character
Input:  Input of the value, line, Column to be mentioned
Output: Nothing
*/
void print_digit(int digit, int line, int column)
{
	char* temp=NULL;
	temp = convertIntegerToChar(digit);
	oled_goto_line(line);
	oled_goto_column(column);
	oled_print(temp,LARGE);
	_delay_ms(1000);
	reset_oled();	
}

/*
Description: Function used to convert the integer to character (the reference for the source code is from GeeksforGeeks) 
Input:  Input of the value
Output: returns a pointer to character after the conversion
*/
char* convertIntegerToChar(int N)
{
    // Count digits in number N
    int m = N;
    int digit = 0;
	char* arr;
    while (m)
	 {
        // Increment number of digits
        digit++;
        // Truncate the last digit from the number
        m /= 10;
    }
    // Declare char array for result 
    // Declare duplicate char array
    char arr1[digit];
    // Memory allocation of array
    arr = (char*)malloc(digit);
    // Separating integer into digits and accommodate it to character array
    int index = 0;
    while (N)
	 {
        // Separate last digit from the number and add ASCII value of character '0' is 48
        arr1[++index] = N % 10 + '0';
        // Truncate the last digit from the number
        N /= 10;
    }
	int i;
	for (i = 0; i < index; i++) 
	{
		arr[i] = arr1[index - i];
	}
    // Char array truncate by null
    arr[i] = '\0';
    // Return char array
    return (char*)arr;
}

/*
Description: Function used to set the brightness on OLED 
Input:  Inputs a value to set the brightness
Output: Nothing
*/
 void contrast(int value)
{
	write_command(0x81);
	value+=100;
	data_command(0x81+value);	 
}
