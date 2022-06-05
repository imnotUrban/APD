#include <stdio.h>
#include <iostream>
#include <map>
#include <tuple>
#include <string>
#include "FuncionesAPD.h"

int main(){
    string einicial, estado, pEntrada;
    Lista eFinales=NULL;
    map<tuple<string,char,char>,tuple<string, string>> T;
    cout << "====================================\n";
    cout << "        AUTOMATA PUSH DOWN          \n";

    ingresoTransiciones(T);
    inputs(estado, eFinales, einicial);
    string qi, qj, AR;      //AR= lo que se le añadira al stack   qi= estado actual    qj= estado al que se ira
    while(true){

      /*Ingreso palabras*/

      cout<<"Ingrese palabra de entrada: ";
      getline(cin,pEntrada);
      quitaEspacio(pEntrada);
      if(pEntrada=="finalizar"){
        break;
      }
      while(validarVacio(pEntrada)){
        cout<<"Ingrese palabra de entrada valida (No vacia): ";
        getline(cin,pEntrada);
      }


      cout<<"La palabra de entrada es: "<<pEntrada<<endl<<endl;


      char a=pEntrada[0];       //a = simbolo actual    tope del stack
      int i = 0;
      string stack = "R";
      qi=einicial;
      char S = stack[0];
      imprimirDI(pEntrada, stack, qi, i);

      while(conseguirDatos(qi,a,S,qj,AR, T)){
          qi=qj;
          a=avanzaPalabra(pEntrada,i);
          S=avanzarStack(stack, AR);
          cout << "├" ;
          imprimirDI(pEntrada, stack, qi, i);
      }
      cout<<endl<<endl;
      Lista p = eFinales;
      while(p != NULL && (p->info != qi)){
         p = p->nexo;
      }
      if(p != NULL){
        cout<<"Acepta la palabra por estado final"<<endl;
      }else if(stack.length()==0){
        cout<<"Acepta la palabra por stack vacio"<<endl;
      }else{
        cout<<"No acepta la palabra"<<endl;
      }
      cout<<"Para dejar de ingresar palabras, escriba finalizar \n";
    }

    exit(EXIT_SUCCESS);
}