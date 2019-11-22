#ifndef SIGNATURE_LOADER_HPP
#define SIGNATURE_LOADER_HPP

#include <string>
#include <map>
#include <fstream>

#include "GulgEngine/Signature.hpp"
#include "GulgEngine/GulgDeclarations.hpp"

namespace Gg {

class SignatureLoader {

	public:

		SignatureLoader();

		bool loadFile(const std::string path);

		bool existingName(const std::string name) const;

		Signature getSignature(const std::string name) const;

		size_t getNumberOfSignatures() const;

	private:

		std::map<std::string, Signature> m_signatures;               
};

}

#endif