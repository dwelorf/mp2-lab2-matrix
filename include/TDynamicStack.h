#ifndef TDYNAMICSTACK_H
#define TDYNAMICSTACK_H

#include <algorithm>
#include <stdexcept>

template<typename T>
class TDynamicStack {
private:
    int top;
    size_t memSize;
    T* pMem;

public:
    TDynamicStack(size_t _memSize = 1) :
        top(-1), memSize(_memSize), pMem(new T[memSize]) {
    }

    ~TDynamicStack() {
        delete[] pMem;
    }

    size_t size() const {
        return top + 1;
    }

    bool IsEmpty() const {
        return top == -1;
    }

    bool IsFull() const {
        return top == static_cast<int>(memSize) - 1;
    }

    T Pop() {
        if (IsEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return pMem[top--];
    }

    void Push(const T& val) {
        if (IsFull()) {
            size_t newSize = memSize * 2;
            T* tmpMem = new T[newSize];

            std::copy(pMem, pMem + memSize, tmpMem);

            delete[] pMem;
            pMem = tmpMem;
            memSize = newSize;
        }
        pMem[++top] = val;
    }

    T& Top() {
        if (IsEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return pMem[top];
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return pMem[top];
    }
};

#endif