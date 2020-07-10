#include "FreeRTOS.h"
#include "sapi.h"
#include "types.h"

extern xQueueHandle q1;

void fsmButtonError( tButtonFSM* config );
void fsmButtonInit( tButtonFSM* config );
void fsmButtonUpdate( tButtonFSM* config );
void buttonPressed( tButtonFSM* config );
void buttonReleased( tButtonFSM* config );

/* accion de el evento de tecla pulsada */
void buttonPressed( tButtonFSM* config )
{
    config->tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void buttonReleased( tButtonFSM* config )
{
    //config->tiempo_up = xTaskGetTickCount();
    config->tiempo_medido = xTaskGetTickCount() - config->tiempo_down;

    char str[MSG_LEN];

    if (config->tiempo_medido > 0)
    {
        sprintf(str,"TEC%d T=%d ms", config->index, config->tiempo_medido);//xQueueSend(config->)
        if( pdFALSE == (xQueueSend(q1, &str, portMAX_DELAY )))  // QueueSend(q1)
        {
            printf("Unsent data\r\n");
        }
        xSemaphoreGive( config->semaphoreButton );
    }
}

void fsmButtonError( tButtonFSM* config )
{
    config->fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( tButtonFSM* config )
{
    config->contFalling = 0;
    config->contRising = 0;
    config->fsmButtonState = BUTTON_UP;  // Set initial state
}

#define DEBOUNCE_TIME 40

// FSM Update Sate Function
void fsmButtonUpdate( tButtonFSM* config )
{
   // static bool_t flagFalling = FALSE;
    //static bool_t flagRising = FALSE;

    switch( config->fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( config->button ) )
            {
                config->fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( config->contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( config->button ) )
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
            if( gpioRead( config->button ) )
            {
                config->fsmButtonState = STATE_BUTTON_RISING;
            }
            break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( config->contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( config->button ) )
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
