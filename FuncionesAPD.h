#ifndef FUNCIONESAPD.h
#define FUNCIONESAPD.h

using namespace std;

struct nodo{           // Lista enlazada que posteriormente utilizaremos para guardar los estados finales
  string info;        // estado final almacenada en la info del nodo
  struct nodo *nexo;  // puntero al siguiente nodo
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
void inputs(bool &aceptaF, Lista &eFinales,string &einicial);
char avanzarStack(string &stack, string AR);
char avanzaPalabra(string palabra, int &i);

// Funcion inputs: Funcion para el ingreso de las variables solicitadas
// aceptaF: si acepta por estados finales o no
// eFinales: lista enlazada con los estados finales.
// einicial: estado inicial
void inputs(bool &aceptaF, Lista &eFinales,string &einicial){
    string estado, temp;
    cout<<"====================================\n";
    while(true){                                            // while para el ingreso de la palabra inicial
      cout<<"Ingrese el estado inicial: ";
      getline(cin,einicial);
      quitaEspacio(einicial);
      if(einicial == "")                  ManejoError(1);
      else if (!esAlfanumerico(einicial)) ManejoError(4);
      else break;
    }
    cout<<"====================================\n";
    while(true){                                            // while para el ingreso de cual metodo utiliza el APD para aceptar palabras.
      cout<<"Ingrese (1) si el APD acepta por stack vacio y (2) si acepta por estado final: ";
      getline(cin, estado);
      quitaEspacio(estado);
      cout<<endl;
      if (estado == "") ManejoError(1);
      else if ((estado != "1") && (estado != "2")) ManejoError(0);
      else break;
    }
    aceptaF = (estado == "2");
    cout<<"====================================\n";
    while(aceptaF){                                         // while para el ingreso de estados finales, mientras el APD utilice estados finales.
      cout << "Ingrese un estado final: ";
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
    cout<<"====================================\n"
        << "Estado inicial: "<<einicial<<endl;
    if(aceptaF){                                            // imprime los estados finales, Si el APD acepta por estado final.
        Lista p = eFinales;
        cout<<"Los estados finales del APD son: " << endl;
        while(p != NULL){
          cout << p->info << " ";
          p = p->nexo;
        }
        cout << endl;
    }
}
// Funciòn ingresoTransiciones: ingresa transiciones a la estructura utilizada
// M: map utilizado para almacenar transiciones
void ingresoTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M){
  string input="",qi,qj,AR,t1,t2; // qi es estado actual, qj es estado destino, AR simbolos que se adieren al stack.
  char a,R;                       // a simbolo que se lee de la palabra, R simbolo que se lee del stack. EJ del todo: (qi,a,R)=(qj,AR).
  int i,n;                        // i es un indice utilizado para leer el ingreso y n es el tamaño del ingreso.
  bool elim;                      // booleano que explica si el ingreso hace referencia a borrar una transiciòn.
  cout << "====================================\n"
      << "Las transiciones se ingresan de la forma: (qi,a,R)=(qj,B) \n"
      << "El simbolo \'@\' representa la palabra vacia.\n"
      << "Para dejar de ingresar transiciones, escriba \'siguiente\' \n"
      << "Si necesita mas ayuda, ingrese \'ayuda\'\n";
  while(true){                                   //while para ingresar transiciones
    cout << "Ingresa la transicion: ";
    getline(cin,input);
    quitaEspacio(input);                         //Quita espacios del ingreso

    i=0;
    n=input.length();                            //las variables i,n se actualizan con el ingreso actual
    
    if(aMinuscula(input,n)=="siguiente") break;  //se sale del ingreso de transiciones
    if(aMinuscula(input,n)=="ayuda"){            //Imprime el mensaje de ayuda y se salta las validaciones siguientes para el input.
      mensajeAyuda();
      continue;
    }
    if(aMinuscula(input,n)=="visualizar"){       //Imprime las transiciones y se salta las validaciones siguientes para el input.
      imprimirTransiciones(M);
      continue;
    }
    if(n==0){                                    //se ingresa vacio
      ManejoError(1);
      continue;
    }

    if(input[i]=='-'){                           //se considera que el ingreso busca eliminar una transicion
      elim=true;
      i++;
    }
    else elim=false;

    // valida que la entrada sea de la forma (estado inicial, simbolo de la palabra a leer, simbolo al tope del stack) = (estado resultante, palabra a añadir al stack)
    if(!(validaChar(input,'(',i,n)&& validaPalabra(input,i,n,qi)&& validaChar(input,',',i,n)&& validaSimbolo(input,i,n,a))) continue;
    if(!(validaChar(input,',',i,n)&& validaSimbolo(input,i,n,R)&& validaChar(input,')',i,n)&& validaChar(input,'=',i,n))) continue;
    if(!(validaChar(input,'(',i,n)&& validaPalabra(input,i,n,qj)&& validaChar(input,',',i,n)&& validaPalabra(input,i,n,AR)&& validaChar(input,')',i,n))) continue;
    if(i<n){                                     //El ingreso es una transicion pero le proceden simbolos innecesarios
      ManejoError(3);
      continue;
    }
    if(a=='@' && R=='@'){                        //Se prohibe el ingreso de una transiciòn que ocupe palabra vacia y stack vacio (provoca while infinito). 
        ManejoError(6);
        continue;
    }
    if(elim){                                    //Si se busca eliminar la transiciòn 
      if(conseguirDatos(qi,a,R,t1,t2,M) && qj==t1 && AR==t2){ //Si existe una transiciòn con esa entrada y la salida corresponde a esta
          M.erase({qi,a,R});                                  //se borra la transiciòn
          cout<<"Eliminacion exitosa.\n";
        }
        else ManejoError(5);
    }
    else{                                        //se ingresa la transiciòn
      cout<<"Ingreso exitoso. ";
      if(M.count({qi,a,R})==1) cout << "Sobreescribiendo transicion";
      cout << endl;
      M[{qi,a,R}]={qj,AR};
    }
  }
}
// funcion quitaEspacio: quita los espacios de la palabra ingresada.
// palabra: string que se busca eliminar los espacios.
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
// funcion aMinuscula: Devuelve un string que corresponde al string ingresado pero en minuscula.
// str: string ingresado (se utiliza para intentos de ingresar: ayuda, siguiente y finalizar) en el programa
// n: tamaño del string ingresado
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
// funcion esAlfanumerico(char): Devuelve si un caracter corresponde a un valor alfanumerico o no.
// c: caracter ingresado.
bool esAlfanumerico(char c){
  int i= (int)(c); //consigue el codigo ASCII del char c
  if(48<=i && i<=57) return true; //es un numero
  if(65<=i && i<=90) return true; //es letra (mayuscula)
  if(97<=i && i<=122) return true; //es letra (minuscula)
  if(64==i) return true; //es @ (se utiliza como palabra vacia)
  return false;
}
// funcion esAlfanumerico(str): Devuelve si un string esta compuesto de caracteres alfanumericos o no.
// s: string ingresado.
bool esAlfanumerico(string s){
  int n = s.length();
  for(int i=0; i<n; i++)
    if(!esAlfanumerico(s[i])) return false;
  return true;
}
// funcion validaPalabra: Valida si una palabra, desde una posicion del string ingresado, existe.
// str: string ingresado.
// i: posiciòn del string desde el que se valida (se pasa por referencia para avanzar i)
// n: tamaño del string ingresado.
// output: variable pasada por referencia que contendra la palabra.
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
// funcion validaSimbolo: Valida si un simbolo, en una posicion del string ingresado, existe.
// str: string ingresado.
// i: posiciòn del string en la que se valida (se pasa por referencia para avanzar i)
// n: tamaño del string ingresado.
// output: variable pasada por referencia que contendra el simbolo.
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
// funcion validaChar: Valida si un char, en una posicion del string ingresado, es igual al caracter esperado.
// str: string ingresado.
// obj: caracter esperado.
// i: posiciòn del string en la que se valida (se pasa por referencia para avanzar i)
// n: tamaño del string ingresado.
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
// funcion conseguirDatos: devuelve si existe la transicion segun los datos ingresados
// qi: estado actual
// a: simbolo de la palabra a leer
// R: simbolo en el tope del stack
// qj: estado resultante (si existe, esta variable lo contendra)
// AR: palabra que se añade al stack (si existe, esta variable lo contendra)
// M: map utilizado para almacenar transiciones.
bool conseguirDatos(string qi, char a, char R, string &qj, string &AR,map<tuple<string,char,char>,tuple<string, string>> &M){
  if(M.count({qi,a,R})!=1){                   // Si no existe valor en el map para la llave compuesta del estado actual del APD {qi, a, R}
    return false;                             // Retorna falso, no existe transiciòn.
  }
  qj=get<0>(M[{qi,a,R}]);                     // Le da los valores resultantes de la transiciòn a qj y a AR
  AR=get<1>(M[{qi,a,R}]);
  return true;                                // Retorna verdadero si existe transiciòn.
}
// funcion ManejoError: recibe un valor entero que sirve como codigo para imprimir una razon de error.
// opt: valor entero ingresado
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
}
// mensaje de ayuda
void mensajeAyuda(){
  cout<<"====================================\n"
      <<"El ingreso de transiciones debe hacerse individualmente y de la forma: (qi,a,R)=(qj,B)\n"
      <<"Donde:\n\t\'qi\' es el estado actual del APD\n\t\'a\' es el simbolo de la palabra que se esta leyendo\n"
      <<"\t\'R\' es el simbolo del STACK que se esta leyendo\n\t\'qj\' es el estado destino de la transicion\n"
      <<"\t\'B\' es la palabra que se agrega al STACK\nEj:\t(q0,a,R)=(q0,AR)\n"
      <<"Para visualizar las transiciones agregadas, ingrese \'visualizar\'\n"
      <<"Para eliminar una transicion, ingrese \'-\' seguido de su transicion. Ej: -(qi,a,A)=(qj,B)\n"
      <<"Para dar termino al ingreso de transiciones, ingrese \'siguiente\'\n"
      <<"Solo debe ingresar valores alfanumericos.\nCualquier espacio en blanco es ignorado, para ingresar la palabra vacia, use \'@\'.\n";
}
// funcion imprimirTransiciones: imprime las transiciones almacenadas en el map.
// M: map utilizado para almacenar transiciones.
void imprimirTransiciones(map<tuple<string,char,char>,tuple<string, string>> &M){
  cout<<"====================================\n"
      << "Transiciones del APD:\n";
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
// funcion avanzarStack: elimina el caracter al inicio de la palabra que representa el stack. Luego añade AR al stack y devuelve el caracter a al tope del stack.
// stack: string que representa el stack.
// AR: palabra a añadir al stack.
char avanzarStack(string &stack, string AR){
    if(stack.length()>1)                            // si en el stack hay mas de 1 caracter
        stack=stack.substr(1,stack.length()-1);     // el stack se actualiza sin contemplar el primer caracter que poseia (simbolo al tope del stack)
    else stack="";                                  // caso contrario pasa a ser un string vacio
    
    if(AR!="@") stack=AR+stack;                     // si al stack no se le va a añadir la palabra vacia, a este se le añade la palabra
    
    if(stack.length()==0) return '@';               // si el stack esta vacio, retorna un simbolo que representa el vacio
    else return stack[0];                           // si no, retorna un caracter que esta al tope del stack.
}
// funcion avanzaPalabra: avanza la palabra segun el contador de posiciòn y 
// retorna el caracter que representa vacio si la posiciòn esta mas alla del largo de la palabra. Caso contrario retorna el caracter en la posicion de la palabra.
// palabra: string de la palabra que se esta leyendo.
// i: contador de posiciòn (pasado por referencia).
char avanzaPalabra(string palabra, int &i){
    i++;                                            // Se avanza el contador considerando que se avanza de simbolo
    int n=palabra.length();                         // n largo de la palabra
    if(i>=n){                                       // Si i se encuentra mas alla del largo de la palabra, retorna el simbolo que representa el vacio.
        return '@';
    }else{                                          // Si no, retorna el caracter en la siguiente posicion de la palabra.
        return palabra[i];
    }
}
// funcion imprimirDI: imprime una descripciòn instantanea 
// p: palabra que se lee
// s: stack 
// q: estado actual
// i: posiciòn de la palabra
void imprimirDI(string p, string s, string q, int i){
  int n= p.length(), j= n-i;                        // n largo de la palabra, j cantidad de simbolos que quedan en la palabra.
  string pal, stack;                                // strings que representan como se imprimiran la palabra y el stack.
  
  if(i >= n) pal = "@";                             // Si i se encuentra mas alla del largo de la palabra, utiliza el simbolo que representa el vacio como palabra.
  else pal = p.substr(i,j);                         // Si no, utiliza los simbolos que quedan por leer como palabra.

  if (s == "") stack = "@";                         // Si el stack esta vacio, utiliza el simbolo que representa el vacio como stack.
  else stack = s;                                   // Si no, utiliza directamente el stack.

  cout << "(" << q << ", " << pal << ", " << stack << ")";  // Se imprime segun lo que se verifico anteriormente.
}
// funcion insertarInfo: inserta estados finales en la lista enlazada Lista.
// eFinales: Lista enlazada ingresada.
// estadoF: estado final que se va a ingresar a la lista enlazada.
void insertarInfo(Lista &eFinales, string estadoF){
  Lista q;
  q = new(struct nodo);
  q->info = estadoF;
  q->nexo = eFinales;
  eFinales = q; 
}

#endif