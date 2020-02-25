// Llama a la libreria servo para el control del motor brushles de dron
//Servo mimotor;                   // Cambio de nombre para llamar mas facilmente a las funciones de la libreria servo
int dato = 0;
int val = 0;
String inputString = "";         // String para almacenar el texto enviado por Serial

void setup() {
  // Iniciando comunicacion serial a 9600 baudrate
  Serial.begin(9600);
  inputString.reserve(200);
  analogWriteResolution(11);
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
      dato =1100;
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
      analogWrite(A9, map(dato, 0, 2047, 0, 2047));
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
      dato =1900;// deseado 1100us
//      val = map(dato, 0, 1023, 0, 140);
//      mimotor.write(val);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
      enviar(dato);
      Serial.print("Iniciando analisis Tipo 1\n");
   }   
   // Si el String es "hemo2" Se envia el comando de velocidad para analisis tipo 2     
   else if (inputString=="hemo2"){
      dato =1100; //1500us deseado
//      val = map(dato, 0, 1023, 0, 255);
//      mimotor.write(val);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
      enviar(dato);
      Serial.print("Iniciando analisis Tipo 2\n");
   }
   // Si el String es "hemo3" Se envia el comando de velocidad para analisis tipo 3
   else if (inputString=="hemo3"){
      dato = 1500; //1900us deseado max
      enviar(dato);
//      analogWrite(A9, map(cal(dato), 0, 2047, 0, 2047));
      enviar(dato);
      Serial.print("Iniciando analisis Tipo 3\n");
      Serial.print(cal(dato));
   }

   else{

       Serial.print("error no se reconoce el comando ingresado \n");
   }
    

}
void loop(){
  // No se utiliza 
}
int cal (int dato) {
  int salida;
  return salida = (dato*2047)/4990;// Regla de 3 simple
}
void enviar(int dato) {
  int val = map(cal(dato), 0, 2047, 0, 2047);
//  int valc= val -2;
  analogWrite(A9, val);
}
