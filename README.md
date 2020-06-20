# VoiceRemoteTx

## Hardware Configuration

### Interfacing nRF24L01 with MCU

Pinout of nRF24L01 is following:

<img src="./docs/images/nRF24L01-Pinout.png" width="280">


| nRF24L01 | Arduino Nano/Uno | Arduino Mega |
|--|--|--|
| V<sub>cc</sub> (3.3V) | 3.3V | 3.3V |
| GND | GND | GND |
| CSN<sup>[1](#footnote1)</sup> | 8 | 8 |
| CE<sup>[1](#footnote1)</sup> | 7 | 7 |
| SCK | 13 | 52 |
| MOSI | 11 | 51 |
| MISO | 12 | 50 |

<a name="footnote1">1</a>: Declared in code

The interfacing was commenced with female to female jumper cable