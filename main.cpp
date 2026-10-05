/*
Seif Hussein Mohamed    20247012
Osama Ehab Mahmoud      20247001
*/

#include "BST.h"
#include <iostream>
#include <fstream>
using namespace std;


int main() {
    BST hospital;

    // read the input file and load its data into the BST
    ifstream file("appointments.txt");
    
    cout << "\nSystem starting...\n";

    if(!file.is_open()) cout << "File load failed. Starting system empty.";
    else { 
        int n;
        file >> n;
        file.ignore();
        
        for(int i = 0; i < n; i++) {
            string name, dept;
            int priorityLvl;

            getline(file, name);
            file >> priorityLvl;
            file.ignore();
            getline(file, dept);

            hospital.scheduleAppointment(name, priorityLvl, dept, false);
        }

        file.close();
        cout << n << " appointments loaded successfully.";
    }


    while(true) {
        int userChoice = 0;
        string name, dept;
        int priorityLvl;

        cout << "\n\tHospital System Menu\n";
        cout << "\t====================" << endl
            << "[1] Schedule an appointment" << endl 
            << "[2] Display all appointments" << endl
            << "[3] Search for an appointment" << endl
            << "[4] Cancel an appointment" << endl
            << "[5] Display more urgent than" << endl
            << "[6] Display less urgent than" << endl
            << "[7] Exit" << endl
            << "----------------------------" << endl
            << "Pick an option: ";
        cin >> userChoice;
        cin.ignore();

        switch(userChoice) {
            case 1: {
                cout << "Enter Patient Name: ";
                getline(cin, name);
                cout << "Enter Medical Department: ";
                getline(cin, dept);
                cout << "Enter Priority Level (1 is the most urgent): ";
                cin >> priorityLvl;

                // input validation for priority level
                if(priorityLvl <= 0) {
                    cout << "Priority level can't be less than 1.";
                    break;
                }

                hospital.scheduleAppointment(name, priorityLvl, dept);
                break;
            }

            case 2: {
                hospital.displayAppointments();
                break;
            }
            
            case 3: {
                cout << "Enter priority level to start search: ";
                cin >> priorityLvl;

                // input validation
                if(priorityLvl <= 0) {
                    cout << "Priority level can't be less than 1.";
                    break;
                }

                hospital.searchForAppointment(priorityLvl);
                break;
            }

            case 4: {
                cout << "Enter priority level to cancel: ";
                cin >> priorityLvl;

                // input validation
                if(priorityLvl <= 0) {
                    cout << "Priority level can't be less than 1.";
                    break;
                }

                hospital.cancelAppointment(priorityLvl);
                break;
            }

            case 5: {
                cout << "Enter priority level to start search: ";
                cin >> priorityLvl;

                // input validation
                if(priorityLvl <= 0) {
                    cout << "Priority level can't be less than 1.";
                    break;
                }

                hospital.displayMoreUrgentThan(priorityLvl);
                break;
            }

            case 6: {
                cout << "Enter priority level to start search: ";
                cin >> priorityLvl;

                // input validation
                if(priorityLvl <= 0) {
                    cout << "Priority level can't be less than 1.";
                    break;
                }

                hospital.displayLessUrgentThan(priorityLvl);
                break;
            }

            case 7: {
                cout << "System Closing..."; 
                return 0;
            }

            default: {
                cout << "Invalid Option. Try again.\n";
                break;
            }

        }
    }   
}