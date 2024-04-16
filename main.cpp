#include "studentas.h"
#include "funkcijos.h"
#include "timer.h"

int main() {
    std::vector<Studentas> studentai;
    int meniuPasirinkimas = 0;
    double timeSum;
    do {
        if(meniuPasirinkimas != 0) {
            std::cout << std::endl; //palikti tarpą po kiekvieno meniu pasirinkimo, kad būtų lengviau skaityti programos eigą
        }
        std::cout << "Meniu:\n"
            << "1 - Įvesti duomenis ranka\n"
            << "2 - Generuoti pažymius\n"
            << "3 - Generuoti pažymius ir studentų vardus, pavardes\n"
            << "4 - Nuskaityti studentų duomenis iš failo\n"
            << "5 - Generuoti atsitiktinį studentų sąrašo failą\n"
            << "6 - Rūšiuoti studentus pagal nepatenkinamus ir patenkinamus galutinius įvertinimus\n"
            << "7 - Baigti darbą\n"
            << "Pasirinkimas: ";
        meniuPasirinkimas = patikrintiSkaiciu(1, 7);
        switch(meniuPasirinkimas) {
            case 1: {
                //duomenys įvedami ranka
                std::string vardas, pavarde;
                int egz, pazymys;
                std::vector<int> namuDarbai;
                std::cout << "  Įveskite studento vardą: ";
                std::cin >> vardas;
                std::cout << "  Įveskite studento pavardę: ";
                std::cin >> pavarde;
                std::cout << "  Įveskite studento egzamino rezultatą: ";
                egz = patikrintiSkaiciu(0, 10);
                int sum = 0;
                while (true) {
                    std::cout << "    Įveskite #" << namuDarbai.size() + 1 << " namų darbo rezultatą (0-10), -1 baigia įvedimą: ";
                    pazymys = patikrintiSkaiciu(-1, 10);
                    if (pazymys == -1)
                        break;
                    namuDarbai.push_back(pazymys);
                    sum += pazymys;
                }
                studentai.push_back(Studentas(vardas, pavarde, egz, namuDarbai, sum));
                break;
            }
            case 2: {
                //atsitiktinai generuojami tik pažymiai
                std::string vardas, pavarde;
                int egz, pazymys;
                std::vector<int> namuDarbai;
                std::cout << "  Įveskite studento vardą: ";
                std::cin >> vardas;
                std::cout << "  Įveskite studento pavardę: ";
                std::cin >> pavarde;
                std::cout << "  Įveskite studento egzamino rezultatą: ";
                egz = patikrintiSkaiciu(0, 10);
                int sum = 0;
                std::cout << "  Įveskite namų darbų skaičių: ";
                int n = patikrintiSkaiciu(0, std::numeric_limits<int>::max());
                for (int v = 0; v < n; v++) {
                    pazymys = generuotiAtsitiktiniSkaiciu(1, 10);
                    namuDarbai.push_back(pazymys);
                    sum += pazymys;
                }
                studentai.push_back(Studentas(vardas, pavarde, egz, namuDarbai, sum));
                break;
            }
            case 3: {
                //atsitiktinai generuojami studento vardas, pavardė bei pažymiai
                std::cout << "  Įveskite studentų skaičių: ";
                int studentuSkaicius = patikrintiSkaiciu(0, std::numeric_limits<int>::max());
                std::cout << "  Įveskite namų darbų skaičių: ";
                int namuDarbuSkaicius = patikrintiSkaiciu(0, std::numeric_limits<int>::max());
                for (int i = 0; i < studentuSkaicius; i++) {
                    std::string vardas = "Vardas" + std::to_string(i + 1);
                    std::string pavarde = "Pavarde" + std::to_string(i + 1);
                    int egz = generuotiAtsitiktiniSkaiciu(1, 10);
                    std::vector<int> namuDarbai;
                    int sum = 0;
                    for (int v = 0; v < namuDarbuSkaicius; v++) {
                        int pazymys = generuotiAtsitiktiniSkaiciu(1, 10);
                        namuDarbai.push_back(pazymys);
                        sum += pazymys;
                    }
                    studentai.push_back(Studentas(vardas, pavarde, egz, namuDarbai, sum));
                }
                break;
            }
            case 4: {
                //failo nuskaitymas
                std::string failoPavadinimas;
                std::cout << "Galimi failai: " << std::flush;
                system("dir *.txt");
                std::cout << "  Įveskite failo pavadinimą: ";
                std::cin >> failoPavadinimas;
                Timer t;
                try {
                    std::ifstream file(failoPavadinimas);
                    if (!file.is_open())
                        throw std::runtime_error("nepavyko atidaryti failo " + failoPavadinimas);

                    std::string eilute;
                    while (std::getline(file, eilute)) {
                        try {
                            std::istringstream eilutesSrautas(eilute);
                            studentai.push_back(Studentas(eilutesSrautas));
                        } catch (std::exception& e) {
                            std::cout << "Nuskaitant eilutę įvyko klaida: " << e.what() << '\n';
                        }
                    }
                    file.close();
                } catch (std::exception& e) {
                    std::cout << "Nuskaitant failą įvyko klaida: " << e.what() << '\n';
                }
                std::cout << "Failo nuskaitymas užtruko " << t.elapsed() << " s\n";
                timeSum += t.elapsed();
                break;
            }
            case 5: {
                //atsitiktinio studentų sąrašo failo generavimas
                std::cout << "Įveskite studentų skaičių: ";
                int studentuSkaicius = patikrintiSkaiciu(1, std::numeric_limits<int>::max());
                std::cout << "Įveskite namų darbų skaičių: ";
                int namuDarbuSkaicius = patikrintiSkaiciu(1, std::numeric_limits<int>::max());
                std::string failoPavadinimas = "studentai_" + std::to_string(studentuSkaicius) + ".txt";
                Timer t;
                std::ofstream rezultatuFailas(failoPavadinimas);
                if (rezultatuFailas.is_open()) {
                    std::stringstream eilutesSrautas;
                    eilutesSrautas << std::left << std::setw(20) << "Vardas" << std::right << std::setw(20) << "Pavarde";
                    for (int i = 0; i < namuDarbuSkaicius; i++) {
                        eilutesSrautas << std::setw(10) << "ND" + std::to_string(i + 1);
                    }
                    eilutesSrautas << std::setw(10) << "Egz." << std::endl;
                    for (int i = 0; i < studentuSkaicius; i++) {
                        std::string vardas = "Vardas" + std::to_string(i + 1);
                        std::string pavarde = "Pavarde" + std::to_string(i + 1);
                        eilutesSrautas << std::left << std::setw(20) << vardas;
                        eilutesSrautas << std::right << std::setw(20) << pavarde;
                        for (int v = 0; v < namuDarbuSkaicius; v++) {
                            int pazymys = generuotiAtsitiktiniSkaiciu(1, 10);
                            eilutesSrautas << std::setw(10) << pazymys;
                        }
                        int egzaminas = generuotiAtsitiktiniSkaiciu(1, 10);
                        eilutesSrautas << std::setw(10) << egzaminas << std::endl;
                    }
                    rezultatuFailas << eilutesSrautas.str();
                    rezultatuFailas.close();
                    std::cout << "Duomenys išsaugoti faile: " << failoPavadinimas << '\n';
                    std::cout << "Atsitiktinio studentų sąrašo failo kūrimas užtruko " << t.elapsed() << " s\n";
                } else {
                    std::cout << "Nepavyko atidaryti failo rašymui: " << failoPavadinimas << '\n';
                }
                break;
            }
            case 6: {
                //studentų rūšiavimas pagal galutinius įvertinimus
                std::cout << "Pasirinkite strategiją:\n1 - Pirmoji strategija\n2 - Antroji strategija\n3 - Trečioji strategija\nPasirinkimas: ";
                int strategija = patikrintiSkaiciu(1, 3);
                Timer t;
                std::vector<Studentas> nepatenkinami;
                switch (strategija) {
                    case 1: {
                        std::vector<Studentas> patenkinami;
                        for (const auto& studentas : studentai) {
                            if (studentas.getGalutinisVid() < 5) {
                                nepatenkinami.push_back(studentas);
                            } else {
                                patenkinami.push_back(studentas);
                            }
                        }
                        studentai.swap(patenkinami);
                        break;
                    }
                    case 2: {
                        Studentas::rikiuotiStudentus(studentai, 3);
                        if (!studentai.empty()) {
                            for (auto it = studentai.end(); it != studentai.begin();) {
                                --it;
                                if (it->getGalutinisVid() < 5) {
                                    nepatenkinami.push_back(std::move(*it));
                                    it = studentai.erase(it);
                                }
                            }
                        }
                        break;
                    }
                    case 3: {
                        auto it = std::partition(studentai.begin(), studentai.end(), [](const Studentas& s) { return s.getGalutinisVid() >= 5; });
                        nepatenkinami.assign(std::make_move_iterator(it), std::make_move_iterator(studentai.end()));
                        studentai.erase(it, studentai.end());
                        break;
                    }
                }
                std::cout << "Rūšiavimas į dvi grupes užtruko " << t.elapsed() << " s\n";
                timeSum += t.elapsed();

                std::cout << "Įveskite surūšiuotų studentų .txt failo pavadinimą: ";
                std::string failoPavadinimas;
                std::cin >> failoPavadinimas;

                std::cout << "Ar norite rikiuoti visų studentų sąrašą? (t/n): ";
                bool arRikiuoti = patikrintiTaipNe();
                if (arRikiuoti) {
                    std::cout << "Rikiuoti studentus pagal:\n1 - Vardą\n2 - Pavardę\n3 - Galutinį (Vid.)\n4 - Galutinį (Med.)\nPasirinkimas: ";
                    int rikiavimoPasirinkimas = patikrintiSkaiciu(1, 4);
                    Timer t2;
                    Studentas::rikiuotiStudentus(nepatenkinami, rikiavimoPasirinkimas);
                    Studentas::rikiuotiStudentus(studentai, rikiavimoPasirinkimas);
                    std::cout << "Rikiavimas užtruko " << t2.elapsed() << " s\n";
                    timeSum += t2.elapsed();
                }
                
                Timer t3;
                Studentas::irasytiStudentuDuomenis(failoPavadinimas + "_nepatenkinami.txt", nepatenkinami);
                std::cout << "Nepatenkinamų studentų įrašymas užtruko " << t3.elapsed() << " s\n";
                timeSum += t3.elapsed();

                Timer t4;
                Studentas::irasytiStudentuDuomenis(failoPavadinimas + "_patenkinami.txt", studentai);
                std::cout << "Patenkinamų studentų įrašymas užtruko " << t4.elapsed() << " s\n";
                timeSum += t4.elapsed();

                std::cout << "Jungiami studentų konteineriai...\n";
                std::move(nepatenkinami.begin(), nepatenkinami.end(), std::back_inserter(studentai));

                std::cout << "Išvis sugaišta laiko (be įvesties): " << timeSum << " s\n";
                break;
            }
            case 7: {
                //darbo baigimas, rikiavimas
                std::cout << "Ar norite rikiuoti visų studentų sąrašą? (t/n): ";
                bool arRikiuoti = patikrintiTaipNe();
                if (arRikiuoti) {
                    std::cout << "Rikiuoti studentus pagal:\n1 - Vardą\n2 - Pavardę\n3 - Galutinį (Vid.)\n4 - Galutinį (Med.)\nPasirinkimas: ";
                    int rikiavimoPasirinkimas = patikrintiSkaiciu(1, 4);
                    Timer t;
                    Studentas::rikiuotiStudentus(studentai, rikiavimoPasirinkimas);
                    std::cout << "Rikiavimas užtruko " << t.elapsed() << " s\n";
                    timeSum += t.elapsed();
                }
                break;
            }
            default:
                std::cout << "Netinkamas pasirinkimas, bandykite iš naujo.\n";
        }
    } while(meniuPasirinkimas != 7);
    std::cout << "Išvesti į:\n1 - Konsolę\n2 - Failą\nPasirinkimas: ";
    int isvestiesPasirinkimas = patikrintiSkaiciu(1, 2);
    if (isvestiesPasirinkimas == 2) {
        std::cout << "Įveskite rezultato .txt failo pavadinimą: ";
        std::string failoPavadinimas;
        std::cin >> failoPavadinimas;
        Timer t;
        Studentas::irasytiStudentuDuomenis(failoPavadinimas + ".txt", studentai);
        timeSum += t.elapsed();
        std::cout << "Išvis sugaišta laiko (be įvesties): " << timeSum << " s\n";
    } else {
        std::cout << "Pavardė       Vardas        Galutinis (Vid.)  Galutinis (Med.)\n";
        std::cout << "-----------------------------------------------------------------\n";
        for (const auto& studentas : studentai) {
            std::cout << std::left << std::setw(14) << studentas.getPavarde() << std::setw(14) << studentas.getVardas()
                      << std::setw(18) << std::fixed << std::setprecision(2) << studentas.getGalutinisVid()
                      << std::setw(18) << studentas.getGalutinisMed() << '\n';
        }
    }
    std::cin.get();
    std::cin.get();
    return 0;
}
