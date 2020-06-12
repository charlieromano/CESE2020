#define DEC    10
#include "sapi.h"


char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}


int main(void){

   boardConfig();

   uartConfig( UART_USB, 115200 );

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */
   adcConfig( ADC_ENABLE ); /* ADC */
   dacConfig( DAC_ENABLE ); /* DAC */

   bool_t ledState1 = OFF;

   /* Contador */
   uint32_t i = 0;

   /* Buffer */
   static char uartBuff_x[10];
   static char uartBuff_y[10];

   /* Variable para almacenar el valor leido del ADC CH1 */
   uint16_t x_value = 0;
   uint16_t y_value = 0;

   /* Variables de delays no bloqueantes */
   delay_t delay1;
   delay_t delay2;

   /* Inicializar Retardo no bloqueante con tiempo en ms */
   delayConfig( &delay1, 500 );
   delayConfig( &delay2, 200 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay1 ) ){

         /* Leo la Entrada Analogica AI0 - ADC0 CH1 */
         x_value = adcRead( CH1 );
         y_value = adcRead( CH2 );

         /* Envío la primer parte del mnesaje a la Uart */
         //uartWriteString( UART_USB, "ADC [CH1;CH2] values: " );

         /* Conversión de x_value entera a ascii con base decimal */
         itoa( x_value, uartBuff_x, DEC );
         itoa( y_value, uartBuff_y, DEC );

         /* Enviar x_value y Enter */
         uartWriteString( UART_USB, "[" );
         uartWriteString( UART_USB, uartBuff_x );
         uartWriteString( UART_USB, "; " );
         uartWriteString( UART_USB, uartBuff_y );
         uartWriteString( UART_USB, "]\r\n" );

         /* Escribo la x_value en la Salida AnalogicaAO - DAC */
         //dacWrite( DAC, x_value );
      }

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay2 ) ){
         ledState1 = !ledState1;
         gpioWrite( LED1, ledState1 );

         /* Si pasaron 20 delays le aumento el tiempo */
         i++;
         if( i == 20 )
            delayWrite( &delay2, 1000 );
      }

   }

   return 0 ;
}

/*==================[end of file]============================================*/
