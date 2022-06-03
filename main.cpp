#include <stdio.h>
#include <iostream>
#include <map>
#include <tuple>
#include <string>

using namespace std;
struct nodo{
  string info;
  struct nodo *nexo;
};
typedef struct nodo *Lista;

void ingresoTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M);
void quitaEspacio(string &palabra);
string aMinuscula(string str, int n);
bool esAlfanumerico(char c);
bool validaPalabra(string str, int &i, int n, string &output);
bool validaSimbolo(string str, int &i, int n, char &output);
bool validaChar(string str,char obj, int &i, int n);
bool conseguirDatos(string qi, char a, char R, string &qj, string &AR, map<tuple<string,char,char>,tuple<string, string>> &M);
void ManejoError(int opt);
void mensajeAyuda();
void imprimirTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M);
void imprimirDI(string p, string s, string q, int i);
void insertarInfo(Lista &eFinales, string estadoF);
bool esAlfanumerico(string s);

void inputs(string &estado,Lista &eFinales,string &einicial);
bool validarVacio(string palabra);
char avanzarStack(string &stack, string AR);
char avanzaPalabra(string palabra, int &i);

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


      cout<<"La palabra de entrada es: "<<pEntrada<<endl;


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
      cout<<endl;
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



void inputs(string &estado, Lista &eFinales,string &einicial){

    cout<<"Ingrese el estado inicial: ";
    getline(cin,einicial);
    quitaEspacio(einicial);
    cout<<endl;
	
    cout<<"Ingrese (1) si el APD acepta por stack vacio y (2) si acepta por estado final: ";
    getline(cin, estado);
    quitaEspacio(estado);
    cout<<endl;
    string temp;
    if(estado=="2"){
        while(true){
          cout << "Ingrese un estado final: "<< endl;
          getline(cin, temp);
          quitaEspacio(temp);
          if(temp.length() == 0) {
            ManejoError(1);
            continue;
          }
          if(temp == "siguiente") break;
          if(esAlfanumerico(temp)){
            insertarInfo(eFinales, temp);
            cout << "ingreso Exitoso!" << endl;
          } else {
            ManejoError(4);
          }
        }
        cout<<endl;
    }



    cout<<endl<<" ===================================="<<endl<<endl;

    cout<<"Estado inicial: "<<einicial<<endl;
    if(estado=="1"){
        cout<<"El APD acepta por stack vacio por lo que no tiene estado final."<<endl;
    }
    else{
        Lista p = eFinales;
        cout<<"Los estados finales del APD son: " << endl;
        while(p != NULL){
          cout << p->info << " ";
          p = p->nexo;
        }
        cout << endl;
        
    }
    

}

bool validarVacio(string palabra){ //Valida si la palabra es vacía
    int n=palabra.length();
    if(n==0){
        return true;
    }
    int cont = 0;
    for(int i = 0; i<n; i++){
        if(palabra[i]!=' '){
            cont++;
        }
    }
    if(cont>0){
        return false;
    }
    return true;
}

void ingresoTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M){
  string input="",qi,qj,AR,t1,t2; // qi es estado actual, qj es estado destino, AR simbolos que se adieren al stack 
  char a,R; //a simbolo que se lee de la palabra, R simbolo que se lee del stack. EJ del todo: (qi,a,R)=(qj,AR)
  int i,n;
  bool elim;
    cout << "====================================\n";
    cout << "Las transiciones se ingresan de la forma: (qi,a,R)=(qj,B) \n";
    cout << "El simbolo \'@\' representa la palabra vacia.\n";
    cout << "Para dejar de ingresar transiciones, escriba \'siguiente\' \n";
    cout << "Si necesita mas ayuda, ingrese \'ayuda\' \n";
  while(true){
    cout << "Ingresa la transicion: ";
    getline(cin,input);
    quitaEspacio(input);

    i=0;
    n=input.length();
    
    if(aMinuscula(input,n)=="siguiente") break;  //se sale del ingreso de transiciones
    if(aMinuscula(input,n)=="ayuda"){
      mensajeAyuda();
      continue;
    }
    if(aMinuscula(input,n)=="visualizar"){
      imprimirTransiciones(M);
      continue;
    }
    if(n==0){  //se ingresa vacio
      ManejoError(1);
      continue;
    }

    if(input[i]=='-'){
      elim=true;
      i++;
    }
    else elim=false;

    if(!(validaChar(input,'(',i,n)&& validaPalabra(input,i,n,qi)&& validaChar(input,',',i,n)&& validaSimbolo(input,i,n,a))) continue;
    if(!(validaChar(input,',',i,n)&& validaSimbolo(input,i,n,R)&& validaChar(input,')',i,n)&& validaChar(input,'=',i,n))) continue;
    if(!(validaChar(input,'(',i,n)&& validaPalabra(input,i,n,qj)&& validaChar(input,',',i,n)&& validaPalabra(input,i,n,AR)&& validaChar(input,')',i,n))) continue;
    if(i<n){
      ManejoError(3);
      continue;
    }
    if(a=='@' && R=='@'){
        ManejoError(6);
        continue;
    }
    if(elim){
      if(conseguirDatos(qi,a,R,t1,t2,M) && qj==t1 && AR==t2){
          M.erase({qi,a,R});
          cout<<"Eliminacion exitosa.\n";
        }
        else ManejoError(5);
    }
    else{
      cout<<"Ingreso exitoso. ";
      if(M.count({qi,a,R})==1) cout << "Sobreescribiendo transicion";
      cout << endl;
      M[{qi,a,R}]={qj,AR};
    }
  }
}
void quitaEspacio(string &palabra){
    string palabraSinEsp="";
    int n=palabra.length();
    for(int i=0; i<n; i++){
        if(palabra[i]==' '){
        }else{
            palabraSinEsp+=palabra[i];
        }
    }
    palabra=palabraSinEsp;
}
string aMinuscula(string str, int n){
  string output="";
  for(int i=0; i<n;i++){
    if(65<=(int)(str[i]) && (int)(str[i])<=90){
      output+=(char)(32+(int)(str[i]));
    }
    else output+=str[i];
  }
  return output;
}
bool esAlfanumerico(char c){
  int i= (int)(c); //consigue el codigo ASCII del char c
  if(48<=i && i<=57) return true; //es un numero
  if(65<=i && i<=90) return true; //es letra (mayuscula)
  if(97<=i && i<=122) return true; //es letra (minuscula)
  if(64==i) return true; //es @ (se utiliza como palabra vacia)
  return false;
}
bool esAlfanumerico(string s){
  int n = s.length();
  for(int i=0; i<n; i++)
    if(!esAlfanumerico(s[i])) return false;
  return true;
}

bool validaPalabra(string str, int &i, int n, string &output){
  if(i>=n){
    ManejoError(2);
    return false;
  }
  int j=i;
  while(i<n && esAlfanumerico(str[i])) i++;
  if(i==j){
    ManejoError(4);
    return false;
  }
  output=str.substr(j,i-j);
  return true;
}
bool validaSimbolo(string str, int &i, int n, char &output){
  if(i>=n){
    ManejoError(2);
    return false;
  }
  if(esAlfanumerico(str[i])){
    output=str[i];
    i++;
    return true;
  }
  ManejoError(4);
  return false;
}
bool validaChar(string str,char obj, int &i, int n){
  if(i>=n){
    ManejoError(2);
    return false;
  }
  if(str[i]==obj){
    i++;
    return true;
  }
  ManejoError(0);
  return false;
}
bool conseguirDatos(string qi, char a, char R, string &qj, string &AR,map<tuple<string,char,char>,tuple<string, string>> &M){
  if(M.count({qi,a,R})!=1){
    return false;
  }
  qj=get<0>(M[{qi,a,R}]);
  AR=get<1>(M[{qi,a,R}]);
  return true;
}
void ManejoError(int opt){
  cout << "====================================\nERROR:\t";
  switch (opt){
  case 0:
    cout << "Error de sintaxis.\n";
    break;
  case 1:
    cout << "Ingreso vacio.\n";
    break;
  case 2:
    cout << "Ingreso insuficiente.\n";
    break;
  case 3:
    cout << "Ingreso excesivo.\n";
    break;
  case 4:
    cout << "Ingreso no alfanumerico.\n";
    break;
  case 5:
    cout<< "No existe transicion a eliminar.\n";
    break;
  case 6:
    cout<< "Transicion ilegal.\n";
    break;
  default:
    cout<<"Error no definido.\n";
    break;
  }
  cout << "Si necesita ayuda, ingrese \'ayuda\'\n";
}
void mensajeAyuda(){
  cout<<"====================================\n";
  cout<<"El ingreso de transiciones debe hacerse individualmente y de la forma: (qi,a,R)=(qj,B)\n";
  cout<<"Donde:\n\t\'qi\' es el estado actual del APD\n\t\'a\' es el simbolo de la palabra que se esta leyendo\n";
  cout<<"\t\'R\' es el simbolo del STACK que se esta leyendo\n\t\'qj\' es el estado destino de la transicion\n";
  cout<<"\t\'B\' es la palabra que se agrega al STACK\nEj:\t(q0,a,R)=(q0,AR)\n";
  cout<<"Para visualizar las transiciones agregadas, ingrese \'visualizar\'\n";
  cout<<"Para eliminar una transicion, ingrese \'-\' seguido de su transicion. Ej: -(qi,a,A)=(qj,B)\n";
  cout<<"Para dar termino al ingreso de transiciones, ingrese \'siguiente\'\n";
  cout<<"Solo debe ingresar valores alfanumericos.\nCualquier espacio en blanco es ignorado, para ingresar la palabra vacia, use \'@\'.\n";
}
void imprimirTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M){
  cout<<"====================================\n";
  cout << "Transiciones del APD:\n";
  string qi,qj,AR;
  char a,R;
  for (auto t : M){
    qi = get<0>(t.first);
    a = get<1>(t.first);
    R = get<2>(t.first);
    qj = get<0>(t.second);
    AR = get<1>(t.second);
    cout << '(' << qi << ',' << a << ',' << R << ")=(" << qj << ',' << AR << ')' << endl;
  }
}


char avanzarStack(string &stack, string AR){
    if(stack.length()>1){
        stack=stack.substr(1,stack.length()-1);
    }else{
        stack="";
    }
    if(AR!="@"){
        stack=AR+stack;
    }
    if(stack.length()==0){
        return '@';
    }else{
        return stack[0];
    }
}

char avanzaPalabra(string palabra, int &i){
    i++;
    int n=palabra.length();
    if(i>=n){
        return '@';
    }else{
        return palabra[i];
    }
}

void imprimirDI(string p, string s, string q, int i){
  int n= p.length(), j= n-i;
  string pal, stack;
  
  if(i >= n) pal = "@";
  else pal = p.substr(i,j);

  if (s == "") stack = "@";
  else stack = s;

  cout << "(" << q << ", " << pal << ", " << stack << ")";
}
void insertarInfo(Lista &eFinales, string estadoF){
  Lista q;
  q = new(struct nodo);
  q->info = estadoF;
  q->nexo = eFinales;
  eFinales = q; 
}
