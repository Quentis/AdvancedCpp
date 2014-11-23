/* Somhegyi Tamas Tibor, BLRGA6 */

/* K�sz�tsd el a referencia l�ncol�st megval�s�t� smart pointer oszt�lyt! Biztos�tsd ugyanazt
 * a funkcionalit�st, mint a referencia sz�ml�l�ssal implement�lt smart pointern�l! Az al�bbi
 * teszt k�d legyen a main f�ggv�ny a leadott forr�sban, felhaszn�lva a refcount0[12].cc-b�l a
 * Person oszt�lyt is! (tov�bbi r�szletek �s seg�ts�g a smart pointer el�ad�s anyag�ban) */

#include<iostream>

using namespace std;

namespace hf06
{

    class Person
    {
        string name;
        int age;

       public:
         Person(char* pName, int age): name(pName), age(age) {}
         void display()
         {
            cout <<"Name = " << name << "  Age = " << age << endl;
         }
         ~Person()
         {
           cout << "Person destructor called" << endl;
         }
    };

    template < typename T > class RLptr
    {
      private:
        T* pData;
        class RLptr* next;
        class RLptr* prev;

        void remove(void)
        {
            if((next != nullptr) && (prev != nullptr))
            {
                next = next->next;
                prev = next;
            }
            else if(next != nullptr)
            {
                next = next->next;
            }
            else if(prev!= nullptr)
            {
                prev = next;
            }
            else
            {
                delete pData; /* No pointer points to the object */
            }
        }

      public:
        explicit RLptr(T* pValue = nullptr) : pData(pValue), next(nullptr), prev(nullptr)
        {
            /* There is only one element in the list if the next and the prev pointers are nullptr */
        }

        RLptr(RLptr<T>& sp) : pData(sp.pData), next(nullptr), prev(nullptr)
        {
            if(pData != nullptr)
            {
                this->next = sp.next;
                this->prev = &sp;
                sp.next = this;
            }
        }

        ~RLptr()
        {
            remove();
        }

        T& operator* () const throw()
        {
          return *pData;
        }

        T* operator-> () const throw()
        {
          return pData;
        }

        T* get() const throw()
        {
          return pData;
        }

        RLptr<T>& operator = (RLptr<T>& sp) throw()
        {
            if (this != &sp) // Avoid self assignment
            {
                remove();

                if(sp.pData != nullptr)
                {
                    pData = sp.pData;
                    next = sp.next;
                    prev = &sp;
                    sp.next = this;
                }
                else
                {
                    next = nullptr;
                    prev= nullptr;
                }
            }
            return *this;
       }
    };

    void hf06(void)
    {
        RLptr<Person> p(new Person("Scott", 25));
        cout << "p is: "; p->display();
        cout << "p obj. address: " << p.get() << endl;
        {
            RLptr<Person> q = p;
            cout << "q is: "; q->display();
            cout << "q obj. address: " << q.get() << endl;
            RLptr<Person> r(new Person("Thomas", 33));
            cout << "r is: "; r->display();
            cout << "r obj. address: " << r.get() << endl;
            r = p;
            cout << "after r = p"  << endl;
            cout << "r obj. address: " << r.get() << endl;
            cout << "p obj. address: " << p.get() << endl;
            cout << "r is: "; r->display();
            RLptr<Person> s;
            RLptr<Person> t;
            t=s;
            cout << "t obj. address: " << t.get() << endl;
            cout << "s obj. address: " << s.get() << endl;
            t=r;
            cout << "after t = r"  << endl;
            cout << "t obj. address: " << t.get() << endl;
            cout << "s obj. address: " << s.get() << endl;
            cout << "r obj. address: " << r.get() << endl;
        }
        p->display();
    }
}
