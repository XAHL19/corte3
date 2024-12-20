#include <iostream>
#include <algorithm> // Para std::max
#include <iomanip>   // Para formatear la visualización
#include <cstdlib>   // Para malloc y free

using namespace std;

// Definición de la estructura del nodo
struct Nodo {
    int valor;
    int altura;
    Nodo* izq;
    Nodo* der;
};

// Función para crear un nuevo nodo usando malloc
Nodo* crearNodo(int val) {
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevoNodo) {
        nuevoNodo->valor = val;
        nuevoNodo->altura = 1;
        nuevoNodo->izq = NULL;
        nuevoNodo->der = NULL;
    }
    return nuevoNodo;
}

// Función para liberar un nodo
void liberarNodo(Nodo* nodo) {
    if (nodo) {
        free(nodo);
    }
}

// Función para obtener la altura de un nodo
int obtenerAltura(Nodo* nodo) {
    return nodo ? nodo->altura : 0;
}

// Función para calcular el factor de equilibrio
int obtenerBalance(Nodo* nodo) {
    return nodo ? obtenerAltura(nodo->izq) - obtenerAltura(nodo->der) : 0;
}

// Actualiza la altura de un nodo
void actualizarAltura(Nodo* nodo) {
    nodo->altura = 1 + max(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));
}

// Rotación simple a la derecha
Nodo* rotacionDerecha(Nodo* y) {
    Nodo* x = y->izq;
    Nodo* T2 = x->der;

    x->der = y;
    y->izq = T2;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

// Rotación simple a la izquierda
Nodo* rotacionIzquierda(Nodo* x) {
    Nodo* y = x->der;
    Nodo* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

// Función para insertar un nodo en el árbol AVL
Nodo* insertarNodo(Nodo* nodo, int valor) {
    if (!nodo) {
        return crearNodo(valor);
    }

    if (valor < nodo->valor) {
        nodo->izq = insertarNodo(nodo->izq, valor);
    } else if (valor > nodo->valor) {
        nodo->der = insertarNodo(nodo->der, valor);
    } else {
        return nodo;
    }

    actualizarAltura(nodo);
    int balance = obtenerBalance(nodo);

    if (balance > 1 && valor < nodo->izq->valor) {
        return rotacionDerecha(nodo);
    }
    if (balance < -1 && valor > nodo->der->valor) {
        return rotacionIzquierda(nodo);
    }
    if (balance > 1 && valor > nodo->izq->valor) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }
    if (balance < -1 && valor < nodo->der->valor) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

// Función para encontrar el nodo con el valor mínimo en un subárbol
Nodo* nodoMinimo(Nodo* nodo) {
    while (nodo->izq != NULL) {
        nodo = nodo->izq;
    }
    return nodo;
}

// Función para eliminar un nodo del árbol AVL
Nodo* eliminarNodo(Nodo* raiz, int valor) {
    if (!raiz) {
        return raiz;
    }

    if (valor < raiz->valor) {
        raiz->izq = eliminarNodo(raiz->izq, valor);
    } else if (valor > raiz->valor) {
        raiz->der = eliminarNodo(raiz->der, valor);
    } else {
        if (!raiz->izq || !raiz->der) {
            Nodo* temp = raiz->izq ? raiz->izq : raiz->der;

            if (!temp) {
                liberarNodo(raiz);
                return NULL;
            } else {
                *raiz = *temp;
                liberarNodo(temp);
            }
        } else {
            Nodo* temp = nodoMinimo(raiz->der);
            raiz->valor = temp->valor;
            raiz->der = eliminarNodo(raiz->der, temp->valor);
        }
    }

    if (!raiz) return raiz;

    actualizarAltura(raiz);
    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0) {
        return rotacionDerecha(raiz);
    }
    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }
    if (balance < -1 && obtenerBalance(raiz->der) <= 0) {
        return rotacionIzquierda(raiz);
    }
    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// Función para mostrar el árbol en orden raíz -> derecha -> izquierda
void mostrarArbol(Nodo* raiz, int nivel = 0) {
    if (raiz != NULL) {
        mostrarArbol(raiz->der, nivel + 1);
        cout << setw(nivel * 4) << "" << raiz->valor << endl;
        mostrarArbol(raiz->izq, nivel + 1);
    }
}

// Función para liberar todos los nodos del árbol
void liberarArbol(Nodo* raiz) {
    if (raiz) {
        liberarArbol(raiz->izq);
        liberarArbol(raiz->der);
        liberarNodo(raiz);
    }
}

int main() {
    Nodo* raiz = NULL;
    int opcion, valor;

    do {
        cout << "\n1. Insertar nodo\n2. Eliminar nodo\n3. Mostrar arbol\n0. Salir\nOpcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Digite el valor del nodo: ";
            cin >> valor;
            raiz = insertarNodo(raiz, valor);
            break;
        case 2:
            cout << "Digite el valor del nodo a eliminar: ";
            cin >> valor;
            raiz = eliminarNodo(raiz, valor);
            break;
        case 3:
            cout << "Arbol AVL:\n";
            mostrarArbol(raiz);
            break;
        case 0:
            cout << "Saliendo del programa...\n";
            liberarArbol(raiz);
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 0);

    return 0;
}
    