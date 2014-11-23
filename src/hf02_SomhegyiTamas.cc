/* Somhegyi Tamas Tibor, BLRGA6 */

/* Val�s�ts meg egy �l�l�nyeket rendszerez� oszt�lyhierarchi�t! Az �l�l�ny �soszt�ly legyen absztrakt oszt�ly,
 * �s t�rolja az �l�l�ny kor�t �vekben (int). A rendszerez�sben �llatokat �s n�v�nyeket k�l�nb�ztet�nk meg.
 * Az �llatokhoz t�roljuk a nev�t (string) �s hogy hetente h�nyszor kell etetni (int), a n�v�nyekhez t�roljuk,
 * hogy vir�gos-e (bool), �s hogy hetente h�nyszor kell �nt�zni (int). K�sz�ts olyan met�dusokat, amellyekkel
 * minden �l�l�nyr�l le lehet k�rdezni a fent megadott adatokat �s egy olyan met�dust, amivel az �sszes adat�t
 * ki lehet �ratni egyszerre! Hozz l�tre konkr�t oszt�lyp�ld�nyokat n�h�ny, min. 4, k�l�nb�z� �l�l�nyb�l �s a
 * r� mutat� pointereket helyezd el egy t�mbben, amely �soszt�ly t�pus� oszt�lyra mutat� pointereket tartalmaz,
 * majd ezt felhaszn�lva �rasd ki az �sszes �l�l�nyr�l az adatait, sz�m�tsd ki, hogy mennyi az �tlag�letkor,
 * illetve, hogy h�ny vir�gos n�v�ny van!*/

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace hf02
{

    enum class CreatureType{ ANIMAL, PLANT };

    class Creature
    {
        protected:
            CreatureType creatureType;
            int age;
        public:
            Creature(int age)
            {
                if(age < 0)
                {
                    throw domain_error("Plant parameters are invalid.");
                }
                this->age = age;
            }

            virtual void print() = 0;

            virtual ~Creature()
            {

            }

            CreatureType getCreatureType() const
            {
                return creatureType;
            }

            int getAge() const
            {
                return age;
            }
    };

    class Plant : public Creature
    {
        public:
            Plant(int age, int wateringPerWeek, bool flowery) : Creature(age)
            {
                if(wateringPerWeek < 0)
                {
                    throw domain_error("Plant parameters are invalid.");
                }

                this->wateringPerWeek = wateringPerWeek;
                this->flowery = flowery;
                this->creatureType = CreatureType::PLANT;
            }

        bool isFlowery() const
        {
            return flowery;
        }

        int getWateringPerWeek() const
        {
            return wateringPerWeek;
        }

        virtual void print() override
        {
            cout << "Plant { " << "wateringPerWeek = " << wateringPerWeek
                               << ", flowery = " << flowery
                               << ", age = " << age << " }" << endl;
        }

        private:
            int wateringPerWeek;
            bool flowery;
    };

    class Animal : public Creature
    {
        public:
            Animal(int age, const string& name, int feedingPerWeek) : Creature(age)
            {
                if(feedingPerWeek < 0)
                {
                    throw domain_error("Animal parameters are invalid.");
                }

                this->feedingPerWeek = feedingPerWeek;
                this->name = name;
                this->creatureType = CreatureType::ANIMAL;
            }

            int getFeedingPerWeek() const
            {
                return feedingPerWeek;
            }

            const string& getName() const
            {
                return name;
            }

            virtual void print() override
            {
                cout << "Animal { " << "name = " << name
                                    << ", feedingPerWeek = " << feedingPerWeek
                                    << ", age = " << age << " }" << endl;
            }

        private:
            int feedingPerWeek;
            string name;
    };

    void hf02()
    {
        Animal dog(5, "Dog", 14), cat(4, "Cat", 7), turtle(83, "Turtle", 7);
        Plant rose(2, 7, true);
        Creature* creatures[] = { &dog, &rose, &cat, &turtle };
        int length = sizeof(creatures)/sizeof(creatures[0]), floweryPlantCount = 0;
        double averageAge = 0.0;

        for (int idx = 0; idx < length; ++idx) {
            creatures[idx]->print();
            averageAge += creatures[idx]->getAge();

            /* The short-circuit of && is important */
            if( ( creatures[idx]->getCreatureType() == CreatureType::PLANT  ) &&
                ( static_cast<Plant*>(creatures[idx])->isFlowery() == true ) )
            {
                floweryPlantCount++;
            }
        }
        averageAge /= length;
        cout << "The average age of the creatures is " << averageAge << "." << endl;
        cout << "The number of flowery plants is " << floweryPlantCount << "." << endl;
        cin.get();
    }
}
