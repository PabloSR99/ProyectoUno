#include "list.h"
#include <ctype.h>
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

  tipoCarta *carta = (tipoCarta *)malloc(sizeof(tipoCarta));
  carta->especial = false;
  carta->numero = rand() % 15;

  /*
  Cancelar turno : 10
  Invertir turno : 11
  +2 : 12
  +4 : 13
  Cambiar color : 14
  */
  if (carta->numero < 13) {
    int colorNumero = (rand() % 4) + 1;

    switch (colorNumero) {
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

  if (carta->numero > 9) {
    carta->especial = true;

    switch (carta->numero) {
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
} //LISTO(?)

bool esValida(tipoPartida *juego, tipoCarta *carta) {

  if (carta->numero == 13 || carta->numero == 14)
    return true;

  if (juego->ultimaJugada->especial == true) {
    if (strcmp(juego->ultimaJugada->tipo, carta->tipo) == 0) {
      return true;
    }
  } else {
    if (carta->numero == juego->ultimaJugada->numero) {
      return true;
    }
  }
  if (strcmp(juego->ultimaJugada->color, carta->color) == 0)
    return true;

  return false;
}

void iniciarPartiada(tipoPartida *juego, List *listaJuego) {

  for (int i = 0; i < 4; i++) {

    tipoJugador *jugadorAux = (tipoJugador *)malloc(sizeof(tipoJugador));

    jugadorAux->numCartas = 7;

    if (i == 0) {
      char nombreAux[20];
      printf("Indica tu usuario: ");
      scanf("%s", nombreAux);
      while (getchar() != '\n');

      strcpy(jugadorAux->nombre, nombreAux);
    } else if (i == 1) {
      strcpy(jugadorAux->nombre, "MR.HUNDOR");
    } else if (i == 2) {
      strcpy(jugadorAux->nombre, "CLAUDIO TOLEDO");
    } else {
      strcpy(jugadorAux->nombre, "MEDIANO");
    }
    jugadorAux->mano = createList();

    for (int f = 0; f < 7; f++) {
      tipoCarta *carta = crearCarta();
      pushBack(jugadorAux->mano, carta);
    }
    pushFrontCircular(listaJuego, jugadorAux);

    if (i == 0)
      puts("\nLos jugadores son:");

    printf("%s", jugadorAux->nombre);
    if (i != 3)
      printf(" - ");
  }
  printf("\n");
}

void mostrarMano(tipoJugador *jugadorActual) {

  tipoCarta *carta = firstList(jugadorActual->mano);
  printf("Jugador: %s\n", jugadorActual->nombre);
  printf("Mazo:");
  int i = 1;
  puts(barrita);
  while (carta != NULL) {
    printf("carta %d:", i);
    if (carta->especial == false) {
      printf(" %d,", carta->numero);
      printf(" %s", carta->color);
    } else {
      if (carta->numero < 13) {

        printf(" %s,", carta->color);
        printf(" %s", carta->tipo);
      } else {
        printf(" %s", carta->tipo);
      }
    }
    puts(barrita);

    carta = nextList(jugadorActual->mano);
    i++;
  }
} //LISTO (?)
void mostrarTop(tipoCarta *carta) {

  puts(barrita);
  printf("ultima jugada\n");
  if (carta->especial == false) {
    printf(" %d,", carta->numero);
    printf(" %s", carta->color);
  } else {
    if (carta->numero < 13) {

      printf(" %s,", carta->color);
      printf(" %s", carta->tipo);
    } else {
      printf(" %s", carta->tipo);
    }
  }
  puts(barrita);
} //LISTO (?)

bool seleccionarCarta(tipoPartida *juego, tipoJugador *jugadorActual, tipoCarta *cartaAJugar) {

  int numero;

  while (1) {
    
    printf("¿Tienes carta valida? 1.- Si / 2.- No, robo carta (pasas turno).\n");
    scanf("%d", &numero);
    while (getchar() != '\n');

    if (numero == 2)
      return false;
    if (numero == 1) {
      printf("Selecciona la posicion de tu carta en el mazo\n");
      scanf("%d", &numero);
      
      cartaAJugar = firstList(jugadorActual->mano);

      for (int a = 1; a < numero; a++) {
        cartaAJugar = nextList(jugadorActual->mano);
      }
      if (esValida(juego, cartaAJugar) == true) {
        return true;
      }
      else { printf("Carta no valida\n");}
    }
  }
}

int validarColor(char *color) {
  //  entrada con color valido
  if (strcmp(color, "Rojo") == 0 || strcmp(color, "Azul") == 0 ||
      strcmp(color, "Verde") == 0 || strcmp(color, "Amarillo") == 0) {
    return 1; // valido
  } else {
    return 0; // invalido
  }
}

void jugarCarta(tipoPartida *juego, tipoJugador *jugadorActual, List *listaJuego) {

  char color[20];
  mostrarTop(juego->ultimaJugada);
  mostrarMano(jugadorActual);
  tipoCarta *cartaAJugar = (tipoCarta*) malloc(sizeof(tipoCarta));
  bool jugo = seleccionarCarta(juego, jugadorActual, cartaAJugar);
  
  if(jugo==true){
    
    popCurrent(jugadorActual->mano);
    juego->ultimaJugada = cartaAJugar;
    mostrarMano(jugadorActual);
    
      if (cartaAJugar->especial) 
    {
      switch (cartaAJugar->numero) 
      {
        case 10:
          if (juego->reversa)
            jugadorActual = prevList(listaJuego);
            
          else
            jugadorActual = nextList(listaJuego);
          break;
    
        case 11:
          if (juego->reversa)
            juego->reversa = false;
          else
            juego->reversa = true;
          break;
    
        case 12:
          if (juego->reversa) {
            jugadorActual = prevList(listaJuego);
            for (int f = 0; f < 2; f++)
            {
              tipoCarta *carta = crearCarta();
              pushBack(jugadorActual->mano, carta);
            }
          } else 
          {
            jugadorActual = nextList(listaJuego);
            for (int f = 0; f < 2; f++) 
            {
              tipoCarta *carta = crearCarta();
              pushBack(jugadorActual->mano, carta);
            }
          }
          break;
    
        case 13:
          if (juego->reversa) {
            jugadorActual = prevList(listaJuego);
            for (int f = 0; f < 4; f++)
            {
              tipoCarta *carta = crearCarta();
              pushBack(jugadorActual->mano, carta);
            }
          } else 
          {
            jugadorActual = nextList(listaJuego);
            for (int f = 0; f < 4; f++) 
            {
              tipoCarta *carta = crearCarta();
              pushBack(jugadorActual->mano, carta);
            }
          }
          printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): ");
          scanf("%s", color);
          juego->ultimaJugada->numero = 0;
          strcpy(juego->ultimaJugada->color , color);
          
          break;
    
        case 14:
          printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
          scanf("%s", color);
          while (!validarColor(color)) {
            printf("El color ingresado '%s' es inválido.\n", color);
            printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
            scanf("%s", color);
          }
          juego->ultimaJugada->numero = 0;
          strcpy(juego->ultimaJugada->color , color);
          break;
      }
    }
  }else{
    tipoCarta *carta = crearCarta();
    pushBack(jugadorActual->mano, carta);
  }
  
  
}

int main(void) {

  srand(time(0)); // Funcion para asignar numeros aleatorios a las cartas

  tipoPartida *juego = (tipoPartida *)malloc(sizeof(tipoPartida));
  juego->ultimaJugada = crearCarta();
  List *listaJuego = createList();

  int opcion;

  printf("~~~~~~| BIENVENIDO AL MENU DE UNO |~~~~~~\n\n");
  while (1) {
    menuTexto(&opcion);

    switch (opcion) {
    case 1:
      printf("\n--------------------------------------------\n");
      tipoPartida *juego = (tipoPartida *)malloc(sizeof(tipoPartida));
      juego->ultimaJugada = crearCarta();
      List *listaJuego = createList();

      iniciarPartiada(juego, listaJuego);
      tipoJugador *jugadorActual;
      jugadorActual = firstList(listaJuego);
      
      while(1){
        jugarCarta(juego, jugadorActual, listaJuego);
        if(juego->reversa==true){
          jugadorActual = prevList(listaJuego);
        }else{
          jugadorActual = nextList(listaJuego);
        }
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
  return 0;
}
