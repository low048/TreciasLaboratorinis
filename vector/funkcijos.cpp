#include "funkcijos.h"

int patikrintiSkaiciu(int maziausias, int didziausias) {
    int skaicius;
    std::string ivestis;
    bool ivestisTeisinga = false;
    while (!ivestisTeisinga) {
        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }
        if (!std::getline(std::cin, ivestis)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignoruoti likusią eilutę
            std::cout << "Įvyko klaida skaitant įvestį, bandykite dar kartą: ";
            continue;
        }
        try {
            skaicius = std::stoi(ivestis);
            if (skaicius < maziausias || skaicius > didziausias) {
                throw std::out_of_range("Skaičius už leistino intervalo.");
            }
            ivestisTeisinga = true;
        } catch (const std::invalid_argument&) {
            std::cout << "Įvesta reikšmė nėra skaičius, bandykite dar kartą: ";
        } catch (const std::out_of_range&) {
            std::cout << "Įvestas skaičius yra už leistino intervalo, bandykite dar kartą: ";
        }
    }
    return skaicius;
}

bool patikrintiTaipNe() {
    std::string ivestis;
    while (true) {
        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }
        std::getline(std::cin, ivestis);
        if (ivestis == "t" || ivestis == "n") {
            return ivestis == "t";
        }
        std::cout << "Įvesta reikšmė nėra 't' arba 'n', bandykite dar kartą: ";
    }
}

int generuotiAtsitiktiniSkaiciu(int maziausias, int didziausias) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(maziausias, didziausias);
    
    return dis(gen);
}
