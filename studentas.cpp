#include "studentas.h"

void Studentas::apskaiciuotiGalutini() {
    if(!namuDarbai_.empty()){
        std::sort(namuDarbai_.begin(), namuDarbai_.end()); //išrikiuoti didėjimo tvarka
        size_t n = namuDarbai_.size();
        if (n % 2 == 1) {
            galutinisMed_ = namuDarbai_[n / 2]; //jei namų darbų skaičius yra nelyginis, mediana per vidurį
        } else {
            galutinisMed_ = (namuDarbai_[n / 2 - 1] + namuDarbai_[n / 2]) / 2.0; //jei ne, mediana dviejų vidurinių skaičių aritmetinis vidurkis
        }
        galutinisMed_ = (galutinisMed_ * 0.4) + (egz_ * 0.6);
        galutinisVid_ = (((double)sum_ / n) * 0.4) + (egz_ * 0.6);
    } else {
        galutinisMed_ = (egz_ * 0.6);
        galutinisVid_ = (egz_ * 0.6);
    }
}

Studentas::Studentas(std::istream& is) : Zmogus{} {
    nuskaitytiStudenta(is);
}

std::istream& Studentas::nuskaitytiStudenta(std::istream& is) {
    is >> vardas_ >> pavarde_;
    int pazymys;
    while (is >> pazymys) {
        namuDarbai_.push_back(pazymys);
        sum_ += pazymys;
    }
    if (!namuDarbai_.empty()) {
        egz_ = namuDarbai_.back();
        sum_ -= egz_;
        namuDarbai_.pop_back(); //pašalinti paskutinį pažymį, kadangi jis yra egzamino įvertinimas
    }
    apskaiciuotiGalutini();
    return is;
}

void Studentas::irasytiStudentuDuomenis(const std::string& failoPavadinimas, const std::vector<Studentas>& studentai) {
    std::ofstream rezultatuFailas(failoPavadinimas);
    if (rezultatuFailas.is_open()) {
        rezultatuFailas << "Pavardė       Vardas        Galutinis (Vid.)  Galutinis (Med.)\n";
        rezultatuFailas << "-----------------------------------------------------------------\n";
        for (const auto& studentas : studentai) {
            std::stringstream eilutesSrautas;
            eilutesSrautas << std::left << std::setw(14) << studentas.getPavarde() << std::setw(14) << studentas.getVardas()
                << std::setw(18) << std::fixed << std::setprecision(2) << studentas.getGalutinisVid()
                << std::setw(18) << studentas.getGalutinisMed() << '\n';
            rezultatuFailas << eilutesSrautas.str();
        }
        rezultatuFailas.close();
        std::cout << "Duomenys išsaugoti faile: " << failoPavadinimas << '\n';
    } else {
        std::cerr << "Nepavyko atidaryti failo rašymui: " << failoPavadinimas << '\n';
    }
}

void Studentas::rikiuotiStudentus(std::vector<Studentas>& studentai, int rikiavimoPasirinkimas) {
    switch (rikiavimoPasirinkimas) {
        case 1:
            std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.getVardas() < b.getVardas(); });
            break;
        case 2:
            std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.getPavarde() < b.getPavarde(); });
            break;
        case 3:
            std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.getGalutinisVid() > b.getGalutinisVid(); });
            break;
        case 4:
            std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.getGalutinisMed() > b.getGalutinisMed(); });
            break;
        default:
            std::cout << "Netinkamas pasirinkimas, naudojamas numatytasis (Vardas).\n";
            std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
                return a.getVardas() > b.getVardas(); });
    }
}
