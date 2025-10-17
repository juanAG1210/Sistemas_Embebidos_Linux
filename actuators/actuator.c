#include "actuator.h"
#include <stdlib.h>


typedef struct { bool on; } LedParams;


static void led_on(Actuator *a){
((LedParams*)a->params)->on = true; // aquí iría GPIO high
}
static void led_off(Actuator *a){
((LedParams*)a->params)->on = false; // aquí iría GPIO low
}
static bool led_status(Actuator *a){
return ((LedParams*)a->params)->on;
}


Actuator make_led_actuator(void){
Actuator a = {0};
a.params = malloc(sizeof(LedParams));
((LedParams*)a.params)->on = false;
a.activate = led_on;
a.deactivate = led_off;
a.status = led_status;
return a;
}
