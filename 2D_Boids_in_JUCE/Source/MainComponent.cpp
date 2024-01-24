#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize(700, 700);

    addAndMakeVisible(boid1);

}
MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    boid1.setBounds(getLocalBounds());
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
