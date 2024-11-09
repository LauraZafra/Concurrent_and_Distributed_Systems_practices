
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
   min_ms    = 5,     // tiempo minimo de espera en sleep_for
   max_ms    = 20 ;   // tiempo máximo de espera en sleep_for

mutex    mtx ;                 // mutex de escritura en pantalla

const int num_fum=3; // Número de hebras fumadoras
const int tam_mostrador = 2;


//-------------------------------------------------------------------------
int Producir_ingrediente()
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
   
   mtx.lock();
   cout << "Estanquero : EMPIEZA a producir ingrediente." << endl;
   mtx.unlock();

   const int ingrediente = aleatorio<0,num_fum-1>() ;

   mtx.lock();
   cout << "Estanquero : TERMINA de producir ingrediente " << ingrediente << endl;
   mtx.unlock();

   return ingrediente ;
}


//-------------------------------------------------------------------------
void Fumar( int num_fumador )
{
   mtx.lock();
   cout << "                                                Fumador " << num_fumador << ":"<< " EMPIEZA a fumar" << endl;
   mtx.unlock();
  
   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
   
   mtx.lock();
   cout << "                                                Fumador " << num_fumador << " TERMINA DE FUMAR!!" << endl;
   mtx.unlock();
}

//----------------------------------------------------------------------


// *****************************************************************************
// clase para monitor Estanco para el problema de los fumadores
class Estanco : public HoareMonitor
{
 private:

 int  mostrador[tam_mostrador];   //   Estado del mostrador: da el ingrediente que hay en el mostrador


 CondVar                    // colas condicion:
   mostrador_vacio,         //  cola donde espera el estanquero (mostrador[i] = -1)
   esta_mi_ingred[num_fum];       //  cola donde espera cada fumador


 public:                    // constructor y métodos públicos
   Estanco() ;             // constructor
   void ponerIngrediente( const int ingre_x, const int ingre_y);
   void esperarRecogidaIngrediente(); 
   void obtenerIngrediente( const int  i ); // i = fumador

   
} ;
// -----------------------------------------------------------------------------

Estanco::Estanco( )
{
   // Mostrador inicialmente vacio
   for (int i = 0; i < tam_mostrador; ++i){
       mostrador[i] = -1;
   }
   mostrador_vacio   = newCondVar();
   for( unsigned i = 0 ; i < num_fum ; i++ ) esta_mi_ingred[i] = newCondVar();

}



// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato
void Estanco::ponerIngrediente( const int ingre_x, const int ingre_y)
   {
      if (mostrador[0] != -1 || mostrador[1] != -1) mostrador_vacio.wait();
      mostrador[0] = ingre_x;
      esta_mi_ingred[ingre_x].signal();

       mostrador[1] = ingre_y;
       esta_mi_ingred[ingre_y].signal();

   }
   
 void Estanco::esperarRecogidaIngrediente() //el estanquero espera mostrador totalmente vacío para poner un ingred nuevo
  {    
      if (mostrador[0] != -1 || mostrador[1] != -1) mostrador_vacio.wait();
   }


void Estanco::obtenerIngrediente( const int  i )
 {
    if (mostrador[0] != i && mostrador[1] != i) //si el ingrediente no está en el mostrador esperamos
        esta_mi_ingred[i].wait();

     assert(mostrador[0] == i || mostrador[1] == i);

     //buscamos en qué parte del mostrador está y lo retiramos
     if (mostrador[0] == i){
         mostrador[0] = -1;
         cout << "Fumador " << i << " recoge ingrediente" << endl << flush;
     }
     if (mostrador[1] == i){
         mostrador[1] = -1;
         cout << "Fumador " << i << " recoge ingrediente" << endl << flush;
     }

     //si el mostrador está vacío lo indicamos
     if (mostrador[0] == -1 && mostrador[1] == -1){
         //cout << "MOSTRADOR VACíO" << endl << flush;
         mostrador_vacio.signal();
     }
}
   
   // -----------------------------------------------------------------------------


// *****************************************************************************
// funciones de hebras
// *****************************************************************************

// -----------------------------------------------------------------------------
void funcion_estanquero( MRef<Estanco> monitor)
{
    int x, y;
    while (true){
        x = Producir_ingrediente();
        do {
            y = Producir_ingrediente();
        } while (x == y);

        cout << "ESTANQUERO HA PRODUCIDO LOS INGREDIENTES " << x << " y " << y << endl << flush;

        monitor->esperarRecogidaIngrediente();

        cout << "En el mostrador hay ingredientes  " << x << " y " << y << endl << flush;

        monitor->ponerIngrediente( x, y ); //en el mostrador
    }
}


// -----------------------------------------------------------------------------
void funcion_fumadora( MRef<Estanco>  monitor, const int i )
{
   while(true)
   {
     monitor->obtenerIngrediente( i ); //sacamos ingrediente y volvemos a dejar monitor vacío
     this_thread::sleep_for( chrono::milliseconds( aleatorio<20,150>() ));

     //Fumar( i ) ;


   }
}
 

// -----------------------------------------------------------------------------

int main()
{
  
   cout << "--------------------------------------------------------------------" << endl
        << "   Problema de los fumadores  (Monitor SU).                         " << endl
        << "--------------------------------------------------------------------" << endl
        << flush ;



   MRef<Estanco> monitor = Create<Estanco>() ;

   thread hebra_estanquero(funcion_estanquero, monitor),
          hebra_fum[num_fum]; //solo se crean

   for( unsigned i = 0 ; i < num_fum ; i++ ) //se inicializan
      hebra_fum[i]=thread ( funcion_fumadora, monitor, i );

   // esperar a que terminen las hebras
   for( unsigned i = 0 ; i < num_fum ; i++ )
      hebra_fum[i].join();

   hebra_estanquero.join();

}
*/