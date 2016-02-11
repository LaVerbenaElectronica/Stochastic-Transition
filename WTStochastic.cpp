//
//  WTStochastic.cpp
//  StochasticSynthesis
//
//  Created by JESUS JARA LOPEZ on 11/08/14.
//
//

#include "WTStochastic.h"
#include <cmath>
#include <random>
#define PI 3.14159265


WTStochastic::WTStochastic(float freq) : WTOscillator(freq) {
    
    
    double phase;
    double delta;
    int fourth = ((int) wtSize/4);
    int half = ((int) wtSize/2);
    float random;

    /************  WAVETABLE  *************/

    for (int i = 0; i < wtSize; i++) {
        waveTableNull[i] = 0.0f;
    }
    
     /************  waveTableSinus  *************/
    
    for (int wtPos = 0; wtPos < waveTableSize(); wtPos++) {
        phase = ((-2 * PI) / wtSize) * wtPos;
        waveTableSinus[wtPos] = sin(phase);
    }
    
    /************  waveTableCos  *************/
    
    for (int wtPos = 0; wtPos < waveTableSize(); wtPos++) {
        phase = ((2 * PI) / wtSize) * wtPos;
        waveTableCos[wtPos] = cos(phase);
    }
    
     /************  waveTableTriangle  *************/
    
    waveTableTriangle[0] = 0;
    for (int wtPos = 0; wtPos < waveTableSize(); wtPos++) {
        if (wtPos < fourth || wtPos > (3 * fourth))  delta = (double) -1/fourth;
        if (wtPos >= fourth && wtPos < (3 * fourth)) delta = (double) 1/fourth;
        waveTableTriangle[wtPos+1] = waveTableTriangle[wtPos] + delta;
    }
    
     /************  waveTableSaw  *************/

    for (int wtPos = 0; wtPos < waveTableSize(); wtPos++) {
        waveTableSaw[wtPos] = 1 - wtPos / (0.5f * (wtSize));
    }
    
     /************  waveTableSquare  *************/
    
    for (int wtPos = 0; wtPos < waveTableSize(); wtPos++) {
        if (wtPos < half)  delta = -0.98;
        if (wtPos >= half)  delta = 0.98;
        waveTableSquare[wtPos] = delta;
    }
    
     /************  waveTableStochastic  *************/
    
    std::random_device rdev{};
    std::default_random_engine e{rdev()};
    std::binomial_distribution<int> d{1000, 0.2};
    std::bernoulli_distribution positivo{0.5f};
    
    waveTableStochastic[0] = 0.0f;
    for (int i = 0; i < wtSize; i++) {
        random = (float) d(e) / 1000;
        if (positivo(e)) random = random * -1;
        random = waveTableStochastic[i] + random;
        //mirror 0.7
        if (random > 0.7) random = 0.7 - (random - 0.7);
        if (random < -0.7) random = -0.7 - (random + 0.7);
        waveTableStochastic[i+1] = random;
    }
    
    
     /************  waveTableNull  *************/
    
    for (int i = 0; i < wtSize; i++) {
        waveTableNull[i] = 0.0f;
    }

}


float WTStochastic::getWaveTableSinus(int index) {
    return waveTableSinus[index];
}

float WTStochastic::getWaveTableCos(int index) {
    return waveTableCos[index];
}

float WTStochastic::getWaveTableSaw(int index) {
    return waveTableSaw[index];
}

float WTStochastic::getWaveTableTriangle(int index) {
    return waveTableTriangle[index];
}

float WTStochastic::getWaveTableSquare(int index) {
    return waveTableSquare[index];
}

float WTStochastic::getWaveTableStochastic(int index) {
    return waveTableStochastic[index];
}

float WTStochastic::getWaveTableNull(int index) {
    return waveTableNull[index];
}





