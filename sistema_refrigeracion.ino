/*
 * Simulador de refrigerasion de servidores
 * Por: Gabriel Barrón Rodríguez
 * Fecha: 18 de abril 2022 (32°C)
 * 
 * Este programa ejemplifica el uso de operadores relacionales y operadores
 * logicos:
 * 
 * Condiciones Iniciales
 * Sensor de temperatura y humedad
 * Boton1: Manual. Activa la refrigeración al ser presionado
 * Boton2: Alta demanda
 * Boton3: Sobre carga de funcionamiento
 * 
 * Led1: Refrigeracion manual
 * Led2: Refrigeración automática
 * 
 * -Que se encienda la refrigeracion manual cada que se presione el boton de manual
 * -La refrigeracion manual tiene prioridad a la refrigeracion automática.
 * -Si la termperatura es alta (mayor a 28, por ejemplo), se activa la refrigeracion automática
 * -Si tengo alta demanda o sobre carga, se activa la refrigeración automática
 * -Si tengo alta demanda o sobrecarga de funcionamiento y ademas temperatura alta, se activan ambas refrigeraciones
 * 
 * Conexiones propuestas
 * Botones 14, 15, 13
 * Leds 4, 2
 * DHT11 12
 * 
 */

// Bibliotecas
#include "DHT.h"

// Constantes
#define DHTPIN 12
#define DHTTYPE DHT11 

//Constantes de botones
const int BOTON1 = 14;
const int BOTON2 = 15;
const int BOTON3 = 13;
//Constantes de leds
const int LED1 = 4;
const int LED2 = 2;
//Nivel de temperatura
const int TEMP_H = 28;

// Variables
int boton1_dato;
int boton2_dato;
int boton3_dato;

int wait = 5000;
long timeNow, timeLast;

// Definición de objetos
DHT dht(DHTPIN, DHTTYPE);

// Condiciones iniciales - Se ejecuta sólo una vez al energizar
void setup() {// Inicio de void setup ()
  // Aquí va tu código
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  conf_buttons_leds();
  timeLast = millis ();
  dht.begin();//Iniciar comunicacion con el sensor DHT

}// Fin de void setup

// Cuerpo del programa - Se ejecuta constamente
void loop() {// Inicio de void loop
  // Espera 2 segundos no bloqueantes
  espera_no_bloqueante();


  Serial.println(t);
  switch_button();

  //********Automático por temperatura, sobrecarga y alta demanda**********
  switch_button2(t);


}// Fin de void loop

// Configuraciones de Botones & Leds
void conf_buttons_leds() {
  //Configurar botones de entrada
  pinMode (BOTON1, INPUT_PULLUP);
  pinMode (BOTON2, INPUT_PULLUP);
  pinMode (BOTON3, INPUT_PULLUP);
  //Configurar Leds
  pinMode (LED1, OUTPUT);
  pinMode (LED2, OUTPUT);
}

/*
 * Método que cambia el estado del LED1 a partir cuando se pulsa el botón
 */
void switch_button() {
  //********Lectura del boton1********
  boton1_dato = digitalRead (BOTON1);
  if (boton1_dato == 1) {
    digitalWrite (LED1, LOW);
  } else {
    digitalWrite (LED1, HIGH);
  }
}

/*
 * Método que dependiendo del estado de botones enciende el LED2
 */
void switch_button2(float t) {
  boton2_dato = digitalRead (BOTON2);
  boton3_dato = digitalRead (BOTON3);
  if (t > TEMP_H || boton2_dato == 1 || boton3_dato == 1) {
    digitalWrite (LED2, HIGH);
  } else {
    digitalWrite (LED2, LOW);
  }  
}

/*
 * Método que realiza espera no bloqueante
 */
void espera_no_bloqueante() {
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait) { // Si cumple que el tiempo > espera
      //*******Lectura del sensor*********
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
       // Check if any reads failed and exit early (to try again).
      if (isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }    
    timeLast = timeNow; // Actualización de seguimiento de tiempo

  }// fin del if (timeNow - timeLast > wait)
}


