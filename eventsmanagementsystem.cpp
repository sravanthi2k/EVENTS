#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Event structure
struct Event {
    int eventId;
    string eventName;
    string eventDate;
    string eventTime;
    string eventReg;
    string eventPrizeWin;
};

const int MAX_SIZE = 10; // Maximum number of events

// Utility function to convert string to int
int stringToInt(const string& str) {
    stringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}

// Hash table structure
struct HashTable {
    Event events[MAX_SIZE];
    bool filled[MAX_SIZE];

    HashTable() {
        for (int i = 0; i < MAX_SIZE; ++i) {
            filled[i] = false;
        }
    }
};

// Double hashing function 1
int hash1(int key) {
    // Choose a prime number less than the hash table size
    int prime = 7;
    return key % prime;
}

// Double hashing function 2
int hash2(int key) {
    // Choose a prime number less than the hash table size
    int prime = 5;
    return prime - (key % prime);
}

// Insert an event into the hash table
void insertEvent(HashTable& ht, int eventId, const string& eventName, const string& eventDate, const string& eventTime, const string& eventReg, const string& eventPrizeWin) {
    int index = hash1(eventId);
    int step = hash2(eventId);
    int collisions = 0;
    bool duplicateFound = false; // Flag to track duplicate events

    while (ht.filled[index]) {
        // Check if the event has the same date and time
        if (ht.events[index].eventDate == eventDate && ht.events[index].eventTime == eventTime) {
            cout << "Two events cannot have the same date and time!" << endl;
            duplicateFound = true;
            break;
        }
        
        cout << "Collision occurred at index " << index << endl;
        index = (index + step) % MAX_SIZE; // Double hashing for collision resolution
        collisions++;
    }

    if (duplicateFound) {
        return;
    }

    ht.events[index].eventId = eventId;
    ht.events[index].eventName = eventName;
    ht.events[index].eventDate = eventDate;
    ht.events[index].eventTime = eventTime;
    ht.events[index].eventReg = eventReg;
    ht.events[index].eventPrizeWin = eventPrizeWin;
    ht.filled[index] = true;
    cout << "Event inserted successfully!";
    if (collisions > 0) {
        cout << " (with " << collisions << " collision(s) resolved)";
    }
    cout << endl;
    
    ofstream outputFile("events.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << eventId << "," << eventName << "," << eventDate << "," << eventTime << "," << eventReg << "," << eventPrizeWin << endl;
        outputFile.close();
    } else {
        cout << "Unable to open file for writing!" << endl;
    }
}

// Search for an event in the events.txt file
void searchEvent(int eventId) {
    ifstream inputFile("events.txt");
    if (inputFile.is_open()) {
        string line;
        bool found = false;

        while (getline(inputFile, line)) {
            stringstream ss(line);
            string currentEventId, eventName, eventDate, eventTime, eventReg, eventPrizeWin;
            getline(ss, currentEventId, ',');
            getline(ss, eventName, ',');
            getline(ss, eventDate, ',');
            getline(ss, eventTime, ',');
            getline(ss, eventReg, ',');
            getline(ss, eventPrizeWin, ',');

            if (stringToInt(currentEventId) == eventId) {
                cout << "Event found:" << endl;
                cout << "Event ID: " << currentEventId << endl;
                cout << "Event Name: " << eventName << endl;
                cout << "Event Date: " << eventDate << endl;
                cout << "Event Time: " << eventTime << endl;
                cout << "Registration Fee: " << eventReg << endl;
                cout << "Prize Money: " << eventPrizeWin << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Event not found!" << endl;
        }

        inputFile.close();
    } else {
        cout << "Unable to open file for reading!" << endl;
    }
}

// Delete an event from the events.txt file
void deleteEvent(int eventId) {
    ifstream inputFile("events.txt");
    if (inputFile.is_open()) {
        string line;
        vector<string> fileContent;

        while (getline(inputFile, line)) {
            stringstream ss(line);
            string currentEventId, eventName, eventDate, eventTime, eventReg, eventPrizeWin;
            getline(ss, currentEventId, ',');
            getline(ss, eventName, ',');
            getline(ss, eventDate, ',');
            getline(ss, eventTime, ',');
            getline(ss, eventReg, ',');
            getline(ss, eventPrizeWin, ',');

            if (stringToInt(currentEventId) != eventId) {
                fileContent.push_back(line);
            }
        }

        inputFile.close();

        ofstream outputFile("events.txt", ios::out);
        if (outputFile.is_open()) {
            for (size_t i = 0; i < fileContent.size(); ++i) {
                outputFile << fileContent[i] << endl;
            }
            outputFile.close();
            cout << "Event deleted successfully!" << endl;
        } else {
            cout << "Unable to open file for writing!" << endl;
        }
    } else {
        cout << "Unable to open file for reading!" << endl;
    }
}

// Modify an event in the events.txt file
void modifyEvent(int eventId, const Event& updatedEvent) {
    ifstream inputFile("events.txt");
    if (!inputFile.is_open()) {
        cout << "Unable to open file for reading!" << endl;
        return;
    }

    string line;
    vector<string> fileContent;
    bool eventFound = false;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string currentEventId, eventName, eventDate, eventTime, eventReg, eventPrizeWin;
        getline(ss, currentEventId, ',');
        getline(ss, eventName, ',');
        getline(ss, eventDate, ',');
        getline(ss, eventTime, ',');
        getline(ss, eventReg, ',');
        getline(ss, eventPrizeWin, ',');

        if (stringToInt(currentEventId) == eventId) {
            // Use the extracted event ID as a string variable
            string extractedEventId = currentEventId;

            stringstream updatedLine;
            updatedLine << extractedEventId << "," << updatedEvent.eventName << ","
                        << updatedEvent.eventDate << "," << updatedEvent.eventTime << ","
                        << updatedEvent.eventReg << "," << updatedEvent.eventPrizeWin;
            fileContent.push_back(updatedLine.str());
            eventFound = true;
        } else {
            fileContent.push_back(line);
        }
    }
    inputFile.close();

    if (!eventFound) {
        cout << "Event not found!" << endl;
        return;
    }

    ofstream outputFile("events.txt", ios::out);
    if (!outputFile.is_open()) {
        cout << "Unable to open file for writing!" << endl;
        return;
    }

    for (vector<string>::iterator it = fileContent.begin(); it != fileContent.end(); ++it) {
        outputFile << *it << endl;
    }
    outputFile.close();

    cout << "Event modified successfully!" << endl;
}

// Display all events from the events.txt file
void displayEvents() {
    ifstream inputFile("events.txt");
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string eventId, eventName, eventDate, eventTime, eventReg, eventPrizeWin;
            getline(ss, eventId, ',');
            getline(ss, eventName, ',');
            getline(ss, eventDate, ',');
            getline(ss, eventTime, ',');
            getline(ss, eventReg, ',');
            getline(ss, eventPrizeWin, ',');

            cout << "Event ID: " << eventId << endl;
            cout << "Event Name: " << eventName << endl;
            cout << "Event Date: " << eventDate << endl;
            cout << "Event Time: " << eventTime << endl;
            cout << "Registration Fee: " << eventReg << endl;
            cout << "Prize Money: " << eventPrizeWin << endl;
            cout << "---------------------------" << endl;
        }
        inputFile.close();
    } else {
        cout << "Unable to open file for reading!" << endl;
    }
}

int main() {
    int choice;
    HashTable ht;
    while (true) {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "EVENT MANAGEMENT SYSTEM " << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "1. Insert Event" << endl;
        cout << "2. Search Event" << endl;
        cout << "3. Delete Event" << endl;
        cout << "4. Modify Event" << endl;
        cout << "5. Display Events" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int eventId;
                string eventName, eventDate, eventTime, eventReg, eventPrizeWin;
                cout << "Enter Event ID: ";
                cin >> eventId;
                cout << "Enter Event Name: ";
                cin.ignore();
                getline(cin, eventName);
                cout << "Enter Event Date: ";
                getline(cin, eventDate);
                cout << "Enter Event Time: ";
                getline(cin, eventTime);
                cout << "Enter Registration Fee: ";
                getline(cin, eventReg);
                cout << "Enter Prize Money: ";
                getline(cin, eventPrizeWin);
                insertEvent(ht, eventId, eventName, eventDate, eventTime, eventReg, eventPrizeWin);
                break;
            }
            case 2: {
                int eventId;
                cout << "Enter Event ID to search: ";
                cin >> eventId;
                searchEvent(eventId);
                break;
            }
            case 3: {
                int eventId;
                cout << "Enter Event ID to delete: ";
                cin >> eventId;
                deleteEvent(eventId);
                break;
            }
            case 4: {
                int eventId;
                Event updatedEvent;
                cout << "Enter Event ID to modify: ";
                cin >> eventId;
                cout << "Enter New Event Name: ";
                cin.ignore();
                getline(cin, updatedEvent.eventName);
                cout << "Enter New Event Date: ";
                getline(cin, updatedEvent.eventDate);
                cout << "Enter New Event Time: ";
                getline(cin, updatedEvent.eventTime);
                cout << "Enter New Registration Fee: ";
                getline(cin, updatedEvent.eventReg);
                cout << "Enter New Prize Money: ";
                getline(cin, updatedEvent.eventPrizeWin);
                modifyEvent(eventId, updatedEvent);
                break;
            }
            case 5: {
                displayEvents();
                break;
            }
            case 6: {
                cout << "Exiting..." << endl;
                return 0;
            }
            default: {
                cout << "Invalid choice! Please try again." << endl;
            }
        }
    }

    return 0;
}
