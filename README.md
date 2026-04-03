# Dumbest possible Exposure timer
Uses keys to set a time in seconds, press go to start the exposure.

This is a plan for a timer for alt printing. In this case a regular photographic timer has too short an exposure time, so I've set up an arduino with a 1602 LCD and a relay to turn on a 12v led light.
Due to cut corners, and my having realised that cutting said corners actually made more sense; Rather than setting minutes and hours it is all in seconds. It will reliably give accurate times from one second to about a million seconds. (actually 0-999,999). 

In practice it doesn't make a difference is an exposure is 15 min or 900 seconds (I have to work it out in the first place, so I may as well be unit agnostic).

![20260402_135725](https://github.com/user-attachments/assets/9dd0d299-1f2e-4298-9d5c-bf6fa22b0f9a)

I decided to use keyboard keys as I had them handy, they are functionally just regular momentary switches though. The project box was just something I had lying about. 

The way it works right now is:

When I flick the set/run switch to 'Set' it runs a loop allowing keystrokes on the numbers keys so I can enter a time of 0-9 for each key (single seconds, tens of seconds, hundreds, etc) with the numbers set to roll over to 0 in the case they go over or under 0-9. Reset will set all numbers to 0 (doesn't reset the arduino).

When I've set the number I want for an exposure I flick the 'Set/Run' switch to 'Run' - At this point pressing 'go' will turn on the relay (allowing 12v to the LED strips) and then will count down to zero before switching off again and resetting the loop to go again. If 'Go' or 'Reset' is pressed while the light is running it'll just turn off the relay and reset the number to 0.

![TimerRelay](https://github.com/user-attachments/assets/2e7a3578-e1aa-42be-9620-ac67dbf23bb7)

I haven't labelled keys in the fritzing but all it's pretty obvious which keys are which if you read the code. 

In practice I'm getting power to the arduino by taking 12v in, using that to feed the VIN pin on the arduino so the arduino runs when it's plugged in and then using the relay to trigger the light. 

I'm using an i2c hat on the LCD, hence the four wires (I couldn't be bothered finding the correct image)

Ignoring the fact that this thing is a bit of a rat nest of wires on the inside, this is about the simplest exposure timer that I could make which will do everything I _need_ to have in an exposure timer. It would be more elegant to make a circuit board, but as a sheild it's OK and I should probably do something more elegant with the functionality of the 'Go/Pause' button, but for now this is fine. 

I may come back to making it more elegant but there's every chance this will turn into a permanent _temporary_ fix.

Other stuff I'm doing on my blog https://secretivesquirrel.wordpress.com
