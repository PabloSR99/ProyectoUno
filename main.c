#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXCHAR 20
#define barrita "\n======================================\n"
#define barrita2 "\n--------------------------------------\n"

typedef struct {
  int numero;
  bool especial;
  char color[9];
  char tipo[15];
} tipoCarta;

typedef struct {
  List *mano;
  char nombre[20];
  int numCartas;
} tipoJugador;

typedef struct {
  tipoCarta *ultimaJugada;
  bool reversa;
} tipoPartida;

tipoCarta *crearCarta() {

  tipoCarta *carta= (tipoCarta*) malloc(sizeof(tipoCarta));
  carta->especial = false;
  carta->numero = (rand() % 14) + 1;

  /*
  Cancelar turno : 10
  Invertir turno : 11
  +2 : 12
  +4 : 13
  Cambiar color : 14
  */
  if(carta->numero < 13){
    int colorNumero = (rand() % 4) + 1;
    
    switch (colorNumero){
    case 1:
      strcpy(carta->color, "Rojo");
      break;
    case 2:
      strcpy(carta->color, "Verde");
      break;
    case 3:
      strcpy(carta->color, "Azul");
      break;
    case 4:
      strcpy(carta->color, "Amarillo");
      break;
    }
  }
  
  if(carta->numero > 9){
    int numeroEspecial = (rand() % 5) + 10;
    carta->especial = true;
    
    switch (numeroEspecial){
    case 10:
      strcpy(carta->tipo, "Cancelar");
      break;
    case 11:
      strcpy(carta->tipo, "Invertir");
      break;
    case 12:
      strcpy(carta->tipo, "+2");
      break;
    case 13:
      strcpy(carta->color, "+4");
      break;
    case 14:
      strcpy(carta->tipo, "Cambia color");
      break;
    }
  }

  return carta;
}
bool esValida(tipoCarta carta, tipoCarta ultimaJugada) {
  // sin tienen igual simbolo se puede tirar, reserva, +2, +4, saltar estan
  // incluidos
  if (carta.numero == ultimaJugada.numero)
    return true;

  // sin tienen igual color se puede tirar, reserva, +2, +4, saltar estan
  // incluidos
  if (strcmp(carta.color, ultimaJugada.color))
    return true;

  return false;
}

void iniciarPartiada(tipoPartida* juego, List *listaJuego) {
  
  for (int i = 0; i < 4; i++) {
    
    tipoJugador* jugadorAux = (tipoJugador*) malloc(sizeof(tipoJugador));
    
    jugadorAux->numCartas = 7;
    
    if (i == 0){
      char nombreAux[20];
      printf("indica tu nombre\n");
      scanf("%s", nombreAux);
      while (getchar() != '\n');
      
      strcpy(jugadorAux->nombre, nombreAux);
    }else if(i == 1){
      strcpy(jugadorAux->nombre, "CLAUDIO TOLEDO");
    }else if(i == 2){
      strcpy(jugadorAux->nombre, "MOHAMED DIAS");
    }else{
      strcpy(jugadorAux->nombre, "MR.HUNDOR");
    }
    printf("%s",jugadorAux->nombre);
    printf("\n");
    jugadorAux->mano = createList();

    
    for (int f = 0; f < 7; f++) {
      tipoCarta *carta = crearCarta();
      pushBack(jugadorAux->mano, carta);
    }

    tipoCarta *cartaux = firstList(jugadorAux->mano);
    for (int f = 0; f < 7; f++) {
      printf("%d, ", cartaux->numero);
      cartaux = nextList(jugadorAux->mano);  
    }
    
    pushFrontCircular(listaJuego, jugadorAux);
  }
}

void mostrarMano(tipoJugador *jugadorActual){

  tipoCarta *carta=firstList(jugadorActual->mano);
  printf("Jugador: %s\n",jugadorActual->nombre);
  puts("Mazo:\n");
  
  while(carta!=NULL){

    printf("%s\n",carta->tipo);
    printf("%d\n",carta->numero);
    printf("%s\n",carta->color);
    carta=nextList(jugadorActual->mano);

    
   /* printf("%s\n",carta->tipo);
    if(carta->numero < 12)
      printf("%s\n",carta->color);
    if(!carta->especial)
      printf("%d\n",carta->numero);
    */
  }
}

void jugarCarta(tipoPartida *juego,tipoJugador *jugadorActual){

  mostrarMano(jugadorActual);

  
}

int main(void) {

  srand(time(0)); //Funcion para asignar numeros aleatorios a las cartas 
  
  tipoPartida *juego = (tipoPartida*) malloc(sizeof(tipoPartida));
  juego->ultimaJugada = crearCarta();

  List *listaJuego = createList();
  iniciarPartiada(juego, listaJuego);

  tipoJugador *jugadorActual;
  jugadorActual=firstList(listaJuego);

  
  while(jugadorActual != NULL){
    jugarCarta(juego,jugadorActual);
    jugadorActual=nextList(listaJuego);
  }
  
  
 

  
  return 0;
}
