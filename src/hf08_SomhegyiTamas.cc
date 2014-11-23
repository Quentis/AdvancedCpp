/* Somhegyi Tamas Tibor, BLRGA6 */

/* �rj gener�tort, amely szinuszjelet bocs�t ki. Param�terei:
 *      -T: peri�dushossz (eg�sz)
 *      -A: amplitud�
 *      -f0: kezd�f�zis (eg�sz)
 * teszteld a gener�tort 20 elem gener�l�s�ra az al�bbi param�terekkel:
 *      -A:1, T:4, f0:0 (0,1,0,-1,...)
 *      -A:1, T:4, f0:1 (1,0,-1,0,...)
 *      -A:2, T:12, f0:0 (0,1,sqrt(3),2,sqrt(3),1,0,-1,...)
 * a gener�lt sorokat �rd is ki k�perny�re!
 *
 * A beadott forr�s teh�t ezeket tartalmazza:
 *      -gener�tor oszt�ly, benne:
 *          -konstruktor,
 *          -operator()();
 *      -main f�ggv�ny, benne:
 *          -3 t�mb defini�l�sa (legyen k�zt�k eleve 20 m�ret� meg �res is),
 *          -t�mb�k felt�lt�se generate()-tel (illetve �res t�mb felt�lt�se generate_n()-nel �s inserter()-rel),
 *          -ki�rat�s copy()-val ostream_iterator-ra.
 * Szorgalmi:
 *      -koszinusz-gener�tor a szinusz-gener�torra r��p�tve (sz�rmaztatott oszt�ly)
 *      -itt a kezd�f�zis lehet ugye nem eg�sz is! */

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
