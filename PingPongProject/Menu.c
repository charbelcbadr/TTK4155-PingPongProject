/*
 * Menu.c
 *
 * Created: 04.10.2021 18:17:00
 *  Author: charbelb
 */ 



#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "Menu.h"
#include "OLED.h"

//Initializing the head to NULL and length and page to 0, length to 0
Node* head=NULL;
int length=0;
int page=0;
extern Node* head;

/* 
Description: Creation of a new node and allocating the memory
Input: Data and size of the new node to be created
Output: Returns the new node after creation
*/
Node* create_new_node(char* data,int size)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = data;
	if(length%2==0)
	{
		new_node->column=10;
		new_node->line=page;
	}
	else
	{
		new_node->column=70;
		new_node->line=page;
		page+=3;
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}
/* 
Description: Appending the new node to the linked list and a Circular Linked list is implemented
Input: Data
Output: Nothing
*/
void append_linked_list(char* data)
{
	Node* new_node = create_new_node(data,length);
	Node* temp = NULL;
	//Checks if the head is the first node
	if (head==NULL)
	{
		head=new_node;
		new_node->next=head;
		new_node->prev=head;
	}
	else
	{
		temp=head;
		//Loop until the last second one of the linked list where the last node is not same
		while(temp->next!=head)
		{
			temp=temp->next;
		}
		temp->next=new_node;
		new_node->next=head;
		head->prev=new_node;
		new_node->prev=temp;
	}
	length++;
}
/* 
Description: Display the attributes which needs to be displayed on the OLED
Input: Data
Output: Returns the node structure
*/
Node* displayMenu()
{
	reset_oled();
	head=NULL;
	length=0;
	page=0;
	append_linked_list("Start");
	append_linked_list("Highscore");
	append_linked_list("Calibrate");
	append_linked_list("Replay");
	append_linked_list("Mode");
	append_linked_list("Contrast");
	/*from front*/
	Node* temp = head;
	do
	{
		//arranging elements based on 2 columms
		oled_goto_line(temp->line);
		oled_goto_column(temp->column);	
		//writing data to the oled 	
		oled_print(temp->data,MEDIUM);		
		temp=temp->next;		
	}while(temp!=head);
	return head;
}
/* 
Description: Display the arrow on the OLED
Input: Nothing
Output: Nothing
*/
void displayArrow(int line,int column)
{
	oled_goto_pos(line,column);
	data_command(0b00011000);
	data_command(0b00011000);
	data_command(0b01111110);
	data_command(0b00111100);
	data_command(0b00011000);
}

/* 
Description: Clear the arrow on the OLED
Input: Nothing
Output: Nothing
*/
void clearArrow(int line,int column)
{
	oled_goto_pos(line,column);
	data_command(0b00000000);
	data_command(0b00000000);
	data_command(0b00000000);
	data_command(0b00000000);
	data_command(0b00000000);
}

/*
Description: Creating a new sub menu for only Modes to be visible and chosen from the user
Input: Nothing
Output: Returns the node structure
*/
Node* Sub_displayMenu()
{
	reset_oled();
	head=NULL;
	length=0;
	page=0;

	append_linked_list("EASY");
	append_linked_list("DIFFICULT");
	clearArrow(1,0);
	/*from front*/
	Node* temp = head;
	do
	{
		//arranging elements based on 2 columns
		oled_goto_line(temp->line);
		oled_goto_column(temp->column);
		//writing data to the oled
		oled_print(temp->data,MEDIUM);
		temp=temp->next;
	}while(temp!=head);
	return head;
}

/*
Description: Creating a new sub menu for only Contrast to be visible and chosen from the user
Input: Nothing
Output: Returns the node structure
*/
Node* Sub_displayMenu_contrast()
{
	reset_oled();
	head=NULL;
	length=0;
	page=0;

	append_linked_list("LOW");
	append_linked_list("MEDIUM");
	append_linked_list("BRIGHT");
	clearArrow(1,0);
	/*from front*/
	Node* temp = head;
	do
	{
		//arranging elements based on 2 columns
		oled_goto_line(temp->line);
		oled_goto_column(temp->column);
		//writing data to the oled
		oled_print(temp->data,MEDIUM);
		temp=temp->next;
	}while(temp!=head);
	return head;
}