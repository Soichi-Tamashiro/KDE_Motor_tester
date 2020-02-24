
/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 * EDICCIONES PARA KDE MOTOR TEST
 * VERSION 1.8: Arreglos de la version anterior 
 *  * (Esta adaptacion tiene 3 pulsadores de interupciones con debounce) 
 *  tres modalidades de prueba: Sencilla, Moderada y Exigente con los siguientes pasos...
   SECUENCIA DE ARMADO - SECUENCIA DE PICOS - SECUENCIA DE DESARMADO 
   ////////////////////////////////////////////////////////PRUEBA MOTORES Y HELICES////////////////////////////////////////////////////////////////////////////////////////
 */

//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 8  //set the number of channels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500 //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino
int paso = 0; // Pasos exclusivamente para secuencia de picos

// 3 LEDS indicadores para los 3 tipos de prueba 
#define ledVerde 11 //Indicador de prueba sencilla
#define ledAmarillo 12 //Indicador de prueba mediana
#define ledRojo 13 //Indicador de prueba achorada 

//DELAYS WITH MILLIS
unsigned long tiempo_fp = 0; // Para calculo de tiempo filtrado interrupción del pulsador  
unsigned long tiempo_dlay1 = 0; // Para calculo delay 3 seg (Doesn't work)
unsigned long tiempo_dlay2 = 0; // Para calculo delay 0.15 seg
unsigned long tiempo_dlay3 = 0; // Para calculo delay 0.1 seg
unsigned long tiempo_dlay4 = 0; // Para calculo delay 0.05 seg
unsigned long tiempo_dlay5 = 0; // Para calculo delay 0.02 seg
unsigned long tiempo_dlay6 = 0; // Para calculo delay 0.02 seg
unsigned long tiempo_dlay7 = 0; // Para calculo delay 1 seg
unsigned long tiempo_dlay8 = 0; // Para calculo delay 1 seg

const unsigned long delay_fp = 250; //  periodo de filtrado pulsador
const unsigned long delay_1 = 3000; //  periodo 3 seg (Doesn't work)
const unsigned long delay_2 = 150; //  periodo de 0.15 seg 
const unsigned long delay_3 = 50; //  periodo de 0.15 seg SEC FACIL (ademas descenso modo exigente)
const unsigned long delay_4 = 40; //  periodo de 0.1 seg SEC MODERADA 
const unsigned long delay_5 = 25; //  periodo de 0.07 seg SEC EXIGENTE (no se esta usando en esta v)
const unsigned long delay_6 = 500; //  periodo de 0.5 (periodo en el que se mantiene pico modo EXIGENTE)
const unsigned long delay_7 = 1000; //  periodo de 1 SEG
const unsigned long delay_8 = 7000; //  periodo de 1 SEG

// VARIABLES PARA INTERRUPCIONES
#define SWITCH_PIN 18 // Boton para interrupciones 18
#define PARADA_PIN 19 // Boton parada de emergencia 19
#define MODO_PIN 20 // Boton Para seleccionar el modo 20
 
volatile int cont = 0; // Variable manipulada por la interrupción SWITCH_PIN - determina lo que hace el programa 
volatile int modo = 0; // Variable manipulada por la interrupción MODO_PIN - determina el modo de la prueba 
volatile int FirstT = 0; // Variable que indica primera vez que se pasa por el while MODO 1, 2, 3
int armado=0; //Para la secuencia de armado

/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];
void setup(){
  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
      ppm[i]= CHANNEL_DEFAULT_VALUE;
  }
  pinMode(ledVerde, OUTPUT); // Prueba 1 Fácil
  pinMode(ledAmarillo, OUTPUT); // Prueba 2 Moderada
  pinMode(ledRojo, OUTPUT); // Prueba 3 Exigente 
  digitalWrite(ledVerde, LOW); 
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Pin de Interrupción paso 
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN),Cambio,FALLING); 
  pinMode(PARADA_PIN, INPUT_PULLUP); // Pin de Interrupción Parada de emergancia 
  attachInterrupt(digitalPinToInterrupt(PARADA_PIN),Parada,RISING); 
  pinMode(MODO_PIN, INPUT_PULLUP); // Pin de Interrupción Modo
  attachInterrupt(digitalPinToInterrupt(MODO_PIN),Modo,FALLING); 
}
void loop(){
  while (modo ==0) { 
//Modo Inicial
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAmarillo, LOW);
      digitalWrite(ledRojo, LOW);
      if (FirstT == 0) {
       RestoreVal();
       FirstT=1; 
      }
}
while (modo ==1) { ///////////////////////////////////////////////////MODO DE PRUEBA SENCILLO ///////////////////////////////////////////////
//Codigo para el modo de prueba 1
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAmarillo, LOW);
      digitalWrite(ledRojo, LOW);
      if (FirstT == 0) {
       RestoreVal();
       FirstT=1; 
      }
      //////ARMADO
       while ((cont==1)&&(modo==1)) {
    if (armado==0) {
      ppm[2] = 1100; // THR
      ppm[3] = 1900; //YAW
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
                    while((ppm[3] > 1500)&&(cont==1)) {
        //delay(100);
               if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[3] -=8; // THR
       }
      }
      while((ppm[2] < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=8; // THR
       }
      } 
      armado =1;
        }
}
    while ((cont==2)&&(modo==1)) {  
//////MOVIMIENTO PROGRESIVO LENTO DE LAS PALANCAS EN AUMENTO Y DESCENSO 
if (paso==0) {
      
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=5; // THR
       }
      if (ppm[2]>1700) {
        paso=1;
       }
}
if (paso==1) {
      
       if (millis() - tiempo_dlay3 > delay_3)  {
        ppm[2] -=5; // THR
          tiempo_dlay3 = millis(); 
          }
      if (ppm[2]<1200) {
        paso=0;
      }
}
}
//
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==1)) {
    ppm[2] = 1500; 
}
       while ((cont==4)&&(modo==1)) {
    ppm[7] = 1900; //Corte de motores
}
       while ((cont>4)&&(modo==1)) {    
 cont=0;
}
//
}
while (modo ==2) { ///////////////////////////////////////////////////MODO DE PRUEBA MODERADO ///////////////////////////////////////////////
//Codigo para el modo de prueba 2
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAmarillo, HIGH);
      digitalWrite(ledRojo, LOW);
      if (FirstT == 0) {
       RestoreVal();
       FirstT=1; 
      }
//////ARMADO
       while ((cont==1)&&(modo==2)) {
    if (armado==0) {
      ppm[2] = 1100; // THR
      ppm[3] = 1900; //YAW
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
                    while((ppm[3] > 1500)&&(cont==1)) {
        //delay(100);
               if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[3] -=8; // THR
       }
      }
      while((ppm[2] < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=8; // THR
       }
      } 
      armado =1;
        }
}
 while ((cont==2)&&(modo==2)) {
//////picos desde hover hasta tope 1900
if (paso==0) {
       if (millis() - tiempo_dlay3 > delay_3)  {
        ppm[2] -=5; // THR
          tiempo_dlay3 = millis(); 
          }
      if (ppm[2]<1400) {
  delay(1000);
               if (millis() - tiempo_dlay7 > delay_7)  {
          tiempo_dlay7 = millis();
         ppm[2] =1400; 
       }
        paso=1;
      }
}
if (paso==1) {
       if (millis() - tiempo_dlay6 > delay_6)  {
        ppm[2] =1900; // THR
          tiempo_dlay6 = millis(); 
          }
        paso=0;
}
}
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==2)) {
    ppm[2] = 1500; 
}
       while ((cont==4)&&(modo==2)) {
    ppm[7] = 1900; //Corte de motores
}
       while ((cont>4)&&(modo==2)) {
   cont=0;
}
}
while (modo ==3) {///////////////////////////////////////////////////MODO DE PRUEBA EXIGENTE ///////////////////////////////////////////////
//Codigo para el modo de prueba 3
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAmarillo, LOW);
      digitalWrite(ledRojo, HIGH);
      if (FirstT == 0) {
       RestoreVal();
       FirstT=1; 
      }
//////ARMADO
       while ((cont==1)&&(modo==3)) {
    if (armado==0) {
      ppm[2] = 1100; // THR
      ppm[3] = 1900; //YAW
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
                    while((ppm[3] > 1500)&&(cont==1)) {
        //delay(100);
               if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[3] -=8; // THR
       }
      }
      while((ppm[2] < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=8; // THR
       }
      } 
      armado =1;
        }
}
 while ((cont==2)&&(modo==3)) {
//////picos desde el minimo 1200 hasta 1900
if (paso==0) {
       if (millis() - tiempo_dlay3 > delay_3)  {
        ppm[2] -=5; // THR
        ppm[3] =1500; // YAW
          tiempo_dlay3 = millis(); 
          }
      if (ppm[2]<1200) {
 delay(1000);
               if (millis() - tiempo_dlay7 > delay_7)  {
          tiempo_dlay7 = millis();
         ppm[2] =1200; 
       }
        paso=1;
      }
      }
if (paso==1) {
       if (millis() - tiempo_dlay6 > delay_6)  {
        ppm[2] =1900; // THR
          tiempo_dlay6 = millis(); 
          }
        paso=0;
}
}
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==3)) {
    ppm[2] = 1500; 
}
       while ((cont==4)&&(modo==3)) {
    ppm[7] = 1900; //Corte de motores
}
       while ((cont>4)&&(modo==3)) {
        cont=0;
}
}
while (modo>4) { ///// Se reinicia el ciclo
  modo=0;
}
while (modo ==4) { ///////////////////////////////////////////////////MODO DE PRUEBA HELICES///////////////////////////////////////////////
//Codigo para el modo de prueba helices
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAmarillo, HIGH);
      digitalWrite(ledRojo, HIGH);
      if (FirstT == 0) {
       RestoreVal();
       FirstT=1; 
      }
      //////ARMADO
       while ((cont==1)&&(modo==4)) {
    if (armado==0) {
      ppm[2] = 1100; // THR
      ppm[3] = 1900; //YAW
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
                    while((ppm[3] > 1500)&&(cont==1)) {
        //delay(100);
               if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[3] -=8; // THR
       }
      }
      while((ppm[2] < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=8; // THR
       }
      } 
      armado =1;
        }
}
    while ((cont==2)&&(modo==4)) {  
//////MOVIMIENTO PROGRESIVO LENTO DE LAS PALANCAS EN AUMENTO Y DESCENSO 
if (paso==0) {
      
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=5; // THR
       }
      if (ppm[2]>1600) {
        paso=1;
       }
}
if (paso==1){
  delay(7000);
               if (millis() - tiempo_dlay8 > delay_8)  {
          tiempo_dlay6 = millis();
         ppm[2] =1600; 
       }
        paso=2;
}
if (paso==2) {
       if (millis() - tiempo_dlay3 > delay_3)  {
        ppm[2] -=5; // THR
          tiempo_dlay3 = millis(); 
          }
      if (ppm[2]<1200) {
        paso=3;
      }
}
if (paso==3){
  delay(7000);
               if (millis() - tiempo_dlay8 > delay_8)  {
          tiempo_dlay6 = millis();
         ppm[2] =1200; 
       }
        paso=4;
}
if (paso==4) {
      
       if (millis() - tiempo_dlay3 > delay_3)  {
          tiempo_dlay3 = millis();
          ppm[2] +=5; // THR
       }
      if (ppm[2]>1500) {
        paso=0;
       }
}
    }
//
 while ((cont==3)&&(modo==4)) {
// Baja hasta el minimo THR
while((ppm[2] > 1130)&&(cont==3)) {
        //delay(100);
         if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          ppm[2] -=25; 
        }
}
 }
       while ((cont==4)&&(modo==4)) {
    ppm[7] = 1900; //Corte de motores
}
       while ((cont==5)&&(modo==4)) {
    ppm[7] = 1900; //Corte de motores
}
       while ((cont>5)&&(modo==4)) {    
 cont=0;
}
//
}
} 
ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
 void Cambio() {      //cambio de estado 
      if (millis() - tiempo_fp > delay_fp)  {
         cont++;
    tiempo_fp = millis(); 
      }
  }

  void Parada() { //Parada de emergencia
     if (millis() - tiempo_fp > delay_fp)  {
ppm[7] = 1900; //Corte de motores
modo=0;
//ppm[2] = 1100;
//ppm[3] = 1100;
    tiempo_fp = millis(); 
      }
  }
  void Modo() { //Modo 
     if (millis() - tiempo_fp > delay_fp)  {
modo++;
FirstT=0;
    tiempo_fp = millis(); 
      }
  }
  void RestoreVal() {
    //Codigo al inicio de cada modo 
      paso=0;
      armado=0;
      cont=0;
      ppm[7] = 1500; //Corte de motores
      ppm[2] = 1500;
      ppm[3] = 1500;
  }
