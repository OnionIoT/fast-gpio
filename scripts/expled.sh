#!/bin/sh

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

# convert hex to decimal
#	argument 1 - hex value
# 	returns hex via echo
HexToDec () {
	hex=$1

	# check if hex has 0x at start
	grep=`echo $hex | grep "0x"`


	if [ "$grep" == "" ]; then
		# add 0x 
		hex=`echo $1 | sed -e 's/^/0x/'`
	fi

	# convert hex to decimal
	dec=$(($hex))

	# return the decimal value
	echo "$dec"
}

# convert hex to duty cycle 
#	argument 1 - hex value (expecting 0x12 format)
# 	returns output via echo
HexToDuty () {
	# convert hex to decimal
	decimal=$( HexToDec $1 )

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
rHex=`echo $hex | sed -e 's/....$//' `
gHex=`echo $hex | sed -e 's/^..//' -e 's/..$//' `
bHex=`echo $hex | sed -e 's/^....//' `

rDuty=$(RgbHexToDuty $rHex)
gDuty=$(RgbHexToDuty $gHex)
bDuty=$(RgbHexToDuty $bHex)

echo "Setting LEDs to: $hex"
echo "Duty: $rDuty $gDuty $bDuty"

#run the pwm
fast-gpio pwm 17 200 $rDuty
fast-gpio pwm 16 200 $gDuty
fast-gpio pwm 15 200 $bDuty