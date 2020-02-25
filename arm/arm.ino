# include <PulsePosition.h>
int val = 0;
int thr =1500;
int yaw=1500;
unsigned long tiempo_dlay1 = 0; // Para calculo delay 3 seg (Doesn't work)
unsigned long tiempo_dlay2 = 0; // Para calculo delay 0.15 seg
unsigned long tiempo_dlay3 = 0; // Para calculo delay 0.1 seg

const unsigned long delay_1 = 3000; //  periodo 3 seg (Doesn't work)
const unsigned long delay_2 = 150; //  periodo de 0.15 seg 
const unsigned long delay_3 = 50; //  periodo de 0.05 seg SEC FACIL (ademas descenso modo exigente)

String inputString = "";         // String para almacenar el texto enviado por Serial

PulsePositionOutput ppmOUT;

void setup() {
  // Iniciando comunicacion serial a 9600 baudrate
  Serial.begin(9600);
  inputString.reserve(200);
//  mimotor.attach(9);
  // initialize the LED pins:
  for (int thisPin = 2; thisPin < 7; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
  analogWriteResolution(11);
  analogWriteFrequency(A9, 200);
// ARM
thr=1500;
  ppmOUT.begin(22); 
  ppmOUT.write(1,1500);
  ppmOUT.write(2,1500);
  ppmOUT.write(3,thr);
  ppmOUT.write(4,yaw);
  ppmOUT.write(5,1500);
  ppmOUT.write(6,1500);
  ppmOUT.write(7,1500);
  ppmOUT.write(8,1500);
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
      thr =400;
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
      analogWrite(A9, map(thr, 0, 2047, 0, 2047));
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
   else if (inputString=="armado"){
//      dato =100;// deseado 1100us
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
//      enviar(dato);

  thr=1100;
  yaw=1900;
  ppmOUT.write(3,thr);
  enviar(thr);
  ppmOUT.write(4,yaw);
      delay(3000); //El delay con millis no esta sirviendo en este caso 
//      if (millis() - tiempo_dlay1  > delay_1)  {
//        tiempo_dlay1 = millis();
//      }
                    while(yaw > 1500) {
        delay(50);
//               if (millis() - tiempo_dlay3 > delay_3)  {
//          tiempo_dlay3 = millis();
          yaw -=8; // YAW
          ppmOUT.write(4,yaw);
     //  }
      }
      while((thr < 1500)) {
        delay(50);
//       if (millis() - tiempo_dlay3 > delay_3)  {
//          tiempo_dlay3 = millis();
          thr +=8; // THR
          enviar(thr);
          ppmOUT.write(3,thr);
       //}
      } 
      Serial.print("Armado 1\n");

      
   }   
   // Si el String es "hemo2" Se envia el comando de velocidad para analisis tipo 2     
   else if (inputString=="hemo2"){
      thr =1500; //1500us deseado
//      val = map(dato, 0, 1023, 0, 255);
//      mimotor.write(val);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
      enviar(thr);
      Serial.print("Iniciando analisis Tipo 2\n");
   }
   // Si el String es "hemo3" Se envia el comando de velocidad para analisis tipo 3
   else if (inputString=="hemo3"){
      thr = 1900; //1900us deseado max
      enviar(thr);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
      enviar(thr);
      Serial.print("Iniciando analisis Tipo 3\n");
      Serial.print(cal(thr));
   }

   else{

       Serial.print("error no se reconoce el comando ingresado \n");
   }
    

}
void loop(){
  // No se utiliza 
}
int cal (int thr) {
  int salida;
  return salida = (thr*2047)/4990;// Regla de 3 simple
}
void enviar(int thr) {
  int val = map(cal(thr), 0, 2047, 0, 2047);
//  int valc= val -2;
  analogWrite(A9, val);
}
