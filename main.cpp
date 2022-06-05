#include <stdio.h>
#include <iostream>
#include <map>
#include <tuple>
#include <string>
#include "FuncionesAPD.h"

int main(){
    string einicial, pEntrada;                    //Declaramos las variables que representan estado inicial y palabra entrada
    bool aceptaF;                                 //bool denota de que metodo acepta el APD (FALSE por stack vacio / TRUE por estado final)
    Lista eFinales=NULL;                          //Lista enlazada de estados finales 
    map<tuple<string,char,char>,tuple<string, string>> T; // map que almacena transiciones donde la key es la situacion actual y value el resultado
                                                          // key: {estado actual, simbolo actual de la palabra, simbolo en el tope del stack}
                                                          // value: {estado resultante, palabra a añadir al stack}
    cout << "====================================\n";
    cout << "        AUTOMATA PUSH DOWN          \n";

    ingresoTransiciones(T);                       //Funcion de ingreso de transiciones
    inputs(aceptaF, eFinales, einicial);          //Funcion de ingreso de las variables argumento
    string qi, qj, AR;                            //AR= lo que se le añadira al stack   qi= estado actual    qj= estado al que se ira
    // while para ingresar palabras al APD 
    while(true){
      while (true){   // while para ingresar una palabra
        cout<<"\nIngrese palabra de entrada: ";
        getline(cin,pEntrada);
        quitaEspacio(pEntrada);                   // Se quitan los espacios de la palabra ingresada.
        if(pEntrada == "") {                      // Si se ingresa una palabra vacia, se pide otra palabra para ingresar o para finalizar
          ManejoError(1);
          continue;
        }
        if(pEntrada=="finalizar") exit(EXIT_SUCCESS);    // Si se ingresa finalizar, se finaliza el programa.
        else break;
      } // Si el ingreso no tuvo ningun error, se hace un break del while

      cout<<"La palabra de entrada es: "<<pEntrada<<endl<<endl;

      char a=pEntrada[0];       //a = simbolo actual, corresponde al primer char de la palabra de entrada.
      int i = 0;                //Contador de posiciòn en la palabra.
      string stack = "R";       //String que simboliza el stack.
      qi=einicial;              //Variable que denota el estado actual, se inicializa como estado inicial.
      char S = stack[0];        //Char en el tope del stack
      imprimirDI(pEntrada, stack, qi, i); // Descripciòn instantanea.

      while(conseguirDatos(qi,a,S,qj,AR, T)){ //while que recorre la palabra con el APD. Depende de que conseguirDatos tenga datos para devolver (TRUE) o no (FALSE).
          qi=qj;                              //El estado actual pasa a ser el estado resultante de la transicion(qj)
          a=avanzaPalabra(pEntrada,i);        //Se avanza la palabra
          S=avanzarStack(stack, AR);          //Se elimina el simbolo en el tope del stack. Luego añade la palabra indicada por la transicion en el stack.
          cout << "├" ;                       
          imprimirDI(pEntrada, stack, qi, i); // Descripciòn instantanea.
      }
      cout<<endl<<endl;
      if(aceptaF){                               //Validaciòn de la palabra segun estado final
        Lista p = eFinales;                     //p apunta a la lista de estados finales.
        while(p != NULL && (p->info != qi))    //mientras p no sea NULL y el info de p sea diferente al estado en el que estamos
          p = p->nexo;                        //se avanza p
        if(p != NULL)                        //Si p no es NULL. se encontro un estado final igual al estado con el que termina de leer la palabra el APD
          cout<<"Acepta la palabra por estado final"<<endl; //Se acepta la palabra.
        else cout<<"No acepta la palabra"<<endl;            //No se acepta la palabra.
      }
      else if(stack.length()==0) cout<<"Acepta la palabra por stack vacio"<<endl; //Validacion de la palabra por stack vacio. Se acepta la palabra si el stack es de tamaño 0
      else cout<<"No acepta la palabra"<<endl;                                    //No se acepta la palabra.

      cout<<"Para dejar de ingresar palabras, escriba finalizar \n";
    }
    exit(EXIT_SUCCESS);
}