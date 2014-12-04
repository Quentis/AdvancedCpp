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

/// \mainpage FlatMatrix project
/// \ref FlatMatrix is a container which makes possible to store objects in matrix structure. The structure of the FlatMatrix is
/// flat which means the elements are stored consecutively in a dynamic array (vector). The matrix contains the rows in
/// reverse order in a row oriented way (the elements of the row are next to each other in the array). This makes the insertion
/// of rows at the beggining of the matrix fast.

#include<vector>
#include<iterator>
#include<stdexcept>
#include<algorithm>
#include<iostream>
#include<utility>
#include<string>
#include<sstream>

using namespace std;

#define FLAT_MATRIX_DEBUG_SUPPORT

/* Template*/
class FlatMatrix {
private:
    /// Number of rows in the matrix (it can be zero but in this case column_dim should be zero too)
    unsigned row_dim;

    /// Number of columns in the matrix (it can be zero but in this case row_dim should be zero too)
    unsigned column_dim;

    /// The elements of the matrix are stored in a vector. The vector is row organized which means the row members are
    /// next to each other (consecutive indexes) so rows can be added and read faster then the columns. The rows are
    /// stored in reverse order because specification states that the operations affects the first rows of the matrix
    /// (the insertion to the vector container is fast if the elements are added at the end of the container).
    /// |         |  row1  |  row2  |  row3  |
    /// |:-------:|:------:|:------:|:------:|
    /// | column1 |  a11   |  a21   |  a31   |
    /// | column2 |  a21   |  a22   |  a32   |
    /// | column3 |  a23   |  a23   |  a33   |
    /// This matrix is stored in the following way:
    /// |         |  idx0  |  idx1  |  idx2  |  idx3  |  idx4  |  idx5  |  idx6  |  idx7  |  idx8  |
    /// |:-------:|:------:|:------:|:------:|:------:|:------:|:------:|:------:|:------:|:------:|
    /// |  value  |  a31   |  a32   |  a33   |  a21   |  a22   |  a23   |  a11   |  a12   |  a13   |
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
    /// Creates an empty matrix (rows and columns can be added later)
    FlatMatrix()
    {
        row_dim = 0;
        column_dim = 0;
    }

    /// Creates a matrix with row_dim and column_dim dimensions and initializes it with fill_value.
    /// If any dimension is zero then it creates an empty matrix.
    /// \param row_dim determines the number of rows
    /// \param column_dim determines the number of columns
    /// \param fill_value determines the value of elements in the matrix (operator=)
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

    /// Copy constructor. It copies the values of the other matrix in accordance with its dimensions. The dimensions
    /// are overwritten also. After the copy the two matrix have the same dimensions and elements however both matrix
    /// has its own copy. (deep copy)
    /// \param fm other FlatMatrix instance
    FlatMatrix(const FlatMatrix& fm)
    {
        row_dim = fm.row_dim;
        column_dim = fm.column_dim;
        elements = fm.elements;
    }

    /// Move constructor. It moves the values of the other matrix in accordance with its dimensions. The dimensions
    /// are overwritten also. After the move the other matrix becomes empty (no elements and the dimensions are zero).
    /// \param fm other FlatMatrix instance
    FlatMatrix(FlatMatrix&& fm)
    {
        row_dim = fm.row_dim;
        column_dim = fm.column_dim;
        fm.row_dim = 0;
        fm.column_dim = 0;
        elements = move(fm.elements);
        /* According to the specification of vector after move assignment the source remains in valid but undefined state.
         * The clear makes sure that no element remains in the source matrix. */
        fm.clear();
    }

    /// Copy assignment. It copies the values of the other matrix in accordance with its dimensions. The dimensions
    /// are overwritten also. After the copy the two matrix have the same dimensions and elements however both matrix
    /// has its own copy. (deep copy)
    /// \param fm other FlatMatrix instance
    /// \return the overwritten FlatMatrix object
    FlatMatrix& operator=(const FlatMatrix& fm)
    {
        if(this != &fm)
        {
            row_dim = fm.row_dim;
            column_dim = fm.column_dim;
            elements = fm.elements;
        }
        return *this;
    }

    /// Move assignment. It moves the values of the other matrix in accordance with its dimensions. The dimensions
    /// are overwritten also. After the move the other matrix becomes empty (no elements and the dimensions are zero).
    /// \param fm other FlatMatrix instance
    /// \return the overwritten FlatMatrix object
    FlatMatrix& operator=(FlatMatrix&& fm)
    {
        if(this != &fm)
        {
            row_dim = fm.row_dim;
            column_dim = fm.column_dim;
            fm.row_dim = 0;
            fm.column_dim = 0;
            elements = move(fm.elements);
            /* According to the specification of vector after move assignment the source remains in valid but undefined state.
             * The clear makes sure that no element remains in the source matrix. */
            fm.clear();
        }
        return *this;
    }

    /// Clears the elements of the matrix and sets the dimensions to zero.
    void clear(void)
    {
        row_dim = 0;
        column_dim = 0;
        elements.clear();
    }

    /// Inserts a row in the matrix at the position determined by row_idx. The row_idx in the range of [1,row_dim]
    /// determines an existing row so in this case the row is inserted at the place of this row and the existing is
    /// shifted in the direction of increasing indexes. If the row_idx is in the range of (row_dim,infinity) then it
    /// is inserted at the end of the matrix. If the row_idx is zero then the row is inserted as the first row of the
    /// matrix. (same behavior as row_idx = 1) If the vector is shorter then the column_dim then false is returned.
    /// If the passed vector is longer then column_dim then only the first column_dim elements are added to the matrix.
    /// \param row_vector determines the values of the new row
    /// \param row_idx determines the insertion point
    /// \return true if the insertion was successful (otherwise false)
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

    /// Inserts a column in the matrix at the position determined by column_idx. The column_idx in the range of [1,column_dim]
    /// determines an existing column so in this case the column is inserted at the place of this column and the existing is
    /// shifted in the direction of increasing indexes. If the column_idx is in the range of (column_dim,infinity) then it
    /// is inserted at the end of the matrix. If the column_idx is zero then the column is inserted as the first column of the
    /// matrix. (same behavior as column_idx = 1) If the vector is shorter then the row_dim then false is returned.
    /// If the passed vector is longer then row_dim then only the first row_dim elements are added to the matrix.
    /// \param column_vector determines the values of the new column
    /// \param column_idx determines the insertion point
    /// \return true if the insertion was successful (otherwise false)
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

    /// \return the number of rows in the matrix.
    unsigned get_row_dim(void)
    {
        return row_dim;
    }

    /// \return the number of columns in the matrix.
    unsigned get_column_dim(void)
    {
        return column_dim;
    }

    /// Copies the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) into
    /// the passed reference. If the indexes are out of range then false is returned.
    /// \param row_idx determines the row index of the required element
    /// \param column_idx determines the column index of the required element
    /// \param element_out is the variable where the required value is copied
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// Returns a reference to the value of the element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]).
    /// If the indexes are out of range then runtime_error is thrown.
    /// \param row_idx determines the row index of the required element
    /// \param column_idx determines the column index of the required element
    /// \return a reference to the required object
    double& get_element(unsigned row_idx, unsigned column_idx)
    {
        return elements[get_flat_index(row_idx, column_idx)];
    }

    /// Copies the elements of the row at row_idx (valid: [1,row_dim]) into the passed vector reference. (operator=)
    /// If the indexes are out of range then false is returned.
    /// \param row_idx determines the required row
    /// \param row_vector_out determines where the elements in the required row shall be copied
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// Copies the elements of the column at column_idx (valid: [1,column_dim]) into the passed vector reference. (operator=)
    /// If the indexes are out of range then false is returned.
    /// \param column_idx determines the required column
    /// \param column_vector_out determines where the elements in the required column shall be copied
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// Overwrites an element at row_idx (valid: [1,row_dim]) and column_idx (valid: [1,column_dim]) with value.
    /// If the indexes are out of range then a runtime_error is thrown.
    /// \param row_idx determines the row index of the required element
    /// \param column_idx determines the column index of the required element
    /// \param value is the new value of the element at the given indexes
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// Overwrites the row at row_idx (valid: [1,row_dim]) with values in the given vector.
    /// If row_idx is out of range then false is returned. If the vector is shorter then the column_dim then false is returned.
    /// If the vector is longer then column_dim then only the first column_dim elements are added to the matrix.
    /// \param row_vector determines the row which overwrites the selected (row_idx) row in the matrix
    /// \param row_idx determines the required row
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// Overwrites the column at column_idx (valid: [1,column_dim]) with values in the given vector.
    /// If column_idx is out of range then false is returned. If the vector is shorter then the row_dim then false is returned.
    /// If the vector is longer then row_dim then only the first row_dim elements are added to the matrix.
    /// \param column_vector determines the column which overwrites the selected (column_idx) column in the matrix
    /// \param column_idx determines the required column
    /// \return true if the passed indexes are in range of dimensions (otherwise false)
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

    /// This auxiliary function converts a string in the src parameter into the specified type in the template parameter
    /// by calling its operator>> which stores the result in the out parameter. If the conversion is successful then
    /// true is returned otherwise false.
    /// \param src is the string which shall be converted
    /// \param out is a reference where the result is stored
    /// \return true if the conversion was successful. (otherwise false)
    static bool converter(const string& src, double& out)
    {
        stringstream str_stream(src, ios_base::in);
        str_stream >> out;

        if((str_stream.failbit || str_stream.badbit) && !str_stream.eof())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    friend istream& operator>>(std::istream &is, FlatMatrix &m)
    {
        enum class State {
            START,
            READ
        } state;

        vector<double> row_vector;
        vector<vector<double>> matrix;

        string element_raw;
        double element;

        unsigned column_dim = 0;
        bool processing_active = true;
        bool success = true;

        state = State::START;

        while(processing_active)
        {
            char c = cin.get();

            switch(state)
            {
                case State::START:
                    if(c == '{')
                    {
                        state = State::READ;
                    }
                    break;
                case State::READ:
                    if((c == ';') || (c == '}'))
                    {
                        if(!element_raw.empty())
                        {
                            if(FlatMatrix::converter(element_raw, element))
                            {
                                row_vector.push_back(element);
                                element_raw.clear();
                            }
                            else
                            {
                                processing_active = false;
                                success = false;
                            }
                            if((column_dim != row_vector.size()) && (column_dim != 0))
                            {
                                processing_active = false;
                                success = false;
                            }
                            else
                            {
                                if(column_dim == 0)
                                {
                                    column_dim = row_vector.size();
                                }

                                if(c == '}')
                                {
                                    processing_active = false;
                                    success = true;
                                }

                                    matrix.push_back(row_vector);
                                    row_vector.clear();
                            }
                        }
                        else
                        {
                            if(c == '}')
                            {
                                processing_active = false;
                                success = true;
                            }
                            else
                            {
                                processing_active = false;
                                success = false;
                            }
                        }
                    }
                    else if(c == ',')
                    {
                        if(!element_raw.empty())
                        {
                            if(FlatMatrix::converter(element_raw, element))
                            {
                                row_vector.push_back(element);
                                element_raw.clear();
                            }
                            else
                            {
                                processing_active = false;
                                success = false;
                            }
                        }
                        else
                        {
                            processing_active = false;
                            success = false;
                        }
                    }
                    else
                    {
                        element_raw += c;
                    }
                    break;
            }
        }

        if(success)
        {
            m.clear();
            if(matrix.size() > 0)
            {
                int idx = matrix.size() - 1;
                for(;idx >= 0; idx--)
                {
                    m.insert_row(matrix[idx], 0);
                }
            }
        }
        return is;
    }

    friend ostream& operator<<(std::ostream &os, const FlatMatrix &m)
    {
        cout << '{';
        for (unsigned i = 1; i <= m.row_dim; ++i) {
            for (unsigned j = 1; j <= m.column_dim; ++j) {
                cout << m.elements[m.get_flat_index(i,j)];
                if(j != m.column_dim)
                {
                    cout << ',';
                }
                else
                {
                    cout << ';';
                }
            }
        }
        cout << '}' << endl;
        return os;
    }

    #ifdef FLAT_MATRIX_DEBUG_SUPPORT
    void print_elements(void)
    {
        for(vector<double>::iterator vi = elements.begin(); vi != elements.end(); ++vi)
        {
            cout << *vi << ' ';
        }
        cout << endl;
    }
    #endif

};

int main(int argc, char *argv[])
{
    FlatMatrix fm1;
    FlatMatrix fm2;
    FlatMatrix fm3;
    FlatMatrix fm_in;

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
//    fm1.insert_column(column_vector1, 1);
//    cout << fm1 << endl;
//    fm1.insert_column(column_vector2, 2);
//    cout << fm1 << endl;
//    fm1.insert_column(column_vector3, 2);
//    cout << fm1 << endl;
//
//    vector<double> row_vector1;
//    row_vector1.push_back(1);
//    row_vector1.push_back(2);
//    row_vector1.push_back(3);
//
//    vector<double> row_vector2;
//    row_vector2.push_back(4);
//    row_vector2.push_back(5);
//    row_vector2.push_back(6);
//
//    vector<double> row_vector3;
//    row_vector3.push_back(7);
//    row_vector3.push_back(8);
//    row_vector3.push_back(9);
//
//    fm2.insert_row(row_vector1, 1);
//    cout << fm2 << endl;
//    fm2.insert_row(row_vector2, 2);
//    cout << fm2 << endl;
//    fm2.insert_row(row_vector3, 1);
//    cout << fm2 << endl;
//
//    fm1 = fm2;
//    fm1.print_elements();
//    fm2.print_elements();
//    fm1 = move(fm2);
//    fm1.print_elements();
//    fm2.print_elements();

    while(1)
    {
        cout << "Give me that stuff!" << endl;
        cin >> fm_in;
        cout << "Here is your staff" << endl;
        cout << fm_in;

        cout << endl << endl;
    }
    cin.get();
}
