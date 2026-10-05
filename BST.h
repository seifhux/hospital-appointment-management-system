/*
Seif Hussein Mohamed    20247012
Osama Ehab Mahmoud      20247001
*/

#include <iostream>
#include <iomanip>
using namespace std;

// We called the "Appointment" class "Node" class
class Node {
public:
    string patientName;
    int priorityLevel;
    string department;
    Node* left;
    Node* right;

    Node(string patientName, int priorityLevel, string department) {
        this->patientName = patientName;
        this->priorityLevel = priorityLevel;
        this->department = department;
        left = nullptr;
        right = nullptr;
    }
};



class BST {
    Node* root;

    /* Recursive helper function for inOrder printing,
    will be used in displayAppointments function */
    int inOrder(Node* node, bool print = true) {
        if(node == nullptr) return 0;

        int count = 0;  // to keep track of the found nodes

        count += inOrder(node->left, print);

        /* this will prevent the appointment to be printed before the count as in the first call
        of the function false will be passed so it stores the value without printing
        Note: same logic will be used in inOrderConditioned */
        if(print) {
            cout << "Patient Name: " << left << setw(20) << node->patientName
                << " | Department: " << setw(15) << node->department
                << " | Priority Level: " << node->priorityLevel << endl;
        }
        count++;

        count += inOrder(node->right, print);
        return count;
    }


    /* Recursive helper function works just as inOrder but with a specific condition,
    will be used in displayMoreUrgent, LessUrgent and searchForAppointment functions*/
    int inOrderConditioned(Node* node, int priority, int condition, bool print = true) {
        if(node == nullptr) return 0;

        int count = 0;  // to keep track of the found nodes
        count += inOrderConditioned(node->left, priority, condition, print);

        // Works for MoreUrgent function
        if(condition == 1 && node->priorityLevel <= priority) {
            if(print) {
                cout << "Patient Name: " << left << setw(20) << node->patientName
                << " | Department: " << setw(15) << node->department
                << " | Priority Level: " << node->priorityLevel << endl;
            }
            count++;
        }

        // Works for LessUrgent function
        else if(condition == 0 && node->priorityLevel >= priority) {
            if(print) {
                cout << "Patient Name: " << left << setw(20) << node->patientName
                << " | Department: " << setw(15) << node->department
                << " | Priority Level: " << node->priorityLevel << endl;
            }
            count++;
        }

        // Works for searchForAppointment function
        else if(condition == 2 && node->priorityLevel == priority) {
            if(print) {
                cout << "Patient Name: " << left << setw(20) << node->patientName
                << " | Department: " << setw(15) << node->department
                << " | Priority Level: " << node->priorityLevel << endl;
            }
            count++;
        }

        count += inOrderConditioned(node->right, priority, condition, print);

        return count;
    }

public:
    BST() { root = nullptr; }

    // showMessage parameter prevents the message from being printed 10 times when reading the input file
    void scheduleAppointment(string name, int priority, string dep, bool showMessage = true) {
        Node* newAppointment = new Node(name, priority, dep);

        if (root == nullptr) {
            root = newAppointment;
            if(showMessage) cout << "Appointment scheduled.\n";
            return;
        }

        Node* tmp = root;
        while (true) {
            if (newAppointment->priorityLevel <= tmp->priorityLevel) {
                if (tmp->left == nullptr) {
                    tmp->left = newAppointment;
                    if(showMessage) cout << "Appointment scheduled.\n";
                    return;
                }
                tmp = tmp->left;
            } else {
                if (tmp->right == nullptr) {
                    tmp->right = newAppointment;
                    if(showMessage) cout << "Appointment scheduled.\n";
                    return;
                }
                tmp = tmp->right;
            }
        }
    }


    void displayAppointments() {
        if(root == nullptr) {
            cout << "No appointments found.";
            return;
        }

        int count = inOrder(root, false);
        cout << count << " appointments found.\n";
        inOrder(root);
    }


    void searchForAppointment(int priority) {
        if(root == nullptr) {
            cout << "No appointments scheduled.";
            return;
        }

        // store the number of appointments first then print if any
        int count = inOrderConditioned(root, priority, 2, false);

        if(count == 0) {
            cout << "No appointments with priority level " << priority << " found.\n";
            return;
        }

        // print appointments if any
        cout << count << " appointments found.\n";
        inOrderConditioned(root, priority, 2);
    }


    void cancelAppointment(int priority) {
        if(root == nullptr) {
            cout << "No appointments found.";
            return;
        }

        int count = 0;   // To count deleted appointments

        while(true) {
            // First find a node with the entered priority
            Node* current = root;
            Node* parent = nullptr;   // to track the parent of current
            bool isLeft = false;      // to track if current is the left or right child of parent

            while(current != nullptr && current->priorityLevel != priority) {
                parent = current;
                if(priority < current->priorityLevel) {
                    current = current->left;
                    isLeft = true;    // current is the left child
                } else {
                    current = current->right;
                    isLeft = false;   // current is the right child
                }
            }

            if(current == nullptr) break;  // when priority is not found in the whole tree

            count++;    // the first found node

            // we have 4 cases to delete "current"
            if(current->left == nullptr && current->right == nullptr) {
                // case 1: current has no children
                if(parent == nullptr) root = nullptr;   // if current was the root
                else if(isLeft) parent->left = nullptr;
                else parent->right = nullptr;
                delete current;

            } else if(current->right == nullptr) {
                // case 2: current has only left child
                if(parent == nullptr) root = current->left;
                else if(isLeft) parent->left = current->left;
                else parent->right = current->left;
                delete current;

            } else if(current->left == nullptr) {
                // case 3: current has only right child
                if(parent == nullptr) root = current->right;
                else if(isLeft) parent->left = current->right;
                else parent->right = current->right;
                delete current;

            } else {
                /* case 4: current has left and right children
                we can't remove current as other cases, so we will use the successor method */

                // first find the successor (the leftmost node of the right subtree)
                Node* succParent = current;
                Node* succ = current->right;

                while(succ->left != nullptr) {
                    succParent = succ;
                    succ = succ->left;
                }

                // then copy the succ's info into current
                current->patientName = succ->patientName;
                current->priorityLevel = succ->priorityLevel;
                current->department = succ->department;

                // then delete the successor node
                if(succParent == current) succParent->right = succ->right;
                else succParent->left = succ->right;
                delete succ;
            }
        }

    if(count == 0)
        cout << "No appointments with priority level " << priority << " found.\n";
    else
        cout << count << " appointments cancelled.\n";
    }


    void displayMoreUrgentThan(int priority) {
        if(root == nullptr) {
            cout << "No appointments found.";
            return;
        }

        int count = inOrderConditioned(root, priority, 1, false);

        if(count == 0) {
            cout << "No more urgent appointments found.\n";
            return;
        }

        cout << count << " more urgent appointments found.\n";
        inOrderConditioned(root, priority, 1);
    }


    void displayLessUrgentThan(int priority) {
        if(root == nullptr) {
            cout << "No appointments found.";
            return;
        }
        int count = inOrderConditioned(root, priority, 0, false);

        if(count == 0) {
            cout << "No less urgent appointments found.\n";
            return;
        }

        cout << count << " less urgent appointments found.\n";
        inOrderConditioned(root, priority, 0);
    }
};