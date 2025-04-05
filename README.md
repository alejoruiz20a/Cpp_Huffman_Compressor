# Compresor Huffman en C++

Este proyecto es un compresor y descompresor de archivos de texto utilizando el algoritmo de **Huffman**. Permite reducir el tamaño de archivos `.txt` mediante compresión binaria y guardarlos con la extensión `.huff`. También permite descomprimir estos archivos para recuperar el texto original.

## Características

- Compresión de archivos de texto usando Huffman
- Generación y serialización del diccionario de códigos Huffman
- Almacenamiento de archivos comprimidos en formato binario `.huff`
- Descompresión desde `.huff` a `.txt`
- Interfaz de consola amigable
- Manejo de errores robusto

## Estructura del archivo `.huff`

Cada archivo comprimido `.huff` contiene:

1. **Tamaño del diccionario** (`size_t`)
2. **Diccionario serializado** (en formato `ASCII:codigo;`)
3. **Datos comprimidos** (en formato binario)

## Uso y Funcionamiento

---------- COMPRESOR DE HUFFMAN ----------
1. Comprimir archivo
2. Descomprimir archivo
0. Salir
Seleccione una opcion:

### Comprimir
- El usuario ingresa la dirección del txt.
- Crea la tabla de frecuencias de los caracteres.
- Crea el arbol binario de Huffman.
- Extrae los codigos del arbol a un diccionario.
- Traduce el txt a codigos usando el diccionario.
- Pasa los codigos a bytes.
- Escribe los codigos y el diccionario en un archivo .huff

### Desomprimir
- El usuario ingresa la dirección del .huff
- Se extrae el diccionario de los codigos.
- Se pasa el resto del archivo de bytes a 1s y 0s.
- Se traducen los 1s y 0s del archivo usando el diccionario.
- Se escribe el texto recuperado en un archivo_descomprimido.txt. 

## Autor
Alejandro Amador Ruiz - aleamador2003@gmail.com
