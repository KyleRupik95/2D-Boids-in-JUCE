/*
  ==============================================================================

    BoidComponent.cpp
    Created: 21 Jan 2024 3:02:37pm
    Author:  Kyle Rupik

  ==============================================================================
*/

#include "BoidComponent.h"

BoidComponent::BoidComponent()
{
    for (int i = 0; i < NumOfBoids; ++i)
    {
        boidX[i] = rand() % 695 + 5;
        boidY[i] = rand() % 695 + 5;
        boidSpeed[i] = 1;
        boidMoveAngle[i] = rand() % 16;
    }
    startTimer(50);
}

BoidComponent::~BoidComponent()
{

}

void BoidComponent::paint(juce::Graphics& g)
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
    g.fillRect(boidX[NumOfBoids - 1] - 1, boidY[NumOfBoids - 1] - 1, BoidWidth, BoidHeight);
}


void BoidComponent::resized()
{

}


void BoidComponent::timerCallback()
{
    const auto bounds = getLocalBounds();
    const auto leftBoundary = 5;
    const auto rightBoundary = bounds.getWidth() - 5;
    const auto topBoundary = 5;
    const auto bottomBoundary = bounds.getHeight() - 5;

    //testMovement();

    AverageDirectionOfNeighbours();
    convertSpeedAndAngleToMovement();
    //boundary rule stops boids flying off the screen
    boundaryRule(leftBoundary, rightBoundary, topBoundary, bottomBoundary);

    //rule for moving all boids towards the average center point of all boids x and y coordinates
   // moveToFlock();

    //rule for stopping boids from flying into each other
  //  personalSpace();

    repaint();
}

void BoidComponent::testMovement()
{
    for (int index = 0; index < NumOfBoids; ++index)
    {
        boidX[index] = boidX[index] + 3;
        boidY[index] = boidY[index] + 1;
    }
}


void BoidComponent::boundaryRule(int leftBoundary, int rightBoundary, int topBoundary, int bottomBoundary)
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

void BoidComponent::moveToFlock()
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

void BoidComponent::personalSpace()
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

void BoidComponent::convertSpeedAndAngleToMovement()
{
    for (int index = 0; index < NumOfBoids; ++index)
    {
        if (boidMoveAngle[index] >= 0.0  && boidMoveAngle[index] < 0.5)  // 0
        {
            boidY[index] -= 3 * boidSpeed[index];
            // boidX[index] -= 0;
        }
        if (boidMoveAngle[index] >= 0.5 && boidMoveAngle[index] < 1.5) // 1
        {
            boidY[index] -= 3 * boidSpeed[index];
            boidX[index] += 1 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 1.5 && boidMoveAngle[index] < 2.5) // 2
        {
            boidY[index] -= 2 * boidSpeed[index];
            boidX[index] += 2 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 2.5 && boidMoveAngle[index] < 3.5) // 3
        {
            boidY[index] -= 1 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 3.5 && boidMoveAngle[index] < 4.5) // 4
        {
            //boidY[index] += 0 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 4.5 && boidMoveAngle[index] < 5.5) // 5
        {
            boidY[index] += 1 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 5.5 && boidMoveAngle[index] < 6.5) // 6
        {
            boidY[index] += 2 * boidSpeed[index];
            boidX[index] += 2 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 6.5 && boidMoveAngle[index] < 7.5) // 7
        {
            boidY[index] += 3 * boidSpeed[index];
            boidX[index] += 1 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 7.5 && boidMoveAngle[index] < 8.5) // 8
        {
            boidY[index] += 3 * boidSpeed[index];
            // boidX[index] -= 0;
        }
        if (boidMoveAngle[index] >= 8.5 && boidMoveAngle[index] < 9.5) //9
        {
            boidY[index] += 3 * boidSpeed[index];
            boidX[index] -= 1 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 9.5 && boidMoveAngle[index] < 10.5) //10
        {
            boidY[index] += 2 * boidSpeed[index];
            boidX[index] -= 2 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 10.5 && boidMoveAngle[index] < 11.5) //11
        {
            boidY[index] += 1 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 11.5 && boidMoveAngle[index] < 12.5) //12
        {
            //boidY[index] -= 0 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 12.5 && boidMoveAngle[index] < 13.5) //13
        {
            boidY[index] -= 1 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 13.5 && boidMoveAngle[index] < 14.5) //14
        {
            boidY[index] -= 2 * boidSpeed[index];
            boidX[index] -= 2 * boidSpeed[index];
        }
        if (boidMoveAngle[index] >= 14.5 && boidMoveAngle[index] < 15.5) //15
        {
            boidY[index] -= 3 * boidSpeed[index];
            boidX[index] -= 1 * boidSpeed[index];
        }
    }
}

void BoidComponent::AverageDirectionOfNeighbours()
{
    //int distanceX[NumOfBoids];
    //int distanceY[NumOfBoids];
    std::vector<float> neighbouringBoid;
    float totalAngleOfNeighbours = 0;
    float AverageAngleOfNeighbours = 0;
    bool pushBack = false;
    float numOfNeighbours = 0;
    int distanceX = 0;
    int distanceY = 0;

    for (int index1 = 0; index1 < NumOfBoids; ++index1)
    {
        for (int index2 = 0; index2 < NumOfBoids; ++index2)
        {
            if (index1 != index2)
            {
                distanceX = boidX[index1] - boidX[index2];
                distanceY = boidX[index1] - boidX[index2];
            }
            if (distanceX <= neighbourSearchDistance && distanceX > 0) //scan right
                pushBack = true;
            if (distanceX >= -neighbourSearchDistance && distanceX < 0) //scan left
                pushBack = true;
            if (distanceY <= neighbourSearchDistance && distanceY > 0) //scan down
                pushBack = true;
            if (distanceY >= -neighbourSearchDistance && distanceY < 0) // scan up
                pushBack = true;
            if (pushBack == true)
            {
                totalAngleOfNeighbours += boidMoveAngle[index2];
                numOfNeighbours += 1;
                pushBack = false;
            }
        }
        AverageAngleOfNeighbours = totalAngleOfNeighbours / numOfNeighbours;
        boidMoveAngle[index1] = AverageAngleOfNeighbours;
        totalAngleOfNeighbours = 0;
        numOfNeighbours = 0;
        pushBack = false;
    }
}


/*
movement list

0 = UP 3 = Y - 3

  UP


      for (int firstIndex = 0; firstIndex < NumOfBoids; ++firstIndex)
    {
        for (int secondIndex = 0; secondIndex < NumOfBoids; ++secondIndex)
        {
            std::vector<float> neighbouringBoid;
            neighbouringBoid.push_back(1.f);
            neighbouringBoid.clear();
        }
    }




        int distanceX[NumOfBoids];
    int distanceY[NumOfBoids];
    std::vector<float> neighbouringBoid;
    float totalAngleOfNeighbours = 0;
    float AverageAngleOfNeighbours = 0;
    bool pushBack = false;

    for (int Index1 = 0; Index1 < NumOfBoids; ++Index1)
    {
        neighbouringBoid.clear();
        for (int Index2 = 0; Index2 < NumOfBoids; ++Index2)
        {
            if (Index2 < NumOfBoids - 1)
            {
                distanceX[Index2] = boidX[Index1] - boidX[Index2 + 1];
                distanceY[Index2] = boidY[Index1] - boidY[Index2 + 1];
            }
        }
        for (int Index3 = 0; Index3 < NumOfBoids; ++Index3)
        {
            if (distanceX[Index3] <= neighbourSearchDistance  && distanceX[Index3] > 0) //scan right
                pushBack = true;
            if (distanceX[Index3] >= -neighbourSearchDistance && distanceX[Index3] < 0) //scan left
                pushBack = true;
            if (distanceY[Index3] <= neighbourSearchDistance  && distanceY[Index3] > 0) //scan down
                pushBack = true;
            if (distanceY[Index3] >= -neighbourSearchDistance && distanceY[Index3] < 0) // scan up
                pushBack = true;
            if (pushBack == true)
            {
                neighbouringBoid.push_back(boidMoveAngle[Index3]);
                pushBack = false;
            }
            pushBack = false;
        }
        for (int Index4 = 0; Index4 < neighbouringBoid.size(); ++Index4)
        {
            totalAngleOfNeighbours += neighbouringBoid[Index4];
        }
        AverageAngleOfNeighbours = totalAngleOfNeighbours / neighbouringBoid.size();
        boidMoveAngle[Index1] = AverageAngleOfNeighbours;
    }



        int distanceX[NumOfBoids];
    int distanceY[NumOfBoids];
    std::vector<float> neighbouringBoid;
    float totalAngleOfNeighbours = 0;
    float AverageAngleOfNeighbours = 0;
    bool pushBack = false;

    for (int firstIndex = 0; firstIndex < NumOfBoids; ++firstIndex)
    {
        neighbouringBoid.clear();
        for (int secondIndex = 0; secondIndex < NumOfBoids; ++secondIndex)
        {
            if (secondIndex < NumOfBoids - 1)
            {
                distanceX[secondIndex] = boidX[firstIndex] - boidX[secondIndex + 1];
                distanceY[secondIndex] = boidY[firstIndex] - boidY[secondIndex + 1];
                if (distanceX[secondIndex] <=  neighbourSearchDistance && distanceX[secondIndex] > 0)
                    pushBack = true;
                if (distanceX[secondIndex] >= -neighbourSearchDistance && distanceX[secondIndex] < 0)
                    pushBack = true;
                if (distanceY[secondIndex] <=  neighbourSearchDistance && distanceY[secondIndex] > 0)
                    pushBack = true;
                if (distanceY[secondIndex] >= -neighbourSearchDistance && distanceY[secondIndex] < 0)
                    pushBack = true;
                if (pushBack == true)
                {
                    neighbouringBoid.push_back(boidMoveAngle[secondIndex]);
                    pushBack = false;
                }
            }
        }
        for (int thirdIndex = 0; thirdIndex < neighbouringBoid.size(); ++thirdIndex)
        {
            DBG(neighbouringBoid[thirdIndex]);
            totalAngleOfNeighbours += neighbouringBoid[thirdIndex];
        }
        AverageAngleOfNeighbours = totalAngleOfNeighbours / (float)neighbouringBoid.size();
        boidMoveAngle[firstIndex] = AverageAngleOfNeighbours;
    }

*/