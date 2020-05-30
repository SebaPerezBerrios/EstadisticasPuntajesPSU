#include <omp.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "lib/Structs.h"

typedef std::vector<unsigned int> arreglo;

void leerArchivo(std::ifstream&, std::vector<arreglo>&);
void calcularResultados(std::vector<arreglo>&);
Puntajes parse(const std::string&);
void _parseRut(std::string::const_iterator&);
unsigned int parseNumero(std::string::const_iterator&);
void parsePuntoComa(std::string::const_iterator&);
void participante();

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Se nececita un nombre de archivo de puntajes" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream archivoEntrada(argv[1]);
  std::vector<arreglo> puntajes(6);

  leerArchivo(archivoEntrada, puntajes);
  archivoEntrada.close();

  calcularResultados(puntajes);

  participante();

  return EXIT_SUCCESS;
}

void calcularResultados(std::vector<arreglo>& puntajes) {
  std::vector<Resultado> resultados(6);
  std::vector<std::string> nombres = {"Nem", "Ranking", "Matematica", "Lenguaje", "Ciencias", "Historia"};
#pragma omp parallel
#pragma omp single
  for (size_t i = 0; i < 6; i++) {
#pragma omp task
    resultados[i] = Resultado(nombres[i], puntajes[i]);
  }

  for (size_t i = 0; i < 6; i++) {
    std::cout << resultados[i] << std::endl;
    std::cout << std::endl;
  }
}

void leerArchivo(std::ifstream& entrada, std::vector<arreglo>& resultado) {
  std::istream_iterator<std::string> iteradorEntrada(entrada);
  std::istream_iterator<std::string> finStream;
  while (iteradorEntrada != finStream) {
    auto puntajes = parse(*++iteradorEntrada);
    resultado[0].push_back(puntajes.nem);
    resultado[1].push_back(puntajes.ranking);
    resultado[2].push_back(puntajes.matematica);
    resultado[3].push_back(puntajes.lenguaje);
    resultado[4].push_back(puntajes.ciencias);
    resultado[5].push_back(puntajes.historia);
  }
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
  std::cout << std::endl << "Ivan Pérez" << std::endl;
  std::cout << std::endl << "Lester Vasquez" << std::endl;
  std::cout << std::endl << "version OpenMP C++" << std::endl;
}
