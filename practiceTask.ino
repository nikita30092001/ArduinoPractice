#include <EEPROM.h>

#define LED 9
#define LATENCY 1000
#define BLINK_TIME 10
#define WRITE_TIME 5000
#define COUNTER 10
#define ANALOG_MAX 1023

void setup() {
  pinmode(LED, output);
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  blink_by_adc();
  send_to_uart();
  write_in_eeprom();
}

void blink_by_adc()
{
  static unsigned long last_time = millis();
  static bool is_light = false;

  unsigned long current_time = millis();
  if current_time - last_time >= ANALOG_MAX - analogRead(A0)
  {
    digitalWrite(LED, HIGH);
    is_light = true;
    last_time = current_time;
  }
  else if current_time - last_time >= BLINK && is_light
  {
    digitalWrite(LED, LOW);
    is_light = false;
  }
  
}

void send_to_uart()
{
  static unsigned long last_time = millis();

  unsigned long current_time = millis();
  if current_time - last_time >= LATENCY
  {
    Serial.println(analogRead(A0));
  }
}

void write_in_eeprom()
{
  static unsigned long last_time = millis();
  static int i = 0;

  unsigned long current_time = millis();
  if current_time - last_time >= WRITE_TIME 
  {
    if i < COUNTER
    {
      EEPROM.put(i * sizeof(int), analogRead(A0));
      i++;
    }
    else
    {
      clean_eeprom();
      i = 0;
    }
  }
}

void clean_eeprom()
{
  for(int i = 0; i < COUNTER; i++)
  {
    EEPROM.put(i * sizeof(int), 0);
  }
}
