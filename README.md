# DGP_Hardware_test
 <img src="https://img.shields.io/badge/Arduino-20B2AA?style=for-the-badge&logo=Arduino&logoColor=white">
<br/><br/>

contribution by jihoonkimtech. <br/>
*last updated at 03/31.*
<br/><br/>

## Main Source
<img src="Supplementary materials/source_structure.png">

### DGP_Arduino_code
> main source

<br/>

### DGP Library (Only header)
#### DGP_Gyro
> Function set of Gyro
- DGP_Gyro(uint8_t d, uint8_t c, float t = 0.15)
  - constructor of DGP_Gyro class.
- void init(boolean dev = false)
  - initial communication with MPU6050
- int16_t getX()
  - get calulated x
- int16_t getY()
  - get calulated y
- boolean getCali()
  - get isCompCali. isCompCali is check completed calibration.
- void calibration(boolean dev = false)
  - calibrating. set the cali_x, cali_y. and calculating reference ratio. then isCompCali set. if on devMode? display area and calibrated x, y value.
- void disableCali()
  - clear isCompCali.
- boolean checkPosture(boolean dev = false)
  - check is x, y value is in reference ratio area. if true = good posture. if on devMode? display area and x, y value.
- void printSerialCali()
  - dispaly calibrated x, y

#### DGP_Servo
> Function set of Servo
- DGP_Servo(uint8_t l_servo_pin, uint8_t l_sensor_pin, uint8_t r_servo_pin, uint8_t r_sensor_pin)
  - constructor of DGP_Servo class. store pin number of servo and sensor
- void init()
  - initializing servo and sensors.
- void setMaleRef(uint8_t arr[2][3])
  - set male wind power reference using array parameter
- void setFemaleRef(uint8_t arr[2][3])
  - set female wind power reference using array parameter
- void setUser(char op, uint8_t pL,uint8_t pR)
  - setting user's information include operator, parameters
- void winding()
  - winding band to tight then hold
- void unwinding()
  - unwinding band
- void printSerialRefs()
  - display set reference
- void printSerialUsrInfo()
  - display user's information

#### DGP_Fields
> Extract fields from transferred data
- void getOp()
  - get operator field data
- void getPL()
  - get left parameter field data
- void getPR()
  - get right parameter field data
- void extractField(String str)
  - extract fields from string then save private variable
- void printSerialField()
  - display private variables

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