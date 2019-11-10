//#include <avr/interrupt.h>


							/*digipin 1  */
							/*digipin 2  */   
							/*digipin 3  */   
							/*digipin 4  */   
#define MosfetGatePin 5		/*digipin 5  */   
							/*digipin 6  */   
#define SettButtPin 7		/*digipin 7  */   
							/*digipin 8  */
							/*digipin 9  */   
							/*digipin 10 */   
							/*digipin 11 */  
							/*digipin 12 */  
#define ledPin 13			/*digipin 13 */   
							/*analogpin 0 */ 
							/*analogpin 1 */
							/*analogpin 2 */
							/*analogpin 3 */
							/*analogpin 4 */
							/*analogpin 5 */
							/*analogpin 6 */
							/*analogpin 7 */

enum Rejim {OFF, ON, UP, DOWN, SETUP_UP, SETUP_ON, SETUP_DOWN};

volatile bool SetButtState = false;
volatile Rejim Rej = OFF;
char buttonState = 0;
int pwm=0;
unsigned long VoshodMilisec=5000;
int DaySec=2;
unsigned long ZakatMilisec=5000;
volatile unsigned long AntiDizTime=0;
unsigned long PressedTime=0;



//=====================================================================================================
void Timer1DisEnable(byte ON1_OFF0, unsigned long PeriodMilis )
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
	Rej=OFF;
	pinMode(SettButtPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(MosfetGatePin, OUTPUT);
	Serial.begin(9600);
	attachInterrupt(0, int0, CHANGE);
	Serial.println("STAAAAAAAAAAAAAAAAAAAAART");
	
   //Timer1DisEnable(1,40);
}
//=====================================================================================================
void RaspRej()
{
switch (Rej)
	{
	case OFF: 
		Serial.println("			Rejim: OFF");
		break;	
	case ON: 
		Serial.println("			Rejim: ON");
		break;	
	case UP: 
		Serial.println("			Rejim: UP");
		break;	
	case DOWN: 
		Serial.println("			Rejim: DOWN");
		break;	
	case SETUP_UP: 
		Serial.println("			Rejim: SETUP_UP");
		break;	
	case SETUP_DOWN: 
		Serial.println("			Rejim: SETUP_DOWN");
		break;	
	case SETUP_ON: 
		Serial.println("			Rejim: SETUP_ON");
		break;	
	}

}
//=====================================================================================================

void int0(){
	//Serial.println("preryvanie po knopke NAJATA __________________");

	if(millis()-AntiDizTime>10) 
   {
    if(digitalRead(SettButtPin)==HIGH){
	    //Serial.println("хуй");
		Serial.println("preryvanie po knopke NAJATA 3333");
		PressedTime=millis();
    }
    else{
	    if ((millis() - PressedTime) > 3000){
		    Rej = SETUP_UP;
		    RaspRej();
		    Timer1DisEnable(1, 2000);
	    }
	    Serial.print("preryvanie po knopke otpuwena cherez ");
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
	
	bool SetButTempState = digitalRead(SettButtPin);
	if (SetButTempState != SetButtState && millis() - AntiDizTime > 100) {          //устнановки времени

		switch (Rej) 
		{
		case OFF: 
		case ON: 
		case UP: 
		case DOWN: 
			if (!SetButTempState && (millis() - AntiDizTime) / 1000 > 2)//если кнопка была нажата дольше 5 секунд
			{
				Rej = SETUP_UP;
				VoshodMilisec = 0;
				DaySec = 0;
				ZakatMilisec = 0;
			}
			break;	
		case SETUP_UP: 
			if (SetButTempState)
			{
				VoshodMilisec = millis();
			}
			else
			{
				Rej = SETUP_ON;
				VoshodMilisec = millis() - VoshodMilisec;
				Serial.print("Vremya voshoda: ");
				Serial.println(VoshodMilisec);
				//ZakatMilisec = millis();
			}
			break;	
		case SETUP_ON: 
			if (SetButTempState)
			{
				ZakatMilisec = millis();
			}
			else
			{
				Rej = SETUP_DOWN;
				DaySec = (millis() - ZakatMilisec) / 1000;
				Serial.print("Vremya ON: ");
				Serial.println(DaySec);
			}
			break;	
		case SETUP_DOWN: 
			if (SetButTempState)
			{
				ZakatMilisec = millis();
			}
			else
			{
				Rej = OFF;
				ZakatMilisec = millis() - ZakatMilisec;
				Serial.print("Vremya zakata: ");
				Serial.println(ZakatMilisec);
				Serial.println("============================");
			}
			break;	
		}
		RaspRej();
		SetButtState = SetButTempState;
//		Serial.print(SetButtState);
//		Serial.print("   ->  ");
//		Serial.println((millis()-AntiDizTime)/1000);
		AntiDizTime = millis();
	}// установки времени

	

}

//=====================================================================================================
