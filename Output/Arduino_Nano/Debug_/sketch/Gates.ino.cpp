#include <Arduino.h>
#line 1 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
#line 1 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
#include <C:\Users\Basay\Documents\ArduinoData\packages\arduino\hardware\avr\1.8.1\libraries\EEPROM\src\EEPROM.h >



// всё по ESP >>


//#include <ELClient.h>
//#include <ELClientWebServer.h>
//#include "Pages.h"


// << всё по ESP

//info : Sketch uses 6504 bytes(21 %) of program storage space.Maximum is 30720 bytes.
//info : Global variables use 479 bytes(23 %) of dynamic memory, leaving 1569 bytes for local variables.Maximum is 2048 bytes.

// нажать кнопку настройки и держать 10 сек
// отпустить. Если начался периодический рассвет - значит, зашли в режим настройки
// для установки времени рассвета - нажать и держать столько времени, сколько нужен рассвет
// после отпускания - загорится постоянно - это режим макс свечения
// нажать и держать столько, сколько нужно
// после отпускания - начнётся периодический закат
// нажать и держать закат
// потом всё потухнет - значит, сохранилось.
// нажать сброс и проверить.
// если Led13 мигает - значит, ошибка чтения из еепром при старте арды. Должно быть: (0 < ВРЕМЯ < 5_минут) 



/*
 *
 *
 *http://192.168.1.5/rgb?param=_FFFF00
 *
 *
// Create aREST instance
aREST rest = aREST();
// NeoPixel Init
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS,
	PIN, 
	NEO_GRB + NEO_KHZ800);
void setup() {
	Serial.begin(115200);

	// Register RGB function
	rest.function(&quot ; rgb&quot ; , setPixelColor);
	Serial.println(&quot ; Try DHCP...&quot ;);
	if (Ethernet.begin(macAdd) == 0) {
		Serial.println(&quot ; DHCP FAIL...Static IP&quot ;);
		Ethernet.begin(macAdd, ip, myDns, myGateway);
	}

	server.begin();
	Serial.print(&quot ; server IP : &quot ;);
	Serial.println(Ethernet.localIP());

	pixels.begin();
	Serial.println(&quot ; Setup complete.\n&quot ;);
}

void loop() {
	// listen for incoming clients
	EthernetClient client = server.available();
	rest.handle(client);
	wdt_reset();
}

int setPixelColor(String hexColor) {
	hexColor = &quot; 0x&quot; + hexColor;
	Serial.println(&quot ; Hex color &quot ; + hexColor);
	long n = strtol(&amp ; hexColor[0], NULL, 16);
	Serial.println(&quot ; N : &quot ; + String(n));
	long r = n &gt; &gt; 16;
	long g = n &gt; &gt; 8 &amp; 0xFF;
	long b = n &amp; 0xFF;
  
	// set single pixel color
	return 1;
}*/








							/*digipin 1  */
#define OpenSignalPin 2		/*digipin 2  */   
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

enum Rejim {OFF, UP, ON, DOWN, SETUP_UP, SETUP_ON, SETUP_DOWN};

volatile bool SetButtState = false;
unsigned int CurPwmInSett = 0;
volatile Rejim Rej = OFF;
//char buttonState = 0;
int pwm=0;
struct Times
{
	unsigned long VoshodMilisec;
	int DaySec;
	unsigned long ZakatMilisec;
} Periods;
bool EEPROMreadEroor = false;
byte EELed = 0; // для непостоянного горения, а мигания диода ошибки
volatile unsigned long AntiDizTime=0;
//unsigned long PressedTime=0;
byte FG[9]; // FromGod
byte FGC;

//=====================================================================================================
#line 132 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
void Timer1DisEnable(byte ON1_OFF0, unsigned long PeriodMilis );
#line 169 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
void setup();
#line 199 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
void RaspRej();
#line 228 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
void int0();
#line 242 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
void loop();
#line 132 "F:\\Basay\\Dropbox\\BSK\\Gates\\Arda\\GatesInStudio\\sketches\\Gates.ino"
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
	//if(Rej!=OFF)
		analogWrite(MosfetGatePin, pwm);
	//Serial.println(digitalRead(MosfetGatePin)) ;
	if (EEPROMreadEroor)
	{
		if (EELed > 50) { 
			digitalWrite(ledPin, !digitalRead(ledPin)); 
			EELed = 0; 
		}
		else
			EELed++;
	}
	//int razn=millis()-AntiDizTime;
    //AntiDizTime=millis();
    //Serial.print("_______________INTERRUPT   ");
    //Serial.println(razn);
    
    //digitalWrite(ledPin, !digitalRead(ledPin));
    //Periods.VoshodMilisec
}
//=====================================================================================================

void setup() {
	// put your setup code here, to run once:
	FGC = 0;
	Rej=OFF;
	pinMode(SettButtPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(MosfetGatePin, OUTPUT);
	Serial.begin(115200);
	attachInterrupt(0, int0, RISING);
	Periods.VoshodMilisec = 5000;
	Periods.DaySec = 2;
	Periods.ZakatMilisec = 10000;
	
	Times EepT;
	EEPROM.get(10, EepT);
	Serial.print("EEPROM.Rassvet: "); Serial.println(EepT.VoshodMilisec);
	Serial.print("EEPROM.Day: "); Serial.println(EepT.DaySec);
	Serial.print("EEPROM.Zakat: "); Serial.println(EepT.ZakatMilisec);
	
	if (EepT.VoshodMilisec > 0 && EepT.VoshodMilisec < 300000) Periods.VoshodMilisec = EepT.VoshodMilisec; else EEPROMreadEroor = true;
	if (EepT.DaySec > 0 && EepT.DaySec < 300) Periods.DaySec = EepT.DaySec;	else EEPROMreadEroor = true; // 300000 = 5 минут
	if (EepT.ZakatMilisec > 0 && EepT.ZakatMilisec < 300000) Periods.ZakatMilisec = EepT.ZakatMilisec; else EEPROMreadEroor = true;
	
	Serial.println("STAAAAAAAAAAAAAAAAAAAAART");
	Serial.print("Rassvet: ");	Serial.println(Periods.VoshodMilisec);
	Serial.print("Day: ");	Serial.println(Periods.DaySec);
	Serial.print("Zakat: ");	Serial.println(Periods.ZakatMilisec);
    Timer1DisEnable(1,10);
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
	if(millis()-AntiDizTime>3000) // три секунды - чтоб только одно включение апа было
   {
	   Rej = UP;
	   Serial.println("preryvanie po signalu vorot");
	   RaspRej();
	   AntiDizTime = millis();
   }
}



//=====================================================================================================

void loop() {
	int incomingByte;
	if (Serial.available() > 0)
	{
		//если есть доступные данные
		// считываем байт
		incomingByte = Serial.read();
		// отсылаем то, что получили
		FG[FGC] = incomingByte;
		FGC++;
		Serial.println(incomingByte);
	}
	else
	{
		FGC = 0;
		FG[0] = 0;
		FG[1] = 0;
		FG[2] = 0;
		FG[3] = 0;
		FG[4] = 0;
		FG[5] = 0;
		FG[6] = 0;
		FG[7] = 0;
		FG[8] = 0;
	}

	if (FGC == 9) // HUI345FCK //OPN либо 000...255 // H=72 _ U=85 _ I=73 _  F=70 C=67 K=75
	{
		Serial.print(FG[0]); Serial.print(" "); Serial.print(FG[1]); Serial.print(" "); Serial.print(FG[2]); Serial.print(" ");
		Serial.print(FG[3]); Serial.print(" "); Serial.print(FG[4]); Serial.print(" "); Serial.print(FG[5]); Serial.print(" ");
		Serial.print(FG[6]); Serial.print(" "); Serial.print(FG[7]); Serial.print(" "); Serial.print(FG[8]); Serial.print(" ");
		Serial.println("FGC=8");
		
       // 72 85 73     79 80 78   70 67 75
		
		if (FG[0] == 72 && FG[1] ==85 && FG[2] ==73 && FG[6] ==70 &&FG[7] ==67 &&FG[8] == 75)
		{
			Serial.println("hui___fck");
			// O=79 P=80 N=78 0=48 _ 9=57
			if(FG[3] == 79 && FG[4] == 80 && FG[5] == 78)
			{
				Rej = UP;
				Serial.println("Prishla komanda vskrytiya");
				RaspRej();
				
			}
			
		}
			
		FGC = 0;
	}
	
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
				CurPwmInSett = 0;
				Periods.VoshodMilisec = 0;
				Periods.DaySec = 0;
				Periods.ZakatMilisec = 0;
			}
			break;	
		case SETUP_UP: 
			if (SetButTempState)
			{
				Periods.VoshodMilisec = millis();
			}
			else
			{
				Rej = SETUP_ON;
				Periods.VoshodMilisec = millis() - Periods.VoshodMilisec;
				Serial.print("Vremya voshoda: ");
				Serial.println(Periods.VoshodMilisec);
				//Periods.ZakatMilisec = millis();
			}
			break;	
		case SETUP_ON: 
			if (SetButTempState)
			{
				Periods.ZakatMilisec = millis();
			}
			else
			{
				Rej = SETUP_DOWN;
				CurPwmInSett = 65530;
				Periods.DaySec = (millis() - Periods.ZakatMilisec) / 1000;
				Serial.print("Vremya ON: ");
				Serial.println(Periods.DaySec);
			}
			break;	
		case SETUP_DOWN: 
			if (SetButTempState)
			{
				Periods.ZakatMilisec = millis();
			}
			else
			{
				Rej = OFF;
				Periods.ZakatMilisec = millis() - Periods.ZakatMilisec;
				Serial.print("Vremya zakata: ");
				Serial.println(Periods.ZakatMilisec);
				EEPROM.put(10, Periods);
				Serial.println(" =========SAVED===================");
				
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

	switch (Rej)
	{
	case OFF: 
		pwm = 0;
		//analogWrite(MosfetGatePin, 0);
		break;	
	case UP: 
		if (millis() - AntiDizTime < Periods.VoshodMilisec ) {
			pwm = map(millis() - AntiDizTime, 0, Periods.VoshodMilisec, 0, 255);
			pwm = constrain(pwm, 0, 255);
		}	
		else{
			Rej = ON;
			AntiDizTime = millis();
			RaspRej();
		}
		break;	
	case ON: 
		if ((millis() - AntiDizTime)/1000 < Periods.DaySec) {
			pwm = 255;
		}	
		else {
			Rej = DOWN;
			AntiDizTime = millis();
			RaspRej();
		}
		break;	
	case DOWN: 
		if (millis() - AntiDizTime < Periods.ZakatMilisec) {
			pwm = map(millis() - AntiDizTime, 0, Periods.ZakatMilisec, 255, 0);
			pwm = constrain(pwm, 0, 255);
		}	
		else {
			Rej = OFF;
			RaspRej();
		}
		break;	
	case SETUP_UP: 
		if (CurPwmInSett < 65530)
		{
			pwm = map(CurPwmInSett, 0, 65530, 0, 255);
			pwm = constrain(pwm, 0, 255);
			CurPwmInSett++;
		}
		else CurPwmInSett = 0;
		break;	
	case SETUP_ON: 
		pwm = 255;
		break;	
	case SETUP_DOWN: 
		if (CurPwmInSett >0 )
		{
			pwm = map(CurPwmInSett, 0, 65530, 0, 255);
			pwm = constrain(pwm, 0, 255);
			CurPwmInSett--;
		}
		else CurPwmInSett = 65530;

		break;	
	}

}


//=====================================================================================================

