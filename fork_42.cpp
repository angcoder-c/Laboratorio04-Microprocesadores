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
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main() {
    int n;
    cout << "Ingresa la cantidad de procesos hijo a crear: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork not created");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Código del proceso hijo
            cout << "Child Process: " << i + 1 
                 << ", PID: " << getpid() 
                 << ", PPID: " << getppid() << endl;
            exit(EXIT_SUCCESS); // Importante: salir del hijo para no seguir creando más hijos
        }
        // El padre continúa el ciclo
    }

    // El padre espera a todos los hijos
    for (int i = 0; i < n; i++) {
        wait(nullptr);
    }

    cout << "Todos los procesos hijo han finalizado." << endl;
    return 0;
}
