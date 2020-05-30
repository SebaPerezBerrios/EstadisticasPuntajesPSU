#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "lib/Structs.h"

typedef std::vector<unsigned int> arreglo;

void leerArchivo(std::ifstream&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&);
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
  std::vector<unsigned int> nem;
  std::vector<unsigned int> ranking;
  std::vector<unsigned int> matematica;
  std::vector<unsigned int> lenguaje;
  std::vector<unsigned int> ciencias;
  std::vector<unsigned int> historia;

  leerArchivo(archivoEntrada, nem, ranking, matematica, lenguaje, ciencias, historia);
  archivoEntrada.close();

  std::cout << Resultado("Nem", nem) << std::endl << std::endl;
  std::cout << Resultado("Ranking", ranking) << std::endl << std::endl;
  std::cout << Resultado("Matematica", matematica) << std::endl << std::endl;
  std::cout << Resultado("Lenguaje", lenguaje) << std::endl << std::endl;
  std::cout << Resultado("Ciencias", ciencias) << std::endl << std::endl;
  std::cout << Resultado("Historia", historia) << std::endl << std::endl;
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
  std::cout << std::endl << "version secuencial C++" << std::endl;
}
