//
// Created by Laura Zafra Alarcos on 15/11/23.
// desde impresora_resuelto quitando cosas
//

/*
 * Laura Zafra Alarcos
 */


/*

#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <thread>
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int
   num_items = 30 ;   // número de items a producir/consumir
int
   siguiente_dato = 0 ; // siguiente valor a devolver en 'producir_dato'

constexpr int
   min_ms    = 5,     // tiempo minimo de espera en sleep_for
   max_ms    = 20 ;   // tiempo máximo de espera en sleep_for

mutex    mtx ;                 // mutex de escritura en pantalla


 const int num_consumidoras=3, // Número de hebras consumidora
           num_productoras=2; // Número de hebras productoras


//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------



//**********************************************************************
unsigned producir_dato(const int num_hebra, int j)
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
   const int dato = num_hebra*num_items/num_productoras+j;
   mtx.lock();
   cout << "producido: " << dato << endl << flush ;
   mtx.unlock();
   return dato ;
}


//----------------------------------------------------------------------
void consumir_dato( const int num_hebra, unsigned dato )
{

   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
   mtx.lock();
   cout << "                  hebra consumidora " <<num_hebra<<" consume: " << dato << endl<<flush ;
   mtx.unlock();
}

//----------------------------------------------------------------------


// *****************************************************************************
// clase para monitor buffer, version FIFO, semántica SU, multiples prod/cons
class Buffer : public HoareMonitor
{
 private:
 static const int           // constantes ('static' ya que no dependen de la instancia)
   num_celdas_total = 5;   //   núm. de entradas del buffer
 int                        // variables permanentes
   buffer[num_celdas_total],//   buffer de tamaño fijo, con los datos
   primera_libre ,
   primera_ocupada,
   n;		     //   Contador de items disponibles para leer

 CondVar                    // colas condicion:
   ocupadas,                //  cola donde espera el consumidor (n>0)
   libres;                 //  cola donde espera el productor  (n<num_celdas_total)

 public:                    // constructor y métodos públicos
   Buffer() ;             // constructor
   int  leer();                // extraer un valor (sentencia L) (consumidor)
   void escribir( int valor ); // insertar un valor (sentencia E) (productor)
} ;
// -----------------------------------------------------------------------------

Buffer::Buffer(  )
{
   primera_libre = 0 ;
   primera_ocupada = 0;
   n = 0;
   ocupadas      = newCondVar();
   libres        = newCondVar();
}
// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato

int Buffer::leer(  )
{
   // esperar bloqueado hasta que 0 < n
   //Cambiamos la condicion para el funcionamiento correcto
   if ( n == 0 )
      ocupadas.wait();


   // hacer la operación de lectura, actualizando estado del monitor
   const int valor = buffer[primera_ocupada] ;
   primera_ocupada = (primera_ocupada + 1) % num_celdas_total;;
   n--;

   // señalar al productor que hay un hueco libre, por si está esperando
   libres.signal();

   // devolver valor
   return valor ;
}
// -----------------------------------------------------------------------------

void Buffer::escribir( int valor )
{
   // esperar bloqueado hasta que n < num_celdas_total
   //Cambiamos la condicion para el funcionamiento correcto
   if( n == num_items)
      libres.wait();

   // Insercion, actualizando estado del monitor
   buffer[primera_libre] = valor ;
   primera_libre = (primera_libre + 1) % num_celdas_total;
   n++;

   // Avisar de que ya hay una celda ocupada (por si esta esperando)
   ocupadas.signal();
}



// *****************************************************************************
// funciones de hebras
// *****************************************************************************

// -----------------------------------------------------------------------------
void funcion_hebra_productora( MRef<Buffer> monitor, const int num )
{
   for( unsigned j = 0 ; j < num_items/num_productoras ; j++ )
   {
      int dato = producir_dato(num,j) ;
      monitor->escribir( dato );
   }
}


// -----------------------------------------------------------------------------
void funcion_hebra_consumidora( MRef<Buffer>  monitor, const int num )
{
   for( unsigned j = 0 ; j < num_items/num_consumidoras ; j++ )
   {
      int valor = monitor->leer();
      consumir_dato( num,valor ) ;
   }
}

// -----------------------------------------------------------------------------

int main()
{

   cout << "--------------------------------------------------------------------" << endl
        << "   Problema de productores-consumidores  (Monitor SU, buffer FIFO).    " << endl
        << "--------------------------------------------------------------------" << endl
        << flush ;

   // crear monitor  ('monitor' es una referencia al mismo, de tipo MRef<...>)
   MRef<Buffer> monitor = Create<Buffer>() ;

   // crear y lanzar las hebras
   thread hebra_prod[num_productoras], hebra_cons[num_consumidoras];

   for( unsigned i = 0 ; i < num_productoras ; i++ )
      hebra_prod[i]=thread ( funcion_hebra_productora, monitor,i );

   for( unsigned i = 0 ; i < num_consumidoras ; i++ )
      hebra_cons[i]=thread (funcion_hebra_consumidora, monitor,i );


   // esperar a que terminen las hebras
   for( unsigned i = 0 ; i < num_productoras ; i++ )
      hebra_prod[i].join();
   for( unsigned i = 0 ; i < num_consumidoras ; i++ )
      hebra_cons[i].join();
}
*/

