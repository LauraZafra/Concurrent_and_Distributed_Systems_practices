
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
   int buffer[num_items]; //Vector donde se almacenan los datos producidos


   const int N_PRODUCTORAS=4; // numero de hebras productores
   const int N_CONSUMIDORAS=4; //numero de hebras consumidores
   const unsigned NUM_ITEMS_PRODUCTOR = num_items/N_PRODUCTORAS; //tiene que ser divisor de num_items
   const unsigned NUM_ITEMS_CONSUMIDOR = num_items/N_CONSUMIDORAS; //tiene que ser divisor de num_items
   int primera_libre [N_PRODUCTORAS] = {0}; //Indice en buffer de la primera celda libre, se incrementa al escribir y se decrementa al leer
   int pos= 0; // posicion dentro del buffer

   Semaphore libres=tam_vec;
   Semaphore ocupados=0;
   Semaphore puede_acceder=1;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato( int n )
{
    this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

    const unsigned dato_producido = primera_libre[n] + n * NUM_ITEMS_PRODUCTOR ;
    primera_libre[n]++ ;
    cont_prod[dato_producido] ++ ;
    cout << "producido: " << dato_producido << endl << flush ;
    return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato, int n )
{
  // assert( dato < num_items );
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

void  funcion_hebra_productora(int n)
{
   for( unsigned i = 0 ; i < NUM_ITEMS_PRODUCTOR ; i++ )
   { //lo de dentro del for es nuestro

        int dato = producir_dato(n) ;

        sem_wait(libres);
        sem_wait(puede_acceder);

        buffer[pos] = dato;
        pos++;

        sem_signal(puede_acceder);
        sem_signal(ocupados);

   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora(int n)
{
   for( unsigned i = 0 ; i < NUM_ITEMS_CONSUMIDOR ; i++ )
   { //lo de dentro del for es nuestro
        int dato ;
        
        sem_wait(ocupados);
        sem_wait(puede_acceder);

        pos--;
        dato = buffer[pos];

        sem_signal(puede_acceder);
        sem_signal(libres);

        consumir_dato( dato, n);
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "-----------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución LIFO)." << endl
        << "------------------------------------------------------------------" << endl
        << flush ;

   thread hebra_productora[N_PRODUCTORAS],
          hebra_consumidora[N_CONSUMIDORAS];

   //Cuando se pasa una función como parámetro no se escriben los paréntesis
   //Se escribe el nombre de la función llamada y los parámetros de esta como parámetros de la función llamadora
   for (int i = 0; i < N_PRODUCTORAS; i++ ) {
       hebra_productora[i] = thread(funcion_hebra_productora, i );
   }

   for (int i = 0; i < N_CONSUMIDORAS; i++ ) {
        hebra_consumidora[i] = thread ( funcion_hebra_consumidora, i );
   }

   //Con el join aseguramos que las hebras acaben antes que el main
   for (int i = 0 ; i < N_PRODUCTORAS; i++ ) {
       hebra_productora[i].join();
   }

   for (int i = 0; i < N_CONSUMIDORAS; i++ ) {
        hebra_consumidora[i].join();
   }

   test_contadores();
}

*/
