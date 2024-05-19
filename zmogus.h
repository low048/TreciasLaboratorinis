#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>

class Zmogus {
protected:
    std::string vardas_; ///< Vardas
    std::string pavarde_; ///< Pavardė

public:
    /// Konstruktorius su parametrais
    Zmogus(const std::string& vardas, const std::string& pavarde) : vardas_{vardas}, pavarde_{pavarde} {}
    /// Konstruktorius be parametrų
    Zmogus() : vardas_{""}, pavarde_{""} {}

    /// Kopijavimo konstruktorius
    Zmogus(const Zmogus& other) : vardas_{other.vardas_}, pavarde_{other.pavarde_} {}

    /// Perkėlimo konstruktorius
    Zmogus(Zmogus&& other) noexcept : vardas_{std::move(other.vardas_)}, pavarde_{std::move(other.pavarde_)} {
        other.vardas_ = "";
        other.pavarde_ = "";
    }

    /// Destruktorius
    virtual ~Zmogus() {}

    // Getteriai
    std::string getVardas() const { return vardas_; } ///< Grąžina vardą
    std::string getPavarde() const { return pavarde_; } ///< Grąžina pavardę

    // Setteriai
    void setVardas(const std::string& v) { vardas_ = v; } ///< Nustato vardą
    void setPavarde(const std::string& p) { pavarde_ = p; } ///< Nustato pavardę

    /// Visiškai virtuali funkcija
    virtual void atspausdintiInformacija() const = 0;
};

#endif
