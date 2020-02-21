//Programa de prueba de motores para Teensy
int thr = 0; //  Representa el throttle valor en us
int salida = 0; // la salida pwm mapeada
int paso = 0; // Pasos exclusivamente para secuencia de picos

#define ledVerde 10 //Indicador de prueba sencilla
#define ledAmarillo 11 //Indicador de prueba mediana
#define ledRojo 12 //Indicador de prueba achorada 


//DELAYS WITH MILLIS
unsigned long tiempo_fp = 0; // Para calculo de tiempo filtrado interrupción del pulsador  
unsigned long tiempo_dlay1 = 0; // Para calculo delay 
unsigned long tiempo_dlay2 = 0; // Para calculo delay 
unsigned long tiempo_dlay3 = 0; // Para calculo delay
unsigned long tiempo_dlay4 = 0; // Para calculo delay 
unsigned long tiempo_dlay5 = 0; // Para calculo delay 


const unsigned long delay_fp = 250; //  periodo de filtrado pulsador
const unsigned long delay_1 = 3000; //  periodo 3 seg 
const unsigned long delay_2 = 50; //  periodo de 0.05 seg 
const unsigned long delay_3 = 500; //  periodo de 0.5 seg
const unsigned long delay_4 = 1000; //  periodo de 1 seg
const unsigned long delay_5 = 7000; //  periodo de 7 seg 

// VARIABLES PARA INTERRUPCIONES
#define MODO_PIN 17 // Boton Para seleccionar el modo 17
#define SWITCH_PIN 18 // Boton para iniciar sec 18
#define PARADA_PIN 19 // Boton parada de emergencia 19

volatile int cont = 0; // Variable manipulada por la interrupción SWITCH_PIN - determina lo que hace el programa 
volatile int modo = 0; // Variable manipulada por la interrupción MODO_PIN - determina el modo de la prueba 
volatile int FirstT = 0; // Variable que indica primera vez que se pasa por el while MODO 1, 2, 3

int armado=0; //Para la secuencia de armado  
int apagado=0;  
int apagadosec=0;   

void setup() {
  pinMode(ledVerde, OUTPUT); // Prueba 1 Fácil
  pinMode(ledAmarillo, OUTPUT); // Prueba 2 Moderada
  pinMode(ledRojo, OUTPUT); // Prueba 3 Exigente 
  digitalWrite(ledVerde, LOW); 
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Pin de Interrupción paso 
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN),Cambio,FALLING); 
  pinMode(PARADA_PIN, INPUT_PULLUP); // Pin de Interrupción Parada de emergancia 
  attachInterrupt(digitalPinToInterrupt(PARADA_PIN),Parada,RISING); 
  pinMode(MODO_PIN, INPUT_PULLUP); // Pin de Interrupción Modo
  attachInterrupt(digitalPinToInterrupt(MODO_PIN),Modo,FALLING);
  analogWriteFrequency(A9, 200);
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
      thr = 1100; // THR
      enviar(thr);
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
      while((thr < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=5; // THR
          enviar(thr);
       }
      } 
      armado =1;
        }
}
    while ((cont==2)&&(modo==1)) {  
//////MOVIMIENTO PROGRESIVO LENTO DE LAS PALANCAS EN AUMENTO Y DESCENSO 
if (paso==0) {
      
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=5; // THR
          enviar(thr);
       }
      if (thr>1700) {
        paso=1;
       }
}
if (paso==1) {
      
       if (millis() - tiempo_dlay2 > delay_2)  {
        thr -=5; // THR
        enviar(thr);
          tiempo_dlay2 = millis(); 
          }
      if (thr<1200) {
        paso=0;
      }
}
}
//
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==1)) {
    thr = 1500; 
    enviar(thr);
}
       while ((cont==4)&&(modo==1)) {
    thr = 1000; //Corte de motores
    enviar(thr);
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
      thr = 1100; // THR
      enviar(thr);
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
      while((thr < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=5; // THR
          enviar(thr);
       }
      } 
      armado =1;
        }
}
 while ((cont==2)&&(modo==2)) {
//////picos desde hover hasta tope 1900
if (paso==0) {
       if (millis() - tiempo_dlay2 > delay_2)  {
        thr -=5; // THR
        enviar(thr);
          tiempo_dlay2 = millis(); 
          }
      if (thr<1400) {
  delay(1000);
               if (millis() - tiempo_dlay4 > delay_4)  {
                if (modo==2) {
                  thr =1400;
                  enviar(thr); 
                }
          tiempo_dlay4 = millis();
       }
        paso=1;
      }
}
if (paso==1) {
       if (millis() - tiempo_dlay3 > delay_3)  {
        if (modo==2) {
        thr =1700; // THR en realidad 1900 pero se pone 1800 para probar
        enviar(thr);
        }
          tiempo_dlay3 = millis(); 
          }
        paso=0;
}
}
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==2)) {
    thr = 1500;
    enviar(thr); 
}
       while ((cont==4)&&(modo==2)) {
    thr = 1000; //Corte de motores
    enviar(thr);
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
      thr = 1100; // THR
      enviar(thr);
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
      while((thr < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=5; // THR
          enviar(thr);
       }
      } 
      armado =1;
        }
}
 while ((cont==2)&&(modo==3)) {
//////picos desde el minimo 1200 hasta 1900
if (paso==0) {
       if (millis() - tiempo_dlay2 > delay_2)  {
        thr -=5; // THR
        enviar(thr);
          tiempo_dlay2 = millis(); 
          }
      if (thr<1350) {
 delay(1000);
               if (millis() - tiempo_dlay4 > delay_4)  {
                if (modo==3){
                 thr =1350;
                 enviar(thr); 
                }
          tiempo_dlay4 = millis();
       }
        paso=1;
      }
      }
if (paso==1) {
       if (millis() - tiempo_dlay3 > delay_3)  {
        if (modo==3) {
        thr =1850; // THR
        enviar(thr);         
        }

          tiempo_dlay3 = millis(); 
          }
        paso=0;
}
}
 //////Palanca regresa al centro
       while ((cont==3)&&(modo==3)) {
    thr = 1500; 
    enviar(thr);
}
       while ((cont==4)&&(modo==3)) {
    thr = 1000; //Corte de motores
    enviar(thr);
}
       while ((cont>4)&&(modo==3)) {
        cont=0;
}
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
      thr = 1100; // THR
      enviar(thr);
      delay(3000); //El delay con millis no esta sirviendo en este caso 
      if (millis() - tiempo_dlay1  > delay_1)  {
        tiempo_dlay1 = millis();
              paso =0; 
      }
      while((thr < 1500)&&(cont==1)) {
        //delay(100);
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=5; // THR
          enviar(thr);
       }
      } 
      armado =1;
        }
}
    while ((cont==2)&&(modo==4)) {  
//////MOVIMIENTO PROGRESIVO LENTO DE LAS PALANCAS EN AUMENTO Y DESCENSO 
if (paso==0) {
      
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=4; // THR
          enviar(thr);
       }
      if (thr>1600) {
        paso=1;
       }
}
if (paso==1){
  delay(7000);
               if (millis() - tiempo_dlay5 > delay_5)  {
          tiempo_dlay5 = millis();
         thr =1600;
         enviar(thr); 
       }
        paso=2;
}
if (paso==2) {
       if (millis() - tiempo_dlay2 > delay_2)  {
        thr -=4; // THR
        enviar(thr);
          tiempo_dlay2 = millis(); 
          }
      if (thr<1200) {
        paso=3;
      }
}
if (paso==3){
  delay(7000);
               if (millis() - tiempo_dlay5 > delay_5)  {
          tiempo_dlay5 = millis();
         thr =1200; 
         enviar(thr);
       }
        paso=4;
}
if (paso==4) {
      
       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=4; // THR
          enviar(thr);
       }
      if (thr>1500) {
        apagado++; //cada aumento de apagado representa alrededor de 25 segundos

        if (apagado==69) { // Representa al rededor de 29 minutos de prueba 
          cont=3;
        }
        paso=0;
       }
}
    }
//
 while ((cont==3)&&(modo==4)) {
// Baja hasta el minimo THR
while((thr > 1130)&&(cont==3)) {
        //delay(100);
         if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr -=4;
          enviar(thr); 
        }
        if ((thr<1140)) {
          cont=4;
        }

}
 }
       while ((cont==4)&&(modo==4)) {
    thr = 1000; //Corte de motores
    enviar(thr);
    apagadosec=2;
    cont=5;
    
}
       while ((cont==5)&&(modo==4)) {
    thr = 1000; //Corte de motores
    enviar(thr);
}
       while ((cont>5)&&(modo==4)) {    
 cont=0;
}
}
while (modo>4) { ///// Se reinicia el ciclo
  modo=0;
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
thr = 700; //Corte de motores
enviar(thr);
modo=0;
cont=0;
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
      apagado=0;
      apagadosec=0; 
      thr = 500;
      enviar(thr);
  }
int cal (int thr) {
  int rtres;
  return rtres = (thr*2047)/4990;// Regla de 3 simple
}
void enviar(int thr) {
  salida = map(cal(thr), 0, 2047, 0, 255);
  analogWrite(A9, salida); //Salida Motor 1
  analogWrite(A8, salida); //Salida Motor 2
  analogWrite(A7, salida); //Salida Motor 3
  analogWrite(A6, salida); //Salida Motor 4
}
