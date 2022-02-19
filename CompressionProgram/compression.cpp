// Written by : Ege Ata Ceylan
//This program is an implementation of a version of the Ziv-Lempel data compression algorithm, which is the basis for must popular compression programs such as winzip, zip or gzip.
/*
 This is how the algorithm operates: The Ziv-Lempel compression method maps strings of input characters into
 numeric codes. To begin with, all characters that may occur in the text file (that is, the alphabet) are assigned a code.
 
 For example, suppose the input file to be compressed has the string: aaabbbbbbaabaaba
 
 This string is composed of the characters 'a' and 'b'. Assuming our alphabet of symbols is just {a, b},
 initially 'a' is assigned the code 0 and 'b' the code l. The mapping between character strings and their codes
 is stored in a dictionary. Each dictionary entry has two fields: key and code. The character string represented
 by code is stored in the field key.
 The initial dictionary for our example is given by the first two columns below;
 
 Code 0  1
 Key  a  b
 
 Beginning with the dictionary initialized as above, the Ziv-Lempel compressor
 repeatedly finds the longest prefix, p, of the unencoded part of the input file that is
 in the dictionary and outputs its code. If there is a next character c in the input
 file, then pc (pc is the prefix string p followed by the character c) is assigned the
 next code and inserted into the dictionary. This strategy is called the Ziv-Lempel rule.
 
 Let us try the Ziv-Lempel method on our example string. The longest prefix of the input
 that is in the initial dictionary is 'a'. Its code, 0, is output, and the string 'aa'
 (p = ‘a’ and c = ‘a’) is assigned the code 2 and entered into the dictionary. 'aa' is the longest prefix of the
 remaining string that is in the dictionary. Its code, 2, is output; the string 'aab' (p = ‘aa’, c =’b’) is
 assigned the code 3 and entered into the dictionary. Notice that even though 'aab' has the code 3 assigned to
 it, only the code 2 for 'aa' is output. The suffix 'b' will be part of the next code output. The reason for not
 outputting 3 is that the code table is not part of the compressed file. Instead, the code table has to be
 reconstructed during decompression using the compressed file. This reconstruction is possible only if we adhere
 strictly to the Lempel-Ziv rule.
 
 Following the output of the code 2, the code for 'b' is output; 'bb' is assigned the code 4 and entered into the
 code dictionary. Then, the code for 'bb' is output, and 'bbb' is entered into the table with code 5. Next, the
 code 5 is output, and 'bbba' is entered with code 6. Then, the code 3 is output for 'aab', and 'aaba' is entered
 into the dictionary with code 7. Finally, the code 7 is output for the remaining string 'aaba'. Our sample
 string is thus encoded as the sequence of codes: 0 2 1 4 5 3 7.
 */

//WE ONLY GOING TO STORE MAXIMUM OF 4096 DISTINCT PAIRS IN THE HASH TABLE

//The compression program will read the input from a file called compin and will produce a file called compout.

#include <iostream>
#include <string>
#include <fstream>
#include "hash_Table.h"
using namespace std;

//CREATING A STRUCT IN ORDER TO STORE THE KEY AND CODE PAIRS IN THE HASH TABLE AND OVERLOADING THE == AND != OPERATORS SO THAt THE HASH TABLE FUNCTIONS WORK WITH THE STRUCT
struct keyCodePair{
    string key;
    int code;
    bool operator==(const keyCodePair &rhs) const
    {return (key==rhs.key);
    }
    bool operator!=(const keyCodePair &rhs) const
    {
        return !(*this==rhs);
    }
};

int main()
    {
//      CREATING AND ITEM_NOT_FOUND OBJECT FOR THE CONSTRUCTOR OF THE HASH TABLE
        keyCodePair notFound;
        notFound.key="itemSearchWasUnsuccessfull";
        notFound.code=-111;
        
//      CREATING A HASHTABLE WITH THE GIVEN ITEM_NOT_FOUND OBJECT AND THE SIZE
        HashTable<keyCodePair> wordDictionary =  HashTable<keyCodePair> (notFound,8190);
        

        string currentChar;
        //  ADDING THE FIRST 256 SINGLE CHARECTERS OF THE ASCII TABLE TO THE HASH TABLE
        for (int i= 0; i <256; i++)
        {
            currentChar = (char) i;
            keyCodePair temp;
            temp.code = i;
            temp.key = currentChar;
            wordDictionary.insert(temp);
        }

//      CREATING IFSTREAM AND OFSTREAM OBJECTS IN ORDER TO READ THE CHARECTER AND OUTPUT THE CODES TO THE FILES. ALSO CREATING STRING VARIABLES FOR THE FILE NAMES AND OPENING THOSE FILES
        ifstream inFile;
        ofstream ofFile;
        string inputFileName = "compin.txt";
        inFile.open(inputFileName.c_str());
        string outputFileName = "compout.txt";
        ofFile.open(outputFileName.c_str());
        
        
        char ch; //CHAR VARIABLE TO READ EACH CHAR FROM THE FILE ONE BY ONE
        
//      STRING VARIABLES TO STORE THE LAST LONGEST PREFIX AND ALSO TO TURN ch INTO A STRING VARIABLE
        string lastLongestPrefix;
        string currentReadCharecter;
        
        int currentCode = 256; // COUNTER TO KEEP TRACK OF THE CODES TO BE GIVEN FOR THE NEW ENTRIES TO THE HASH TABLE
        
//      STRUCTS VARIABLES IN ORDER TO THE SEARCHES ON THE HASH TABLE AND TO PRINT OUT THE CODES OF THE TEXTS
        keyCodePair lastLongestPrefixValue;
        keyCodePair tempEntry;
        
//      READING THE FIRST CHARECTER FROM THE FILE
        inFile.get(ch);
        lastLongestPrefix = ch;
        while (inFile.get(ch)) //UNTIL WE REACH THE END OF THE FILE
        {
//          SINCE WE ARE ALLOWED TO STORE ONLY 4096 DISTINCT PAIRS IN THE HASH TABLE IF THE CURRENT-CODE IS <= 4095 WE ADD NEW VALUES TO THE HASH TABLE
            if( currentCode <= 4095){
                
            currentReadCharecter = ch;
            tempEntry.key = lastLongestPrefix + currentReadCharecter;
            tempEntry.code = -1;
//          IF lastLongestPrefix + currentReadCharecter IS IN THE HASH TABLE
            if (wordDictionary.find(tempEntry)!=notFound)
            {
                lastLongestPrefix = lastLongestPrefix + currentReadCharecter;
            }
//          IF lastLongestPrefix + currentReadCharecter IS NOT IN THE HASH TABLE
            else
            {
//              WE GET THE VALUE OF THE LAST LONGEST PREFIX FROM THE HASH TABLE AND WRITE ITS CODE TO THE OUTPUT FILE
                tempEntry.key = lastLongestPrefix;
                lastLongestPrefixValue = wordDictionary.find(tempEntry);
                ofFile<< lastLongestPrefixValue.code<<" ";
                
//              WE ADD A NEW PAIR WITH THE KEY EQUAL TO lastLongestPrefix + currentReadCharecter AND INCREMENT THE CURRENT-CODE
                tempEntry.key = lastLongestPrefix + currentReadCharecter;
                tempEntry.code = currentCode;
                currentCode++;
                wordDictionary.insert(tempEntry);
                
//              WE ASSING THE currentReadCharecter TO lastLongestPrefix IN ORDER TO KEEP ON SEARCHING IN THE GIVEN TEXT
                lastLongestPrefix = currentReadCharecter;
            }
            currentReadCharecter="";
            }
//          IF THE CURRENT CODE IS BIGGER THAN 4095, WHICH MEANS THAT WE CANNOT CREATE ANY NEW PAIRS TO STORE IN THE HASH TABLE
            else
            {
                currentReadCharecter = ch;
                tempEntry.key = lastLongestPrefix + currentReadCharecter;
                tempEntry.code = -1;

//              IF lastLongestPrefix + currentReadCharecter IS IN THE HASH TABLE
                if (wordDictionary.find(tempEntry)!=notFound)
                {
                    lastLongestPrefix = lastLongestPrefix + currentReadCharecter;
                }
//              IF lastLongestPrefix + currentReadCharecter IS NOT IN THE HASH TABLE
                else
                {
//                  WE GET THE VALUE OF THE LAST LONGEST PREFIX FROM THE HASH TABLE AND WRITE ITS CODE TO THE OUTPUT FILE
                    tempEntry.key = lastLongestPrefix;
                    lastLongestPrefixValue = wordDictionary.find(tempEntry);
                    ofFile<< lastLongestPrefixValue.code<<" ";
//                  WE ASSING THE currentReadPreCharecter TO lastLongestPrefix  IN ORDER TO KEEP ON SEARCHING IN THE GIVEN TEXT
                    lastLongestPrefix = currentReadCharecter;
                }
                currentReadCharecter="";
            }
            
        }

//      WE GET THE VALUE OF THE LAST LONGEST PREFIX FROM THE HASH TABLE AND WRITE ITS CODE TO THE OUTPUT FILE
        tempEntry.key = lastLongestPrefix;
        lastLongestPrefixValue = wordDictionary.find(tempEntry);
        ofFile<< lastLongestPrefixValue.code<< " "<<endl;
        
//      CLOSING THE INPUT AND THE OUTPUT FILE
        inFile.close();
        ofFile.close();
        
        
    return 0;
}
