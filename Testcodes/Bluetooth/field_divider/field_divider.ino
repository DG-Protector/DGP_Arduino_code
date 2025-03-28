/*
 * field divider
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

/* about fields
 * |op  |paramL  |separtor |paramR |eof  |
 * op is operator (it's char)
   - 'u' or 'U' : user setting.
   - 'l' or 'L' : low power setting for female(lowercase) or male(uppercase). no need param.
   - 'm' or 'M : middle power setting for female(lowercase) or male(uppercase). no need param.
   - 'h' or 'H' : high power setting for female(lowercase) or male(uppercase). no need param.
 * paramL is parameter of left motor (it's short)
   - only values between 0 and 100
 * separtor is for distinguish between parameters (it's always (char)',')
 * paramR is parameter of right motor (it's short)
   - only values between 0 and 100
 * eof is for point end of field (it's always (char)'.')
*/

// structure of Field
struct Fields{
  char op;
  short paramLeft = 0;
  short paramRight = 0;
};

struct Fields F;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if(Serial.available()){             // if available serial comm
    String cmd = Serial.readString();
    F = extractField(cmd);
    printSerialField(F);
  }
}

// funcion for extrat field at string
struct Fields extractField(String str){
  struct Fields f;

  // read op
  f.op = str.charAt(0);          // copy op field
  str.remove(0,1);               // remove op in origin fields

  bool isNeedParams = false;
  switch(f.op){ // check need param
    case 'l':   // case of low power for female setting
    case 'L':   // case of low power for male setting
    case 'm':   // case of middle power for female setting
    case 'M':   // case of middle power for male setting
    case 'h':   // case of high power for female setting
    case 'H':   // case of high power for male setting
                // no need param
      break;
    case 'u':
    case 'U':               // case of user setting
      isNeedParams = true;  // need param
      break;
    default:
      break;
  }

  if(isNeedParams){
    short ref = 0;
    ref = str.indexOf(',');                          // check index for separator
    f.paramLeft = str.substring(0, ref).toInt(); // extract paramL
    str.remove(0, ref+1);                            // remove paramL & separator

    ref = str.indexOf('.');                          // check index for end of field
    f.paramRight = str.substring(0, ref).toInt();// extract paramR
    str.remove(0, ref+1);                            // remove paramR & end of field
    }
    return f;
}

// print fields for debug
void printSerialField(struct Fields f){
  Serial.println("=====start=====");
  Serial.print("op : ");
  Serial.println(f.op);
  Serial.print("pL : ");
  Serial.println(f.paramLeft);
  Serial.print("pR : ");
  Serial.println(f.paramRight);
  Serial.println("======end======");
}
