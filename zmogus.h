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

    virtual ~Zmogus() {}

    //visiskai virtuali funkcija
    virtual void atspausdintiInformacija() const = 0;
};

#endif