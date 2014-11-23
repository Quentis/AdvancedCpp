/* Somhegyi Tamas Tibor, BLRGA6 */

/* A hf03_sineloop_tmpl.cc kiegészítésvel készíts függvénytáblázatot sin(x) függvényre úgy, hogy a sin_t tömböt
 * rekurzív LOOP és SINE használatával. Javaslat:
 *    -fejlesztés során a sin_t tömb méretét tartsd alacsonyan (hogy gyorsan forduljon a kód)
 *    -elõbb az órán vett LOOP-pal, majd az elõrefelé számláló LOOP_INC sablonnal dolgozz!
 * Szorgalmi:
 *    -cos() és exp() függvényekre is mûködjön a program (legyenek további tömbök).
 *    -teszteld a megvalósítást különbözõ tömbmérettel és különbözõ precízió beállításával! Mit tapasztalsz?
 *    -a VALTYPE típustól hogyan függ az eredmények pontossága és a lefordított kód mérete? Minek mi az oka?
 *    -hol van matematikai hiba a kiadott kódvázlatban? Milyen bemeneti érték (ill. intervallum) esetén jön elõ a hiba?
 */

#include <iostream>
#include <cmath>

////////////////
// konstansok //
////////////////

#define M_PI 3.14159265358979323846

/// Tömb mérete
#define SIZE 720
/// Approximáció mélysége
#define PREC 10
/// Érték típusa
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

    /// közvetlenül hívott tag
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
    /// rekurzív törzs
    /// Az inline fv.-nek legyen egy olyan pointer a paramétere,
    /// ami a sin_t tömbbe mutat!
    /// Erre a pointerre tárolja a sin() értéket,
    /// És rekurzív hívásnál léptesd a pointert!
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

    /// Mivel a loop<> visszafelé számolja I-t,
    /// szükséges egy olyan loop-ot is csinálni,
    /// ami elõrefelé lépteti I-t.
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

    /// a feltöltendõ tömb
    VALTYPE sin_t[SIZE];

    /// sin(alpha) lekérdezése.
    /// Egy bemenõ és több kimenõ paramétere van.
    /// \param alpha szög (deg)
    /// \param alpha_lo_idx szög indexe a táblázatban (alsó közelítés)
    /// \param alpha_hi_idx szög indexe a táblázatban (felsõ közelítés)
    /// \param alpha_lo szög alsó közelítésének értéke
    /// \param alpha_hi szög felsõ közelítésének értéke
    /// \param sin_lo alsó közelítés szinusza
    /// \param sin_hi felsõ közelítés szinusza
    /// \return egzakt találat
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

    // ellenõrzés
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
