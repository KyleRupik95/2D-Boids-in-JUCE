/*
  ==============================================================================

    MovementTest.cpp
    Created: 21 Jan 2024 3:02:37pm
    Author:  Kyle Rupik

  ==============================================================================
*/

#include "MovementTest.h"

MovementTest::MovementTest()
{
    startTimer(50);
    for (int i = 0; i < NumOfBoids; ++i)
    {
        boidX[i] = rand() % 695 + 5;
        boidY[i] = rand() % 695 + 5;
        boidMoveAngle[NumOfBoids] = rand() % 360;
    }
}

MovementTest::~MovementTest()
{

}

void MovementTest::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    //background
    g.setColour(Colours::black);
    g.fillRect(bounds);

    for (int index = 0; index < NumOfBoids - 1; ++index)
    {
        g.setColour(Colours::white);
        g.fillRect(boidX[index], boidY[index], BoidWidth, BoidHeight);
    }
    g.setColour(Colours::indianred);
    g.fillRect(boidX[NumOfBoids - 1], boidY[NumOfBoids - 1], BoidWidth, BoidHeight);
}


void MovementTest::resized()
{

}


void MovementTest::timerCallback()
{
    const auto bounds = getLocalBounds();
    const auto leftBoundary = 5;
    const auto rightBoundary = bounds.getWidth() - 5;
    const auto topBoundary = 5;
    const auto bottomBoundary = bounds.getHeight() - 5;

    //boundary rule stops boids flying off the screen
    boundaryRule(leftBoundary, rightBoundary, topBoundary, bottomBoundary);

    testMovement();
    //rule for moving all boids towards the average center point of all boids x and y coordinates
   // moveToFlock();

    //rule for stopping boids from flying into each other
   // personalSpace();

    repaint();
}

void MovementTest::boundaryRule(int leftBoundary, int rightBoundary, int topBoundary, int bottomBoundary)
{
    for (int index = 0; index < NumOfBoids; ++index)
    {
        if (boidX[index] == rightBoundary)
            boidX[index] = boidX[index] - 5;
        if (boidX[index] == leftBoundary)
            boidX[index] = boidX[index] + 5;
        if (boidY[index] == bottomBoundary)
            boidY[index] = boidY[index] - 5;
        if (boidY[index] == topBoundary)
            boidY[index] = boidY[index] + 5;
    }
}

void MovementTest::testMovement()
{
    for (int index = 0; index < NumOfBoids; ++index)
    {
            boidX[index] = boidX[index] + 3;
            boidY[index] = boidY[index] + 1;
    }
}

void MovementTest::moveToFlock()
{
    int averageX = 0;
    int averageY = 0;
    int totalX = 0;
    int totalY = 0;
    for (int index = 0; index < NumOfBoids; ++index)
    {
        totalX += boidX[index];
        totalY += boidY[index];
    }
    averageX = totalX / NumOfBoids;
    averageY = totalY / NumOfBoids;

    // simplified explanation.... 
    // if X position of boid[i] is bigger than the average X of all boids  
    // -move to get it closer to X
    // repeated for the 4 X,Y directions. up, down, left, right.
    for (int index = 0; index < NumOfBoids; ++index)
    {
        if (boidX[index] > averageX)
            boidX[index] -= move;
        if (boidX[index] < averageX)
            boidX[index] += move;
        if (boidY[index] > averageY)
            boidY[index] -= move;
        if (boidY[index] < averageY)
            boidY[index] += move;
    }
}

void MovementTest::personalSpace()
{
    int distanceX[NumOfBoids];
    int distanceY[NumOfBoids];

    for (int firstIndex = 0; firstIndex < NumOfBoids; ++firstIndex)
    {
        for (int secondIndex = 0; secondIndex < NumOfBoids; ++secondIndex)
        {
            if (secondIndex < NumOfBoids - 1)
                distanceX[secondIndex] = boidX[firstIndex] - boidX[secondIndex + 1];
        }
        for (int thirdIndex = 0; thirdIndex < NumOfBoids; ++thirdIndex)
        {
            if (distanceX[thirdIndex] < spacing && distanceX[thirdIndex] > 0)
            {
                boidX[firstIndex] += move2;
            }
            if (distanceX[thirdIndex] > -spacing && distanceX[thirdIndex] < 0)
            {
                boidX[firstIndex] -= move2;
            }
        }
        for (int secondIndex = 0; secondIndex < NumOfBoids; ++secondIndex)
        {
            if (secondIndex < NumOfBoids - 1)
                distanceY[secondIndex] = boidY[firstIndex] - boidY[secondIndex + 1];
        }
        for (int thirdIndex = 0; thirdIndex < NumOfBoids; ++thirdIndex)
        {
            if (distanceY[thirdIndex] < spacing && distanceY[thirdIndex] > 0)
            {
                boidY[firstIndex] += move2;
            }
            if (distanceY[thirdIndex] > -spacing && distanceY[thirdIndex] < 0)
            {
                boidY[firstIndex] -= move2;
            }
        }
    }
}

void MovementTest::moveToRelativeCentre()
{
    for (int index = 0; index < NumOfBoids; ++index)
    {
        if (boidMoveAngle[index]);
    }
}

void MovementTest::convertAngle()
{

}

/*
movement list

0 = UP 3 = Y - 3

  UP
*/