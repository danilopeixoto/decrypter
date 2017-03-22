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

#ifndef DECRYPTER_H
#define DECRYPTER_H

#include "global.h"
#include "types.h"
#include <fstream>
#include <string>

DECRYPTER_NAMESPACE_BEGIN

struct DecrypterData;

enum RequestType {
	Input = 0,
	Output,
	None
};

class Decrypter {
private:
	Character * inputFilename, * outputFilename;
	DecrypterData * data;
	
	std::fstream file;

	bool isValidKey(std::string &) const;

public:
	Decrypter();
	~Decrypter();

	Decrypter & operator =(const Decrypter &);

	Character * getInputFilename();
	Character * getOutputFilename();
	DecrypterData * getData();
	const Character * getInputFilename() const;
	const Character * getOutputFilename() const;
	const DecrypterData * getData() const;

	Bool isEmpty() const;
	Bool hasInputFilename() const;
	Bool hasOutputFilename() const;
	Bool hasData() const;

	void pause() const;
	void message(const Character *) const;
	void space() const;
	
	Decrypter & read();
	Decrypter & write();

	Decrypter & request(const RequestType & = RequestType::None);
	Decrypter & decrypt();
	Decrypter & clearInputFilename();
	Decrypter & clearOutputFilename();
	Decrypter & clearData();
	Decrypter & clear();
};

DECRYPTER_NAMESPACE_END

#endif