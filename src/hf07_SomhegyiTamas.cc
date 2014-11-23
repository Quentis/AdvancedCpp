/* Somhegyi Tamas Tibor, BLRGA6 */

/* Melyik oper�tora h�nyszor h�v�dott meg az OutputIteratornak?
 *     -Eg�sz�tsd ki a hf07_oiCnt.cc-ben megadott ostream_iterator -t, hogy az sz�molja a f�ggv�nyh�v�sokat
 *     -Haszn�ld a copy fv.-ben ezt a saj�t OstreamIteratorCnt iter�tort a mostani ostream_iterator helyett
 *     -A copy lefut�sa ut�n �rd ki, mi h�nyszor h�v�dott meg (k�ls� objektum el�r�se ld. foreach2_demo.cc )
 * Szorgalmi
 *     -m�dos�tsd az OstreamIteratorCnt -t sablonn�, hogy ne csak int -ekre m�k�dj�n
 *     -az egyes f�ggv�nyek h�v�s�nak sorrendj�t is jegyezze meg (sz�ml�l�k mellett history) */

#include <iostream>
#include <iterator>

using namespace std;

namespace hf07
{

    int A[]={0,6,1,4,63,21,87};
    int AN=sizeof(A)/sizeof(int);

    class OstreamIteratorCnt : public ostream_iterator<int> {
    private:
        static int increment_counter;
        static int assign_counter;
        static int dereference_counter;

    public:
        /* All of the valid ostream_iterator constructor can be used in the derived class */
        OstreamIteratorCnt (ostream_type& s) : ostream_iterator<int>(s) {}

        OstreamIteratorCnt (ostream_type& s, const char_type* delimiter) : ostream_iterator<int>(s, delimiter) {}

        OstreamIteratorCnt (const OstreamIteratorCnt& x) : ostream_iterator<int>(x) {}

        OstreamIteratorCnt& operator=(const int value){
            OstreamIteratorCnt::assign_counter++;
            std::ostream_iterator<int>::operator=(value);
            return *this;
        };

        OstreamIteratorCnt& operator*()
        {
            OstreamIteratorCnt::dereference_counter++;
            static_cast<std::ostream_iterator<int> >(*this).operator*();
            return *this;
        }

        OstreamIteratorCnt& operator++(){
            OstreamIteratorCnt::increment_counter++;
            static_cast<std::ostream_iterator<int> >(*this).operator++();
            return *this;
        }

        friend ostream& operator<<(ostream& os, OstreamIteratorCnt& osIterCnt)
        {
            cout << "OstreamIteratorCnt { " << "increment_counter: "     << OstreamIteratorCnt::increment_counter
                                            << ", assign_counter: "      << OstreamIteratorCnt::assign_counter
                                            << ", dereference_counter: " << OstreamIteratorCnt::dereference_counter << " }";
            return os;
        }
    };

    int OstreamIteratorCnt::increment_counter = 0;
    int OstreamIteratorCnt::assign_counter = 0;
    int OstreamIteratorCnt::dereference_counter = 0;

    void hf07(void){
        //ostream_iterator => OstreamIteratorCnt
        OstreamIteratorCnt oi(cout,"\t");

        copy(A, A + AN, oi);
        cout << endl << "Az egyes operatorok ennyiszer lettek meghivva az oi objektumban:" << endl;
        cout << oi;
    }

}
