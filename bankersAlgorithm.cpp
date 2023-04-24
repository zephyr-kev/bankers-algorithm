#include <iostream>
#include <thread>
#include <fstream>
using namespace std;

const int T = 4; // Threads
const int R = 3; // Resources

string resourceNames[R]; // Store resource name
int maxResource[R]; // Avaliable resources of each type

string threadName[T]; // Store thread name
int resourcesRequested[T][R]; // Number of each resource that each thread may want
int resourcesAllocated[T][R]; // Number of each resource that each thread is using
int neededResources[T][R]; // Number of resources that each thread might still request

int currentAvailResources[R]; // Current available resources

void readFile(string filename) {
    ifstream fin;
    fin.open(filename);
    char ignore;

    // Get Resources
    for (int i = 0; i < R; i++) {
        fin >> resourceNames[i] >> maxResource[i];
    }

    fin >> ignore; // Handles '#', I don't know the proper way to do this...

    // Get Threads and Resources Requested
    for (int i = 0; i < T; i++) {
        fin >> threadName[i];
        for (int j = 0; j < R; j++) {
            fin >> resourcesRequested[i][j];
        }
    }

    fin >> ignore;

    // Get Resources Allocated
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            fin >> resourcesAllocated[i][j];
        }
    }
}

void calcNeededResources() {
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            // Needed = Max need to complete - Allocated
            neededResources[i][j] = resourcesRequested[i][j] - resourcesAllocated[i][j];
        }
    }
}

void getAvailResources() {
    // Get available resources
    for (int i = 0; i < R; i++) {
        currentAvailResources[i] = maxResource[i];
    }

    // Available = Max - Sum of Allocated
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < R; j++) {
            currentAvailResources[j] -= resourcesAllocated[i][j];
        }
    }
}

bool isSafeState() {
    string completedThreads[T];
    bool finished[T];

    // Set all of finished to false.
    for (int i = 0; i < T; i++) {
        finished[i] = false;
    }

    calcNeededResources();
    getAvailResources();

    for (int i = 0; i < T; i++) {

        /*
        cout << "-----------Completed threads: ";
        for (int i = 0; i < T; i++) {
            cout << completedThreads[i] << " ";
        }
        cout << endl;

        cout << "Thread: " << i << endl;
        */
        // Find thread that can be completed
        if (!finished[i]) {
            // Check each thread
            for (int j = 0; j < T; j++) {
                
                // Check if process is already completed.
                bool processCompleted = false;
                for (int i = 0; i < T; i++) {
                    if (threadName[j] == completedThreads[i]) {
                        processCompleted = true;
                    }
                }

                if (!processCompleted) {
                    //cout << "Process: " << threadName[j] << endl;
                    // Check each resource
                    int hasAllResources = 0;
                    for (int k = 0; k < R; k++) {
                        //cout << "Current for " << resourceNames[k] << ": " << currentAvailResources[k] << endl;
                        //cout << "Needed resources " << resourceNames[k] << ": " << neededResources[j][k] << endl;
                        if (currentAvailResources[k] >= neededResources[j][k]) {
                            hasAllResources++;
                        }
                    }
                    if (hasAllResources == R) {
                        // Set thread to finished if it has all resources to complete.
                        finished[i] = true;
                        completedThreads[i] = threadName[j];
                        //Add back resources to available.
                        for (int k = 0; k < R; k++) {
                            currentAvailResources[k] += resourcesAllocated[j][k];
                        }
                        break;
                    }
                }
            }
        }
    }

    bool isSafeState;

    for (int i = 0; i < T; i++) {
        if (!finished[i]) {
            isSafeState = false;
            cout << "No safe state available." << endl;
        }
        else {
            isSafeState = true;
        }
    }

    if (isSafeState) {
        for (int i = 0; i < T; i++) {
            cout << completedThreads[i] << " ";
        }
        cout << endl;
    }
    
    return isSafeState;
}

void displayResources() {
    cout << "--Resources--" << endl;
    for (int i = 0; i < R; i++) {
        cout << "Resource Name: " << resourceNames[i] << " | Total Amount: " << maxResource[i] << endl;
    }
}

void displayThreads() {
    cout << "--Threads--" << endl;
    for (int i = 0; i < T; i++) {
        cout << "Thread Name: " << threadName[i] << endl;
        cout << "Resources requested | ";
        for (int j = 0; j < R; j++) {
            cout << resourceNames[j] << ":" << resourcesRequested[i][j] << " ";
        }
        cout << endl;
    }
}

void displayResourcesAllocated() {
    cout << "--Resources Allocated--" << endl;
    for (int i = 0; i < T; i++) {
        cout << "Thread Name: " << threadName[i] << endl;
        cout << "Resources allocated | ";
        for (int j = 0; j < R; j++) {
            cout << resourceNames[j] << ":" << resourcesAllocated[i][j] << " ";
        }
        cout << endl;
    }
}

void displayNeededResources() {
    cout << "--Resources Needed--" << endl;
    for (int i = 0; i < T; i++) {
        cout << "Thread Name: " << threadName[i] << endl;
        cout << "Resources needed | ";
        for (int j = 0; j < R; j++) {
            cout << resourceNames[j] << ":" << neededResources[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    readFile("data.txt");
    /*
    displayResources();
    cout << endl;
    displayThreads();
    cout << endl;
    displayResourcesAllocated();
    cout << endl;
    calcNeededResources();
    displayNeededResources();
    cout << endl;
    */


    bool bankersAlgorithm = isSafeState();
    cout << bankersAlgorithm << endl;

    return 0;
}