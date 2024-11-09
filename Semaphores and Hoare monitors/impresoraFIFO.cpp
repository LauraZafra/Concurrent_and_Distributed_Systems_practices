//
///*
// * Laura Zafra Alarcos
// */
//
//
////
//// Created by Laura Zafra Alarcos on 12/10/23.
////
////NO ESTA COMPROBADO QUE ESTÉ BIEN: NO CORREGIDO POR EL PROFESOR
//
//#include <iostream>
//#include <cassert>
//#include <thread>
//#include <random> // dispositivos, generadores y distribuciones aleatorias
//#include <chrono> // duraciones (duration), unidades de tiempo
//#include "scd.h"
//
//using namespace std;
//using namespace scd;
//
////**********************************************************************
//// Variables globales
//const unsigned
//num_items = 40 ,   // número de items
//tam_vec = 10,   // tamaño del buffer
//divisor = 5;
//
//unsigned
//        cont_prod[num_items] = {
//        0 } , // contadores de verificación: para cada dato, número de veces que se ha producido.
//cont_cons[num_items] = { 0 } , // contadores de verificación: para cada dato, número de veces que se ha consumido.
//siguiente_dato = 0;  // siguiente dato a producir en 'producir_dato' (solo se usa ahí)
//
////Variables + semáforos añadidos
//int buffer[tam_vec]; //Vector donde se almacenan los datos producidos
//int primera_libre = 0; //Indice en buffer de la primera celda libre, se incrementa al escribir
//int primera_ocupada = 0;  //Indice en buffer de la primera celda ocupada, se decrementa al leer
//int n_celdas_ocupadas = 0; //Número de celdas ocupadas en el buffer en cada momento, se actualiza en inserción y extracción
//
//Semaphore libres = tam_vec;
//Semaphore ocupados = 0;
//Semaphore contador_celdas = 1; //control de acceso a la variable n_celdas_ocupadas, funciona igual que un lock
//Semaphore impresora = 0; //debe bloquearse hasta que sea desbloqueada por otra hebra
//Semaphore productor = 0; //regula la producción para no ser interrumpida por la impresora
//Semaphore acceso_buffer = 1; //regula los accesos lectura/escritura en el buffer
//
////**********************************************************************
//// funciones comunes a las dos soluciones (fifo y lifo)
////----------------------------------------------------------------------
//
//unsigned producir_dato () {
//    this_thread::sleep_for (chrono::milliseconds (aleatorio<20 , 100> ()));
//    const unsigned dato_producido = siguiente_dato;
//    siguiente_dato ++;
//    cont_prod[ dato_producido ] ++;
//    cout << "producido: " << dato_producido << endl << flush;
//    return dato_producido;
//}
////----------------------------------------------------------------------
//
//void consumir_dato (unsigned dato) {
//    assert(dato < num_items);
//    cont_cons[ dato ] ++;
//    this_thread::sleep_for (chrono::milliseconds (aleatorio<20 , 100> ()));
//
//    cout << "consumido: " << dato << endl;
//
//}
//
//
////----------------------------------------------------------------------
//
//void test_contadores () {
//    bool ok = true;
//    cout << "comprobando contadores ....";
//    for ( unsigned i = 0 ; i < num_items ; i ++ ) {
//        if ( cont_prod[ i ] != 1 ) {
//            cout << "error: valor " << i << " producido " << cont_prod[ i ] << " veces." << endl;
//            ok = false;
//        }
//        if ( cont_cons[ i ] != 1 ) {
//            cout << "error: valor " << i << " consumido " << cont_cons[ i ] << " veces" << endl;
//            ok = false;
//        }
//    }
//    if ( ok )
//        cout << endl << flush << "solución (aparentemente) correcta." << endl << flush;
//}
//
////----------------------------------------------------------------------
//
//void funcion_hebra_productora () {
//    for ( unsigned i = 0 ; i < num_items ; i ++ ) { //lo de dentro del for es nuestro
//
//        int dato = producir_dato ();
//
//        //Acceso de los índices del buffer
//        sem_wait (libres);
//        sem_wait (acceso_buffer);
//        buffer[ primera_libre ] = dato;
//        primera_libre ++;
//        primera_libre = primera_libre % tam_vec; //nos aseguramos de no irnos fuera del vector
//        sem_signal (acceso_buffer);
//
//        //Acceso a contador celdas ocupadas (se actualiza justo después de producir)
//        sem_wait (contador_celdas);
//        n_celdas_ocupadas ++;
//        //cout << "Celdas ocupadas (añadido): " << n_celdas_ocupadas << endl; <-- para testear
//
//
//        //Si dato es múltiplo de 5, llamamos a impresora
//        if (dato % divisor == 0){
//            sem_signal (impresora);
//            sem_wait (productor);
//        }
//
//        sem_signal (contador_celdas); //incrementar el contador y esperar a que la impresora lo imprima
//        sem_signal (ocupados);
//
//    }
//}
//
////----------------------------------------------------------------------
//
//void funcion_hebra_consumidora () {
//    for ( unsigned i = 0 ; i < num_items ; i ++ ) { //lo de dentro del for es nuestro
//        int dato;
//
//        //Acceso de los índices del buffer
//        sem_wait (ocupados);
//        sem_wait (acceso_buffer);
//        dato = buffer[ primera_ocupada ];
//        primera_ocupada ++;
//        primera_ocupada = primera_ocupada % tam_vec; //nos aseguramos de no irnos fuera del vector
//        sem_signal (acceso_buffer);
//        sem_signal (libres);
//
//        consumir_dato (dato);
//
//        //Acceso a contador celdas ocupadas (se actualiza justo después de consumir)
//        sem_wait (contador_celdas);
//        n_celdas_ocupadas --;
//        //cout << "Celdas ocupadas (liberado): " << n_celdas_ocupadas << endl; <-- para testear
//        sem_signal (contador_celdas);
//    }
//}
//
//
//void funcion_hebra_impresora (){
//    for (int i = 0; i < num_items/divisor; i ++){ //con 40 items y 5 como divisor se ejecuta 8 veces
//        //Espera a ser desbloqueada
//        sem_wait(impresora);
//
//        //Muestra número celdas ocupadas, con semáforos nos aseguramos de que no se sobreescriba mientras leemos
//        //sem_wait (contador_celdas); no hace falta porque ya estamos dentro del lock contador_celdas
//        cout << "Celdas ocupadas: " << n_celdas_ocupadas << endl;
//        //sem_signal (contador_celdas); no hace falta porque ya estamos dentro del lock contador_celdas
//
//        sem_signal(productor);
//    }
//}
//
////----------------------------------------------------------------------
//
//int main () {
//    cout << "-----------------------------------------------------------------" << endl
//         << "Problema de los productores-consumidores IMPRESORA (solución FIFO)." << endl
//         << "------------------------------------------------------------------" << endl
//         << flush;
//
//    thread hebra_productora (funcion_hebra_productora) ,
//            hebra_consumidora (funcion_hebra_consumidora),
//            hebra_impresora (funcion_hebra_impresora);
//
//    hebra_productora.join ();
//    hebra_consumidora.join ();
//    hebra_impresora.join();
//
//    test_contadores ();
//}
//
//
//
