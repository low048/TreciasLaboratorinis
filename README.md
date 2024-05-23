<details>
<summary>Instaliavimo iš setup.msi instrukcija</summary>
<br>

- Parsisiųskite setup.msi iš projekto "releases" puslapio.
- Atidarykite parsisiųstą failą.
- Pasirinkite, kur norite instaliuoti programą.
- Sekite instrukcijas ekrane.

</details>

<details>
<summary>Pirminio kodo kompiliavimo instrukcija</summary>

## Projekto persikėlimas į savo kompiuterį

- Instaliuokite GitHub Desktop.
- Atsidarykite projekto [repozitorijos puslapį](https://github.com/low048/PirmasLaboratorinis).
- Paspauskite Code mygtuką ir pasirinkite Open with GitHub Desktop.
- Pasirinkite, kur norite išsaugoti projekto kopiją.
- Paspauskite Clone.


## Kompiliatorius

- Parsisiųskite [Cygwin](https://www.cygwin.com/setup-x86_64.exe).
- Instaliuokite Cygwin nepasirinkdami jokių pakuočių (packages), bet neištrinkite `setup-x86_64.exe`.
- Atidarykite cmd, ir įvykdykite šias komandas:
    - `cd C:\failo\vieta\kompiuteryje` (pvz. "cd C:\Users\User\Downloads", kuriame yra setup-x86_64.exe).
    - `setup-x86_64.exe -q -P wget -P gcc-g++ -P make -P diffutils -P libmpfr-devel -P libgmp-devel -P libmpc-devel`
- Palaukite kol užsidarys naujai atidarytas langas, tada bus įdiegtos visos reikiamos pakuotės programos paleidimui.

## Projekto kompiliavimas

- Atsidarykite `Cygwin64 Terminal`, tada jame įveskite:
    - `cd "C:\projekto\vieta\kompiuteryje"` (pvz. "cd C:\Users\User\Desktop\PirmasLaboratorinis", kuriame yra projekto kopija).
<br>Galimų tolesnių komandų sąrašas:
    - `make program` - jei norite kompiliuoti programą
    - `make test` - jei norite kompiliuoti "unit tests"
    - `make examples` - jei norite kompiliuoti Vector veikimo pavyzdžius
    - `make all` - jei norite kompiliuoti visas anksčiau minėtas versijas
    - `make clean`- jei norite ištrinti visas kompiliuotas versijas

</details>

<details>
<summary>Programos naudojamas</summary>
<br>

- Atsidarykite naujai instaliuotą ar sukompiliuotą programą.
- Sekite instrukcijas, kurios rodomos konsolės langelyje.

Programos paleidimo pavyzdys:
![pvz](https://raw.githubusercontent.com/low048/AntrasLaboratorinis/V1.1/TyrimuNuotraukos/PenktasTyrimas/struct/O3/2.png)


</details>

<details open>
<summary>Testavimas</summary>
<br>

Kompiuterio parametrai: 
- Ryzen 5 6600HS
- NVME SSD
- 16 GB LPDDR5 6400 MHz RAM

Kiekvieno programos paleidimo tyrimų metu nuotraukas galite matyti [čia](https://github.com/low048/PirmasLaboratorinis/tree/V1.0/TyrimuNuotraukos/).

Testavimas buvo atliktas su failais:
- `studentai_1000.txt` - 1k stud., 15 n.d., 197 KB (iš savos atsitiktinių studentų failo generavimo funkcijos)
- `studentai10000.txt` - 10k stud., 15 n.d., 2,022 KB (iš anksto generuotas, patalpintas VMA)
- `studentai100000.txt` - 100k stud., 20 n.d., 25,098 KB (iš anksto generuotas, patalpintas VMA)
- `studentai1000000.txt` - 1 mil. stud., 7 n.d., 124,024 KB (iš anksto generuotas, patalpintas VMA)
- `studentai_10000000.txt` - 10 mil. stud., 15 n.d., 1,962,891 KB (iš savos atsitiktinių studentų failo generavimo funkcijos)

# Pakeitimai (v3.0)

- Sukurta sava Vector klasė, kaip std::vector alternatyva.

## Kai kurių funkcijų aprašymai

### `emplace_back`

Ši funkcija sukuria elementą vektoriaus pabaigoje ir grąžina nuorodą į jį. 

**Parametrai:** 
- `Args&&... args` - Kintamųjų skaičius argumentų, kurie perduodami naujo objekto konstruktoriui.

**Grąžinamas tipas:** `reference` - Nuoroda į naujai sukurtą elementą.

<details>
<summary>Veikimo pavyzdys</summary>
<br>

```cpp
#include <vector>
#include <cassert>
#include <iostream>
#include <string>
 
struct President
{
    std::string name;
    std::string country;
    int year;
 
    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        std::cout << "I am being constructed.\n";
    }
 
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        std::cout << "I am being moved.\n";
    }
 
    President& operator=(const President& other) = default;
};
 
int main()
{
    std::vector<President> elections;
    std::cout << "emplace_back:\n";
    auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
    assert(ref.year == 1994 && "uses a reference to the created object (C++17)");
 
    std::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));
 
    std::cout << "\nContents:\n";
    for (President const& president: elections)
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";
 
    for (President const& president: reElections)
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";
}
```

Išvestis:

```
std::vector<T,Allocator>::push_back Example
std::vector letters_str holds: "abc" "def"
Moved-from string s holds: ""

std::vector<T,Allocator>::emplace_back Example
emplace_back:
I am being constructed.

push_back:
I am being constructed.
I am being moved.
```
</details>

### `insert_range`

Ši funkcija įterpia elementus iš nurodyto intervalo į vektoriaus nurodytą poziciją.

**Parametrai:**
- `const_iterator pos` - Pozicija, į kurią bus įterpiami elementai.
- `R&& rg` - Intervalas, iš kurio bus kopijuojami elementai.

**Grąžinamas tipas:** `iterator` - Iteratorius į pirmą įterptą elementą.

<details>
<summary>Veikimo pavyzdys</summary>
<br>

```cpp
#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <list>
 
int main()
{
    auto container = std::vector{1, 2, 3, 4};
    auto pos = std::next(container.begin(), 2);
    assert(*pos == 3);
    const auto rg = std::list{-1, -2, -3};
 
#ifdef __cpp_lib_containers_ranges
    container.insert_range(pos, rg);
#else
    container.insert(pos, rg.cbegin(), rg.cend());
#endif
    assert(std::ranges::equal(container, std::vector{1, 2, -1, -2, -3, 3, 4}));
}
```
</details>

### `erase_if`

Ši funkcija ištrina visus elementus, kurie atitinka nurodytą sąlygą.

**Parametrai:**
- `Pred pred` - Predikatas, kuris nusprendžia, ar elementas turi būti ištrintas.

**Grąžinamas tipas:** `size_type` - Ištrintų elementų skaičius.

<details>
<summary>Veikimo pavyzdys</summary>
<br>

```cpp
#include <complex>
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>
 
void println(std::string_view comment, const auto& c)
{
    std::cout << comment << "{ ";
    for (const auto& x : c)
        std::cout << x << ' ';
    std::cout << "}\n";
}
 
int main()
{
    std::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    println("Initially, cnt = ", cnt);
 
    std::erase(cnt, '3');
    println("After erase '3', cnt = ", cnt);
 
    auto erased = std::erase_if(cnt, [](char x) { return (x - '0') % 2 == 0; });
    println("After erase all even numbers, cnt = ", cnt);
    std::cout << "Erased even numbers: " << erased << '\n';
 
    std::vector<std::complex<double>> nums{{2, 2}, {4, 2}, {4, 8}, {4, 2}};
    #ifdef __cpp_lib_algorithm_default_value_type
        std::erase(nums, {4, 2});
    #else
        std::erase(nums, std::complex<double>{4, 2});
    #endif
    println("After erase {4, 2}, nums = ", nums);
}
```

Išvestis:

```
Initially, cnt = { 0 1 2 3 4 5 6 7 8 9 }
After erase '3', cnt = { 0 1 2 4 5 6 7 8 9 }
After erase all even numbers, cnt = { 1 5 7 9 }
Erased even numbers: 5
After erase {4, 2}, nums = { (2,2) (4,8) }
```
</details>

### `assign`

Ši funkcija priskiria vektoriui naujas reikšmes iš nurodyto intervalo.

**Parametrai:**
- `InputIt first` - Intervalo pradžia.
- `InputIt last` - Intervalo pabaiga.

**Grąžinamas tipas:** nėra (void).

<details>
<summary>Veikimo pavyzdys</summary>
<br>

```cpp
#include <vector>
#include <iostream>
#include <string>
 
int main()
{
    std::vector<char> characters;
 
    auto print_vector = [&]()
    {
        for (char c : characters)
            std::cout << c << ' ';
        std::cout << '\n';
    };
 
    characters.assign(5, 'a');
    print_vector();
 
    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_vector();
 
    characters.assign({'C', '+', '+', '1', '1'});
    print_vector();
}
```

Išvestis:

```
a a a a a
b b b b b b
C + + 1 1
```
</details>

### `push_back`

Ši funkcija prideda elementą į vektoriaus pabaigą.

**Parametrai:**
- `const T& value` - Pridedamas elementas.

**Grąžinamas tipas:** nėra (void).

<details>
<summary>Veikimo pavyzdys</summary>
<br>

```cpp
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
 
int main()
{
    std::vector<std::string> letters;
 
    letters.push_back("abc");
    std::string s{"def"};
    letters.push_back(std::move(s));
 
    std::cout << "std::vector letters holds: ";
    for (auto&& e : letters)
        std::cout << std::quoted(e) << ' ';
 
    std::cout << "\nMoved-from string s holds: " << std::quoted(s) << '\n';
}
```

Išvestis:

```
std::vector letters holds: "abc" "def"
Moved-from string s holds: ""
```
</details>

# Testavimo rezultatai (v3.0)

## 6-as tyrimas - tuščių std::vector vs Vector konteinerių užpildymo spartos palyginimas

Naudojamas 3 bandymų vidurkis.

| Dydis       | std::vector (s) | Vector (s)  |
|-------------|-----------------|-------------|
| 10000       | 0.000318933     | 0.000263833 |
| 100000      | 0.0026147       | 0.0025965   |
| 1000000     | 0.0246464       | 0.0252233   |
| 10000000    | 0.230314        | 0.278832    |
| 100000000   | 2.2361          | 2.69485     |

## 7-as tyrimas - veiksmų su įvairaus dydžio failais veikimo spartos palyginimas naudojant std::vector vs Vector
### std::vector:

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.583564            | 2.90436              | 47.0824               |
| Studentų rūšiavimas į dvi grupes         | 0.0206489           | 0.166762             | 1.75543               |
| Studentų rikiavimas (pagal galutinį vid.)| 0.210474            | 2.71605              | 36.9104               |
| Nepatenkinamų stud. įrašymas             | 0.101835            | 1.00919              | 6.36996               |
| Patenkinamų stud. įrašymas               | 0.142624            | 1.40209              | 13.2886               |
| Visas laikas (be įvesties)               | 1.06205             | 8.1996               | 105.409               |

### Vector:

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.441618            | 1.97501              | 31.777                |
| Studentų rūšiavimas į dvi grupes         | 0.0132866           | 0.0973608            | 1.1143                |
| Studentų rikiavimas (pagal galutinį vid.)| 0.106655            | 1.36616              | 20.9515               |
| Nepatenkinamų stud. įrašymas             | 0.10986             | 0.961898             | 9.04419               |
| Patenkinamų stud. įrašymas               | 0.143081            | 1.42063              | 12.51                 |
| Visas laikas (be įvesties)               | 0.815772            | 5.8222               | 75.3985               |


# Pakeitimai (SENA VERSIJA - v2.0)

- Sukurta programos kodo dokumentacija naudojant `Doxygen`.
- Realizuoti unit testai naudojant `GoogleTest` karkasą.

# Pakeitimai (SENA VERSIJA - v1.5)

Vietoje vienos klasės `Studentas` sukurtos dvi: bazinė (abstrakti, negalima kurti jos objektų) klasė `Zmogus`, skirta bendrai aprašyti žmogų, ir iš jos išvestinė (derived) klasė - `Studentas`.

# Pakeitimai (SENA VERSIJA - v1.2)

## Rule of five

- Kopijavimo konstruktorius sukuria naują `Studentas` objektą, kuris yra kopija esamo `Studentas` objekto. Pavyzdžiui, `Studentas s2(s1)`; sukurs `Studentas` objektą `s2`, kuris yra kopija `s1`.
- Perkėlimo konstruktorius sukuria naują `Studentas` objektą, perkeliant duomenis iš esamo `Studentas` objekto, naudojant `std::move`. Pavyzdžiui, `Studentas s4(std::move(s1));` sukurs `Studentas` objektą `s4`, perkeliant duomenis iš `s1`.
- Kopijavimo priskyrimo operatorius priskiria esamam `Studenta`s objektui kopiją kito Studentas objekto. Pavyzdžiui, `s3 = s1;` priskirs `s3` kopiją `s1`.
- Perkėlimo priskyrimo operatorius priskiria esamam `Studentas` objektui išteklius iš kito `Studentas` objekto, naudojant `std::move`. Pavyzdžiui, `s5 = std::move(s2);` priskirs `s5` išteklius iš `s2`.
- Destruktorius valo `Studentas` objekto duomenis, kai objektas yra sunaikinamas. Pavyzdžiui, `~Studentas() { namuDarbai_.clear(); }` išvalys `namuDarbai_` vektorių, kai `Studentas` objektas bus sunaikintas.

## Perdengti įvesties/išvesties operatoriai

- Įvesties operatorius leidžia nuskaityti `Studentas` objekto duomenis iš srauto. Pavyzdžiui, `iss >> s6;` nuskaitys `Studentas` objekto duomenis iš `iss` srauto ir įrašys juos į `s6`. Formatas - `vardas pavarde nd1 nd2 ... ndj egz`.
- Išvesties operatorius leidžia išvesti `Studentas` objekto duomenis į srautą. Pavyzdžiui, `std::cout << s1`; išves `s1` objekto duomenis į konsolę. Formatas - `vardas pavarde galutinisVid galutinisMed`.

# Testavimo rezultatai (SENA VERSIJA - v1.1)

## 5-as tyrimas - struct ir klasės realizacijų palyginimas naudojant skirtingus optimizacijos flag'us

Naudojamas `std::vector` konteineris su 3-ąja strategija, kadangi ši realizacija buvo sparčiausia.

### Be optimizacijos

**class**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.55441             | 2.99869              |
| Studentų rūšiavimas į dvi grupes         | 0.0294032           | 0.324522             |
| Studentų rikiavimas (pagal galutinį vid.)| 0.268387            | 3.62094              |
| Nepatenkinamų stud. įrašymas             | 0.0594664           | 0.834831             |
| Patenkinamų stud. įrašymas               | 0.146798            | 1.14285              |
| Visas laikas (be įvesties)               | 1.05953             | 8.9231               |

**struct**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.567829            | 2.82114              |
| Studentų rūšiavimas į dvi grupes         | 0.0165831           | 0.132552             |
| Studentų rikiavimas (pagal galutinį vid.)| 0.190172            | 2.19651              |
| Nepatenkinamų stud. įrašymas             | 0.0476958           | 0.41684              |
| Patenkinamų stud. įrašymas               | 0.061474            | 0.589398             |
| Visas laikas (be įvesties)               | 0.884936            | 6.15744              |

### O1

**class**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.290447            | 1.60373              |
| Studentų rūšiavimas į dvi grupes         | 0.011564            | 0.155452             |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0767983           | 1.11478              |
| Nepatenkinamų stud. įrašymas             | 0.109401            | 0.738442             |
| Patenkinamų stud. įrašymas               | 0.144365            | 1.01084              |
| Visas laikas (be įvesties)               | 0.633825            | 4.62428              |

**struct**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.291732            | 1.37662              |
| Studentų rūšiavimas į dvi grupes         | 0.0042403           | 0.0318562            |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0192268           | 0.309694             |
| Nepatenkinamų stud. įrašymas             | 0.0450104           | 0.37274              |
| Patenkinamų stud. įrašymas               | 0.0627002           | 0.549383             |
| Visas laikas (be įvesties)               | 0.424141            | 2.64151              |

### O2

**class**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.308291            | 1.5634               |
| Studentų rūšiavimas į dvi grupes         | 0.0125242           | 0.163663             |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0768268           | 1.09284              |
| Nepatenkinamų stud. įrašymas             | 0.0838686           | 0.746606             |
| Patenkinamų stud. įrašymas               | 0.119298            | 1.069                |
| Visas laikas (be įvesties)               | 0.602014            | 4.63658              |

**struct**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.283916            | 1.49784              |
| Studentų rūšiavimas į dvi grupes         | 0.0040509           | 0.0505524            |
| Studentų rikiavimas (pagal galutinį vid.)| 0.019015            | 0.297101             |
| Nepatenkinamų stud. įrašymas             | 0.0422741           | 0.398234             |
| Patenkinamų stud. įrašymas               | 0.0632793           | 0.542183             |
| Visas laikas (be įvesties)               | 0.413627            | 2.78692              |


### O3

**class**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.300681            | 1.4992               |
| Studentų rūšiavimas į dvi grupes         | 0.015888            | 0.117621             |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0856192           | 1.06428              |
| Nepatenkinamų stud. įrašymas             | 0.0939197           | 0.709202             |
| Patenkinamų stud. įrašymas               | 0.134521            | 1.00598              |
| Visas laikas (be įvesties)               | 0.631756            | 4.39729              |

**struct**

| Veiksmas                                 | studentai100000.txt | studentai1000000.txt |
|------------------------------------------|---------------------|----------------------|
| Duomenų nuskaitymas                      | 0.279097            | 1.413                |
| Studentų rūšiavimas į dvi grupes         | 0.0032647           | 0.0528338            |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0192365           | 0.287692             |
| Nepatenkinamų stud. įrašymas             | 0.0474045           | 0.406698             |
| Patenkinamų stud. įrašymas               | 0.0620375           | 0.572647             |
| Visas laikas (be įvesties)               | 0.412173            | 2.73402              |

### Failų dydžiai

| Dydis (KB) | Be optimizacijos | O1 | O2 | O3 |
|------------|------------------|----|----|----|
| class      | 357              | 178| 162| 205|
| struct     | 350              | 168| 162| 171|

# Testavimo rezultatai (SENA VERSIJA - v1.0)

## 3-as tyrimas - veiksmų su įvairaus dydžio failais veikimo spartos palyginimas naudojant skirtingus konteinerius
### std::vector:

| Veiksmas                                 | studentai_1000.txt | studentai10000.txt | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|--------------------|--------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.0079811          | 0.0487976          | 0.531535            | 2.61098              | 40.1398               |
| Studentų rūšiavimas į dvi grupes         | 0.0003127          | 0.0032477          | 0.0311697           | 0.40542              | 2.92201               |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0012382          | 0.0124778          | 0.183998            | 2.35545              | 33.5949               |
| Nepatenkinamų stud. įrašymas             | 0.0135206          | 0.0074135          | 0.0548416           | 0.52273              | 4.82187               |
| Patenkinamų stud. įrašymas               | 0.0023206          | 0.0117405          | 0.0714214           | 0.95434              | 7.61723               |
| Visas laikas (be įvesties)               | 0.0263308          | 0.0848641          | 0.87404             | 6.84993              | 89.0968               |

`std::vector` suteikia greitą atsitiktinę prieigą ir yra efektyvus pridedant/pašalinant elementus konteinerio gale, tačiau lėčiau vyksta rikiavimas.

### std::list:

| Veiksmas                                 | studentai_1000.txt | studentai10000.txt | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|--------------------|--------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.005016           | 0.0542886          | 0.554947            | 2.57551              | 40.0213               |
| Studentų rūšiavimas į dvi grupes         | 0.0002704          | 0.0027486          | 0.0294175           | 0.304561             | 2.68456               |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0003741          | 0.0046876          | 0.0601057           | 0.916619             | 15.4971               |
| Nepatenkinamų stud. įrašymas             | 0.0023782          | 0.0082674          | 0.069381            | 0.561922             | 23.1234               |
| Patenkinamų stud. įrašymas               | 0.0021207          | 0.0129539          | 0.0906158           | 0.897952             | 7.86134               |
| Visas laikas (be įvesties)               | 0.0110467          | 0.0840572          | 0.805627            | 5.25787              | 89.1898               |

`std::list` yra dvipusis sąrašas, kuris leidžia greitai įterpti ir pašalinti elementus bet kurioje vietoje. Tačiau, kadangi elementai nėra saugomi gretimuose atminties blokuose, atsitiktinė prieiga yra lėta.

### std::deque:

| Veiksmas                                 | studentai_1000.txt | studentai10000.txt | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|--------------------|--------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.0047706          | 0.0471314          | 0.516918            | 2.47154              | 39.3773               |
| Studentų rūšiavimas į dvi grupes         | 0.0002705          | 0.0027023          | 0.0244668           | 0.243373             | 2.14507               |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0014055          | 0.0162116          | 0.226202            | 2.95295              | 41.2255               |
| Nepatenkinamų stud. įrašymas             | 0.0026466          | 0.0094148          | 0.0765059           | 0.734575             | 7.40374               |
| Patenkinamų stud. įrašymas               | 0.002045           | 0.0122805          | 0.107698            | 0.976536             | 10.6144               |
| Visas laikas (be įvesties)               | 0.01218            | 0.0890443          | 0.952917            | 7.38004              | 100.768               |

`std::deque` yra kaip tarpinė būsena tarp `std::vector` ir `std::list`. Jis leidžia greitai pridėti ir pašalinti elementus tiek priekyje, tiek gale, bet atsitiktinė prieiga yra šiek tiek lėtesnė nei std::vector.


## 4-as tyrimas - rūšiavimo realizacijų veikimo spartos palyginimas

### 1 strategija - du nauji to paties tipo konteineriai

| Failas               | std::vector | std::list  | std::deque |
|----------------------|-------------|------------|------------|
| studentai_1000.txt   | 0.0003127   | 0.0002704  | 0.0002705  |
| studentai10000.txt   | 0.0032477   | 0.0027486  | 0.0027023  |
| studentai100000.txt  | 0.0311697   | 0.0294175  | 0.0244668  |
| studentai1000000.txt | 0.40542     | 0.304561   | 0.243373   |
| studentai_1000000.txt| 2.92201     | 2.68456    | 2.14507    |

### 2 strategija - tik vienas naujas nepatenkinamų studentų konteineris

| Failas               | std::vector | std::list  | std::deque |
|----------------------|-------------|------------|------------|
| studentai_1000.txt   | 0.0012996   | 0.0006707  | 0.0015146  |
| studentai10000.txt   | 0.200978    | 0.0067772  | 0.0175728  |
| studentai100000.txt  | 0.22196     | 0.0954995  | 0.248444   |
| studentai1000000.txt | 2.58776     | 1.25379    | 3.08339    |
| studentai_1000000.txt| 36.8585     | 20.8087    | 41.5902    |

Visose realizacijose rūšiavimas vyksta lėčiau, tačiau tai yra daug efektyviau atminties naudojimo atžvilgiu.

### 3 strategija - naudojamos optimizacijos 2-ai strategijai pagerinti

Kadangi 1-osios strategijos konteinerių veikimą **pateiktais** algoritmais nelabai būtų galima pagreitinti, pasirinkau optimizuoti 2-ąją strategiją, nes ši iš pradžių buvo itin lėta.

Optimizacijai panaudojau `std::partition` algoritmą, kuris su predikatu pertvarko vektoriaus elementus taip, kad patenkinami būtų priekyje, o nepatenkinami gale. Tokiu būdu išvengiama pakartotinų `erase` operacijų, kuri šiems konteineriams yra brangi, nes reikia pertvarkyti visus elementus taip, kad būtų užpildomas naujai atsiradęs tarpas. Taip pat konteinerio elementai yra perkeliami naudojant `std::make_move_iterator`, kad nereikėtų jų kopijuoti.

| Failas               | std::vector | std::list  | std::deque |
|----------------------|-------------|------------|------------|
| studentai_1000.txt   | 0.0001595   | 0.0002096  | 0.000229   |
| studentai10000.txt   | 0.0016814   | 0.0021475  | 0.0020181  |
| studentai100000.txt  | 0.0172146   | 0.0208541  | 0.0218965  |
| studentai1000000.txt | 0.132258    | 0.194196   | 0.197143   |
| studentai_1000000.txt| 1.33541     | 1.81953    | 1.93628    |

# Testavimo rezultatai (SENA VERSIJA - v0.4)
## 1-as tyrimas – atsitiktinių studentų failų generavimas

Kiekvienam failui generuojama 15 namų darbų pažymių.

| Bandymo nr. | 1,000 (s)  | 10,000 (s)    | 100,000 (s)   | 1,000,000 (s) | 10,000,000 (s) |
|------------|-------------|---------------|---------------|---------------|----------------|
| #1         | 0.0122881   | 0.0562507     | 0.525285      | 5.12021       | 50.9886        |
| #2         | 0.0065812   | 0.0552215     | 0.487528      | 4.86378       | 47.3717        |
| #3         | 0.0060949   | 0.0546876     | 0.495838      | 4.73056       | 50.3248        |
| Avg        | 0.0083214   | 0.0553866     | 0.5028837     | 4.90485       | 49.5617        |

## 2-as tyrimas – veiksmai su įvairaus dydžio failais

| Veiksmas                                 | studentai_1000.txt | studentai10000.txt | studentai100000.txt | studentai1000000.txt | studentai_1000000.txt |
|------------------------------------------|--------------------|--------------------|---------------------|----------------------|-----------------------|
| Duomenų nuskaitymas                      | 0.004684           | 0.0439758          | 0.525648            | 2.51385              | 40.7778               |
| Studentų rūšiavimas į dvi grupes         | 0.000408           | 0.003108           | 0.0323843           | 0.319982             | 3.22454               |
| Studentų rikiavimas (pagal galutinį vid.)| 0.0014113          | 0.0134936          | 0.195734            | 2.53566              | 36.1703               |
| Nepatenkinamų stud. įrašymas             | 0.0022855          | 0.0060508          | 0.0517857           | 0.49798              | 5.58282               |
| Patenkinamų stud. įrašymas               | 0.0013947          | 0.0082172          | 0.0670584           | 0.60272              | 7.11938               |
| Visas laikas (be įvesties)               | 0.0112775          | 0.0763587          | 0.875746            | 6.4748               | 92.8779               |

# Testavimo rezultatai (SENA VERSIJA - v0.2)

## Preliminarus tyrimas
### Failo nuskaitymas

| Bandymo nr. | 10k (s) | 100k (s) | 1 mil (s) |
|-------------|---------|----------|-----------|
| #1          | 0.04551 | 0.54182  | 2.57977   |
| #2          | 0.04652 | 0.53072  | 2.56835   |
| #3          | 0.05092 | 0.54170  | 2.55759   |
| Avg         | 0.04765 | 0.53808  | 2.56857   |

### Rikiavimas pagal 3-ią pasirinkimą (galutinis pažymys, vid.)

| Bandymo nr.  | 10k (s) | 100k (s) | 1 mil (s) |
|--------------|---------|----------|-----------|
| #1           | 0.01309 | 0.16808  | 2.18078   |
| #2           | 0.01324 | 0.17555  | 2.23208   |
| #3           | 0.01122 | 0.17587  | 2.11809   |
| Avg          | 0.01252 | 0.17317  | 2.17698   |

</details>
