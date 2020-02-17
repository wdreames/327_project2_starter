/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 *
 *  Finalized on: Feb 17, 2020
 *  	Author: William Reames
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>

#include "array_functions.h"
#include "constants.h"
#include "utilities.h"

using namespace constants;
using namespace std;

const string DEFAULT_WORD_VALUE = "";
const int DEFAULT_OCCURANCES_VALUE = 0;
const int MAX_LENGTH = 1000;

//Used to hold each word and the amount of times it appears
struct word_occurances {
	int occurances;
	string word;
};

/* Holds each occurance of a word
 * Initializes with a length of 1000 to allow for ample space during testing and grading.
 */
word_occurances words[MAX_LENGTH] = { };
int next_slot = 0;

//zero out array that tracks words and their occurrences
void clearArray() {
	/* Although it isn't necessary to zero out the array, it ensures
	 * that we will not run into any bad data later on.
	 */
	for (int i = 0; i < getArraySize(); i++) {
		words[i].occurances = DEFAULT_OCCURANCES_VALUE;
		words[i].word = DEFAULT_WORD_VALUE;
	}
	next_slot = 0;
}

//how many unique words are in array
int getArraySize() {
	return next_slot;
}

//get data at a particular location
string getArrayWordAt(int i) {
	if (i < getArraySize()) {
		return words[i].word;
	} else {
		return DEFAULT_WORD_VALUE;
	}
}

int getArrayWord_NumbOccur_At(int i) {
	if (i < getArraySize()) {
		return words[i].occurances;
	} else {
		return DEFAULT_OCCURANCES_VALUE;
	}
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(fstream &myfstream) {
	if (myfstream.is_open()) {
		string line;
		while (!myfstream.eof()) {
			getline(myfstream, line);
			processLine(line);
		}
		return true;
	} else {
		return false;
	}
}

/*take 1 line and extract all the tokens from it
 feed each token to processToken for recording
 Used the given code on github*/
void processLine(string &myString) {
	stringstream ss(myString);
	string tempToken;

	while (getline(ss, tempToken, CHAR_TO_SEARCH_FOR)) {
		processToken(tempToken);
	}
}

/*Keep track of how many times each token seen*/
//Input is a string
//The string may contain characters that need to be removed with strip_unwanted_chars()
//Add the manipulated string to the list
void processToken(string &token) {
	string clone = token;  //Makes a copy of token so token remains unchanged
	toUpper(clone);
	if (strip_unwanted_chars(clone)) { //If the method returns false, there won't be anything to add
		if (getArraySize() > 0) {
			for (int i = 0; i < getArraySize(); i++) {
				string temp_val = getArrayWordAt(i);
				toUpper(temp_val);
				if (clone == temp_val) { //Tests if the word is already in the array
					words[i].occurances++; //If it is it adds to the number of occurances
					return; //Once it finds the place to add the word, it exits the method
				}
			}
		}
		//If word does not exist in the array, it is added to it.
		strip_unwanted_chars(token);
		words[next_slot].word = token;
		words[next_slot].occurances = 1; //When the word is added, there is only 1 instance of it
		next_slot += 1;
	}
}

/*if you are debugging the file must be in the project parent directory
 in this case Project2 with the .project and .cProject files*/
bool openFile(fstream &myfile, const string &myFileName,
		ios_base::openmode mode) {
	if (myfile.is_open()) {
		return false;
	} else {
		myfile.open(myFileName, mode);
		if (myfile.is_open()) {
			return true;
		} else {
			return false;
		}
	}
}

/*if myfile is open then close it*/
void closeFile(fstream &myfile) {
	if (myfile.is_open()) {
		myfile.close();
	}
}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const string &outputfilename) {
	fstream myfile;
	if (getArraySize() == 0) {
		return FAIL_NO_ARRAY_DATA;
	} else if (openFile(myfile, outputfilename, ios::out)) {
		for (int i = 0; i < getArraySize(); i++) {
			myfile << getArrayWordAt(i) << " " << getArrayWord_NumbOccur_At(i)
					<< endl;
		}
		closeFile(myfile);
		return SUCCESS;
	} else {
		return FAIL_FILE_DID_NOT_OPEN;
	}
}

/*
 * Sort myEntryArray based on so enum value.
 * The presence of the enum implies a switch statement
 * Uses an insertion sort method for all cases
 */
void sortArray(constants::sortOrder so) {
	switch (so) {
	case NONE:
		break; //No sorting required
	default:  //Default sortArray will use an ascending sort
	case ASCENDING:
		for (int i = 1; i < getArraySize(); i++) {
			word_occurances temp_val = words[i]; //Pulls out the word_occurances struct at index i
			string temp_word = temp_val.word; //Gets the word value in the struct
			toUpper(temp_word);  //Makes it uppercase for testing
			int j = i - 1;
			while (j >= 0) {
				//Pulls out a different word to compare and makes it uppercase
				string current_word = getArrayWordAt(j);
				toUpper(current_word);
				//Compares the values and decides how to proceed with the inerstion sort
				if (current_word > temp_word) {
					words[j + 1] = words[j];
					j = j - 1;
				} else {
					break;
				}
			}
			//Adds the original value back into the array after shifting over the other elements
			words[j + 1] = temp_val;
		}
		break;
	case DESCENDING:
		for (int i = 1; i < getArraySize(); i++) {
			word_occurances temp_val = words[i]; //Pulls out the word_occurances struct at index i
			string temp_word = temp_val.word; //Gets the word value in the struct
			toUpper(temp_word);  //Makes it uppercase for testing
			int j = i - 1;
			while (j >= 0) {
				//Pulls out a different word to compare and makes it uppercase
				string current_word = getArrayWordAt(j);
				toUpper(current_word);
				//Compares the values and decides how to proceed with the inerstion sort
				if (current_word < temp_word) {
					words[j + 1] = words[j];
					j = j - 1;
				} else {
					break;
				}
			}
			//Adds the original value back into the array after shifting over the other elements
			words[j + 1] = temp_val;
		}
		break;
	case NUMBER_OCCURRENCES:
		for (int i = 1; i < getArraySize(); i++) {
			word_occurances temp_val = words[i]; //Pulls out the word_occurances struct at index i
			int j = i - 1;
			while (j >= 0 && getArrayWord_NumbOccur_At(j) < temp_val.occurances) //Loops through and tests when you find where the list is sorted
			{
				words[j + 1] = words[j];
				j = j - 1;
			}
			//Adds the original value back into the array after shifting over the other elements
			words[j + 1] = temp_val;
		}
		break;
	}
}

