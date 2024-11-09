
/*
 * Laura Zafra Alarcos
 */



/*

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "scd.h" // Esta biblioteca nos permite tener semaforos en c++


using namespace std ;
using namespace scd ; //Hay que poner esto para tener una semantica correcta en los semaforos

//**********************************************************************
// Variables globales
const unsigned
   num_items = 40 ,   // número de items
	tam_vec   = 10 ;   // tamaño del buffer
unsigned
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido.
   cont_cons[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha consumido.
   siguiente_dato       = 0 ;  // siguiente dato a producir en 'producir_dato' (solo se usa ahí)

   //Variables + semáforos puestos por nosotros
   int buffer[tam_vec]; //Vector donde se almacenan los datos producidos
   int primera_libre = 0; //Indice en buffer de la primera celda libre, se incrementa al escribir y se decrementa al leer


    Semaphore libres = tam_vec;  //inicializado a número de entradas libres del buffer
    Semaphore ocupados = 0; //inicializado a número de entradas ocupadas del buffer
    Semaphore puede_acceder = 1;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato()
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   const unsigned dato_producido = siguiente_dato ;
   siguiente_dato++ ;
   cont_prod[dato_producido] ++ ;
   cout << "producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "consumido: " << dato << endl ;

}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora(  )
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   { //lo de dentro del for es nuestro

        int dato = producir_dato() ;

       //sección crítica
        sem_wait(libres);

        sem_wait(puede_acceder);

        buffer[primera_libre] = dato;
        primera_libre++;

        sem_signal(puede_acceder);

        sem_signal(ocupados);

   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora(  )
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   { //lo de dentro del for es nuestro
        int dato  ; //es una variable local, cada hebra va a tener su copia (no hay problemas de EM)

        //sección crítica
        sem_wait(ocupados);

        sem_wait(puede_acceder);

        primera_libre--;
        dato = buffer[primera_libre];

        sem_signal(puede_acceder);

        sem_signal(libres);

        consumir_dato( dato );
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "-----------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución LIFO)." << endl
        << "------------------------------------------------------------------" << endl
        << flush ;

   thread hebra_productora ( funcion_hebra_productora ),
          hebra_consumidora( funcion_hebra_consumidora );

   hebra_productora.join() ;
   hebra_consumidora.join() ;

   test_contadores();
}

*/
