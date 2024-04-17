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

class Studentas {
private:
    std::string vardas_;
    std::string pavarde_;
    std::vector<int> namuDarbai_;
    int sum_ = 0; //namų darbų rezultatų suma
    int egz_ = 0; //egzamino rezultatas
    float galutinisVid_ = 0.0;
    float galutinisMed_ = 0.0;

public:
    //konstruktoriai
    Studentas(const std::string& vardas, const std::string& pavarde, int egz, const std::vector<int>& namuDarbai, int sum)
        : vardas_{vardas}, pavarde_{pavarde}, egz_{egz}, namuDarbai_{namuDarbai}, sum_{sum} {
        apskaiciuotiGalutini();
    }
    Studentas() : vardas_{""}, pavarde_{""}, sum_{0}, egz_{0}, galutinisVid_{0.0}, galutinisMed_{0.0} {}
    Studentas(std::istream& is);

    //kopijavimo konstruktorius
    Studentas(const Studentas& other)
        : vardas_{other.vardas_}, pavarde_{other.pavarde_}, namuDarbai_{other.namuDarbai_}, sum_{other.sum_}, egz_{other.egz_}, galutinisVid_{other.galutinisVid_}, galutinisMed_{other.galutinisMed_} {}

    //perkėlimo konstruktorius
    Studentas(Studentas&& other) noexcept
        : vardas_{std::move(other.vardas_)}, pavarde_{std::move(other.pavarde_)}, namuDarbai_{std::move(other.namuDarbai_)}, sum_{other.sum_}, egz_{other.egz_}, galutinisVid_{other.galutinisVid_}, galutinisMed_{other.galutinisMed_} {
            other.sum_ = 0;
            other.egz_ = 0;
            other.galutinisVid_ = 0.0;
            other.galutinisMed_ = 0.0;
        }
    
    //kopijavimo priskyrimo operatorius
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

    //perkėlimo priskyrimo operatorius
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

    //destruktorius
    ~Studentas() {
        namuDarbai_.clear();
    }

    //įvesties operatorius
    friend std::istream& operator>>(std::istream& is, Studentas& studentas) {
        return studentas.nuskaitytiStudenta(is);
    }

    //išvesties operatorius
    friend std::ostream& operator<<(std::ostream& os, const Studentas& studentas) {
        os << studentas.vardas_ << " " << studentas.pavarde_ << " " << studentas.galutinisVid_ << " " << studentas.galutinisMed_;
        return os;
    }

    //getteriai
    std::string getVardas() const { return vardas_; }
    std::string getPavarde() const { return pavarde_; }
    std::vector<int> getNamuDarbai() const { return namuDarbai_; }
    int getSum() const { return sum_; }
    int getEgz() const { return egz_; }
    float getGalutinisVid() const { return galutinisVid_; }
    float getGalutinisMed() const { return galutinisMed_; }

    //setteriai
    void setVardas(const std::string& v) { vardas_ = v; }
    void setPavarde(const std::string& p) { pavarde_ = p; }
    void setNamuDarbai(const std::vector<int>& nd) { namuDarbai_ = nd; }
    void setSum(int s) { sum_ = s; }
    void setEgz(int e) { egz_ = e; }
    void setGalutinisVid(float gv) { galutinisVid_ = gv; }
    void setGalutinisMed(float gm) { galutinisMed_ = gm; }

    //nario funckijos
    void apskaiciuotiGalutini();
    std::istream& nuskaitytiStudenta(std::istream&);

    //klasės funkcijos
    static void irasytiStudentuDuomenis(const std::string& failoPavadinimas, const std::vector<Studentas>& studentai);
    static void rikiuotiStudentus(std::vector<Studentas>& studentai, int rikiavimoPasirinkimas);
};

#endif
