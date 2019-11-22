#include "GulgEngine/Signature.hpp"

namespace Gg {

Signature::Signature() {}

Signature::Signature(const size_t signatureSize) {

	m_signature.resize(signatureSize);
	for(size_t i{0}; i < m_signature.size(); i++) { m_signature[i] = false; }
}

Signature::Signature(const std::vector<bool> sign): m_signature{sign} {}

Signature::Signature(const Signature &second): m_signature{second.m_signature} {}

void Signature::operator=(const Signature &second) { m_signature = second.m_signature; }

bool Signature::operator<(const Signature &second) const {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to compare two signatures with differents sizes. Will return false." << std::endl;
		return false;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { 

		if(m_signature[i] != second.m_signature[i] && m_signature[i] == true) { return false; } // So second.m_signature[i] == false
	}

	return true;
}

bool Signature::operator>(const Signature &second) const {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to compare two signatures with differents sizes. Will return false." << std::endl;
		return false;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { 

		if(m_signature[i] != second.m_signature[i] && m_signature[i] == false) { return false; } // So second.m_signature[i] == false
	}

	return true;
}

bool Signature::operator==(const Signature &second) const {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to compare two signatures with differents sizes. Will return false." << std::endl;
		return false;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { 

		if(m_signature[i] != second.m_signature[i]) { return false; } 
	}

	return true;
}

Signature Signature::operator+(const Signature &second) const {

	std::vector<bool> newSignature{m_signature};

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to add two signatures with differents sizes. Will return empty signature." << std::endl;
		return Signature{newSignature};
	}

	for(size_t i{0}; i < m_signature.size(); i++) { newSignature[i] = m_signature[i] | second.m_signature[i]; }
	return Signature{newSignature};
}

void Signature::operator+=(const Signature &second) {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to add two signatures with differents sizes. Operation is not going to be applied." << std::endl;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { m_signature[i] = m_signature[i] | second.m_signature[i]; }
}

Signature Signature::operator-(const Signature &second) const {

	std::vector<bool> newSignature{m_signature};

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to substract two signatures with differents sizes. Will return empty signature." << std::endl;
		return Signature{newSignature};
	}

	for(size_t i{0}; i < m_signature.size(); i++) { 

		if(m_signature[i] == true && m_signature[i] == second.m_signature[i]) { newSignature[i] = false; }
		else { newSignature[i] = m_signature[i] | second.m_signature[i]; }
	}

	return Signature{newSignature};
}

void Signature::operator-=(const Signature &second) {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to substract two signatures with differents sizes. Operation is not going to be applied." << std::endl;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { 

		if(m_signature[i] == true && second.m_signature[i] == true) { m_signature[i] = false; }
	}
}

Signature Signature::operator|(const Signature &second) const {

	std::vector<bool> newSignature{m_signature};

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to apply | operator in two signatures with differents sizes. Will return empty signature." << std::endl;
		return Signature{newSignature};
	}

	for(size_t i{0}; i < m_signature.size(); i++) { newSignature[i] = m_signature[i] | second.m_signature[i]; }

	return Signature{newSignature};
}

void Signature::operator|=(const Signature &second) {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to apply | operator in two signatures with differents sizes. Operation is not going to be applied." << std::endl;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { m_signature[i] = m_signature[i] | second.m_signature[i]; }
}


Signature Signature::operator&(const Signature &second) const {

	std::vector<bool> newSignature{m_signature};

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to apply & operator in two signatures with differents sizes. Will return empty signature." << std::endl;
		return Signature{newSignature};
	}

	for(size_t i{0}; i < m_signature.size(); i++) { newSignature[i] = m_signature[i] & second.m_signature[i]; }

	return Signature{newSignature};
}

void Signature::operator&=(const Signature &second) {

	if(m_signature.size() != second.m_signature.size()) {

		std::cout << "Gulg warning: try to apply & operator in two signatures with differents sizes. Operation is not going to be applied." << std::endl;
	}

	for(size_t i{0}; i < m_signature.size(); i++) { m_signature[i] = m_signature[i] & second.m_signature[i]; }
}


void Signature::changeBit(const size_t bit, const bool value) {

	if(bit >= m_signature.size()) { std::cout << "Gulg warning: try to change bit value which is out of bond. Nothing will append." << std::endl; }
	else { m_signature[bit] = value; }
}

std::string Signature::asString() const {

	std::string result;
	for(size_t i{0}; i < m_signature.size(); i++) { result += std::to_string(m_signature[i]); }
	return result;
}

std::ostream& operator<<(std::ostream &stream, const Signature &sign) {

	stream << sign.asString();
	return stream;
}

size_t Signature::getSignatureSize() const { return m_signature.size(); }


}