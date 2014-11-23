/* K�vetelm�nyek
 * -------------------------------------------------------------------------------------------------------------------
 * A sz�veges feladatspecifik�ci� alapj�n el kell k�sz�teni a kont�ner terv�t. A tervnek ki kell t�rnie az al�bbiakra:
 *     -interf�szek specifik�ci�ja - bemeneti �s kimeneti form�tummal
 *     -bels� adatstrukt�ra
 *     -megk�t�sek: a sablonban haszn�lt t�pusoknak milyen intef�szekkel kell rendelkezni�k (pl. bool operator<())
 * A megtervezett sablont implement�lnikell.
 *     -Vigy�zzunk arra, hogy a specifik�lt interf�szek mind el�rhet�ek legyenek, ugyanakkor ne legyen olyan interf�sz,
 *      ami a specifik�ci�ban nem szerepelt, f�leg ha nem is �gy m�k�dik, ahogy az t�le elv�rt lenne.
 *      (Ez tipikus probl�ma olyan esetben, ha netal�n szabv�nyos kont�nerb�l (public) sz�rmaztatn�nk a saj�tunkat)
 *     -Tapasztalat, hogy sablont k�nnyebb fejleszteni, ha el�bb konkr�t t�pusokkal dolgozunk, s csak akkor
 *      sablonos�tjuk a kont�nert �s a f�ggv�nyeket, ha az a konkr�t t�pusunkkal megfelel�en m�k�dik.
 * Az implement�lt kont�nert tesztelnikell.
 *     -C�lszer� kipr�b�lni a kont�nert olyan "dummy" oszt�lyokkal, melyeknek semmilyen oper�toruk sincs, se default
 *      konstruktoruk. Ilyenkor a ford�t� hiba�zeneteket ad, �s ezek alapj�n ellen�rizhetj�k, hogy a konkr�t t�roland�
 *      t�pusoknak milyen interf�szekkel kell rendelkezni�k. H�tha valamit figyelmetlens�gb�l nem specifik�ltunk.
 *     -Az �rt�kel�s egyik szempontja a hat�konys�g, sk�l�zhat�s�g, teh�t hogy nagy mennyis�g� adat eset�n sem m�k�dik
 *      indokolatlanul lassan a kont�ner.
 * A forr�st -- c�lszer�en az implement�l�ssal p�ruzamosan -- dokument�lni kell doxygenst�lusban.
 *     -Minden tagv�ltoz�hoz legyen megjegyz�s f�zve.
 *     -A tagf�ggv�nyeknek legyen dokument�lva az �sszes bemeneti param�tere �s a visszat�r�si �rt�ke is.
 *
 * Feladat
 * -------------------------------------------------------------------------------------------------------------------
 * Alapfeladat
 *     K�sz�ts m�trix adatt�rol�si strukt�r�t. A t�rol�ban "flat" m�don, azaz egydimenzi�s t�mbben legyenek t�rolva az
 *     adatok (pl. vector-ban). A m�trixnak legyenek konstruktorai; sorait, oszlopait, cell�it lehessen el�rni, m�dos�tani;
 *     �s a teljes m�trixnak is legyen ki�r� �s beolvas� elj�r�sa.
 *
 * Speci�lis kieg�sz�t�k (BLRGA6 -> 7.A.b.1)
 *     -7.A.*.* Lehessen b�v�teni a m�trixot sorral, illetve oszloppal
 *     -7.*.b.* Sok sor, kev�s oszlop
 *     -7.*.*.1 A m�veletek els�sorban a m�trix elej�t �rintik
 * */

#include<vector>

using namespace std;

class FlatMatrix {

    int row_count;
    int column_count;
    vector<double> elements;


    void insert_row(vector<double>& )




};

int main(int argc, char *argv[])
{

}
