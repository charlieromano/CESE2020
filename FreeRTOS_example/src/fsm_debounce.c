#include "sapi.h"
#include "types_config.h"
#include "FreeRTOS.h"


void fsmButtonError( tConfig* config );
void fsmButtonInit( tConfig* config );
void fsmButtonUpdate( tConfig* config );
void buttonPressed( tConfig* config );
void buttonReleased( tConfig* config );

/* accion de el evento de tecla pulsada */
void buttonPressed( tConfig* config )
{
    config->tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void buttonReleased( tConfig* config )
{
    config->tiempo_up = xTaskGetTickCount();
    config->tiempo_medido = config->tiempo_up - config->tiempo_down;

}

void fsmButtonError( tConfig* config )
{
    config->fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( tConfig* config )
{
    config->contFalling = 0;
    config->contRising = 0;
    config->fsmButtonState = BUTTON_UP;  // Set initial state
}

#define DEBOUNCE_TIME 40

// FSM Update Sate Function
void fsmButtonUpdate( tConfig* config )
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
