//
// Created by Laura Zafra Alarcos on 15/11/23.
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
        num_items = 40 ; // número de items a producir/consumir //m
int
        siguiente_dato = 0 ; // siguiente valor a devolver en 'producir_dato'

constexpr int
min_ms = 5, // tiempo minimo de espera en sleep_for
max_ms = 20 ; // tiempo máximo de espera en sleep_for

mutex mtx ; // mutex de escritura en pantalla
unsigned cont_prod[num_items] = {0}, // contadores de verificación: producidos
cont_cons[num_items] =  {0}; // contadores de verificación: consumidos

//MULTIPLES: igual que en semaforos
// numero ejemplo np y nc
const int numproductores=10, //np 10
numconsumidores=4; //nc
//cada productor produce p= 40/10 = 4 items
const int p=num_items/numproductores;
//cada consumidor consume c= 40/4 = 10 items
const int c=num_items/numconsumidores;

//array compartido con n p entradas
//que indique, en cada momento, para cada hebra productora, //cuantos items ha producido ya:
int vproductos[numproductores];
//inicializar vector:
void iniciclizarVprod(){
    for(int i=0; i<numproductores; i++){ vproductos[i]=0;
    }
}

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
// ----------------------------------------------------------------------
int producir_dato( unsigned productor )
{
    assert( productor >= 0 && productor <= (numproductores-1) ); ///
    assert(productor<numproductores);
    this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
    const int valor_producido =productor*p + vproductos[productor];//= siguiente_dato ; //mu //siguiente_dato ++ ;
    mtx.lock();
    cout << "hebra productora " << productor << ", produce " << valor_producido << endl <<
         flush ;
    mtx.unlock();
    cont_prod[valor_producido]++ ;
    vproductos[productor]++;//vector compartido que cuenta cuántos productos ha producido
    //cada productor //Mu
    return valor_producido ;
}

//----------------------------------------------------------------------
void consumir_dato( unsigned valor_consumir, unsigned consumidor ) {
    assert(consumidor >= 0 && consumidor <= (numconsumidores - 1)); ///
    assert(consumidor < numconsumidores);//
    if (num_items <= valor_consumir) {
        cout << " valor a consumir === " << valor_consumir << ", num_items == "
             << num_items << endl;
        assert(valor_consumir < num_items);
    }
    cont_cons[valor_consumir]++;
    this_thread::sleep_for(chrono::milliseconds(aleatorio<min_ms, max_ms>()));
    mtx.lock();
    cout << " hebra consumidora" << consumidor << ", consume: " <<
         valor_consumir << endl;
    mtx.unlock();
}

// *****************************************************************************
// clase para monitor buffer, version LIFO, semántica SC, multiples prod/cons
class ProdConsMu : public HoareMonitor {
private:
    static const int //constantes ('static' ya que no dependen de la instancia)
    num_celdas_total = 10; //núm. de entradas del buffer
    int //variables permanentes
    buffer[num_celdas_total], //buffer de tamaño fijo, con los datos
    primera_libre, //indice de celda de la próxima inserción (!=num de celdas ocupadas)
    primera_ocupada;

    CondVar
            ocupadas, // cola donde espera el consumidor (n>0)
    libres; // cola donde espera el productor (n<num_celdas_total)
public:
    ProdConsMu();

    int leer();

    void escribir(int valor);
};

ProdConsMu::ProdConsMu( ) {
    primera_libre = 0 ;
    ocupadas = newCondVar();
    libres = newCondVar();
}

// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato
int ProdConsMu::leer( ) {
// esperar bloqueado hasta que 0 < primera_libre
if ( primera_libre == 0 )
    ocupadas.wait();
    cout << "leer: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
    assert( 0 < primera_libre );
// hacer la operación de lectura, actualizando estado del monitor
primera_libre-- ;
    const int valor = buffer[primera_libre] ;
// señalar al productor que hay un hueco libre, por si está esperando
libres.signal();
// devolver valor
    return valor ;
}

// -----------------------------------------------------------------------------
void ProdConsMu::escribir( int valor ) {
// esperar bloqueado hasta que primera_libre < num_celdas_total
if ( primera_libre == num_celdas_total )
    libres.wait();
    cout << "escribir: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
    assert( primera_libre < num_celdas_total );
// hacer la operación de inserción, actualizando estado del monitor
buffer[primera_libre] = valor ;
    primera_libre++ ;
// señalar al consumidor que ya hay una celda ocupada (por si esta esperando)
    ocupadas.signal();
}

// *****************************************************************************
// funciones de hebras
void funcion_hebra_productora( MRef<ProdConsMu> monitor, int productor ) {
    for( unsigned i = 0 ; i < p ; i++ ) {
        int valor = producir_dato( productor ) ;
    monitor->escribir( valor );
}
}
// -----------------------------------------------------------------------------
void funcion_hebra_consumidora( MRef<ProdConsMu> monitor, int consumidor ) {
    for( unsigned i = 0 ; i < c ; i++ ){
        int valor = monitor->leer();
    consumir_dato( valor, consumidor ) ;
    }
}
// -----------------------------------------------------------------------------

int main() {
    iniciclizarVprod(); //
    cout << "--------------------------------------------------------------------" << endl <<
    "Problema del productor-consumidor múltiples (Monitor SU, buffer LIFO). " <<
         endl
         << "--------------------------------------------------------------------" << endl << flush ;
// crear monitor ('monitor' es una referencia al mismo, de tipo MRef<...>)
MRef<ProdConsMu> monitor = Create<ProdConsMu>() ;

    thread hebras_prod[numproductores], hebras_cons[numconsumidores];
    for(int i = 0; i < numproductores; i++){
        hebras_prod[i]=thread( funcion_hebra_productora, monitor,i );
    }
    for(int i = 0; i < numconsumidores; i++){
        hebras_cons[i] = thread( funcion_hebra_consumidora, monitor,i ); }
// esperar a que terminen las hebras
 for(int i = 0; i < numproductores; i++) hebras_prod[i].join();
    for(int i = 0; i < numconsumidores; i++) hebras_cons[i].join();
    cout << "fin" << endl;
    //test_contadores() ;
}
*/