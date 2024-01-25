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
#define BoidWidth 3
#define BoidHeight 3

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

    /** function for testing different movements ie + 3 in X direction, + 1 in y direction */
    void testMovement();

    /** Boundary rule stops boids flying off the edge of the screen */
    void boundaryRule(int leftBoundary, int rightBoundary, int topBoundary, int bottomBoundary);

    void moveToFlock();

    void personalSpace();

    void convertSpeedAndAngleToMovement();
    void averageDirectionOfNeighbours();
    void turnToRelativeCentre();

    void turnAwayFromBoundary();

    int getNextXCoord(int boidX,float direction, float speed);
    int getNextYCoord(int boidY, float direction, float speed);

private:
    const int move = 1;
    const int move2 = 2;
    const int spacing = 3;
    const int neighbourSearchDistance = 500;
    const float pi = 3.14159265358979323846;
    int boidX[NumOfBoids];
    int boidY[NumOfBoids];
    int boidSpeed[NumOfBoids];
    float boidMoveDirection[NumOfBoids];

    double bruh = 0.f;

};