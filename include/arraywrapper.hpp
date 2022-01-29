#ifndef __arraywrapper_hpp_
#define __arraywrapper_hpp_

#include <chrono>
#include <functional>
#include <ostream>

class ArrayWrapper {
private:
    int _size;
    float *_data;
    float *_indices; /// as input to shaders
    float waitQueue = 0;

    void delay();
public:
    struct Stats{
        unsigned int size = 0;
        unsigned long long int reads = 0;
        unsigned long long int writes = 0;
        unsigned long long int compares = 0;
        unsigned long long int swaps = 0;
        std::chrono::duration<double> waited = std::chrono::seconds(0);

        Stats() {
            reset();
        }

        void reset() {
            reads = 0; writes = 0; compares = 0; swaps = 0; waited = std::chrono::seconds(0);
        }
    };
    Stats stats;
    std::function<float(int)> get_delay;

    ArrayWrapper(int size);
    ~ArrayWrapper();

    float get(int idx);
    void set(int idx, float val);
    void swap(int i, int j);
    int compare(int idx1, int idx2);
    void shuffle();

    int size() { return _size; }
    float* data() { return _data; }
    float* indices() { return _indices; }
};

std::ostream& operator<<(std::ostream& o, const ArrayWrapper::Stats& stats);

#endif
