#include <LiquidCrystal.h>

LiquidCrystal lcd_1(8, 9, 4, 5, 6, 7);

// VALORES ANALOGICAS:
// SELECT (640 A 668)
// UP (99 A 127)
// DOWN (255 A 280)
// RIGHT (0 A 60)
// LEFT(399 A 435)

//matriz de animacion
byte customChar_A[8]={
  B00000,
  B00000,
  B00000,
  B00011,
  B00011,
  B00011,
  B00111,
  B00111
};
byte customChar_B[8]={
  B00111,
  B00111,
  B00111,
  B00111,
  B00111,
  B00111,
  B00111,
  B00000
};
byte customChar_C[8]={
  B00000,
  B00010,
  B01001,
  B00010,
  B10000,
  B00101,
  B10010,
  B10000
};

byte customChar_D[8]={
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B00000
};

byte customChar_E[8]={
  B00000,
  B00000,
  B01000,
  B00100,
  B10000,
  B00010,
  B01000,
  B00000
};


//VARIABLES PARA EL CONTADOR ASCENDIENTE
int contador = 0;   // contador para el numero de veces presinados

int calculo_tiempoSeg = 0; // valor que va a tomar como bandera

//VARIABLES PARA EL TIMER
unsigned long tiempo_actual; // variable que almacena el tiempo de los millis 
unsigned long tiempo_anterior = 0;// variable que almacena el tiempo anterior
unsigned long delta_tiempo;// variable que realiza el calculo del tiempo transcurrido
int contador2 = 1; // contador para los segundos
int mostrarHora = LOW;

//VARIABLES PARA EL RELOJ
int option;
int contadorSegs = 0;
int contadorMins = 0;
int contadorHs = 0;
int hrI;
int minI;
int botonAnalo;

//VARIABLES PARA EL RELE
const int rele = 13;

void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2); 
  
  //Creacion de los caracteres
  lcd_1.createChar(0, customChar_A); //Primer matriz
  lcd_1.createChar(1, customChar_B); //Segunda matriz
  lcd_1.createChar(2, customChar_C); //Tercera matriz
  lcd_1.createChar(3, customChar_D); //Cuarta matriz
  lcd_1.createChar(4, customChar_E); //Quinta matriz
  
  //Pines Rele
  pinMode(rele, OUTPUT);
}


void loop() {
  tiempo_actual = millis();
  delta_tiempo = tiempo_actual - tiempo_anterior;
  
  botonAnalo = analogRead(A0);
  
  
   if (Serial.available() != 0) {
    int option = Serial.parseInt();
    
    if (option == 1 || option == 2) {
      switch (option) {
        case 1:
        	Serial.println("Ingrese la hora: ");
        	while(Serial.available() == 0){
              
        	}
        	hrI = Serial.parseInt();
        	if (hrI < 0 || hrI > 23){
        		Serial.println("Error no se aceptan valores negativos o mayores de 23");
            }else{
            	contadorHs = hrI;	
            }
        	break;
        case 2:
        	Serial.println("Ingrese los minutos: ");
        	while(Serial.available() == 0){
              
        	}
        	minI = Serial.parseInt();
            if (minI < 0 || minI > 59){
				Serial.println("Error no se aceptan valores negativos o mayores de 59");
            }else{
            	contadorMins = minI;
            }
        	break;
      }
    }
  }
  
  if (delta_tiempo >= 1000 && analogRead(A0) >= 1023){
    if(!mostrarHora)
    {
      lcd_1.clear();
      lcd_1.setCursor(6, 1);
      if(contadorHs < 10) lcd_1.print("0");
      lcd_1.print(contadorHs);
      lcd_1.print(":");
      lcd_1.setCursor(9, 1);
      if(contadorMins < 10) lcd_1.print("0");
      lcd_1.print(contadorMins);
    }
  }
  
  if (contadorSegs > 59) {
    contadorMins++;
  	contadorSegs = 0; 
  }
  
  if (contadorMins > 59) {
    contadorHs++;
    contadorSegs= 0;
  	contadorMins = 0;
  }
  
  if (contadorHs > 23) {
  	contadorHs = 0;
    contadorMins = 0;
  }
  
  
  String boton = botones_LCD(int botonAnalo);
  
  
  // para que el contador no se exceda de 60
  // BOTON UP
  if (boton == "btnArriba"){
    if (contador < 60){
          lcd_1.clear();
          contador++;
          Serial.println("+");
          lcd_1.setCursor(0, 0);
          lcd_1.print("Cant de minutos:");
          lcd_1.setCursor(0, 1);
          lcd_1.print(contador);
          mostrarHora = HIGH;
        }
  }
  
  // Control para no ingresar numeros negativos
  // BOTON DOWN
  if (boton == "btnAbajo"){
    if (contador >= 1){
          lcd_1.clear();
          contador--;
          Serial.println("-");
          lcd_1.setCursor(0, 0);
          lcd_1.print("Cant de minutos:");
          lcd_1.setCursor(0, 1);
          lcd_1.print(contador);
          mostrarHora = HIGH;
      }
  }

  
  // para no setear con un valor negativo o 0
  // BOTON SELECT
  if (boton == "btnSelect"){
    if (contador > 0){
          contador2 = 0;
          calculo_tiempoSeg = contador * 60;
          lcd_1.clear();
          lcd_1.setCursor(0, 0);
          lcd_1.print("Intervalo cada:");
          lcd_1.setCursor(0, 1);
          lcd_1.print(contador);
          lcd_1.setCursor(2, 1);
          lcd_1.print("Minutos");
          mostrarHora = LOW;
    }
  }
  
  // Para eliminar el intevalo BOTON RIGHT
  if (boton == "btnDerecha"){
      calculo_tiempoSeg = 0;
    }
  //LEFT(399 A 435)
  if (boton == "btnIzquierda"){
  	digitalWrite(rele, HIGH);
    espera();
    digitalWrite(rele, LOW);
  }
  
  if (delta_tiempo >= 1000){
  	contador2++;
    //Serial.println(delta_tiempo);
    Serial.println(contador2);
    contadorSegs++;
    tiempo_anterior = tiempo_actual;
  }
  
  if (contador2 == calculo_tiempoSeg){	
    contador2 = 0;
    lcd_1.clear();//Iniciamos la animacion
    lcd_1.setCursor(3,0);
    lcd_1.write((byte)0);
    lcd_1.setCursor(3,1);
    lcd_1.write((byte)1);
    lcd_1.setCursor(4,0);
    lcd_1.write((byte)2);
    lcd_1.setCursor(4,1);
    lcd_1.write((byte)3);
    lcd_1.setCursor(5,0);
    lcd_1.write((byte)4);
    lcd_1.setCursor(7,0);
    lcd_1.print("SPRAY");
    lcd_1.setCursor(6,1);
    lcd_1.print("ACTIVADO");
    digitalWrite(rele, HIGH);
    espera();
    digitalWrite(rele, LOW);
    lcd_1.clear();
  }
}

void espera(){
	delay(900);
}


//Creamos una funcion que devuelva un texto de que boton se utilizo basandose en los rangos de los valores
String botones_LCD(int buttonReply) {
  String text;
  Serial.println(buttonReply);
  //if (buttonReply < 60) text = "btnDerecha";
  if (buttonReply > 0 && buttonReply < 60) text = "btnDerecha";
  //if (buttonReply > 99 && buttonReply < 127)text = "btnArriba";
  if (buttonReply > 99 && buttonReply < 127) text = "btnArriba";
  //if (buttonReply > 255 && buttonReply < 280)  text = "btnAbajo";
  if (buttonReply > 255 && buttonReply < 280) text = "btnAbajo";
  // if (buttonReply > 399 && buttonReply < 435)  text = "btnIzquierda";
  if (buttonReply > 399 && buttonReply < 435) text = "btnIzquierda";
  // if (buttonReply > 640 && buttonReply < 668)  text = "btnSelect";
  if (buttonReply > 640 && buttonReply < 668) text = "btnSelect";
  return text;
}
