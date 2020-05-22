#include <omp.h>

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
      size_t inicioIteracion = 0;
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