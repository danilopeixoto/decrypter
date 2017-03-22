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

#include "decrypter.h"
#include "exception.h"
#include "decrypterData.h"
#include "decrypterMatrix.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cctype>
#include <streambuf>

DECRYPTER_NAMESPACE_BEGIN

Decrypter::Decrypter() :
	inputFilename(DECRYPTER_NULL), outputFilename(DECRYPTER_NULL),
	data(DECRYPTER_NULL) {}
Decrypter::~Decrypter() {
	clear();
}

Decrypter & Decrypter::operator =(const Decrypter & decrypter) {
	clear();

	if (decrypter.hasInputFilename()) {
		inputFilename = new Character[std::strlen(decrypter.getInputFilename()) + 1];
		std::strcpy(inputFilename, decrypter.getInputFilename());
	}

	if (decrypter.hasOutputFilename()) {
		outputFilename = new Character[std::strlen(decrypter.getOutputFilename()) + 1];
		std::strcpy(outputFilename, decrypter.getOutputFilename());
	}

	if (decrypter.hasData()) {
		data = new DecrypterData;
		*data = *decrypter.getData();
	}

	return *this;
}

bool Decrypter::isValidKey(std::string & key) const {
	if (key.length() == 0)
		return false;

	bool abort = false;

	#pragma omp parallel for
	for (Int i = 0; i < (Int)key.length(); i++) {
		#pragma omp flush(abort)
		if (!abort) {
			key[i] = std::tolower(key[i]);

			if (!std::isalpha(key[i])) {
				abort = true;
				#pragma omp flush(abort)
			}
		}
	}
	
	return !abort;
}

Character * Decrypter::getInputFilename() {
	return inputFilename;
}
Character * Decrypter::getOutputFilename() {
	return outputFilename;
}
DecrypterData * Decrypter::getData() {
	return data;
}
const Character * Decrypter::getInputFilename() const {
	return (const Character *)inputFilename;
}
const Character * Decrypter::getOutputFilename() const {
	return (const Character *)outputFilename;
}
const DecrypterData * Decrypter::getData() const {
	return (const DecrypterData *)data;
}

Bool Decrypter::isEmpty() const {
	return inputFilename == DECRYPTER_NULL &&
		outputFilename == DECRYPTER_NULL  &&
		data == DECRYPTER_NULL;
}
Bool Decrypter::hasInputFilename() const {
	return inputFilename != DECRYPTER_NULL;
}
Bool Decrypter::hasOutputFilename() const {
	return outputFilename != DECRYPTER_NULL;
}
Bool Decrypter::hasData() const {
	return data != DECRYPTER_NULL;
}

void Decrypter::pause() const {
	std::system("pause");
}
void Decrypter::message(const Character * text) const {
	std::cout << text << std::endl;
}
void Decrypter::space() const {
	std::cout << std::endl;
}

Decrypter & Decrypter::read() {
	clearData();

	try {
		if (!hasInputFilename())
			throw Exception(ExceptionType::ReadError);

		file.open(inputFilename, std::fstream::in);

		if (!file.is_open())
			throw Exception(ExceptionType::ReadError);

		std::string content;

		std::getline(file, content);

		if (!isValidKey(content))
			throw Exception(ExceptionType::InvalidKey);

		data = new DecrypterData;

		data->setKey(content.c_str());
		
		content.assign(std::istreambuf_iterator<Character>(file),
			std::istreambuf_iterator<Character>());
		
		data->setData(content.c_str());
		
		file.close();
	}
	catch (const std::exception & exception) {
		message(exception.what());
		space();

		pause();
		std::exit(EXIT_FAILURE);
	}

	return *this;
}
Decrypter & Decrypter::write() {
	try {
		if (!hasData())
			throw Exception(ExceptionType::WriteError);

		if (!data->hasData())
			throw Exception(ExceptionType::WriteError);
		
		file.open(outputFilename, std::fstream::out);

		if (!file.is_open())
			throw Exception(ExceptionType::WriteError);

		file << data->getData();
		
		file.close();
	}
	catch (const std::exception & exception) {
		message(exception.what());
		space();

		pause();
		std::exit(EXIT_FAILURE);
	}

	return *this;
}

Decrypter & Decrypter::request(const RequestType & type) {
	switch (type) {
	case RequestType::Input:
		clearInputFilename();

		inputFilename = new Character[DECRYPTER_MAX_PATH_LENGTH];

		message("Enter a filename to open:");
		std::cin >> inputFilename;

		break;
	case RequestType::Output:
		clearOutputFilename();

		outputFilename = new Character[DECRYPTER_MAX_PATH_LENGTH];

		message("Enter a filename to save:");
		std::cin >> outputFilename;

		break;
	default:
		message("Enter a filename to open:");
		std::cin >> inputFilename;
	}
	
	return *this;
}
Decrypter & Decrypter::decrypt() {
	Character alpha[27] = "abcdefghijklmnopqrstuvwxyz";
	
	const Character * key = data->getKey();
	const Character * d = data->getData();

	UInt columnCount = std::strlen(key);
	UInt rowCount = std::strlen(d) / columnCount;

	UInt * columnIndices = new UInt[columnCount];

	UInt count = 0;
	
	for (UInt i = 0; i < 26; i++) {
		for (UInt j = 0; j < columnCount; j++) {
			if (alpha[i] == key[j])
				columnIndices[j] = count++;
		}
	}
	
	DecrypterMatrix * matrix = new DecrypterMatrix(DECRYPTER_NULL, rowCount, columnCount);

	#pragma omp parallel for
	for (Int i = 0; i < (Int)matrix->getColumnCount(); i++) {
		#pragma omp parallel for
		for (Int j = 0; j < (Int)matrix->getRowCount(); j++)
			(*matrix)[j][i] = d[columnIndices[i] * rowCount + j];
	}

	data->setData(matrix->getData());
	
	delete[] columnIndices;
	delete matrix;
	
	return *this;
}
Decrypter & Decrypter::clearInputFilename() {
	if (hasInputFilename())
		delete[] inputFilename;

	inputFilename = DECRYPTER_NULL;

	return *this;
}
Decrypter & Decrypter::clearOutputFilename() {
	if (hasOutputFilename())
		delete[] outputFilename;

	outputFilename = DECRYPTER_NULL;

	return *this;
}
Decrypter & Decrypter::clearData() {
	if (hasData())
		delete data;

	data = DECRYPTER_NULL;

	return *this;
}
Decrypter & Decrypter::clear() {
	clearInputFilename();
	clearOutputFilename();
	clearData();

	return *this;
}

DECRYPTER_NAMESPACE_END