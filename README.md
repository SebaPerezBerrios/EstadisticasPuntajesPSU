# PuntajesPSU
## Descripción
Este repositorio contiene programas que realizan cálculos estadísticos sobre un archivo de puntajes en formato `.csv`.
Este procedimiento se implementa en cuatro programas distintos. 
- Se incluye la implementacion en `C++` que usa la biblioteca OpenMP para paralelizar el procesamiento.
- Se incluye la implementacion en `C++` que usa la biblioteca OpenMPI para distribuir el procesamiento.
- Se incluye la implementacion en `C++` que procesa los elementos secuencialmente.
- Se incluye la implementacion en `Dart` que procesa los elementos secuencialmente.
## Compilación
```
make -C SecuencialCPP
```
## Ejecución
```
time ./SecuencialCPP/dist/programa <nombre archivo puntajes>
```
## Integrantes
- Léster Vasquez
- Ivan Pérez
- Sebastian Pérez
