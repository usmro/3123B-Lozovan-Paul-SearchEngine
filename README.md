# 🔍 Advanced Search Engine - C++ Project

> **Student:** Lozovan Paul  
> **Grupa:** 3123B  
> **Facultate:** Facultatea de Inginerie Electrică și Știința Calculatoarelor (FIESC)  
> **Universitate:** Universitatea „Ștefan cel Mare” din Suceava (USV) / @usmro  

---

## 📌 Descrierea Proiectului
Acest proiect implementează un **motor de căutare inversat (Inverted Index Search Engine)** , dezvoltat complet în **Modern C++ (C++17)** pentru mediul Linux. 

Aplicația scanează automat folderul curent după fișiere text (`.txt`), le procesează, elimină zgomotul (stop-words) și mapează fiecare cuvânt către fișierele și liniile exacte unde acesta apare. Căutările pot fi efectuate atât în mod simplu, cât și avansat, utilizând logica booleană (`AND` / `OR`).

---

## 🛠️ Concepte OOP & Design Patterns Implementate

Proiectul respectă bunele practici de inginerie software și include:

* **Single Responsibility Principle:** Separarea clară a logicii în clase dedicate:
    * `Document`: Model de date simplu care reține metadatele fișierului.
    * `Index`: Structura de date pură (`std::unordered_map`) optimizată pentru mapare.
    * `SearchEngine`: Controlerul principal care coordonează fluxul aplicației.
* **Design Pattern-ul Observer:** Clasa `SearchEngine` acționează ca un *Subject* (Observabil). La fiecare căutare executată, aceasta notifică automat clasa `Logger` (*Observer*), care jurnalizează activitatea.
* **Standard Template Library (STL):** Utilizarea intensivă a containerelor performante (`unordered_map`, `set`, `vector`, `pair`) și a algoritmilor matematici de seturi (`std::set_intersection`, `std::set_union`).
* **Standardul C++17:** Scanarea automată a directoarelor nativă din Linux utilizând biblioteca `<filesystem>`.

---

## 🚀 Funcționalități Cheie

| Funcționalitate | Descriere |
| :--- | :--- |
| **Scanare Automată** | Detectează fișierele `.txt` la pornire fără introducere manuală. |
| **Căutare Avansată** | Suport pentru operatori booleeni: `cuvant1 AND cuvant2` sau `cuvant1 OR cuvant2`. |
| **Afișare la nivel de linie** | Indică exact linia din fișier unde a fost identificat cuvântul. |
| **Filtrare Stop-Words** | Ignoră automat cuvintele de legătură uzuale (`si`, `la`, `un`, `este` etc.). |
| **Logger Automat** | Înregistrează statusul fiecărei tranzacții de căutare în consolă. |

---

## 💻 Ghid de Compilare și Rulare (Linux / WSL)

Pentru compilare este necesar un compilator `g++` care suportă standardul **C++17**.

### 1. Compilarea proiectului
Deschide terminalul în folderul proiectului și rulează:
```bash
sudo apt update
sudo apt install -y build-essential git
git clone https://github.com/usmro/3123B-Lozovan-Paul-SearchEngine.git
cd 3123B-Lozovan-Paul-SearchEngine
g++ -std=c++17 -Wall -Wextra main.cpp -o engine
./engine