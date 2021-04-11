#include <iomanip>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

using tanksCount_t = unsigned long long;

const tanksCount_t minTanksCount = 10;
const tanksCount_t maxTanksCount = 100000000;

class Tank {
    unsigned int _serial;   // serial number
    double _x, _y;          // coordinates
    
public:
    Tank(unsigned int serial, double x, double y) : _serial(serial), _x(x), _y(y) {};
    
    auto getSerialNumber() const {
        return _serial;
    }
    
    auto getCoordinates() const {
        return std::make_pair(_x, _y);
    }
};

void printPrediction(std::string type, tanksCount_t prediction, tanksCount_t reality) {
    auto diffValue = static_cast<long long>(reality) - static_cast<long long>(prediction);
    auto diffPercentage = diffValue / static_cast<double>( (reality + prediction) / 2.0 ) * 100;

    std::cout << type << ": " << prediction << " (" << diffPercentage << " %)" << std::endl;
}

int main(int argc, const char * argv[]) {
    std::random_device rd;
    std::mt19937_64 mt(rd());
    
    std::uniform_real_distribution<double> coordinateDist(0.0, 1.0);
    std::uniform_int_distribution<tanksCount_t> tanksCountDist(minTanksCount, maxTanksCount);
    
    // generate tanks
    std::vector<Tank> tanks;
    tanksCount_t numberOfTanks = tanksCountDist(mt);
    
    std::cout << "Generating the tanks... ";
    for(auto i = 0; i < numberOfTanks; ++i) {
        // tanks are set to a (x,y) coordinate in quadrant I
        // (with 0.0 <= x <= 1.0 and 0.0 <= y <= 1.0)
        // and have a sequentially generated serial number
        Tank tank(i + 1, coordinateDist(mt), coordinateDist(mt));
        tanks.emplace_back(tank);
    }
    std::cout << "[done]" << std::endl;
    
    // create the capture area
    const double length = 0.005;
    
    double x1 = coordinateDist(mt);
    double y1 = coordinateDist(mt);
    
    // ensure that the capture area is strictly inside the map
    for(auto& coordinate : {&x1, &y1}) {
        if (*coordinate + length >= 1.0) {
            *coordinate -= length;
        }
    }
    
    auto x2 = x1 + length;
    auto y2 = y1 + length;
    
    // capture tanks
    // (i.e., tanks present in the capture area)
    std::vector<Tank> capturedTanks;
    for(auto const& tank : tanks) {
        auto [x, y] = tank.getCoordinates();
        if (x > x1 && x < x2 && y > y1 && y < y2) {
            capturedTanks.emplace_back(tank);
        }
    }
        
    // k: number of captured tanks
    auto k = static_cast<unsigned int>( capturedTanks.size() );
    
    // m: largest sample serial number
    auto m = std::max_element(capturedTanks.begin(),
                              capturedTanks.end(),
                              [](Tank const& a, Tank const& b) { return a.getSerialNumber() < b.getSerialNumber(); })->getSerialNumber();
    
    std::cout << "\nOBSERVATION" << std::endl;
    std::cout << "Capture area: ";
    std::cout << "(" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ")" << std::endl;
    std::cout << "Number of captured tanks (k): " << k << std::endl;
    std::cout << "Largest serial number captured (m): " << m << std::endl;
    
    std::cout << "\nREALITY" << std::endl;
    std::cout << "Number of tanks: " << numberOfTanks << std::endl;
    
    std::cout << "\nPREDICTIONS" << std::endl;
    
    // Frequentist prediction
    auto f = m + ( m / k ) - 1;
    
    printPrediction("Frequentist", f, numberOfTanks);
    
    // Bayesian median
    auto bmedian = m + ( m * log(2) / (k - 1) );
    
    printPrediction("Bayesian median", bmedian, numberOfTanks);

    // Bayesian mean
    auto bmean = (m - 1) * ( (k - 1) / (k - 2) );
    
    printPrediction("Bayesian mean", bmean, numberOfTanks);

    return 0;
}
