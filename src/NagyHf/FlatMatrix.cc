/* Követelmények
 * -------------------------------------------------------------------------------------------------------------------
 * A szöveges feladatspecifikáció alapján el kell készíteni a konténer tervét. A tervnek ki kell térnie az alábbiakra:
 *     -interfészek specifikációja - bemeneti és kimeneti formátummal
 *     -belsõ adatstruktúra
 *     -megkötések: a sablonban használt típusoknak milyen intefészekkel kell rendelkezniük (pl. bool operator<())
 * A megtervezett sablont implementálnikell.
 *     -Vigyázzunk arra, hogy a specifikált interfészek mind elérhetõek legyenek, ugyanakkor ne legyen olyan interfész,
 *      ami a specifikációban nem szerepelt, fõleg ha nem is úgy mûködik, ahogy az tõle elvárt lenne.
 *      (Ez tipikus probléma olyan esetben, ha netalán szabványos konténerbõl (public) származtatnánk a sajátunkat)
 *     -Tapasztalat, hogy sablont könnyebb fejleszteni, ha elõbb konkrét típusokkal dolgozunk, s csak akkor
 *      sablonosítjuk a konténert és a függvényeket, ha az a konkrét típusunkkal megfelelõen mûködik.
 * Az implementált konténert tesztelnikell.
 *     -Célszerû kipróbálni a konténert olyan "dummy" osztályokkal, melyeknek semmilyen operátoruk sincs, se default
 *      konstruktoruk. Ilyenkor a fordító hibaüzeneteket ad, és ezek alapján ellenõrizhetjük, hogy a konkrét tárolandó
 *      típusoknak milyen interfészekkel kell rendelkezniük. Hátha valamit figyelmetlenségbõl nem specifikáltunk.
 *     -Az értékelés egyik szempontja a hatékonyság, skálázhatóság, tehát hogy nagy mennyiségû adat esetén sem mûködik
 *      indokolatlanul lassan a konténer.
 * A forrást -- célszerûen az implementálással páruzamosan -- dokumentálni kell doxygenstílusban.
 *     -Minden tagváltozóhoz legyen megjegyzés fûzve.
 *     -A tagfüggvényeknek legyen dokumentálva az összes bemeneti paramétere és a visszatérési értéke is.
 *
 * Feladat
 * -------------------------------------------------------------------------------------------------------------------
 * Alapfeladat
 *     Készíts mátrix adattárolási struktúrát. A tárolóban "flat" módon, azaz egydimenziós tömbben legyenek tárolva az
 *     adatok (pl. vector-ban). A mátrixnak legyenek konstruktorai; sorait, oszlopait, celláit lehessen elérni, módosítani;
 *     és a teljes mátrixnak is legyen kiíró és beolvasó eljárása.
 *
 * Speciális kiegészítõk (BLRGA6 -> 7.A.b.1)
 *     -7.A.*.* Lehessen bõvíteni a mátrixot sorral, illetve oszloppal
 *     -7.*.b.* Sok sor, kevés oszlop
 *     -7.*.*.1 A mûveletek elsõsorban a mátrix elejét érintik
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
