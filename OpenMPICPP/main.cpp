#include <mpi.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "lib/FuncionesMPI.h"
#include "lib/Structs.h"

#define Orquestador 0
#define Termino "TERM"

typedef std::vector<unsigned int> arreglo;

struct Puntajes;

void enviarDatos(int, int, const std::string&);
void leerArchivo(std::ifstream&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&, arreglo&);
void calcularResultado(int, int);
void escribirResultados(int, int);

Puntajes parse(const std::string&);
void _parseRut(std::string::const_iterator&);
unsigned int parseNumero(std::string::const_iterator&);
void parsePuntoComa(std::string::const_iterator&);
void participante();

int main(int argc, char** argv) {
  int mi_rango;
  int procesosTotales;
  int procesosReservados = 1;
  int procesosMinimos = 2;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

  MPI_Comm_size(MPI_COMM_WORLD, &procesosTotales);
  if (procesosTotales < procesosMinimos) {
    std::cerr << "ERROR, el programa necesita al menos 2 nodos de procesamiento" << std::endl;
    return EXIT_FAILURE;
  }

  if (mi_rango == Orquestador) {
    if (argc < 2) {
      std::cerr << "Se nececita un nombre de archivo de puntajes" << std::endl;
      return EXIT_FAILURE;
    }
    enviarDatos(procesosReservados, procesosTotales, argv[1]);
    escribirResultados(procesosReservados, procesosTotales);
    participante();
  }

  if (mi_rango != Orquestador) { /* Esclavo */
    calcularResultado(procesosReservados, procesosTotales);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}

void enviarDatos(int procesosReservados, int procesosTotales, const std::string& nombreArchivo) {
  std::ifstream archivoEntrada(nombreArchivo);
  arreglo nem;
  arreglo ranking;
  arreglo matematica;
  arreglo lenguaje;
  arreglo ciencias;
  arreglo historia;

  leerArchivo(archivoEntrada, nem, ranking, matematica, lenguaje, ciencias, historia);
  archivoEntrada.close();

  size_t iteradorProceso = 0;
  int procesosEsclavos = procesosTotales - procesosReservados;

  // envio de mensajes
  std::vector<arreglo> puntajes = {nem, ranking, matematica, lenguaje, ciencias, historia};
  std::vector<std::string> nombres = {"Nem", "Ranking", "Matematica", "Lenguaje", "Ciencias", "Historia"};
  for (size_t i = 0; i < 6; i++) {
    size_t idProcesoEsclavo = procesosReservados + (iteradorProceso++) % procesosEsclavos;
    enviarString(idProcesoEsclavo, nombres[i]);
    enviarDimension(idProcesoEsclavo, puntajes[i].size());
    enviarDatos(idProcesoEsclavo, puntajes[i]);
  }

  // envio de mensajes de termino a esclavos
  for (int idProcesoEsclavo = procesosReservados; idProcesoEsclavo < procesosTotales; idProcesoEsclavo++) {
    enviarString(idProcesoEsclavo, Termino);
  }
}

void calcularResultado(int idProcesoEsclavo, int procesosTotales) {
  while (true) {
    auto nombreEstadistico = recibirString(Orquestador);
    if (nombreEstadistico == Termino) {
      enviarString(Orquestador, Termino);
      break;
    }
    auto dimensionArreglo = recibirDimension(Orquestador);
    arreglo puntajes(dimensionArreglo);
    recibirDatos(Orquestador, puntajes);

    auto resultado = Resultado(nombreEstadistico, puntajes);
    std::stringstream salida;
    salida << resultado << std::endl;
    enviarString(Orquestador, salida.str());
  }
}

void escribirResultados(int procesosReservados, int procesosTotales) {
  size_t iteradorProceso = 0;
  std::vector<bool> completados(procesosTotales, false);
  int procesosEsclavos = procesosTotales - procesosReservados;
  int totalCompletados = 0;
  while (totalCompletados != procesosEsclavos) {
    size_t idProcesoEsclavo = procesosReservados + (iteradorProceso++) % procesosEsclavos;
    if (!completados[idProcesoEsclavo]) {
      std::string mensaje = recibirString(idProcesoEsclavo);

      if (mensaje == Termino) {
        completados[idProcesoEsclavo] = true;
        ++totalCompletados;
      } else {
        std::cout << mensaje << std::endl;
      }
    }
  }
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
  std::cout << std::endl << "Lester Vasquez" << std::endl;;
  std::cout << std::endl << "version OpenMPI C++" << std::endl;
}
