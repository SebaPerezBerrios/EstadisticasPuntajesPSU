
import 'dart:io';
import 'dart:math';
import 'package:args/args.dart';

//comentarios en dart

main(List<String> args) async{
  print (args);
if (args.length < 1) {
  print("Error, se necesita un archivo de puntajes");
  exitCode = 1;
  return;
}
List<int> NEM = List();
List<int> RANK = List();
List<int> MAT = List();
List<int> LEN = List();
List<int> CIEN = List();
List<int> HIST = List();
List<int> MODAS = List();
List<double> PROMEDIOS = List();
List<double> DESV_ESTAND = List();
List<double> MEDIANAS = List();
List<String> lines = await new File(args[0]).readAsLines();//cambiar el nombre del archivo al final, este es uno de prueba con menos datos

for(int i=0;i<lines.length;i++){ //recorre linea x linea la lista que contiene el archivo guardando los puntajes en las listas respectivas

  NEM.add(int.parse(lines[i].substring(9,12)));
  RANK.add(int.parse(lines[i].substring(13,16)));
  MAT.add(int.parse(lines[i].substring(17,20)));
  LEN.add(int.parse(lines[i].substring(21,24)));
  CIEN.add(int.parse(lines[i].substring(25,28)));
  HIST.add(int.parse(lines[i].substring(29,32)));
}

//calcula y guarda los promedios de cada categoria y los guarda
PROMEDIOS.add(calcularPromedio(NEM));
PROMEDIOS.add(calcularPromedio(RANK));
PROMEDIOS.add(calcularPromedio(MAT));
PROMEDIOS.add(calcularPromedio(LEN));
PROMEDIOS.add(calcularPromedio(CIEN));
PROMEDIOS.add(calcularPromedio(HIST));

//calcula y guarda las desv, estandar de cada categoria
DESV_ESTAND.add(calcularDesvEstandar(NEM, PROMEDIOS[0]));
DESV_ESTAND.add(calcularDesvEstandar(RANK, PROMEDIOS[1]));
DESV_ESTAND.add(calcularDesvEstandar(MAT, PROMEDIOS[2]));
DESV_ESTAND.add(calcularDesvEstandar(LEN, PROMEDIOS[3]));
DESV_ESTAND.add(calcularDesvEstandar(CIEN, PROMEDIOS[4]));
DESV_ESTAND.add(calcularDesvEstandar(HIST, PROMEDIOS[5]));

//calcula y guarda las modas de cada categoria
MODAS.add(calcularModa(NEM));
MODAS.add(calcularModa(RANK));
MODAS.add(calcularModa(MAT));
MODAS.add(calcularModa(LEN));
MODAS.add(calcularModa(CIEN));
MODAS.add(calcularModa(HIST));

//calcula y guarda las medianas de cada categoria
MEDIANAS.add(calcularMediana(NEM));
MEDIANAS.add(calcularMediana(RANK));
MEDIANAS.add(calcularMediana(MAT));
MEDIANAS.add(calcularMediana(LEN));
MEDIANAS.add(calcularMediana(CIEN));
MEDIANAS.add(calcularMediana(HIST));

print("===NEM===");
print("Promedio =  ${PROMEDIOS[0]}");
print("Desviacion estandar = ${DESV_ESTAND[0]}");
print("Moda = ${MODAS[0]}");
print("Mediana = ${MEDIANAS[0]}\n");

print("===RANKING===");
print("Promedio =  ${PROMEDIOS[1]}");
print("Desviacion estandar = ${DESV_ESTAND[1]}");
print("Moda = ${MODAS[1]}");
print("Mediana = ${MEDIANAS[1]}\n");

print("===MATEMATICA===");
print("Promedio =  ${PROMEDIOS[2]}");
print("Desviacion estandar = ${DESV_ESTAND[2]}");
print("Moda = ${MODAS[2]}");
print("Mediana = ${MEDIANAS[2]}\n");

print("===LENGUAJE===");
print("Promedio =  ${PROMEDIOS[3]}");
print("Desviacion estandar = ${DESV_ESTAND[3]}");
print("Moda = ${MODAS[3]}");
print("Mediana = ${MEDIANAS[3]}\n");

print("===CIENCIAS===");
print("Promedio =  ${PROMEDIOS[4]}");
print("Desviacion estandar = ${DESV_ESTAND[4]}");
print("Moda = ${MODAS[4]}");
print("Mediana = ${MEDIANAS[4]}\n");

print("===HISTORIA===");
print("Promedio =  ${PROMEDIOS[5]}");
print("Desviacion estandar = ${DESV_ESTAND[5]}");
print("Moda = ${MODAS[5]}");
print("Mediana = ${MEDIANAS[5]}\n");

participantes();

}

void participantes(){
  stdout.writeln('');
  stdout.writeln("=== Tarea ===");
  stdout.writeln('');
  stdout.writeln("Ivan Perez");
  stdout.writeln("Sebastian Perez");
  stdout.writeln("Lester Vasquez");
  stdout.writeln("version secuencial Dart");
}

double calcularPromedio(List<int> lista){
  var sumatoria = lista.fold(0, (suma, num) => suma + num);
  return sumatoria/(lista.length);
}

double calcularDesvEstandar(List<int> valores, double promedio){
  double sumatoria=0;
  for(int i=0;i<valores.length;i++){
    sumatoria = sumatoria + pow((valores[i] - promedio),2);
  }
  return sqrt(sumatoria/(valores.length-1));
}

//funcion que calcula la mediana, tambien ordena la lista en orden ascendente(?)
double calcularMediana(List<int> lista){
  lista.sort(); //el sort esta bien
  var l = lista.length;
  if(l%2 == 0){
    return (lista[(l~/2) - 1] + lista[l~/2])/2.0;
  }
  return lista[l~/2].toDouble(); //en impar funciona
}

//si no hay moda retorna el primer valor, si hay mas de 1 moda retorna la primera
int calcularModa(List<int> lista){
  var aux = new Map();
  int max=0,val;

  for(int i=0;i<lista.length;i++){
    if(!aux.containsKey(lista[i])){
      aux[lista[i]] = 1;
    }
    else{
      aux[lista[i]]++;
    }
  }
  aux.forEach((key, value) {
    if (value > max) {
      max = value;
      val = key;
    }
  });
  return val;
}
