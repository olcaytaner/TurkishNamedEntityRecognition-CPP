//
// Created by Olcay Taner Yıldız on 18.01.2019.
//

#include <sstream>
#include "NamedEntityWord.h"
#include "NamedEntitySentence.h"
#include "NamedEntityType.h"

NamedEntitySentence::NamedEntitySentence() {
}

NamedEntitySentence::NamedEntitySentence(string sentence) : Sentence(sentence) {
    string entityType, candidate;
    NamedEntityType type = NamedEntityType::NONE;
    istringstream iss(sentence);
    vector<string> wordArray(istream_iterator<string>{iss}, istream_iterator<string>());
    for (const string& word : wordArray){
        if (!word.empty()){
            if (word != "<b_enamex"){
                if (word.rfind("TYPE=\"", 0) == 0){
                    int typeIndexEnd = word.find_first_of('\"', 6);
                    if (typeIndexEnd != -1){
                        entityType = word.substr(6, typeIndexEnd - 6);
                        type = getNamedEntityType(entityType);
                    }
                    if (word.find("e_enamex>", word.size() - 9) == word.size() - 9){
                        candidate = word.substr(word.find_first_of('>') + 1, word.find_first_of('<') - word.find_first_of('>') - 1);
                        if (!candidate.empty()){
                            words.emplace_back(NamedEntityWord(candidate, type));
                        }
                        type = NamedEntityType::NONE;
                    } else {
                        candidate = word.substr(word.find_first_of('>') + 1);
                        if (!candidate.empty()){
                            words.emplace_back(NamedEntityWord(candidate, type));
                        }
                    }
                } else {
                    if (word.find("e_enamex>", word.size() - 9) == word.size() - 9){
                        candidate = word.substr(0, word.find_first_of('<'));
                        if (!candidate.empty()){
                            words.emplace_back(NamedEntityWord(candidate, type));
                        }
                        type = NamedEntityType::NONE;
                    } else {
                        if (!word.empty()){
                            words.emplace_back(NamedEntityWord(word, type));
                        }
                    }
                }
            }
        }
    }
}
