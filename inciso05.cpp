#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

/*
* Angel Gabriel Chavez Otzoy
 * 24248
 * Programacion de microprocesadores
 * Laboratorio 04
 * Fecha: 05 / 08 / 2025

a) Crear tres procesos hijos desde el padre.
b) Cada hijo imprime un mensaje con su número cada segundo durante 3 segundos.
c) El padre no espera a los hijos; imprime un mensaje y finaliza.
 */
 using namespace std;

int main() {
 cout << "[PADRE] PID= "
 << getpid() << " PPID= "
 << getppid() << endl;

 // a) Crear tres procesos hijos desde el padre.
 for (int i = 1; i <= 3; i++) {
  pid_t pid = fork();

  if (pid == 0) {
   // PROCESO HIJO
   cout << "[HIJO-" << i << "] PID= " << getpid() << " PPID= " << getppid() << endl;

   // b) Cada hijo imprime un mensaje con su número cada segundo durante 3 segundos.
   for (int time = 1; time <= 3; time++) {
    sleep(1);
    cout << "[HIJO-" << i << "] Mensaje " << time << "/3 - PID= " << getpid() << endl;
   }
   cout << "[HIJO-" << i << "] Terminando - PID= " << getpid() << endl;
   exit(EXIT_SUCCESS);

  } else if (pid < 0) {
   perror("fork");
   exit(EXIT_FAILURE);

  } else {
   // c) El padre no espera a los hijos;
   cout << "[PADRE] Hijo " << i << " creado - PID= " << pid << endl;
   // wait(nullptr);
  }
 }

 // c) imprime un mensaje y finaliza.
 cout << "[PADRE] Hijos creados." << endl;
 cout << "[PADRE] Terminando - PID= " << getpid() << endl;

 return 0;
}