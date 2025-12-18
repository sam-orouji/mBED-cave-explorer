// This header has all the (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's what the name
// means.
#include "globals.h"

#include "hardware.h"

// We need to actually instantiate all of the globals (i.e. declare them once
// without the extern keyword). That's what this file does!

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);             // LCD Screen (tx, rx, reset)
Nav_Switch myNav(p12, p15, p14, p16, p13); // initialize nav switch
BufferedSerial pc(USBTX, USBRX, 115200);                  // USB Console (tx, rx)
DigitalIn button1(p21);                 // Pushbuttons (pin)
DigitalIn button2(p22);
DigitalIn button3(p23);
DigitalIn button4(p24);

AnalogOut DACout(p18);                  // Speaker (pin)
PwmOut speaker(p25); 

int hardware_init()
{    
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    button4.mode(PullUp);
    return ERROR_NONE;
}

// lookup your NavSwitch under mbed site > Hardware> Components
// and look at demo code
GameInputs read_inputs() 
{
    GameInputs in;
    // 1. read and invert the buttons (the buttons are active-low)
    in.b1 = !button1;
    in.b2 = !button2;
    in.b3 = !button3;
    in.b4 = !button4;

    // 2. read the navswitch
    in.up = myNav.up();
    in.down = myNav.down();
    in.left = myNav.left();
    in.right = myNav.right();
    in.center = myNav.center();
    // 1. Read action buttons from in 
    //  Low voltage means "pressed"
    
    //2. Read navigation switch buttons from in
    return in;
}


// A shortcut to serial port print
void print(const char* c)
{
    pc.write(c, strlen(c));
}