Compresor Huffman en C++
Compresor y descompresor de archivos basado en el algoritmo de Huffman implementado en C++. Permite comprimir archivos de texto a formato binario (.huff) y descomprimirlos a su forma original.

Requisitos
Compilador g++ compatible con C++17

Sistema Windows con MinGW-w64 instalado

Instalación
Descargar MinGW-w64 desde winlibs.com

Añadir la ruta de instalación a las variables de entorno del sistema

Compilación
Compilar el proyecto con:

Copy
g++ -std=c++17 -o huffman main.cpp HuffmanNode.cpp
Uso
Ejecutar el programa:

Copy
./huffman
El menú ofrece estas opciones:

Comprimir archivo de texto a .huff

Descomprimir archivo .huff a texto

Salir del programa

