#include "LoadSound.hpp"

bool loadSound(FMOD::Studio::System **soundSystem,
			   FMOD::Studio::Bank **masterBank,
			   FMOD::Studio::Bank **masterStringBank,
			   FMOD::Studio::EventDescription** descriptions) {

 	FMOD_RESULT fmodResult;

    fmodResult = FMOD::Studio::System::create(soundSystem);
    fmodError(fmodResult, "system initialization");
    if(fmodResult != FMOD_OK) { return false; }

    // Initialize FMOD Studio, which will also initialize FMOD Core
    fmodResult = (*soundSystem)->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    fmodError(fmodResult, "system initialization");
	if(fmodResult != FMOD_OK) { return false; }


    fmodResult = (*soundSystem)->loadBankFile("Datas/SoundTest/projet/Build/Desktop/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, masterBank);
    fmodError(fmodResult, "bank load");
    if(fmodResult != FMOD_OK) { return false; }


    fmodResult = (*soundSystem)->loadBankFile("Datas/SoundTest/projet/Build/Desktop/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, masterStringBank);
    fmodError(fmodResult, "string bank load");
    if(fmodResult != FMOD_OK) { return false; }

    int nbEvents{0};
    fmodResult = (*masterBank)->getEventList(descriptions, 100, &nbEvents);
    fmodError(fmodResult, "event description list");
    if(fmodResult != FMOD_OK) { return false; }

    return true;
}

FMOD::Studio::EventInstance *getSoundInstance(const std::string path,
											  FMOD::Studio::EventDescription* descriptions,
											  FMOD::Studio::Bank *masterBank) {


	FMOD::Studio::EventInstance *result{nullptr};

	int nbDescriptions{0};
	FMOD_RESULT fmodResult;
	fmodResult = masterBank->getEventCount(&nbDescriptions);
    fmodError(fmodResult, "bank nb event description");
    if(fmodResult != FMOD_OK) { return result; }


    if(path.size() >= 1024) {

    	std::cout << "Too long name for event." << std::endl;
    	return result;
    }

    char eventPath[1024];

    for(int i{0}; i < nbDescriptions; i++) {

    	fmodResult = descriptions[i].getPath(&eventPath[0], 1024, nullptr);
    	fmodError(fmodResult, "event path");
    	// if(fmodResult != FMOD_OK) { return result; }
			//
    	// else if (strcmp(eventPath, path.c_str()) != 0) {
			//
    	// 	fmodResult = descriptions[i].createInstance(&result);
    	// 	fmodError(fmodResult, "event instance creation");
    	// }
    }

    return result;
}

void fmodError(const FMOD_RESULT result, const std::string errorString) {

	if(result != FMOD_OK) {

        std::cout << "Error " << result << " with FMOD studio API in " << errorString << ": " << FMOD_ErrorString(result) << std::endl;
    }
}
