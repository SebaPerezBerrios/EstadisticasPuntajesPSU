#include <omp.h>

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
void calcularResultados(arreglo&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&);
double promedio(const arreglo&);
double desviacionEstandar(const arreglo&, int);
unsigned int moda(const arreglo&);
unsigned int moda_sec(const arreglo&);
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
  Resultado(){};
  Resultado(const std::string& nombre, arreglo& datos) {
    _nombre = nombre;
    _promedio = promedio(datos);
    _desviacionEstandar = desviacionEstandar(datos, _promedio);
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

struct Puntajes {
  unsigned int nem;
  unsigned int ranking;
  unsigned int matematica;
  unsigned int lenguaje;
  unsigned int ciencias;
  unsigned int historia;
};

void calcularResultados(arreglo& nem, arreglo& ranking, arreglo& matematica, arreglo& lenguaje, arreglo& ciencias,
                        arreglo& historia) {
  Resultado resultadoNem;
  Resultado resultadoRanking;
  Resultado resultadoMatematica;
  Resultado resultadoLenguaje;
  Resultado resultadoCiencias;
  Resultado resultadoHistoria;

#pragma omp parallel
#pragma omp single
  {
#pragma omp task
    resultadoNem = Resultado("Nem", nem);
#pragma omp task
    resultadoRanking = Resultado("Ranking", ranking);
#pragma omp task
    resultadoMatematica = Resultado("Matematica", matematica);
#pragma omp task
    resultadoLenguaje = Resultado("Lenguaje", lenguaje);
#pragma omp task
    resultadoCiencias = Resultado("Ciencias", ciencias);
#pragma omp task
    resultadoHistoria = Resultado("Historia", historia);
  }
  // calcular moda, algoritmo paralelo
  resultadoNem._moda = moda(nem);
  resultadoRanking._moda = moda(ranking);
  resultadoMatematica._moda = moda(matematica);
  resultadoLenguaje._moda = moda(lenguaje);
  resultadoCiencias._moda = moda(ciencias);
  resultadoHistoria._moda = moda(historia);

  std::cout << resultadoNem << std::endl;
  std::cout << resultadoRanking << std::endl;
  std::cout << resultadoMatematica << std::endl;
  std::cout << resultadoLenguaje << std::endl;
  std::cout << resultadoCiencias << std::endl;
  std::cout << resultadoHistoria << std::endl;
}

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

  calcularResultados(nem, ranking, matematica, lenguaje, ciencias, historia);

  participante();

  return EXIT_SUCCESS;
}

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
  // definir particionado por hilo
  size_t hilos = omp_get_max_threads();
  size_t particion = puntajes.size() / hilos;
  std::vector<size_t> particiones(hilos, particion);
  auto resto = puntajes.size() - particion * hilos;
  for (auto& particion : particiones) {
    if (resto--) particion++;
  }

  std::vector<std::unordered_map<unsigned int, unsigned int>> frecuencia(hilos);
#pragma omp parallel
#pragma omp single
  for (size_t hilo = 0; hilo < hilos; hilo++) {
#pragma omp task
    {
      // calculo offset
      auto inicioIteracion = 0;
      for (size_t i = 0; i < hilo; i++) inicioIteracion += particiones[i];

      auto iteradorOffset = puntajes.begin() + inicioIteracion;
      for (auto puntaje = iteradorOffset; puntaje < iteradorOffset + particiones[hilo]; puntaje++) {
        auto resultado = frecuencia[hilo].insert({*puntaje, 1});
        // existe elemento previamente
        if (!resultado.second) {
          ++resultado.first->second;
        }
      }
    }
  }
  // acumulador
  std::unordered_map<unsigned int, unsigned int> frecuenciaTotal;
  for (const auto& frecuenciaHilo : frecuencia) {
    for (const auto& par : frecuenciaHilo) {
      auto resultado = frecuenciaTotal.insert(par);
      // existe elemento previamente
      if (!resultado.second) {
        resultado.first->second += par.second;
      }
    }
  }

  return max_element(frecuenciaTotal.begin(), frecuenciaTotal.end(), comparadorPares())->first;
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
  std::cout << std::endl << "version OpenMP C++" << std::endl;
}
