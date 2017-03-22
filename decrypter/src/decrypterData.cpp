// Copyright (c) 2017, Danilo Peixoto. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "decrypterData.h"
#include <string>

DECRYPTER_NAMESPACE_BEGIN

DecrypterData::DecrypterData() : data(DECRYPTER_NULL), key(DECRYPTER_NULL) {}
DecrypterData::DecrypterData(const Character * data, const Character * key) {
	this->data = DECRYPTER_NULL;
	this->key = DECRYPTER_NULL;
	
	if (data != DECRYPTER_NULL) {
		this->data = new Character[std::strlen(data) + 1];
		std::strcpy(this->data, data);
	}

	if (key != DECRYPTER_NULL) {
		this->key = new Character[std::strlen(key) + 1];
		std::strcpy(this->key, key);
	}
}
DecrypterData::~DecrypterData() {
	clear();
}

DecrypterData & DecrypterData::operator =(const DecrypterData & decrypterData) {
	clear();

	if (decrypterData.hasData()) {
		data = new Character[std::strlen(decrypterData.getData()) + 1];
		std::strcpy(data, decrypterData.getData());
	}

	if (decrypterData.hasKey()) {
		key = new Character[std::strlen(decrypterData.getKey()) + 1];
		std::strcpy(key, decrypterData.getKey());
	}

	return *this;
}
Character & DecrypterData::operator [](UInt i) {
	return data[i];
}
const Character & DecrypterData::operator [](UInt i) const {
	return data[i];
}

DecrypterData & DecrypterData::setData(const Character * data) {
	clearData();

	if (data != DECRYPTER_NULL) {
		this->data = new Character[std::strlen(data) + 1];
		std::strcpy(this->data, data);
	}

	return *this;
}
DecrypterData & DecrypterData::setKey(const Character * key) {
	clearKey();

	if (key != DECRYPTER_NULL) {
		this->key = new Character[std::strlen(key) + 1];
		std::strcpy(this->key, key);
	}

	return *this;
}
Character * DecrypterData::getData() {
	return data;
}
Character * DecrypterData::getKey() {
	return key;
}
const Character * DecrypterData::getData() const {
	return (const Character *)data;
}
const Character * DecrypterData::getKey() const {
	return (const Character *)key;
}

Bool DecrypterData::isEmpty() const {
	return data == DECRYPTER_NULL && key == DECRYPTER_NULL;
}
Bool DecrypterData::hasData() const {
	return data != DECRYPTER_NULL;
}
Bool DecrypterData::hasKey() const {
	return key != DECRYPTER_NULL;
}

DecrypterData & DecrypterData::clearData() {
	if (hasData())
		delete[] data;

	data = DECRYPTER_NULL;

	return *this;
}
DecrypterData & DecrypterData::clearKey() {
	if (hasKey())
		delete[] key;

	key = DECRYPTER_NULL;

	return *this;
}
DecrypterData & DecrypterData::clear() {
	clearData();
	clearKey();

	return *this;
}

DECRYPTER_NAMESPACE_END