CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -O2
INCLUDE := -Isensor -Iactuators -Icontroller


SRC_SENSOR := sensor/sensor.c
SRC_ACTS := actuators/led_actuator.c actuators/buzzer_actuator.c
SRC_CTL := controller/ctl.c
OBJS_COMMON := $(SRC_SENSOR:.c=.o) $(SRC_ACTS:.c=.o) $(SRC_CTL:.c=.o)


.PHONY: all ctl64 ctl32 clean


all: ctl64


%.o: %.c
$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


ctl64: CFLAGS += -m64
ctl64: $(OBJS_COMMON)
$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@


ctl32: CFLAGS += -m32
ctl32: $(OBJS_COMMON)
$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@


clean:
rm -f $(OBJS_COMMON) ctl64 ctl32
