/*
  ==============================================================================

    BoidComponent.h
    Created: 21 Jan 2024 3:02:37pm
    Author:  Kyle Rupik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define NumOfBoids 500
#define BoidWidth 1
#define BoidHeight 1

class BoidComponent : public juce::Component, public Timer
{
public:
    /** BoidComponent constructor */
    BoidComponent();

    /** BoidComponent destructor */
    ~BoidComponent() override;

    /**  */
    void paint(juce::Graphics&) override;

    /**  */
    void resized() override;

    /**  */
    void timerCallback() override;

    /** Boundary rule stops boids flying off the edge of the screen */
    void boundaryRule(int leftBoundary, int rightBoundary, int topBoundary, int bottomBoundary);

    void moveToFlock();

    void personalSpace();

private:
    const int move = 1;
    const int move2 = 2;
    const int spacing = 3;
    //int velocity[NumOfBoids];
    int boidX[NumOfBoids];
    int boidY[NumOfBoids];

};