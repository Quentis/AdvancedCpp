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

            for (unsigned idx = 0; idx < row_dim * column_dim; ++idx) {
                elements.push_back(fill_value);
            }
        }
        else
        {
            this->row_dim = 0;
            this->column_dim = 0;
        }

    }

    /* Inserts a row in the matrix at the position determined by row_idx. The row_idx in the range of [1,row_dim]
     * determines an existing row so in this case the row is inserted at the place of this row and the existing is
     * shifted in the direction of increasing indexes. If the row_idx is in the range of (row_dim,infinity) then it
     * is inserted at the end of the matrix. If the row_idx is zero then the row is inserted as the first row of the
     * matrix. (same behavior as row_idx = 1) If the vector is shorter then the column_dim then false is returned.
     * If the passed vector is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    bool insert_row(const vector<double>& row_vector, unsigned row_idx)
    {
        bool success;
        unsigned row_vector_size = row_vector.size();

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
                row_dim++;
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
        unsigned column_vector_size = column_vector.size();

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
                unsigned new_size = elements.size() + row_dim;
                unsigned column_vector_idx = 0, elements_idx = 0;
                unsigned insert_idx;

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

                for(unsigned idx = 0; idx < new_size; ++idx)
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
    bool get_row_const(unsigned row_idx, vector<double> row_vector_out) const
    {
        if( (1 < row_idx) && (row_idx <= row_dim) )
        {
            vector<double> temp_vector;
            unsigned offset = get_flat_index(row_idx, 1);

            temp_vector.insert(temp_vector.begin(), elements.begin() + offset, elements.begin() + offset + column_dim);
            row_vector_out = move(temp_vector);
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Copies the elements of the column at column_idx (valid: [1,column_dim]) into the passed vector reference. (operator=)
     * If the indexes are out of range then false is returned. */
    bool get_column_const(unsigned column_idx, vector<double> column_vector_out) const
    {
        if( (1 < column_idx) && (column_idx <= row_dim) )
        {
            vector<double> temp_vector;
            unsigned column_flat_idx = elements.size() - column_dim + column_idx;

            for(unsigned idx = 0; idx < column_dim; idx++)
            {
                temp_vector.push_back(elements[column_flat_idx - idx * column_dim]);
            }

            column_vector_out = move(temp_vector);
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Overwrites an element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) with value.
     * If the indexes are out of range then a runtime_error is thrown. */
    bool modify_element(unsigned row_idx, unsigned column_idx, const double& value)
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

    /* Overwrites the row at row_idx (valid: [1,row_dim]) with values in the given vector.
     * If row_idx is out of range then false is returned. If the vector is shorter then the column_dim then false is returned.
     * If the vector is longer then column_dim then only the first column_dim elements are added to the matrix.*/
    bool modify_row(const vector<double>& row_vector, unsigned row_idx)
    {
        if( (1 < row_idx) && (row_idx <= row_dim) && (row_vector.size() != column_dim))
        {
            unsigned base = get_flat_index(row_idx, 1);
            for(unsigned offset = 0; offset < column_dim; offset++)
            {
                elements[base + offset] = row_vector[offset];
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Overwrites the column at column_idx (valid: [1,column_dim]) with values in the given vector.
     * If column_idx is out of range then false is returned. If the vector is shorter then the row_dim then false is returned.
     * If the vector is longer then row_dim then only the first row_dim elements are added to the matrix.*/
    bool modify_column(const vector<double>& column_vector, unsigned column_idx)
    {
        if( (1 < column_idx) && (column_idx <= row_dim) )
        {
            unsigned column_flat_idx = elements.size() - column_dim + column_idx;

            for(unsigned idx = 0; idx < column_dim; idx++)
            {
                elements[column_flat_idx - idx * column_dim] = column_vector[idx];
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    friend istream& operator>>(std::istream &is, FlatMatrix &m)
    {
        enum class State {
            READ,
            COLUMN,
            ROW
        } state;

        vector<double> row_vector;
        vector<vector<double>> matrix;

        string element_raw;
        vector<string> row_vector_raw;
        vector<vector<string>> matrix_raw;

        bool processing_active = true;

        state = State::READ;

        while(processing_active)
        {
            char c = cin.get();

            switch(state)
            {
                case State::READ:
                    if(c == '\n')
                    {
                        state = State::ROW;
                    }
                    else if(c == ' ')
                    {
                        state = State::COLUMN;
                    }
                    else if(c != '\r')
                    {
                        element_raw += c;
                    }
                    break;

                case State::COLUMN:
                    if(c == '\n')
                    {
                        state = State::ROW;
                    }
                    else if(c == ' ')
                    {

                    }
                    break;

                case State::ROW:
                    if(c == '\n')
                    {
                        processing_active = false;
                    }
                    else if(c == '\r')
                    {
                        state = State::READ;
                    }
                    break;
            }
        }
        return is;
    }

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
    FlatMatrix fm1;
    FlatMatrix fm2;
    FlatMatrix fm3;

    cin >> fm3;

    vector<double> column_vector1;
    column_vector1.push_back(1);
    column_vector1.push_back(2);
    column_vector1.push_back(3);

    vector<double> column_vector2;
    column_vector2.push_back(4);
    column_vector2.push_back(5);
    column_vector2.push_back(6);

    vector<double> column_vector3;
    column_vector3.push_back(7);
    column_vector3.push_back(8);
    column_vector3.push_back(9);

    fm1.insert_column(column_vector1, 1);
    cout << fm1 << endl;
    fm1.insert_column(column_vector2, 2);
    cout << fm1 << endl;
    fm1.insert_column(column_vector3, 2);
    cout << fm1 << endl;

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

    fm2.insert_row(row_vector1, 1);
    cout << fm2 << endl;
    fm2.insert_row(row_vector2, 2);
    cout << fm2 << endl;
    fm2.insert_row(row_vector3, 1);
    cout << fm2 << endl;
}
