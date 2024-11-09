/*
 * Laura Zafra Alarcos
 */


/*


#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

// numero de fumadores 

const int num_fumadores = 3 ;

//Declarar el array de semaforos
Semaphore ingredientes[num_fumadores]={0, 0,0};
Semaphore mostrador_vacio = 1;

//-------------------------------------------------------------------------
// Función que simula la acción de producir un ingrediente, como un retardo
// aleatorio de la hebra (devuelve número de ingrediente producido)

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
    int i;
    while (true){
        i = producir_ingrediente();
        sem_wait (mostrador_vacio);

        cout << "En el mostrador hay ingrediente " << i << endl;

        sem_signal(ingredientes[i]);
    }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
   while( true )
   {
       sem_wait(ingredientes[num_fumador]);
       cout << "Retira el ingrediente " << num_fumador << endl;
       sem_signal (mostrador_vacio);
       fumar(num_fumador);
   }
}

//----------------------------------------------------------------------

int main()
{
   // declarar hebras y ponerlas en marcha
   // ......

   //Crear hebras
    thread hebra_estanquero ( funcion_hebra_estanquero );
    thread hebra_fumador_0 ( funcion_hebra_fumador, 0 ); //Tiene papel y tabaco, le falta cerillas
    thread hebra_fumador_1 ( funcion_hebra_fumador, 1 ); //Tiene papel y cerillas, le falta tabaco
    thread hebra_fumador_2 ( funcion_hebra_fumador, 2 ); //Tiene cerillas y tabaco, le falta papel

    //Unimos las hebras
    hebra_estanquero.join();
    hebra_fumador_0.join();
    hebra_fumador_1.join();
    hebra_fumador_2.join();
}*/
