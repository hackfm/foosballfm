/*
  Hack.fm
  
  Goal detection
  
  @author marek@last.fm
*/

String inputString = "";        
boolean stringComplete = false; 

const int BUTTON_PIN = 2;     
const int BUTTON_LED_PIN =  4;

String state = "";

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  pinMode(BUTTON_LED_PIN, OUTPUT);  
  pinMode(BUTTON_PIN, INPUT);  
}

void loop() {
  
  /* React to serial input */
  if (stringComplete) {
    
    boolean handled = false;
    
    if (inputString == "led:1") {
        digitalWrite(BUTTON_LED_PIN, HIGH);
        handled = true;
        
    }
    if (inputString == "led:0") {
        digitalWrite(BUTTON_LED_PIN, LOW);
        handled = true;
    }
    
    /* Send feedback */
    if (handled) {
        Serial.println("ack:"+inputString);
    } 
    else
    {
        Serial.println("err:"+inputString);
    }
    
    /* Clean up */
    inputString = "";
    stringComplete = false;
    
  }
  
  /* Read current state */
  String newState = "sta:";
  
  /* Light barriers */
  int barrier1 = analogRead(A0);
  int barrier2 = analogRead(A1);
  
  newState += (barrier1 > 200) ? "1" : "0";
  newState += ":";
  newState += (barrier2 > 200) ? "1" : "0";
  newState += ":";
  
  /* Button */
  boolean buttonState = digitalRead(BUTTON_PIN);
  newState += buttonState;
  
  /* Send new state should it have changed */
  if (newState != state) {
      state = newState;
      Serial.println(state);
  }
  
  /* Is that really needed? */
  delay(10);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '\n') {
        stringComplete = true;
    } 
    else
    {
        inputString += inChar;
    }
  }
}


