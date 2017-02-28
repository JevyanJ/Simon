/*
 * ARDUINO SAYS
 * Programado por: Marina Rodríguez Garrido
 *
*/


#include "pitches.h"



int maxsec = 25;              // Número máximo de valores de secuencia. NOTA: debe coincidir con el tamaño de secuencia.
int secuencia[25];            // Vector de valores aleatorios entre 0 y 3.
int numsec = 0;               // Número actual de elementos de secuencia.

int espera = 500;             // Tiempo que se mantiene el led encendido cuando muestra la secuencia.

int numbotones = 4;           // Número de botones (y leds). NOTA: debe coincidir con el tamaño de pinled y pinboton.
int pinled[] = {8,9,10,11};   // Lista de pines para los led.
int pinboton[] = {2,3,4,5};   // Lista de pines para los botones.
int pinzum=A1;                // Pin del zumbador.



int notas_botones[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5};

/* *** MÚSICA ***
 * Cada canción usa 3 variables obligatorias y dos opcionales:
 *   mel_<nombre>   -> Notas de la canción (consultar el fichero pitches.h para ver la lista de notas)
 *   dur_<nombre>   -> Duración de cada nota (4 = quarter note, 8 = eighth note, etc.)
 *   tam_<nombre>   -> Tamaño de los dos vectores anteriores (Deben ser iguales)
 *   luz_<nombre>   -> Luz que se tiene que encender con cada nota (opcional)
 *   ltam_<nombre>  -> Tamaño del vector de luces (opcional)
*/


//Música inicio interacción de usuario
int mel_comenzar[]={NOTE_C4};
int dur_comenzar[]={4};
int tam_comenzar=1;



//Música inicio del juego
int mel_iniciojuego[]={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int dur_iniciojuego[]={4, 8, 8, 4, 4, 4, 4, 4};
int tam_iniciojuego=8;
int luz_iniciojuego[]={1, 0, 0, 1, 0, -1, 2, 3};
int ltam_iniciojuego = 8;


//Música perder
int mel_perder[]={0, NOTE_C4, NOTE_B3, NOTE_AS3};
int dur_perder[]={2,4,4,1};
int tam_perder=4;
int luz_perder1[]={-1, 0, 1, 0};
int luz_perder2[]={-1, 2, 3, 2};
int ltam_perder = 4;


//Música ganar
int mel_bien[]={NOTE_E5, NOTE_G5, NOTE_C6};
int dur_bien[]={4,4,4};
int tam_bien=3;




void setup() {
	
  Serial.begin(19200);
  randomSeed(analogRead(1)); //para que no se repita la secuencia
  for (int i=0; i<numbotones; i=i+1){
    pinMode (pinled[i], OUTPUT);
  }
  for (int i=0; i<numbotones; i=i+1){
    pinMode (pinboton[i], INPUT);
  }
  pinMode (pinzum, OUTPUT);

  Serial.println("EMPIEZA LA PARTIDA");
  reproducir(mel_iniciojuego, dur_iniciojuego, tam_iniciojuego, luz_iniciojuego, ltam_iniciojuego);
  
}




void loop() {
	
  //INIT parte de mostrar secuencia de luces
  
  delay(1000);
  rellenar(); // Añadimos un nuevo valor a la secuencia
  for (int i=0; i<numsec; i=i+1) {
    encender(i, espera);
    delay (espera);
  }
//  reproducir (mel_comenzar, dur_comenzar, tam_comenzar);
  
  //FIN parte de mostrar secuencia de luces
  
  
  //INIT parte de pulsar botones

  Serial.println("Turno del jugador.");
  
  int comprobar=0; //Igual a 0 si no se pulsa ningún botón, +1 botón correcto, -1 botón incorrecto
  for (int i=0; i<numsec && comprobar!=-1; i=i+1) {
    int boton_correcto=secuencia[i];
    comprobar=0;
	
    int boton_pulsado=pulsar_boton(); //Se queda esperando a que se pulse un botón
	
    delay(100);
    if (boton_pulsado==boton_correcto){
          comprobar=1;
    }
     else {
          comprobar=-1;
     }
}
  if (comprobar==1){
    //has ganado la ronda
    Serial.println("Has acertado :)");
    delay(200);
    reproducir(mel_bien, dur_bien, tam_bien);
    if (numsec==maxsec){
      //has ganado el juego
      ganarjuego();
    }
  }
  else {
    //has perdido
    perder ();
  }
  
  //FIN parte de pulsar botones 

}




/******************************
*                             *
*          FUNCIONES          *
*                             *
******************************/


/*
 * ENCENDER
 * Recibe una posición del vector de secuencia y un tiempo.
 * Enciende el led correspondiente durante el tiempo indicado.
 *
*/
void encender(int posic, int duracion){
  int valor=secuencia[posic];
  int pin=pinled[valor];
  digitalWrite(pin, HIGH);
  tone(pinzum, notas_botones[valor]);
  delay(duracion);
  noTone(pinzum);
  digitalWrite(pin, LOW);
}

/*
 * get Pin Led
 * Recibe el indice de la luz y devuelve el pin en el que está.
 * Si recibe un valor fuera de rango (0, 1, 2, 3) devuelve null
 *
*/
int getPinLed(int indice) {
  if(indice < 0 or indice > 3) {
    return NULL;
  } else {
    return pinled[indice];
  }
}

/*
 * RELLENAR
 * Inserta un nuevo elemento entre 0 y 3 en secuencia
*/
void rellenar() {
    secuencia[numsec]=random(4);
    numsec=numsec+1;
}



/*
 * REPRODUCIR
 * Reproduce melodías en el zumbador.
 * Recibe como parámetros las notas y la duración de cada nota, como vectores de enteros, y el tamaño de los vectores
 * Fuente: https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone
*/
void reproducir(int nota[], int duracion[], int tam) {
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote<tam; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / duracion[thisNote];
    tone(pinzum, nota[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinzum);
  }
}


/*
 * REPRODUCIR
 * Reproduce melodías en el zumbador.
 * Recibe como parámetros las notas y la duración de cada nota, como vectores de enteros, y el tamaño de los vectores
 * Fuente: https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone
 * 
 * Luces.
 * Opcionalmente esta función puede recibir un vector de enteros con los índices de los pines que debe encender y el tamaño
 * del vector.
 * La función encenderá y apagará los leds al ritmo de las notas. Si el vector de luces es menor que el de la música, repetirá
 * la secuencia hasta que la música pare.
*/
void reproducir(int nota[], int duracion[], int tam, int luces[], int tam_luces) {

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote<tam; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500 / duracion[thisNote];

    
    int luz = getPinLed(luces[thisNote % tam_luces]);    

    if(luz != NULL) {
      digitalWrite(luz, HIGH);
    }
    tone(pinzum, nota[thisNote]);
    delay(noteDuration);
    noTone(pinzum);
    if(luz != NULL) {
      digitalWrite(luz, LOW);
    }
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinzum);
  }
}

/*
 * REPRODUCIR
 * Reproduce melodías en el zumbador.
 * Recibe como parámetros las notas y la duración de cada nota, como vectores de enteros, y el tamaño de los vectores
 * Fuente: https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone
 * 
 * Luces.
 * Opcionalmente esta función puede recibir un vector de enteros con los índices de los pines que debe encender y el tamaño
 * del vector.
 * La función encenderá y apagará los leds al ritmo de las notas. Si el vector de luces es menor que el de la música, repetirá
 * la secuencia hasta que la música pare.
*/
void reproducir(int nota[], int duracion[], int tam, int luces1[], int luces2[], int tam_luces) {

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote<tam; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500 / duracion[thisNote];

    
    int luz1 = getPinLed(luces1[thisNote % tam_luces]);
    int luz2 = getPinLed(luces2[thisNote % tam_luces]);
    if(luz1 != NULL) {
      digitalWrite(luz1, HIGH);
    }
    if(luz2 != NULL) {
      digitalWrite(luz2, HIGH);
    }
    tone(pinzum, nota[thisNote]);
    delay(noteDuration);
    noTone(pinzum);
    if(luz1 != NULL) {
      digitalWrite(luz1, LOW);
    }
    if(luz2 != NULL) {
      digitalWrite(luz2, LOW);
    }    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinzum);
  }
}


/*
 * PERDER
 * Ejecuta las acciones necesarias cuando se pierde el juego.
*/
void perder(){
  reproducir(mel_perder, dur_perder, tam_perder, luz_perder1, luz_perder2, ltam_perder);
  Serial.println("Has perdido :(");
  Serial.print("Tu nivel es ");
  Serial.println(numsec);
  numsec=0;
  Serial.println("Pulsa cualquier botón para empezar de nuevo");
  pulsar_boton();
  Serial.println("---------------------------------------------------------");
  Serial.println("PARTIDA NUEVA");
  delay(500);
}



/* 
 * GANARJUEGO
 * Ejecuta las acciones necesarias cuando se gana el juego.
*/
void ganarjuego(){
  Serial.println("HAS GANADO :D");
  //TODO música de ganar juego
  numsec=0;
  Serial.println("Pulsa cualquier boton para empezar de nuevo");
  pulsar_boton();
  Serial.println("---------------------------------------------------------");
  Serial.println("PARTIDA NUEVA");
  delay(2000);
}



/* 
 * PULSAR_BOTON
 * Mantiene el sistema en espera hasta que se pulsa un botón.
 * Una vez pulsado enciende el led correspondiente y espera a que se suelte.
 * 
 * Devuelve el número del botón que se ha pulsado.
*/
int pulsar_boton(){
  while (true){
      for (int j=0; j<numbotones; j=j+1){
        if (digitalRead(pinboton[j])== HIGH){
          int pin=pinled[j];
          int nota = notas_botones[j];
          digitalWrite(pin, HIGH);
          tone(pinzum, nota);
          while (digitalRead(pinboton[j])== HIGH){
            delay(2);
          }
          digitalWrite(pin, LOW);
         noTone(pinzum);
          return(j);
        }
      }
  }
}
