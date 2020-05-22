#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <vector>

typedef std::vector<unsigned int> arreglo;

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
    suma += ((puntaje - promedio) * (puntaje - promedio));
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