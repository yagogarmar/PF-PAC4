#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>    
#include <mutex>
#include <random>    // Biblioteca para generación aleatoria avanzada

using namespace std;

mutex consoleMutex;

void moverCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicion;
    posicion.X = x;
    posicion.Y = y;
    SetConsoleCursorPosition(hConsole, posicion);
}

class Participante {
private:
    string nombre;
    int velocidad;
    bool pausa;
    bool victoria;
    int posicion;
    int fila;

public:
    Participante(string p_nombre, int p_velocidad, int p_fila)
        : nombre(p_nombre), velocidad(p_velocidad), pausa(false),
          posicion(0), victoria(false), fila(p_fila) {}

    void mostrarPos() {
        lock_guard<mutex> lock(consoleMutex);
        moverCursor(posicion, fila);
        cout << nombre[0];
    }

    void eliminarAnterior() {
        lock_guard<mutex> lock(consoleMutex);
        moverCursor(posicion, fila);
        cout << "-";
    }

    bool getVictoria() const { return victoria; }
    void setVictoria(bool p_vict) { victoria = p_vict; }

    void avanzar() {
        if (posicion >= 100) {
            victoria = true;
        } else {
            posicion += velocidad;
        }
    }

    string getName() const { return nombre; }
};

// Participantes
Participante animal1("Gato", 5, 0);
Participante animal2("Perro", 5, 1);
Participante animal3("Tortuga", 2, 2);

void avanzarAnimal(Participante& animal, int minDelay, int maxDelay) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(minDelay, maxDelay);

    while (!animal1.getVictoria() && !animal2.getVictoria() && !animal3.getVictoria()) {
        int time = distrib(gen);  // Genera un tiempo aleatorio en el rango especificado
        animal.eliminarAnterior();
        animal.avanzar();
        animal.mostrarPos();
        this_thread::sleep_for(chrono::milliseconds(time));
    }
}

int main() {
    system("cls");
    cout << "-----------------------------------------------------------------------------------------------------|-----" << endl;
    cout << "-----------------------------------------------------------------------------------------------------|-----" << endl;
    cout << "-----------------------------------------------------------------------------------------------------|-----" << endl;

    // Iniciar hilos para cada animal
    thread hiloA1(avanzarAnimal, ref(animal1), 200, 1500);
    thread hiloA2(avanzarAnimal, ref(animal2), 200, 1500);
    thread hiloA3(avanzarAnimal, ref(animal3), 200, 1500);

    hiloA1.join();
    hiloA2.join();
    hiloA3.join();

    cout << endl << endl << endl;

    if (animal1.getVictoria()) {
        cout << "El ganador es el " << animal1.getName();
    } else if (animal2.getVictoria()) {
        cout << "El ganador es el " << animal2.getName();
    } else {
        cout << "El ganador es el " << animal3.getName();
    }

    cout << endl;

    return 0;
}
