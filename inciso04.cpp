/*
 * Angel Gabriel Chavez Otzoy
 * 24248
 * Programacion de microprocesadores
 * Laboratorio 04
 * Fecha: 05 / 08 / 2025

a) El proceso padre crea un hijo.
b) El hijo crea a su vez un “nieto”.
c) Cada proceso imprime su PID y el de su padre.
d) El nieto duerme 2 segundos y finaliza.
e) El hijo espera al nieto y luego termina.
f) El padre espera al hijo y luego finaliza.
 */

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

int main() {
    cout << "[PADRE] - PID: " << getpid()
    << ", PPID: " << getppid() << endl;

    // a. El proceso padre crea un hijo.
    pid_t hijo = fork();

    if (hijo == 0) {
        // PROCESO HIJO
        cout << "[HIJO] - PID: " << getpid()
        << ", PPID: " << getppid() << endl;

        // b) El hijo crea a su vez un "nieto"
        pid_t nieto = fork();

        if (nieto == 0) {
            // PROCESO NIETO
            // c) Cada proceso imprime su PID y el de su padre
            cout << "[NIETO] - PID: " << getpid()
            << ", PPID: " << getppid() << endl;

            // d) El nieto duerme 2 segundos y finaliza
            cout << "[NIETO] Durmiendo 2s." << endl;
            sleep(2);
            cout << "[NIETO] Terminando - PID: " << getpid() << endl;

            exit(EXIT_SUCCESS);

        } else if (nieto > 0) {
            // PROCESO HIJO
            cout << "[HIJO] Esperando al nieto - PID: " << nieto << endl;

            int status;
            pid_t terminated_child = wait(&status);

            // e) El hijo espera al nieto y luego termina
            cout << "[HIJO] El nieto - PID: " << terminated_child << " terminado con estado: " << status << endl;
            cout << "[HIJO] Terminando - PID: " << getpid() << endl;

        } else {
            // Error al crear nieto
            perror("[HIJO] fork para nieto fallido");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);

    } else if (hijo > 0) {
        // PROCESO PADRE
        cout << "[PADRE] Hijo creado con PID: " << hijo << endl;
        cout << "[PADRE] PID: " << getpid() << ", PPID: " << getppid() << endl;
        cout << "[PADRE] Esperando al hijo " << endl;

        int status;
        pid_t terminated_child = wait(&status);

        // f) El padre espera al hijo y luego finaliza
        cout << "[PADRE] El hijo - PID: " << terminated_child << " terminado con estado: " << status << endl;
        cout << "[PADRE] Terminando - PID: " << getpid() << endl;

    } else {
        // Error al crear hijo
        perror("[PADRE] fork para hijo fallido");
        exit(EXIT_FAILURE);
    }
    return 0;
}