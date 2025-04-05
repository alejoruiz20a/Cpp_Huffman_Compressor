#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <bitset>
#include <algorithm>
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = filesystem;

class NodoHuffman {
public:
    char simbolo;
    int frecuencia;
    NodoHuffman* izquierdo;
    NodoHuffman* derecho;

    NodoHuffman(char simbolo, int frecuencia, NodoHuffman* izquierdo = nullptr, NodoHuffman* derecho = nullptr)
        : simbolo(simbolo), frecuencia(frecuencia), izquierdo(izquierdo), derecho(derecho) {}
};

struct CompararNodos {
    bool operator()(NodoHuffman* a, NodoHuffman* b) {
        return a->frecuencia > b->frecuencia;
    }
};

class Huff {
public:
    vector<unsigned char> msg;
    map<char, string> dicc;

    Huff(vector<unsigned char> compressed, map<char, string> codigos)
        : msg(compressed), dicc(codigos) {}
};

map<char, int> tablaFrecuencias(const string& text) {
    map<char, int> frecuencias;
    for (char c : text) {
        frecuencias[c]++;
    }
    return frecuencias;
}

NodoHuffman* construir_arbol_huffman(const map<char, int>& frecuencias) {
    priority_queue<NodoHuffman*, vector<NodoHuffman*>, CompararNodos> cola;

    for (const auto& par : frecuencias) {
        cola.push(new NodoHuffman(par.first, par.second));
    }

    while (cola.size() > 1) {
        NodoHuffman* izquierdo = cola.top();
        cola.pop();
        NodoHuffman* derecho = cola.top();
        cola.pop();

        int frecuencia_combinada = izquierdo->frecuencia + derecho->frecuencia;
        NodoHuffman* padre = new NodoHuffman('\0', frecuencia_combinada, izquierdo, derecho);
        cola.push(padre);
    }

    return cola.top();
}

void obtener_codigos_huffman_aux(NodoHuffman* nodo, const string& codigo_actual, map<char, string>& codigos) {
    if (nodo->simbolo != '\0') {
        codigos[nodo->simbolo] = codigo_actual;
    } else {
        if (nodo->izquierdo != nullptr) {
            obtener_codigos_huffman_aux(nodo->izquierdo, codigo_actual + "0", codigos);
        }
        if (nodo->derecho != nullptr) {
            obtener_codigos_huffman_aux(nodo->derecho, codigo_actual + "1", codigos);
        }
    }
}

map<char, string> obtener_codigos_huffman(NodoHuffman* raiz) {
    map<char, string> codigos;
    if (raiz != nullptr) {
        obtener_codigos_huffman_aux(raiz, "", codigos);
    }
    return codigos;
}

vector<unsigned char> stringToBytes(const string& bitString) {
    string bits = bitString;
    while (bits.length() % 8 != 0) {
        bits += '0';
    }

    vector<unsigned char> byteArray;
    for (size_t i = 0; i < bits.length(); i += 8) {
        string byteStr = bits.substr(i, 8);
        unsigned char byte = static_cast<unsigned char>(bitset<8>(byteStr).to_ulong());
        byteArray.push_back(byte);
    }

    return byteArray;
}

string bytesToString(const vector<unsigned char>& byteArray) {
    string bitString;
    for (unsigned char byte : byteArray) {
        bitString += bitset<8>(byte).to_string();
    }
    return bitString;
}

string serializarDiccionario(const map<char, string>& dicc) {
    stringstream ss;
    for (const auto& par : dicc) {
        ss << static_cast<int>(par.first) << ":" << par.second << ";";
    }
    return ss.str();
}

map<char, string> deserializarDiccionario(const string& data) {
    map<char, string> dicc;
    stringstream ss(data);
    string item;
    
    while (getline(ss, item, ';')) {
        size_t separator = item.find(':');
        if (separator != string::npos) {
            int ascii = stoi(item.substr(0, separator));
            string code = item.substr(separator + 1);
            dicc[static_cast<char>(ascii)] = code;
        }
    }
    
    return dicc;
}

Huff comprimir(const string& text) {
    map<char, int> frecuencias = tablaFrecuencias(text);
    NodoHuffman* arbol = construir_arbol_huffman(frecuencias);
    map<char, string> codigos = obtener_codigos_huffman(arbol);
    
    string compressedBits;
    for (char c : text) {
        compressedBits += codigos[c];
    }
    
    vector<unsigned char> compressedBytes = stringToBytes(compressedBits);
    
    delete arbol;
    
    return Huff(compressedBytes, codigos);
}

string descomprimir(const Huff& huff) {
    string bitString = bytesToString(huff.msg);
    map<string, char> diccInvertido;
    
    for (const auto& par : huff.dicc) {
        diccInvertido[par.second] = par.first;
    }
    
    string decoded;
    string codigoActual;
    
    for (char bit : bitString) {
        codigoActual += bit;
        auto it = diccInvertido.find(codigoActual);
        if (it != diccInvertido.end()) {
            decoded += it->second;
            codigoActual.clear();
        }
    }
    
    return decoded;
}

string leerArchivo(const string& ruta) {
    ifstream archivo(ruta, ios::binary);
    if (!archivo) {
        throw runtime_error("No se pudo abrir el archivo: " + ruta);
    }
    
    string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    return contenido;
}

void guardarArchivoComprimido(const string& ruta, const Huff& huff) {
    ofstream archivo(ruta, ios::binary);
    if (!archivo) {
        throw runtime_error("No se pudo crear el archivo: " + ruta);
    }
    
    string diccionarioSerializado = serializarDiccionario(huff.dicc);
    size_t diccSize = diccionarioSerializado.size();
    
    archivo.write(reinterpret_cast<const char*>(&diccSize), sizeof(size_t));
    
    archivo.write(diccionarioSerializado.c_str(), diccSize);
    
    archivo.write(reinterpret_cast<const char*>(huff.msg.data()), huff.msg.size());
    
    archivo.close();
}

Huff cargarArchivoComprimido(const string& ruta) {
    ifstream archivo(ruta, ios::binary);
    if (!archivo) {
        throw runtime_error("No se pudo abrir el archivo: " + ruta);
    }
    
    size_t diccSize;
    archivo.read(reinterpret_cast<char*>(&diccSize), sizeof(size_t));
    
    vector<char> diccBuffer(diccSize);
    archivo.read(diccBuffer.data(), diccSize);
    string diccionarioSerializado(diccBuffer.begin(), diccBuffer.end());
    
    map<char, string> dicc = deserializarDiccionario(diccionarioSerializado);
    
    vector<unsigned char> compressedData(
        (istreambuf_iterator<char>(archivo)),
        istreambuf_iterator<char>()
    );
    
    archivo.close();
    return Huff(compressedData, dicc);
}

void guardarArchivoTexto(const string& ruta, const string& contenido) {
    ofstream archivo(ruta);
    if (!archivo) {
        throw runtime_error("No se pudo crear el archivo: " + ruta);
    }
    
    archivo << contenido;
    archivo.close();
}

void mostrarMenu() {
    cout << "\n---------- COMPRESOR DE HUFFMAN ----------" << endl;
    cout << "1. Comprimir archivo" << endl;
    cout << "2. Descomprimir archivo" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

string obtenerNombreSinExtension(const string& ruta) {
    fs::path path(ruta);
    return path.stem().string();
}

int main() {
    int opcion;
    string rutaArchivo;
    
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); 
        
        try {
            switch (opcion) {
                case 1: { // Comprimir
                    cout << "Ingrese la ruta del archivo a comprimir: ";
                    getline(cin, rutaArchivo);
                    
                    string contenido = leerArchivo(rutaArchivo);
                    
                    Huff resultado = comprimir(contenido);
                    
                    string nombreSalida = obtenerNombreSinExtension(rutaArchivo) + ".huff";
                    
                    guardarArchivoComprimido(nombreSalida, resultado);
                    
                    cout << "Archivo comprimido guardado como: " << nombreSalida << endl;
                    cout << "Tamanio original: " << contenido.size() << " bytes" << endl;
                    cout << "Tamanio comprimido: " << resultado.msg.size() << " bytes" << endl;
                    break;
                }
                case 2: { // Descomprimir
                    cout << "Ingrese la ruta del archivo .huff a descomprimir: ";
                    getline(cin, rutaArchivo);
                    
                    Huff huff = cargarArchivoComprimido(rutaArchivo);
                    
                    string textoDescomprimido = descomprimir(huff);
                    
                    string nombreSalida = obtenerNombreSinExtension(rutaArchivo) + "_descomprimido.txt";
                    
                    guardarArchivoTexto(nombreSalida, textoDescomprimido);
                    
                    cout << "Archivo descomprimido guardado como: " << nombreSalida << endl;
                    break;
                }
                case 0: // Salir
                    cout << "Saliendo del programa... DEV: Alejandro Amador Ruiz" << endl;
                    break;
                default:
                    cout << "Opcion no valida. Intente nuevamente." << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
        
    } while (opcion != 0);
    
    return 0;
}