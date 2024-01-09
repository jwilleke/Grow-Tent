# Capacitive Soil Moisture Sensor

Having heard all the issues with corrsion on Reistance Soil Moisture senrors, decided we should look at Capacitive Soil Moisture Sensor

We used the [diymore 5pcs Capacitive Soil Moisture Sensor Module](https://www.amazon.com/dp/B07RYCNFZ5) 3.3-5.5V Wide Voltage Wire Corrosion Resistant Soil Humidity Detection 3-Pin Gravity Sensor Garden Watering DIY Module from Amazon.

We are currenlty using three of these senors.

- rosemary
- sage
- parsley

> NOTE: One of the sensors cables was wired wrong which was very trobling. Check everything!

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

We will scale from 0-100% and this code seems to work well and auto adjust the scaling.

``` yaml
    filters:    
      - lambda: !lambda |-
          if (id(dryValue) < x) {
            // Auto-calibrate dryValue
            id(dryValue) = x;
          }
          if (id(wetValue) > x) {
          // Auto-calibrate wetValue
            id(wetValue) = x;
          }
          // Scale x: dryValue->wetValue, 0->100
          return (x - id(dryValue)) * (100 - 0) / (id(wetValue) - id(dryValue)) + 0;
```

Our intent is to use home assistant to inturprut the poits where the plants require water.
