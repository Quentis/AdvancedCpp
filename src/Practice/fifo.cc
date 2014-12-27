#include<cstring>
#include<cstdint>
#include<new>
#include<utility>

using namespace std;

template<typename T>
struct fifo_element {
    uint32_t next;
    T payload;
};

template<typename T>
class fifo
{
    private:
    static const size_t DEFAULT_POOL_SIZE = 16;
    static const size_t DEFAULT_MAX_POOL_SIZE = DEFAULT_POOL_SIZE * 4 ;
    static const int32_t INVALID_INDEX = 0x7FFFFFF;

    /* functional */
    int32_t head;
    int32_t tail;

    fifo_element<T>* pool;
    size_t pool_size;
    size_t max_pool_size;

    /* It points the first empty element */
    int32_t empty_head;

    bool realloc(size_t new_pool_size)
    {
        bool retVal = false;

        if(new_pool_size > pool_size)
        {
            fifo_element<T>* new_pool = new(nothrow) fifo_element<T>[new_pool_size];

            if(new_pool != NULL)
            {
                /* Copy the actual elements */
                memcpy(new_pool, pool, pool_size * sizeof(fifo_element<T>));
                delete[] pool;

                /* Add the new empty elements */
                new_pool[new_pool_size - 1].next = empty_head;
                empty_head = pool_size;
                for (int32_t idx = pool_size; idx < new_pool_size; ++idx)
                {
                    new_pool[idx].next = ( idx != (new_pool_size - 1) ) ? (idx + 1) : INVALID_INDEX;
                }

                /* Set the new values */
                pool = new_pool;
                pool_size = new_pool_size;

                retVal = true;
            }
        }

        return retVal;
    }

    size_t calculate_new_pool_size(void)
    {
        size_t new_pool_size = 2 * pool_size;
        if(new_pool_size > max_pool_size)
        {
            new_pool_size = max_pool_size;
        }
        return new_pool_size;
    }

    bool push_back_ipml(T& element, bool copy_nmove)
    {
        bool success = true;

        if(empty_head == INVALID_INDEX)
        {
            size_t new_pool_size = calculate_new_pool_size();
            success = realloc(new_pool_size);
        }

        if(success)
        {
            /* At this point there must be an empty element */
            int32_t new_idx = empty_head;
            empty_head = pool[empty_head].next;

            if(tail == INVALID_INDEX)
            {
                head = new_idx;
                tail = new_idx;
            }
            else
            {
                pool[tail].next = new_idx;
                tail = new_idx;
            }

            pool[new_idx].next = INVALID_INDEX;
            pool[new_idx].payload = copy_nmove ? element : move(element);
        }

        return success;
    }

    void init(size_t pool_size, size_t max_pool_size)
    {
        this->head = INVALID_INDEX;
        this->tail = INVALID_INDEX;
        this->pool = new fifo_element<T>[pool_size];
        this->pool_size = pool_size;
        this->max_pool_size = max_pool_size;
        this->empty_head = 0;

        for (int32_t idx = 0; idx < this->pool_size; ++idx)
        {
            this->pool[idx].next = ( idx != (this->pool_size - 1) ) ? (idx + 1) : INVALID_INDEX;
        }
    }

    public:
    fifo()
    {
        init(DEFAULT_POOL_SIZE, DEFAULT_MAX_POOL_SIZE);
    }

    fifo(size_t pool_size, size_t max_pool_size)
    {
        init(pool_size, max_pool_size);
    }
    ~fifo()
    {
        delete[] pool;
    }

    bool push_back(T& element)
    {
        return push_back_ipml(element, true);
    }

    bool push_back(T&& element)
    {
        return push_back_ipml(element, false);
    }

    bool pop_front(T& dest)
    {
        bool success;

        if(head == INVALID_INDEX)
        {
            success = false;
        }
        else
        {
            int32_t empty_idx = head;

            /* Copies the payload into the out parameter*/
            dest = move(pool[head].payload);

            if(head == tail)
            {
                head = INVALID_INDEX;
                tail = INVALID_INDEX;
            }
            else
            {
                head = pool[head].next;
            }

            pool[empty_idx].next = empty_head;
            empty_head = empty_idx;

            success = true;
        }

        return success;
    }
};

#include<iostream>
#include<list>

void test_int(void)
{
    fifo<int> int_fifo(2,6);
    int debug;
    bool result;


    debug = 1;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 2;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);

    debug = 3;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);
    cout << "pop_front: " << debug << ", result: " << result << endl;

    debug = 4;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 5;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);
    cout << "pop_front: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);
    cout << "pop_front: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);
    cout << "pop_front: " << debug << ", result: " << result << endl;

    result = int_fifo.pop_front(debug);
    cout << "pop_front: " << debug << ", result: " << result << endl;

    debug = 6;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 7;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 8;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 9;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 10;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 11;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

    debug = 12;
    result = int_fifo.push_back(debug);
    cout << "pushback: " << debug << ", result: " << result << endl;

}

//int main(int argc, char *argv[])
//{
//    test_int();
//    return 0;
//}
