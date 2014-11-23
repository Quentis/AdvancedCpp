/* Somhegyi Tamas Tibor, BLRGA6 */

/* Készíts C++ programot, amely idõpontot (óra és perc formájában) tároló struktúrák egyszeresen láncolt listáját kezeli.
 * A program kérjen be egy idõpontot, eközben ne fogadjon el helytelen adatokat a az óra (0<=óra<=23, egész) és perc
 * (0<=perc<=59, egész) megadásakor, és egy idõtartamot percben, ami nullánál nagyobb és max. 300, egész értéket fogadhat el.
 *  Egyértelmûen add a felhasználó tudtára, hogy mit kérsz be és milyen formában! Kezeld a hibás bemenetet, azaz kérd be újra,
 * amíg nem megfelelõ az input (pl. negatív szám, betû szám helyett, stb.) és jelezd a felhasználónak a hibát!
 * A láncolt listára fûzd fel a következõ 4 idõpontot: a bekért idõpont, valamint a bekért idõponthoz hozzáadva a idõtartamot 1x, 2x, 3x.
 * Az idõpont óraként átfordul 23:59 után 00:00 következik. Ezután a láncolt listán végighaladva táblázatos formában írasd ki
 * az idõpontot számmal és ebbõl származtatva szövegesen ékezetek nélküli magyar szavakkal, úgy hogy minden sorban
 * kiférjenek az adatok, és a leghosszabb sorban 1-1 üres karakternyi hely legyen az elválasztó oszlophatároló ‘|’ karakter
 * mellett. A  szöveges oszlop legyen középre igazítva (elõtte és utána lévõ üres helyek számának különbsége max. 1 lehet).
 * Minta megjelenítés a honlapon! */

#include <iostream>
#include <stdexcept>
#include <limits>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

namespace hf01
{
    const char* decimalZeroToString[] = { "nulla", "egy", "ket", "harom", "negy", "ot", "hat", "het", "nyolc", "kilenc" };
    const char* decimalOneToString[] = { "dummy", "tizen", "huszon", "harminc", "negyven", "otven", "hatvan" };

    struct Time
    {
        const uint8_t hour_max = 23;
        const uint8_t hour_min =  0;
        const uint8_t minute_max = 59;
        const uint8_t minute_min = 0;

        uint32_t hour;
        uint32_t minute;
        struct Time* next;

        Time(int32_t hour, int32_t minute)
        {
            string exceptionStr;
            if( hour < hour_min )
            {
                exceptionStr += "Invalid value as hour - Less than min\n";
            }
            else if( hour > hour_max )
            {
                exceptionStr += "Invalid value as hour - More than max\n";
            }

            if( minute < minute_min )
            {
                exceptionStr += "Invalid value as minute - Less than min\n";
            }
            else if( minute > minute_max )
            {
                exceptionStr += "Invalid value as minute - More than max\n";
            }

            if(!exceptionStr.empty())
            {
                throw invalid_argument(string(exceptionStr));
            }

            this->hour = hour;
            this->minute = minute;

        }

        void add(uint32_t minutes)
        {
            minute = minute + minutes;
            hour = ( hour + ( minute / (minute_max + 1) ) ) % (hour_max + 1);
            minute = minute % (minute_max + 1);
        }

        string getNumAsStr(uint32_t num)
        {
            string str;
            switch(num)
            {
                case 10:
                    str = "tiz";
                    break;
                case 20:
                    str = "husz";
                    break;
                default:
                    if( (num / 10) % 10 != 0)
                    {
                        str += decimalOneToString[(num / 10) % 10];
                    }
                    str += decimalZeroToString[num % 10];
                    break;
            }
            return str;
        }

        string toString(bool numAsText)
        {
            string str;

            if(numAsText)
            {
                str += getNumAsStr(hour);
                str += " ora ";
                if(minute != 0)
                {
                    str += getNumAsStr(minute);
                    str += " perc";
                }
            }
            else
            {
                str += (char) (((hour / 10) % 10) + 48);
                str += (char) ((hour % 10) + 48);
                str += ":";
                str += (char) (((minute / 10) % 10) + 48);
                str += (char) ((minute % 10) + 48);
            }
            return str;
        }

    };

    struct TimeList
    {
        Time* head = nullptr;
        Time* tail = nullptr;
        int length; /* Not necessary but it can be useful */

        void addTime(Time* time)
        {
            time->next = nullptr;
            if(tail == nullptr)
            {
                head = time;
                tail = time;
                length = 1;
            }
            else
            {
                tail->next = time;
                tail = time;
                length++;
            }
        }

        Time* getTime(void)
        {
            Time* time = nullptr;
            if(head != nullptr)
            {
                time = head;
                head = head->next;
                length--;
                if(head == nullptr)
                {
                    tail = nullptr;
                }
            }
            return time;
        }

    };

    bool consoleInError(const char* errMsg)
    {
        bool isInvalid = !cin;
        if(isInvalid)
        {
            cout << errMsg << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<int>::max(),'\n');
            cin.get();  /* New line is removed here */
        }
        return isInvalid;
    }

    void consoleRead(TimeList& tList, uint32_t& deltaMinute)
    {
        bool isTimeValid = false;
        int32_t hour, minute;
        char c;

        cout << "Please provide the time and the delta minutes in the following format:"
                "\"hh:mm dt\" where (0 <= hh <= 23, 0 <= mm <= 59) and (0 <= dt <= 300)\n" << endl;

        while(!isTimeValid)
        {
            cin >> hour;
            if (consoleInError("The provided input for hour is not a number.\n") ) continue;

            cin >> c;
            if(c != ':')
            {
                cout << "Hours and minutes should be separated by ':'.\n";
                continue;
            }

            cin >> minute;
            if (consoleInError("The provided input for minute is not a number.\n") ) continue;

            cin >> deltaMinute;
            if (consoleInError("The provided input as delta minutes is not a number. (0 < deltaMinutes < 300)\n") ) continue;

            Time* timePtr[4] = { nullptr, nullptr, nullptr, nullptr };

            try
            {
                for(uint32_t i = 0; i < 4; i++)
                {
                    timePtr[i] = new Time(hour, minute);
                }
            }
            catch(const invalid_argument& e)
            {
                cout << e.what();
                for(uint32_t i = 0; i < 4; i++)
                {
                    if(timePtr[i] != nullptr)
                    {
                        delete timePtr[i];
                    }
                }
                continue;
            }

            if( (deltaMinute < 0) || (300 < deltaMinute) )
            {
                cout << "Valid range of delta minutes is [0,300].\n";
            }
            else
            {
                for(uint32_t i = 0; i < 4; i++)
                {
                    tList.addTime(timePtr[i]);
                }
                isTimeValid = true;
            }
            cin.get();  /* Removes the new line character */
        }

        Time* timePtr = tList.head;
        for(uint32_t i = 0; timePtr!= nullptr; i++)
        {
            timePtr->add(i * deltaMinute);
            timePtr = timePtr->next;
        }
    }

    void printConsole(TimeList& tList)
    {
        Time* timePtr = tList.head;
        uint32_t maxWidth = 0, strLen = 0;

        for(uint32_t i = 0; timePtr!= nullptr; i++)
        {
            strLen = timePtr->toString(true).size();
            if(strLen > maxWidth)
            {
                maxWidth = strLen;
            }
            timePtr = timePtr->next;
        }
        maxWidth += 2;

        timePtr = tList.head;
        for(uint32_t i = 0; timePtr!= nullptr; i++)
        {
            uint8_t leftSpaceCnt, rightSpaceCnt;

            cout << "| " << timePtr->toString(false) << " |";
            strLen = timePtr->toString(true).size();

            leftSpaceCnt = (maxWidth - strLen) / 2;
            rightSpaceCnt = maxWidth - strLen - leftSpaceCnt;

            cout << string(leftSpaceCnt,' ') <<timePtr->toString(true) << string(rightSpaceCnt,' ') << "|" << endl;
            timePtr = timePtr->next;
        }
        cout.flush();
    }

    void hf01(void)
    {
        uint32_t deltaMinute = 0;

        hf01::TimeList tList;
        hf01::consoleRead(tList, deltaMinute);
        hf01::printConsole(tList);
        deltaMinute++;
        cout << "Press any key to exit..." << endl;
        cin.get();
    }
}
