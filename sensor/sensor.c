#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


static FILE *g_csv = NULL;


static int file_exists(const char *path){
FILE *f = fopen(path, "r");
if (!f) return 0;
fclose(f);
return 1;
}


void sensor_init(void) {
// Opción CSV si existe el archivo
if (file_exists("sensor_feed.csv")) {
g_csv = fopen("sensor_feed.csv", "r");
} else {
g_csv = NULL;
}
// Semilla para aleatorio como respaldo
srand((unsigned)time(NULL));
}


static double read_csv_or_restart(void){
if (!g_csv) return 0.0; // no CSV
double x = 0.0;
int rc = fscanf(g_csv, "%lf", &x);
if (rc == 1) return x;
// Reiniciar si EOF o error
clearerr(g_csv);
fseek(g_csv, 0, SEEK_SET);
if (fscanf(g_csv, "%lf", &x) == 1) return x;
return 0.0; // si el csv está vacío
}


double sensor_read(void) {
if (g_csv) {
return read_csv_or_restart();
}
// Aleatorio 0..100 si no hay CSV
return (double)(rand() % 101);
}
