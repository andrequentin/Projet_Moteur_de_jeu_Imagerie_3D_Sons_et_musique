#ifndef LOAD_SOUND_HPP
#define LOAD_SOUND_HPP

#include <string>
#include <map>
#include <iostream>

#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>


bool loadSound(FMOD::Studio::System **soundSystem, 
			   FMOD::Studio::Bank **masterBank, 
			   FMOD::Studio::Bank **masterStringBank,
			   FMOD::Studio::EventDescription** descriptions);

FMOD::Studio::EventInstance *getSoundInstance(const std::string path, 
											  FMOD::Studio::EventDescription *descriptions,
											  FMOD::Studio::Bank *masterBank);


void fmodError(const FMOD_RESULT result, const std::string errorString);



#endif