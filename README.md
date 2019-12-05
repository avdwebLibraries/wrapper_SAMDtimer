# Avdweb_SAMDtimer

SAMD21 Timer library for the SAM15x15 and Arduino Zero compatible boards.

This library needs the [Adafruit_ZeroTimer](https://github.com/adafruit/Adafruit_ZeroTimer/) library to run.

For more information visit http://www.avdweb.nl/arduino/libraries/samd21-timer.html

## Basic usage

### 1. Using a timer with an output pin

The library is easy to use; the following code generates a square wave of 1Hz to pin 5 of the Arduino Zero:

    SAMDtimer mytimer1 = SAMDtimer(3, TC_COUNTER_SIZE_16BIT, 5, 1e6,true);

**Explanation:**

- `Mytimer1`: this is the object name of you choice.
- `3`: the pin-table shows that timer3 should be used for output pin 5.
- `TC_COUNTER_SIZE_16BIT`: don't change. This parameter exists just to make the library expandable.
- `5`: output pin 5
- `1e6`: we need to specify the period time in microseconds.
- `true`: optional parameter, specifie if the timer should be enabled (started) or not.

Alert readers might notice that the first two parameters are not strictly necessary because these could be derived from the used pin and period time. However, this would make the timer library less expandable and there will be occur more mistakes with the pin assignments, if more timers are used.

**Notes**

- The maximum period time is 1398080us (0.7Hz).
- The minimum period time is = 2us (500kHz).
- For now, the library has three timers available (number 3, 4, 5), only in 16-bit mode.
- There is no check on the proper parameters values.
- Without specifying the pulse width, it is the half of the period time (duty cycle 50%).

### 2. Timer interrupts

A timer can also be used for calling interrupts, without using a timer output pin. To periodically execute an ISR, simply use this code:

    SAMDtimer mytimer2 = SAMDtimer(4, myISR, 5e5, true);

**Explanation:**

- `Mytimer2`: this is the object name of you choice.
- `4`: take any free timer
- `myISR`: the name of the ISR of you choice.
- `5e5`: the period time in microseconds.
- `true`: optional parameter, specifie if the timer should be enabled (started) or not.

The ISR should look as follows:

    void myISR ()
    {
        //your code here
    }

### 3. Attaching interrupts to a timer with an output pin

A timer can be used for both calling an interrupt and steering its output pin at the same time. This has to be carried out in two steps: first create a timer with an output pin, and than attach an ISR to it.

    Mytimer3.attachInterrupt(myISR);

**Explanation:**

- `Mytimer3`: an existing timer with an output pin, see paragraph 1.
- `myISR`: the name of the ISR, see paragraph 2.

**Notes**

- If the timer is disabled, then the ISR is disabled also.
