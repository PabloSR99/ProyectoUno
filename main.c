#include "list.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXCHAR 20
#define barrita "\n================================"
#define barrita2 "\n-------------------------------"

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
  int numJugador;
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

void mostrarReglas() {
  puts("BIENVENIDO A UNO: EL JUEGO DE CARTAS\n\n");
  puts("EL OBJETIVO DEL JUEGO SERÁ QUEDARSE SIN CARTAS EN MANO.\n");
  puts("A CONTINUACION TE MOSTRAREMOS LAS REGLAS DEL JUEGO:\n");
  puts("1. CADA JUGADOR TENDRA AL INICIO DE LA PARTIDA 7 CARTAS DE DISTINTO "
       "TIPO Y COLORES. PUEDEN TOCAR DE TIPO NUMERO DE 0 A 9 Y ESPECIALES.\n");
  puts("2. EXISTEN 2 TIPOS DE CARTAS, LAS CARTAS COMUNES DE COLOR Y NUMERICAS "
       "DE 0 A 9 DE DISTINTOS COLORES Y LAS ESPECIALES COMO CAMBIO DE SENTIDO, "
       "SALTO DE TURNO, +2 CARTAS, +4 CARTAS Y CAMBIAR COLOR.\n");
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
} // LISTO(?)

tipoCarta *crearCartaMazo() {

  tipoCarta *carta = (tipoCarta *)malloc(sizeof(tipoCarta));
  carta->especial = false;
  carta->numero = rand() % 9;

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
  return carta;
}

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
      while (getchar() != '\n')
        ;
      jugadorAux->numJugador = 1;
      strcpy(jugadorAux->nombre, nombreAux);
    } else if (i == 1) {
      strcpy(jugadorAux->nombre, "2");
      jugadorAux->numJugador = 2;
    } else if (i == 2) {
      strcpy(jugadorAux->nombre, "3");
      jugadorAux->numJugador = 3;
    } else {
      strcpy(jugadorAux->nombre, "4");
      jugadorAux->numJugador = 4;
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
      printf(" [ %d,", carta->numero);
      printf(" %s ]", carta->color);
    } else {
      if (carta->numero < 13) {

        printf(" [ %s,", carta->color);
        printf(" %s ]", carta->tipo);
      } else {
        printf(" [ %s ]", carta->tipo);
      }
    }
    puts(barrita);

    carta = nextList(jugadorActual->mano);
    i++;
  }
} // LISTO (?)

void mostrarTop(tipoCarta *carta) {

  puts("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  printf("Ultima carta jugada\n");
  if (carta->especial == false) {
    printf(" [ %d,", carta->numero);
    printf(" %s ]\n", carta->color);
  } else {
    if (carta->numero < 13) {

      printf(" [ %s,", carta->color);
      printf(" %s ]\n", carta->tipo);
    } else {
      printf(" [ %s ]\n", carta->tipo);
    }
  }
  puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
} // LISTO (?)

void mostrarJugada(tipoCarta *carta, tipoJugador *jugador) {

  puts("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  printf("El jugador %s jugó\n", jugador->nombre);
  if (carta->especial == false) {
    printf(" [ %d,", carta->numero);
    printf(" %s ]\n", carta->color);
  } else {
    if (carta->numero < 13) {

      printf(" [ %s,", carta->color);
      printf(" %s ]\n", carta->tipo);
    } else {
      printf(" [ %s ]\n", carta->tipo);
    }
  }
  puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

bool hayCarta(tipoPartida *juego, tipoJugador *jugadorActual) {

  tipoCarta *aux = (tipoCarta *)malloc(sizeof(tipoCarta));

  aux = firstList(jugadorActual->mano);

  while (aux != NULL) {
    if (esValida(juego, aux) == true)
      return true;

    aux = nextList(jugadorActual->mano);
  }

  return false;
}

tipoCarta *seleccionarCarta(tipoPartida *juego, tipoJugador *jugadorActual,
                            int carta) {

  tipoCarta *aux = (tipoCarta *)malloc(sizeof(tipoCarta));
  ;

  while (1) {
    aux = firstList(jugadorActual->mano);

    for (int a = 1; a < carta; a++) {
      aux = nextList(jugadorActual->mano);
    }
    popCurrent(jugadorActual->mano);
    return aux;
  }
}

tipoCarta *seleccionarCartaAutomatico(tipoPartida *juego,
                                      tipoJugador *jugadorActual) {

  tipoCarta *aux = (tipoCarta *)malloc(sizeof(tipoCarta));

  aux = firstList(jugadorActual->mano);

  while (aux != NULL) {
    if (esValida(juego, aux) == true) {
      popCurrent(jugadorActual->mano);
      return aux;
    }

    aux = nextList(jugadorActual->mano);
  }
  return aux;
}

int validarColor(char *color) {
  //  entrada con color valido
  if (strcmp(color, "Rojo") == 0 || strcmp(color, "Azul") == 0 ||
      strcmp(color, "Verde") == 0 || strcmp(color, "Amarillo") == 0) {
    return 1; // valido
  }
  return 0; // invalido
} // LISTO

void printBool(bool value) { printf("%s\n", value ? "true" : "false"); }

void validarUNO(tipoJugador *jugador) {
  if (jugador->numCartas == 1) {
    printf("UNO\n");
  }
}

bool validarGanador(tipoJugador *jugador) {
  if (is_empty(jugador->mano) == true)
    return true;
  return false;
}

bool jugarCarta(tipoPartida *juego, tipoJugador *jugadorActual,
                List *listaJuego) {

  char color[20];
  mostrarTop(juego->ultimaJugada);
  mostrarMano(jugadorActual);

  tipoCarta *cartaAJugar = (tipoCarta *)malloc(sizeof(tipoCarta));
  bool jugo = false;
  bool pasar = false;
  int numero, carta;

  // Para la automatizacion de color
  char *colores[] = {"Rojo", "Azul", "Verde", "Amarillo"};

  if (jugadorActual->numJugador == 1) {

    do {
      printf("\n¿Tienes carta valida? 1.- Si / 2.- No, robo carta (pasas "
             "turno).\n");
      scanf("%d", &numero);
      while (getchar() != '\n')
        ;

      if (numero == 2) {
        pasar = true;
      }

      if (numero == 1) {

        jugo = hayCarta(juego, jugadorActual);

        if (jugo) {

          puts("Selecciona la posicion de tu carta en el mazo\n");
          scanf("%d", &carta);
          cartaAJugar = seleccionarCarta(juego, jugadorActual, carta);

          if (!esValida(juego, cartaAJugar)) {
            jugo = false;
            puts("\nCarta seleccionada invalida.\n");
          }
        }
      }

    } while (jugo == false && pasar == false);

  } else {
    carta = jugadorActual->numCartas;
    jugo = hayCarta(juego, jugadorActual);

    if (jugo == true) {
      cartaAJugar = seleccionarCartaAutomatico(juego, jugadorActual);
    }
  }

  if (jugo == true) {

    juego->ultimaJugada = cartaAJugar;
    mostrarJugada(juego->ultimaJugada, jugadorActual);
    jugadorActual->numCartas--;
    validarUNO(jugadorActual);
    if (validarGanador(jugadorActual) == true)
      return true;

    if (cartaAJugar->especial) {
      /*
  Cancelar turno : 10
  Invertir turno : 11
  +2 : 12
  +4 : 13
  Cambiar color : 14
  */
      switch (cartaAJugar->numero) {
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
          for (int f = 0; f < 2; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }
        } else {
          jugadorActual = nextList(listaJuego);
          for (int f = 0; f < 2; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }
        }
        break;

      case 13:
        if (juego->reversa) {
          jugadorActual = prevList(listaJuego);
          for (int f = 0; f < 4; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }
        } else {
          jugadorActual = nextList(listaJuego);
          for (int f = 0; f < 4; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }
        }
        if (jugadorActual->numJugador == 1) {
          printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): ");
          scanf("%s", color);

          while (!validarColor(color)) {
            printf("El color ingresado '%s' es inválido.\n", color);
            printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
            scanf("%s", color);
          }
        } else {
          strcpy(color, colores[rand() % 4]);
        }

        juego->ultimaJugada->numero = -1;
        strcpy(juego->ultimaJugada->color, color);

        break;

      case 14:
        if (jugadorActual->numJugador == 1) {
          printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
          scanf("%s", color);
          while (!validarColor(color)) {
            printf("El color ingresado '%s' es inválido.\n", color);
            printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
            scanf("%s", color);
          }
        } else {
          strcpy(color, colores[rand() % 4]);
        }
        juego->ultimaJugada->numero = 0;
        strcpy(juego->ultimaJugada->color, color);
        break;
      }
    }
  } else {
    tipoCarta *carta = crearCarta();
    pushBack(jugadorActual->mano, carta);
    jugadorActual->numCartas++;
  }
  return false;
}

int main(void) {

  srand(time(0)); // Funcion para asignar numeros aleatorios a las cartas
  tipoPartida *juego = (tipoPartida *)malloc(sizeof(tipoPartida));
  juego->ultimaJugada = crearCartaMazo();
  List *listaJuego = createList();
  int opcion;
  printf("~~~~~~| BIENVENIDO AL MENU DE UNO |~~~~~~\n\n");

  while (1) {

    menuTexto(&opcion);

    switch (opcion) {

    case 1:
      printf("\n--------------------------------------------\n");
      tipoPartida *juego = (tipoPartida *)malloc(sizeof(tipoPartida));
      juego->ultimaJugada = crearCartaMazo();
      List *listaJuego = createList();
      int cont = 0;
      iniciarPartiada(juego, listaJuego);
      tipoJugador *jugadorActual;
      jugadorActual = firstList(listaJuego);
      bool ganador = false;
      while (1) {
        ganador = jugarCarta(juego, jugadorActual, listaJuego);
        if (ganador == true) {

          break;
        }
        if (juego->reversa == true) {
          jugadorActual = prevList(listaJuego);

        } else {
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
