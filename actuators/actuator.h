#ifndef ACTUATOR_H
#define ACTUATOR_H


#include <stdbool.h>


typedef struct Actuator {
void *params;
void (*activate)(struct Actuator *);
void (*deactivate)(struct Actuator *);
bool (*status)(struct Actuator *);
} Actuator;


// Constructores de backends
Actuator make_led_actuator(void);
Actuator make_buzzer_actuator(void);


#endif // ACTUATOR_H
