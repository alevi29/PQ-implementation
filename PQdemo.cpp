#include "PQ.h"
using namespace std;

int main() {
    vector <int> IDs = {134, 410, 252, 953};
    vector <int> priorities = {4, 2, 1, 3};
    cout << "PQ 1 test:" << endl;
    PQ <int> pq;
    pq.insert(2, 4);
    pq.insert(6, 2);
    pq.insert(0, 6);
    pq.insert(12, 40);
    pq.updatePriority(12, 0);
    cout << "\nInserting elements and displaying heap:\n";
    pq.display();
    cout << "\nThe task with minimum priority is: " << pq.findMin() << endl;
    cout << "The size of the heap is: " << pq.size() << endl;
    pq.deleteMin();
    cout << "\nDisplaying heap after deleting min element:\n";
    pq.display();
    cout << "size of heap: " << pq.size() << endl;
    pq.makeEmpty();
    cout << "\nClearing and displaying empty heap\n";
    pq.display();
    cout << "size of PQ: " << pq.size() << endl;
    pq.insert(2, 4);
    pq.insert(6, 2);
    cout << "\ntesting insertion of values after clearing heap: " << endl;
    pq.display();

    cout << "\nPQ 2 test:" << endl;
    PQ <int> pq2(IDs, priorities);
    cout << "\nDisplaying heap initialized with ID and priorities arrays:\n";
    pq2.display();
    cout << "\nDisplaying the ID with minimum priority without deleting it: " << pq2.findMin() << endl;
    pq2.updatePriority(410, 5);
    pq2.updatePriority(252, 6);
    cout << "\nDisplaying tree again after changing priority of task 410 to 5 and task 252 to 6:\n";
    pq2.display();
    return 0;
}