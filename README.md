# Capacitive Soil Moisture Sensor

Having heard all the issues with corrsion on Reistance Soil Moisture senrors, decided we should look at Capacitive Soil Moisture Sensor

We used the [diymore 5pcs Capacitive Soil Moisture Sensor Module](https://www.amazon.com/dp/B07RYCNFZ5) 3.3-5.5V Wide Voltage Wire Corrosion Resistant Soil Humidity Detection 3-Pin Gravity Sensor Garden Watering DIY Module from Amazon.

We are currenlty using three of these senors.

- rosemary 

> NOTE: One of the sensors cables was wired wrong which was very trobling.

Specifications:

- Analog output
- Interface: PH2.0-3P
- Size: 99mmx16mm
- Output Voltage: DC 0-3.0V
- Operating Voltage: DC 3.3-5.5V
- Supports 3-Pin Gravity Sensor interface

## Testing Voltages for Calibration

These were the ranges acorss the three sensors.

- rosemary - For testing this was Dry (2.12 - 2.18 v)
- Sage - For testing this was in water (2.02 - 2.07 v)
- parsley - For testing this was in soil (.910 - .930 v)

We will scale from as

``` yaml
    filters:
     - calibrate_linear:
      - .935 -> 100 (i.e 100 % wet)
      - 2.0 -> 0 (i.e. VERY DRY)
```

Our intent is to use home assistant to inturprut the poits where the plants require water.
