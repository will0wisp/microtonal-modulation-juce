/*
  ==============================================================================
    A namespace "IDs" containing all of the juce::Identifiers for this project.
 
    Identifiers.h
    Created: 15 Jun 2021 11:34:01am
    Author:  Willow Weiner

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace IDs
{
//related to MidiProcessor object
const juce::Identifier midiProcessor("midiProcessor");//MidiProcessor::midiProcessorValues ValueTree
const juce::Identifier lastNotePlayed("lastNotePlayed");
const juce::Identifier modCenter("modCenter");
const juce::Identifier modPivot("modPivot");


//related to Scale object
const juce::Identifier scale("scale"); //this is the Scale::scaleValues ValueTree
const juce::Identifier scaleDescription("description");
const juce::Identifier scaleNotes("scaleNotes");
const juce::Identifier fundamentalFreq("fundamentalFreq");

//related to KeyboardMap object
const juce::Identifier keyboardMap("keyboardMap"); //KeyboardMap::keyboardMapValues ValueTree
//TODO: rename this so it is not confusing. it sounds like it belongs to the scale
//although it is part of information for the keyboardMap
const juce::Identifier scaleLength("scaleLength");
const juce::Identifier retuneRangeLowerBound("retuneRangeLowerBound");
const juce::Identifier retuneRangeUpperBound("returnRangeUpperBound");
const juce::Identifier middleNote("middleNote");
const juce::Identifier referenceNote("referenceNote");
const juce::Identifier referenceFreq("referenceFreq");
const juce::Identifier formalOctaveScaleDegree("formalOctaveScaleDegree");
const juce::Identifier keyboardMapping("keyboardMapping");


} //end namespace IDs
