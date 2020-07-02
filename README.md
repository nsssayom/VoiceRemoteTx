# VoiceRemoteTx

## Hardware Configuration

### Interfacing nRF24L01 with MCU

Pinout of nRF24L01 is following:

<img src="./docs/images/nRF24L01-Pinout.png" width="280">

The power consumption of this module is just around 12mA during transmission, which is even lower than a single LED. The operating voltage of the module is from 1.9 to 3.6V, but the other pins tolerate 5V logic. SCK, MOSI and MISO - these pins are for the SPI communication and they need to be connected to the SPI pins of the Arduino, but note that each Arduino board have different SPI pins. The pins CSN and CE can be connected to any digital pin of the Arduino board and they are used for setting the module in standby or active mode, as well as for switching between transmit or command mode. The last pin is an interrupt pin which doesn’t have to be used.

For the Tx module of this project, pins of nRF24L01 module will be connected to Arduino according to the following table:

| nRF24L01 | Arduino Nano/Uno | Arduino Mega |
|--|--|--|
| V<sub>cc</sub> (3.3V) | 3.3V | 3.3V |
| GND | GND | GND |
| CSN<sup>[1](#footnote1)</sup> | 8 | 8 |
| CE<sup>[1](#footnote1)</sup> | 7 | 7 |
| SCK | 13 | 52 |
| MOSI | 11 | 51 |
| MISO | 12 | 50 |
| IRQ | Not Connected | Not Connected |

### Interfacing the Keypad with MCU

This keypad has 12 buttons, arranged in a 3×4 grid. The keys are connected into a matrix, so we need 7 microcontroller pins (3-columns and 4-rows) to connect the key pad. The pinout of the keypad module is shown below:

<img src="./docs/images/keypad-pinout.png" width="200">

Keypad pins will be connected with the following arduino pins:

| Keypad Pin | Arduino Pin |
|--|--|
| R1 | D9 |
| R2 | A1 <sup>[2](#footnote2)</sup> |
| R3 | D5 |
| R4 | D6 |
| C1 | D2 |
| C2 | D3 |
| C3 | D4 |

### Interfacing Microphone Module with MCU

The microphone (or preamp) out should be connected to A0 pin of the Arduino.

| Microphone (or Preamp) Pin | Arduino Pin |
|--|--|
| Output | AO |
| GND | GND |

## Loading Sketch

> This project is developed and tested in Debian Buster machine. For all standard Debian and Ubuntu based systems following instructions are expected to work.


### Install VS Code

Use [Snap](https://snapcraft.io/code) to install VS Code if not installed already.
```bash
$ sudo snap install code --classic
```

If your system does not have snap installed already, install it.

```bash
$ sudo apt update
$ sudo apt install snapd
```

### Install PlatformIO

PlatformIO was used for smoother development and debugging features. It has all the necessary tool-chains for embedded system development. Install the [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) for VS Code.

Launch VS Code Quick Open ( <kbd>Ctrl</kbd> + <kbd>P</kbd> ), paste the following command, and press enter.

```bash
ext install platformio.platformio-ide
```

### Configuring PlatformIO  

Plug in the Arduino USB cable in a USB port and issue following command:

```bash
$ dmesg | grep tty
```

The last line of the output should contain the name of the serial port the arduino is connected to. It should be something like `ttyACM0`  or `ttyUSB0`.

Now navigate to __platformio.ini__ file located in the project root directory and set the `upload_port` and `monitor_port` to the serial port name the Arduino is connected to.

Example:

```ini
[env:nanoatmega328]
platform = atmelavr
board = nanoatmega328
framework = arduino
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
monitor_speed = 115200
```

Follow the [official documentation](https://docs.platformio.org/en/latest/integration/ide/vscode.html) of platformIO if necessary.

### Upload Sketch

Upon connecting the board and configuring it correctly, Open VS Code command pallette with pressing <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd>.

Use the `PlatformIO: Upload` command to upload the code to the board.

## Data Format

The buffer size for data transfer is 32 bit. Upon every button press a 32 bit packet will be transmitted. Audio transmission will be achieved using multiple 32 bit packets. A header packet will indicate the commencement of an audio transmission and a stop packet will symbolize the end.

### Button Signal Format

The 32 bit button signal packet will be of following structure:

| Character Index | 0 | 1-6 | 7-22 | 23-29 | 30 | 31 |
|--|--|--|--|--|--|--|
| Character | ␂ | &lt;inpt>| 0 or 1 | &lt;/inpt>| ␄ | Not used yet |

> Assuming a 0 based index system.

According to the use case of this remote, only one button press can be registered at a time. So, any one of the characters from index `7` to `22` would be `1` and the rest would be `0`. The character set to `1` would symbolize the button being pressed.

| Character at Index set to `1`| Corresponding Button |
|--|--|
| 7 | 0 |
| 8 | 1 |
| 9 | 2 |
| 10 | 3 |
| 11 | 4 |
| 12 | 5 |
| 13 | 6 |
| 14 | 7 |
| 15 | 8 |
| 16 | 9 |
| 18 | # |
___

##### Footnotes

<a name="footnote1">[1]</a> Declared in code</br>
<a name="footnote2">[2]</a> Originally D10 was used. But that pin was damaged later.
