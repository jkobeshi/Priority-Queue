// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{ start, end } {
        updatePriorities();
        // TODO: Implement this function.
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for (int i = int(data.size()) - 1; i >= 0; i--) {
            fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE &val) {
        data.push_back(val);
        fixUp(int(size())-1);
        // TODO: Implement this function.
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        if (!empty()) {
            std::swap(data.front(), data.back());
            data.pop_back();
            fixDown(0);
        }
        // TODO: Implement this function.
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.
        return data.front();
        // These lines are present only so that this provided file compiles.
        // TODO: Delete this line
        // TODO: Delete or change this line
    } // top()

    
    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        // TODO: Delete or change this line
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used for your heap implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables.  You don't need
    //       a "heapSize", since you can call your own size() member function,
    //       or check data.size().

    // TODO: Add any additional member functions or data you require here.
    //       For instance, you might add fixUp() and fixDown().

    void fixUp(int ind) {
        if (this->compare(data[((ind - 1) / 2)], data[ind])) {
            std::swap(data[((ind - 1) / 2)], data[ind]);
            if (ind > 0) {
                fixUp(((ind - 1) / 2));
            }
        }
    }
    void fixDown(int ind) {
        int extr = ind;
        if ((((2 * ind) + 1) < int(size())) && this->compare(data[ind], data[((2 * ind) + 1)])) {
            extr = ((2 * ind) + 1);
        }
        if ((((2 * ind) + 2) < int(size())) && this->compare(data[extr], data[((2 * ind) + 2)])) {
            extr = ((2 * ind) + 2);
        }
        if (extr != ind) {
            std::swap(data[extr], data[ind]);
            fixDown(extr);
        }
    }
    
}; // BinaryPQ


#endif // BINARYPQ_H
