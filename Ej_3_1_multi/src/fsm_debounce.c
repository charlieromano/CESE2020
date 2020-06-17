
#include "sapi.h"

#include "tipos.h"
#include "FreeRTOS.h"


void fsmButtonError( tLedTecla* config );
void fsmButtonInit( tLedTecla* config );
void fsmButtonUpdate( tLedTecla* config );
void buttonPressed( tLedTecla* config );
void buttonReleased( tLedTecla* config );

/* accion de el evento de tecla pulsada */
void buttonPressed( tLedTecla* config )
{
	config->tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void buttonReleased( tLedTecla* config )
{
	config->tiempo_up = xTaskGetTickCount();
	config->tiempo_medido = config->tiempo_up - config->tiempo_down;

	if (config->tiempo_medido > 0)
		xQueueSend( config->queue_tec_pulsada , &(config->tiempo_medido),  portMAX_DELAY  );
}

void fsmButtonError( tLedTecla* config )
{
	config->fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( tLedTecla* config )
{
	config->contFalling = 0;
	config->contRising = 0;
	config->fsmButtonState = BUTTON_UP;  // Set initial state
}

#define DEBOUNCE_TIME 40

// FSM Update Sate Function
void fsmButtonUpdate( tLedTecla* config )
{
   // static bool_t flagFalling = FALSE;
    //static bool_t flagRising = FALSE;

    switch( config->fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( config->tecla ) )
            {
            	config->fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( config->contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    buttonPressed(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_UP;
                }

                config->contFalling = 0;
            }

            config->contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( config->tecla ) )
			{
				config->fsmButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( config->contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( config->tecla ) )
                {
                	config->fsmButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    buttonReleased(config);
                }
                else
                {
                	config->fsmButtonState = STATE_BUTTON_DOWN;
                }
                config->contRising = 0;
            }
            config->contRising++;

            /* LEAVE */
            break;

        default:
            fsmButtonError(config);
            break;
    }
}
