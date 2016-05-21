# fast-gpio
Provides access to GPIOs by directly writing to the hw registers, implements sw PWM as well

# usage

    fast-gpio set-input <gpio>
    fast-gpio set-output <gpio>
    fast-gpio get-direction <gpio>
    fast-gpio read <gpio>
    fast-gpio set <gpio> <value: 0 or 1>
    fast-gpio pwm <gpio> <freq in Hz> <duty cycle percentage>

## verbose output modes

Verbose flags available:

* **-q** quite output
* **-v** verbose output
* **-u** ubus output
* **-d** debug output

Examples:

    fast-gpio -q set-input 5
    fast-gpio -u read 5
    > {"cmd":"Read", "pin":12, "val":"1"}