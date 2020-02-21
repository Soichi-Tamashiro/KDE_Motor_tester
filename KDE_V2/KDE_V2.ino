// Llama a la libreria servo para el control del motor brushles de dron
#include <Servo.h>
Servo mimotor;                   // Cambio de nombre para llamar mas facilmente a las funciones de la libreria servo
int dato = 0;
int val = 0;
String inputString = "";         // String para almacenar el texto enviado por Serial
// ------------------------------------------------------------------------------
//   Variables
// ------------------------------------------------------------------------------
int thr = 0; // Antes era dato, representa el throttle
int salida = 0; // la salida pwm mapeada
int paso = 0; // Pasos exclusivamente para secuencia de picos

#define ledVerde 11 //Indicador de prueba sencilla
#define ledAmarillo 12 //Indicador de prueba mediana
#define ledRojo 13 //Indicador de prueba achorada


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
#define SWITCH_PIN 18 // Boton para interrupciones 18
#define PARADA_PIN 19 // Boton parada de emergencia 19
#define MODO_PIN 20 // Boton Para seleccionar el modo 20

volatile int cont = 0; // Variable manipulada por la interrupción SWITCH_PIN - determina lo que hace el programa
volatile int modo = 0; // Variable manipulada por la interrupción MODO_PIN - determina el modo de la prueba
volatile int FirstT = 0; // Variable que indica primera vez que se pasa por el while MODO 1, 2, 3

int armado=0; //Para la secuencia de armado
int apagado=0;
int apagadosec=0;

// ------------------------------------------------------------------------------

void setup() {
  // Iniciando comunicacion serial a 9600 baudrate
  Serial.begin(9600);
  inputString.reserve(200);
//  mimotor.attach(9);
  // initialize the LED pins:
  for (int thisPin = 2; thisPin < 7; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
  pinMode(ledVerde, OUTPUT); // Prueba 1 Fácil
  pinMode(ledAmarillo, OUTPUT); // Prueba 2 Moderada
  pinMode(ledRojo, OUTPUT); // Prueba 3 Exigente
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  // pinMode(SWITCH_PIN, INPUT_PULLUP); // Pin de Interrupción paso
  // attachInterrupt(digitalPinToInterrupt(SWITCH_PIN),Cambio,FALLING);
  // pinMode(PARADA_PIN, INPUT_PULLUP); // Pin de Interrupción Parada de emergancia
  // attachInterrupt(digitalPinToInterrupt(PARADA_PIN),Parada,RISING);
  // pinMode(MODO_PIN, INPUT_PULLUP); // Pin de Interrupción Modo
  // attachInterrupt(digitalPinToInterrupt(MODO_PIN),Modo,FALLING);
  analogWriteFrequency(A9, 200);
}
void enviar(){
  salida = map(thr, 0, 2047, 0, 256);
  analogWrite(A9, salida);
}
/*
 * El evento serial ocurre cuando se detecta una trama a traves del bus serial
 */
void serialEvent()
{
  // Mientras se este enviando datos a traves del bus serial
  while (Serial.available() > 0){

        // Almacena la trama de datos hasta que se presione ENTER
        if(Serial.available() >0)
        {
          inputString = Serial.readStringUntil('\n');

        }



   }
   // accion
 // ------------------------------------------------------------------------------
 //   OFF
 // ------------------------------------------------------------------------------
   if (inputString=="off"){

      Serial.print("Sistema Apagado\n");
   }
   // ------------------------------------------------------------------------------
   //   Secuencia 1
   // ------------------------------------------------------------------------------
   else if (inputString=="sec1"){
     //Codigo para el modo de prueba 1
        digitalWrite(ledVerde, HIGH);
        digitalWrite(ledAmarillo, LOW);
        digitalWrite(ledRojo, LOW);
        if (FirstT == 0) {
         // RestoreVal();
         FirstT=1;
        }
        //////ARMADO
         while (armado==0) {
      if (armado==0) {
        thr = 1100; // THR
        enviar();
        delay(3000); //El delay con millis no esta sirviendo en este caso
        if (millis() - tiempo_dlay1  > delay_1)  {
          tiempo_dlay1 = millis();
                paso =0;
        }
        while((thr < 1500)) {
          //delay(100);
         if (millis() - tiempo_dlay2 > delay_2)  {
            tiempo_dlay2 = millis();
            thr +=5; // THR
            enviar();
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
            enviar();
         }
        if (thr>1700) {
          paso=1;
         }
  }
  if (paso==1) {

         if (millis() - tiempo_dlay2 > delay_2)  {
          thr -=5; // THR
          enviar();
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
      enviar();
  }
         while ((cont==4)&&(modo==1)) {
      thr = 1000; //Corte de motores
      enviar();
  }
         while ((cont>4)&&(modo==1)) {
   cont=0;
  }
  //

    Serial.print("Iniciando analisis Tipo 1\n");
   }
   // ------------------------------------------------------------------------------
   //   Secuencia 2
   // ------------------------------------------------------------------------------
   else if (inputString=="sec2"){
     //Codigo para el modo de prueba 2
     digitalWrite(ledVerde, LOW);
     digitalWrite(ledAmarillo, HIGH);
     digitalWrite(ledRojo, LOW);
     if (FirstT == 0) {
      // RestoreVal();
      FirstT=1;
     }
//////ARMADO
      while ((cont==1)&&(modo==2)) {
   if (armado==0) {
     thr = 1100; // THR
     enviar();
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
         enviar();
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
       enviar();
         tiempo_dlay2 = millis();
         }
     if (thr<1400) {
 delay(1000);
              if (millis() - tiempo_dlay4 > delay_4)  {
               if (modo==2) {
                 thr =1400;
                 enviar();
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
       enviar();
       }
         tiempo_dlay3 = millis();
         }
       paso=0;
}
}
//////Palanca regresa al centro
      while ((cont==3)&&(modo==2)) {
   thr = 1500;
   enviar();
}
      while ((cont==4)&&(modo==2)) {
   thr = 1000; //Corte de motores
   enviar();
}
      while ((cont>4)&&(modo==2)) {
  cont=0;
}

      Serial.print("Iniciando analisis Tipo 2\n");
   }
   // ------------------------------------------------------------------------------
   //   Secuencia 3
   // ------------------------------------------------------------------------------
   else if (inputString=="sec3"){
     //Codigo para el modo de prueba 3
     digitalWrite(ledVerde, LOW);
     digitalWrite(ledAmarillo, LOW);
     digitalWrite(ledRojo, HIGH);
     if (FirstT == 0) {
      // RestoreVal();
      FirstT=1;
     }
//////ARMADO
      while ((cont==1)&&(modo==3)) {
   if (armado==0) {
     thr = 1100; // THR
     enviar();
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
         enviar();
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
       enviar();
         tiempo_dlay2 = millis();
         }
     if (thr<1350) {
delay(1000);
              if (millis() - tiempo_dlay4 > delay_4)  {
               if (modo==3){
                thr =1350;
                enviar();
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
       enviar();
       }

         tiempo_dlay3 = millis();
         }
       paso=0;
}
}
//////Palanca regresa al centro
      while ((cont==3)&&(modo==3)) {
   thr = 1500;
   enviar();
}
      while ((cont==4)&&(modo==3)) {
   thr = 1000; //Corte de motores
   enviar();
}
      while ((cont>4)&&(modo==3)) {
       cont=0;
}
      Serial.print("Iniciando analisis Tipo 3\n");
   }
   else if (inputString=="sec4"){
     //Codigo para el modo de prueba helices
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAmarillo, HIGH);
      digitalWrite(ledRojo, HIGH);
      if (FirstT == 0) {
       // RestoreVal();
       FirstT=1;
      }
      //////ARMADO
       while ((cont==1)&&(modo==4)) {
    if (armado==0) {
      thr = 1100; // THR
      enviar();
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
          enviar();
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
          enviar();
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
         enviar();
       }
        paso=2;
}
if (paso==2) {
       if (millis() - tiempo_dlay2 > delay_2)  {
        thr -=4; // THR
        enviar();
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
         enviar();
       }
        paso=4;
}
if (paso==4) {

       if (millis() - tiempo_dlay2 > delay_2)  {
          tiempo_dlay2 = millis();
          thr +=4; // THR
          enviar();
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
          enviar();
        }
        if ((thr<1140)) {
          cont=4;
        }

}
 }
       while ((cont==4)&&(modo==4)) {
    thr = 1000; //Corte de motores
    enviar();
    apagadosec=2;
    cont=5;

}
       while ((cont==5)&&(modo==4)) {
    thr = 1000; //Corte de motores
    enviar();
}
       while ((cont>5)&&(modo==4)) {
 cont=0;
}
     Serial.print("Iniciando analisis Tipo 4\n");
   }
   else{

       Serial.print("error no se reconoce el comando ingresado \n");
   }


}
void loop(){
  // No se utiliza
}
