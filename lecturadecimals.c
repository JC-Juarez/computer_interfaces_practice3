// Modified by Juan Carlos Juárez
// Reading a Decimal Number

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

char chr=0;
float acum=0;
float decimalAcum = 0;
int decimalNumber = 1;
int decimalState = 0;

void main() {
	while (chr!=13 && chr != 32) {
		if (kbhit()) {
			chr=getch();
			if (chr!=13 && chr != 32) {
				if(decimalState == 0){
					if ((chr>='0')&&(chr<='9')) {
						acum*=10;
						acum+=(float)(chr-'0');
						printf("%c [%d] (%f)\n",chr,chr,acum);
					}else if(chr == 46){
						decimalState = 1;
						printf(". [46] \n");
					}
				}else{
					if ((chr>='0')&&(chr<='9')) {
						float currDecimal = (float)(chr-'0');
						int i;
						for(i = 0; i < decimalNumber; i += 1){
							currDecimal *= 0.1;
						}
						decimalNumber += 1;
						decimalAcum += currDecimal;
						printf("%c [%d] (%f)\n",chr,chr,(acum + decimalAcum));
					}else if(chr == 46){
						break;
					}
				}
			}
		}
	}
	acum += decimalAcum;
	printf("\nFin --> %f\n",acum);
}
