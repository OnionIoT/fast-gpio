#!/bin/sh

# include the Onion sh lib
. /usr/lib/onion/lib.sh


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

rDuty=$(ExpLedHexToDuty $rHex)
gDuty=$(ExpLedHexToDuty $gHex)
bDuty=$(ExpLedHexToDuty $bHex)

echo "Setting LEDs to: $hex"
echo "Duty: $rDuty $gDuty $bDuty"

#run the pwm
if [ $rHex != "00" ]; then        
        fast-gpio pwm 17 200 $rDuty;  
else                                             
        fast-gpio set 17 1            
fi;                                              
                                      
if [ $gHex != "00" ]; then    
        fast-gpio pwm 16 200 $gDuty;
else                          
        fast-gpio set 16 1          
fi;                              
                            
if [ $bHex != "00" ]; then       
        fast-gpio pwm 15 200 $bDuty;
else                      
        fast-gpio set 15 1          
fi;             
>>>>>>> 39debc7efabed7bde62fc1d00a5309386e323665
