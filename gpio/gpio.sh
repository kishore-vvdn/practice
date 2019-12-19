#!/bin/bash

GPIO=2
GPIODIR=/sys/class/gpio/gpio$GPIO

echo "Configuring GPIO $GPIO"

#check if the gpio is already exported
if [ ! -e "$GPIODIR" ]
then
	echo "Exporting GPIO"
	echo $GPIO > /sys/class/gpio/export
else
	echo "GPIO already exported"
fi

echo "Current direction: `cat $GPIODIR/direction`"
echo "Set GPIO as input to read the value"
echo in > $GPIODIR/direction
echo "Current value: `cat $GPIODIR/value`"
echo "Set GPIO as output to change the value"
echo out > $GPIODIR/direction
echo "Set value as low"
echo 0 > $GPIODIR/value
echo "New value: `cat $GPIODIR/value`"


