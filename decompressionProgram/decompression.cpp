// Written by : Ege Ata Ceylan
//This program is an implementation of a decompression algorithm
/*
 The decompression algorithm proceeds as follows: For
 decompression, we input the codes one at a time and replace
 them by the texts they denote. The code-to-text mapping can be
 reconstructed in the following way. The codes assigned for
 single character texts are entered into the dictionary at the
 initialization (just as we did for compression). As before, the
 dictionary entries are code-text pairs. This time, however, the
 dictionary is searched for an entry with a given code (rather
 than with a given string).The first code in the compressed file must correspond to a single
 character and so may be replaced by the corresponding character.
 For all other codes x in the compressed file, we have two cases to consider:
 
 1) The code x is already in the dictionary: When x is in the dictionary, the corresponding
 text, text(x), to which it corresponds, is extracted from the dictionary and output. Also,
 from the working of the compressor, we know that if the code that precedes x in the
 compressed file is q and text (q) is the corresponding text, then the compressor would have
 created a new code for the text text(q) followed by the first character (that we will denote
 by fc(x)), of text(x). So, we enter the pair (next code, text(q)fc(x)) into the dictionary.
 
 2) The code x is NOT in the dictionary: This case arises only when the current text segment
 has the form text(q)text(q)fc(q) and text(x) = text(q)fc(q) The corresponding compressed file
 segment is qx. During compression, text(q)fc(q) is assigned the code x, and the code x is output for the text text(q)fc(q).
 During decompression, after q is replaced by text(q), we encounter the code x. However, there is no
 code-to-text mapping for x in our table. We are able to decode x using the fact that this
 situation arises only when the decompressed text segment is text(q)text(q)fc(q). When we
 encounter a code x for which the code-to-text mapping is undefined, the code-to-text mapping
 for x is text(q)fc(q), where q is the code that precedes x in the file.
 
 An example is given below with the same string which was given as example for the compression algorithm
 
 aaabbbbbbaabaaba   , which was compressed into the code sequence 0 2 1 4 5 3 7.
 
 1. To begin, we initialize the dictionary with the pairs (0, a) and (l, b), and obtain the first two entries in the dictionary above.
 2. The first code in the compressed file is 0. It is replaced by the text 'a'.
 3. The next code, 2, is undefined. Since the previous code 0 has text (0) = 'a', fc(0)='a' then text(2) = text(0)fc(0) = 'aa'.
 So, for code 2, 'aa' is output, and (2, 'aa') is entered into the dictionary.
 4. The next code, 1, is replaced by text(1) = ‘b’ and (3, text(2)fc(l) ) = (3, 'aab') is entered into the dictionary.
 5. The next code, 4, is not in the dictionary. The code preceding it is 1 and so text(4)= text(l)fc(l)= 'bb'.
 The pair (4, 'bb') is entered into the dictionary, and 'bb' is output to the decompressed file.
 6. When the next code, 5, is encountered, (5, 'bbb') is entered into the dictionary and 'bbb' is output to the decompressed file.
 7. The next code is 3 which is already in the dictionary so text(3) = 'aab' is output to the decompressed file, and the pair (6, text (5)fc(3)) = (6, 'bbba')
 is entered into the dictionary.
 8. Finally, when the code 7 is encountered, (7, text(3)fc(3)) = (7, 'aaba') is entered into the dictionary and ‘aaba' output.

 */

//The decompression program will read the input from a file called compout and will produce a file called decompout.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
int main()
{
//  CREATING A VECTOR OF STRING WITH SIZE EQUAL TO 4096 AND ALL INITIAL VALUES EQUAL TO AN EMPTY STRING IN ORDER THE DETECT IF A STRING WASNT COMPOSED FOR THAT CODE
    vector<string> codesAndValues(4096,"");
    
//  ADDING THE FIRST 256 SINGLE CHARECTERS TO THE VECTOR
    string currentChar;
    for (int i= 0; i <256; i++)
    {
        currentChar = (char) i;
        codesAndValues[i]=currentChar;
    }
    
//  CREATING IFSTREAM AND OFSTREAM OBJECTS IN ORDER TO READ THE CODES AND OUTPUT THE TEXTS OF THE CODES TO THE FILES. ALSO CREATING STRING VARIABLES FOR THE FILE NAMES AND OPENING THOSE FILES
    ifstream inFile;
    ofstream ofFile;
    string inputFileName = "compout.txt";
    string outputFileName = "decompout.txt";
    inFile.open(inputFileName.c_str());
    ofFile.open(outputFileName.c_str());
    
//  INTEGER VARIABLES IN ORDER TO STORE THE PREVIOUS READ CODE AND THE CURRENT READ CODE
    int oldCode;
    int newCode;

//  STRING VARIABLES IN ORDER TO STORE THE VALUE OF A GIVEN CODE AND TO STORE THE FIRST CHARECTER OF A GIVEN TEXT
    string valueOfTheText;
    string firstCharecterOfTheText;
    
//  COUNTER TO KEEP TRACK OF THE CODES TO BE GIVEN FOR THE NEW ENTRIES THE HASH TABLE/DICTIONARY
    int currentCode = 256;
    
//  READING THE FIRST CHARECTER AND OUTPUTTING THE TEXT OF THAT CODE
    inFile>>oldCode;
    ofFile<<codesAndValues[oldCode];
    
    while (inFile>>newCode) //UNTIL WE READ ALL OF THE CODES FROM THE FILE
    {
//      IF A TEXT IS NOT COMPOSED FOR THE CURRENCTLY READ CODE
        if (codesAndValues[newCode]=="")
        {
//          GETTING THE TEXT OF THE PREVIOUSLY READ CODE AND ALSO GETTING ITS FIRST CHARECTER
            valueOfTheText = codesAndValues[oldCode];
            firstCharecterOfTheText = valueOfTheText.substr(0,1);
            
//          COMPOSING A NEW TEXT EQAUL TO valueOfTheText + firstCharecterOfTheText WITH CODE EQUAL TO CURRENT-CODE AND INCREMENTING THE CURRENT-CODE BY ONE
            codesAndValues[currentCode] = valueOfTheText + firstCharecterOfTheText;
            currentCode++;
            
//          WRITING THE NEWLY COMPOSED TEXT VALUE TO THE OUTPUT FILE
            ofFile<<valueOfTheText + firstCharecterOfTheText;
            
//          WE ASSING THE newCode TO oldCode IN ORDER TO KEEP ON COMPOSING AND SEARCHING FOR NEW CODES AND VALUES
            oldCode = newCode;
        }
//      IF A TEXT IS COMPOSED FOR THE CURRENCTLY READ CODE
        else
        {
//          GETTING THE TEXT OF THE NEWLY READ CODE AND ALSO GETTING ITS FIRST CHARECTER AND WRITING THE TEXT OF THE NEWLY READED CODE TO THE OUTPUT FILE
            valueOfTheText = codesAndValues[newCode];
            ofFile<<valueOfTheText;
            firstCharecterOfTheText = valueOfTheText.substr(0,1);
            
//          COMPOSING A NEW TEXT EQAUL TO TEXT OF THE PREVIOUSLT READ CODE FIRST CHARECTER OF THE NEWLY READ CODE'S TEXT, WITH CODE EQUAL TO CURRENT-CODE AND INCREMENTING THE CURRENT-CODE BY ONE
            codesAndValues[currentCode] = codesAndValues[oldCode]+firstCharecterOfTheText;
            currentCode++;
            
//          WE ASSING THE newCode TO oldCode IN ORDER TO KEEP ON COMPOSING AND SEARCHING FOR NEW CODES AND VALUES
            oldCode = newCode;
        }
    }
    
//  CLOSING THE INPUT AND THE OUTPUT FILES
    inFile.close();
    ofFile.close();

    return 0;
}
