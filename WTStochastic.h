//
//  WTStochastic.h
//  StochasticSynthesis
//
//  Created by JESUS JARA LOPEZ on 11/08/14.
//
//

#ifndef StochasticSynthesis_WTStochastic_h
#define StochasticSynthesis_WTStochastic_h

#include <iostream>
#include "WTOscillator.h"

class WTStochastic : public WTOscillator {
    
protected:
    float waveTableNull[wtSize];
    float waveTableSinus[wtSize];
    float waveTableCos[wtSize];
    float waveTableTriangle[wtSize];
    float waveTableSaw[wtSize];
    float waveTableSquare[wtSize];
    float waveTableStochastic[wtSize];
    
public:
    WTStochastic(float freq);
    float getWaveTableSinus(int index);
    float getWaveTableCos(int index);
    float getWaveTableSaw(int index);
    float getWaveTableTriangle(int index);
    float getWaveTableSquare(int index);
    float getWaveTableStochastic(int index);
    float getWaveTableNull(int index);
};



#endif
