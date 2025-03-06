#define ENA 22  
#define IN1_A 17   
#define IN2_A 5 
#define ENC_A 15  
#define ENB 23  
#define IN1_B 18   
#define IN2_B 19 
#define ENC_B 16  

volatile long pulses_left = 0;
volatile long pulses_right=0;
unsigned long last_left_pulses_time = 0;
unsigned long last_right_pulses_time=0;
unsigned long debounceDelay = 2000;

unsigned long previousMillis = 0;
const unsigned long interval = 100;
long previous_pulses_left = 0;
long previous_pulses_right = 0;

void IRAM_ATTR ENC_A_ISR() {
  unsigned long time = micros();
  if (time - last_left_pulses_time > debounceDelay) {
    pulses_left++;
    last_left_pulses_time = time;
  }
}
void IRAM_ATTR ENC_B_ISR(){
  unsigned long time=micros();
  if(time-last_right_pulses_time>debounceDelay){
    pulses_right++;
    last_right_pulses_time=time;
  }
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENB, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), ENC_A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_B), ENC_B_ISR,RISING);

  Serial.begin(9600);

  digitalWrite(IN1_A, LOW);
  digitalWrite(IN2_A, HIGH);
  analogWrite(ENA, 85);
  digitalWrite(IN1_B, LOW);
  digitalWrite(IN2_B, HIGH);
  analogWrite(ENB, 85);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    long current_pulses_left = pulses_left;
    long current_pulses_right = pulses_right; 
    long pulse_diff_left = current_pulses_left - previous_pulses_left;
    long pulse_diff_right = current_pulses_right - previous_pulses_right;
    float speed_left = (pulse_diff_left * 1000.0) / interval;
    float speed_right = (pulse_diff_right * 1000.0) / interval;

    Serial.print(speed_left);
    Serial.print(",");
    Serial.println(speed_right);

    previous_pulses_left = current_pulses_left;
    previous_pulses_right=current_pulses_right;
  }
}