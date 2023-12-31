// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to call updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes).
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues, you have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Eecs281PQ.h"
#include "BinaryPQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int *b) const {
        return *a < *b;
    }
};


// TODO: Make sure that you're using this-compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, and call
// this function from main().
void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };
    struct HiddenDataComp {
        bool operator()(const HiddenData &/*a*/, const HiddenData &/*b*/) const {
            // TODO: Finish this functor; when you do, uncomment the
            // parameters in the line above
            return false;
        }
    };

    cout << "Testing " << pqType << " with hidden data" << endl;

    // TODO: Add code here to actually test with the HiddenData type.
} // testHiddenData()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data;
    data.reserve(100);
    data.push_back(1);
    data.push_back(5);
    data.push_back(21);
    data.push_back(32);

    // NOTE: If you add more data to the vector, don't push the pointers
    // until AFTER the vector stops changing size!  Think about why.
    // You can add up to 100 values, or change the reserve if you want more.

    for (size_t i = 0; i < data.size(); ++i) {
        pq->push(&data[i]);
    } // for

    // Change the first value (which is pointed to by the pq), and check it.

    data[0] = 10;
    data[3] = 0;
    data[1] = 50;
    data[2] = 29;
    pq->updatePriorities();
    assert(*pq->top() == 50);
    cout << "testUpdatePrioritiesHelper() succeeded!" << endl;
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;

    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } // if
    // TODO: Add more types here inside 'else if' statements, like in main().
    else if (pqType == "Sorted") {
        pq = new SortedPQ<int *, IntPtrComp>;
    }
    else if (pqType == "Binary") {
        pq = new BinaryPQ<int*, IntPtrComp>;
    }
    else if (pqType == "Pairing") {
        pq = new PairingPQ<int*, IntPtrComp>;
    }
    if (!pq) {
        cout << "Invalid pq pointer; did you forget to create it?" << endl;
        return;
    } // if

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    assert(pq->top() == 3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);
    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());
    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());
     
    pq->push(5);
    assert(pq->top() == 5);
    pq->push(2);
    assert(pq->top() == 5);
    pq->push(10);
    assert(pq->top() == 10);
    pq->push(3);
    assert(pq->top() == 10);
    pq->pop();
    assert(pq->top() == 5);
    pq->pop();
    assert(pq->top() == 3);
    assert(pq->size() == 2);
    pq->push(29);
    assert(pq->top() == 29);
    pq->push(31);
    assert(pq->top() == 31);
    pq->push(60);
    assert(pq->top() == 60);
    pq->push(56);
    assert(pq->top() == 60);
    pq->push(21);
    assert(pq->top() == 60);
    pq->push(42);
    assert(pq->top() == 60);
    pq->push(35);
    assert(pq->top() == 60);
    pq->push(11);
    assert(pq->top() == 60);
    pq->push(9);
    assert(pq->top() == 60);
    pq->push(0);
    assert(pq->top() == 60);
    pq->push(1);
    assert(pq->size() == 13);
    assert(pq->top() == 60);
    assert(!pq->empty());
    pq->pop();
    assert(pq->top() == 56);
    assert(pq->size() == 12);
    assert(!pq->empty());
    pq->pop();
    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().
//
void testPairing(vector<int> & vec) { 
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);

    pq1->push(3);
    pq2->pop();
    assert(pq2->top() == 7);
    assert(pq1->size() == 6);
    assert(pq2->size() == 4);
    assert(!pq1->empty());
    assert(pq1->top() == 10);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());

    cout << "Basic tests done." << endl;
    // TODO: Add more code to test addNode(), updateElt(), etc.
    pq3->updateElt(pq3->addNode(5), 20);
    assert(pq3->top() == 20);
    pq3->updateElt(pq3->addNode(5), 15);
    assert(pq3->top() == 20);
    cout << "Calling destructors" << endl;
    delete pq1;
    delete pq2;
    delete pq3;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    unsigned int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(10);
        vec.push_back(7);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()