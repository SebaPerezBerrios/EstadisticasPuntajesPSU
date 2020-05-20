#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

typedef std::vector<int> arreglo;

struct Puntajes;

void leerArchivo(std::ifstream&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&);
double promedio(const arreglo&);
double desviacionEstandar(const arreglo&, int);
unsigned int moda(const arreglo&);
double mediana(arreglo&);
Puntajes parse(const std::string&);
void _parseRut(std::string::const_iterator&);
unsigned int parseNumero(std::string::const_iterator&);
void parsePuntoComa(std::string::const_iterator&);
void participante();

struct Resultado {
  std::string _nombre;
  double _promedio;
  double _desviacionEstandar;
  unsigned int _moda;
  double _mediana;
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
    os << "Mediana: " << datos._mediana << std::endl;
    return os;
  }
};

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Se nececita un nombre de archivo de puntajes" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream archivoEntrada(argv[1]);
  std::vector<int> nem;
  std::vector<int> ranking;
  std::vector<int> matematica;
  std::vector<int> lenguaje;
  std::vector<int> ciencias;
  std::vector<int> historia;

  leerArchivo(archivoEntrada, nem, ranking, matematica, lenguaje, ciencias, historia);
  archivoEntrada.close();

  std::cout << Resultado("Nem", nem) << std::endl;
  std::cout << Resultado("Ranking", ranking) << std::endl;
  std::cout << Resultado("Matematica", matematica) << std::endl;
  std::cout << Resultado("Lenguaje", lenguaje) << std::endl;
  std::cout << Resultado("Ciencias", ciencias) << std::endl;
  std::cout << Resultado("Historia", historia) << std::endl;
  participante();

  return EXIT_SUCCESS;
}

struct Puntajes {
  unsigned int nem;
  unsigned int ranking;
  unsigned int matematica;
  unsigned int lenguaje;
  unsigned int ciencias;
  unsigned int historia;
};

void leerArchivo(std::ifstream& entrada, arreglo& nem, arreglo& ranking, arreglo& matematica, arreglo& lenguaje,
                 arreglo& ciencias, arreglo& historia) {
  std::istream_iterator<std::string> iteradorEntrada(entrada);
  std::istream_iterator<std::string> finStream;

  while (iteradorEntrada != finStream) {
    auto puntajes = parse(*++iteradorEntrada);
    nem.push_back(puntajes.nem);
    ranking.push_back(puntajes.ranking);
    matematica.push_back(puntajes.matematica);
    lenguaje.push_back(puntajes.lenguaje);
    ciencias.push_back(puntajes.ciencias);
    historia.push_back(puntajes.historia);
  }
}

double promedio(const arreglo& puntajes) {
  unsigned long suma = 0;
  for (auto puntaje : puntajes) {
    suma += puntaje;
  }
  return ((double)suma) / puntajes.size();
}

double desviacionEstandar(const arreglo& puntajes, int promedio) {
  unsigned long suma = 0;
  for (auto puntaje : puntajes) {
    suma += pow(puntaje - promedio, 2);
  }
  return sqrt(suma / puntajes.size());
}

typedef std::pair<unsigned int, unsigned int> intPair;

struct comparadorPares {
  bool operator()(intPair const& lhs, intPair const& rhs) { return lhs.second < rhs.second; }
};

unsigned int moda(const arreglo& puntajes) {
  std::unordered_map<unsigned int, unsigned int> frecuencia;
  for (auto puntaje : puntajes) {
    auto resultado = frecuencia.insert({puntaje, 1});
    // existe elemento previamente
    if (!resultado.second) {
      ++resultado.first->second;
    }
  }
  return max_element(frecuencia.begin(), frecuencia.end(), comparadorPares())->first;
}

double mediana(arreglo& puntajes) {
  std::sort(puntajes.begin(), puntajes.end());
  auto cantidad = puntajes.size();
  if (cantidad % 2 == 0) {
    // promedio si cantidad de datos es par
    return (puntajes[cantidad / 2 - 1] + puntajes[cantidad / 2]) / 2.0;
  }
  return puntajes[cantidad / 2];
}

Puntajes parse(const std::string& linea) {
  std::string::const_iterator iterador = linea.begin();
  _parseRut(iterador);
  parsePuntoComa(iterador);
  auto nem = parseNumero(iterador);
  parsePuntoComa(iterador);
  auto ranking = parseNumero(iterador);
  parsePuntoComa(iterador);
  auto matematica = parseNumero(iterador);
  parsePuntoComa(iterador);
  auto lenguaje = parseNumero(iterador);
  parsePuntoComa(iterador);
  auto ciencias = parseNumero(iterador);
  parsePuntoComa(iterador);
  auto historia = parseNumero(iterador);
  return Puntajes{.nem = nem,
                  .ranking = ranking,
                  .matematica = matematica,
                  .lenguaje = lenguaje,
                  .ciencias = ciencias,
                  .historia = historia};
}

void _parseRut(std::string::const_iterator& iterador) {
  while (*iterador != ';') {
    ++iterador;
  }
}

unsigned int parseNumero(std::string::const_iterator& iterador) {
  unsigned int resultado = 0;
  while (*iterador != ';' && *iterador != '\0') {
    resultado = resultado * 10 + (*iterador++ - '0');
  }
  return resultado;
}

void parsePuntoComa(std::string::const_iterator& iterador) { ++iterador; }

void participante() {
  std::cout << std::endl << "=== Tarea ===" << std::endl;
  std::cout << std::endl << "Sebastián Pérez Berrios" << std::endl;
  std::cout << std::endl << "version secuencial C++" << std::endl;
}
