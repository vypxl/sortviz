#ifndef __arraywrapper_hpp_
#define __arraywrapper_hpp_

#include <ostream>

class ArrayWrapper {
private:
    int _size;
    float *_data;
    float *_indices; /// as input to shaders
public:
    struct Stats{
        unsigned int size = 0;
        unsigned long long int reads = 0;
        unsigned long long int writes = 0;
        unsigned long long int compares = 0;
        unsigned long long int swaps = 0;

        Stats() {
            reset();
        }

        void reset() {
            reads = 0; writes = 0; compares = 0; swaps = 0;
        }
    };
    Stats stats;

    ArrayWrapper(int size);
    ~ArrayWrapper();

    void delay(int micros);

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
