//    Nam Ton
//    CECS 325-01
//    Prog 7 - 3n + 1
//    04/11/2023
//
//    I certify that this program is my own original work. I did not copy any part of this program 
//    from any other source. I further certify that I typed each and every line of code in this program.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>

using namespace std;

class BigInt {
private:
    vector<char> v; 
    bool is_negative = false;

public:
    // Constructors
    BigInt();
    BigInt(int);
    BigInt(string);

    // Arithmetic operations
    BigInt operator+(const BigInt&);
    BigInt operator++();
    BigInt operator++(int);
    BigInt operator*(const BigInt&);
    BigInt half();

    // Logical operations
    bool isOdd() const;
    bool isEven() const;
    bool operator==(const BigInt&) const;
    bool operator<(const BigInt&) const;

    // Output stream
    friend ostream& operator<<(ostream&, const BigInt&);
};

// Constructors
BigInt::BigInt() {
    v.push_back(0);
}

BigInt::BigInt(int n) {
    if (n < 0) {
        is_negative = true;
        n = -n;
    }
    while (n) {
        v.push_back(n % 10);
        n /= 10;
    }
    if (v.empty()) {
        v.push_back(0);
    }
}

BigInt::BigInt(string s) {
    if (s[0] == '-') {
        is_negative = true;
        s = s.substr(1);
    }
    
    // ePos: position of the 'e' character in the input string (if exists)
    size_t ePos = s.find('e');
    int exp = 0;
    if (ePos != string::npos) {
        exp = stoi(s.substr(ePos + 1));
        s = s.substr(0, ePos);
    }

    // dotPos: position of the '.' character in the input string (if exists)
    size_t dotPos = s.find('.');
    if (dotPos != string::npos) {
        s.erase(dotPos, 1);
        exp -= s.size() - dotPos;
    }

    for (int i = s.size() - 1; i >= 0; i--) {
        v.push_back(s[i] - '0');
    }
    
    while (exp > 0) {
        *this = *this * 10;
        exp--;
    }

    while (exp < 0) {
        *this = this->half();
        exp++;
    }

    if (v.empty()) {
        v.push_back(0);
    }
}



BigInt BigInt::operator+(const BigInt &b) {
    if (is_negative != b.is_negative) {
        // If the signs are different, the addition is not supported.
        throw std::invalid_argument("Addition with different signs is not supported.");
    }

    BigInt result;
    result.v.clear();

    char carry = 0;
    for (size_t i = 0; i < max(v.size(), b.v.size()) || carry; ++i) {
        char sum = carry;
        if (i < v.size()) {
            sum += v[i];
        }
        if (i < b.v.size()) {
            sum += b.v[i];
        }

        carry = sum >= 10;
        if (carry) {
            sum -= 10;
        }
        result.v.push_back(sum);
    }

    return result;
}

BigInt BigInt::operator++() {
    *this = *this + BigInt(1);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    *this = *this + BigInt(1);
    return temp;
}

BigInt BigInt::operator*(const BigInt &b) {
    BigInt result;
    result.v.resize(v.size() + b.v.size());

    for (size_t i = 0; i < v.size(); ++i) {
        char carry = 0;
        for (size_t j = 0; j < b.v.size() || carry; ++j) {
            char prod = result.v[i + j] + carry;
            if (j < b.v.size()) {
                prod += v[i] * b.v[j];
            }

            carry = prod / 10;
            result.v[i + j] = prod % 10;
        }
    }

    while (!result.v.empty() && result.v.back() == 0) {
        result.v.pop_back();
    }

    return result;
}

BigInt BigInt::half() {
    BigInt result;
    result.v.clear();

    char carry = 0;
    for (int i = v.size() - 1; i >= 0; --i) {
        char cur = carry * 10 + v[i];
        carry = cur % 2;
        result.v.push_back(cur / 2);
    }
    reverse(result.v.begin(), result.v.end());
    while (!result.v.empty() && result.v.back() == 0) {
        result.v.pop_back();
    }

    return result;
}

bool BigInt::isOdd() const {
    return v[0] % 2 == 1;
}

bool BigInt::isEven() const {
    return v[0] % 2 == 0;
}

bool BigInt::operator==(const BigInt &b) const {
    return is_negative == b.is_negative && v == b.v;
}

bool BigInt::operator<(const BigInt &b) const {
    if (is_negative != b.is_negative) {
        return is_negative;
    }

    if (v.size() != b.v.size()) {
        return (is_negative) ? (v.size() > b.v.size()) : (v.size() < b.v.size());
    }

    for (int i = v.size() - 1; i >= 0; --i) {
        if (v[i] != b.v[i]) {
            return (is_negative) ? (v[i] > b.v[i]) : (v[i] < b.v[i]);
        }
    }

    return false;
}

ostream& operator<<(ostream &os, const BigInt &b) {
    if (b.is_negative) {
        os << '-';
    }
    int scientific_notation_threshold = 8;  
    if (b.v.size() > scientific_notation_threshold) {
        os << static_cast<char>('0' + b.v[b.v.size() - 1]);
        os << '.';
        for (int i = b.v.size() - 2; i >= b.v.size() - 8; --i) {  
            os << static_cast<char>('0' + b.v[i]);
        }
        os << "e" << (b.v.size() - 1);
    } else {
        for (int i = b.v.size() - 1; i >= 0; --i) {
            os << static_cast<char>('0' + b.v[i]);
        }
    }

    return os;
}



struct ThreeNp1 {
    BigInt start;
    BigInt steps;
    BigInt max;
    BigInt odd;
    BigInt even;
};

void print(ThreeNp1 temp) {
    cout << "start:" << temp.start << endl;
    cout << "steps:" << temp.steps << endl;
    cout << "max:" << temp.max << endl;
    cout << "odds:" << temp.odd << endl;
    cout << "evens:" << temp.even << endl;
}

void findThreeNp1(BigInt n, ThreeNp1 & Np1, bool printSteps = false) {
    if (printSteps) {
        cout << "->" << '(' << n << ')';
    }
    if (Np1.max < n) {
        Np1.max = n;
    }
    if (n == BigInt(1)) {
        return;
    } else if (n.isEven()) {
        Np1.even++;
        Np1.steps++;
        findThreeNp1(n.half(), Np1, printSteps);
    } else if (n.isOdd()) {
        Np1.odd++;
        Np1.steps++;
        BigInt tempN(n);
        findThreeNp1(tempN * BigInt(3) + BigInt(1), Np1, printSteps);
    } else {
        cout << "How the hell did I get here?\n";
        return;
    }
}

int main() {
    BigInt MAX(INT_MAX);
    cout << "The largest integer is: " << MAX << endl;
    cout << "Twice the largest integer is: " << MAX + MAX << endl;
    
    
    while (true) {
        string input;
        cout << "Enter a starting number or MAX: ";
        cin >> input;
        if (input == "q" || input == "quit") {
            break;
        }
        BigInt start(input);

        if (input == "MAX" || input == "max") {
            //consider if the user wants to start as int max
            start = MAX;
        }

        bool printSteps = false;
        ThreeNp1 N = {start, 0, 0, 0, 0};
        findThreeNp1(start, N, printSteps);
        cout << endl;
        print(N);
    }

    return 0;
}
