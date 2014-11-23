/* Somhegyi Tamas Tibor, BLRGA6 */

/* Írj generátort, amely szinuszjelet bocsát ki. Paraméterei:
 *      -T: periódushossz (egész)
 *      -A: amplitudó
 *      -f0: kezdõfázis (egész)
 * teszteld a generátort 20 elem generálására az alábbi paraméterekkel:
 *      -A:1, T:4, f0:0 (0,1,0,-1,...)
 *      -A:1, T:4, f0:1 (1,0,-1,0,...)
 *      -A:2, T:12, f0:0 (0,1,sqrt(3),2,sqrt(3),1,0,-1,...)
 * a generált sorokat írd is ki képernyõre!
 *
 * A beadott forrás tehát ezeket tartalmazza:
 *      -generátor osztály, benne:
 *          -konstruktor,
 *          -operator()();
 *      -main függvény, benne:
 *          -3 tömb definiálása (legyen köztük eleve 20 méretû meg üres is),
 *          -tömbök feltöltése generate()-tel (illetve üres tömb feltöltése generate_n()-nel és inserter()-rel),
 *          -kiíratás copy()-val ostream_iterator-ra.
 * Szorgalmi:
 *      -koszinusz-generátor a szinusz-generátorra ráépítve (származtatott osztály)
 *      -itt a kezdõfázis lehet ugye nem egész is! */

#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdint>
#include <cmath>

using namespace std;

#define M_PI 3.14159265358979323846

namespace hf08
{
    class SinGenerator : public unary_function<void,double>
    {
    private:
        double A;
        int T;
        int f0;

        int t;
    public:
        SinGenerator(double A, int T, int f0) : A(A), T(T), f0(f0), t(T) { }

        double operator()()
        {
            t = (t < T - 1) ? t + 1 : 0;
            return sin((2 * M_PI * (t + f0))/ T);
        }

    };

    void test(double A, int T, int f0)
    {
        cout << "Test( A = " << A << ", T = " << T << ", f0 = " << f0 << ")" << endl;

        const int length = 20;
        double array[length];
        vector<double> x(length);
        vector<double> y(0);

        generate(array, array + length, SinGenerator(A, T, f0));
        generate(x.begin(), x.end(), SinGenerator(A, T, f0));
        generate_n(inserter(y,y.begin()), length, SinGenerator(A, T, f0));

        ostream_iterator<double> out_it(cout, " ");

        cout << "Array (generate):" << endl;
        copy(array, array + length, out_it);
        cout << endl;

        cout << "Vector (generate):" << endl;
        copy(x.begin(), x.end(), out_it);
        cout << endl;

        cout << "Vector (generate_n)" << endl;
        copy(y.begin(), y.end(), out_it);
        cout << endl;
    }

    void hf08(void){
        cout.precision(4);
        cout << fixed;
        cout << "---------------------------------------------------------------------" << endl;
        test(1, 4, 0);
        cout << "---------------------------------------------------------------------" << endl;
        test(1, 4, 1);
        cout << "---------------------------------------------------------------------" << endl;
        test(2, 12, 0);
        cout << "---------------------------------------------------------------------" << endl;
        cin.get();
    }

}
