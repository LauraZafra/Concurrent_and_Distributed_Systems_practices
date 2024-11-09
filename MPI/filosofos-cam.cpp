// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-cam.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// -----------------------------------------------------------------------------

/*
Laura Zafra Alarcos
GIIADE
--------------------------------
Corregido por el profesor
*/

/*
Si todos los filosofos cogen primero el tenedor izquierdo o el derecho, hay interbloqueo: camarero para evitarlo
*/

#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>
#include <assert.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_filosofos = 5 ,              // número de filósofos 
   num_filo_ten  = 2*num_filosofos, // número de filósofos y tenedores 
   num_procesos  = num_filo_ten + 1,   // número de procesos total (filo, ten y camarero)
   id_camarero = num_filo_ten; // el camarero tiene el id n_filo + n_ten

//Etiquetas constantes
const int etiq_quiero_sentarme = 0;
const int etiq_quiero_levantarme = 1;


//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

// ---------------------------------------------------------------------
/*
En cada iteración del bucle un filósofo realiza repetidamente estas acciones:
1. Sentarse <-- camarero coordina: envío síncrono al camarero
2. Tomar los tenedores (primero el tenedor izquierdo y después el derecho).
3. Comer (bloqueo de duración aleatoria).
4. Soltar tenedores (el orden da igual).
5. Levantarse <-- camarero coordina: envío síncrono al camarero
6. Pensar (bloqueo de duración aleatoria).
*/
void funcion_filosofos( int id ) //esta función cambia respecto a la versión original
{
  int id_ten_izq = (id+1)              % num_filo_ten, //id. tenedor izq.
      id_ten_der = (id+num_filo_ten-1) % num_filo_ten, //id. tenedor der.
      valor; //variable auxiliar
      //MPI_Status estado ;

  while ( true )
  {

    //SENTARSE
    cout <<"Filósofo " << id << " solicita sentarse" << endl; 
    //digo al camarero que quiero sentarme
    MPI_Ssend (&valor, 0, MPI_INT, id_camarero, etiq_quiero_sentarme, MPI_COMM_WORLD);
    //solo continúo si el camarero lo ha recibido: las condiciones de la mesa lo permiten

    cout <<"Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
    // ... solicitar tenedor izquierdo (completar)
    MPI_Ssend (&valor, 0, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);
    //MPI_Ssend( void* buf_emi, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm ); <-- sintaxis

    cout <<"Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
    // ... solicitar tenedor derecho (completar)
    MPI_Ssend (&valor, 0, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

    cout <<"Filósofo " <<id <<" comienza a comer" <<endl ;
    sleep_for( milliseconds( aleatorio<10,100>() ) );

    cout <<"Filósofo " <<id <<" suelta ten. izq. " <<id_ten_izq <<endl;
    // ... soltar el tenedor izquierdo (completar)
    MPI_Ssend (&valor, 0, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

    cout<< "Filósofo " <<id <<" suelta ten. der. " <<id_ten_der <<endl;
    // ... soltar el tenedor derecho (completar)
    MPI_Ssend (&valor, 0, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

    //LEVANTARSE
    cout <<"Filósofo " << id << " solicita levantarse" << endl; 
    //digo al camarero que quiero levantarme
    MPI_Ssend (&valor, 0, MPI_INT, id_camarero, etiq_quiero_levantarme, MPI_COMM_WORLD);
    //solo continúo si el camarero lo ha recibido: las condiciones de la mesa lo permiten

    cout << "Filosofo " << id << " comienza a pensar" << endl;
    sleep_for( milliseconds( aleatorio<10,100>() ) );
 }
}
// ---------------------------------------------------------------------
/*
Cada proceso tenedor ejecuta un bucle infinito, al inicio de cada iteración está libre, y da estos dos pasos:
1. Espera hasta recibir un mensaje de cualquier filósofo, al recibirlo el tenedor pasa a estar ocupado por ese filósofo.
2. Espera hasta recibir un mensaje del filósofo que lo adquirió en el paso anterior. Al recibirlo, pasa a estar libre.
*/
void funcion_tenedores( int id )
{
  int valor, id_filosofo ;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {
      // ...... recibir petición de cualquier filósofo (completar)
      MPI_Recv (&valor, 0, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado); //nos vale cualquier filósofo
      // ...... guardar en 'id_filosofo' el id. del emisor (completar)
      id_filosofo = estado.MPI_SOURCE;
      cout <<"Ten. " <<id <<" ha sido cogido por filo. " <<id_filosofo <<endl;

      // ...... recibir liberación de filósofo 'id_filosofo' (completar)
      MPI_Recv (&valor, 0, MPI_INT, id_filosofo, 0, MPI_COMM_WORLD, &estado); //tiene que liberar el filósofo que ha ocupado
      cout <<"Ten. "<< id<< " ha sido liberado por filo. " <<id_filosofo <<endl ;
  }
}

// ---------------------------------------------------------------------
/*
Cada proceso filosofo pide permiso para sentarse o levantarse al camarero mediante un envío síncrono (Ssend):
Debemos implementar espera selectiva: camarero acepta solicitudes para sentarse en función del uso de los tenedores
1. Llevará una cuenta (s) del número de filósofos sentados.
2. Solo cuando s < 4 aceptará las peticiones de sentarse. 
3. Siempre aceptará las peticiones para levantarse.
*/
void funcion_camarero() //esta función se añade respecto a la versión original
{
    static int s = 0; //nº de filósofos sentados en la mesa (pueden estar usando tenedores o no)
    int valor; //auxiliar, no nos importa lo que valga
    MPI_Status estado ; // metadatos de las recepciones
    int etiq_aceptable;

    while ( true )
    {
        //Al camarero le llegan peticiones para sentarse o para levantarse: ¿cómo las distinguimos? usando etiquetas
        //Nos llega la petición que puede ser de cualquier filósofo con cualquier intención: identificamos la intención con la etiqueta

        if (s < 4){ //si la mesa está vacía no voy a recibir peticiones para levantarse, no hace falta contemplar ese caso
            etiq_aceptable = MPI_ANY_TAG;
        } else { //si la mesa está llena (s == 4) solo recibo peticiones para levantarse
            etiq_aceptable = etiq_quiero_levantarme;
        }
        
        MPI_Recv (&valor, 0, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado);

        //Marco mesa como ocupada / liberada
        if (estado.MPI_TAG == etiq_quiero_sentarme){
            assert (s < 4); 
            s++; //añado un comensal al contador
            cout << "El fiósofo " << estado.MPI_SOURCE << " se ha sentado" << endl;
        } 
        else if (estado.MPI_TAG == etiq_quiero_levantarme) {
            assert (s >= 0);
            s--; //quito un comensal del contador
            cout << "El fiósofo " << estado.MPI_SOURCE << " se ha levantado" << endl;
        } 
    }
}

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( id_propio == 0 ) // solo el primero escribe titulo, indep. del rol
      {
         cout << "----------------------------------------------------------------------" << endl;
         cout << "PROBLEMA CENA DE LOS FILÓSOFOS CON CAMARERO CON MPI (PASO DE MENSAJES)" << endl;
         cout << "----------------------------------------------------------------------" << endl;
      } 


   if ( num_procesos == num_procesos_actual )
   {
      // ejecutar la función correspondiente a 'id_propio'
      if (id_propio == id_camarero) // si soy el camarero
        funcion_camarero();
      else if ( id_propio % 2 == 0 )          // si es par
         funcion_filosofos( id_propio ); //   es un filósofo
      else                             // si es impar
         funcion_tenedores( id_propio ); //   es un tenedor
   }
   
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
