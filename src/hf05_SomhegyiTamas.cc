/* Somhegyi Tamas Tibor, BLRGA6 */

/* Statikus tömbök vektorrá alakítása + Vektorok elemeinek párosítása és map-be tétele + map kiírása
 * -Az alábbi három tömbbõl készíts három vektort:
 *     -int hour[]={6,8,14,8,18,8,21};
 *     -int min[]={11,22,33,44,55,31,41};
 *     -double dat[]={36.5,37.3,38.5,37.1,37.1,37.5,37.4};
 * -Definiáld a TimeStamp osztályt! Van benne
 *     -kétparaméteres konstruktor (óra,perc)
 *     -rendezési reláció (op<), ez const, és a paramétere is const &!
 *     -friend kiírófv. (óra:perc)
 * -Töltsd fel a vektorokból a map konténert
 *     -for ciklus iteráljon végig egyszerre a vektorokon
 *     -az órának és percnek megfelelõ elemekbõl hozd létre a TimeStamp -et
 *     -a map-be az adatokat op[] vagy insert() tagfv.-nyel is beteheted
 * -Írasd ki a map tartalmát */

#include <vector>
#include <map>
#include <iterator>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace hf05
{
    class TimeStamp
    {
    private:
        static const uint32_t hour_max = 23;
        static const uint32_t min_max = 59;

        uint32_t hour;
        uint32_t min;

    public:
        TimeStamp(uint32_t hour, uint32_t min)
        {
            if((hour > hour_max) || (min > min_max))
            {
                throw invalid_argument("Not valid timestamp input parameters.");
            }
            this->hour = hour;
            this->min = min;
        }

        bool operator<(const TimeStamp& other) const
        {
            return (hour < other.hour) || ( (hour == other.hour) && (min < other.min) );
        }

        friend ostream &operator<<(ostream &os, const TimeStamp &timeStamp)
        {
            return os << timeStamp.hour << ":" << timeStamp.min;
        }
    };

    void hf05()
    {
        uint32_t hour[]={6,8,14,8,18,8,21};
        uint32_t minutes[]={11,22,33,44,55,31,41};
        double dat[]={36.5,37.3,38.5,37.1,37.1,37.5,37.4};

        vector<int> hour_vector;
        vector<int> min_vector;
        vector<double> dat_vector;

        for (size_t idx = 0; idx < sizeof(hour)/sizeof(hour[0]); ++idx) {
            hour_vector.push_back(hour[idx]);
        }

        for (size_t idx = 0; idx < sizeof(minutes)/sizeof(minutes[0]); ++idx) {
            min_vector.push_back(minutes[idx]);
        }

        for (size_t idx = 0; idx < sizeof(dat)/sizeof(dat[0]); ++idx) {
            dat_vector.push_back(dat[idx]);
        }

        const size_t min_size = min(min(hour_vector.size(), min_vector.size()), dat_vector.size());
        map<TimeStamp, double> measurementsMap;

        for (size_t idx = 0; idx < min_size; idx++)
        {
            measurementsMap.insert(pair<TimeStamp,double>(TimeStamp(hour_vector[idx], min_vector[idx]), dat_vector[idx]));
        }

        for ( map<TimeStamp,double>::const_iterator cmi = measurementsMap.cbegin();
              cmi != measurementsMap.cend(); cmi++)
        {
            cout << (*cmi).first <<" => "<<(*cmi).second << endl;
        }
    }

}
