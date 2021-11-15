// Modified by Juan Carlos Juárez
// Serial Port Interface Calculator

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define TRANS_COUNT 9
#define EDO_COUNT 15

char chr=0;
int acum1=0;
int acum2=0;
float res=0;
enum Oper{Suma,Resta,Mult,Div};
enum Oper oper;

int edo=0;
int edoAnt=0;
int trans=0;

float decimal1 = 0;
float decimal2 = 0;

int times1 = 1;
int times2 = 1;

float currDecimal1 = 0;
float currDecimal2 = 0;

float final1 = 0;
float final2 = 0;

int i = 0;


HANDLE *serial;

int aux;
DCB config;
COMMTIMEOUTS touts;

					//0-->Inválida
					//6-->Digito
					//7-->Operador
int chrTrans[TRANS_COUNT]=
					{ 0,'(',')','=',  8, 27, 6 , 7, '.'};
int mtzTrans[EDO_COUNT][TRANS_COUNT]={
					{ 0, 1 , 0 , 0 , 0 , 0 , 0 , 0,  0},

					{ 1, 1 , 1 , 1 , 99, 99, 2 , 1,  1},
					{ 2, 2 , 2 , 2 , 99, 99, 3 , 2,  4},
					{ 3, 2 , 2 , 2 , 99, 99, 2 , 2,  2},

					{ 4, 4 , 4 , 4 , 99, 99, 5 , 4,  4},
					{ 5, 5 , 5 , 5 , 99, 99, 6 , 7,  5},
					{ 6, 5 , 5 , 5 , 99, 99, 5 , 5,  5},

					{ 7, 7 , 7 , 7 , 99, 99, 8 , 7,  7},
					{ 8, 8 , 8 , 8 , 99, 99, 9 , 8,  10},
					{ 9, 8 , 8 , 8 , 99, 99, 8 , 8,  8},

					{ 10, 10 , 10 , 10 , 99, 99, 11 , 10,  10},
					{ 11, 11 , 13 , 11 , 99, 99, 12 , 11,  11},
					{ 12, 11 , 11 , 11 , 99, 99, 11 , 11,  11},

					{ 13, 13 , 13 , 14 , 99, 99, 13 , 13,  13},
					{ 14, 13 , 13 , 14 , 99, 99, 13 , 13,  13}};
					


int calcTrans(char chr) {
	int trans=0;
	if ((chr>='0')&&(chr<='9')){	//Digito
		return(6);
	}else if (chr == '.') {
		//printf("co");
        return(8);
    }
	switch (chr) {
		case'+':
		case'-':
		case'*':
		case'/':
				return(7);
	}
	for (trans=5;trans>0;trans--)
		if (chr==chrTrans[trans])
			break;
	return(trans);
}

int sigEdo(int edo, int trans) {
	return(mtzTrans[edo][trans]);
}

int ejecutaEdo(int edo) {
	switch(edo) {
		case 0:
				break;
		case 1:
				acum1=0;
				printf("%c",chr);
				break;
		case 2:
				printf("%c",chr);
				acum1*=10;
				acum1+=(chr-'0');
				//printf("**2**");
				break;
		case 3:
				printf("%c",chr);
				acum1*=10;
				acum1+=(chr-'0');
				//printf("**3**");
				return(2);
		case 4:
				printf(".");
				times1 = 1;
				decimal1 = 0;
				//acum2=0;	//Preparar la entrada al estado 4
				break;
		case 5:
				printf("%c",chr);
				currDecimal1 = chr - '0';
				
				for(i = 0; i < times1; i++){
					currDecimal1 *= 0.1;
				}
				decimal1 += currDecimal1;
				times1++;
				break;
		case 6:
				printf("%c",chr);
				currDecimal1 = chr - '0';
				
				for(i = 0; i < times1; i++){
					currDecimal1 *= 0.1;
				}
				decimal1 += currDecimal1;
				times1++;
				return(5);
		case 7:
				//printf(" ----- %f ----- %d \n",decimal1, acum1);
				//printf("correct");
				acum2=0;
				switch (chr) {
					case'+':
							oper=Suma;
							break;
					case'-':
							oper=Resta;
							break;
					case'*':
							oper=Mult;
							break;
					case'/':
							oper=Div;
							break;
				}
				printf("%c",chr);
				break;

		case 8:
				printf("%c",chr);
				acum2*=10;
				acum2+=(chr-'0');
				//printf("**2**");
				break;

		case 9:
				printf("%c",chr);
				acum2*=10;
				acum2+=(chr-'0');
				//printf("**2**");
				return (8);

		case 10:
				printf(".");
				times2 = 1;
				decimal2 = 0;
				//acum2=0;	//Preparar la entrada al estado 4
				break;

		case 11:
				printf("%c",chr);
				currDecimal2 = chr - '0';
				
				for(i = 0; i < times2; i++){
					currDecimal2 *= 0.1;
				}
				decimal2 += currDecimal2;
				times2++;
				break;

		case 12:
				printf("%c",chr);
				currDecimal2 = chr - '0';
				
				for(i = 0; i < times2; i++){
					currDecimal2 *= 0.1;
				}
				decimal2 += currDecimal2;
				times2++;
				return (11);

		case 13:
				//printf(" ----- %f ----- %d \n",decimal1, acum1);
				//printf(" ----- %f ----- %d \n",decimal2, acum2);
				//printf("correct");
				printf("%c",chr);
				break;


		case 14: // Pending
				printf("%c",chr);
				final1 = (float)acum1;
				final2 = (float)acum2;
				final1 += decimal1;
				final2 += decimal2;
				switch(oper) {
					case Suma:
							res=final1+final2;
							break;
					case Resta:
							res=final1-final2;
							break;
					case Mult:
							res=final1*final2;
							break;
					case Div:
							if (acum2)
								res=final1/final2;
							else
								res=-1;
							break;
				}
				printf("%f\n",res);
				final1 = 0;
				final2 = 0;
				return(0);
		case 99:
				printf("\n<<<Captura cancelada>>>\n");
				return(0);	//Estado aceptor, rompe la rutina y marca estado de salida
	}
	return(edo);	//Para estados no aceptores regresar el estado ejecutado
}

void main() {
	serial=CreateFile("COM4",GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (serial!=INVALID_HANDLE_VALUE) {
		printf("Communication has been opened\n");

		//Configurar protocolo y velocidad
		GetCommState(serial,&config);
		config.BaudRate=CBR_9600;
		config.fParity=0;
		config.fBinary=1;
		config.StopBits=ONESTOPBIT;
		config.ByteSize=8;
		SetCommState(serial,&config);

		//Configurar "timeouts"
		touts.ReadTotalTimeoutConstant=0;
		touts.ReadIntervalTimeout=0;
		touts.ReadTotalTimeoutMultiplier=0;
		SetCommTimeouts(serial,&touts);

		while (chr!='e') {		//El caracter 'e' termina la ejecución del programa (exit)
				ReadFile(serial,&chr,1,&aux,NULL); // Now we take the input from the Serial Interface
				if (chr!='e') {
					trans=calcTrans(chr);	//Calcular la transición según la entrada del teclado
					if (trans) {			//Validar por transición valida (la transición 0 es inválida)
						//printf("**** %d ****", edo);
						edoAnt=edo;					//Guardar el estado anterior
						edo=sigEdo(edoAnt,trans);	//Calcular el siguiente estado
						if (edoAnt!=edo)			//Solo si hay cambio de estado hay que ...
							edo=ejecutaEdo(edo);	// ... ejecutar el nuevo estado y asignar estado de continuidad
					}
				}
		}
	}
	CloseHandle(serial);
	printf("\nFin!!!\n");
}