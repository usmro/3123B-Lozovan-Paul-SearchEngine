#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

class Observer {
public:
    virtual void update(const std::string& cautare, bool gasit) = 0;
    virtual ~Observer() = default;
};

class Subject {
protected:
    std::vector<Observer*> observatori;
public:
    void ataseaza(Observer* obs) { observatori.push_back(obs); }
    void notifica(const std::string& cautare, bool gasit) {
        for (auto* obs : observatori) {
            obs->update(cautare, gasit);
        }
    }
};

class Logger : public Observer {
public:
    void update(const std::string& cautare, bool gasit) override {
        std::cout << "S-a efectuat o cautare pentru: '" << cautare 
                  << "' | Status: " << (gasit ? "Gasit" : "Negasit") << std::endl;
    }
};

class Document {
public:
    std::string caleFisier;
    std::string continut;
    Document(std::string cale, std::string text) : caleFisier(cale), continut(text) {}
};

class Index {
private:
    std::unordered_map<std::string, std::set<std::pair<int, int>>> dateIndex;
    std::set<std::string> stopWords = {"si", "in", "la", "un", "o", "de", "cu", "este", "pe"};

public:
    std::string cleanWord(std::string word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        return word;
    }

    bool esteStopWord(const std::string& cuvant) {
        return stopWords.count(cuvant) > 0;
    }

    void adauga(const std::string& cuvant, int docId, int linie) {
        std::string cuvantCurat = cleanWord(cuvant);
        if (!cuvantCurat.empty() && !esteStopWord(cuvantCurat)) {
            dateIndex[cuvantCurat].insert({docId, linie});
        }
    }

    std::set<std::pair<int, int>> obtineLocatii(const std::string& cuvant) {
        std::string cuvantCurat = cleanWord(cuvant);
        if (dateIndex.count(cuvantCurat)) {
            return dateIndex[cuvantCurat];
        }
        return {};
    }
};

class SearchEngine : public Subject {
private:
    Index indexulMeu;
    std::vector<Document> colectie;

public:
    void adaugaDocument(const std::string& cale, const std::string& continut) {
        int docId = colectie.size();
        colectie.push_back(Document(cale, continut));

        std::stringstream ss(continut);
        std::string linie;
        int numarLinie = 1;

        while (std::getline(ss, linie)) {
            std::stringstream lineStream(linie);
            std::string cuvant;
            while (lineStream >> cuvant) {
                indexulMeu.adauga(cuvant, docId, numarLinie);
            }
            numarLinie++;
        }
    }

    void incarcaDinFisierFizic(const std::string& cale) {
        std::ifstream file(cale);
        if (!file.is_open()) return;
        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        adaugaDocument(cale, text);
    }
    std::set<std::pair<int, int>> cautaSimplu(const std::string& cuvant) {
        auto rezultate = indexulMeu.obtineLocatii(cuvant);
        notifica(cuvant, !rezultate.empty());
        return rezultate;
    }

    std::set<std::pair<int, int>> cautaAvansat(const std::string& cuvant1, const std::string& op, const std::string& cuvant2) {
        auto locatii1 = indexulMeu.obtineLocatii(cuvant1);
        auto locatii2 = indexulMeu.obtineLocatii(cuvant2);
        std::set<std::pair<int, int>> rezultate;

        if (op == "and") {
            std::set_intersection(locatii1.begin(), locatii1.end(),
                                  locatii2.begin(), locatii2.end(),
                                  std::inserter(rezultate, rezultate.begin()));
        } else if (op == "or") {
            std::set_union(locatii1.begin(), locatii1.end(),
                           locatii2.begin(), locatii2.end(),
                           std::inserter(rezultate, rezultate.begin()));
        }

        notifica(cuvant1 + " " + op + " " + cuvant2, !rezultate.empty());
        return rezultate;
    }

    std::string getCaleDocument(int id) const { return colectie[id].caleFisier; }
};

int main() {
    SearchEngine engine;
    Logger logger;
    engine.ataseaza(&logger);

    std::cout << "Se indexeaza fisierele text din folder\n";
    for (const auto& entry : fs::directory_iterator("./")) {
        if (entry.path().extension() == ".txt") {
            engine.incarcaDinFisierFizic(entry.path().string());
        }
    }

    std::string linieInterogare;
    std::cin.clear();

    while (true) {
        std::cout << "\nCautare: ";
        std::getline(std::cin, linieInterogare);

        if (linieInterogare.empty()) break;

        std::stringstream ss(linieInterogare);
        std::vector<std::string> componente;
        std::string bucata;

        while (ss >> bucata) {
            componente.push_back(bucata);
        }

        std::set<std::pair<int, int>> rezultate;

        if (componente.size() == 1) {
            rezultate = engine.cautaSimplu(componente[0]);
        } 
        else if (componente.size() == 3) {
            std::string c1 = componente[0];
            std::string op = componente[1];
            std::string c2 = componente[2];

            if (op != "and" && op != "or") {
                std::cout << "Operator invalid\n";
                continue;
            }
            rezultate = engine.cautaAvansat(c1, op, c2);
        } 
        else {
            std::cout << "Format gresit\n";
            continue;
        }

        if (!rezultate.empty()) {
            for (const auto& p : rezultate) {
                std::cout << "Gasit in: " << engine.getCaleDocument(p.first) 
                          << " | Linia: " << p.second << std::endl;
            }
        } else {
            std::cout << " -> Niciun rezultat gasit.\n";
        }
    }

    return 0;
}