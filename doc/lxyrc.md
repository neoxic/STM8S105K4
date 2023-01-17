LXYRC Skid Steer Loader
=======================

![](/img/lxyrc1.jpg)

Check out this video: https://www.youtube.com/watch?v=tH5bEEqhnM0


Firmware features
-----------------

+ Hydraulic pump and valve control
+ Dual motor control (tracks)
+ Acceleration ramping
+ LED lighting (headlights, tail light, blinkers, reverse)
+ iBUS servo link with FlySky transmitter


Rationale
---------

The firmware controls response of the pump depending on input on channels 1, 2 and 5. The main goal is to make the operation smooth and fluid comparing to a simple channel mix in the transmitter. The conventional approach does not take into account the fact that there is some travel distance before a valve begins to open at `VALVE_MIN`. In this model, a valve is not immediately fully open upon reaching that point, but rather upon reaching `VALVE_MAX` which takes some more travel. A better strategy is to keep the pump off until a valve is about to start opening and then give the input reduced weight until the valve is fully open. As a bonus, there is no need to create a channel mix on the transmitter for the pump.

![](/img/pump1.jpg)

Another feature is that the digital servos of the valves are also controlled by the firmware. The main reason for that is servo trimming. Due to their design, the valve servos are not centered mechanically in this model hence they must be trimmed. Consequently, the trimming values need to be further taken into account in the firmware for proper pump control. Since there is no point in keeping the same values in two places, they are stored in the firmware only. It simplifies the model's setup on the transmitter to a great extent. As another bonus, the pump/servo refresh rate is fixed at 250Hz.

The track motors are controlled by the firmware in a differential fashion, i.e. channel 3 is forward/reverse and channel 4 is left/right. This makes it possible to use two independent ESCs (or a dual one working in independent mode) for the tracks without having to configure a channel mix on the transmitter.

The blinkers indicate the direction of turning while the model is moving forward or turning around. Additionally, there are two forced blinking modes (emergency and strobe) that are controlled by a 3-way switch on channel 7.


Pinout
------

| Pin | I/O | Description       |
|-----|-----|-------------------|
| D6  | IN  | iBUS servo        |
| C1  | OUT | Lift valve        |
| C2  | OUT | Bucket valve      |
| C3  | OUT | Tool valve        |
| C4  | OUT | Pump              |
| D4  | OUT | Left track        |
| D3  | OUT | Right track       |
| D7  | OUT | Light             |
| D2  | OUT | Left blinker (*)  |
| D0  | OUT | Right blinker (*) |
| C7  | OUT | Reverse (*)       |
| E5  | OUT | Status LED (*)    |
| D5  | OUT | DEBUG             |

(*) active low


Channel mapping
---------------

| # | Action           |
|---|------------------|
| 1 | Bucket           |
| 2 | Lift arm         |
| 3 | Forward/reverse  |
| 4 | Left/right       |
| 5 | Tool             |
| 6 | Light on/off     |
| 7 | Blinkers 1/2/off |


Installation
------------

```
cmake -B build    # Add -D PROG=type to override programmer type
cd build
make
make flash-opts   # Required only once
make flash-lxyrc
```


Upgrading the model
-------------------

My model came without a remote, receiver and light controller. I ended up replacing the rest of the internals because the brushed ESCs were terribly loud and the brushless HobbyKing ESC was simply not suitable for a pump due to an unavoidable 300ms startup delay. The list of the components that I used:

* FlySky FS-i6S transmitter with FS-X6B receiver (iBUS servo + battery voltage sensor)
* Dual VNH5019 motor driver (tracks)
* SPEDIX 25A ESC (pump)
* 2 unprotected 18650 cells as a 2S battery pack

The receiver was trimmed off of unneeded connectors in order to be sandwitched with the board using a heat shrink tube. The dual motor driver was built according to [these instructions](https://github.com/neoxic/STM8-RC-Dual-Motor-Driver) and configured to work in independent mode since differential mixing is already done by the firmware. Note that you can use any track drivers (or a dual one working in independent mode) and/or ESCs with this firmware that suit your needs.

![](/img/lxyrc2.jpg)

You might also want to replace the track motor leads and solder one 0.1uF ceramic capacitor from each terminal to the motor case to protect the internals from electrical noise coming from the motors.

![](/img/lxyrc3.jpg)

For blinkers and reverse lights, I used regular 3mm white/yellow LEDs slightly thinned at the base, so they could fit in the plastic brackets. The same size LEDs were used as headlights.

![](/img/lxyrc4.jpg)

If you intend to use a BlHeli ESC like I did, plug it into the receiver directly and calibrate throttle after a startup beep by moving it first to the maximum and then to the minimum endpoint. When you are done, get into BlHeliSuite and switch _"Motor Direction"_ to _"Bidirectional"_ (or _"Bidirectional Rev."_ should the motor direction be reversed). Additionally, set _"Startup Power"_ to _"1.00"_, turn off _"Low RPM Power Protect"_ and _"Demag Compensation"_, and set _"Beacon Delay"_ to _"Infinite"_. Take a look at the settings I ended up with.

In order to be able to change BlHeli settings later when the model was fully assembled, I wrote a separate piece of stub firmware called [active low signal passthrough](passthru.md) that allowed to configure the ESC via the SWIM pin.

![](/img/blheli1.png)

The LEDs on the schematics denote LED connection points rather than the actual number of diodes. For example, there are two headlights and two reverse lights in this model.

![](/img/circuit1.png)
