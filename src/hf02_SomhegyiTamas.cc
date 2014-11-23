/* Somhegyi Tamas Tibor, BLRGA6 */

/* Valósíts meg egy élõlényeket rendszerezõ osztályhierarchiát! Az élõlény õsosztály legyen absztrakt osztály,
 * és tárolja az élõlény korát években (int). A rendszerezésben állatokat és növényeket különböztetünk meg.
 * Az állatokhoz tároljuk a nevét (string) és hogy hetente hányszor kell etetni (int), a növényekhez tároljuk,
 * hogy virágos-e (bool), és hogy hetente hányszor kell öntözni (int). Készíts olyan metódusokat, amellyekkel
 * minden élõlényrõl le lehet kérdezni a fent megadott adatokat és egy olyan metódust, amivel az összes adatát
 * ki lehet íratni egyszerre! Hozz létre konkrét osztálypéldányokat néhány, min. 4, különbözõ élõlénybõl és a
 * rá mutató pointereket helyezd el egy tömbben, amely õsosztály típusú osztályra mutató pointereket tartalmaz,
 * majd ezt felhasználva írasd ki az összes élõlényrõl az adatait, számítsd ki, hogy mennyi az átlagéletkor,
 * illetve, hogy hány virágos növény van!*/

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
