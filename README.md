# CW Decoder

An Arduino-based Morse Code (CW) decoder that captures, processes, and decodes continuous wave transmissions in real-time.

## Overview

This project implements a real-time Morse code decoder using an Arduino microcontroller, specifically designed for testing and debugging CW beacon strings produced by the on-board computer (OBC) of satellites based on the [BIRDS Bus Open Source Project](https://github.com/BIRDSOpenSource). 

The decoder samples digital signals from a CW beacon input, processes the timing patterns to identify dots and dashes, and converts them into readable text. While originally developed for satellite beacon analysis, it can be extended or adapted for other CW decoding applications. The decoder uses hardware timer interrupts for precise timing and includes visual feedback through the built-in LED.

## Applications

### Primary Use Case: BIRDS Satellite Beacon Testing
This decoder was specifically developed for testing and debugging CW beacon transmissions from satellites based on the BIRDS Bus Open Source Project. It enables:

- **Beacon validation**: Verify that satellite OBC beacon strings are transmitted correctly
- **Ground station testing**: Test receiving equipment and antenna systems
- **Signal analysis**: Analyze beacon timing, content, and transmission quality
- **Development support**: Debug beacon generation software and transmission protocols

### Extended Applications
The decoder can be adapted for various other CW applications:
- Amateur radio CW reception and logging
- Educational Morse code learning tools
- Automated CW contest logging
- Vintage radio equipment testing
- General purpose Morse code monitoring

## Features

- **Real-time CW decoding**: Captures and decodes Morse code transmissions as they occur
- **Hardware timer-based sampling**: Uses Timer1 with 60ms intervals for consistent timing
- **Edge detection synchronization**: Automatically syncs with CW signal timing on state changes
- **Visual feedback**: Built-in LED mirrors the CW input signal
- **Complete character set**: Supports all letters (A-Z) and digits (0-9)
- **Word separation**: Detects and handles both character and word boundaries
- **Buffer overflow protection**: Prevents memory corruption with bounds checking

## Hardware Requirements

- Arduino (Uno, Nano, or compatible board)
- CW signal source connected to digital pin 12
- Serial connection for output display (USB or dedicated serial interface)

## Pin Configuration

| Pin | Function | Description |
|-----|----------|-------------|
| 12  | CW_IN    | Digital input for CW signal |
| 13  | LED_BUILTIN | Visual indicator (mirrors CW signal) |

## Technical Specifications

### Timing Parameters
- **Sample Rate**: 60ms intervals (configurable via `TIMER_SETTING`)
- **Timer Prescaler**: 64 (16MHz ÷ 64 = 250kHz timer frequency)
- **Buffer Size**: 500 samples (30 seconds at 60ms intervals)

### Signal Pattern Recognition
- **Dot (·)**: 1 HIGH sample followed by 1 LOW sample
- **Dash (−)**: 3 consecutive HIGH samples followed by 1 LOW sample
- **Character separator**: 3 consecutive LOW samples
- **Word separator**: 12 consecutive LOW samples

## How It Works

### 1. Signal Capture
The program waits for a CW transmission to begin (HIGH signal on pin 12), then starts the hardware timer interrupt. Every 60ms, the interrupt service routine samples the input and stores the digital state in a buffer.

### 2. Edge Detection Synchronization
When a signal edge is detected (state change), the timer is reset to synchronize with the CW timing, ensuring accurate dot/dash detection regardless of when sampling begins.

### 3. Binary to Morse Conversion
After capturing 500 samples, the raw binary data is processed to identify:
- Single HIGH samples as dots (·)
- Three consecutive HIGH samples as dashes (−)
- Spacing patterns for character and word separation

### 4. Morse to Text Translation
The morse code string is tokenized and each pattern is matched against a lookup table containing the complete International Morse Code alphabet and numerals.

### 5. Output
The decoded text is transmitted via serial communication at 9600 baud and can be viewed in the Arduino IDE Serial Monitor or any serial terminal.

## Installation and Setup

1. **Clone or download** this repository
2. **Open** `cw_decoder.ino` in the Arduino IDE
3. **Connect** your CW signal source to digital pin 12
4. **Upload** the sketch to your Arduino
5. **Open** the Serial Monitor at 9600 baud to view decoded output

## Usage

1. Power on the Arduino with the CW decoder loaded
2. Connect your CW signal source to pin 12
3. Open the Serial Monitor (9600 baud)
4. Begin CW transmission - the decoder will automatically start when it detects a signal
5. Watch the built-in LED mirror the CW signal
6. View the decoded text in the Serial Monitor

## Configuration

### Adjusting Timing
The decoder timing can be adjusted by modifying the `TIMER_SETTING` constant:
```cpp
#define TIMER_SETTING 15000  // 15000 = 60ms intervals
```

Calculate new values using: `TIMER_SETTING = (desired_ms * 250000) / 1000`

### Buffer Size
Increase the buffer size for longer transmissions:
```cpp
#define BUFFER_SIZE 1000L  // Doubles capture time to ~60 seconds
```

## Supported Characters

### Letters
A-Z (complete International Morse Code alphabet)

### Numbers  
0-9 (complete set)

### Special Characters
- Space (word separator)
- ? (unknown/unrecognized patterns)

## Troubleshooting

### No Output
- Check serial connection and baud rate (9600)
- Verify CW signal is connected to pin 12
- Ensure CW signal levels are compatible with Arduino (0-5V)

### Incorrect Decoding
- Adjust `TIMER_SETTING` for different CW speeds
- Check signal quality and timing consistency
- Verify proper grounding between CW source and Arduino

### LED Not Responding
- Check that pin 12 is receiving the CW signal
- Verify signal voltage levels (should be 0V/5V logic levels)

## License

This program is free software licensed under the GNU General Public License v2.0. See the [LICENSE](LICENSE) file for details.

## Author

Copyright (C) 2023 Victor Hugo Schulz

## Related Projects

This CW decoder is part of the satellite development ecosystem surrounding the [BIRDS Bus Open Source Project](https://github.com/BIRDSOpenSource), which provides:

- Open source satellite bus designs
- On-board computer (OBC) implementations  
- Communication protocols and beacon systems
- Educational satellite development resources

For more information about the BIRDS project and satellite beacon systems, visit the [BIRDS GitHub repository](https://github.com/BIRDSOpenSource).

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## Version History

- **v1.0**: Initial release with basic CW decoding functionality
- Real-time processing with hardware timer interrupts
- Complete alphanumeric character support
- Visual feedback and serial output
