#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
  #define OLED_DC     6
  #define OLED_CS     7
  #define OLED_RESET  8
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/



//int tablero[5][5] = {            // tablero[ejeX largo][ejeY ancho]
//  {0, 0, 0, 0, 1,},
//  {0, 1, 0, 0, 0,},
//  {0, 0, 0, 1, 0,},
//  {1, 0, 0, 0, 0,},
//  {0, 0, 1, 0, 0,},
//};
//int tablero[5][5] = {            // tablero[ejeX largo][ejeY ancho]
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//  {0, 0, 1, 0, 0,},
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//};
int tablero[5][5] = {            // tablero[ejeX largo][ejeY ancho]
  {0, 1, 0, 1, 0,},
  {1, 0, 1, 0, 1,},
  {1, 0, 0, 0, 1,},
  {0, 1, 0, 1, 0,},
  {0, 0, 1, 0, 0,},
};
//int tablero[5][5] = {            // tablero[ejeX largo][ejeY ancho]
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//  {0, 0, 0, 0, 0,},
//};
//int tablero[5][5] = {            // tablero[ejeX largo][ejeY ancho]
//  {1, 1, 1, 1, 1,},
//  {1, 1, 1, 1, 1,},
//  {1, 1, 1, 1, 1,},
//  {1, 1, 1, 1, 1,},
//  {1, 1, 1, 1, 1,},
//};

int anchoBloque = 8;
int largoBloque = 21;


int separador = 112;              // Coordenada de la barra de separación del juego y datos


int posBarra = 55;                // Posición de la barra al iniciar. Se cambia con el valor analógico leído en A0
int barraLong = 10;               // Longitud de la barra
int barraMLong = barraLong / 2;   // Longitud de media barra


int bolaDirX = 1;                 // Dirección (signo) y velocidad (valor) en eje Y de la bola
int bolaDirY = -1;                // Dirección (signo) y velocidad (valor) en eje X de la bola

int bolaX = 55;                   // Coordenada X de la bola
int bolaY = 55;                   // Coordenada Y de la bola
int PbolaX = 55;                  // Coordenada X anterior de la bola
int PbolaY = 55;                  // Coordenada Y anterior de la bola


const int zumb = 6;

void setup() {
  Serial.begin(9600);

  pinMode(zumb, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.dim(true);

  // Clear the buffer
  display.clearDisplay();                                             // Limpiar datos a enviar a la pantalla

  display.drawRect(0, 0, display.width(), display.height(), WHITE);   // Dibujar rectángulo contorno
  display.drawRect(0, 0, 113, display.height(), WHITE);               // Dibujar rectángulo tablero

  display.display();                                                  // Enviar datos a la pantalla
  
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 5; i++) {
      display.fillRect(i * 22 + 2, j * 9 + 2, 21, 8, WHITE);          // Aparecer tablero en horizontal
      display.display();
      delay(50);
    }
  }
  display.dim(true);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      display.fillRect(i * 22 + 2, j * 9 + 2, 21, 8, BLACK);          // Desaparecer tablero en vertical
      display.display();
      delay(50);
    }
  }
  display.display();
  delay(1000);
  //display.invertDisplay(true);
}


void loop() {

  actBarra();
  mostrar();
  actBola();
  
  display.display();
//  delay(100);
}

void rebotes() {
  if (bolaX <= 1 || bolaX >= separador - 1) {   // Rebotes laterales
    bolaDirX = -bolaDirX;
    tone(zumb, 500, 50);
  }
  if(bolaY <= 2) {  //Rebote superior
    bolaDirY = -bolaDirY;
    tone(zumb, 500, 50);
  }
//  if (bolaY == 47) {    // Rebote parte inferior de bloques SOLO PRUEBA
//    bolaDirY = -bolaDirY;
//  }
  if(bolaX > posBarra - barraMLong && bolaX < posBarra + barraMLong && bolaY == display.height() - 5) {   // Rebote barra
    bolaDirY = -bolaDirY;
    tone(zumb, 600, 50);
  }
  if(bolaY == display.height() - 2) {   // Rebote suelo SOLO PRUEBA
    bolaDirY = -bolaDirY;
    tone(zumb, 100, 50);
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if(tablero[j][i] == 1) {
//        if(bolaX > i * 22 + 2-1 && bolaY > j * 9 + 2 && bolaY < j * 9 + 2 + 8  ||  bolaX < i * 22 + 2 + 21 && bolaY > j * 9 + 2 && bolaY < j * 9 + 2 + 8) {   // Rebote lateral bloques
//          bolaDirX = -bolaDirX;
//          tablero[j][i] = 0;
//        }
        if(bolaX > i*22+2-1 && bolaX < i*22+2+20+1 && bolaY == j * 9 + 2-1 || bolaX > i*22+2-1 && bolaX < i*22+2+20+1 && bolaY == i * 9 + 2 +7+1) {             // Rebote inferior y superior bloques
          bolaDirY = -bolaDirY;
          tablero[j][i] = 0;
          tone(zumb, 240, 50);
        }
      }
    }
  }
}

void actBarra() {
  posBarra = map(analogRead(A0), 0, 1023, 1 + barraMLong, separador - barraMLong);
  display.drawFastHLine(1, display.height() - 4, separador - 1, BLACK);
  display.drawFastHLine(posBarra - barraMLong, display.height() - 4, barraLong, WHITE);
//  display.display();
}

void actBola() {
  PbolaX = bolaX;                             // Guardando coordenada X anterior de la bola
  PbolaY = bolaY;                             // Guardando coordenada Y anterior de la bola
  bolaX += bolaDirX;                          // Actualizando coordenada X de la bola
  bolaY += bolaDirY;                          // Actualizando coordenada Y de la bola

  rebotes();
  
  display.drawPixel(PbolaX, PbolaY, BLACK);   // Borrar posición anterior de la bola
  display.drawPixel(bolaX, bolaY, WHITE);     // Mostrar posición nueva de la bola
//  display.display();
}

void mostrar() {
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 5; i++) {
      if (tablero[j][i] == 0) {
        display.fillRect(i * 22 + 2, j * 9 + 2, 21, 8, BLACK);    // Si valor de tablero es 0 "mostrar" bloque negro
      } else {
        display.fillRect(i * 22 + 2, j * 9 + 2, 21, 8, WHITE);    // Si valor de tablero es 1 mostrar bloque blanco
      }
    }
  }
//  display.display();
}
