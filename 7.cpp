#include <iostream>
using namespace std;

int main()
{
    int r = 4; // Number of resources
    int p = 5; // Number of processes

    int existing_resources[r] = {6, 3, 4, 2};
    int allocation[p][r] = {{3, 0, 1, 1},
                            {0, 1, 0, 0},
                            {1, 1, 1, 0},
                            {1, 1, 0, 1},
                            {0, 0, 0, 0}};
    int max_need[p][r] = {{4, 1, 1, 1},
                          {0, 2, 1, 2},
                          {4, 1, 1, 0},
                          {1, 1, 1, 1},
                          {2, 1, 1, 0}};

    // Calculate the remaining need matrix
    int remaining_need[p][r];
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            remaining_need[i][j] = max_need[i][j] - allocation[i][j];
        }
    }

    // Calculate the currently available resources
    int current_avail[r];
    for (int j = 0; j < r; j++)
    {
        int colSum = 0;
        for (int i = 0; i < p; i++)
        {
            colSum += allocation[i][j];
        }
        current_avail[j] = existing_resources[j] - colSum;
    }

    // Displaying the initial matrices
    cout << "Initial Allocation Matrix:\n";
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            cout << allocation[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nExisting Resources:\n";
    for (int j = 0; j < r; j++)
    {
        cout << existing_resources[j] << " ";
    }
    cout << endl;

    cout << "\nRemaining Need Matrix:\n";
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            cout << remaining_need[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Initialize finish array to keep track of completed processes
    bool finish[p] = {false};
    int safe[p]; // Safe sequence storage
    int h = 0;   // Safe sequence index

    // Main loop to find a safe sequence
    int count = 0;
    while (count < p)
    {
        bool found = false;
        for (int i = 0; i < p; i++)
        {
            if (!finish[i])
            { // Process i is not finished
                bool canAllocate = true;
                for (int j = 0; j < r; j++)
                {
                    if (remaining_need[i][j] > current_avail[j])
                    {
                        canAllocate = false;
                        break;
                    }
                }

                // If the process can be allocated safely
                if (canAllocate)
                {
                    // Add the process to the safe sequence
                    safe[h++] = i;
                    finish[i] = true; // Mark process as finished
                    count++;          // Increase count of finished processes
                    found = true;

                    // Update available resources by adding allocated resources
                    for (int j = 0; j < r; j++)
                    {
                        current_avail[j] += allocation[i][j];
                    }
                }
            }
        }

        // If no process was found in this iteration, the system is not in a safe state
        if (!found)
        {
            cout << "System is not in a safe state.\n";
            return 0;
        }
    }

    // If we reach here, then the system is in a safe state
    cout << "System is in a safe state.\nSafe sequence: ";
    for (int i = 0; i < p; i++)
    {
        cout << "P" << safe[i];
        if (i < p - 1)
            cout << " -> ";
    }
    cout << endl;

    return 0;
}
