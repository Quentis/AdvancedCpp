// multibyte character
//  - mblen()

#include <iostream>
#include <cstdlib>
#include <clocale>
using namespace std;

 // megadjuk a beállítandó LOCALE-t
#define LOCALE_DEF "en_US.utf8"
//#define LOCALE_DEF "en_US.iso-8859-1"
//#define LOCALE_DEF "C"

//int main(int argc, char *argv[])
//{
//    char mb0[]="Aa";    // ez itt a referenciasztring
//    char mb1[]="ó"; // ez a szting - utf8-ként értelmezve egyetlen karaktert tartalmaz
//
//    char *loc=setlocale(LC_CTYPE,LOCALE_DEF);   // beállítjük, hogy a multibyte char-ral dolgozó fv.-ek
//                        // milyen locale-lal dolgozzanak
//    cerr<<"setlocale returned "<<loc<<endl;
//
//        // mblen: megállapítja, hogy a sztring első értéke hány karaktert foglal el
//    cout<<"mblen("<<mb0<<",2): "<<mblen(mb0,2)<<endl;
//    cout<<"mblen("<<mb1<<",2): "<<mblen(mb1,2)<<endl;
//    cout<<"***"<<endl<<endl;
//
//        // mblen 2. paramétere: max. hány karaktert értelmezzen a megadott sztringből
//    cout<<"mblen("<<mb0<<",1): "<<mblen(mb0,1)<<endl;
//    cout<<"mblen("<<mb1<<",1): "<<mblen(mb1,1)<<endl;
//
//    return 0;
//}
