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

#include "decrypterMatrix.h"
#include <string>

DECRYPTER_NAMESPACE_BEGIN

DecrypterMatrix::DecrypterMatrix() :
	data(DECRYPTER_NULL), access(DECRYPTER_NULL), rows(0), columns(0) {}
DecrypterMatrix::DecrypterMatrix(const Character * data, UInt rows, UInt columns) {
	this->data = DECRYPTER_NULL;
	this->access = DECRYPTER_NULL;
	this->rows = rows;
	this->columns = columns;
	
	if (rows != 0 && columns != 0) {
		this->data = new Character[rows * columns + 1];
		this->data[rows * columns] = '\0';

		access = new Character *[rows];

		#pragma omp parallel for
		for (Int i = 0; i < (Int)rows; i++)
			access[i] = this->data + i * columns;
		
		if (data != DECRYPTER_NULL)
			std::strcpy(this->data, data);
	}
}
DecrypterMatrix::~DecrypterMatrix() {
	clear();
}

DecrypterMatrix & DecrypterMatrix::operator =(const DecrypterMatrix & decrypterMatrix) {
	clear();

	rows = decrypterMatrix.getRowCount();
	columns = decrypterMatrix.getColumnCount();

	if (rows != 0 && columns != 0) {
		data = new Character[rows * columns + 1];
		data[rows * columns] = '\0';

		access = new Character *[rows];

		#pragma omp parallel for
		for (Int i = 0; i < (Int)rows; i++)
			access[i] = data + i * columns;

		if (!decrypterMatrix.isEmpty())
			std::strcpy(data, decrypterMatrix.getData());
	}
	
	return *this;
}
Character * DecrypterMatrix::operator [](UInt i) {
	return access[i];
}
const Character * DecrypterMatrix::operator [](UInt i) const {
	return access[i];
}

DecrypterMatrix & DecrypterMatrix::setData(const Character * data, UInt rows, UInt columns) {
	clear();

	this->rows = rows;
	this->columns = columns;

	if (rows != 0 && columns != 0) {
		this->data = new Character[rows * columns + 1];
		this->data[rows * columns] = '\0';

		access = new Character *[rows];

		#pragma omp parallel for
		for (Int i = 0; i < (Int)rows; i++)
			access[i] = this->data + i * columns;

		if (data != DECRYPTER_NULL)
			std::strcpy(this->data, data);
	}
	
	return *this;
}
Character * DecrypterMatrix::getData() {
	return data;
}
const Character * DecrypterMatrix::getData() const {
	return (const Character *)data;
}

UInt DecrypterMatrix::getRowCount() const {
	return rows;
}
UInt DecrypterMatrix::getColumnCount() const {
	return columns;
}
Bool DecrypterMatrix::isEmpty() const {
	return data == DECRYPTER_NULL;
}

DecrypterMatrix & DecrypterMatrix::clear() {
	if (data != DECRYPTER_NULL) {
		delete[] data;
		delete[] access;
	}

	data = DECRYPTER_NULL;
	access = DECRYPTER_NULL;
	rows = 0;
	columns = 0;

	return *this;
}

DECRYPTER_NAMESPACE_END