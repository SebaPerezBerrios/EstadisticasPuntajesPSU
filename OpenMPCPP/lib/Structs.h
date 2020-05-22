#include <iostream>

#include "Estadistica.h"

struct Resultado {
  std::string _nombre;
  double _promedio;
  double _desviacionEstandar;
  unsigned int _moda;
  double _mediana;
  Resultado() {}
  Resultado(const std::string& nombre, arreglo& datos) {
    _nombre = nombre;
    _promedio = promedio(datos);
    _desviacionEstandar = desviacionEstandar(datos, _promedio);
    _moda = moda(datos);
    _mediana = mediana(datos);
  }
  friend std::ostream& operator<<(std::ostream& os, const Resultado& datos) {
    os << "===" << datos._nombre << "===" << std::endl;
    os << "Promedio: " << datos._promedio << std::endl;
    os << "Desviacion Estandar: " << datos._desviacionEstandar << std::endl;
    os << "Moda: " << datos._moda << std::endl;
    os << "Mediana: " << datos._mediana;
    return os;
  }
};

struct Puntajes {
  unsigned int nem;
  unsigned int ranking;
  unsigned int matematica;
  unsigned int lenguaje;
  unsigned int ciencias;
  unsigned int historia;
};