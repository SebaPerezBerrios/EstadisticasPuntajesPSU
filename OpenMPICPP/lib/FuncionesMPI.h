#include <mpi.h>

#include <vector>

typedef std::vector<unsigned int> arreglo;

void enviarString(int idProceso, const std::string& nombre) {
  MPI_Send(nombre.data(), nombre.size() + 1, MPI_CHAR, idProceso, 0, MPI_COMM_WORLD);
}

std::string recibirString(int idProceso) {
  MPI_Status estado;
  char nombre[256];
  MPI_Recv(nombre, 256, MPI_CHAR, idProceso, 0, MPI_COMM_WORLD, &estado);
  if (estado.MPI_ERROR != 0) {
    std::cerr << "ERROR: " << estado.MPI_ERROR << std::endl;
    throw "ERROR MPI";
  }
  return std::string(nombre);
}

size_t recibirDimension(int idProceso) {
  size_t resultado = 0;
  MPI_Status estado;
  MPI_Recv(&resultado, 1, MPI_INT64_T, idProceso, 0, MPI_COMM_WORLD, &estado);
  if (estado.MPI_ERROR != 0) {
    std::cerr << "ERROR: " << estado.MPI_ERROR << std::endl;
    throw "ERROR MPI";
  }
  return resultado;
}

void enviarDimension(int idProceso, size_t resultado) {
  MPI_Send(&resultado, 1, MPI_INT64_T, idProceso, 0, MPI_COMM_WORLD);
}

void recibirDatos(int idProceso, arreglo& resultado) {
  MPI_Status estado;
  MPI_Recv(resultado.data(), resultado.size(), MPI_INT, idProceso, 0, MPI_COMM_WORLD, &estado);
  if (estado.MPI_ERROR != 0) {
    std::cerr << "ERROR: " << estado.MPI_ERROR << std::endl;
    throw "ERROR MPI";
  }
}

void enviarDatos(int idProceso, arreglo& resultado) {
  MPI_Send(resultado.data(), resultado.size(), MPI_INT, idProceso, 0, MPI_COMM_WORLD);
}