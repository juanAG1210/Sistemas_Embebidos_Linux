#ifndef SENSOR_H
#define SENSOR_H


#include <stddef.h>


// Inicializa la fuente del sensor (semilla rand o abre CSV si existe)
void sensor_init(void);


// Lee un valor de sensor como double
// Si hay CSV, lee siguiente valor; al llegar a EOF, reinicia.
// Si no hay CSV, retorna un aleatorio en [0,100].
double sensor_read(void);


#endif // SENSOR_H
