/*--------------------------------------------------------------------------- 
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERIA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
*
* Curso: CC3086 Programacion de Microprocesadores
* Actividad: Ejemplo Laboratorio 04
* Descripcion: Llamada al sistema para creacion de procesos hijos
------------------------------------------------------------------------------*/
#include <iostream>
#include <sys/wait.h>  // Solo disponible en UNIX
#include <unistd.h>    // Solo disponible en UNIX
#include <cstdlib>     // Para EXIT_SUCCESS y EXIT_FAILURE


using std::cout; 
using std::cin;
using std::endl;

int main() {
    int a; // Se agrega la variable 'a'
    cout << "Ingrese el valor de a: ";
    cin >> a; // Se ingresa por teclado justo despues de su declaracion

    pid_t num_pid;				
	num_pid = fork();
	
	// Si el hijo no se crea correctamente
    if (num_pid == -1) {
        perror("fork not created");
        exit(EXIT_FAILURE);
    } 
	// Si el proceso actual es el padre, el OS detecta un ID válido
	else if (num_pid > 0) {
        cout << "Printed from Parent Num. Process: " << getpid() << endl;
        
        a = a+5;
        cout << "a printed from Parent: " << a<< endl;
		
        wait(nullptr);
    } 
	// Si el proceso actual es en Child, el OS no detecta un ID válido, 
	//localmente ID = 0,  implica que ha sido creado desde otro proces
	else { // == 0
        cout << "Printed from Child Num. Process: " << getpid() << endl;
		
		a = a+10;
		cout << "a printed from Child: " << a << endl;

        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}

