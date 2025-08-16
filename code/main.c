#include "lcd.h"
#include "keypad.h"
#include <stdlib.h>

float calc(float a, char op, float b){
	float r = 0;
	switch(op){
		case '+': r = a + b; break;
		case '-': r = a - b; break;
		case '*': r = a * b; break;
		case '/': r = (b != 0) ? (a / b) : 0; break;
		default:  r = 0;
	}
	return r;
}

int main(void)
{
	char ch1[16];               
	char ch2[16];               
	unsigned char i1 = 0;       
	unsigned char i2 = 0;       
	unsigned char state = 0;    
	char oper = 0;

	ch1[0] = '\0';
	ch2[0] = '\0';

	keypad_init();
	lcd_init();
	lcd_command(0x80);
	lcd_display("calculator");
	lcd_command(0xC0);

	while (1)
	{
		char key = keypad_scan();
		if (key == '\0') continue;

		if (key >= '0' && key <= '9') {
			
			if (state == 0) {
				
				if (i1 < 15) { 
					ch1[i1++] = key; 
					ch1[i1] = '\0'; 
					lcd_data(key); 
					}
				} else {
				if (i2 < 15) { 
					ch2[i2++] = key; 
					ch2[i2] = '\0'; 
					lcd_data(key); 
					}
			}
		}
		else if ((key=='+' || key=='-' || key=='*' || key=='/') && state==0 && i1>0) {
			oper = key;
			state = 1;
			i2 = 0;
			ch2[0] = '\0';
			lcd_data(key);
		}
		else if (key == '#') {
			if (oper && i1>0 && i2>0) {
				float n1 = atof(ch1);
				float n2 = atof(ch2);
				float res = calc(n1, oper, n2);
				lcd_display("=");
				lcd_number_display(res);   
			}

			// reset for next expression
			i1 = i2 = 0;
			state = 0;
			oper = 0;
			ch1[0] = '\0';
			ch2[0] = '\0';

			
		}
		else if (key == 'C') {
			i1 = i2 = 0;
			state = 0;
			oper = 0;
			ch1[0] = '\0';
			ch2[0] = '\0';
			lcd_command(0x01);
			lcd_command(0x80); 
			lcd_display("calculator");
			lcd_command(0xC0);
		}

		_delay_ms(120); 
	}
}
