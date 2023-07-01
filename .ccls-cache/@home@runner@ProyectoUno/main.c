#include "list.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAXCHAR 20
#define barrita "\n================================"
#define barrita2 "\n-------------------------------"
#define barrita3 "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
#define DELAY 45
#define texto1 "\nEl jugador "
#define texto2 " jugo\n"


typedef struct {
  int numero;
  bool especial;
  char color[9];
  char tipo[15];
/*Cancelar turno : 10
  Invertir turno : 11
  +2 : 12
  +4 : 13
  Cambiar color : 14*/
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

// mostrarLento y mostrarVariableLento son funciones para que imprimas caracteres mas lento.
void mostrarLento(const char* texto, unsigned int retraso) {
  for (const char* c = texto; *c != '\0'; c++) {
    putchar(*c);
    fflush(stdout);  // Para que el carácter se muestre inmediatamente en pantalla
    usleep(retraso * 1000);  // Retraso en milisegundos
  }
}

void mostrarVariableLento(const char* texto, const char* variable, unsigned int retraso) {
    printf("%s%s", texto, variable);
    fflush(stdout);
    usleep(retraso * 1000);
}

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
  puts("¡Bienvenido a UNO Solitario: el juego de cartas en el que podrás "
       "disfrutar de manera individual este popular juego!\n\n");
  puts("El objetivo del juego es simple: quedarse sin cartas en la mano.\n");
  puts("A CONTINUACION TE MOSTRAREMOS LAS REGLAS DEL JUEGO:\n");
  puts("1. Cada jugador recibe 7 cartas al comienzo del juego.\n");
  puts("2. Las cartas pueden ser de cuatro colores: rojo, azul, verde y amarillo.\n");
  puts("3. Las cartas numéricas tienen un número del 0 al 9 y deben coincidir "
       "en color o número con la carta superior del mazo. \n");
  puts("4. Las cartas especiales tienen acciones adicionales:\n");
  puts("   - Cambio de sentido: invierte el orden de juego.\n"
       "   - Salto de turno: salta al siguiente jugador.\n"
       "   - +2: obliga al siguiente jugador a robar dos cartas y perder su "
       "turno.\n"
       "   - +4 y cambio de color: obliga al siguiente jugador a robar cuatro "
       "cartas y cambiar el color del juego.\n"
       "   - Cambio de color: Puedes cambiar el color del Mazo. \n");
  puts("5. Si te quedas sin cartas validas, debes robar una carta. "
       "del mazo.\n");
  puts("6. Si tienes una carta válida pero decides no jugarla "
       "estratégicamente, puedes hacerlo.\n");
  puts("7. El juego continúa hasta que un jugador se quede sin cartas en la "
       "mano.\n");
  puts("¡Diviértete jugando UNO Solitario!\n");

  puts(" !!! Ten en cuenta que los demás jugadores seguirán las reglas "
       "automáticamente  ¡¡¡ ");
}
/*Esta función tomará una variable tipo carta, se asignará un número 
aleatorio,dependiendo el número se asignará si es especial,y finalmente asignar 
el color a la carta.
*/
tipoCarta *crearCarta() {

  tipoCarta *carta = (tipoCarta *)malloc(sizeof(tipoCarta));
  carta->especial = false;
  carta->numero = rand() % 15;
  
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
} 

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
/*Esta función tomará la carta que querrá usar el usuario y la comparara con la 
variable tipoCarta “ultimaJugada”, compara las variables, según las condiciones la 
función retorna TRUE o FALSE y dirá si es válida o no.*/
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
/*: Esta función se encargará de realizar el comienzo del juego, junto a ello hacer 
varias acciones llamando diversas funciones,primero se creará la lista de los 
jugadores,al crear el perfil de un jugador se le asignan sus cartas que serán hechas por la
función crearCarta, Con esa misma función se asignará la ultimaJugada y finalmente asignar  el sentido.*/
void iniciarPartiada(tipoPartida *juego, List *listaJuego) {

  for (int i = 0; i < 4; i++) {

    tipoJugador *jugadorAux = (tipoJugador *)malloc(sizeof(tipoJugador));

    jugadorAux->numCartas = 7;

    if (i == 0) {
      char nombreAux[20];
      printf("Indica tu usuario: ");
      scanf("%s", nombreAux);


        while (getchar() != '\n') ;
  
        jugadorAux->numJugador = 1;
        strcpy(jugadorAux->nombre, nombreAux);
    }
    
    if (i == 1) {
      strcpy(jugadorAux->nombre, "MR.HUNDOR");
      jugadorAux->numJugador = 2;
    }
      if (i == 2) {
        strcpy(jugadorAux->nombre, "CLAUDIO TOLEDO");
        jugadorAux->numJugador = 3;
    }
      if(i == 3){
        strcpy(jugadorAux->nombre, "VICENTE MEDIANO");
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
/* Esta función  toma la lista de la mano del jugador y se mostraran sus cartas 
para que las pueda ver y saber si tienes cartas válidas para jugar.*/
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
} 
// En esta función se muestra la última carta jugada sobre la mesa
void mostrarTop(tipoCarta *carta) {

  puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
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
} 
/*si se juega una carta en el turno,la muestra indicando tambien el jugador*/
void mostrarJugada(tipoCarta *carta, tipoJugador *jugador) {
  
  mostrarLento(barrita3, DELAY);
  mostrarLento(texto1, DELAY);
  mostrarLento(jugador->nombre, DELAY);
  mostrarLento(texto2, DELAY);

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
  mostrarLento(barrita3, DELAY);
  printf("\n");
  if (jugador->numJugador == 4){
    printf("Si la consola se detiene, presione enter para continuar.\n");
    while (getchar() != '\n');
  }
}
/*La función Hay carta tomara la lista de la  mano del jugador actual y la recorre,si encuentra 
una carta válida verificada por la función esValida, retorna TRUE y se podrá jugar el turno, sino 
retorna FALSE y se saltara el turno del jugador actual.*/
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
/* En esta función el usuario ya habrá visto su mano y entonces tendrá que seleccionar la carta 
que va a querer jugar, indicando el número de la posición de la carta en la que se encuentra en su mano.*/
tipoCarta *seleccionarCarta(tipoPartida *juego, tipoJugador *jugadorActual) {

  tipoCarta *aux = (tipoCarta *)malloc(sizeof(tipoCarta));
  int numCarta;

  puts("Selecciona la posicion de tu carta en el mazo:");
  scanf("%d", &numCarta);

  while (numCarta > jugadorActual->numCartas) {
    puts("\nIngrese un numero dentro del rango.");
    scanf("%d", &numCarta);
  }

  while (1) {
    aux = firstList(jugadorActual->mano);

    for (int a = 1; a < numCarta; a++) {
      aux = nextList(jugadorActual->mano);
    }
    popCurrent(jugadorActual->mano);
    return aux;
  }
}
/*Esta función se encarga de seleccionar la carta de los jugadores virtuales en caso que tengan
para poder jugar, eso ya estará verificado por la función hayCarta.*/
tipoCarta *seleccionarCartaAutomatico(tipoPartida *juego,tipoJugador *jugadorActual) {

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
/*valida el color cuando hay q cambiaro*/
int validarColor(char *color) {
  //  entrada con color valido
  if (strcmp(color, "Rojo") == 0 || strcmp(color, "Azul") == 0 ||
      strcmp(color, "Verde") == 0 || strcmp(color, "Amarillo") == 0) {
    return 1; // valido
  }
  return 0; // invalido
} 

//Esta función hará que cuando un jugador tenga 1 carta, imprimirá por pantalla que el dicho jugador tiene una carta.
void validarUNO(tipoJugador *jugador) {
  if (jugador->numCartas == 1) {
    char char1[20];
    char char2[20];
    char char3[20];

    strcpy(char1, "\nATENCION MESA:\n");
    strcpy(char2, "EL JUGADOR [ ");
    strcpy(char3, " ] GRITO UNO.\n");
    
    mostrarLento(char1, DELAY);
    mostrarVariableLento(char2, jugador->nombre, DELAY);
    mostrarLento(char3, DELAY);

    printf("Si la consola se detiene, presione enter para continuar.\n");
    while(getchar() != '\n');
    
  }
}
/*Esta función luego de que el jugador juegue su carta, se verificará que  la lista de 
su mano sea nula y dictará como ganador el jugador actual y terminará la partida*/
bool validarGanador(tipoJugador *jugador) {
  if (is_empty(jugador->mano) == true) {

    /*Efecto de poner borrar el historial de lña consola y poner el texto de
    ganador en la esquina superior.*/
    
    printf("\033[2J\033[H");
    puts("\n!!! Winner Winner Chicken Dinner ¡¡¡\n");
    printf("> El jugador %s ha ganado la partida. <\n\n", jugador->nombre);
    puts("!!! Winner Winner Chicken Dinner ¡¡¡");

    return true;
  }
  return false;
}
/*Esta función se encarga de tomar la carta seleccionada por el jugador actual y realizar la acción de 
dicha carta si es especial o no, incluyendo también el caso que no hallan cartas jugables y el jugador 
pase de turno.Tambien si un jugador se le acaban las cartas,va a retornar true y termina el juego,sino
retorna false y se sigue jugando*/
bool jugarCarta(tipoPartida *juego, tipoJugador *jugadorActual, List *listaJuego) {

  char color[20];
  mostrarTop(juego->ultimaJugada);

  if (jugadorActual->numJugador == 1)
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

        // Meti dentro del hayCarta el scanf del N° de la carta
        jugo = hayCarta(juego, jugadorActual);

        if (jugo) {

          cartaAJugar = seleccionarCarta(juego, jugadorActual);

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

      switch (cartaAJugar->numero) {
      case 10:
        if (juego->reversa)
          jugadorActual = prevList(listaJuego);

        else
          jugadorActual = nextList(listaJuego);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("%s pierde su turno\n",jugadorActual->nombre);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        break;

      case 11:
        if (juego->reversa)
          juego->reversa = false;
        else
          juego->reversa = true;
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("Se cambia el sentido del juego\n");
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("%s roba 2 cartas\n y pierde su turno\n",jugadorActual->nombre);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        break;

      case 13:
        if (juego->reversa) {

          jugadorActual =
              prevList(listaJuego); // Retrocede un jugador en la lista

          for (int f = 0; f < 4; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }

        } else {
          jugadorActual = nextList(listaJuego); // Avanza un jugador en la lista

          for (int f = 0; f < 4; f++) {
            tipoCarta *carta = crearCarta();
            pushBack(jugadorActual->mano, carta);
            jugadorActual->numCartas++;
          }
        }
        // Se queda guardado el jugadorActual con el nodo del jugador siguiente

        if (juego->reversa) {

          jugadorActual = nextList(listaJuego);
          if (jugadorActual->numJugador == 1) {

            printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): ");
            scanf("%s", color);

            while (!validarColor(color)) {
              printf("El color ingresado '%s' es inválido.\n", color);
              printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
              scanf("%s", color);
            }
            jugadorActual = prevList(listaJuego);

          } else {
            strcpy(color, colores[rand() % 4]);
            jugadorActual = prevList(listaJuego);
          }

        } else {
          jugadorActual = prevList(listaJuego);
          if (jugadorActual->numJugador == 1) {

            printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): ");
            scanf("%s", color);

            while (!validarColor(color)) {
              printf("El color ingresado '%s' es inválido.\n", color);
              printf("Ingrese un color (Rojo, Azul, Verde, Amarillo): \n");
              scanf("%s", color);
            }
            jugadorActual = nextList(listaJuego);

          } else {
            strcpy(color, colores[rand() % 4]);
            jugadorActual = nextList(listaJuego);
          }
        }
        juego->ultimaJugada->numero = -1;
        strcpy(juego->ultimaJugada->color, color);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("%s roba 4 cartas\ny pierde su turno\n",jugadorActual->nombre);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("cambio de color a %s\n",color);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("%s cambio el color a %s\n",jugadorActual->nombre,color);
        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        break;
      }
    }
  } else {
    tipoCarta *carta = crearCarta();
    pushBack(jugadorActual->mano, carta);
    jugadorActual->numCartas++;
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("%s roba 1 carta\ny pasa de turno\n",jugadorActual->nombre);
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  }
  return false;
}

int main(void) {

  srand(time(0)); // Funcion para asignar numeros aleatorios a las cartas
  tipoPartida *juego = (tipoPartida *)malloc(sizeof(tipoPartida));
  juego->ultimaJugada = crearCartaMazo();
  List *listaJuego = createList();
  int opcion;
  printf("~~~~~~| BIENVENIDO AL MENU DE UNO |~~~~~~\n");

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

      free(juego);
      free(listaJuego);

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
    opcion=-1;
  }
  return 0;
}
