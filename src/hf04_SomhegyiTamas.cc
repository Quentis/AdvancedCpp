/* Somhegyi Tamas Tibor, BLRGA6 */

/* K�sz�tsd el STL t�rol�k haszn�lata n�lk�l a k�rk�r�s puffer /FIFO t�rol�, k�rbefordul� �r�ssal, olvas�ssal/
 * oszt�ly sablont (CircularBuffer), amely tetsz�leges t�pus� elemeket k�pes t�rolni! A puffer m�rete min. 1,
 * max. 1000, amelyet a konstruktor param�terek�nt lehet megadni. Ha nincs megadva, vagy nincs az el�bb megadott
 * tartom�nyban, akkor alap�rtelmezett m�rete 10. A remove m�velet kiveszi a legels� (legkor�bban betett) elemet
 * a pufferb�l, az insert m�velet pedig a k�vetkez� �res helyre teszi a param�terk�nt �tadott elemet, amennyiben
 * ez lehets�ges (nincs fel�l�r�s!). Az isEmpty �s isFull bool visszat�r�si t�pus� fv. inform�ci�t ad a puffer
 * �llapot�r�l �rtelemszer�en (�res, megtelt). �res pufferb�l t�rt�n� kiv�telkor, illetve teli pufferbe t�rt�n�
 * �r�skor keletkezzen kiv�teles esem�ny (const char* t�pus�)!
 * A puffer objektum operator<< seg�ts�g�vel t�rt�n� ki�rat�sakor a pufferben t�rolt elemeket �rjuk ki.
 * K�sz�ts olyan f�ggv�ny sablont (getMax), amely k�t azonos t�pus� v�ltoz� k�z�l a nem kisebbet adja vissza,
 * �s ez a f�ggv�ny k�rk�r�s puffer objektumok �sszehasonl�t�sakor a nem kevesebb elemet tartalmaz�t adja vissza!
 * A k�vetkez� programr�szletnek m�k�dnie kell az elk�sz�tett oszt�llyal �s f�ggv�nysablonnal, ezt legyen a teszt
 * a main()-ben:*/

#include<iostream>

using namespace std;

namespace hf04
{

    template<class T, int N = 10>
    class CircularBuffer
    {
    private:
        static const int sizeMin = 1, sizeMax = 1000;

        T* storage;
        int firstElement;
        int count;
        int size;

    public:
        CircularBuffer(const CircularBuffer<T>& cb)
        {
            firstElement = cb.firstElement;
            count = cb.count;
            size = cb.size;
            storage = new T[cb.size];

            if((firstElement + count - 1) >= size)
            {
                for (int idx = firstElement; idx < size; idx++)
                {
                    storage[idx] = cb.storage[idx];
                }

                for (int idx = 0; idx < (firstElement + count) % size; idx++)
                {
                    storage[idx] = cb.storage[idx];
                }
            }
            else
            {
                for (int idx = firstElement; idx < (firstElement + count); idx++)
                {
                    storage[idx] = cb.storage[idx];
                }
            }
        }

        CircularBuffer(int size)
        {
            if((size < sizeMin) && (size > sizeMax))
            {
                cout << "CircularBuffer constraint violation: " << sizeMin << " <= "
                        << size << "<=" << sizeMax << ". It defaults to " << N;
                size = N;
            }

            firstElement = 0;
            count = 0;
            storage = new T[size];
            this->size = size;
        }

        ~CircularBuffer()
        {
            delete[] storage;
        }

        bool isEmpty()
        {
            if(count == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool isFull()
        {
            if(size == count)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        T& remove(void)
        {
            int firstElementTemp = firstElement;
            if(isEmpty())
            {
                throw "Remove request on empty CircularBuffer object.";
            }

            firstElement = (firstElement + 1) % size;
            count--;
            return storage[firstElementTemp];
        }

        void insert(const T& element)
        {
            if(isFull())
            {
                throw "Insert request on full CircularBuffer object.";
            }
            const int firstEmptyElement = (firstElement + count) % size;
            storage[firstEmptyElement] = element;
            count++;
        }

        bool operator<(const CircularBuffer<T>& other)
        {
            if(count < other.count)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator<=(const CircularBuffer<T>& other)
        {
            if(count <= other.count)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator>(const CircularBuffer<T>& other)
        {
            if(count > other.count)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator>=(const CircularBuffer<T>& other)
        {
            if(count >= other.count)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        friend ostream& operator<<(ostream& os, const CircularBuffer<T>& circBufObj)
        {
          cout << "CircularBuffer { ";
          if((circBufObj.firstElement + circBufObj.count - 1) >= circBufObj.size)
          {
              for (int idx = circBufObj.firstElement; idx < circBufObj.size; idx++)
              {
                  os << circBufObj.storage[idx] << " ";
              }

              for (int idx = 0; idx < (circBufObj.firstElement + circBufObj.count) % circBufObj.size; idx++)
              {
                  os << circBufObj.storage[idx] << " ";
              }
          }
          else
          {
              for (int idx = circBufObj.firstElement; idx < (circBufObj.firstElement + circBufObj.count); idx++)
              {
                  os << circBufObj.storage[idx] << " ";
              }
          }
          cout << "}";
          return os;
        }
    };

    template<class T>
    T getMax(T x, T y)
    {
        if(x <= y)
        {
            return y;
        }
        else
        {
            return x;
        }
    }

    void hf04()
    {
        CircularBuffer<double> dcb(2);
        try
        {
            dcb.insert(3.3);
            dcb.insert(4.4);
            cout << "cb= " << dcb << endl;
            double r = dcb.remove();
            cout << "r= " << r << endl;
            cout << "cb= " << dcb << endl;
            dcb.insert(5.5);
            cout << "cb= " << dcb << endl;
            dcb.insert(6.6);
        }
        catch (const char* e)
        {
            cerr << e << endl;
        }

        float f1 = 1.23, f2 = 3.21;
        cout << getMax(f1, f2) << endl;

        CircularBuffer<int> icb1(1200);
        CircularBuffer<int> icb2 = icb1;
        try
        {
            icb1.insert(5);
            cout << getMax(icb2, icb1) << endl;
            icb1.remove();
            icb1.remove();
        }
        catch (const char* e)
        {
            cerr << e << endl;
        }
    }
}
