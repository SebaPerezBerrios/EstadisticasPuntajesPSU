
import 'dart:convert';
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


//print(NEM);
print(PROMEDIOS[0]);
print(DESV_ESTAND[0]);
//print(RANK);
print(PROMEDIOS[1]);
print(DESV_ESTAND[1]);
//print(MAT);
print(PROMEDIOS[2]);
print(DESV_ESTAND[2]);
//print(LEN);
print(PROMEDIOS[3]);
print(DESV_ESTAND[3]);
//print(CIEN);
print(PROMEDIOS[4]);
print(DESV_ESTAND[4]);
//print(HIST);
print(PROMEDIOS[5]);
print(DESV_ESTAND[5]);



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
  //double sumatoria = valores.fold(0, (suma, num) => (num - promedio)*(num - promedio));
  //double sumatoria = valores.fold(0, (suma, num) => pow((num - promedio),2));
  double sumatoria=0;
  for(int i=0;i<valores.length;i++){
    sumatoria = sumatoria + pow((valores[i] - promedio),2);
  }
  return sqrt(sumatoria/(valores.length-1));
}
