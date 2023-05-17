//    Nam Ton
//    CECS 325-01
//    Prog 6 - 3n + 1
//    04/11/2023
//
//    I certify that this program is my own original work. I did not copy any part of this program 
//    from any other source. I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

// Recursive function that goes through the collatz sequence and returns the number of steps
int collatz(int n, int &odds, int &evens, vector<int> &steps) {
    steps.push_back(n);
    if (n == 1) return n;
    if (n % 2 == 0) {
        evens++;
        return collatz(n / 2, odds, evens, steps);
    } else {
        odds++;
        if (n > (INT_MAX - 1) / 3) {
            throw overflow_error("overflow detected");
        }
        return collatz(3 * n + 1, odds, evens, steps);
    }
}

// Goes through the collatz sequence and prints out the results 
// Checks overflow and if overflow is detected(it will not affect the next run)
void process(int n, bool &overflowcheck) {
    if (overflowcheck) {
        cout << "// previous error does not affect next run" << endl;
        overflowcheck = false;
    }
    cout << "Solving 3n+1 - starting value:" << n << endl;

    int odds = 0, evens = 0;
    int max_val = n;
    vector<int> steps;

    try {
        collatz(n, odds, evens, steps);

        for (int step : steps) {
            cout << "->(" << step << ")";
            if (step > max_val) max_val = step;
        }
        cout << endl;

        cout << " start:" << n << endl;
        cout << " steps:" << (odds + evens) << endl;
        cout << " max:" << max_val << endl;
        cout << " odds:" << odds << endl;
        cout << " evens:" << evens << endl;
    } catch (overflow_error &e) {
        for (size_t i = 0; i < steps.size() - 1; i++) {
            cout << "->(" << steps[i] << ")";
        }
        cout << "->(###overflow###)" << endl;

        cout << "overflow detected for n:" << steps.back() << endl;
        cout << "3n + 1:" << (3 * steps.back() + 1) << endl;
        cout << "overflow" << endl;
        overflowcheck = true;
    }
}

// Main function that takes in the command line arguments and runs the program 
// (If use negative number program exits - makes it easier to test)
int main(int argc, char *argv[]) {
    bool overflowcheck = false;
    
    if (argc == 1) {
        while (true) {
            int n;
            cout << "Enter a 3n+1 candidate number: ";
            cin >> n;

            if (n < 0) {
                break;
            }

            process(n, overflowcheck);
        }
    } else {
        for (int i = 1; i < argc; i++) {
            int n = stoi(argv[i]);
            process(n, overflowcheck);
        }
    }
    return 0;
}