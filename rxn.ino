const int led = PB0;
const int button = PD2;
const int reaction = PD3;
const uint8_t mintime = 2;
const uint8_t maxtime = 4;
volatile uint8_t randomnum;
const uint8_t load = 0;
volatile uint16_t rawtime;
volatile uint8_t flag = 0;
volatile uint8_t overflow = 0;
volatile uint8_t int0 = 0;
volatile uint8_t int1 = 0;


float realtime;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  //setting led as output
  DDRB = (1 << led) | DDRB;
  PORTB = ~(1 << led) & PORTB;


  //setting up timer2 so it can be used to generate random number between 2s to 4s
  TCCR2A = 0;
  TCCR2B = ~(1 << CS22) & TCCR2B;
  TCCR2B = (1 << CS20) | TCCR2B;
  TCNT2 = load;
  OCR2A = 255;
  TIMSK2 = (1 << OCIE2A) | TIMSK2;


  //setting the button as input for first time to get random num between 2 to 4
  DDRD = ~(1 << button) & DDRD;
  //default input pullup
  PORTD = (1 << button) | PORTD;
  //setting up int0 external interrupt
  EICRA = ~(1 << ISC00) & EICRA;
  EICRA = (1 << ISC01) | EICRA;
  EIMSK = (1 << INT0) | EIMSK;



  //setting external interrupt for the button at pin3(int1)
  DDRD = ~(1 << reaction) & DDRD;
  //default input pullup
  PORTD = (1 << reaction) | PORTD;
  EICRA = (1 << ISC11) | EICRA;
  EICRA = ~(1 << ISC10) & EICRA;
  EIMSK = (1 << INT1) | EIMSK;

  //instructions
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Press red button");
  display.setCursor(0, 12);
  display.print("to begin");
  display.display();


  sei();                //setting global interrupts
}

void loop() {
  //interfacing with display idk yet about i2c registers yet
  if (int0 == 1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Press blue button");
    display.setCursor(0, 12);
    display.print("when Led turns on");
    display.display();
    int0 = 0;
  }

  if (flag == 1) {
    realtime = ((overflow * 65536) + rawtime) * 0.000016;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(realtime);
    display.setTextSize(2);
    display.setCursor(55, 0);
    display.print("sec");
    display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Press Red to Reset");
    display.display();
    flag = 0;
  }

}

//interrupt when timer2 reaches 4
ISR(TIMER2_COMPA_vect) {
  TCNT2 = load;
}

//when button is pressed which generates random num
ISR(INT0_vect) {
  int0 = 1;
  randomnum = TCNT2;
  randomnum = (randomnum % (maxtime - mintime + 1)) + mintime;


  //turn the led on after randomnum seconds
  TCCR1A = 0;
  TIFR1 = (1 << OCF1A) | TIFR1;
  TCCR1B = (1 << CS12) | TCCR1B;
  TCCR1B = ~(1 << CS11) & TCCR1B;
  TCCR1B = (1 << CS10) | TCCR1B;
  TCNT1 = load;
  OCR1A = randomnum * 15625;
  TIMSK1 = (1 << OCIE1A) | TIMSK1;
}

//turn the led on after randomnum seconds
ISR(TIMER1_COMPA_vect) {
  overflow = 0;
  PORTB = (1 << led) | PORTB;
  TCNT1 = load;
  //once the led goes high counter is resetted to calculate reaction time so presclaer needs to be set
  TCCR1B = (1 << CS12) | TCCR1B;
  TCCR1B = ~(1 << CS11) & TCCR1B;
  TCCR1B = ~(1 << CS10)  & TCCR1B; //Max can be counted upto 1.048 seconds
  //turn off the compare match A interrupt
  TIMSK1 = ~(1 << OCIE1A) & TIMSK1;

  //enable overflow interrupt
  TIMSK1 = (1 << TOIE1) | TIMSK1;
}

//measure time when reaction button goes low
ISR(INT1_vect) {
  int1 = 1;
  if ((1 << led) & PORTB) {
    //when reaction button is pressed we want snapshot of the current time
    rawtime = TCNT1;
    PORTB = ~(1 << led) & PORTB; // turning off the led
    //stopping the timer game over
    TCCR1B = load;

    flag = 1;
  }
}


ISR(TIMER1_OVF_vect) {
  //we dont want to miss the reaction time even after 1.048 sec beacuse timer gets resetted
  //each time it overflow we increment a variable which is then multiplied
  overflow++;
}
