
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "Please input numbers to find average." << endl;
        return 0;
    }

    double sum = 0;

    for (int i = 1; i < argc; ++i) {
        sum += stod(argv[i]);
    }

    cout << "---------------------------------" << endl;
    cout << "Average of " << argc - 1 << " numbers = " << sum / (argc - 1) << endl;
    cout << "---------------------------------" << endl;

    return 0;
}