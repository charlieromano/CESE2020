
typedef enum BombSignals{
	UP_SIG,
	DOWN_SIG,
	ARM_SIG,
	TICK_SIG
};

typedef enum BombStates{
	SETTING_STATE,
	TIMING_STATE
};

typedef Struct EventTag{
	uint16_t sig;

} Event;

typedef struct TickEvtTag{
	Event super;
	uint8_t fine_time;
} TickEvt;

typedef struct Boomb1Tag {
	uint8_t state;
	uint8_t timeout;
	uint8_t code;
	uint8_t defuse;
} Bomb1;

#define TRAN(target_)(me->state=(uint8_t)(target_)) // Macro for taking a state transition

void Bomb1_ctor(Bomb1 *me, uint8_t defuse){ //the "constructor"
	me->defuse=defuse;
}

void Bomb1_init(Bomb1 *me){
	me->timeout=INIT_TIMEOUT;
	TRAN(SETTING_STATE);
}

void Bomb1_dispatch(Bomb1 *me, Event const *e){
	switch(me-state){
		case SETTING_STATE:{
			switch(e->sig){
				case UP_SIG:{
					if(me->timeout<60){
						--me->timeout;
						
					}

				}
				break;

			}
		break;
		case TIMING_STATE:{
			switch(e->sig){

			}
		break;
		}
	}
}