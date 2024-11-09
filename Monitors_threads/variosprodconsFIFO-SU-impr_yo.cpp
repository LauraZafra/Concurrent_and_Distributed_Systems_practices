
/*
 * Laura Zafra Alarcos
 */


/*
//
// Created by Laura Zafra Alarcos on 13/11/23.
//

*/
/* ************************************************************************************************
 * Ejercicio P2.1 (Productores-consumidores múltiples, ver. FIFO, con impresora, con un monitor SU)
 * ************************************************************************************************
 * Copia tu solución FIFO al problema de los productores-consumidores con monitores SU (basado en
 * la clase HoareMonitor) en un archivo nuevo llamado p2_pc_impr.cpp, y extiéndelo para cumplir
 * estos requisitos adicionales a los del problema original:
 *
 *  (1) Asegúrate que el número total de items a producir sea múltiplo de 5 (además de múltiplo del
 *  número de productores y múltiplo del número de consumidores).
 *
 *  (2) Se debe crear una nueva hebra llamada impresora, que ejecuta un bucle. En cada iteración la
 *  hebra impresora debe llamar a un nuevo método del monitor llamado metodo_impresora. Ese método
 *  devuelve un valor lógico. El bucle de la hebra impresora acaba cuando ese método devuelve false.
 *
 *  (3) Si al inicio del método metodo_impresora la hebra impresora detecta que ya se han insertado en
 *  el buffer todos los múltiplos de 5 que se tenían que insertar, devuelve false, en otro caso
 *  (todavía quedan múltiplos de 5 por insertar), imprime un mensaje (se describe a continuación)
 *  y después devuelve true (ten en cuenta que en total se deben insertar T/5 múltiplos de 5, donde
 *  T es el numero total de items a producir por todos los productores).
 *
 *  (4) En el mensaje que la hebra impresora imprime (en metodo_impresora) se debe incluir el número
 *  total de nuevos múltiplos de 5 que se han insertado en el vector, contados desde la anterior llamada
 *  a metodo_impresora o desde el inicio de la simulación (si es la primera llamada). Si dicho número es
 *  cero, la hebra debe esperar bloqueada hasta que dicho número sea mayor que 0, antes de imprimir el
 *  mensaje (por tanto, nunca imprime un cero).
 *
 *  (5) Ten en cuenta que las hebras productoras son responsables de contabilizar la cantidad de
 *  múltiplos de 5 insertados y de desbloquear a la hebra impresora cuando sea necesario hacerlo,
 *  to-do ello debe gestionarse dentro del método escribir del monitor.
 * ************************************************************************************************
 *//*


#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <thread>
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int
        num_items = 30 ,
// número de items a producir/consumir, múltiplo de 5, de productores (2) y de consumidores (3)
        multiplo = 5;
int
        siguiente_dato = 0 ; // siguiente valor a devolver en 'producir_dato'

constexpr int
        min_ms    = 5,     // tiempo minimo de espera en sleep_for
max_ms    = 20 ;   // tiempo máximo de espera en sleep_for


mutex
        mtx ;                 // mutex de escritura en pantalla
unsigned
        cont_prod[num_items] = {0}, // contadores de verificación: producidos
cont_cons[num_items] = {0}; // contadores de verificación: consumidos

const int num_consumidoras=3, // Número de hebras consumidora
num_productoras=2; // Número de hebras productoras

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato(const int num_hebra, int j)
{

    this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
    const int valor_producido = siguiente_dato ;
    siguiente_dato ++ ;
    mtx.lock();
    cout << "hebra productora, produce " << valor_producido << endl << flush ;
    mtx.unlock();
    cont_prod[valor_producido]++ ;
    return valor_producido ;
}
//----------------------------------------------------------------------

void consumir_dato(const int num_hebra, unsigned valor_consumir )
{
    */
/*
    if ( num_items <= valor_consumir )
    {
        cout << " valor a consumir === " << valor_consumir << ", num_items == " << num_items << endl ;
        assert( valor_consumir < num_items );
    }
    *//*

    cont_cons[valor_consumir] ++ ;
    this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
    mtx.lock();
    cout << "                  hebra consumidora, consume: " << valor_consumir << endl ;
    mtx.unlock();
}
//----------------------------------------------------------------------

void test_contadores()
{
    bool ok = true ;
    cout << "comprobando contadores ...." << endl ;

    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        if ( cont_prod[i] != 1 )
        {
            cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
            ok = false ;
        }
        if ( cont_cons[i] != 1 )
        {
            cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
            ok = false ;
        }
    }
    if (ok)
        cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

// *****************************************************************************
// clase para monitor buffer, version FIFO, semántica SC, multiples prod/cons

class Buffer : public HoareMonitor
{
private:
    static const int           // constantes ('static' ya que no dependen de la instancia)
    num_celdas_total = 5;   //   núm. de entradas del buffer
    int                        // variables permanentes
    buffer[num_celdas_total],//   buffer de tamaño fijo, con los datos
    primera_libre,           //   indice de celda de la próxima inserción
    primera_ocupada,         //  indice de celda de la próxima extracción
    n,                       //  celdas ocupadas
    contador_multiplos,
    multiplos_impresos;
    CondVar                    // colas condicion:
    ocupadas,                //  cola donde espera el consumidor (n>0) <-- no se espera cuando
    libres,                 //  cola donde espera el productor  (n<num_celdas_total) <-- no se espera cuando
    impresora;
public:                    // constructor y métodos públicos / procedimientos exportados
    Buffer() ;             // constructor
    int  leer();                // extraer un valor (sentencia L) (consumidor)
    void escribir( int valor ); // insertar un valor (sentencia E) (productor)
    bool metodo_impresora ();
    void incContador ();
} ;
// -----------------------------------------------------------------------------

Buffer::Buffer(  ) //implementación del constructor
{
    primera_libre = 0 ;  //   indice de celda de la próxima inserción
    primera_ocupada=0;   //   indice de celda de la próxima extracción
    n=0;                 //   celdas ocupadas
    ocupadas      = newCondVar(); //  cola condición donde el consumidor espera a (n>0)
    libres        = newCondVar(); //  cola condición donde el productor espera a (n<num_celdas_total)
    impresora     = newCondVar();
    contador_multiplos = 0;
    multiplos_impresos = 0;
}
// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato

int Buffer::leer(  )
{
    // esperar bloqueado hasta que 0 < n
    if ( n == 0 ) ocupadas.wait();

    //cout << "leer: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
    assert( 0 < n  ); //no es necesario, por si acaso


    const int valor=buffer[primera_ocupada]; //orden FIFO
    primera_ocupada=(primera_ocupada+1)%num_celdas_total;
    n--;

    // señalar al productor que hay un hueco libre, por si está esperando
    libres.signal();

    // devolver valor
    return valor ;
}
// -----------------------------------------------------------------------------


*/
/*
 *  (5) Ten en cuenta que las hebras productoras son responsables de contabilizar la cantidad de
 *  múltiplos de 5 insertados y de desbloquear a la hebra impresora cuando sea necesario hacerlo,
 *  to-do ello debe gestionarse dentro del método escribir del monitor.
 *//*

void Buffer::escribir( int valor )
{
    // esperar bloqueado hasta que n < num_celdas_total
    if ( n == num_celdas_total ) libres.wait();

    //cout << "escribir: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
    assert( n < num_celdas_total );

    //¿Hay que llamar a impresora?
    if (valor % multiplo == 0){
        incContador();
        impresora.signal();
    }

    buffer[primera_libre] = valor ;
    primera_libre=(primera_libre+1)%num_celdas_total;
    n++;

    // señalar al consumidor que ya hay (al menos) una celda ocupada (por si esta esperando)
    ocupadas.signal();
}
// *****************************************************************************
*/
/*
 *  (3) Si al inicio del método metodo_impresora la hebra impresora detecta que ya se han insertado en
 *  el buffer todos los múltiplos de 5 que se tenían que insertar, devuelve false, en otro caso
 *  (todavía quedan múltiplos de 5 por insertar), imprime un mensaje (se describe a continuación)
 *  y después devuelve true (ten en cuenta que en total se deben insertar T/5 múltiplos de 5, donde
 *  T es el numero total de items a producir por todos los productores).
 *
 *  (4) En el mensaje que la hebra impresora imprime (en metodo_impresora) se debe incluir el número
 *  total de nuevos múltiplos de 5 que se han insertado en el vector, contados desde la anterior llamada
 *  a metodo_impresora o desde el inicio de la simulación (si es la primera llamada). Si dicho número es
 *  cero, la hebra debe esperar bloqueada hasta que dicho número sea mayor que 0, antes de imprimir el
 *  mensaje (por tanto, nunca imprime un cero).
 *//*

bool Buffer::metodo_impresora( )
{
    // esperar bloqueado hasta que se inserte un múltiplo de 5
    if (contador_multiplos == multiplos_impresos) impresora.wait();

    if (contador_multiplos == num_items/multiplo) return false; //si el contador ya ha llegado a 6
    else {
        multiplos_impresos++;
        cout << "Se han insertado en el vector " << contador_multiplos << " múltiplos de " << (num_items/multiplo) << endl;
        return true;
    }
}
// *****************************************************************************


void Buffer::incContador()
{
    contador_multiplos++;
}
// *****************************************************************************

// funciones de hebras

void funcion_hebra_productora( MRef<Buffer> monitor, const int num) //Monitor Reference, ProdConsSU1: nombre de la subclase
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int valor = producir_dato(num, i) ; //fuera del monitor, no necesita EM
        monitor->escribir( valor ); //escribir es un método del monitor
    }
}
// -----------------------------------------------------------------------------

void funcion_hebra_consumidora( MRef<Buffer>  monitor, const int num)
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int valor = monitor->leer();
        consumir_dato( num, valor ) ;
    }
}
// -----------------------------------------------------------------------------
*/
/*
*  (2) Se debe crear una nueva hebra llamada impresora, que ejecuta un bucle. En cada iteración la
*  hebra impresora debe llamar a un nuevo método del monitor llamado metodo_impresora. Ese método
*  devuelve un valor lógico. El bucle de la hebra impresora acaba cuando ese método devuelve false.
*//*

void funcion_hebra_impresora( MRef<Buffer>  monitor, const int num)
{
    bool valor = true;
    while (valor)
    {
        valor = monitor->metodo_impresora(); //tiene que estar con la flecha
    }
}
// -----------------------------------------------------------------------------

int main()
{
    cout << "--------------------------------------------------------------------" << endl
         << "Problema de varios productores-consumidores (impresora) (Monitor SU, buffer FIFO). " << endl
         << "--------------------------------------------------------------------" << endl
         << flush ;

    // crear monitor  ('monitor' es una referencia al mismo, de tipo MRef<...>)
    MRef<Buffer> monitor = Create<Buffer>() ; //   monitor es una referencia a una instancia de ProdConsSU1

    // crear y lanzar las hebras
    thread hebra_prod[num_productoras], hebra_cons[num_consumidoras], hebra_impresora;

    for( unsigned i = 0 ; i < num_productoras ; i++ )
        hebra_prod[i]=thread ( funcion_hebra_productora, monitor,i );

    for( unsigned i = 0 ; i < num_consumidoras ; i++ )
        hebra_cons[i]=thread (funcion_hebra_consumidora, monitor,i );

    hebra_impresora = thread (funcion_hebra_impresora, monitor, 1);

    // esperar a que terminen las hebras
    for( unsigned i = 0 ; i < num_productoras ; i++ )
        hebra_prod[i].join();
    for( unsigned i = 0 ; i < num_consumidoras ; i++ )
        hebra_cons[i].join();
    hebra_impresora.join();

    test_contadores() ;
}

*/
