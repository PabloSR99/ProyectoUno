#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
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


void menuTexto(int *opcion) {

  printf("\nElija una opcion para continuar:\n\n");
  printf("1. Iniciar partida.\n");
  printf("2. Ver reglas del juego.\n");
  printf("0. Salir del juego.\n");
  /*
    Usar string para comprobar que lo ingresa el usuario es un numero valido
    para nuestro menu
  */
  char comprobar[2];
  while (1) {
    scanf("%s", comprobar);

    if (isdigit(comprobar[0]) && strlen(comprobar) == 1)
    // ver que sea un numero de un digito
    {

      *opcion = atoi(comprobar);
      // ya cuando se comprueba se actualiza opcion
      return;
    }
    printf("Entrada invalida, ingrese una opcion valida entre 0 y 2\n");
  }
}

void mostrarReglas(){
  
}

tipoCarta *crearCarta() {

  tipoCarta *carta= (tipoCarta*) malloc(sizeof(tipoCarta));
  carta->especial = false;
  carta->numero = rand() % 15;

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
    carta->especial = true;
    
    switch (carta->numero){
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
      strcpy(carta->tipo, "+4");
      break;
    case 14:
      strcpy(carta->tipo, "Cambia color");
      break;
    }
  }

  return carta;
}

bool esValida(tipoPartida *juego, tipoCarta *carta) {

  if(carta->numero==13 || carta->numero==14)return true;
  
  if(juego->ultimaJugada->especial==true){
    if(strcmp(juego->ultimaJugada->tipo,carta->tipo)==0){
      return true;
    }
  }else{
     if (carta->numero == juego->ultimaJugada->numero){
        return true;
      }
  }
  if(strcmp(juego->ultimaJugada->color,carta->color)==0) return true;
    
  return false;
}

void iniciarPartiada(tipoPartida* juego, List *listaJuego){

  for (int i = 0; i < 4; i++) {
    
    tipoJugador* jugadorAux = (tipoJugador*) malloc(sizeof(tipoJugador));
    
    jugadorAux->numCartas = 7;
    
    if (i == 0){
      char nombreAux[20];
      printf("Indica tu usuario: ");
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
    jugadorAux->mano = createList();

    
    for (int f = 0; f < 7; f++) {
      tipoCarta *carta = crearCarta();
      pushBack(jugadorAux->mano, carta);
    }
    pushFrontCircular(listaJuego, jugadorAux);

    if( i < 1)
      puts("\nLos jugadores son:");
      
    
    printf("%s", jugadorAux->nombre);
    if( i != 3)
      printf(" - ");
  }
}

void mostrarMano(tipoJugador *jugadorActual){
  
  tipoCarta *carta=firstList(jugadorActual->mano);
  printf("\n\nJugador: %s\n",jugadorActual->nombre);
  puts("Mazo:\n");
  int i = 1;
  
  while(carta!=NULL){
    printf("carta %d:",i);
    if(carta->especial==false){
      printf(" %d,",carta->numero);
      printf(" %s\n",carta->color);
    }else{
      if(carta->numero<13){
      
        printf(" %s,",carta->color);
        printf(" %s\n",carta->tipo);
      }else{
        printf(" %s\n",carta->tipo);
      }  
    }
    puts(barrita);
    
    carta=nextList(jugadorActual->mano);
    i++;
  }
}

tipoCarta* seleccionarCarta(tipoPartida *juego, tipoJugador*jugadorActual){

  tipoCarta *aux;
  int numero;

  while(1){

    printf("Tienes carta valida? 1.- Si / 2.- No");
    scanf("%d",&numero);
    if(numero==2)return NULL;
    if(numero==1){
      puts("Selecciona la posicion de tu carta en el mazo");
      scanf("%d",&numero);
      aux = firstList(jugadorActual->mano);
  
      for(int a = 1 ; a < numero ; a++){
        aux=nextList(jugadorActual->mano);
      }
      if(esValida(juego,aux)==true){
        return aux;
      }
    }
  }
} 
void mostrarTop(tipoCarta *carta){
  
   printf("ultima jugada\n");
    if(carta->especial==false){
      printf(" %d,",carta->numero);
      printf(" %s\n",carta->color);
    }else{
      if(carta->numero<13){
      
        printf(" %s,",carta->color);
        printf(" %s\n",carta->tipo);
      }else{
        printf(" %s\n",carta->tipo);
      }  
    }
    puts(barrita);


  
}
void jugarCarta(tipoPartida *juego,tipoJugador *jugadorActual){
  mostrarTop(juego->ultimaJugada);
  mostrarMano(jugadorActual);
    
  tipoCarta*cartaAJugar=seleccionarCarta(juego,jugadorActual);

  

  
}

int main(void) {

  srand(time(0)); //Funcion para asignar numeros aleatorios a las cartas

  tipoPartida *juego = (tipoPartida*) malloc(sizeof(tipoPartida));
  juego->ultimaJugada = crearCarta();
  List *listaJuego = createList();

  int opcion ;

  printf("~~~~~~BIENVENIDO AL MENU DE UNO~~~~~~\n\n");
  while (1) {
    menuTexto(&opcion);

    switch (opcion) {
    case 1:
      printf("\n--------------------------------------------\n");
        tipoPartida *juego = (tipoPartida*) malloc(sizeof(tipoPartida));
        juego->ultimaJugada = crearCarta();
        List *listaJuego = createList();
        
        iniciarPartiada(juego, listaJuego);  

        /*
        while(1){
          jugarCarta(juego,jugadorActual);
          jugadorActual=nextList(listaJuego);
        }
        */
      
        tipoJugador *jugadorActual;
        jugadorActual = firstList(listaJuego);
        
        for(int i=0 ; i < 4 ; i++ ){
          jugarCarta(juego,jugadorActual);
          jugadorActual=nextList(listaJuego);
        }
      
      printf("\n--------------------------------------------\n");
      
      break;
    case 2:
      printf("\n--------------------------------------------\n");

      mostrarReglas();
      printf("\n--------------------------------------------\n");

      break;
    case 0:

      exit(EXIT_SUCCESS);

    default:
      printf("Elija una opcion del menu para continuar:\n\n");
    }
  }
  


  
  /*
  while(1){
    jugarCarta(juego,jugadorActual);
    jugadorActual=nextList(listaJuego);
  }
  */

  tipoJugador *jugadorActual;
  jugadorActual = firstList(listaJuego);
  
  for(int i=0 ; i < 4 ; i++ ){
    jugarCarta(juego,jugadorActual);
    jugadorActual=nextList(listaJuego);
  }
 

  
  return 0;
}
