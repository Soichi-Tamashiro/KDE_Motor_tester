// Llama a la libreria servo para el control del motor brushles de dron
#include <Servo.h> 
Servo mimotor;                   // Cambio de nombre para llamar mas facilmente a las funciones de la libreria servo
int dato = 0;
int val = 0;
String inputString = "";         // String para almacenar el texto enviado por Serial

void setup() {
  // Iniciando comunicacion serial a 9600 baudrate
  Serial.begin(9600);
  inputString.reserve(200);
//  mimotor.attach(9);
  // initialize the LED pins:
  for (int thisPin = 2; thisPin < 7; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
  analogWriteFrequency(A9, 200);
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
   // Si el String es "apagado" Se envia el comando de apagado al motor
   if (inputString=="off"){
      dato =400;
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
      analogWrite(A9, map(dato, 0, 2000, 0, 255));
      /* Serial.print("Secuencia de Apagado Iniciada");
      delay(1000);
      Serial.print(".");
      delay(1000);
      Serial.print(".");
      delay(1000);
      Serial.print(".");
      Serial.print("\n");
      delay(1000); */
      Serial.print("Sistema Apagado\n");
   }
   // Si el String es "hemo1" Se envia el comando de velocidad para analisis tipo 1
   else if (inputString=="hemo1"){
      dato =700;
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
      analogWrite(A9, map(dato, 0, 2010, 0, 255));
      Serial.print("Iniciando analisis Tipo 1\n");
   }   
   // Si el String es "hemo2" Se envia el comando de velocidad para analisis tipo 2     
   else if (inputString=="hemo2"){
      dato =1200; //1220
//      val = map(dato, 0, 1023, 0, 255);
//      mimotor.write(val);
      analogWrite(A9, map(dato, 0, 2020, 0, 255));
      Serial.print("Iniciando analisis Tipo 2\n");
   }
   // Si el String es "hemo3" Se envia el comando de velocidad para analisis tipo 3
   else if (inputString=="hemo3"){
      dato = 1900; //1930
//      val = map(dato, 0, 1023, 0, 255);
//      mimotor.write(val);
      analogWrite(A9, map(dato, 0, 2030, 0, 255));
      Serial.print("Iniciando analisis Tipo 3\n");
   }

   else{

       Serial.print("error no se reconoce el comando ingresado \n");
   }
    

}
void loop(){
  // No se utiliza 
}
