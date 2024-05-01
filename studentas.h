#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>  
#include <sstream>  
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "zmogus.h"

class Studentas : public Zmogus {
private:
    std::vector<int> namuDarbai_; ///< Namų darbų rezultatų vektorius
    int sum_ = 0; ///< Namų darbų rezultatų suma
    int egz_ = 0; ///< Egzamino rezultatas
    float galutinisVid_ = 0.0; ///< Galutinis balas (vidurkis)
    float galutinisMed_ = 0.0; ///< Galutinis balas (mediana)

public:
    /// Konstruktorius su parametrais
    Studentas(const std::string& vardas, const std::string& pavarde, int egz, const std::vector<int>& namuDarbai, int sum)
        : Zmogus{vardas, pavarde}, egz_{egz}, namuDarbai_{namuDarbai}, sum_{sum} {
        apskaiciuotiGalutini();
    }
    /// Konstruktorius be parametrų
    Studentas() : Zmogus{}, sum_{0}, egz_{0}, galutinisVid_{0.0}, galutinisMed_{0.0} {}
    /// Konstruktorius iš įvesties srauto
    Studentas(std::istream& is);

    /// Kopijavimo konstruktorius
    Studentas(const Studentas& other)
        : Zmogus{other}, namuDarbai_{other.namuDarbai_}, sum_{other.sum_}, egz_{other.egz_}, galutinisVid_{other.galutinisVid_}, galutinisMed_{other.galutinisMed_} {}

    /// Perkėlimo konstruktorius
    Studentas(Studentas&& other) noexcept
        : Zmogus{std::move(other)}, namuDarbai_{std::move(other.namuDarbai_)}, sum_{other.sum_}, egz_{other.egz_}, galutinisVid_{other.galutinisVid_}, galutinisMed_{other.galutinisMed_} {
            other.sum_ = 0;
            other.egz_ = 0;
            other.galutinisVid_ = 0.0;
            other.galutinisMed_ = 0.0;
        }

    /// Kopijavimo priskyrimo operatorius
    Studentas& operator=(const Studentas& other) {
        if (this != &other) {
            vardas_ = other.vardas_;
            pavarde_ = other.pavarde_;
            namuDarbai_ = other.namuDarbai_;
            sum_ = other.sum_;
            egz_ = other.egz_;
            galutinisVid_ = other.galutinisVid_;
            galutinisMed_ = other.galutinisMed_;
        }
        return *this;
    }

    /// Perkėlimo priskyrimo operatorius
    Studentas& operator=(Studentas&& other) noexcept {
        if (this != &other) {
            vardas_ = std::move(other.vardas_);
            pavarde_ = std::move(other.pavarde_);
            namuDarbai_ = std::move(other.namuDarbai_);
            sum_ = other.sum_;
            egz_ = other.egz_;
            galutinisVid_ = other.galutinisVid_;
            galutinisMed_ = other.galutinisMed_;
            other.sum_ = 0;
            other.egz_ = 0;
            other.galutinisVid_ = 0.0;
            other.galutinisMed_ = 0.0;
        }
        return *this;
    }

    /// Destruktorius
    ~Studentas() {
        namuDarbai_.clear();
    }

    /// Įvesties operatorius
    friend std::istream& operator>>(std::istream& is, Studentas& studentas) {
        return studentas.nuskaitytiStudenta(is);
    }

    /// Išvesties operatorius
    friend std::ostream& operator<<(std::ostream& os, const Studentas& studentas) {
        os << studentas.vardas_ << " " << studentas.pavarde_ << " " << studentas.galutinisVid_ << " " << studentas.galutinisMed_;
        return os;
    }

    // Getteriai
    std::vector<int> getNamuDarbai() const { return namuDarbai_; } ///< Grąžina namų darbų pažymių vektorių
    int getSum() const { return sum_; } ///< Grąžina pažymių sumą
    int getEgz() const { return egz_; } ///< Grąžina egzamino pažymį
    float getGalutinisVid() const { return galutinisVid_; } ///< Grąžina galutinį balą (vidurkis)
    float getGalutinisMed() const { return galutinisMed_; } ///< Grąžina galutinį balą (mediana)

    // Setteriai
    void setNamuDarbai(const std::vector<int>& nd) { namuDarbai_ = nd; } ///< Nustato namų darbų pažymių vektorių
    void setSum(int s) { sum_ = s; } ///< Nustato pažymių sumą
    void setEgz(int e) { egz_ = e; } ///< Nustato egzamino pažymį
    void setGalutinisVid(float gv) { galutinisVid_ = gv; } ///< Nustato galutinį balą (vidurkis)
    void setGalutinisMed(float gm) { galutinisMed_ = gm; } ///< Nustato galutinį balą (mediana)

    // Nario funckijos
    void apskaiciuotiGalutini(); ///< Apskaičiuoja galutinį balą (vidurkį ir medianą)
    std::istream& nuskaitytiStudenta(std::istream&); ///< Nuskaito studento informaciją iš įvesties srauto
    void atspausdintiInformacija() const override {
        std::cout << "Vardas: " << vardas_ << ", Pavarde: " << pavarde_ << std::endl;
    } ///< Atspausdina studento informaciją

    // Klasės funkcijos
    static void irasytiStudentuDuomenis(const std::string& failoPavadinimas, const std::vector<Studentas>& studentai); ///< Įrašo studento duomenis į failą
    static void rikiuotiStudentus(std::vector<Studentas>& studentai, int rikiavimoPasirinkimas); ///< Rikiuoja studentų vektorių pagal pasirinkimą
};

#endif
