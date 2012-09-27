#include <Servo.h>

Servo Base;      //Motor de rotacion de la base
Servo HombroI;   //Motor izquierdo del hombro
Servo HombroD;   //Motor derecho del hombro
Servo Codo;      //Motor del codo
Servo MunecaE;   //Motor de elevacion de la muneca
Servo MunecaR;   //Motor de rotacion de la muneca
Servo Pinza;     //Motor de cierre de la pinza

typedef struct {
  signed int Base;
  signed int Hombro;
  signed int Codo;
  signed int MunecaE;
  signed int MunecaR;
  signed int Pinza;
} T_POSICION;

//Posiciones de los motores
T_POSICION Posicion = { 7000, 7500, 4000, 10000, 7000, 13000 };
//Nota: Los valores iniciales estan calibrados a la posicion de descanso de todos
//los motores

const T_POSICION Lim_Sup =
{ 18000, 18000, 18000, 14000, 18000, 13000 };

const T_POSICION Lim_Inf =
{ 0, 0, 3500, 0, 0, 4800 };

//Diferencia en grados de las posiciones de los motores gemelos del hombro
const int OffsetHombro = 7;
//Nota: Esta calibracion permite a los motores alinearse para disminuir el consumo
//excesivo de corriente

void setup() {
  //Establece los pines de los botones como entrada
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  //Activa los pull-up internos en los pines con botones
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(5, 1);
  digitalWrite(6, 1);

  Base.attach(13);
  HombroI.attach(12);
  HombroD.attach(11);
  Codo.attach(10);
  MunecaR.attach(8);
  MunecaE.attach(9);
  Pinza.attach(7);
}

void loop() {
  if (!digitalRead(3)) {
    Posicion.Pinza -= Umbralizar(analogRead(0)) / 4;
    Posicion.Codo -= Umbralizar(analogRead(1)) / 4;
  }

  if (!digitalRead(4)) {
    Posicion.MunecaR += Umbralizar(analogRead(0)) / 4;
    Posicion.MunecaE += Umbralizar(analogRead(1)) / 4;
  }

  if (!digitalRead(5)) {
    Posicion.Base -= Umbralizar(analogRead(0)) / 4;
    Posicion.Hombro += Umbralizar(analogRead(1)) / 4;
  }

  Posicion.Pinza   = Recortar(Posicion.Pinza,   Lim_Sup.Pinza,   Lim_Inf.Pinza);
  Posicion.Codo    = Recortar(Posicion.Codo,    Lim_Sup.Codo,    Lim_Inf.Codo);
  Posicion.MunecaR = Recortar(Posicion.MunecaR, Lim_Sup.MunecaR, Lim_Inf.MunecaR);
  Posicion.MunecaE = Recortar(Posicion.MunecaE, Lim_Sup.MunecaE, Lim_Inf.MunecaE);
  Posicion.Base    = Recortar(Posicion.Base,    Lim_Sup.Base,    Lim_Inf.Base);
  Posicion.Hombro  = Recortar(Posicion.Hombro,  Lim_Sup.Hombro,  Lim_Inf.Hombro);

  Pinza.write(Posicion.Pinza / 100);
  Base.write(Posicion.Base / 100);
  HombroI.write(Posicion.Hombro / 100);
  HombroD.write(Posicion.Hombro / 100 + OffsetHombro);
  Codo.write(Posicion.Codo / 100);
  MunecaR.write(Posicion.MunecaR / 100);
  MunecaE.write(Posicion.MunecaE / 100);
  delay(10);
}

signed int Umbralizar(signed int Valor) {
  signed int Resultado;
  Resultado = Valor - 512;
  if (Resultado < 32 && Resultado > -32) {
    Resultado = 0;
  }
  return Resultado;
}

signed int Recortar(signed int Valor, signed int Maximo, signed int Minimo) {
  if (Valor > Maximo) return Maximo;
  if (Valor < Minimo) return Minimo;
  return Valor;
}
