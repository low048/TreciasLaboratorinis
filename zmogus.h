#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>

class Zmogus {
protected:
    std::string vardas_;
    std::string pavarde_;

public:
    //konstruktoriai
    Zmogus(const std::string& vardas, const std::string& pavarde) : vardas_{vardas}, pavarde_{pavarde} {}
    Zmogus() : vardas_{""}, pavarde_{""} {}

    //kopijavimo konstruktorius
    Zmogus(const Zmogus& other) : vardas_{other.vardas_}, pavarde_{other.pavarde_} {}

    //perkėlimo konstruktorius
    Zmogus(Zmogus&& other) noexcept : vardas_{std::move(other.vardas_)}, pavarde_{std::move(other.pavarde_)} {
        other.vardas_ = "";
        other.pavarde_ = "";
    }

    virtual ~Zmogus() {}
    
    //getteriai
    std::string getVardas() const { return vardas_; }
    std::string getPavarde() const { return pavarde_; }

    //setteriai
    void setVardas(const std::string& v) { vardas_ = v; }
    void setPavarde(const std::string& p) { pavarde_ = p; }

    //visiskai virtuali funkcija
    virtual void atspausdintiInformacija() const = 0;
};

#endif