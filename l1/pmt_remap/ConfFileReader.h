/*
 *  ConfFileReader.h
 *
 *  Created on: 30/gen/2015
 *      Author: Valeria Fascianelli
 *      Email: vxf441@hep.ph.bham.ac.uk
 */

#ifndef PMTDECODER_CONFFILEREADER_H_
#define PMTDECODER_CONFFILEREADER_H_

#include <fstream>
#include <sstream>
#include <string>

class ConfFileReader {
public:
	ConfFileReader(const char* filename);
	ConfFileReader(const std::string& filename);

	bool nextLine();

	template<typename T>
	T getField(const int n);

	template<typename T>
	T getLine();

	bool inputFailed() const;
	bool isValid() const;

private:
	ConfFileReader(const ConfFileReader& rhs); // not implemented!
	ConfFileReader& operator=(const ConfFileReader& rhs); // not implemented!

	void skip_fields(std::istringstream& ist, const int n);

	std::ifstream file;
	std::string line;
	bool failed;
};

// Definition of FileReader::get template
template<typename T>
T ConfFileReader::getField(const int n) {
	failed = false;
	std::istringstream ist(line);
	this->skip_fields(ist, n - 1);
	T rval;
	ist >> rval;
	if (ist.fail()) {
		failed = true;
		return T();
	} else {
		return rval;
	}
}

template<typename T>
T ConfFileReader::getLine() {
	failed = false;
	std::istringstream ist(line);
	T rval;
	ist >> rval;
	if (ist.fail()) {
		failed = true;
		return T();
	} else {
		return rval;
	}
}

#endif

#endif /* PMTDECODER_CONFFILEREADER_H_ */
