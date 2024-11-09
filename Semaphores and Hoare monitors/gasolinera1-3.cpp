
/*
 * Laura Zafra Alarcos
 */


/*
//
// Created by Laura Zafra Alarcos on 13/10/23.
//

#include <iostream>
#include <cassert>
#include <thread>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std;
using namespace scd;

//**********************************************************************
// Constantes globales --> se cumple C<A y D<B
const unsigned A = 4; //nº coches diésel
const unsigned B = 6; //nº coches gasolina
const unsigned C = 3; //nº surtidores diésel
const unsigned D = 2; //nº surtidores gasolina

//**********************************************************************
//Semáforos
Semaphore surtidores_diesel = C;
Semaphore surtidores_gasolina = D;

//**********************************************************************
//Funciones circular y repostar

void circular (int i, string tipo) { //cuando los coches no están en la gasolinera
    this_thread::sleep_for (chrono::milliseconds (aleatorio<20 , 100> ()));
    cout << "Coche número " << i << " de " << tipo << " preparado para repostar." << endl << flush;
}

void repostar (int i, string tipo) {
    cout << "Coche número " << i << " de " << tipo << " comienza a repostar." << endl << flush;
    this_thread::sleep_for (chrono::milliseconds (aleatorio<20 , 100> ()));
    cout << "Coche número " << i << " de " << tipo << " termina de repostar." << endl << flush;
}

//**********************************************************************
//FUNCIONES HEBRAS con bucle infinito

void funcion_hebra_diesel (int n_coche){
    while (true){
        sem_wait (surtidores_diesel);
        repostar(n_coche, "diesel");
        sem_signal(surtidores_diesel);
        circular(n_coche, "diesel");
    }
}

void funcion_hebra_gasolina (int n_coche){
    while (true){
        sem_wait (surtidores_gasolina);
        repostar(n_coche, "gasolina");
        sem_signal(surtidores_gasolina);
        circular(n_coche, "gasolina");
    }
}

//**********************************************************************

int main () {
    cout << "-----------------------------------------------------------------" << endl
         << "Problema de la gasolinera." << endl
         << "------------------------------------------------------------------" << endl
         << flush; //El cout sale de inmediato del buffer, no espera

    //Crear e inicializar las 10 hebras
    thread hebras_coche_diesel [A];
    thread hebras_coche_gasolina [B];

    for (int i = 0; i < A; i++){
        hebras_coche_diesel[i] = thread (funcion_hebra_diesel, i); //poner con thread y sin paréntesis
    }
    for (int i = 0; i < B; i++){
        hebras_coche_gasolina[i] = thread (funcion_hebra_gasolina, i);
    }

    //Hacemos los 10 join
    for (int i = 0; i < A; i++){
        hebras_coche_diesel[i].join();
    }
    for (int i = 0; i < B; i++){
        hebras_coche_gasolina[i].join();
    }
}*/
