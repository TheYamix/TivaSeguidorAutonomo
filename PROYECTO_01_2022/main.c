/****************************************************************************************/
/****************************************************************************************/
/***** PONTIFICIA UNIVERSIDAD CATÓLICA DEL PERÚ *****/
/***** PONTIFICIA UNIVERSIDAD CATÓLICA DEL PERÚ *****/
/***** FACULTAD DE CIENCIAS E INGENIERÍA *****/
/***** SISTEMAS DIGITALES *****/
/****************************************************************************************/
/***** Proyecto: Robot Autonomo - Seguidor de Linea *****/
/****************************************************************************************/
/***** Microcontrolador: TM4C123GH6PM *****/
/***** EvalBoard: Tiva C Series TM4C123G LaunchPad *****/
/***** Autor: Nuñez Alvarez Yamil Ivan *****/
/***** Fecha: Nobiembre 2022 *****/
/***** *****/
/****************************************************************************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"

#define rojo 0x02
#define azul 0x04
#define verde 0x08
#define magenta (rojo|azul)
#define amarillo (rojo|verde)
#define blanco (rojo|azul|verde)

volatile uint16_t contador=1;

//funcion timer
void ConfiguraTimer_1ms(void){
	 //Descativamos el modulo del systick
	 NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
	 //TiempoDeseado=1ms; FreqCPU=16MHZ
	 //valor que debe ir en el registro RELOAD=X
	 //X=(TiempoDeseado)*(FreqCPU) -1
	 //X= [[1*10^(-3)] * [16*10^(6)]] -1 =15999
	 //15999 en hexadecimal es 00003E7F*/
	 NVIC_ST_RELOAD_R = (NVIC_ST_RELOAD_R&0xFF000000)|0x00003E7F;

	 // Iniciamos el contador con cero (escribiendo cualquier valor)
	 NVIC_ST_CURRENT_R &= ~(0x00FFFFFF);
	 //Se habilita el modulo del Systick
	 NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN |NVIC_ST_CTRL_ENABLE;
			
}
//funcion para actulizar el valor del contador
void SysTick_Handler(void){
	//preguntamos si paso 1ms
	if((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)){ 
		//cada vez que pase 1ms, el contador de tiempo se incrementa
		contador++;
	}
}

//configuramos el uart0 que usara los puertos PD6 y PD7
void ConfigUART2(void){
	
	//Se activa la señal de reloj del UART0
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART2;
	//Se activa la señal de reloj del GPIOA
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	//Desactivamos el modulo UART0
	UART2_CTL_R &= ~UART_CTL_UARTEN;
	
	
	//desbloqueo del puerto PD7
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R |= (0x80);
	
	//Se desactiva las funciones analogicas
	GPIO_PORTD_AMSEL_R &= ~(0xC0);
	//Activamos funciones alternas en PD6 y PD7 para el uso de comunicacion serial
	GPIO_PORTD_AFSEL_R |= 0xC0;
	//Se conecta el UART0 con los PD6 y PD7
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)|0x11000000;
	//Se activan funciones digitales
	GPIO_PORTD_DEN_R |= 0xC0;
	
	//CONFIGURACION DE VELOCIDAD
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R |= (0x80);
	//Se tiene una velocidad de 9600 bps y una frecuencia de 16Mz -> 16*10^6
	//BRD=(16*10^6)/((16/(2^0))*9600)
	//BRD=104.166 /8.6805
	//DVINT=104  / 8
	//DIVFRAC=(104.166*64) = 11 (aproximado al entero mas cercano)/ 556
	UART2_IBRD_R =(UART2_IBRD_R & 0xFFFF0000) | (104); 
	UART2_FBRD_R =(UART2_FBRD_R & 0xFFFFFFC0) | (11); 
	
	//CONFIGURACION DE TRAMA Y BUFFERS
	
	//8, N, 1
	//X1110X00
	//01110000 = 0x70
	UART2_LCRH_R = (UART2_LCRH_R & 0xFFFFFF00) | (0x70);
	
	//Se habilita el RX y TX
	UART2_CTL_R |= UART_CTL_RXE | UART_CTL_TXE; //(Ya esta por defecto)
	
	//FINALMENTE ACTIVAMOS EL UART
	
	UART2_CTL_R |= UART_CTL_UARTEN;
}
//configuramos el uart0 que usara los puertos PA0 y PA1
void ConfigUART0(void){
	
	//Se activa la señal de reloj del UART0
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
	//Se activa la señal de reloj del GPIOA
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	//Desactivamos el modulo UART0
	UART0_CTL_R &= ~UART_CTL_UARTEN;
	
	//CONFIGURACION DE PINES PA0 Y PA1 PARA EL RX Y TX
	
	//Se desactiva las funciones analogicas
	GPIO_PORTA_AMSEL_R &= ~(0x03);
	//Activamos funciones alternas en PA0 y PA1 para el uso de comunicacion serial
	GPIO_PORTA_AFSEL_R |= 0x03;
	//Se conecta el UART0 con los PA0 y PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011;
	//Se activan funciones digitales
	GPIO_PORTA_DEN_R |= 0x03;
	
	//CONFIGURACION DE VELOCIDAD
	
	//Se tiene una velocidad de 9600 bps y una frecuencia de 16Mz -> 16*10^6
	//BRD=(16*10^6)/((16/(2^0))*9600)
	//BRD=104.166
	//DVINT=104
	//DIVFRAC=(104.166*64) = 11 (aproximado al entero mas cercano)
	UART0_IBRD_R =(UART0_IBRD_R & 0xFFFF0000) | (104); 
	UART0_FBRD_R =(UART0_FBRD_R & 0xFFFFFFC0) | (11); 
	
	//CONFIGURACION DE TRAMA Y BUFFERS
	
	//8, N, 1
	//X1110X00
	//01110000 = 0x70
	UART0_LCRH_R = (UART0_LCRH_R & 0xFFFFFF00) | (0x70);
	
	//Se habilita el RX y TX
	//UART0_CTL_R |= UART_CTL_RXE | UART_CTL_TXE; (Ya esta por defecto)
	
	//FINALMENTE ACTIVAMOS EL UART
	
	UART0_CTL_R |= UART_CTL_UARTEN;
}
//CONFIGURACION DE LOS LEDS A PRENDER
void config_portF(){
	//Se habilita la señal de reloj del Puerto F
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	//Espera a que se active
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0){}

	GPIO_PORTF_DIR_R |= 0x0E; //PF1, PF2 y PF3 como salidas
	GPIO_PORTF_AFSEL_R &= ~(0x0E); //Se desactivan las funciones alternas
	GPIO_PORTF_DEN_R |= 0x0E; //Se activan las funciones digitales
	GPIO_PORTF_DATA_R &= ~(0x0E); //Se apagan los leds
		
}
//Funcion que recibe un caracter por el UART2
uint8_t rxUart2(){
	uint8_t car;
	while ((UART2_FR_R & UART_FR_RXFE)!=0){}//mientras el fifo no este full
	car = UART2_DR_R & 0xFF;
	return car;
}
//Funcion que transmite un caracter por el UART2
void txUart2(uint8_t car){
	
	while ((UART2_FR_R & UART_FR_TXFF)!=0){} //mientras el fifo este full
  	UART2_DR_R = car & 0xFF;
}
//Funcion que transmite una cadena-uart2
void TxCadena2( uint8_t Cadena[]){
	uint8_t i;
	for( i = 0; Cadena[i] != '\0'; i++ )
	txUart2(Cadena[i]);
}
/////////////////////////////////////////////////////////////////////////////
//Funcion que recibe un caracter por el UART0
uint8_t rxUart(){
	uint8_t car;
	while ((UART0_FR_R & UART_FR_RXFE)!=0){}//mientras el fifo no este full
	car = UART0_DR_R & 0xFF;
	return car;
}
uint8_t rxUart2_espera(){
	uint8_t car;
	if((UART2_FR_R & UART_FR_RXFE)==0){}//mientras el fifo no este full
	car = UART2_DR_R & 0xFF;
	return car;
}
//Funcion que transmite un caracter por el UART0
void txUart(uint8_t car){
	
	while ((UART0_FR_R & UART_FR_TXFF)!=0){} //mientras el fifo este full
	UART0_DR_R = car & 0xFF;
}

//Funcion que transmite una cadena-uart0
void TxCadena( uint8_t Cadena[]){
	uint8_t i;
	for( i = 0; Cadena[i] != '\0'; i++ )
	txUart(Cadena[i]);
}

//////////////////////////////////////////////////////////////

void Config_PortB_salidas(void){
 unsigned long temp;
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // Activación del reloj del puerto B
 temp = SYSCTL_RCGC2_R;     	// Espera de activación de reloj del puerto B
 GPIO_PORTB_DIR_R   |= 0x10;	// Pin PB4 como salida
 GPIO_PORTB_AMSEL_R &= ~(0x10); // Deshabilitar la función analógica
 GPIO_PORTB_AFSEL_R |= 0x10;	// Función alterna habilitada
 GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFF00FFF) | 0x77000; // PB4 conectado a T1CCP0
 GPIO_PORTB_DEN_R |= 0x10;  	// Activar función digital
}
  
void ConfiguraPWM(void){
 SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;  	// Activamos señal de reloj para TIMER 1A
 while(!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R1)); // Espera activación del reloj
 TIMER1_CTL_R &= ~(0x0101);  // TIMER1: Modo cuenta descendente, inhabilitado
 TIMER1_CFG_R  = 0x00000004; //16bits
 TIMER1_TAMR_R = 0x0000000A; //TnAMS bit a 1, TnCMR bit a 0, y TnMR a 2
 //Configuración de frecuencia base = 200Hz--> val_tope=79999
 TIMER1_TAPR_R = 0x1; 	//8 bits más significativos
 TIMER1_TAILR_R = 0x387F; // 16 bits menos significativos
 //Ciclo de trabajo = 5% --> valor_comp= 3999
 TIMER1_TAPMR_R = 0x0;   	// 8 bits más significativos
 TIMER1_TAMATCHR_R = 0x7CFF
	; // 16 bits menos significativos
 TIMER1_CTL_R |= 0x0101; 	// Habilitamos el timer y comenzamos a generar el PWM
}

//////////////////////////////////////////////////////////////


void config_portE(void){
	//Se habilita la señal de reloj del Puerto F
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	//Espera a que se active
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4)==0){}

	GPIO_PORTE_DIR_R |= 0x1C; //PE2, PE3 y PE4 como salidas
	GPIO_PORTE_AFSEL_R &= ~(0x1C); //Se desactivan las funciones alternas
	GPIO_PORTE_DEN_R |= 0x1C; //Se activan las funciones digitales
	GPIO_PORTE_DATA_R &= ~(0x1C); 	
}
void config_I2C1 (void)
{
	uint32_t temp;
	SYSCTL_RCGCI2C_R |= (1<<1);
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	temp = SYSCTL_RCGC2_R;
	GPIO_PORTA_AFSEL_R |= (1<<7)|(1<<6); // Se activan las funciones alternas de PA6 y PA7.
	GPIO_PORTA_PUR_R |= (1<<6);
	GPIO_PORTA_PUR_R |= (1<<7);
	GPIO_PORTA_ODR_R |= (1<<7);
	GPIO_PORTA_DEN_R |= (1<<6);
	GPIO_PORTA_DEN_R |= (1<<7);
	GPIO_PORTA_PCTL_R &= ~(0xFF000000);
	GPIO_PORTA_PCTL_R |= (0x33000000);
	I2C1_MCR_R = 0x10;
	I2C1_MTPR_R = 0x09;
} //Fin configuracion I2C1

void config_I2C0 (void)
{
uint32_t temp;
SYSCTL_RCGCI2C_R |= (1<<0);
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
temp = SYSCTL_RCGC2_R;
GPIO_PORTB_AFSEL_R |= (1<<2)|(1<<3); // Se activan las funciones alternas de PB2 y PB3.
GPIO_PORTB_PUR_R |= (1<<2);
GPIO_PORTB_PUR_R |= (1<<3);
GPIO_PORTB_ODR_R |= (1<<3);
GPIO_PORTB_DEN_R |= (1<<2);
GPIO_PORTB_DEN_R |= (1<<3);
GPIO_PORTB_PCTL_R &= ~(0x0000FF00);
GPIO_PORTB_PCTL_R |= (0x00003300);
I2C0_MCR_R = 0x10;
I2C0_MTPR_R = 0x09;
}//Fin configuracion I2C0

//Funciones I2C1
////////////////////////////////////////////////////////////////////////////////
void config_portB_entradas(void){ // pines para los sensores
	//Se habilita la señal de reloj del Puerto B
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	//Espera a que se active
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1)==0){}

	GPIO_PORTB_DIR_R &= ~(0x60); //PB5 y PB6 como entradas
	GPIO_PORTB_AFSEL_R &= ~(0x60); //Se desactivan las funciones alternas
	GPIO_PORTB_DEN_R |= 0x60; //Se activan las funciones digitales
			
}

void config_portC(void){
	//Se habilita la señal de reloj del Puerto C
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	//Espera a que se active
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R2)==0){}

	GPIO_PORTC_DIR_R |= 0xC0; //PC6 Y PC7 como salidas
	GPIO_PORTC_AFSEL_R &= ~(0xC0); //Se desactivan las funciones alternas
	GPIO_PORTC_DEN_R |= 0xC0; //Se activan las funciones digitales
	GPIO_PORTC_DATA_R &= ~(0xC0); 
			
}
//FUNCION PRINCIPAL
int main(void){
	//Se ingresan las configuraciones
	config_portF();
	ConfigUART2();
	ConfigUART0();
	config_I2C0();
	config_I2C1();
  Config_PortB_salidas(); 
	config_portB_entradas();
	config_portC();
  ConfiguraPWM(); 
  config_portE();
	uint32_t i;
	uint8_t car,car1,car2,car3;
  uint8_t centinela=1,centinela2=1;
	//Se inicia el timer
	ConfiguraTimer_1ms();
	//INICIALMENTE APAGAMOS LOS MOTORES
  GPIO_PORTE_DATA_R &= ~(0x0C);
  GPIO_PORTC_DATA_R &= ~(0xC0);
	
	//Ponemos a 1 el PE4, que enviara un 1 logico hacia el STBY del driver de motor
	GPIO_PORTE_DATA_R |=(0x10);	
	while(1){
		//Reinicio de variables iniciales
		car='0';
		car1='0';
		car2='0';
		car3='0';
	 //se espera que un caracter llegue desde el otro dispositivo
	 //mediante bluetooth
	 car=rxUart2();
	 //Si el caracter recibido es 1 se ejecutara lo correspondiente al modo autonomo
	 if(car=='1'){
		 while(car!='q'){
			 //Cada vez que recibamos un caracter, encenderemos un led como indicador de esto(led verde)
			 car=rxUart2();
			 //OPCIONES DE MOVIMIENTO DEL CARRO
			 contador=1;
			 switch(car){
				 //HACIA ADELANTE
				 case 'w':
					 //apago los motores
					 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
					 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
					 //Motor derecho e izquierdo hacia adelante
					 GPIO_PORTE_DATA_R |=(0x08); //AIN 1
					 GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
					 GPIO_PORTC_DATA_R |=(0x80);// BIN 1
					 GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
					 centinela=1;
					 break;
				 //HACIA ATRAS
				 case 's':
					 //apago los motores
					 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
					 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
					 //Motor derecho e izquierdo hacia atras
					 GPIO_PORTE_DATA_R &= ~(0x08);//AIN 1
					 GPIO_PORTE_DATA_R |=(0x04);// AIN 2
					 GPIO_PORTC_DATA_R &= ~(0x80);// BIN 1
					 GPIO_PORTC_DATA_R |=(0x40);// BIN 2
					 break;
				 case 'd':
						 //giro hacia la derecha
						 //apago los motores
						 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
						 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
						 //Se reinicia el contador del timer
						 contador=1;
						 //Un giro de aproximadamente 15 grados 
						 while(contador<150){
							 GPIO_PORTE_DATA_R &= ~(0x08); //AIN 1
							 GPIO_PORTE_DATA_R |=(0x04);// AIN 2
							 GPIO_PORTC_DATA_R |=(0x80);// BIN 1
							 GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
						 }
						 //apago los motores
						 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
						 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
						 break;
				 case 'a':
						 //giro a a izquierda
						 //apago los motores
						 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
						 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
						 //Se reinicia el contador del timer
						 contador=1;
						 //Un giro de aproximadamente 15 grados 
						 while(contador<150){
							 GPIO_PORTE_DATA_R |=(0x08); //AIN 1
							 GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
							 GPIO_PORTC_DATA_R &= ~(0x80);// BIN 1
							 GPIO_PORTC_DATA_R |=(0x40);// BIN 2
						 }
						 //apago los motores
						 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
						 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
						 break;
					 //STOP:
				 case 'p': 
					 //apago los motores
					 GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
					 GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
					 break;
				}
			 
			}
		  //Al presionar q se apagan los motores
			GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
			GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
			
		}else{
			//Ingresa a modo seguidor de linea
			if(car=='2'){
				//espera un caracter de confirmacion para iniciar 
				car=rxUart2();
				//el caracter g indica el inicio del seguimiento
				if(car=='g'){
					while(centinela2){
						//sensor conectado al PB5 - derecho
						if((GPIO_PORTB_DATA_R & 0x20)==0x20){
							//detecta linea negra
							contador=1;
							//gira aproximadamente 5 grados
							while(contador<50){
								 GPIO_PORTE_DATA_R &= ~(0x08); //AIN 1
								 GPIO_PORTE_DATA_R |=(0x04);// AIN 2
								 GPIO_PORTC_DATA_R |=(0x80);// BIN 1
								 GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
							 }
							//sigue avanzando hacia adelante
							 GPIO_PORTE_DATA_R |=(0x08); //AIN 1
							 GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
							 GPIO_PORTC_DATA_R |=(0x80);// BIN 1
							 GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
						}else{
							//sensor conectado al PB6 - izquierdo
							if((GPIO_PORTB_DATA_R & 0x40)==0x40){
								//detecta linea negra
								contador=1;
								//gira aproximadamente 5 grados
								while(contador<50){
									 GPIO_PORTE_DATA_R |=(0x08); //AIN 1
									 GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
									 GPIO_PORTC_DATA_R &= ~(0x80);// BIN 1
									 GPIO_PORTC_DATA_R |=(0x40);// BIN 2
								}
								//sigue avanzando hacia adelante
								GPIO_PORTE_DATA_R |=(0x08); //AIN 1
							  GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
							  GPIO_PORTC_DATA_R |=(0x80);// BIN 1
							  GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
								
							}else{
								//Avanza hasta que detecte algo
								GPIO_PORTE_DATA_R |=(0x08); //AIN 1
								GPIO_PORTE_DATA_R &= ~(0x04);// AIN 2
								GPIO_PORTC_DATA_R |=(0x80);// BIN 1
								GPIO_PORTC_DATA_R &= ~(0x40);// BIN 2
							}
						}
						//espera a que se reciba un caracter en especifico y lo verifica
						//no se queda esperando, solo pasa 
						car=rxUart2_espera();
						if(car=='q'){
							centinela2=0;
							//apago los motores
						  GPIO_PORTE_DATA_R &= ~(0x0C);//MOTOR 1
						  GPIO_PORTC_DATA_R &= ~(0xC0);//MOTOR 2
						}
						
					}
					centinela2=1;
				}
				
			}
			
		}
	 
	}
}
