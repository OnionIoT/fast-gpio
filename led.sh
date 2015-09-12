#!/bin/sh -v

#read color argument
hex=$1;

#split into color components
hex=`echo $hex | sed -e 's/0x//' `
rHex=`echo $hex | sed -e 's/....$//' -e 's/^/0x/' `
gHex=`echo $hex | sed -e 's/^..//' -e 's/..$//' -e 's/^/0x/' `
bHex=`echo $hex | sed -e 's/^....//' -e 's/^/0x/' `

#convert to decimal
rDec=$(($rHex))
gDec=$(($gHex))
bDec=$(($bHex))

#convert to duty cycle
rDuty=$(($rDec*100/255))
gDuty=$(($gDec*100/255))
bDuty=$(($bDec*100/255))

#flip the duty cycle to account for active-low leds
rDuty=$((100-$rDuty))
gDuty=$((100-$gDuty))
bDuty=$((100-$bDuty))


echo "Colors: $rHex $gHex $bHex"
echo "Colors: $rDec $gDec $bDec"
echo "Colors: $rDuty $gDuty $bDuty"

#run the pwm
./fast-gpio pwm 17 200 $rDuty
./fast-gpio pwm 16 200 $gDuty
./fast-gpio pwm 15 200 $bDuty