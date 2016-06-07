# PrintApplySensorInterceptor
Intercepts the signal from the PrintApply box sensor and delays it according to applicator velocity.

## Parts list:
1 x 100uF, 50v electrolytic capacitor 
1 x 10uF, 50v electrolytic capacitor 
1 x Yellow LED
1 x Red LED
1 x P2N2222A transistor
1 x 1K Ohm, 1W resistor
2 x 10K Ohm, 1/6 resistor
8 x 330 Ohm, 1/6 resistor
1 x SPST lever microswitch
1 x Breakaway pin headers, 24 pin minimum 
2 x Sparkfun RJ45 breakout boards 
2 x Sparkfun RJ45 connectors (non-MagJack)
1 x Arduino Enclosure
1 x mini breadboard and wiring kit
http://www.amazon.com/Boundto-120PCS-Breadboard-Solderless-Prototype/dp/B017FXDLE6
1 x Addicore L7805CV 5v voltage regulator
1 x TO-220 Heatsink, 2.5W
1 x #6-1/4 hex cap screw
1 x #6 washer
1 x #6 nut

## Example output from the Arduino
Starting...

Minimum Velocity: 1000
Maximum Velocity: 55000
Minimum Delay Time: 10
Maximum Delay Time: 100

Ready!
Collecting accelerometer data...
Box detected!
Random Stroke Delay: 76 ms
Velocity: 15985

Waiting...
Collecting accelerometer data...
Box detected!
Random Stroke Delay: 25 ms
Velocity: 46485

Waiting...
Collecting accelerometer data...
Box detected!
Random Stroke Delay: 18 ms
Velocity: 50725

Waiting...

![PrintApplySensorInterceptor inside FoxIV Mod6](/img/Interceptor_InsideCase.JPG)

![Departure Sensor Switch placement](/img/Interceptor_DepartureSwitch.JPG)

![ADXL345 placement on applicator](/img/Interceptor_ApplicatorAccelerometerFar.JPG)
