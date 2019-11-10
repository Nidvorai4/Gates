//#include <avr/interrupt.h>
/*digipin 1  */
/*digipin 2  */   #define ButtonPin 2
/*digipin 3  */   
/*digipin 4  */   
/*digipin 5  */   
/*digipin 6  */   #define MosfetGatePin 6
/*digipin 7  */   
/*digipin 8  */
/*digipin 9  */   
/*digipin 10 */   
/*digipin 11 */  
/*digipin 12 */  
/*digipin 13 */   #define ledPin 13
/*analogpin 0 */ 
/*analogpin 1 */
/*analogpin 2 */
/*analogpin 3 */
/*analogpin 4 */
/*analogpin 5 */
/*analogpin 6 */
/*analogpin 7 */

enum Rejim {OFF, ON,UP,DOWN,SETUP_UP,SETUP_DOWN};



char buttonState = 0;
int pwm=0;
unsigned long VoshodMilisec=5000;
int DaySec=2;
unsigned long ZakatMilisec=5000;
volatile unsigned long AntiDizTime=0;
unsigned long PressedTime=0;



//=====================================================================================================
void Timer1DisEnable(byte ON1_OFF0, unsigned long PeriodMilis = 1000)
{
    cli();  // отключить глобальные прерывания
    TCCR1A = 0;   // установить регистры в 0
    TCCR1B = 0;
    OCR1A = 15624/1000*PeriodMilis; // установка регистра совпадения
    TCCR1B |= (1 << WGM12);  // включить CTC режим 
    TCCR1B |= (ON1_OFF0 << CS10); // Установить биты на коэффициент деления 1024
    TCCR1B |= (ON1_OFF0 << CS12);
    TIMSK1 |= (ON1_OFF0 << OCIE1A);  // включить прерывание по совпадению таймера 
    sei(); // включить глобальные прерывания
}
//=====================================================================================================
ISR(TIMER1_COMPA_vect)
{
    int razn=millis()-AntiDizTime;
    AntiDizTime=millis();
    Serial.print("_______________INTERRUPT   ");
    Serial.println(razn);
    
    digitalWrite(ledPin, !digitalRead(ledPin));
    
}
//=====================================================================================================

void setup() {
    // put your setup code here, to run once:
   Rejim Rej=OFF;
   pinMode(ButtonPin, INPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(MosfetGatePin, OUTPUT);
   Serial.begin(9600);
   attachInterrupt(0, int0, CHANGE);
   Timer1DisEnable(1,40);
}
//=====================================================================================================
void int0(){
  if(millis()-AntiDizTime>10) 
   {
    if(digitalRead(ButtonPin)==HIGH){
      Serial.println("прерывание по кнопке НАЖАТА");
      PressedTime=millis();
    }
    else   {
      Serial.print("прерывание по кнопке отпущена через ");
      Serial.println((millis()-PressedTime)/1000);
    }
   }
   AntiDizTime=millis();
}

//=====================================================================================================

void RassvetZakat()
{
    unsigned long StartTime=millis();
    unsigned long CurTime=0;
    while(CurTime<VoshodMilisec)
    {
      pwm=map(CurTime,0,VoshodMilisec,0,255);
      pwm=constrain(pwm,0,255);
      analogWrite(MosfetGatePin, pwm);
      Serial.println(pwm);
      delay(10);
      CurTime=millis()-StartTime;
    }
    analogWrite(MosfetGatePin, 255);
    delay(DaySec*1000);
    StartTime=millis();
    CurTime=0;
    while(CurTime<ZakatMilisec)
    {
      pwm=map(CurTime,0,VoshodMilisec,255,0);
      pwm=constrain(pwm,0,255);
      analogWrite(MosfetGatePin, pwm);
      Serial.println(pwm);
      delay(10);
      CurTime=millis()-StartTime;
    }
}

//=====================================================================================================

void loop() {
  buttonState = digitalRead(ButtonPin);
  if (buttonState == HIGH) {   
    digitalWrite(ledPin, HIGH); 
    Serial.println("_______________ON"); 
    Timer1DisEnable(0);
    Timer1DisEnable(1);    
  }
  else {
    
    digitalWrite(ledPin, LOW);
    digitalWrite(MosfetGatePin, LOW); 
    //Serial.println("OFF");
  }
}

//=====================================================================================================
