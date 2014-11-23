/* Somhegyi Tamas Tibor, BLRGA6 */

/* A hf03_sineloop_tmpl.cc kieg�sz�t�svel k�sz�ts f�ggv�nyt�bl�zatot sin(x) f�ggv�nyre �gy, hogy a sin_t t�mb�t
 * rekurz�v LOOP �s SINE haszn�lat�val. Javaslat:
 *    -fejleszt�s sor�n a sin_t t�mb m�ret�t tartsd alacsonyan (hogy gyorsan forduljon a k�d)
 *    -el�bb az �r�n vett LOOP-pal, majd az el�refel� sz�ml�l� LOOP_INC sablonnal dolgozz!
 * Szorgalmi:
 *    -cos() �s exp() f�ggv�nyekre is m�k�dj�n a program (legyenek tov�bbi t�mb�k).
 *    -teszteld a megval�s�t�st k�l�nb�z� t�mbm�rettel �s k�l�nb�z� prec�zi� be�ll�t�s�val! Mit tapasztalsz?
 *    -a VALTYPE t�pust�l hogyan f�gg az eredm�nyek pontoss�ga �s a leford�tott k�d m�rete? Minek mi az oka?
 *    -hol van matematikai hiba a kiadott k�dv�zlatban? Milyen bemeneti �rt�k (ill. intervallum) eset�n j�n el� a hiba?
 */

#include <iostream>
#include <cmath>

////////////////
// konstansok //
////////////////

#define M_PI 3.14159265358979323846

/// T�mb m�rete
#define SIZE 720
/// Approxim�ci� m�lys�ge
#define PREC 10
/// �rt�k t�pusa
//#define VALTYPE float
#define VALTYPE double
//#define VALTYPE long double

using namespace std;

namespace hf03
{
    template<int N, int I, int J, int K>
    class SineSeries {
    public:
        enum { go = (K+1 != J) };

        static inline float accumulate()
        {
            return 1-(I*2*M_PI/N)*(I*2*M_PI/N)/(2*K+2)/(2*K+3) * SineSeries<N*go,I*go,J*go,(K+1)*go>::accumulate();
        }
    };

    template<>
    class SineSeries<0,0,0,0> {
    public:
        static inline float accumulate()
        { return 1; }
    };

    /// k�zvetlen�l h�vott tag
    template<int N, int I>
    class Sine {
    public:
        static inline VALTYPE sin()
        {
            return (I*2*M_PI/N) * SineSeries<N,I,PREC,0>::accumulate();
        }
    };


    template<int I>
    class loop {
    /// rekurz�v t�rzs
    /// Az inline fv.-nek legyen egy olyan pointer a param�tere,
    /// ami a sin_t t�mbbe mutat!
    /// Erre a pointerre t�rolja a sin() �rt�ket,
    /// �s rekurz�v h�v�sn�l l�ptesd a pointert!
    public:
        static inline void fillRecursive(VALTYPE* dest)
        {
            loop< I-1 >::fillRecursive();
        }

    };

    template<>
    class loop<0> {
        static inline void fillRecursive(VALTYPE* dest)
        {

        }
    };

    /// Mivel a loop<> visszafel� sz�molja I-t,
    /// sz�ks�ges egy olyan loop-ot is csin�lni,
    /// ami el�refel� l�pteti I-t.
    template<int N, int I>

    class loop_inc{
    public:
        static inline void calculate(VALTYPE* dest)
        {
            *dest = Sine<N,I>::sin();
            dest++;
            loop_inc<(N<I+1) ? 0 : N,(I+1) % (N+1)>::calculate(dest);
        }
    };

    template<>
    class loop_inc<0,0> {
    public:
        static inline void calculate(VALTYPE* dest)
        {

        }
    };

    /// a felt�ltend� t�mb
    VALTYPE sin_t[SIZE];

    /// sin(alpha) lek�rdez�se.
    /// Egy bemen� �s t�bb kimen� param�tere van.
    /// \param alpha sz�g (deg)
    /// \param alpha_lo_idx sz�g indexe a t�bl�zatban (als� k�zel�t�s)
    /// \param alpha_hi_idx sz�g indexe a t�bl�zatban (fels� k�zel�t�s)
    /// \param alpha_lo sz�g als� k�zel�t�s�nek �rt�ke
    /// \param alpha_hi sz�g fels� k�zel�t�s�nek �rt�ke
    /// \param sin_lo als� k�zel�t�s szinusza
    /// \param sin_hi fels� k�zel�t�s szinusza
    /// \return egzakt tal�lat
    bool sin_lookup(
        const VALTYPE &alpha,
        int &alpha_lo_idx,
        int &alpha_hi_idx,
        VALTYPE &alpha_lo,
        VALTYPE &alpha_hi,
        VALTYPE &sin_lo,
        VALTYPE &sin_hi
        ){
        alpha_lo_idx=(int)((alpha)/360*SIZE)%SIZE;
        alpha_lo=alpha_lo_idx*360.0/SIZE;

        bool alpha_match=(alpha==alpha_lo);

        alpha_hi_idx=alpha_match?
            alpha_lo_idx:
            (alpha_lo_idx+1)%SIZE;
        alpha_hi=alpha_hi_idx*360.0/SIZE;

        sin_lo=sin_t[alpha_lo_idx];
        sin_hi=sin_t[alpha_hi_idx];
        return alpha_match;
    }

    bool arcsin_lookup(
        const VALTYPE &val,
        int &val_lo_idx,
        int &val_hi_idx,
        VALTYPE &val_lo,
        VALTYPE &val_hi,
        VALTYPE &alpha_lo,
        VALTYPE &alpha_hi
        ){
        for (val_hi_idx=0;val_hi_idx<=SIZE/4 && sin_t[val_hi_idx]<=val;val_hi_idx++);
        for (val_lo_idx=SIZE/4;val_lo_idx>=0 && sin_t[val_lo_idx]>=val;val_lo_idx--);
        if (!(val_hi_idx==val_lo_idx || val_hi_idx==val_lo_idx+1)){
            return false;
        }
        val_lo=360.0*val_lo_idx/SIZE;
        val_hi=360.0*val_hi_idx/SIZE;
        alpha_lo=sin_t[val_lo_idx];
        alpha_hi=sin_t[val_hi_idx];
        return true;
    }

    void hf03(){
        loop_inc<SIZE,0>::calculate(sin_t);

    // ellen�rz�s
      for (int i=0;i<SIZE;i++)
          cout<<"sin("<<((double)360*i)/SIZE<<")="<<sin_t[i]<<endl;

        VALTYPE alpha,val;
        VALTYPE alpha_lo,alpha_hi;
        VALTYPE sin_lo,sin_hi;
        VALTYPE val_lo,val_hi;
        int alpha_lo_idx,alpha_hi_idx;
        int val_lo_idx,val_hi_idx;

        cout<<"alpha (deg)?";
        cin>>alpha;

        bool alpha_match=sin_lookup(alpha,
        alpha_lo_idx,alpha_hi_idx,alpha_lo,alpha_hi,
        sin_lo,sin_hi);
        cout<<"sin("<<alpha_lo<<")="<<sin_lo<<endl;
        if (!alpha_match){
        cout<<"sin("<<alpha_hi<<")="<<sin_hi<<endl;
        }

        cout<<"sin(alpha)?";
        cin>>val;
        if (arcsin_lookup(val,val_lo_idx,val_hi_idx,val_lo,val_hi,alpha_lo,alpha_hi)){
            cout<<"arcsin("<<val_lo<<")="<<alpha_lo<<endl;
            cout<<"arcsin("<<val_hi<<")="<<alpha_hi<<endl;
        }
    }
}
