//
// Created by Laura Zafra Alarcos on 20/12/23.
//

/*
¿cual es el mínimo tiempo de espera que queda al final de las iteraciones del ciclo secundario con tu solución?
El mínimo tiempo de espera teórico ocurre en el ciclo secundario 2, donde ocurre un tiempo de espera:
Tespera = 500 - 100 - 150 - 240 = 10ms 

¿sería planificable si la tarea D tuviese un tiempo cómputo de 250 ms?
Si tenemos en cuenta que el cambio de contexto entre procesos/hebras lleva un tiempo por mínimo que sea, no sería posible.
En este caso, en el ciclo secundario 2 que es cuando se ejecuta la tarea D, el tiempo que nos "sobra" hasta que empieza el 
tercer ciclo secundario es 0, y se produciría un retraso en la ejecución. Si queremos que la tarea D tenga un tiempo de 
cómputo >= 10, habrá que aumentar el tiempo de cada ciclo secundario, cambiando la planificación.
*/

#include <string>
#include <iostream> // cout, cerr
#include <thread>
#include <chrono>   // utilidades de tiempo
#include <ratio>    // std::ratio_divide

using namespace std ;
using namespace std::chrono ;
using namespace std::this_thread ;

// tipo para duraciones en segundos y milisegundos, en coma flotante:
//typedef duration<float,ratio<1,1>>    seconds_f ;
typedef duration<float,ratio<1,1000>> milliseconds_f ;

// -----------------------------------------------------------------------------
// tarea genérica: duerme durante un intervalo de tiempo (de determinada duración)

void Tarea( const std::string & nombre, milliseconds tcomputo )
{
    cout << "   Comienza tarea " << nombre << " (C == " << tcomputo.count() << " ms.) ... " ;
    sleep_for( tcomputo );
    cout << "fin." << endl ;
}

// -----------------------------------------------------------------------------
// tareas concretas del problema:

void TareaA() { Tarea( "A", milliseconds(100) );  }
void TareaB() { Tarea( "B", milliseconds(150) );  }
void TareaC() { Tarea( "C", milliseconds(200) );  }
void TareaD() { Tarea( "D", milliseconds(240) );  }

// -----------------------------------------------------------------------------
// implementación del ejecutivo cíclico:

int main( int argc, char *argv[] )
{
    // Ts = duración del ciclo secundario (en unidades de milisegundos, enteros)
    const milliseconds Ts_ms( 500 );

    // ini_sec = instante de inicio de la iteración actual del ciclo secundario
    time_point<steady_clock> ini_sec = steady_clock::now();

    while( true ) // ciclo principal
    {
        cout << endl
             << "---------------------------------------" << endl
             << "Comienza iteración del ciclo principal." << endl ;


        for( int i = 1 ; i <= 4 ; i++ ) // ciclo secundario (4 iteraciones)
        {
            cout << endl << "Comienza iteración " << i << " del ciclo secundario." << endl ;

            switch( i )
            {
                case 1 : TareaA(); TareaB(); TareaC(); 
                    break ;
                case 2 : TareaA(); TareaB(); TareaD(); 
                    break ;
                case 3 : TareaA(); TareaB(); TareaC();           
                    break ;
                case 4 : TareaA(); TareaB();          
                    break ;
            }

            // calcular el siguiente instante de inicio del ciclo secundario
            ini_sec += Ts_ms ;

            // esperar hasta el inicio de la siguiente iteración del ciclo secundario
            sleep_until( ini_sec );
            time_point<steady_clock> fin_sec = steady_clock::now(); steady_clock::duration duracion = fin_sec - ini_sec;
            cout << "Duración esperada: " << Ts_ms.count() << " milisegundos" << endl;
            cout << "Retraso: " << milliseconds_f(duracion).count() << " milisegundos" << endl;
        }
    }
}

