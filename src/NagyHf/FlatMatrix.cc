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
#include<iterator>
#include<stdexcept>
#include<algorithm>
#include<iostream>
#include<utility>

using namespace std;

/* Template*/
class FlatMatrix {
private:
    unsigned row_dim;    /// Number of rows in the matrix (it can be zero but in this case column_dim should be zero too)
    unsigned column_dim; /// Number of columns in the matrix (it can be zero but in this case row_dim should be zero too)

    /** The elements of the matrix are stored in a vector. The vector is row organized which means the row members are
     *  next to each other (consecutive indexes) so rows can be added and read faster then the columns. The rows are
     *  stored in reverse order because specification states that the operations affects the first rows of the matrix
     *  (the insertion to the vector container is fast if the elements are added at the end of the container).
     *  [a11 a12 a13]
     *  [a21 a22 a23] --> [a31 a32 a33 a21 a22 a23 a11 a12 a13]
     *  [a31 a32 a33] */
    vector<double> elements;

private:
    unsigned get_flat_index(unsigned row_idx, unsigned column_idx) const
    {
        /* This makes possible to get the flat index of the next columns and rows after the last ones
         * in order to help the inserter functions */
        if((row_idx > row_dim + 1) || (column_idx > column_dim + 1) || (row_idx == 0) || (column_idx == 0))
        {
            throw runtime_error("Invalid dimensions.");
        }

        return (row_dim - row_idx) * column_dim + (column_idx - 1);
    }

public:
    /** Creates an empty matrix (rows and columns can be added later)*/
    FlatMatrix()
    {
        row_dim = 0;
        column_dim = 0;
    }

    /* Creates a matrix with row_dim and column_dim dimensions and initializes it with fill_value.
     * If any dimension is zero then it creates an empty matrix. */
    FlatMatrix(unsigned row_dim, unsigned column_dim, double& fill_value)
    {
        if( (row_dim == 0) || (column_dim == 0) )
        {
            this->row_dim = row_dim;
            this->column_dim = column_dim;

            for (int idx = 0; idx < row_dim * column_dim; ++idx) {
                elements.push_back(fill_value);
            }
        }
        else
        {
            this->row_dim = 0;
            this->column_dim = 0;
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
    // TODO: Currently not supported
    //FlatMatrix(vector<double>::iterator row_begin, vector<double>::iterator row_end,
    //           unsigned row_dim, unsigned column_dim, bool row_organized)
    //{
    //
    //}

    /* Inserts a row in the matrix at the position determined by row_idx. The row_idx in the range of [1,row_dim]
     * determines an existing row so in this case the row is inserted at the place of this row and the existing is
     * shifted in the direction of increasing indexes. If the row_idx is in the range of (row_dim,infinity) then it
     * is inserted at the end of the matrix. If the row_idx is zero then the row is inserted as the first row of the
     * matrix. (same behavior as row_idx = 1) If the vector is shorter then the column_dim then false is returned.
     * If the passed vector is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    bool insert_row(const vector<double>& row_vector, unsigned row_idx)
    {
        bool success;
        size_t row_vector_size = row_vector.size();

        if((row_dim == 0) || (column_dim == 0))
        {   /* The matrix is empty */

            if(row_vector_size == 0)
            {
                success = false;
            }
            else
            {
                row_dim = 1;
                column_dim = row_vector_size;

                elements = row_vector;

                success = true;
            }
        }
        else
        {   /* The matrix is not empty => strict dimension checks */

            if(row_vector_size >= column_dim)
            {
                vector<double>::iterator vi_dst;

                if(row_idx >= row_dim)
                {
                    vi_dst = elements.begin();
                }
                else if(row_idx <= 1)
                {
                    vi_dst = elements.end();
                }
                else
                {
                    unsigned row_start_flat_idx = get_flat_index(row_idx, 1);
                    vi_dst = elements.begin() + row_start_flat_idx;
                }

                elements.insert(vi_dst, row_vector.begin(), row_vector.end());

                success = true;
            }
            else
            {
                success = false;
            }
        }
        return success;
    }

    /* Inserts a column in the matrix at the position determined by column_idx. The column_idx in the range of [1,column_dim]
     * determines an existing column so in this case the column is inserted at the place of this column and the existing is
     * shifted in the direction of increasing indexes. If the column_idx is in the range of (column_dim,infinity) then it
     * is inserted at the end of the matrix. If the column_idx is zero then the column is inserted as the first column of the
     * matrix. (same behavior as column_idx = 1) If the vector is shorter then the row_dim then false is returned.
     * If the passed vector is longer then row_dim then only the first row_dim elements are added to the matrix.*/
    bool insert_column(const vector<double>& column_vector, unsigned column_idx)
    {
        bool success;
        size_t column_vector_size = column_vector.size();

        if((column_dim == 0) || (row_dim == 0))
        {   /* The matrix is empty */

            if(column_vector_size == 0)
            {
                success = false;
            }
            else
            {
                column_dim = 1;
                row_dim = column_vector_size;

                for(vector<double>::const_reverse_iterator vi = column_vector.rbegin(); vi != column_vector.rend();++vi)
                {
                    elements.push_back(*vi);
                }

                success = true;
            }
        }
        else
        {   /* The matrix is not empty => strict dimension checks */
            if(column_vector_size >= row_dim)
            {
                size_t new_size = elements.size() + row_dim;
                size_t column_vector_idx = 0, elements_idx = 0;
                size_t insert_idx;

                if(column_idx < 1)
                {
                    insert_idx = 0;
                }
                else if(column_idx > column_dim)
                {
                    insert_idx = column_dim;
                }
                else
                {
                    insert_idx = column_idx - 1;
                }

                vector<double> temp_vector(new_size);

                column_dim++;

                for(size_t idx = 0; idx < new_size; ++idx)
                {   if(idx != insert_idx)
                    {
                        temp_vector[idx] = elements[elements_idx];
                        ++elements_idx;
                    }
                    else
                    {
                        temp_vector[idx] = column_vector[column_vector_size - column_vector_idx - 1];
                        ++column_vector_idx;
                        insert_idx += column_dim;
                    }
                }

                elements = move(temp_vector);
                success = true;
            }
            else
            {
                success = false;
            }
        }
        return success;
    }

    /* Copies the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) into
     * the passed reference. If the indexes are out of range then false is returned. */
    bool get_element_const(unsigned row_idx, unsigned column_idx, double& element_out) const
    {
        if( (row_idx > row_dim) || (row_idx < 1) || (column_idx > column_dim) || (column_idx < 1))
        {
            return false;
        }
        else
        {
            elements[get_flat_index(row_idx, column_idx)];
            return true;
        }
    }

    /* Returns a reference to the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]).
     * If the indexes are out of range then false is returned. */
    double& get_element(unsigned row_idx, unsigned column_idx)
    {
        return elements[get_flat_index(row_idx, column_idx)];
    }

    /* Copies the elements of the row at row_idx (valid: [1,row_dim]) into the passed vector reference. (operator=)
     * If the indexes are out of range then false is returned. */
    bool get_row_const(unsigned row_idx, vector<double>) const
    {

    }

    /* Copies the elements of the column at column_idx (valid: [1,column_dim]) into the passed vector reference. (operator=)
     * If the indexes are out of range then false is returned. */
    bool get_column_const(unsigned column_idx, vector<double>) const
    {

    }

    /* Overwrites an element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) with value.
     * If the indexes are out of range then a runtime_error is thrown. */
    bool modify_element(unsigned row_idx, unsigned column_idx, double value)
    {

    }

    /* Overwrites the row at row_idx (valid: [1,row_dim]) with values in the given vector.
     * If row_idx is out of range then false is returned. If the vector is shorter then the column_dim then false is returned.
     * If the vector is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    bool modify_row(vector<double> row_vector, unsigned row_idx)
    {

    }

    /* Overwrites the column at column_idx (valid: [1,column_dim]) with values in the given vector.
     * If column_idx is out of range then false is returned. If the vector is shorter then the row_dim then false is returned.
     * If the vector is longer then row_dim then only the first row_dim elements are added to the matrix.*/
    bool modify_column(vector<double> column_vector, unsigned column_idx)
    {

    }

    //friend istream &operator>>(std::istream &is, FlatMatrix &m)
    //{

    //}

    friend ostream& operator<<(std::ostream &os, const FlatMatrix &m)
    {
        for (unsigned i = 1; i <= m.row_dim; ++i) {
            for (unsigned j = 1; j <= m.column_dim; ++j) {
                cout << m.elements[m.get_flat_index(i,j)];
                if(j != m.column_dim)
                {
                    cout << " ";
                }
                else
                {
                    cout << endl;
                }
            }
        }
        return os;
    }

};

int main(int argc, char *argv[])
{
    FlatMatrix fm;

//    vector<double> column_vector1;
//    column_vector1.push_back(1);
//    column_vector1.push_back(2);
//    column_vector1.push_back(3);
//
//    vector<double> column_vector2;
//    column_vector2.push_back(4);
//    column_vector2.push_back(5);
//    column_vector2.push_back(6);
//
//    vector<double> column_vector3;
//    column_vector3.push_back(7);
//    column_vector3.push_back(8);
//    column_vector3.push_back(9);
//
//    fm.insert_column(column_vector1, 1);
//    cout << fm << endl;
//    fm.insert_column(column_vector2, 2);
//    cout << fm << endl;
//    fm.insert_column(column_vector3, 2);
//    cout << fm << endl;

    vector<double> row_vector1;
    row_vector1.push_back(1);
    row_vector1.push_back(2);
    row_vector1.push_back(3);

    vector<double> row_vector2;
    row_vector2.push_back(4);
    row_vector2.push_back(5);
    row_vector2.push_back(6);

    vector<double> row_vector3;
    row_vector3.push_back(7);
    row_vector3.push_back(8);
    row_vector3.push_back(9);

    fm.insert_row(row_vector1, 1);
    cout << fm << endl;
    fm.insert_row(row_vector2, 2);
    cout << fm << endl;
    fm.insert_row(row_vector3, 1);
    cout << fm << endl;
}
