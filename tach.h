
/* Single Channel Encoder as Speedometer
   John Donnal 2018
*/

#ifndef TACH_H
#define TACH_H

/**
 * Includes
 */
#include "mbed.h"

/**
 * Defines
 */
#define NSAMP 5

/**
 * Tach Interface.
 */
class Tach {

public:

    /**
     * Constructor.
     *
     * Tracks rising and falling edges with interrupts, provides
     * rotations per second (Hz) output
     *
     * @param input mbed pin for input channel.
     * @param pulsesPerRev Number of pulses in one revolution.
     * @param encoding The encoding to use. Uses X2 encoding by default. X2
     */
    Tach(PinName input, int pulsesPerRev);

    /**
     * Read the state of the encoder.
     *
     * @return The current state of the encoder as a 2-bit number, where:
     *         bit 1 = The reading from channel B
     *         bit 2 = The reading from channel A
     */
    float getSpeed(void);
    int getCount(void);
   
private:

    /**
     * Update the pulse count.
     *
     * Called on every rising/falling edge of input
     *
     * update current speed estimate
     */
    void update(void);

    /**
     * Called on every rising edge of channel index to update revolution
     * count by one.
     */
    void index(void);

    InterruptIn input_;
    Timer timer_;
    
    int          pulsesPerRev_;
    float        speed_;
    int          count_;
    int          dt_;
    float        speedBuffer_[NSAMP];
};

#endif /* TACH_H */
