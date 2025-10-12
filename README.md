Display Characters on SAA1064 7-Segment Custom Shield with K64F
===============================================================

Features
--------
* Displays common characters and decimal point numbers on 4 digit 7-segment LEDs.
* Supports longer strings with scrolling display.
* Can be used on other 7-segment setups supporting i2c Inter-Integrated Circuit communication * 

\* Most probably only with adapting the encoded character constants in the source code. See [Implementation](#Implementation) .

Board Setup
-----------
The package from the internet of things course during CH Open 2015 is very good to experiment with different sensors and other peripherals.
[CH Open 2015 Custom Shield](https://os.mbed.com/users/marcel1691/notebook/internet-of-things-kit-iotkit-smd-shield/).

It has an integrated 4 digit display on address 0x70.

![Display Demo](https://github.com/evolutionsoftswiss/k64f-saa1064-7segment-display-mbed/blob/master/k64f-saa1064-7segment-display-mbed.gif)

Above you see the running main method. The K64F base board under the custom shield is branched with standard USB to supply power and program loading.

Implementation
--------------
Displaying characters on a 7-segment display requires hardware knowledge and low-level bit manipulation. Each character is represented by **seven segments plus a decimal point**, forming an **8-bit pattern** (1 byte). The SAA1064 uses these patterns to turn individual segments **on or off**. 


### Segment Bit Mapping
Below is a typical 7-segment layout and the corresponding bit numbering used in the `SEGMENT_MAP`:

```
      a
     ---
  f |   | b
     -g-
  e |   | c
     ---
      d   dp
```

| Segment | Bit Position |
|---------|--------------|
| a       | 6            |
| b       | 7            |
| c       | 2            |
| d       | 1            |
| e       | 0            |
| f       | 5            |
| g       | 4            |
| dp      | 3            |

Example: To display `0` with no decimal point:  

```cpp
SEGMENT_MAP['0'] = 0xE7; // 11100111
```
This turns a, b, c, d, e, f ON, g AND dp OFF.

```cpp
SEGMENT_MAP['0'] | 0x08 // turns on decimal point
```
### Adapting to Different Hardware
Different displays may use a different segment-to-bit wiring, so the patterns in SEGMENT_MAP might not work out-of-the-box. To adapt:

   * Identify which bit controls which segment on your hardware.
   * Update the SEGMENT_MAP values so that each segment lights up correctly.
   * Initialize SAA1064 object with pins and address from your hardware:

```cpp
SAA1064(PinName sda = D14, PinName scl = D15, uint8_t deviceAddress = SAA1064_SA0 );
```

