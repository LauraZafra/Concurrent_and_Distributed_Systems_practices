//
///*
// * Laura Zafra Alarcos
// */
//
//#include <iostream>
//#include <cassert>
//#include <thread>
//#include <mutex>
//#include <random>
//#include "scd.h"
//
//using namespace std ;
//using namespace scd ;
//
////**********************************************************************
//// Variables y constantes globales del programa concurrente
//const unsigned
//   m = 30 ,   // numero total de items
//	tam_vec   = 2 ,   // capacidad del almacen
//   num_prods=3, // Número de hebras productoras
//   num_cons=2, // Número de hebras consumidoras
//   num_iter_prod=m/num_prods, // Numero de iteraciones productoras
//   num_iter_cons=m/num_cons; // Numero de iteraciones consunmidoras
//
//int almacen[tam_vec];
//int primera_libre=0, primera_ocupada = 0;
//
//Semaphore prod_puede = tam_vec,cons_puede=0;
//
//mutex mutex_pila;
//mutex mutex_lectura, mutex_escritura;
//
//
////**********************************************************************
//unsigned producir_dato(const int num_prod,  const int j)
//{
//   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
//   int result= num_prod*num_iter_prod+j;
//   cout << "Productor "<<num_prod<<" produce: " << result << endl << flush ;
//   return result;
//}
//
////----------------------------------------------------------------------
//void consumir_dato( const int num_consum, unsigned dato )
//{
//   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
//
//   cout << "                  Consumidor "<<num_consum<<" consume:  " << dato << endl<<flush ;
//
//}
//
////----------------------------------------------------------------------
//
//void  funcion_hebra_productora(const int num_prod )
//{
//   for( unsigned j = 0 ; j < num_iter_prod; j++ )
//   {
//      int dato = producir_dato(num_prod, j) ;
//
//      sem_wait(prod_puede);
//
//      // ESCRITURA LIFO
//      /*mutex_pila.lock();
//        almacen[primera_libre] = dato;
//        primera_libre++;
//      mutex_pila.unlock();*/
//
//
//
//      // ESCRITURA FIFO
//      mutex_escritura.lock();
//        almacen[primera_libre] = dato;
//        primera_libre  = (primera_libre + 1) % tam_vec;
//      mutex_escritura.unlock();
//
//
//      sem_signal(cons_puede);
//   }
//}
//
////----------------------------------------------------------------------
//
//void funcion_hebra_consumidora( const int num_consum)
//{
//   for( unsigned j = 0 ; j < num_iter_cons ; j++ )
//   {
//      int dato ;
//
//      sem_wait(cons_puede);
//
//      // LECTURA LIFO
//      /*mutex_pila.lock();
//        primera_libre--;
//        dato = almacen[primera_libre];
//      mutex_pila.unlock(); */
//
//      // LECTURA FIFO
//      mutex_lectura.lock();
//        dato = almacen[primera_ocupada];
//        primera_ocupada = (primera_ocupada + 1) % tam_vec;
//      mutex_lectura.unlock();
//
//      sem_signal(prod_puede);
//
//      consumir_dato(num_consum,  dato ) ;
//    }
//}
////----------------------------------------------------------------------
//
//int main()
//{
//   cout << "******************************************************************" << endl
//        << "**********Productores-consumidores (solucion FIFO )***************" << endl
//        << "******************************************************************" << endl
//        << flush ;
//
//   thread hebra_productora[num_prods],hebra_consumidora[num_cons];
//   for(unsigned i=0;i<num_prods;i++){
//      hebra_productora[i]=thread(funcion_hebra_productora,i);
//   }
//   for(unsigned i=0;i<num_cons;i++){
//      hebra_consumidora[i]=thread(funcion_hebra_consumidora,i);
//   }
//
//   for(unsigned i=0;i<num_prods;i++){
//      hebra_productora[i].join();
//   }
//   for(unsigned i=0;i<num_cons;i++){
//      hebra_consumidora[i].join();
//   }
//
//
//}
//
