/*
 * Laura Zafra Alarcos
 */


/*

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

mutex mx;

constexpr int 
	escritores = 5,
	lectores = 20,
	min_ms = 5,
	max_ms = 20;
int a;

class Lectores_Escritores : public HoareMonitor{
	private:
	bool escribiendo;
	int nlectores;
    int cuanto_leo;

	CondVar lectura,
		escritura;

	public:
	Lectores_Escritores();
	void lectura_ini();
	void lectura_fin();
	void escritura_ini();
	void escritura_fin();
};


Lectores_Escritores::Lectores_Escritores(){ //constructor
	escribiendo = false;
	lectura = newCondVar();
	escritura = newCondVar();
	nlectores = 0;
	cuanto_leo = 3;

}

void Lectores_Escritores::lectura_ini(){
	
	if (escribiendo || cuanto_leo == 0 ) { //se tienen que cumplir las dos condiciones (&&) para continuar
		lectura.wait();
	}
	
	nlectores++;
	if (!escritura.empty()){ //empty devuelve true si no hay hebras esperando, o false si hay al menos una
		cuanto_leo--; //si hay al menos una hebra esperando en la CondVar escritura esperando
	}
	if (cuanto_leo > 0){
		lectura.signal();
	}

}
void Lectores_Escritores::lectura_fin(){
	nlectores--;

	if (nlectores == 0){
		cuanto_leo = 3;
		escritura.signal();
	}

}

void Lectores_Escritores::escritura_ini(){
	if (escribiendo || nlectores > 0){
		escritura.wait();
	}
	escribiendo = true;
}

void Lectores_Escritores::escritura_fin(){
	escribiendo = false;
	if (!lectura.empty()){ //si hay al menos una hebra esperando en lectura
		lectura.signal();
	}
	else {
		escritura.signal(); //si no hay hebras esperando para leer, seguimos escriniendo
	}
}



void funcion_hebra_escritora(MRef<Lectores_Escritores> monitor, int i){
	while (true){
		monitor->escritura_ini();
		
		mx.lock();
		cout << "Escritor: " << i << " escribiendo" << endl;
		cout << "Escritor: " << i << " termina de escribir" << endl;
		mx.unlock();
		chrono::milliseconds duracion(aleatorio < min_ms, max_ms > ());
		this_thread::sleep_for(duracion);

		monitor->escritura_fin();
		
		
	}	
	
}
void funcion_hebra_lectora(MRef<Lectores_Escritores> monitor, unsigned i){
	while (true){
		monitor->lectura_ini();
		
		mx.lock();
		cout << "Lector: " << i << " leyendo" << endl;
		cout << "Lector: " << i << " termina de termina de leer" << endl;
		
		mx.unlock();
		chrono::milliseconds duracion(aleatorio < min_ms, max_ms > ());
		this_thread::sleep_for(duracion);
		monitor->lectura_fin();
		
		
	}	
	
}



int main()
{
	MRef<Lectores_Escritores> monitor = Create<Lectores_Escritores>();
	
	thread hebras_escritoras[escritores],
	       hebras_lectoras[lectores];
	
	for (int i = 0; i < escritores; i++){
		hebras_escritoras[i] = thread(funcion_hebra_escritora,monitor,i);
	}
	
	for (int i = 0; i < lectores; i++){
		hebras_lectoras[i] = thread(funcion_hebra_lectora,monitor,i);
	}
	
	for (int i = 0; i < escritores; i++){
		hebras_escritoras[i].join();
	}
	
	for (int i = 0; i < lectores; i++){
		hebras_lectoras[i].join();
	}
	
	
	
	
}

*/
