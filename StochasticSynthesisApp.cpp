#include "cinder/app/AppNative.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Callback.h"
#include "cinder/Timeline.h"
#include "cinder/CinderMath.h"
#include "WTStochastic.h"
#include "cinder/gl/gl.h"
#include <vector>
#include <random>

using namespace ci;
using namespace ci::app;
using namespace std;

class StochasticSynthesisApp : public AppNative {

    WTStochastic *mySignal;
    vector<float> waveBuffer;
    enum signal_choosed {SINUS, COSINUS, TRIANGLE, SAW, SQUARE, STOCHASTIC, ZERO};
    signal_choosed signal = ZERO;
    
    float error = 0.04;
    int changes = 200;
    
    random_device rd{};
    random_device sd{};
    default_random_engine gen{rd()};
    default_random_engine gen2{sd()};
    uniform_int_distribution<int> amplitude_real{0, 2000};
    bernoulli_distribution positivo{0.7f};
    
public:
    
	void setup();
    void stochasticWave(uint64_t inSampleOffset, uint32_t ioSampleCount, audio::Buffer32f *ioBuffer);
    void keyDown( KeyEvent event );
	void update();
	void draw();
};

void StochasticSynthesisApp::setup()
{
    mySignal = new WTStochastic(220.0f);
    audio::Output::play( audio::createCallback( this, &StochasticSynthesisApp::stochasticWave ) );
}


void StochasticSynthesisApp::update()
{
    int pos;
    float value, target;
    
    for (int x=0; x<changes; x++){
    
        pos = (int) amplitude_real(gen);
        value = mySignal->getWaveTable(pos);
        
        //Here we choose the final target according to the users chose.
        switch (signal) {
            case SINUS:
                target = mySignal->getWaveTableSinus(pos);
                break;
            case COSINUS:
                target = mySignal->getWaveTableCos(pos);
                break;
            case TRIANGLE:
                target = mySignal->getWaveTableTriangle(pos);
                break;
            case SAW:
                target = mySignal->getWaveTableSaw(pos);
                break;
            case SQUARE:
                target = mySignal->getWaveTableSquare(pos);
                break;
            case STOCHASTIC:
                target = mySignal->getWaveTableStochastic(pos);
                break;
            case ZERO:
                target = mySignal->getWaveTableNull(pos);
                break;
            default:
                target = mySignal->getWaveTableNull(pos);
                break;
        }
        
        //Here we change stochastically the original signal to the target
        if (value < (target - error)) {
            if (positivo(gen2)) {
                value += error;
            } else {
                value -= error;
            }
        } else if (value > (target + error)) {
            if (positivo(gen2)) {
                value -= error;
            } else {
                value += error;
            }
        } else {
            value = target;
        }
        
        mySignal->setWaveTable(pos, value);
        
    }
}

void StochasticSynthesisApp::keyDown( KeyEvent event )
{
    if( event.getCode() == app::KeyEvent::KEY_a ) {
		signal = SINUS;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_s ) {
		signal = COSINUS;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_d ) {
		signal = TRIANGLE;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_f ) {
        signal = SAW;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_g ) {
		signal = SQUARE;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_h ) {
		signal = STOCHASTIC;
	}
    
    if( event.getCode() == app::KeyEvent::KEY_j ) {
		signal = ZERO;
	}
}

void StochasticSynthesisApp::stochasticWave( uint64_t inSampleOffset, uint32_t ioSampleCount, audio::Buffer32f *ioBuffer ) {
    
    if (waveBuffer.size() < ioSampleCount)           //waveBuffer tiene el valor de la tabla de onda
        waveBuffer.assign(ioSampleCount, 0.0f);
    
	for( int  i = 0; i < ioSampleCount; i++ ) {
        
		waveBuffer[i] = mySignal->tick();            //se rellena con los valores de las amplitudes
		//se guarda en un buffer el doble de grande en estereo
		ioBuffer->mData[i*ioBuffer->mNumberChannels] = waveBuffer[i];
		ioBuffer->mData[i*ioBuffer->mNumberChannels + 1] = waveBuffer[i];
	}
}


void StochasticSynthesisApp::draw()
{
    gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
    gl::color( Color( 1.0f, 1.0f, 1.0f ) );
    
    const float waveHeight = 200.0f;
    float halfWindowWidth = getWindowWidth() / 2;
    
    glBegin( GL_LINE_STRIP );
	
    for( float x = -halfWindowWidth; x < halfWindowWidth; x += 0.5f ) {
        float windowPhase = (x + halfWindowWidth) / getWindowWidth();
        //float sample = waveBuffer[(int)(windowPhase * waveBuffer.size())];
        float sample = mySignal->getWaveTable((int)(windowPhase * 2000));
        //cout << sample << endl;
        
        gl::vertex( getWindowCenter() + Vec2f( x, waveHeight * sample ) );
    }
    
    glEnd();
}

CINDER_APP_NATIVE( StochasticSynthesisApp, RendererGl )
