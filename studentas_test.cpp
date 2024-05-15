#include "googletest/include/gtest/gtest.h"
#include <sstream>
#include <vector>
#include <iostream>
#include "studentas.h"

//patikrinamas konstruktorius bei išvedimo operatorius
TEST(StudentasTest, KonstruktoriusIrIsvedimas) {
    Studentas s1("Jonas", "Jonaitis", 9, {8, 9, 10}, 27);
    std::ostringstream os;
    os << s1;
    EXPECT_EQ(os.str(), "Jonas Jonaitis 9.00 9.00");
    std::cout << "Sukurtas s1: " << s1 << '\n';
}

//patikrinamas kopijavimo konstruktorius bei išvedimo operatorius
TEST(StudentasTest, KopijavimoKonstruktoriusIrIsvedimas) {
    Studentas s1("Jonas", "Jonaitis", 9, {8, 9, 10}, 27);
    Studentas s2(s1);
    std::ostringstream os;
    os << s2;
    EXPECT_EQ(os.str(), "Jonas Jonaitis 9.00 9.00");
    std::cout << "s1 nukopijuotas į s2: " << s2 << '\n';
}

//modifikuojamas s2 ir patikrinama, kad s1 liko nepakitęs
TEST(StudentasTest, Modifikacija) {
    Studentas s1("Jonas", "Jonaitis", 9, {8, 9, 10}, 27);
    Studentas s2(s1);
    s2.setVardas("Petras");
    std::ostringstream os1, os2;
    os1 << s1;
    os2 << s2;
    EXPECT_EQ(os1.str(), "Jonas Jonaitis 9.00 9.00"); 
    EXPECT_EQ(os2.str(), "Petras Jonaitis 9.00 9.00");
    std::cout << "Modifikuotas s2: " << s2 << '\n';
    std::cout << "s1 liko: " << s1 << '\n';
}

//patikrinamas kopijavimo priskyrimo operatorius bei išvedimo operatorius
TEST(StudentasTest, KopijavimoPriskyrimas) {
    Studentas s1("Jonas", "Jonaitis", 9, {8, 9, 10}, 27);
    Studentas s3;
    s3 = s1;
    std::ostringstream os;
    os << s3;
    EXPECT_EQ(os.str(), "Jonas Jonaitis 9.00 9.00");
    std::cout << "s1 priskirtas s3: " << s3 << '\n';
}

//patikrinamas perkėlimo konstruktorius bei išvedimo operatorius
TEST(StudentasTest, PerkelimoKonstruktorius) {
    Studentas s1("Jonas", "Jonaitis", 9, {8, 9, 10}, 27);
    Studentas s4(std::move(s1));
    std::ostringstream os;
    os << s4;
    EXPECT_EQ(os.str(), "Jonas Jonaitis 9.00 9.00");
    std::cout << "s1 perkeltas į s4: " << s4 << '\n';
    EXPECT_NO_THROW(std::ostringstream() << s1);
    std::cout << "s1 po perkėlimo: " << s1 << '\n';
}

//patikrinamas perkėlimo priskyrimo operatorius bei išvedimo operatorius
TEST(StudentasTest, PerkelimoPriskyrimas) {
    Studentas s2("Petras", "Petraitis", 8, {7, 8, 9}, 24);
    Studentas s5;
    s5 = std::move(s2);
    std::ostringstream os;
    os << s5;
    EXPECT_EQ(os.str(), "Petras Petraitis 8.00 8.00");  
    std::cout << "s2 perkeltas į s5: " << s5 << '\n';
    EXPECT_NO_THROW(std::ostringstream() << s2);
    std::cout << "s2 po perkėlimo: " << s2 << '\n';
}

//patikrinamas įvesties operatorius
TEST(StudentasTest, IvestiesOperatorius) {
    std::istringstream iss("Petras Petraitis 8 7 8 9 8");
    Studentas s6;
    iss >> s6;
    std::ostringstream os;
    os << s6;
    EXPECT_EQ(os.str(), "Petras Petraitis 8.00 8.00");
    std::cout << "Nuskaityta iš srauto į s6: " << s6 << '\n';
}