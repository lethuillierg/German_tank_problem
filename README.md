# German Tank Problem Simulator

Simple [German tank problem](https://en.wikipedia.org/wiki/German_tank_problem) simulator.

A random number of tanks are generated (between 10 and 100,000,000). 

A sequentially generated serial number is affected to each of them. Furthermore, each tank is set to a random point in Quadrant I (0.0 ≤ x ≤ 1.0; 0.0 ≤ y ≤ 1.0).

Then, a capture area (a square with length .005 randomly located in Quadrant I) is created. Tanks present in this area are captured.

Subsequently, frequentist and Bayesian median and mean predictions are computed.

## Compile

```
$ g++ -std=c++17 -o german_tank_problem main.cpp
```

## Run

```
$ ./german_tank_problem
Generating the tanks... [done]

OBSERVATION
Capture area: (0.01321,0.253244), (0.01821,0.258244)
Number of captured tanks (k): 1038
Largest serial number captured (m): 42111118

REALITY
Number of tanks: 42152094

PREDICTIONS
Frequentist: 42151686 (0.000967928 %)
Bayesian median: 42139265 (0.0304397 %)
Bayesian mean: 42111117 (0.0972595 %)
```