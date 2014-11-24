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
#include<iterator>
#include<stdexcept>
#include<algorithm>

using namespace std;

/* Template*/
class FlatMatrix {
private:

    unsigned row_dim;    /// Number of rows in the matrix (it can be zero but in this case column_dim should be zero too)
    unsigned column_dim; /// Number of columns in the matrix (it can be zero but in this case row_dim should be zero too)

    /** The elements of the matrix are stored in a vector. The vector is row organized which means the row members are
     *  next to each other (consecutive indexes) so rows can be added and read faster then the columns. The rows are
     *  stored in reverse order because specification states that the operations affects the first rows of the matrix
     *  (the insertion to the vector container is fast if the elemnts are added at the end of the container).
     *  [a11 a12 a13]
     *  [a21 a22 a23] --> [a31 a32 a33 a21 a22 a23 a11 a12 a13]
     *  [a31 a32 a33] */
    vector<double> elements;

private:
    unsigned get_flat_index(unsigned row_idx, unsigned column_idx)
    {
        if((row_idx > row_dim) || (column_idx > column_dim) || (row_idx == 0) || (column_idx == 0))
        {
            throw runtime_error("Invalid dimensions.");
        }

        return (row_dim - row_idx) * column_dim + (column_idx - 1);
    }

    void insert_row_vector(const vector<double>& row_vector, unsigned row_idx)
    {
        if((row_dim != 0) && (column_dim != 0))
        {
            if(row_vector.size() != column_dim)
            {
                throw runtime_error("Row vector and FlatMatrix dimension mismatch.");
            }

        }
        else if((row_dim == 0) && (column_dim == 0))
        {
            int row_vector_size = row_vector.size();

            if(row_vector_size == 0)
            {
                throw runtime_error("Row vector dimension can't be zero.");
            }

            row_dim = 1;
            column_dim = row_vector_size;

            elements = row_vector;
        }
        else
        {
            throw runtime_error("Invalid FlatMatrix dimensions.");
        }
    }

    void insert_column_vector(vector<double> column_vector, unsigned column_idx)
    {

    }

public:
    /** Creates an empty matrix (rows and columns can be added later)*/
    FlatMatrix()
    {
        row_dim = 0;
        column_dim = 0;
    }

    /* Creates a matrix with row_dim and column_dim dimensions and initializes it with fill_value.
     * If the dimensions are both zeros then it creates an empty matrix. If one of the dimension
     * parameters is zero and the other is not it is considered invalid and a runtime_error is thrown. */
    FlatMatrix(unsigned row_dim, unsigned column_dim, double& fill_value)
    {
        if((row_dim == 0) && (column_dim == 0))
        {
            this->row_dim = 0;
            this->column_dim = 0;
        }
        else if((row_dim == 0) && (column_dim == 0))
        {
            this->row_dim = row_dim;
            this->column_dim = column_dim;

            for (int idx = 0; idx < row_dim * column_dim; ++idx) {
                elements.push_back(fill_value);
            }
        }
        else
        {
            throw runtime_error("Invalid FlatMatrix dimensions.");
        }

    }

    /** Creates a matrix from an iterator container based on the dimension parameters. If the dimensions are both zeros
      * then it creates an empty matrix. If one of the dimension parameters is zero and the other is not it is considered
      * invalid and a runtime_error is thrown.
      * The iterator should contain the values of the matrix. If the row_organized parameter is true then it means
      * the elements of the rows otherwise the columns are consecutive in the iterator.
      *  [a11 a12 a13]
      *  [a21 a22 a23] --> [a11 a12 a13 a21 a22 a23 a31 a32 a33]    //row_organized is true
      *  [a31 a32 a33]     [a11 a21 a31 a12 a22 a32 a13 a23 a33]    //row_organized is false
      */
    FlatMatrix(vector<double>::iterator row_begin, vector<double>::iterator row_end,
               unsigned row_dim, unsigned column_dim, bool row_organized)
    {

    }

    /* Inserts a row in the matrix at the position determined by row_idx. The row_idx in the range of [1,row_dim]
     * determines an existing row so in this case the row is inserted at the place of this row and the existing is
     * shifted in the direction of increasing indexes. If the row_idx is in the range of (row_dim,infinity) then it
     * is inserted at the end of the matrix. If the row_idx is zero then the row is inserted as the first row of the
     * matrix. (same behavior as row_idx = 1) If the iterator is shorter then the column_dim then the undefined
     * elements has the value of *fill_value however if in this case the fill_value is nullptr then a runtime_error is
     * thrown. If the iterator is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    bool insert_row(vector<double>::iterator row_begin, vector<double>::iterator row_end, unsigned row_idx, double* fill_value)
    {

    }

    /* Inserts a column in the matrix at the position determined by column_idx. The column_idx in the range of [1,column_dim]
     * determines an existing column so in this case the column is inserted at the place of this column and the existing is
     * shifted in the direction of increasing indexes. If the column_idx is in the range of (column_dim,infinity) then it
     * is inserted at the end of the matrix. If the column_idx is zero then the column is inserted as the first column of the
     * matrix. (same behavior as column_idx = 1) If the iterator is shorter then the row_dim then the undefined
     * elements has the value of *fill_value however if in this case the fill_value is nullptr then a runtime_error is
     * thrown. If the iterator is longer then row_dim then only the first row_dim elements are added to the matrix.*/
    bool insert_column(vector<double>::iterator column_begin, vector<double>::iterator column_end, unsigned column_idx, double* fill_value)
    {

    }

    /* Returns the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]).
     * If the indexes are out of range then a runtime_error is thrown. */
    double get_element_const(unsigned row_idx, unsigned column_idx) const
    {

    }

    /* Returns a reference to the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]).
     * If the indexes are out of range then a runtime_error is thrown. */
    double& get_element(unsigned row_idx, unsigned column_idx)
    {

    }

    /* Returns a vector with the values of the row at row_idx (valid: [1,row_dim]).
     * If row_idx is out of range then a runtime_error is thrown. */
    vector<double> get_row_const(unsigned row_idx) const
    {

    }

    /* Returns a vector with the values of the column at column_idx (valid: [1,column_dim]).
     * If column_idx is out of range then a runtime_error is thrown. */
    vector<double> get_column_const(unsigned row_idx) const
    {

    }

    /* Overwrites an element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) with value.
     * If the indexes are out of range then a runtime_error is thrown. */
    void modify_element(unsigned row_idx, unsigned column_idx, double value)
    {

    }

    /* Overwrites the row at row_idx (valid: [1,row_dim]) with values in the given iterator.
     * If row_idx is out of range then a runtime_error is thrown. If the iterator is shorter then the column_dim then the
     * undefined elements has the value of *fill_value however if in this case the fill_value is nullptr then a runtime_error is
     * thrown. If the iterator is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    void modify_row(vector<double>::iterator row_begin, vector<double>::iterator row_end, unsigned row_idx, double* fill_value)
    {

    }

    /* Overwrites the column at column_idx (valid: [1,column_dim]) with values in the given iterator.
     * If column_idx is out of range then a runtime_error is thrown. If the iterator is shorter then the row_dim then the
     * undefined elements has the value of *fill_value however if in this case the fill_value is nullptr then a runtime_error is
     * thrown. If the iterator is longer then row_dim then only the first row_dim elements are added to the matrix.*/
    void modify_column(vector<double>::iterator row_begin, vector<double>::iterator row_end, unsigned column_idx, double* fill_value)
    {

    }

    istream &operator>>(std::istream &is, FlatMatrix &m)
    {

    }

    ostream &operator<<(std::ostream &os, const FlatMatrix &m)
    {

    }

};

int main(int argc, char *argv[])
{

}
