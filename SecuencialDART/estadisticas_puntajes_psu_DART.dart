
import 'dart:io';
import 'dart:math';


//comentarios en dart

main() async{

List<int> NEM = List();
List<int> RANK = List();
List<int> MAT = List();
List<int> LEN = List();
List<int> CIEN = List();
List<int> HIST = List();
List<double> PROMEDIOS = List();
List<double> DESV_ESTAND = List();
List<double> MEDIANAS = List();
List<String> lines = await new File('puntajes.csv').readAsLines();//cambiar el nombre del archivo al final, este es uno de prueba con menos datos
/*lines.forEach((String line){ 
  print(line);//imprime linea x linea
});*/

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

//calcula y guarda las medianas de cada categoria
MEDIANAS.add(calcularMediana(NEM));
MEDIANAS.add(calcularMediana(RANK));
MEDIANAS.add(calcularMediana(MAT));
MEDIANAS.add(calcularMediana(LEN));
MEDIANAS.add(calcularMediana(CIEN));
MEDIANAS.add(calcularMediana(HIST));

/*
//datos para pruebas
List<int> test = List();
test.add(int.parse('3'));
test.add(int.parse('1'));
test.add(int.parse('45'));
test.add(int.parse('67'));
test.add(int.parse('3'));
test.add(int.parse('7'));
//test.add(int.parse('8'));

*/

print("===NEM===");
//print(NEM);
print("Promedio =  ${PROMEDIOS[0]}");
print("Desviacion estandar = ${DESV_ESTAND[0]}");
print("Mediana = ${MEDIANAS[0]}\n");

print("===RANKING===");
//print(RANK);
print("Promedio =  ${PROMEDIOS[1]}");
print("Desviacion estandar = ${DESV_ESTAND[1]}");
print("Mediana = ${MEDIANAS[1]}\n");

print("===MATEMATICA===");
//print(MAT);
print("Promedio =  ${PROMEDIOS[2]}");
print("Desviacion estandar = ${DESV_ESTAND[2]}");
print("Mediana = ${MEDIANAS[2]}\n");

print("===LENGUAJE===");
//print(LEN);
print("Promedio =  ${PROMEDIOS[3]}");
print("Desviacion estandar = ${DESV_ESTAND[3]}");
print("Mediana = ${MEDIANAS[3]}\n");

print("===CIENCIAS===");
//print(CIEN);
print("Promedio =  ${PROMEDIOS[4]}");
print("Desviacion estandar = ${DESV_ESTAND[4]}");
print("Mediana = ${MEDIANAS[4]}\n");

print("===HISTORIA===");
//print(HIST);
print("Promedio =  ${PROMEDIOS[5]}");
print("Desviacion estandar = ${DESV_ESTAND[5]}");
print("Mediana = ${MEDIANAS[5]}\n");




//lee el archivo linea x linea y imprimirlo linea x linea
/* 
  new File('puntajes_test.csv')
    .openRead()
    .transform(utf8.decoder)
    .transform(new LineSplitter())
    .forEach((var l) {

      //NEM.add(int.parse(l.substring(9,12)));
      //RANK.add(int.parse(l.substring(13,16)));
      //MAT.add(int.parse(l.substring(17,20)));
      //LEN.add(int.parse(l.substring(21,24)));
      //CIEN.add(int.parse(l.substring(25,28)));
      //HIST.add(int.parse(l.substring(29,32)));

      //print(l);
      //print('     ');
      //print(NEM);
    });
*/


    //print(NEM.isEmpty); //por ahora esto no funciona

/* //LEER EL ARCHIVO COMPLETO, IMPRIMIRLO COMPLETO Y COPIARLO COMPLETO EN OTRO ARCHIVO
  var file = File('puntajes_test.csv');
  var contents;
  if(await file.exists()){
    //read file
    contents = await file.readAsString(); //leo el archivo y guardo todo en contents
    print(file.readAsStringSync()); //este comando imprime el contenido completo del archivo

    print('   ');
    print(contents); //otra forma de imprimir el archivo completo

    //write file
    var salida = await File('estadisticas_psu.csv').writeAsString(contents); //escribo en el archivo
    salida.writeAsString('este comando sirve para escribir en el archivo salida');
    print(await salida.exists());
    print(await salida.length());

  */

  participantes();

}

void participantes(){
  stdout.writeln('');
  stdout.writeln("=== Tarea ===");
  stdout.writeln('');
  stdout.writeln("Ivan Perez Alarcon");
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


//funcion para calcular la moda, si hay 2 modas arroja la ultima
//ESTA MALA LA FUNCION SE DEMORA DEMASIADO
int calcularModa(List<int> valores){
  int cont,moda;
  for(int a=0;a<valores.length;a++){
    cont=0;
    for(int b=0;b<valores.length;b++){
      if(valores[a]== valores[b]){
        cont++;
      }
      if(cont>=2){
        moda = valores[a];
      }
    }
  }
  return moda;
}