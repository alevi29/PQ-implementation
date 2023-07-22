#ifndef PQ_H
#define PQ_H
#include <bits/stdc++.h>
#include "dsexceptions.h"
using namespace std;

// PQ class
//
// Constructors:
// PQ --> constructs a new empty queue
// PQ( tasks, priorities ) --> constructs a new queue with a given set of task IDs and priorities 
// ******************PUBLIC OPERATIONS*********************
// void insert( x, p )       --> Insert task ID x with priority p 
// ID findMin( )  --> Return a task ID with the smallest priority, without removing it
// ID deleteMin( )   --> Remove and return a task ID with the smallest priority
// void updatePriority( x, p )   --> Changes priority of ID x to p (if x not in PQ, inserts x);
// bool isEmpty( )   --> Return true if empty; else false
// int size() --> return the number of task IDs in the queue 
// void makeEmpty( )  --> Remove all task IDs (and their priorities)
// ******************ERRORS********************************
// Throws UnderflowException as warranted


template <typename ID>
// ID is the type of task IDs to be used; the type must be ID (i.e., have < defined), so IDs can be AVL Tree keys.
class PQ
{

  public:

    struct AvlNode
    {
        ID element;
        AvlNode   *left;
        AvlNode   *right;
        int       height;
        int heapIndex;

        AvlNode( const ID & ele, AvlNode *lt, AvlNode *rt, int index, int h = 0 )
                : element{ ele }, left{ lt }, right{ rt }, heapIndex{ index }, height{ h } { }

        AvlNode( ID && ele, AvlNode *lt, AvlNode *rt, int index, int h = 0 )
                : element{ std::move( ele ) }, left{ lt }, right{ rt }, heapIndex{ index }, height{ h } { }
    };

    class AvlTree
    {
    public:

        AvlTree( ) : root{ nullptr }
        { }

        AvlTree( const AvlTree & rhs ) : root{ nullptr }
        {
            root = clone( rhs.root );
        }

        AvlTree( AvlTree && rhs ) : root{ rhs.root }
        {
            rhs.root = nullptr;
        }

        ~AvlTree( )
        {
            makeEmpty( );
        }

        /**
         * Deep copy.
         */
        AvlTree & operator=( const AvlTree & rhs )
        {
            AvlTree copy = rhs;
            std::swap( *this, copy );
            return *this;
        }

        /**
         * Move.
         */
        AvlTree & operator=( AvlTree && rhs )
        {
            std::swap( root, rhs.root );

            return *this;
        }

        /**
         * Find the smallest item in the tree.
         * Throw UnderflowException if empty.
         */
        const ID & findMin( ) const
        {
            if( isEmpty( ) )
                throw UnderflowException{ };
            return findMin( root )->element;
        }

        /**
         * Find the largest item in the tree.
         * Throw UnderflowException if empty.
         */
        const ID & findMax( ) const
        {
            if( isEmpty( ) )
                throw UnderflowException{ };
            return findMax( root )->element;
        }

        /**
         * Returns true if x is found in the tree.
         */
        bool contains( const ID & x ) const
        {
            return contains( x, root );
        }

        /**
         * Test if the tree is logically empty.
         * Return true if empty, false otherwise.
         */
        bool isEmpty( ) const
        {
            return root == nullptr;
        }

        /**
         * Print the tree contents in sorted order.
         */
        void printTree( ) const
        {
            if( isEmpty( ) )
                cout << "Empty tree" << endl;
            else
                printTree( root );
        }

        /**
         * Make the tree logically empty.
         */
        void makeEmpty( )
        {
            makeEmpty( root );
        }

        /**
         * Insert x into the tree; duplicates are ignored.
         */
        void insert( const ID & x, int index )
        {
            insert( x, root, index );
        }

        /**
         * Insert x into the tree; duplicates are ignored.
         */
        void insert( ID && x )
        {
            insert( std::move( x ), root );
        }

        /**
         * Remove x from the tree. Nothing is done if x is not found.
         */
        void remove( const ID & x )
        {
            remove( x, root );
        }

        AvlNode* find(const ID &x) {
            if (isEmpty()) {
                return nullptr;
            }
            return find(root, x);
        }
    private:
        AvlNode *root;


        /**
         * Internal method to insert into a subtree.
         * x is the item to insert.
         * t is the node that roots the subtree.
         * Set the new root of the subtree.
         */
        void insert( const ID & x, AvlNode * & t, int curIndex )
        {
            if( t == nullptr )
                t = new AvlNode{ x, nullptr, nullptr, curIndex };
            else if( x < t->element )
                insert( x, t->left, curIndex );
            else if( t->element < x )
                insert( x, t->right, curIndex );

            balance( t );
        }

        /**
         * Internal method to insert into a subtree.
         * x is the item to insert.
         * t is the node that roots the subtree.
         * Set the new root of the subtree.
         */
        void insert( ID && x, AvlNode * & t )
        {
            if( t == nullptr )
                t = new AvlNode{ std::move( x ), nullptr, nullptr };
            else if( x < t->element )
                insert( std::move( x ), t->left );
            else if( t->element < x )
                insert( std::move( x ), t->right );

            balance( t );
        }

        /**
         * Internal method to remove from a subtree.
         * x is the item to remove.
         * t is the node that roots the subtree.
         * Set the new root of the subtree.
         */
        void remove( const ID & x, AvlNode * & t )
        {
            if( t == nullptr )
                return;   // Item not found; do nothing

            if( x < t->element )
                remove( x, t->left );
            else if( t->element < x )
                remove( x, t->right );
            else if( t->left != nullptr && t->right != nullptr ) // Two children
            {
                t->element = findMin( t->right )->element;
                remove( t->element, t->right );
            }
            else
            {
                AvlNode *oldNode = t;
                t = ( t->left != nullptr ) ? t->left : t->right;
                delete oldNode;
            }

            balance( t );
        }

        static const int ALLOWED_IMBALANCE = 1;

        // Assume t is balanced or within one of being balanced
        void balance( AvlNode * & t )
        {
            if( t == nullptr )
                return;

            if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE )
                if( height( t->left->left ) >= height( t->left->right ) )
                    rotateWithLeftChild( t );
                else
                    doubleWithLeftChild( t );
            else
            if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE )
                if( height( t->right->right ) >= height( t->right->left ) )
                    rotateWithRightChild( t );
                else
                    doubleWithRightChild( t );

            t->height = max( height( t->left ), height( t->right ) ) + 1;
        }

        /**
         * Internal method to find the smallest item in a subtree t.
         * Return node containing the smallest item.
         */
        AvlNode * findMin( AvlNode *t ) const
        {
            if( t == nullptr )
                return nullptr;
            if( t->left == nullptr )
                return t;
            return findMin( t->left );
        }

        /**
         * Internal method to find the largest item in a subtree t.
         * Return node containing the largest item.
         */
        AvlNode * findMax( AvlNode *t ) const
        {
            if( t != nullptr )
                while( t->right != nullptr )
                    t = t->right;
            return t;
        }


        /**
         * Internal method to test if an item is in a subtree.
         * x is item to search for.
         * t is the node that roots the tree.
         */
        bool contains( const ID & x, AvlNode *t ) const
        {
            if( t == nullptr )
                return false;
            else if( x < t->element )
                return contains( x, t->left );
            else if( t->element < x )
                return contains( x, t->right );
            else
                return true;    // Match
        }
/****** NONRECURSIVE VERSION*************************
    bool contains( const ID & x, AvlNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

        /**
         * Internal method to make subtree empty.
         */
        void makeEmpty( AvlNode * & t )
        {
            if( t != nullptr )
            {
                makeEmpty( t->left );
                makeEmpty( t->right );
                delete t;
            }
            t = nullptr;
        }

        /**
         * Internal method to print a subtree rooted at t in sorted order.
         */
        void printTree( AvlNode *t ) const
        {
            if( t != nullptr )
            {
                printTree( t->left );
                cout << "task: " << t->element << ", index in heap: " << t->heapIndex << ", node address: " << t << endl;
                printTree( t->right );
            }
        }

        /**
         * Internal method to clone subtree.
         */
        AvlNode * clone( AvlNode *t ) const
        {
            if( t == nullptr )
                return nullptr;
            else
                return new AvlNode{ t->element, clone( t->left ), clone( t->right ), t->height };
        }
        // Avl manipulations
        /**
         * Return the height of node t or -1 if nullptr.
         */
        int height( AvlNode *t ) const
        {
            return t == nullptr ? -1 : t->height;
        }

        int max( int lhs, int rhs ) const
        {
            return lhs > rhs ? lhs : rhs;
        }

        /**
         * Rotate binary tree node with left child.
         * For AVL trees, this is a single rotation for case 1.
         * Update heights, then set new root.
         */
        void rotateWithLeftChild( AvlNode * & k2 )
        {
            AvlNode *k1 = k2->left;
            k2->left = k1->right;
            k1->right = k2;
            k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
            k1->height = max( height( k1->left ), k2->height ) + 1;
            k2 = k1;
        }

        /**
         * Rotate binary tree node with right child.
         * For AVL trees, this is a single rotation for case 4.
         * Update heights, then set new root.
         */
        void rotateWithRightChild( AvlNode * & k1 )
        {
            AvlNode *k2 = k1->right;
            k1->right = k2->left;
            k2->left = k1;
            k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
            k2->height = max( height( k2->right ), k1->height ) + 1;
            k1 = k2;
        }

        /**
         * Double rotate binary tree node: first left child.
         * with its right child; then node k3 with new left child.
         * For AVL trees, this is a double rotation for case 2.
         * Update heights, then set new root.
         */
        void doubleWithLeftChild( AvlNode * & k3 )
        {
            rotateWithRightChild( k3->left );
            rotateWithLeftChild( k3 );
        }

        /**
         * Double rotate binary tree node: first right child.
         * with its left child; then node k1 with new right child.
         * For AVL trees, this is a double rotation for case 3.
         * Update heights, then set new root.
         */
        void doubleWithRightChild( AvlNode * & k1 )
        {
            rotateWithLeftChild( k1->right );
            rotateWithRightChild( k1 );
        }

        AvlNode* find(AvlNode* node, const ID &x) {
            if (node != nullptr) {
                if (node->element == x) {
                    return node;
                }
                else {
                    AvlNode* found = find(node->left, x);
                    if (found == nullptr) {
                        found = find(node->right, x);
                    }
                    return found;
                }
            }
            else {
                return nullptr;
            }
        }
    };

    // Constructor
    // Initializes a new empty PQ
    PQ() {
        AvlTree tasks;
        vector <pair <int, AvlNode*>> heap;// stores pairs of priority and avl node
        tree = tasks;
        objects = heap;
        thesize = 0;
    }

    // Constructor
    // Initializes a new PQ with a given set of tasks IDs and priorities  
    //      priority[i] is the priority for ID task[i] 
    PQ( const std::vector<ID> & tasks, const std::vector<int> & priorities ) {
        thesize = 0;
        int n = priorities.size();
        AvlTree taskTree;
        vector <pair <int, AvlNode*>> heap;
        objects = heap;
        tree = taskTree;
        for (int i = 0; i < n; i++) {
            insert(tasks[i], priorities[i]);
        }
    }
						     
    // Emptiness check 
    bool isEmpty( ) const {
        return objects.empty();
    }

    // Deletes and Returns a task ID with minimum priority
    //    Throws exception if queue is empty
    const ID & deleteMin() {
        if (isEmpty()) {
            throw UnderflowException{};
        }
        thesize--;
        ret = objects[0].second->element;
        objects[0] = objects[thesize];
        objects.pop_back();
        percolateDown(0);
        tree.remove(ret);
        return ret;
    }

    // Returns an ID with minimum priority without removing it
    //     Throws exception if queue is empty
    const ID & findMin() const {
        if (isEmpty()) {
            throw UnderflowException{};
        }
        return objects[0].second->element;
    }

    // Insert ID x with priority p.
    void insert(const ID &x, int p) { // inserts ID x with priority p
        tree.insert(x, thesize);
        objects.push_back(pair <int, AvlNode*> (p, tree.find(x)));
        percolateUp(thesize);
        thesize++;
    }

    // Update the priority of ID x to p
    //    Inserts x with p if s not already in the queue
    void updatePriority( const ID & x, int p ) {
        AvlNode* node = tree.find(x);
        if (node == nullptr) {
            insert(x, p);
        }
        else {
            int temp = objects[node->heapIndex].first;
            objects[node->heapIndex].first = p;
            if (p > temp) {
                percolateDown(node->heapIndex);
            }
            else {
                percolateUp(node->heapIndex);
            }
        }
    }

    // Return the number of task IDs in the queue
    int size() const {
        return objects.size();
    }

    // Delete all IDs from the PQ
    void makeEmpty( ) {
        objects.clear();
        thesize = 0;
        tree.makeEmpty();
    }

    void display() {
        cout << "tree:" << endl;
        tree.printTree();
        cout << "heap:" << endl;
        if (thesize == 0) {
            cout << "Empty heap" << endl;
        }
        else {
            for (int i = 0; i < thesize; i++) {
                cout << "priority: " << objects[i].first << ", node: " << objects[i].second << endl;
            }
        }
    }

private:

    void percolateDown(int index) {
        int child = lchild(index);
        while (child < thesize) {
            if (rchild(index) < thesize && objects[rchild(index)].first < objects[child].first) {
                child = rchild(index);
            }
            if (objects[child].first < objects[index].first) {
                swap(objects[child].second->heapIndex, objects[index].second->heapIndex);
                swap(objects[child], objects[index]);
                index = child;
                child = lchild(index);
            }
            else {
                break;
            }
        }
    }

    void percolateUp(int index) {
        int p = parent(index);
        while (index > 0 && objects[index].first < objects[p].first) {
            swap(objects[index].second->heapIndex, objects[p].second->heapIndex);
            swap(objects[index], objects[p]);
            index = p;
            p = parent(index);
        }
    }

    int lchild(int index) {
        return 2 * index + 1;
    }

    int rchild(int index) {
        return 2 * index + 2;
    }

    int parent(int index) {
        return floor((index - 1) / 2);
    }

    int thesize;
    vector <pair <int, AvlNode*>> objects;
    ID ret;
    AvlTree tree;

};


#endif
