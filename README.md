# DGP_Hardware_test
 <img src="https://img.shields.io/badge/Arduino-20B2AA?style=for-the-badge&logo=Arduino&logoColor=white">
<br/><br/>

contribution by jihoonkimtech. <br/>
*last updated at 03/30.*
<br/><br/>

## Main Source
<img src="Supplementary materials/source_structure.png">

### DGP_Arduino_code
> main source

<br/>

### DGP Library (Only header)
#### DGP_Gyro
> Function set of Gyro

#### DGP_Servo
> Function set of Servo

#### DGP_Fields
> Extract fields from transferred data

<br/>

## Testcodes
### Gyro
*Source code for Gyro to check values on Serial plotter*
> Source is located in /Gyro <br/>

- gyro testing code in `/gyro_test`

- gyro calibration code in `/gyro_calibration`

- gyro for y,p,r display code in `/ypr_plotter`

<br/>

### Servo
*Source code for testing Servo moter rotating at 360 degree*
> Source is located in /Servo

- servo tester using button code in `/servo_tester_for_360`

- servo tester using button code in `/servo_tester_for_360_Serial`

- servo tester but it's rotate per cycle code in `/servo_tester_per_cycle`

- band winder code in `/band_winder`

<br/>

### Bluetooth
*Source code for testing HC-06 and Extract field from string data*
> Source is located in /Bluetooth

- HC-06 setup using AT command code in `/HC-06_setup`

- extract field from string data using button code in `/field_divider`

- extract field from receive string from Bluetooth code in `/HC-06_tester`

<br/>