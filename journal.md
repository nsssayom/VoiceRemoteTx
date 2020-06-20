# Work Journal

## June 20 2020

### 10.03 AM

Arduino IDE is not well suited for this scale of project. I am looking for alternative with better linting tools and directory management.

### 10:21 AM

Selected [Platform IO](https://platformio.org/) to be used with VS Code. Installing it along with all the dependencies.

Arduino IDE is also one of its dependency. And installed it from SnapCraft using following command:

`$ sudo snap install arduino`.

### 10:40 AM

Installation complete but unable to upload code. Fixed using the following command:

`$ sudo usermod -a -G dialout $USER`

`$ arduino.pip install requests`

### 11:16 AM

Finished up initializing git repo.
