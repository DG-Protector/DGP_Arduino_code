//test code for 

/*
 * FSR tester using serial plotter
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-06
*/

#define L_SENSOR A0
#define R_SENSOR A1

void setup() {
  pinMode(L_SENSOR, INPUT);
  pinMode(R_SENSOR, INPUT); // set input device

  Serial.begin(115200);     // open serial communication
  
}

void loop() {
  int l_sensor_value = analogRead(L_SENSOR);
  int r_sensor_value = analogRead(R_SENSOR);  // read senser's value

  Serial.print("LEFT:");
  Serial.print(l_sensor_value);       // print left sensor's value
  Serial.print(",");                  // separate between values 
  Serial.print("RIGHT:");
  Serial.println(r_sensor_value);     // print left sensor's value
  delay(100);
}
