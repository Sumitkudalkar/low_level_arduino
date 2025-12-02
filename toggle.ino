const int led = PB5;
const uint16_t reset = 0;
const uint16_t target = 31250;
void setup() {
  DDRB = (1<<led) | DDRB;
  PORTB = ~(1<<led) & PORTB;
  TCCR1A = 0;


  TCCR1B = (1<<CS12) | TCCR1B;
  TCCR1B = ~(1<<CS11) & TCCR1B;
  TCCR1B = ~(1<<CS10) & TCCR1B;

  TCNT1 = reset;

  OCR1A = target;
  TIMSK1 = (1<<OCIE1A) | TIMSK1;

  sei();


}

void loop() {
 delay(1000);
 //assume processor is doing something
  

}

ISR(TIMER1_COMPA_vect){
   PORTB = (1<<led)^PORTB;
   TCNT1=reset;
}