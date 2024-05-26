#define TRIG_PIN 9
#define ECHO_PIN 10
#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BUTTON_PIN 4

unsigned long startTime;
unsigned long countdownTime = 600000; 
bool countingDown = false;
bool ledState = false; 
unsigned long previousMillis = 0; 
bool systemOn = false; 

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); 
    if (digitalRead(BUTTON_PIN) == LOW) {
      systemOn = !systemOn; 
      delay(500);
    }
  }

  if (systemOn) {
    long duration;
    int distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);

    distance = (duration / 2) / 29.1;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 10) {
      if (!countingDown) {
        startCountdown();
      }
      updateCountdown();
    } else {
      resetCountdown();
    }

    delay(1000);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
}

void startCountdown() {
  countingDown = true;
  startTime = millis();
  digitalWrite(GREEN_LED_PIN, LOW); 
  digitalWrite(RED_LED_PIN, HIGH); 
  Serial.println("START");
}

void resetCountdown() {
  countingDown = false;
  digitalWrite(RED_LED_PIN, LOW); 
  digitalWrite(GREEN_LED_PIN, HIGH); 
  Serial.println("RESET");
}

void updateCountdown() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  if (elapsedTime >= countdownTime) {
    // If the distance greater than 10 cm reaches 10 minutes, the red light flashes and a notification is sent.
    if (currentTime - previousMillis >= 500) { 
      previousMillis = currentTime;
      ledState = !ledState;
      digitalWrite(RED_LED_PIN, ledState);
      Serial.println("ALERT"); 
    }
  } else {
    digitalWrite(RED_LED_PIN, HIGH);
  }
}

