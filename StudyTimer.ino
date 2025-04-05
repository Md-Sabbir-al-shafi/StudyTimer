#include <Adafruit_LiquidCrystal.h>

int b1 = 0;
int LED1 = 11;
int LED2 = 10;
int btn1 = 8;
int btn2 = 7;
int buz = 9;
bool backlightState = false;
int lastButtonState = HIGH;

// Timer
int StartTimeSec = 1;
int StartTimeMin = 1;

// Break time
int BreakTimeSec = 30;
int BreakTimeMin = 0;

Adafruit_LiquidCrystal lcd_1(0);

void setup()
{

    pinMode(btn1, INPUT_PULLUP);
    pinMode(btn2, INPUT_PULLUP);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(buz, OUTPUT);

    lcd_1.begin(16, 2);
    lcd_1.print("Welcome Albab");
    delay(1000);
    lcd_1.clear();
    Serial.begin(9600);
}

void loop()
{

    int btn1S = digitalRead(btn1);
    Serial.print(btn1S);

    if (btn1S == LOW)
    {
        b1 = 1;
    }

    if (b1 == 1)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);

        startTimer();
    }
    int reading = digitalRead(btn2);

    // Detect button press transition from HIGH to LOW
    if (reading == LOW && lastButtonState == HIGH)
    {
        delay(50);
        if (digitalRead(btn2) == LOW)
        { // Confirm the press
            backlightState = !backlightState;
            lcd_1.setBacklight(backlightState ? 1 : 0);        
        }
    }

    lastButtonState = reading;
   Serial.println(reading);
}

/************Timer***************** */
void startTimer()
{
    lcd_1.setCursor(0, 0);
    lcd_1.print("Timer: ");

    lcd_1.setCursor(0, 1);
    if (StartTimeMin < 10)
    {
        lcd_1.print("0");
    }
    lcd_1.print(StartTimeMin);

    lcd_1.print(":");

    if (StartTimeSec < 10)
    {
        lcd_1.print("0");
    }
    lcd_1.print(StartTimeSec);

    Serial.print("Timer: ");
    if (StartTimeMin < 10)
    {
        Serial.print("0");
    }
    Serial.print(StartTimeMin);

    Serial.print(":");

    if (StartTimeSec < 10)
    {
        Serial.print("0");
    }
    Serial.println(StartTimeSec);

    if (StartTimeSec == 0)
    {
        if (StartTimeMin <= 0)
        {
            StartTimeSec = 0;
            StartTimeMin = 0;

            tone(buz, 1000);
            delay(3000);
            noTone(buz);

            lcd_1.clear();
            lcd_1.setCursor(0, 0);
            lcd_1.print("Break Time");

            startBreakTime();
            return;
        }
        StartTimeSec = 59;
        StartTimeMin--;
    }
    else
    {
        StartTimeSec--;
    }
 // delay(1000);
}

void startBreakTime()
{
  while (BreakTimeMin > 0 || BreakTimeSec > 0)
  {
    lcd_1.setCursor(0, 1);
    if (BreakTimeMin < 10)
    {
      lcd_1.print("0");
    }
    lcd_1.print(BreakTimeMin);
    lcd_1.print(":");
    if (BreakTimeSec < 10)
    {
      lcd_1.print("0");
    }
    lcd_1.print(BreakTimeSec);

    Serial.print("Break Time: ");
    Serial.print(BreakTimeMin);
    Serial.print(":");
    Serial.print(BreakTimeSec);

    //delay(1000);

    if (BreakTimeSec == 0)
    {
      if (BreakTimeMin == 0)
      {
        BreakTimeMin = 1;
        BreakTimeSec = 5;
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Break Time");
        break;  // Exit break time loop and return to main loop
      }
      BreakTimeSec = 59;
      BreakTimeMin--;
    }
    else
    {
      BreakTimeSec--;
    }
    
    // Here you can check button presses while the break timer runs
    int reading = digitalRead(btn2);
    if (reading == LOW && lastButtonState == HIGH)
    {
      delay(50);
      if (digitalRead(btn2) == LOW)  // Confirm the press
      { 
        backlightState = !backlightState;
        if (backlightState == true)
        {
          lcd_1.setBacklight(true);  // Turn the backlight on
        }
        else
        {
          lcd_1.setBacklight(false); // Turn the backlight off
        }
        while (digitalRead(btn2) == LOW); // Wait for button release
        delay(50);
      }
    }
    lastButtonState = reading;
    Serial.print("   ");
    Serial.println(reading);
  }
  
  rst();
}



void rst()
{
    tone(buz, 1000);
    delay(3000);
    noTone(buz);
    lcd_1.clear();

    b1 = 0;

    // Timer
    StartTimeSec = 1;
    StartTimeMin = 1;

    // Break time
    BreakTimeSec = 30;
    BreakTimeMin = 0;
}
