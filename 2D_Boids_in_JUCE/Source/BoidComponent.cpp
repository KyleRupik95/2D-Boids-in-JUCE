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
        boidX[i] = rand() % 650 + 15;
        boidY[i] = rand() % 650 + 15;
        boidSpeed[i] = 1;
        boidMoveDirection[i] = rand() % 16;
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

    //averageDirectionOfNeighbours();
   // turnToRelativeCentre();
   // convertSpeedAndAngleToMovement();
    //boundary rule stops boids flying off the screen
    boundaryRule(leftBoundary, rightBoundary, topBoundary, bottomBoundary);

    //rule for moving all boids towards the average center point of all boids x and y coordinates
    moveToFlock();

    //rule for stopping boids from flying into each other
    personalSpace();

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
        if (boidX[index] > rightBoundary)
            boidX[index] = boidX[index] - 6;
        if (boidX[index] < leftBoundary)
            boidX[index] = boidX[index] + 6;
        if (boidY[index] > bottomBoundary)
            boidY[index] = boidY[index] - 6;
        if (boidY[index] < topBoundary)
            boidY[index] = boidY[index] + 6;
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
        if (boidMoveDirection[index] >= 0.0  && boidMoveDirection[index] < 0.5)  // 0
        {
            boidY[index] -= 3 * boidSpeed[index];
            // boidX[index] -= 0;
        }
        if (boidMoveDirection[index] >= 0.5 && boidMoveDirection[index] < 1.5) // 1
        {
            boidY[index] -= 3 * boidSpeed[index];
            boidX[index] += 1 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 1.5 && boidMoveDirection[index] < 2.5) // 2
        {
            boidY[index] -= 2 * boidSpeed[index];
            boidX[index] += 2 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 2.5 && boidMoveDirection[index] < 3.5) // 3
        {
            boidY[index] -= 1 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 3.5 && boidMoveDirection[index] < 4.5) // 4
        {
            //boidY[index] += 0 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 4.5 && boidMoveDirection[index] < 5.5) // 5
        {
            boidY[index] += 1 * boidSpeed[index];
            boidX[index] += 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 5.5 && boidMoveDirection[index] < 6.5) // 6
        {
            boidY[index] += 2 * boidSpeed[index];
            boidX[index] += 2 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 6.5 && boidMoveDirection[index] < 7.5) // 7
        {
            boidY[index] += 3 * boidSpeed[index];
            boidX[index] += 1 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 7.5 && boidMoveDirection[index] < 8.5) // 8
        {
            boidY[index] += 3 * boidSpeed[index];
            // boidX[index] -= 0;
        }
        if (boidMoveDirection[index] >= 8.5 && boidMoveDirection[index] < 9.5) //9
        {
            boidY[index] += 3 * boidSpeed[index];
            boidX[index] -= 1 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 9.5 && boidMoveDirection[index] < 10.5) //10
        {
            boidY[index] += 2 * boidSpeed[index];
            boidX[index] -= 2 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 10.5 && boidMoveDirection[index] < 11.5) //11
        {
            boidY[index] += 1 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 11.5 && boidMoveDirection[index] < 12.5) //12
        {
            //boidY[index] -= 0 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 12.5 && boidMoveDirection[index] < 13.5) //13
        {
            boidY[index] -= 1 * boidSpeed[index];
            boidX[index] -= 3 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 13.5 && boidMoveDirection[index] < 14.5) //14
        {
            boidY[index] -= 2 * boidSpeed[index];
            boidX[index] -= 2 * boidSpeed[index];
        }
        if (boidMoveDirection[index] >= 14.5 && boidMoveDirection[index] < 15.5) //15
        {
            boidY[index] -= 3 * boidSpeed[index];
            boidX[index] -= 1 * boidSpeed[index];
        }
    }
}

void BoidComponent::averageDirectionOfNeighbours()
{
    float totalAngleOfNeighbours = 0;
    float averageAngleOfNeighbours = 0;
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
                distanceY = boidY[index1] - boidY[index2];
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
                totalAngleOfNeighbours += boidMoveDirection[index2];
                numOfNeighbours += 1;
                pushBack = false;
            }
        }
        averageAngleOfNeighbours = totalAngleOfNeighbours / numOfNeighbours;
        boidMoveDirection[index1] = averageAngleOfNeighbours;
        totalAngleOfNeighbours = 0;
        numOfNeighbours = 0;
        pushBack = false;
    }
}

void BoidComponent::turnToRelativeCentre()
{

    int averageX = 0;
    int averageY = 0;
    int totalX = 0;
    int totalY = 0;
    bool pushBackX = false;
    bool pushBackY = false;
    float numOfNeighbours = 0;
    int distanceX = 0;
    int distanceY = 0;
    double currentNextX = 0.0;
    double currentNextY = 0.0;
    double currentAngleBoid = 0.0;
    double differenceX = 0.0;
    double differenceY = 0.0;
    double relativeAngleMid = 0.0;
    double currentDifX = 0.0;
    double currentDifY = 0.0;

    for (int index1 = 0; index1 < NumOfBoids; ++index1)
    {
        for (int index2 = 0; index2 < NumOfBoids; ++index2)
        {
            if (index1 != index2)
            {
                distanceX = boidX[index1] - boidX[index2];
                distanceY = boidY[index1] - boidY[index2];
            }
            if (distanceX <= neighbourSearchDistance && distanceX > 0) //scan right
                pushBackX = true;
            if (distanceX >= -neighbourSearchDistance && distanceX < 0) //scan left
                pushBackX = true;
            if (distanceY <= neighbourSearchDistance && distanceY > 0) //scan down
                pushBackY = true;
            if (distanceY >= -neighbourSearchDistance && distanceY < 0) // scan up
                pushBackY = true;
            if (pushBackX == true)
            {
                totalX += boidX[index2];
                totalY += boidY[index2];
                numOfNeighbours += 1;
                pushBackX = false;
            }
            if (pushBackY == true)
            {
                totalX += boidX[index2];
                totalY += boidY[index2];
                numOfNeighbours += 1;
                pushBackY = false;
            }
        }
        averageX = totalX / numOfNeighbours;
        averageY = totalY / numOfNeighbours;
        pushBackX = false;
        pushBackY = false;

        differenceX = averageX - boidX[index1];
        differenceY = averageY - boidY[index1];
        relativeAngleMid = atan(differenceX / differenceY);
        relativeAngleMid = relativeAngleMid * 180.0 / pi;

        currentNextX = getNextXCoord(boidX[index1], boidMoveDirection[index1], boidSpeed[index1]);
        currentNextY = getNextYCoord(boidY[index1], boidMoveDirection[index1], boidSpeed[index1]);
        currentDifX = currentNextX - boidX[index1];
        currentDifY = currentDifY - boidY[index1];
        currentAngleBoid = atan(currentDifX / currentDifY);
        currentAngleBoid = currentAngleBoid * 180.0 / pi;

        if (relativeAngleMid > currentAngleBoid)
            boidMoveDirection[index1] += 1.0;
        if (relativeAngleMid < currentAngleBoid)
            boidMoveDirection[index1] -= 1.0;

        numOfNeighbours = 0.0;
        totalX = 0.0;
        totalY = 0.0;
        differenceX = 0.0;
        differenceY = 0.0;
        relativeAngleMid = 0.0;
        currentNextX = 0.0;
        currentNextY = 0.0;
        currentDifX = 0.0;
        currentDifY = 0.0;
        currentAngleBoid = 0.0;

    }
}


void BoidComponent::turnAwayFromBoundary()
{

}

int BoidComponent::getNextXCoord(int boidX, float direction, float speed)
{
    if (direction >= 0.0 && direction < 0.5)  // 0
    {
        return boidX;
    }
    if (direction >= 0.5 && direction < 1.5) // 1
    {
        return boidX + 1 * speed;
    }
    if (direction >= 1.5 && direction < 2.5) // 2
    {  
        return boidX + 2 * speed;
    }
    if (direction >= 2.5 && direction < 3.5) // 3
    {
        return boidX + 3 * speed;
    }
    if (direction >= 3.5 && direction < 4.5) // 4
    {
        return boidX + 3 * speed;
    }
    if (direction >= 4.5 && direction < 5.5) // 5
    {
        return boidX + 3 * speed;
    }
    if (direction >= 5.5 && direction < 6.5) // 6
    {
        return boidX + 2 * speed;
    }
    if (direction >= 6.5 && direction < 7.5) // 7
    {
        return boidX + 1 * speed;
    }
    if (direction >= 7.5 && direction < 8.5) // 8
    {
        return boidX;
    }
    if (direction >= 8.5 && direction < 9.5) //9
    {
        return boidX - 1 * speed;
    }
    if (direction >= 9.5 && direction < 10.5) //10
    {
        return boidX - 2 * speed;
    }
    if (direction >= 10.5 && direction < 11.5) //11
    {
        return boidX - 3 * speed;
    }
    if (direction >= 11.5 && direction < 12.5) //12
    {
        return boidX - 3 * speed;
    }
    if (direction >= 12.5 && direction < 13.5) //13
    {
        return boidX - 3 * speed;
    }
    if (direction >= 13.5 && direction < 14.5) //14
    {
        return boidX - 2 * speed;
    }
    if (direction >= 14.5 && direction < 15.5) //15
    {
        return boidX - 1 * speed;
    }

}
int BoidComponent::getNextYCoord(int boidY, float direction, float speed)
{
    if (direction >= 0.0 && direction < 0.5)  // 0
    {
        return boidY - 3 * speed;
    }
    if (direction >= 0.5 && direction < 1.5) // 1
    {
        return boidY - 3 * speed;
    }
    if (direction >= 1.5 && direction < 2.5) // 2
    {
        return boidY - 2 * speed;
    }
    if (direction >= 2.5 && direction < 3.5) // 3
    {
        return boidY - 1 * speed;
    }
    if (direction >= 3.5 && direction < 4.5) // 4
    {
        return boidY;
    }
    if (direction >= 4.5 && direction < 5.5) // 5
    {
        return boidY + 1 * speed;
    }
    if (direction >= 5.5 && direction < 6.5) // 6
    {
        return boidY + 2 * speed;
    }
    if (direction >= 6.5 && direction < 7.5) // 7
    {
        return boidY + 3 * speed;
    }
    if (direction >= 7.5 && direction < 8.5) // 8
    {
        return boidY + 3 * speed;
    }
    if (direction >= 8.5 && direction < 9.5) //9
    {
        return boidY + 3 * speed;
    }
    if (direction >= 9.5 && direction < 10.5) //10
    {
        return boidY + 2 * speed;
    }
    if (direction >= 10.5 && direction < 11.5) //11
    {
        return boidY + 1 * speed;
    }
    if (direction >= 11.5 && direction < 12.5) //12
    {
        return boidY;
    }
    if (direction >= 12.5 && direction < 13.5) //13
    {
        return boidY - 1 * speed;
    }
    if (direction >= 13.5 && direction < 14.5) //14
    {
        return boidY - 2 * speed;
    }
    if (direction >= 14.5 && direction < 15.5) //15
    {
        return boidY - 3 * speed;
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