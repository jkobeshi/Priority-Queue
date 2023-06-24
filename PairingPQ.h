// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty pairing heap with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 } {
        for (InputIterator iter = start; iter < end; ++iter) {
            push(*iter);
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        root = nullptr;
        numNodes = 0;
        if (other.root != nullptr) {
            std::deque<Node*> data;
            data.push_back(other.root);
            while (!data.empty()) {
                if (data.front()->sibling != nullptr) {
                    data.push_back(data.front()->sibling);
                }
                if (data.front()->child != nullptr) {
                    data.push_back(data.front()->child);
                }
                push(data.front()->elt);
                data.pop_front();
            }
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);
        std::swap(numNodes, temp.numNodes);
        std::swap(root, temp.root);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        if (!empty()) {
            std::deque<Node*> data;
            data.push_back(root);
            while (!data.empty()) {
                if (data.front()->sibling != nullptr) {
                    data.push_back(data.front()->sibling);
                }
                if (data.front()->child != nullptr) {
                    data.push_back(data.front()->child);
                }
                Node* temp = data.front();
                temp->parent = nullptr;
                temp->sibling = nullptr;
                temp->child = nullptr;
                data.pop_front();
                delete temp;
            }
            root = nullptr;
            numNodes = 0;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out of order and
    //              'rebuilds' the pairing heap by fixing the pairing heap invariant.
    //              You CANNOT delete 'old' nodes and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        if (!empty()) {
            std::deque<Node*> data;
            data.push_back(root);
            root = nullptr;
            while (!data.empty()) {
                Node* cur = data.front();
                data.pop_front();
                if (cur->sibling != nullptr) {
                    data.push_back(cur->sibling);
                    cur->sibling = nullptr;
                }
                if (cur->child != nullptr) {
                    data.push_back(cur->child);
                    cur->child = nullptr;
                }
                if (cur->parent != nullptr) {
                    cur->parent = nullptr;
                }
                root = meld(root, cur);
            }
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already done.
    //              You should implement push functionality entirely in the addNode()
    //              function, and this function calls addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the pairing heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the pairing heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        if (!empty()) {
            Node* temp = root;
            std::deque<Node*> dq;
            if (temp->child != nullptr) {
                dq.push_back(temp->child);
                if (dq.back()->sibling == nullptr) {
                    dq.back()->parent = nullptr;
                }
                while (dq.back()->sibling != nullptr) {
                    dq.push_back(dq.back()->sibling);
                    dq[dq.size() - 2]->parent = nullptr;
                    dq[dq.size() - 2]->sibling = nullptr;
                }
            }
            temp->parent = nullptr;
            temp->sibling = nullptr;
            temp->child = nullptr;
            delete temp;
            root = nullptr;
            --numNodes;
            while (dq.size() > 1) {
                dq.push_back(meld(dq[0], dq[1]));
                dq.pop_front(); dq.pop_front();
            }
            root = dq.front();
        }
    } // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numNodes;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return (size() == 0);
    } // empty()


    // Description: Updates the priority of an element already in the pairing heap by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE &new_value) {
        if (node != nullptr && this->compare(node->elt, new_value)) {
            node->elt = new_value;
            if (root != node && this->compare(node->parent->elt, new_value)) {
                if (node->parent->child == node) {
                    node->parent->child = node->sibling;
                }
                else {
                    Node* i = node->parent->child;
                    while (i->sibling != node) {
                        i = i->sibling;
                    }
                    i->sibling = node->sibling;
                }
                node->parent = nullptr;
                node->sibling = nullptr;
                root = meld(root, node);
            }
        }
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE &val) {
        Node* newVal = new Node(val);
        if (empty()) {
            root = newVal;
        }
        else {
            root = meld(root, newVal);
        }
        ++numNodes;
        return newVal;
    } // addNode()


private:
    Node* root = nullptr;
    int numNodes = 0;
    Node* meld(Node* lh, Node* rh) {
        if (lh == nullptr) {
            return rh;
        }
        else if (rh == nullptr) {
            return lh;
        }
        else if (this->compare(lh->elt, rh->elt)) {
            lh->parent = rh;
            lh->sibling = rh->child;
            rh->child = lh;
            return rh;
        }
        else {
            rh->parent = lh;
            rh->sibling = lh->child;
            lh->child = rh;
            return lh;
        }
    }
    // TODO: Add any additional member variables or member functions you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).

    // NOTE: For member variables, you are only allowed to add a "root pointer"
    //       and a "count" of the number of nodes.  Anything else (such as a deque)
    //       should be declared inside of member functions as needed.
};


#endif // PAIRINGPQ_H
