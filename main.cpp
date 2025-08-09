/**
 * Angel Gabriel Chavez Otzoy
 * 24248
 * Programacion de microprocesadores
 * Laboratorio 04
 * Fecha: 05 / 08 / 2025
 */

#include <iostream>
#include <string>
#include <cmath>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <tuple>
// para medir el tiempo de ejecucion
#include <ctime>

using namespace std;

// CODIGO EXTRAIDO DEL LABORATORIO 03
// https://github.com/angcoder-c/Laboratorio3-Microprocesadores/blob/main/main.cpp
// =========================================================================
int binOp2Int(string op) {
    // convertir un operador binario en entero
    int op_int = 0;
    for (int i = op.length()-1; i >=0; i--) {
        if (op[i] == '1') {
            // sumar al resultado, la potencia de dos correspondiente
            op_int += pow(2, op.length()-1-i);
        }
    }
    return op_int;
}

// READ INPUT
string getOpCode (string input) {
    // obtener el opcode en string
    return input.substr(0,3);
}

bool opInvert (string input) {
    // determinar si la operacion es invertida
    if (input[3]=='1') {
        return true;
    }
    return false;
}

int getOp1 (string input) {
    // obtener el operdor 1
    return binOp2Int(input.substr(4,2));
}

int getOp2 (string input) {
    // obtener el operador 2
    return binOp2Int(input.substr(6,2));
}

// VALIDACION DE ENTRADA

bool validarInput (string input) {
    // evaluar que el imput sea valido
    bool valid = false;
    if (input.length()!=8) {
        return false;
    }

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '1' || input[i] == '0') {
            valid = true;
        } else {
            valid = false;
        }
    }
    return valid;
}

tuple<int, string> applyOps (string opcode, int op1, int op2, bool invert) {
    /*
    000 → Suma
    001 → Resta
    010 → Multiplicación
    011 → División entera
    101 → Potencia (A^B)
    110 → A mod B
     */
    int result = 0;
    if (opcode == "000") {
        return tuple<int, string>(op1 + op2, "EXITOSO");
    } else if (opcode == "001") {
        if (invert) {
            result = op2 - op1;
        } else {
            result = op1 - op2;
        }
        return tuple<int, string>(result, "EXITOSO");;
    } else if (opcode == "010") {
        return tuple<int, string>(op1 * op2, "EXITOSO");
    } else if (opcode == "011") {
            if (invert) {
                if (op1 == 0) {
                    return tuple<int, string>(NULL, "ERROR DIVISION POR CERO");
                } else {
                    return tuple<int, string>(op2 / op1, "EXITOSO");
                }
            } else {
                if (op2 == 0) {
                    return tuple<int, string>(NULL, "ERROR DIVISION POR CERO");
                } else {
                    return tuple<int, string>(op1 / op2, "EXITOSO");
                }
            }
    } else if (opcode == "101") {
        if (invert) {
            result = pow(op2, op1);
        } else {
            result = pow(op1, op2);
        }
        return tuple<int, string>(result, "EXITOSO");
    } else if (opcode == "110") {
        if (invert) {
            result = op2 % op1;
        } else {
            result = op1 % op2;
        }
        return tuple<int, string>(result, "EXITOSO");
    }
    return tuple<int, string>(NULL, "ERROR OPERACION NO SOPORTADA");
}

vector<string> split(string s, char delim) {
    // separa el input por espacios en blanco
    vector<string> tokens;
    string token;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delim) {
            tokens.push_back(token);
            token.clear();
        } else if (i+1 == s.length()) {
            token += s[i];
            tokens.push_back(token);
            token.clear();
        }
        else {
            token += s[i];
        }
    }
    return tokens;
}
// =========================================================================


string processInstruction (string input, clock_t init) {
    if (!validarInput(input)) {
        return "ERROR ENTRADA INVALIDA";
    }
    // OBTENER PARAMETROS DE LA INSTRUCCION DE 8 BITS
    string opcode = getOpCode(input);
    bool invert = opInvert(input);
    int op1 = getOp1(input);
    int op2 = getOp2(input);
    tuple<int, string> result = applyOps(opcode, op1, op2, invert);
    clock_t end = clock();
    return "===================="
            "\nINSTRUCCION: "
            + input
            + "\nTIEMPO DE EJECUCION: "
            + to_string(((end - init)*1000)/CLOCKS_PER_SEC)
            + "ms"
            + "\nPROCESO: "
            + to_string(getpid())
            + "\nOPCODE: "
            +  opcode
            + "\nINVERTIDO: "
            + to_string(invert)
            + "\nOP1: "
            + to_string(op1)
            + "\nOP2: "
            + to_string(op2)
            + "\nRESULTADO: "
            + to_string(std::get<0>(result))
            + "\nMENSAJE: "
            + std::get<1>(result)
            + "\n====================";
}

// PROCESO PRINCIPAL
int main() {
    string input_str;
    char inmenu = 'x';

    while (inmenu != '0') {
        cout << "\nMENU\n(1) Evaluar operacion\n(0) Salir\n>>>  ";
        cin >> inmenu;
        cin.ignore();
        if (inmenu == '1') {
            cout << "Input: ";
            getline(cin, input_str);
            vector<string> input = split(input_str, ' ');

            for (int i = 0; i < input.size(); i++) {
                // PROCESOS HIJOS
                pid_t pid = fork();
                clock_t init = clock();

                if (pid < 0) {
                    perror("fork not created");
                    exit(EXIT_FAILURE);

                } else if (pid == 0) {
                    // VALIDAR INSTRUCCION
                    if (validarInput(input[i])) {

                        // OBTENER PARAMETROS DE LA INSTRUCCION DE 8 BITS
                        string result = processInstruction(input[i], init);
                        cout << result << endl;

                    } else {
                        cout << "ERROR ENTRADA INVALIDA" << endl;
                    }
                    exit(EXIT_SUCCESS);
                }
            }

            // ESPERAR A LOS PROCESOS HIJOS
            for (int i = 0; i < input.size(); i++) {
                wait(nullptr);
            }
        }

        if (inmenu == '0') {
            cout << "Bye" << endl;
        }
    }
    return 0;
}