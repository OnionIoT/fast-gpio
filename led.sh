#!/bin/sh -v

Usage () {
	echo "Functionality:"
	echo "	Set colors on Omega Expansion Dock LED"
	echo ""
	echo "Usage:"
	echo "$0 <hex value>"
	echo "	Displays color of hex value on the Exp Dock LED"
	echo ""
	echo "$0 -help"
	echo "	Displays this prompt"
	echo ""
}

# convert hex to duty cycle 
#	argument 1 - hex value (expecting 0x12 format)
# 	returns output via echo
HexToDuty () {
	# convert hex to decimal
	decimal=$(($1))

	# find the duty %
	duty=$(($decimal*100/255))

	#return the duty
	echo "$duty"
}

# flip duty cycle polarity
#	argument 1 - duty cycle (0-100)
#	returns output via echo
FlipDutyPolarity () {
	# flip the polarity
	ret=$((100-$1))

	# return the new duty
	echo "$ret"
}

# performs hex to duty conversion for exp led
# 	argument 1 - hex value (0x12 format)
# 	returns duty via exho
RgbHexToDuty () {
	# hex to duty
	duty=$( HexToDuty $1 )

	# flip the duty polarity
	dutyFlip=$( FlipDutyPolarity $duty )

	# return the duty
	echo "$dutyFlip"
}


########################
##### Main Program #####

# check for arguments
if 	[ $# != 1 ] ||
	[ "$1" == "-help" ] || 
	[ "$1" == "-h" ] || 
	[ "$1" == "--help" ] || 
	[ "$1" == "--h" ];
then
	Usage
	exit;
fi

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

rDuty2=$(RgbHexToDuty $rHex)
gDuty2=$(RgbHexToDuty $gHex)
bDuty2=$(RgbHexToDuty $bHex)


echo "Colors: $rHex $gHex $bHex"
echo "Colors: $rDec $gDec $bDec"
echo "Colors: $rDuty $gDuty $bDuty"
echo "Colors: $rDuty2 $gDuty2 $bDuty2"

#run the pwm
./fast-gpio pwm 17 200 $rDuty
./fast-gpio pwm 16 200 $gDuty
./fast-gpio pwm 15 200 $bDuty