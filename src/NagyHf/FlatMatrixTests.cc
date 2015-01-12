#include "FlatMatrix.h"
#include<iostream>
#include<iomanip>
#include<cfloat>
#include<cmath>

/*============================================ TESTS ============================================*/
bool FlatMatrix_TestCase_Int_DefaultConstructor(void)
{
    bool test_result = false;

    FlatMatrix<int> fm;

    if( (fm.get_row_dim() == 0) && (fm.get_column_dim() == 0))
    {
        test_result = true;
    }

    return test_result;
}

bool FlatMatrix_TestCase_Int_FillConstructor(void)
{
    FlatMatrix<int> fm_row_dim_zero(0,1,122);
    FlatMatrix<int> fm_column_dim_zero(1,0,122);
    FlatMatrix<int> fm(2,3,122);

    if( (fm_row_dim_zero.get_row_dim() != 0) || (fm_row_dim_zero.get_column_dim() != 0))
    {
        return false;
    }

    if( (fm_column_dim_zero.get_row_dim() != 0) || (fm_column_dim_zero.get_column_dim() != 0))
    {
        return false;
    }

    if( (fm.get_row_dim() != 2) || (fm.get_column_dim() != 3) )
    {
        return false;
    }
    else
    {
        int row1_checker[] = {122, 122, 122};
        int row2_checker[] = {122, 122, 122};
        int column1_checker[] = {122, 122};
        int column2_checker[] = {122, 122};
        int column3_checker[] = {122, 122};

        vector<int> row1;
        vector<int> row2;
        vector<int> column1;
        vector<int> column2;
        vector<int> column3;

        if(!fm.get_row(1, row1))
        {
            return false;
        }

        if(!fm.get_row(2, row2))
        {
            return false;
        }

        if(!fm.get_column(1, column1))
        {
            return false;
        }

        if(!fm.get_column(2, column2))
        {
            return false;
        }

        if(!fm.get_column(3, column3))
        {
            return false;
        }

        for (int idx = 0; idx < 3; ++idx)
        {
            if( (row1_checker[idx] != row1[idx]) ||
                (row2_checker[idx] != row2[idx]) )
            {
                return false;
            }
        }

        for (int idx = 0; idx < 2; ++idx)
        {
            if( (column1_checker[idx] != column1[idx]) ||
                (column2_checker[idx] != column2[idx]) ||
                (column3_checker[idx] != column3[idx]) )
            {
                return false;
            }
        }
    }

    return true;
}

bool FlatMatrix_TestCase_Int_RowInsert(void)
{
    FlatMatrix<int> fm_row;
    vector<int> row_vector, row_vector_out;

    row_vector.push_back(1);
    row_vector.push_back(2);
    row_vector.push_back(3);

    if(!fm_row.insert_row(row_vector, 1))
    {
        return false;
    }

    row_vector[0] = row_vector[0] + 3;
    row_vector[1] = row_vector[1] + 3;
    row_vector[2] = row_vector[2] + 3;
    if(!fm_row.insert_row(row_vector, 1))
    {
        return false;
    }

    row_vector[0] = row_vector[0] + 3;
    row_vector[1] = row_vector[1] + 3;
    row_vector[2] = row_vector[2] + 3;
    if(!fm_row.insert_row(row_vector, 2))
    {
        return false;
    }

    if(!fm_row.get_row(1, row_vector_out))
    {
        return false;
    }

    if( (row_vector_out[0] != 4) ||
        (row_vector_out[1] != 5) ||
        (row_vector_out[2] != 6)
    ){
        return false;
    }

    if(!fm_row.get_row(2, row_vector_out))
    {
        return false;
    }

    if( (row_vector_out[0] != 7) ||
        (row_vector_out[1] != 8) ||
        (row_vector_out[2] != 9)
    ){
        return false;
    }

    if(!fm_row.get_row(3, row_vector_out))
    {
        return false;
    }

    if( (row_vector_out[0] != 1) ||
        (row_vector_out[1] != 2) ||
        (row_vector_out[2] != 3)
    ){
        return false;
    }

    return true;
}

bool FlatMatrix_TestCase_Int_ColumnInsert(void)
{
    FlatMatrix<int> fm_column;
    vector<int> column_vector, column_vector_out;

    column_vector.push_back(1);
    column_vector.push_back(2);
    column_vector.push_back(3);

    if(!fm_column.insert_column(column_vector, 1))
    {
        return false;
    }

    column_vector[0] = column_vector[0] + 3;
    column_vector[1] = column_vector[1] + 3;
    column_vector[2] = column_vector[2] + 3;
    if(!fm_column.insert_column(column_vector, 1))
    {
        return false;
    }

    column_vector[0] = column_vector[0] + 3;
    column_vector[1] = column_vector[1] + 3;
    column_vector[2] = column_vector[2] + 3;
    if(!fm_column.insert_column(column_vector, 2))
    {
        return false;
    }

    if(!fm_column.get_column(1, column_vector_out))
    {
        return false;
    }

    if( (column_vector_out[0] != 4) ||
        (column_vector_out[1] != 5) ||
        (column_vector_out[2] != 6)
    ){
        return false;
    }

    if(!fm_column.get_column(2, column_vector_out))
    {
        return false;
    }

    if( (column_vector_out[0] != 7) ||
        (column_vector_out[1] != 8) ||
        (column_vector_out[2] != 9)
    ){
        return false;
    }

    if(!fm_column.get_column(3, column_vector_out))
    {
        return false;
    }

    if( (column_vector_out[0] != 1) ||
        (column_vector_out[1] != 2) ||
        (column_vector_out[2] != 3)
    ){
        return false;
    }

    return true;
}

bool FlatMatrix_TestCase_Int_MixInsert(void)
{
    FlatMatrix<int> fm;
    vector<int> row_vector1;
    vector<int> row_vector2;
    vector<int> column_vector1;
    vector<int> column_vector2;

    int row1_checker_array[] = {2, 5, 1};
    int row2_checker_array[] = {3, 6, 4};

    row_vector1.push_back(1);

    column_vector1.push_back(2);

    row_vector2.push_back(3);
    row_vector2.push_back(4);

    column_vector2.push_back(5);
    column_vector2.push_back(6);

    if(!fm.insert_row(row_vector1, 1))
    {
        return false;
    }

    if(!fm.insert_column(column_vector1, 0))
    {
        return false;
    }

    if(!fm.insert_row(row_vector2, 7))
    {
        return false;
    }

    if(!fm.insert_column(column_vector2, 2))
    {
        return false;
    }

    int value;

    for (int idx = 0; idx < 3; ++idx) {
        if(!fm.get_element(1,idx + 1,value))
        {
            return false;
        }
        else if(value != row1_checker_array[idx])
        {
            return false;
        }

        if(!fm.get_element(2,idx + 1,value))
        {
            return false;
        }
        else if(value != row2_checker_array[idx])
        {
            return false;
        }
    }

    return true;
}

bool FlatMatrix_TestCase_Dbl_MixInsert(void)
{
    FlatMatrix<double> fm;
    vector<double> row_vector1;
    vector<double> row_vector2;
    vector<double> column_vector1;
    vector<double> column_vector2;

    double row1_checker_array[] = {2.0, 5.0, 1.0};
    double row2_checker_array[] = {3.0, 6.0, 4.0};

    row_vector1.push_back(1.0);

    column_vector1.push_back(2.0);

    row_vector2.push_back(3.0);
    row_vector2.push_back(4.0);

    column_vector2.push_back(5.0);
    column_vector2.push_back(6.0);

    if(!fm.insert_row(row_vector1, 1))
    {
        return false;
    }

    if(!fm.insert_column(column_vector1, 0))
    {
        return false;
    }

    if(!fm.insert_row(row_vector2, 7))
    {
        return false;
    }

    if(!fm.insert_column(column_vector2, 2))
    {
        return false;
    }

    double value;

    for (int idx = 0; idx < 3; ++idx) {
        if(!fm.get_element(1,idx + 1,value))
        {
            return false;
        }
        else if(value != row1_checker_array[idx])
        {
            return false;
        }

        if(!fm.get_element(2,idx + 1,value))
        {
            return false;
        }
        else if(value != row2_checker_array[idx])
        {
            return false;
        }
    }

    return true;
}

bool FlatMatrix_TestCase_Int_MixSet(void)
{
    FlatMatrix<int> fm(2,4,0);
    vector<int> row;
    vector<int> column;
    int fm_checker1_row1[] = {2,  5, 3, 9};
    int fm_checker1_row2[] = {0,  0, 0, 0};
    int fm_checker2_row1[] = {2, 12, 3, 9};
    int fm_checker2_row2[] = {0,  5, 0, 0};

    row.push_back(2);
    row.push_back(5);
    row.push_back(3);
    row.push_back(9);

    column.push_back(12);
    column.push_back(5);

    if(!fm.set_row(row, 1))
    {
        return false;
    }

    try
    {
        for(int idx = 0; idx < 4; ++idx)
        {
            int value;
            fm.get_element(1, idx + 1, value);
            if(value != fm_checker1_row1[idx])
            {
                return false;
            }

            fm.get_element(2, idx + 1, value);
            if(value != fm_checker1_row2[idx])
            {
                return false;
            }
        }
    }
    catch(...)
    {
        return false;
    }

    if(!fm.set_column(column, 2))
    {
        return false;
    }

    try
    {
        for(int idx = 0; idx < 4; ++idx)
        {
            int value;
            fm.get_element(1, idx + 1, value);
            if(value != fm_checker2_row1[idx])
            {
                return false;
            }

            fm.get_element(2, idx + 1, value);
            if(value != fm_checker2_row2[idx])
            {
                return false;
            }
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool FlatMatrix_TestCase_Int_CopyConstructor(void)
{
    FlatMatrix<int> fm1(3,3,1);
    FlatMatrix<int> fm2(fm1);

    if(!fm1.set_element(2,2,5))
    {
        return false;
    }

    int value;

    if(!fm1.get_element(2,2,value))
    {
        return false;
    }

    if(value != 5)
    {
        return false;
    }

    if(!fm2.get_element(2,2,value))
    {
        return false;
    }

    if(value != 1)
    {
        return false;
    }
    return true;
}

bool FlatMatrix_TestCase_Int_Serialize(void)
{
    FlatMatrix<int> fm1;
    FlatMatrix<int> fm2;
    vector<int> row1;
    vector<int> row2;

    int fm2_checker_row1[] = {1,2,3};
    int fm2_checker_row2[] = {4,5,6};

    stringstream str_stream;

    row1.push_back(1);
    row1.push_back(2);
    row1.push_back(3);
    row2.push_back(4);
    row2.push_back(5);
    row2.push_back(6);

    if(!fm1.insert_row(row1,1))
    {
        return false;
    }

    if(!fm1.insert_row(row2,2))
    {
        return false;
    }

    str_stream << fm1;

    if( (fm2.get_row_dim()    != 0) ||
        (fm2.get_column_dim() != 0) )
    {
        return false;
    }

    str_stream >> fm2;

    if( (fm2.get_row_dim()    != 2) ||
        (fm2.get_column_dim() != 3) )
    {
        return false;
    }

    int value;
    for (int idx = 0; idx < 3; ++idx) {
        if(!fm2.get_element(1,idx + 1,value))
        {
            return false;
        }
        else if(value != fm2_checker_row1[idx])
        {
            return false;
        }

        if(!fm2.get_element(2,idx + 1,value))
        {
            return false;
        }
        else if(value != fm2_checker_row2[idx])
        {
            return false;
        }
    }

    return true;
}

bool FlatMatrix_TestCase_Dbl_Serialize(void)
{
    FlatMatrix<double> fm1_out;
    FlatMatrix<double> fm1_in;
    FlatMatrix<double> fm2_out;
    FlatMatrix<double> fm2_in;
    vector<double> fm1_row;
    vector<double> fm2_column;

    double fm1_checker[] = {1e-12,2e4,312.48};
    double fm2_checker[] = {0.0,-634.5413,15.321e3};

    stringstream str_stream;

    fm1_row.push_back(1e-12);
    fm1_row.push_back(2e4);
    fm1_row.push_back(312.48);
    fm2_column.push_back(0.0);
    fm2_column.push_back(-634.5413);
    fm2_column.push_back(15.321e3);

    if(!fm1_out.insert_row(fm1_row,1))
    {
        return false;
    }

    if(!fm2_out.insert_column(fm2_column,1))
    {
        return false;
    }

    str_stream.precision(30);
    str_stream << fm1_out << fm2_out;

    if( (fm1_in.get_row_dim()    != 0) ||
        (fm1_in.get_column_dim() != 0) ||
        (fm2_in.get_row_dim()    != 0) ||
        (fm2_in.get_column_dim() != 0) )
    {
        return false;
    }

    str_stream >> fm1_in;
    str_stream >> fm2_in;

    if( (fm1_in.get_row_dim()    != 1) ||
        (fm1_in.get_column_dim() != 3) ||
        (fm2_in.get_row_dim()    != 3) ||
        (fm2_in.get_column_dim() != 1) )
    {
        return false;
    }

    double value;
    for (int idx = 0; idx < 3; ++idx) {
        if(!fm1_in.get_element(1, idx + 1, value))
        {
            return false;
        }
        else if( abs(value - fm1_checker[idx]) > DBL_EPSILON )
        {
            return false;
        }

        if(!fm2_in.get_element(idx + 1, 1, value))
        {
            return false;
        }
        else if( abs(value - fm2_checker[idx]) > DBL_EPSILON )
        {
            return false;
        }
    }

    return true;
}

bool FlatMatrix_TestCase_Str_Serialize(void)
{
    FlatMatrix<string> fm1;
    FlatMatrix<string> fm2;
    vector<string> row1;
    vector<string> row2;

    string fm2_checker_row1[] = {"fm{1,1}","fm{1,2}","fm{1,3}"};
    string fm2_checker_row2[] = {"fm{2;1}","fm{2;2}","fm{2;3}"};

    stringstream str_stream;

    row1.push_back("fm{1,1}");
    row1.push_back("fm{1,2}");
    row1.push_back("fm{1,3}");
    row2.push_back("fm{2;1}");
    row2.push_back("fm{2;2}");
    row2.push_back("fm{2;3}");

    if(!fm1.insert_row(row1,1))
    {
        return false;
    }

    if(!fm1.insert_row(row2,2))
    {
        return false;
    }

    str_stream << fm1;

    if( (fm2.get_row_dim()    != 0) ||
        (fm2.get_column_dim() != 0) )
    {
        return false;
    }

    str_stream >> fm2;

    if( (fm2.get_row_dim()    != 2) ||
        (fm2.get_column_dim() != 3) )
    {
        return false;
    }

    string value;
    for (int idx = 0; idx < 3; ++idx) {
        if(!fm2.get_element(1,idx + 1,value))
        {
            return false;
        }
        else if(value != fm2_checker_row1[idx])
        {
            return false;
        }

        if(!fm2.get_element(2,idx + 1,value))
        {
            return false;
        }
        else if(value != fm2_checker_row2[idx])
        {
            return false;
        }
    }

    return true;
}



// This Pure class is used to check what interface is necessary for FlatMatrix
//class Pure {
//    int value;
//public:
//    Pure(int value):value(value){}
//    Pure() = delete;
//    Pure(const Pure& other) = delete;
//    Pure& operator=(const Pure& other) = delete;
//    Pure& operator=(Pure&& other) = delete;
//};
//
//bool FlatMatrix_TestCase_RequiredInterfaces(void)
//{
//    FlatMatrix<Pure> fm1(2,2,Pure(42));
//    FlatMatrix<Pure> fm2;
//
//    vector<Pure> v2;
//    v2.push_back(Pure(1));
//    v2.push_back(Pure(2));
//    v2.push_back(Pure(3));
//
//    fm2.insert_row(v2, 1);
//
//    return true;
//}


typedef bool(*testcase_t)(void);
struct testcase_info_t {
    testcase_info_t():
    testcase_callback(NULL), testcase_name(""){}

    testcase_info_t(testcase_t testcase_callback, string testcase_name):
    testcase_callback(testcase_callback), testcase_name(testcase_name){}

    testcase_info_t(const testcase_info_t& other):
    testcase_callback(other.testcase_callback), testcase_name(other.testcase_name){}

    testcase_info_t& operator=(const testcase_info_t& other)
    {
        if(this != &other)
        {
            testcase_callback = other.testcase_callback;
            testcase_name = other.testcase_name;
        }
        return *this;
    }

    testcase_t testcase_callback;
    string testcase_name;

    bool call(void)
    {
        if(testcase_callback != NULL)
        {
            return testcase_callback();
        }
        else
        {
            return false;
        }
    }
};

int main(int argc, char *argv[])
{
    testcase_info_t testsuite[] = {
            testcase_info_t{FlatMatrix_TestCase_Int_DefaultConstructor,
                           "FlatMatrix_TestCase_Int_DefaultConstructor"},
            testcase_info_t{FlatMatrix_TestCase_Int_FillConstructor,
                           "FlatMatrix_TestCase_Int_FillConstructor"},
            testcase_info_t{FlatMatrix_TestCase_Int_RowInsert,
                           "FlatMatrix_TestCase_Int_RowInsert"},
            testcase_info_t{FlatMatrix_TestCase_Int_ColumnInsert,
                           "FlatMatrix_TestCase_Int_ColumnInsert"},
            testcase_info_t{FlatMatrix_TestCase_Int_MixInsert,
                           "FlatMatrix_TestCase_Int_MixInsert"},
            testcase_info_t{FlatMatrix_TestCase_Dbl_MixInsert,
                           "FlatMatrix_TestCase_Dbl_MixInsert"},
            testcase_info_t{FlatMatrix_TestCase_Int_MixSet,
                           "FlatMatrix_TestCase_Int_MixSet"},
            testcase_info_t{FlatMatrix_TestCase_Int_CopyConstructor,
                           "FlatMatrix_TestCase_Int_CopyConstructor"},
            testcase_info_t{FlatMatrix_TestCase_Int_Serialize,
                           "FlatMatrix_TestCase_Int_Serialize"},
            testcase_info_t{FlatMatrix_TestCase_Dbl_Serialize,
                           "FlatMatrix_TestCase_Dbl_Serialize"},
            testcase_info_t{FlatMatrix_TestCase_Str_Serialize,
                           "FlatMatrix_TestCase_Str_Serialize"}
    };

    int testsuite_size = sizeof(testsuite) / sizeof(testsuite[0]);
    int testsuite_tc_success_count = 0;

    cout << "FlatMatrix test suite:" << endl;
    for (int idx = 0; idx < testsuite_size; ++idx) {
        cout << setw(45) << left << testsuite[idx].testcase_name << " --> ";
        if(testsuite[idx].call())
        {
            cout << "pass" << endl;
            ++testsuite_tc_success_count;
        }
        else
        {
            cout << "fail" << endl;
        }
    }
    cout << "Summary: " << testsuite_tc_success_count << "\\" << testsuite_size << endl;

}
