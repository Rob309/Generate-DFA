#include <iostream>
#include "PolishForm.h"
#include "DeterministicFiniteAutomaton.h"
#include "NedeterministicFiniteAutomaton.h"
#include <fstream>
#include <regex>


bool isValidRegex(const std::string& regex) {
    try {
        std::regex compiledRegex(regex);
        return true;
    }
    catch (const std::regex_error&) {
        return false;
    }
}

std::string readRegexFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string regex;
    if (file.is_open()) {
        std::getline(file, regex);
        file.close();
    }
    return regex;
}

void mainMenu(DeterministicFiniteAutomaton& dfa, const std::string& regex) {
    int choice;
    do {
        std::cout << "\nMeniu:\n";
        std::cout << "1. Afisare expresie regulata\n";
        std::cout << "2. Afisare automat finit determinist\n";
        std::cout << "3. Verificare cuvant\n";
        std::cout << "4. Iesire\n";
        std::cout << "Alegeti o optiune: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Expresia regulata: " << regex << std::endl;
            break;
        case 2:
            dfa.PrintAutomaton();
            {
                std::ofstream outFile("automaton_output.txt");
                if (outFile.is_open()) {
                    outFile << dfa;
                    outFile.close();
                    std::cout << "Automatul a fost salvat in 'automaton_output.txt'.\n";
                }
                else {
                    std::cerr << "Eroare la deschiderea fisierului de iesire.\n";
                }
            }
            break;
        case 3: {
            std::string word;
            std::cout << "Introduceti un cuvant: ";
            std::cin >> word;
            if (dfa.CheckWord(word)) {
                std::cout << "Cuvantul '" << word << "' este acceptat.\n";
            }
            else {
                std::cout << "Cuvantul '" << word << "' NU este acceptat.\n";
            }
            break;
        }
        case 4:
            std::cout << "Iesire...\n";
            break;
        default:
            std::cout << "Optiune invalida. Incercati din nou.\n";
        }
    } while (choice != 4);
}

int main() {
    std::string filename = "regex_input.txt"; 
    std::string regex = readRegexFromFile(filename);

    if (regex.empty()) {
        std::cerr << "Nu s-a putut citi expresia regulata din fișier.\n";
        return 1;
    }

    if (isValidRegex(regex)) {
        std::cout << "Expresia regulata este valida.\n";
        
        PolishForm poland;
        NondeterministicFiniteAutomaton ned;

        string polishForm = poland.convertToPostfix(regex);

        NFA nfa = ned.createNFAFromPostfix(polishForm);

        DeterministicFiniteAutomaton dfa = dfa.convertNFAtoDFA(nfa);

        string error;
        dfa.VerifyAutomation(error);
        cout << error;


        mainMenu(dfa, regex);
    }
    else {
        std::cerr << "Expresia regulata NU este valida.\n";
    }

	return 0;
}

