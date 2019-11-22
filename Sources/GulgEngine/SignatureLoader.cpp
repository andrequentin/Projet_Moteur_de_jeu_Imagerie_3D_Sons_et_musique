#include "GulgEngine/SignatureLoader.hpp"

namespace Gg {

SignatureLoader::SignatureLoader() {}

bool SignatureLoader::loadFile(const std::string path) {

	std::ifstream file{path};

	if(!file) {

		std::cout << "Gulg warning: can't open file " << path << ", names of signatures will not ne extracted." << std::endl;
		return false;
	}

	std::string currentLine;

	while(std::getline(file, currentLine)) {

		if(existingName(currentLine)) { std::cout << "Gulg warning: the name \"" << currentLine << "\" appears at least twice in file." << std::endl; }
		else { m_signatures.insert(std::make_pair(currentLine, Signature{})); }
	}

	file.close();

	size_t nbNames{m_signatures.size()}, currentNumber{0};

	for(std::map<std::string, Signature>::iterator it{m_signatures.begin()}; it != m_signatures.end(); it++) {

		it->second = Signature{nbNames};
		it->second.changeBit(currentNumber, true);
		++currentNumber;
	}

	return true;
}

bool SignatureLoader::existingName(const std::string name) const {

	if(m_signatures.find(name) == m_signatures.end()) { return false; }
	return true;
}

Signature SignatureLoader::getSignature(const std::string name) const {

	if(existingName(name)) { return m_signatures.at(name); }

	throw std::runtime_error("Gulg error: asked for signature with a name of " + name  + ", which doesn't exist.");
	return Signature{};
}

size_t SignatureLoader::getNumberOfSignatures() const { return m_signatures.size(); }

}