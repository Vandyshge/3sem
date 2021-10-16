#include <iostream>
#include <iomanip>
#include <fstream>

int main () {
    std::ifstream inp;
    std::ofstream out;
    inp.open("data.txt");
    out.open("data.csv");

    if (out.is_open() && inp.is_open()) {
        int d = 13;
        int m = 3;
        int y = 2003;
        out << std::setw(2) << std::setfill('0') << d << "." << std::setw(2) << std::setfill('0') << m << "." << y << '\n';
        
        int n1, n2;
        float tmp;
        inp >> n1 >> n2;
        for (auto i = 0; i < n1; i++) {
            for (auto j = 0; j < n2-1; j++) {
                inp >> tmp;
                out << tmp << ",";
            }
            inp >> tmp;
            out << tmp << "\n";
        }
    }

    inp.close();
    out.close();

return 0;}