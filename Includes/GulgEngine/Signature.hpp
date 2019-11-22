#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <vector>
#include <iostream>

namespace Gg {

class Signature {

	public:

		Signature();
		Signature(const size_t signatureSize);
		Signature(const std::vector<bool> sign);
		Signature(const Signature &second);

		void operator=(const Signature &second);

		bool operator<(const Signature &second) const;
		bool operator>(const Signature &second) const;
		bool operator==(const Signature &second) const;

		Signature operator+(const Signature &second) const;
		void operator+=(const Signature &second);

		Signature operator-(const Signature &second) const;
		void operator-=(const Signature &second);

		Signature operator|(const Signature &second) const;
		void operator|=(const Signature &second);

		Signature operator&(const Signature &second) const;
		void operator&=(const Signature &second);

		void changeBit(const size_t bit, const bool value);

		std::string asString() const;

		size_t getSignatureSize() const;

	private:

		std::vector<bool> m_signature;                  
};

std::ostream& operator<<(std::ostream &stream, const Signature &sign);

}

#endif