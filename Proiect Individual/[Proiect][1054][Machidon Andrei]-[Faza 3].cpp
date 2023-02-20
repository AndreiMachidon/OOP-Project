#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <set>
#include <map>
#include <list>
using namespace std;

class CustomException {
	string mesaj;
public:
	CustomException(string mesaj) {
		this->mesaj = mesaj;
	}

	string getMesaj() {
		return this->mesaj;
	}
};
//interfata(clasa abstracta)
class IFile {
protected:

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) = 0;

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) = 0;

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) = 0;

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& f) = 0;

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& f) = 0;
};

class Imobil : public IFile {
	int anConstruire = 0;
	static float pretMinim;
	char* locatie = NULL;
	float suprafata = 0;
	float istoricPreturi[5];  //istoricPreturi[4] -> pretul curent

public:

	//constructor fara parametrii
	Imobil() :anConstruire(0) {

		//initializam vectorul char cu 'Necunoscut'
		this->locatie = new char[strlen("Necunoscut") + 1];
		strcpy(this->locatie, "Necunoscut");
	}

	//constructor cu 1 parametru
	Imobil(int anConstruire) {

		//initializam vectorul char cu 'Necunoscut'
		this->anConstruire = anConstruire;
		this->locatie = new char[strlen("Necunoscut") + 1];
		strcpy(this->locatie, "Necunoscut");

	};

	//constructor cu toti parametrii
	Imobil(int anConstruire, const char* locatie, float suprafata, float istoricPreturi[5]) {

		//validare an construire
		if (anConstruire > 0) {
			this->anConstruire = anConstruire;
		}

		//validare locatie
		if (locatie != NULL && strlen(locatie) > 1) {
			this->locatie = new char[strlen(locatie) + 1];
			strcpy(this->locatie, locatie);
		}
		else {
			this->locatie = new char[strlen("Necunoscut") + 1];
			strcpy(this->locatie, "Necunoscut");
		}

		//validare suprafata
		if (suprafata > 0) {
			this->suprafata = suprafata;
		}

		//validare vectorul int istoricPreturi
		bool ok = true;
		for (int i = 0; i < 5; i++) {
			if (istoricPreturi[i] < Imobil::pretMinim) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 5; i++) {
				this->istoricPreturi[i] = istoricPreturi[i];
			}
		}

	}

	//constructor de copiere
	Imobil(const Imobil& imb) {

		this->anConstruire = imb.anConstruire;

		this->locatie = new char[strlen(imb.locatie) + 1];
		strcpy(this->locatie, imb.locatie);


		this->suprafata = imb.suprafata;

		for (int i = 0; i < 5; i++) {
			this->istoricPreturi[i] = imb.istoricPreturi[i];
		}
	}

	//destructor
	~Imobil() {
		delete[] this->locatie;
	}

	//overload operator =
	Imobil& operator=(const Imobil& imb) {

		if (this != &imb) {
			this->anConstruire = anConstruire;
			delete[] this->locatie;
			this->locatie = new char[strlen(imb.locatie) + 1];
			strcpy(this->locatie, imb.locatie);

			this->suprafata = imb.suprafata;

			for (int i = 0; i < 5; i++) {
				this->istoricPreturi[i] = imb.istoricPreturi[i];
			}
		}


		return *this;
	}

	//getters
	int getAnConstruire() {
		return this->anConstruire;
	}

	static float getPretMinim() {
		return Imobil::pretMinim;
	}

	char* getLocatie() {
		return this->locatie;
	}

	float getSuprafata() {
		return this->suprafata;
	}

	float* getIstoricPreturi() {
		return this->istoricPreturi;
	}

	float getPretCurent() {
		return this->istoricPreturi[4];
	}

	//setters
	void setLocatie(const char* locatie) {
		if (locatie != NULL && strlen(locatie) > 1) {
			delete[] this->locatie;
			this->locatie = new char[strlen(locatie) + 1];
			strcpy(this->locatie, locatie);
		}
		else {
			throw new CustomException("Locatia introdusa in setter nu este corespunzatoare");
		}
	}

	void setSuprafata(float suprafata) {
		if (suprafata > 0) {
			this->suprafata = suprafata;
		}
		else {
			throw new CustomException("Suprafata introdusa in setter nu este corespunzatoare");
		}
	}

	static void setPretMinim(float pret) {
		if (pret > Imobil::pretMinim) {
			Imobil::pretMinim = pret;
		}
		else {
			throw new CustomException("Pretul minim introdus in setter este sub limita initiala");
		}
	}

	void setIstoricPreturi(float istoricPreturi[5]) {

		bool ok = true;
		for (int i = 0; i < 5; i++) {
			if (istoricPreturi[i] <= Imobil::pretMinim) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 5; i++) {
				this->istoricPreturi[i] = istoricPreturi[i];
			}
		}
		else {
			throw new CustomException("Un pret din istoricul de preturi este mai mic decat pretul minim");
		}
	}



	//overload operator [] -> returneaza pretul i din IstoricPreturi
	float& operator[](int index) {
		if (index > 0 && index < 5) {
			return this->istoricPreturi[index];
		}
		else {
			throw new CustomException("Nu exista un istoric al pretului pe care doriti sa il accesati");
		}
	}

	//overload operator + -> aduna preturile curente ale celor 2 imobile si returneaza suma
	float operator+(const Imobil& imb) {
		return this->istoricPreturi[4] + imb.istoricPreturi[4];
	}

	//overload operator++ -> creste pretul curent cu 10%

	//preincrementare
	Imobil& operator++() {
		this->istoricPreturi[4] *= 1.1;
		return *this;
	}

	//postincrementare
	Imobil operator++(int) {
		Imobil aux = *this;
		this->istoricPreturi[4] *= 1.1;
		return aux;
	}

	//overload operator -- -> scade pretul curent cu 10%

	//predecrementare
	Imobil& operator--() {
		if (this->istoricPreturi[4] * 0.9 > Imobil::pretMinim) {
			this->istoricPreturi[4] *= 0.9;
		}
		else {
			throw new CustomException("Pretul rezultat din decrementare este prea mic");
		}

		return *this;
	}

	//postdecrementare
	Imobil operator--(int) {
		Imobil aux = *this;
		if (this->istoricPreturi[4] * 0.9 > Imobil::pretMinim) {
			this->istoricPreturi[4] *= 0.9;
		}
		else {
			throw new CustomException("Pretul rezultat din decrementare este prea mic");
		}
		return aux;
	}

	//overload cast float ->returneaza pretul curent
	explicit operator float() {
		return this->istoricPreturi[4];
	}



	//overload operator > -> compara pretul pe mp ale celor 2 imobile
	bool operator>(Imobil& imb) {
		float pretMediu1 = this->calculPretPeMp();
		float pretMediu2 = imb.calculPretPeMp();

		return pretMediu1 > pretMediu2;
	}

	//overload operator < ->compara preturile pe mp ale celor 2 imobile
	bool operator<(Imobil& imb) {
		float pretMediu1 = this->calculPretPeMp();
		float pretMediu2 = imb.calculPretPeMp();

		return pretMediu1 < pretMediu2;
	}

	//overload operator ! -> verifica daca imobilul nu are istoricPreturi, locatie si suprafata
	bool operator!() {

		bool ok = true;
		for (int i = 0; i < 5; i++) {
			if (this->istoricPreturi[i] != 0) {
				ok = false;
			}
		}
		if (this->anConstruire == 0 && strcmp(this->locatie, "Necunoscut") == 0 && suprafata == 0 && ok == true) {
			return true;
		}
		else {
			return false;
		}
	}

	//overload operator == -> compara anul constriutii imobiluluilocatia, suprafata si pretul pe mp ale celor 2 imobile
	bool operator==(const Imobil& imb) {

		float sum1 = this->calculPretPeMp();
		float sum2 = this->calculPretPeMp();
		if (this->anConstruire == imb.anConstruire && strcmp(this->locatie, imb.locatie) == 0 && this->suprafata == imb.suprafata && sum1 == sum2) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator !=(const Imobil& imb) {
		float sum1 = this->calculPretPeMp();
		float sum2 = this->calculPretPeMp();
		if (this->anConstruire != imb.anConstruire || strcmp(this->locatie, imb.locatie) != 0 && this->suprafata != imb.suprafata && sum1 != sum2) {
			return true;
		}
		else {
			return false;
		}

	}



	//overload ostream
	friend ostream& operator<<(ostream& out, const Imobil& imb) {
		out << "\n-----------------------";
		out << "\nAn construire: " << imb.anConstruire;
		out << "\nLocatie: " << imb.locatie;
		out << "\nSuprafata: " << imb.suprafata << " mp";
		out << "\nIstoric Preturi: " << endl;
		for (int i = 0; i < 5; i++) {
			out << "\nPretul " << i + 1 << ": " << imb.istoricPreturi[i] << " euro";
		}
		return out;
	}

	//overload istream
	friend istream& operator>>(istream& in, Imobil& imb) {

		cout << "Anul construirii imobilului: ";
		cin >> imb.anConstruire;
		in.ignore();
		char aux1[200];
		cout << "Locatie: ";
		in.getline(aux1, 99);

		if (aux1 != NULL && strlen(aux1) > 1) {
			delete[] imb.locatie;
			imb.locatie = new char[strlen(aux1) + 1];
			strcpy(imb.locatie, aux1);
		}

		cout << "\nSuprafata: ";  in >> imb.suprafata;

		float aux2[5]{};
		cout << "\nIstoric preturi: " << endl;
		for (int i = 0; i < 5; i++) {
			cout << "Pretul " << i + 1 << ": "; in >> aux2[i];
		}

		bool ok = true;
		for (int i = 0; i < 5; i++) {
			if (aux2[i] < Imobil::pretMinim) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 5; i++) {
				imb.istoricPreturi[i] = aux2[i];
			}
		}

		return in;
	}

	//metoda calculeaza pretul pe mp 
	float calculPretPeMp() {
		if (suprafata > 0) {
			return (float)this->istoricPreturi[4] / calculSuprafata();
		}
		else {
			return 0;
		}

	}

	//metoda calculeaza pretul mediu in functie de istoricul preturilor
	float calculPretMediu() {
		float suma = 0;
		for (int i = 0; i < 5; i++) {
			suma += this->istoricPreturi[i];
		}

		return suma / 5;
	}

	virtual float calculSuprafata() {
		return this->suprafata;
	}

	//metoda care permite citirea din fisiere binare
	virtual void readFromBinary(fstream& f) {

		//citim anul construirii imobilului dar nu il alocam pt ca e constant
		f.read((char*)&this->anConstruire, sizeof(int));
		//citim locatia
		//citim dimensiunea
		int lg = 0;
		f.read((char*)&lg, sizeof(int));

		//citim sirul
		char* buffer = new char[lg];
		f.read(buffer, lg);
		delete[] this->locatie;
		this->locatie = new char[lg]; //!!NU MERGE CITIREA DE CHAR* DIN FISIERUL BINAR

		strcpy(this->locatie, buffer);

		//citim suprafata
		f.read((char*)&this->suprafata, sizeof(float));

		//citim istoricul preturior
		for (int i = 0; i < 5; i++) {
			f.read((char*)&this->istoricPreturi[i], sizeof(float));
		}
	}

	//metoda care permite scrierea in fisiere binare
	virtual void writeToBinary(fstream& f) {


		//scriem anul construirii imobilului
		f.write((char*)&this->anConstruire, sizeof(int));

		//scriem locatia
		int lg = strlen(locatie) + 1;
		//prima data scriem dimensiunea sirului de caractere
		f.write((char*)&lg, sizeof(int));

		//scriem sirul de caractere
		f.write(locatie, lg);

		//scriem suprafata
		f.write((char*)&suprafata, sizeof(float));

		//scriem istoricul preturilor
		for (int i = 0; i < 5; i++) {
			f.write((char*)&istoricPreturi[i], sizeof(float));
		}

	}

	//metoda care permite scirerea in fisiere text
	virtual void writeToText(ofstream& out) {

		//scriere an construire
		out << this->anConstruire << endl;

		//scriere locatie
		out << this->locatie << endl;

		//scriere suprafata
		out << this->suprafata << endl;

		//scriere istoricul preturilor
		for (int i = 0; i < 5; i++) {
			out << this->istoricPreturi[i] << endl;
		}
	}

	//metoda care permite citirea din fisiere text
	virtual void readFromText(ifstream& in) {


		//se citeste anul construirii 
		in >> this->anConstruire;
		in.ignore();
		//cirite locatie
		char buffer[100];
		in.getline(buffer, 100);
		delete[] this->locatie;
		this->locatie = new char[strlen(buffer) + 1];
		strcpy(this->locatie, buffer);


		//citire suprafata
		in >> this->suprafata;

		//citire istoricPreturi
		for (int i = 0; i < 5; i++) {
			in >> this->istoricPreturi[i];
		}
	}

	//metoda care permite scrierea in fisiere Csv
	virtual void writeToCsv(ofstream& out) {
		out << this->anConstruire << ", " << this->locatie << ", " << this->suprafata << ", ";
		for (int i = 0; i < 5; i++) {
			out << this->istoricPreturi[i] << ", ";
		}

	}


};

float Imobil::pretMinim = 10000;

class Casa : public Imobil {

	int nrCasa = 0;
	float suprafataMansarda = 0;
	float suprafataGradina = 0;
public:
	//constructor fara parametrii
	Casa() :nrCasa(0), Imobil() {};

	//constructor cu parametrii
	Casa(int anConstruire, const char* locatie, float suprafata, float istoricPreturi[5], int nrCasa, float suprafataMansarda, float suprafataGradina) :Imobil(anConstruire, locatie, suprafata, istoricPreturi) {

		this->nrCasa = nrCasa;

		if (this->suprafataMansarda >= 0) {
			this->suprafataMansarda = suprafataMansarda;
		}

		if (this->suprafataGradina >= 0) {
			this->suprafataGradina = suprafataGradina;
		}
	}

	//constructor de copiere
	Casa(const Casa& c) :Imobil(c) {
		this->nrCasa = c.nrCasa;
		this->suprafataMansarda = c.suprafataMansarda;
		this->suprafataGradina = c.suprafataGradina;
	}

	//operator =
	Casa& operator=(const Casa& c) {
		if (this != &c) {
			this->nrCasa = c.nrCasa;
			this->suprafataMansarda = c.suprafataMansarda;
			this->suprafataGradina = c.suprafataGradina;
		}

		return *this;
	}

	//destructor
	~Casa() {
	};

	//ostream
	friend ostream& operator<<(ostream& out, const Casa& c) {
		out << (Imobil&)c << endl;
		out << "Numarul casei: " << c.nrCasa << endl;
		out << "Suprafata mansardei: " << c.suprafataMansarda << endl;
		out << "Suprafata gardina: " << c.suprafataGradina << endl;

		return out;

	}

	friend istream& operator>>(istream& in, Casa& c) {
		in >> (Imobil&)c;
		cout << "Numarul casei: ";
		in >> c.nrCasa;
		cout << "Suprafata mansardei: ";
		in >> c.suprafataMansarda;
		cout << "Suprafata gardina: ";
		in >> c.suprafataGradina;
		return in;
	}


	float calculSuprafata() {
		return Imobil::calculSuprafata() + suprafataGradina + suprafataMansarda;
	}

	//metoda care scrie textul in fisiere csv
	void writeToCsv(ofstream& out) {

		Imobil::writeToCsv(out);
		out << this->nrCasa << this->suprafataMansarda << ", " << this->suprafataGradina << endl;

	}


	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {
		Imobil::writeToBinary(f);

		//scriem nr casei
		f.write((char*)&this->nrCasa, sizeof(int));

		//scriem suprafataMansardei
		f.write((char*)&this->suprafataMansarda, sizeof(float));

		//scriem suprafata gradinii
		f.write((char*)&this->suprafataGradina, sizeof(float));

	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {
		Imobil::readFromBinary(f);
		//citim nr casei
		f.read((char*)&this->nrCasa, sizeof(int));

		//scriem suprafataMansardei
		f.read((char*)&this->suprafataMansarda, sizeof(float));

		//scriem suprafata gradinii
		f.read((char*)&this->suprafataGradina, sizeof(float));
	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		Imobil::readFromText(in);
		in >> this->nrCasa;
		in >> this->suprafataMansarda;
		in >> this->suprafataGradina;
	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		Imobil::writeToText(out);

		out << this->nrCasa << endl;
		out << this->suprafataMansarda << endl;
		out << this->suprafataGradina << endl;



	}

};

class Apartament : public Imobil {
	int etaj = 0;
	int nrBalcoane = 0;
	float* suprafeteBalcoane = NULL;
	float suprafataLocParcare = 0;
public:
	//constructor fara parametrii
	Apartament() :Imobil() {};

	//constructor cu parametrii
	Apartament(int anConstruire, const char* locatie, float suprafata, float istoricPreturi[5], int etaj, int nrBalcoane, float* suprafeteBalcoane, float suprafataLocParcare) :Imobil(anConstruire, locatie, suprafata, istoricPreturi) {

		if (etaj > 0) {
			this->etaj = etaj;
		}

		if (nrBalcoane >= 0 && suprafeteBalcoane != NULL) {
			this->nrBalcoane = nrBalcoane;
			this->suprafeteBalcoane = new float[this->nrBalcoane];

			for (int i = 0; i < this->nrBalcoane; i++) {
				this->suprafeteBalcoane[i] = suprafeteBalcoane[i];
			}

		}



		if (suprafataLocParcare > 0) {
			this->suprafataLocParcare = suprafataLocParcare;
		}
	}

	//constructor de copiere
	Apartament(const Apartament& a) :Imobil(a) {
		this->etaj = a.etaj;
		this->nrBalcoane = a.nrBalcoane;
		this->suprafeteBalcoane = new float[this->nrBalcoane];

		for (int i = 0; i < this->nrBalcoane; i++) {
			this->suprafeteBalcoane[i] = a.suprafeteBalcoane[i];
		}

		this->suprafataLocParcare = a.suprafataLocParcare;
	}

	//overload operator =
	Apartament& operator=(const Apartament& a) {

		if (this != &a) {
			Imobil::operator=(a);
			this->etaj = a.etaj;
			delete[] this->suprafeteBalcoane;
			this->nrBalcoane = nrBalcoane;
			this->suprafeteBalcoane = new float[this->nrBalcoane];

			for (int i = 0; i < this->nrBalcoane; i++) {
				this->suprafeteBalcoane[i] = suprafeteBalcoane[i];
			}

			this->suprafataLocParcare = suprafataLocParcare;
		}

	}

	//destructor
	~Apartament() {};

	//ostream
	friend ostream& operator<<(ostream& out, const Apartament& a) {
		out << (Imobil&)a << endl;
		out << "Etaj: " << a.etaj << endl;
		out << "Numar Balcoane: " << a.nrBalcoane << endl;
		for (int i = 0; i < a.nrBalcoane; i++) {
			out << "Balcon " << i + 1 << ": " << a.suprafeteBalcoane[i] << " mp" << endl;
		}

		out << "Suprafata locului de parcare: " << a.suprafataLocParcare << " mp";

		return out;
	}

	//istream
	friend istream& operator>>(istream& in, Apartament& a) {
		in >> (Apartament&)a;
		cout << "Etaj: ";
		in >> a.etaj;
		cout << "Numar balconae: ";
		in >> a.nrBalcoane;

		delete[] a.suprafeteBalcoane;
		a.suprafeteBalcoane = new float[a.nrBalcoane];

		for (int i = 0; i < a.nrBalcoane; i++) {
			cout << "Suprafata baclon " << i + 1 << ": ";
			in >> a.suprafeteBalcoane[i];
		}

		return in;
	}


	float calculSuprafata() {
		float  suprafataTotalaBalcoane = 0;
		for (int i = 0; i < this->nrBalcoane; i++) {
			suprafataTotalaBalcoane += this->suprafeteBalcoane[i];
		}

		return Imobil::calculSuprafata() + suprafataTotalaBalcoane + suprafataLocParcare;
	}


	//metoda care scrie textul in fisiere csv
	void writeToCsv(ofstream& out) {
		Imobil::writeToCsv(out);

		out << this->etaj << ", " << this->nrBalcoane << ", ";
		for (int i = 0; i < this->nrBalcoane; i++) {
			out << this->suprafeteBalcoane[i] << ", ";
		}
		out << this->suprafataLocParcare << endl;
	}

	//metoda care scrie in fisiere binare
	void writeToBinary(fstream& f) {
		Imobil::writeToBinary(f);

		f.write((char*)&this->etaj, sizeof(int));
		f.write((char*)&this->nrBalcoane, sizeof(int));
		for (int i = 0; i < this->nrBalcoane; i++) {
			f.write((char*)&this->suprafeteBalcoane[i], sizeof(float));
		}

		f.write((char*)&this->suprafataLocParcare, sizeof(float));
	}

	//metoda care citeste din fisiere binare
	void readFromBinary(fstream& f) {
		Imobil::readFromBinary(f);

		f.read((char*)&this->etaj, sizeof(int));
		f.read((char*)&this->nrBalcoane, sizeof(int));
		delete[] this->suprafeteBalcoane;
		this->suprafeteBalcoane = new float[this->nrBalcoane];
		for (int i = 0; i < this->nrBalcoane; i++) {
			f.read((char*)&this->suprafeteBalcoane[i], sizeof(float));
		}

		f.read((char*)&this->suprafataLocParcare, sizeof(float));
	}

	//metoda care citeste din fisiere text
	void readFromText(ifstream& in) {
		Imobil::readFromText(in);

		in >> this->etaj;
		in >> this->nrBalcoane;
		delete[] this->suprafeteBalcoane;
		this->suprafeteBalcoane = new float[this->nrBalcoane];
		for (int i = 0; i < this->nrBalcoane; i++) {
			in >> this->suprafeteBalcoane[i];
		}

		in >> this->suprafataLocParcare;

	}

	//metoda care scrie intr-un fisier text
	void writeToText(ofstream& out) {
		Imobil::writeToText(out);

		out << this->etaj << endl;
		out << this->nrBalcoane << endl;
		for (int i = 0; i < this->nrBalcoane; i++) {
			out << suprafeteBalcoane[i] << endl;
		}
		out << this->suprafataLocParcare << endl;

	}

};

class Agent {
	const int id;
	char* nume;
	static int nrAgenti;
	int aniVechime = 0;
	float* valoareAnualaVanzari = NULL;
	float valoareaUltimelorVanzari[3];

public:
	//constructor fara parametrii
	Agent() :id(++nrAgenti) {
		this->nume = new char[strlen("Necunoscut") + 1];
		strcpy(this->nume, "Necunoscut");
	};

	//constructor cu un parametru
	Agent(const char* nume) : id(++nrAgenti) {

		//validare nume
		if (nume != NULL && strlen(nume) > 1) {
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			this->nume = new char[strlen("Necunoscut") + 1];
			strcpy(this->nume, "Necunoscut");
		}
	}

	//constructor cu toti parametrii
	Agent(const char* nume, int aniVechime, float* valoareAnualaVanzari, float* valoareaUltimelorVanzari) : id(++nrAgenti) {

		//validare nume 
		if (nume != NULL && strlen(nume) > 1) {
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			this->nume = new char[strlen("Necunoscut") + 1];
			strcpy(this->nume, "Necunoscut");
		}

		//validare nr ani vechime si valoarea vanzarilor pe ani
		if (aniVechime > 0 && valoareAnualaVanzari != NULL) {
			this->aniVechime = aniVechime;
			this->valoareAnualaVanzari = new float[this->aniVechime];

			for (int i = 0; i < this->aniVechime; i++) {
				this->valoareAnualaVanzari[i] = valoareAnualaVanzari[i];
			}
		}

		//validare imobil ultimele vanzari
		bool ok = true;
		for (int i = 0; i < 3; i++) {
			if (valoareaUltimelorVanzari[i] < 0) {
				ok = false;
			}
		}
		if (ok == true) {
			for (int i = 0; i < 3; i++) {
				this->valoareaUltimelorVanzari[i] = valoareaUltimelorVanzari[i];
			}
		}
	}

	//constructor de copiere
	Agent(const Agent& a) :id(nrAgenti++) {

		this->nume = new char[strlen(a.nume) + 1];
		strcpy(this->nume, a.nume);

		//validare ani vechime si imobil anuala vanzari
		if (a.aniVechime > 0 && a.valoareAnualaVanzari != NULL) {
			this->aniVechime = a.aniVechime;
			this->valoareAnualaVanzari = new float[this->aniVechime];
			for (int i = 0; i < this->aniVechime; i++) {
				this->valoareAnualaVanzari[i] = a.valoareAnualaVanzari[i];
			}
		}

		for (int i = 0; i < 3; i++) {
			this->valoareaUltimelorVanzari[i] = a.valoareaUltimelorVanzari[i];
		}
	}

	//destructor
	~Agent() {
		delete[] this->nume;
		delete[] this->valoareAnualaVanzari;

	}

	//overload operator = 
	Agent& operator=(const Agent& a) {
		if (this != &a) {
			delete[] this->nume;
			this->nume = new char[strlen(a.nume) + 1];
			strcpy(this->nume, a.nume);

			if (a.aniVechime > 0 && a.valoareAnualaVanzari != NULL) {
				delete[] this->valoareAnualaVanzari;
				this->aniVechime = a.aniVechime;
				this->valoareAnualaVanzari = new float[this->aniVechime];
				for (int i = 0; i < this->aniVechime; i++) {
					this->valoareAnualaVanzari[i] = a.valoareAnualaVanzari[i];
				}
			}
			else {
				this->aniVechime = 0;
				this->valoareAnualaVanzari = NULL;
			}

			for (int i = 0; i < 3; i++) {
				this->valoareaUltimelorVanzari[i] = a.valoareaUltimelorVanzari[i];
			}
		}

		return *this;
	}



	//getters

	int getId() {
		return this->id;
	}

	static int getNrAgenti() {
		return Agent::nrAgenti;
	}

	char* getNume() {
		return this->nume;
	}

	int getNrAniVechime() {
		return this->aniVechime;
	}

	float* getValoareAnualaVanzari() {
		return this->valoareAnualaVanzari;
	}

	float* getValoareaUltimelorVanzari() {
		return this->valoareaUltimelorVanzari;
	}

	//setters
	void setNume(const char* nume) {
		if (nume != NULL && strlen(nume) > 1) {
			delete[] this->nume;
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			throw new CustomException("Numele introdus in setter nu este valid");
		}
	}

	void setValoareAnualaVanzari(int aniVechime, float* valoareAnualaVanzari) {

		//prima validare
		if (aniVechime > 0 && valoareAnualaVanzari != NULL) {

			//a doua validare(verifica valorile din vectorul primit ca parametru)
			bool ok = true;
			for (int i = 0; i < aniVechime; i++) {
				if (valoareAnualaVanzari[i] < 0) {
					ok = false;
				}
			}

			//daca toate elementele din vectorul primit sunt corespunzatoare se copiaza vectorul
			if (ok == true) {
				delete[] this->valoareAnualaVanzari;
				this->aniVechime = aniVechime;
				this->valoareAnualaVanzari = new float[this->aniVechime];
				for (int i = 0; i < this->aniVechime; i++) {
					this->valoareAnualaVanzari[i] = valoareAnualaVanzari[i];
				}
			}
			else {
				throw new CustomException("O valoare din vectorul valoareAnualaVanzari primit ca parametru nu este corespunzatoare");
			}
		}
		else {
			throw new CustomException("Numarul anilor vechime sau vectorul valoareAnualaVanzari nu sunt corespunzatoare");
		}

	}

	void setValoareaUltimelorVanzari(float valoareaUltimelorVanzari[3]) {

		//validare
		bool ok = true;
		for (int i = 0; i < 3; i++) {
			if (valoareaUltimelorVanzari[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 3; i++) {
				this->valoareaUltimelorVanzari[i] = valoareaUltimelorVanzari[i];
			}
		}
		else {
			throw new CustomException("O valoare din vectorul valorilor ultimelor vanzari nu este corespunzator");
		}
	}

	//overload operator [] -> returneaza valoarea veniturilor obtinute de agent in anul i
	float& operator[](int index) {
		if (index >= 0 && index < this->aniVechime) {
			return this->valoareAnualaVanzari[index];
		}
		else {
			throw new CustomException("Nu exista anul pe care doriti sa il accesati");
		}
	}

	//overload operator++ ->creste valoarea ultimelor vanzari cu 20%

	//preincrementare
	Agent& operator++() {
		for (int i = 0; i < 3; i++) {
			this->valoareaUltimelorVanzari[i] *= 1.2;
		}
		return *this;
	}

	//postincrementare
	Agent operator++(int) {
		Agent aux = *this;
		for (int i = 0; i < 3; i++) {
			this->valoareaUltimelorVanzari[i] *= 1.2;
		}
		return aux;
	}

	//overload operator -- ->scade valoarea ultimelor vanzari cu 20%

	//predecrementare
	Agent& operator--() {
		for (int i = 0; i < 3; i++) {
			this->valoareaUltimelorVanzari[i] *= 0.8;
		}
		return *this;
	}

	//postdecrementare
	Agent operator--(int) {
		Agent aux = *this;
		for (int i = 0; i < 3; i++) {
			this->valoareaUltimelorVanzari[i] *= 0.8;
		}
		return aux;
	}

	//overload cast la float -> returneaza valoarea totala a vanzarilor efectuate de agent de-a lungul carierei
	explicit operator float() {
		float sum = 0;
		for (int i = 0; i < this->aniVechime; i++) {
			sum += this->valoareAnualaVanzari[i];
		}

		return sum;
	}

	//overload operator +(float) ->agauga la vectorul valorii vanzarilor anuale valoarea (float) si incrementeaza numarul de ani vechime
	void operator+(float valoare) {
		if (valoare > 0) {
			Agent aux = *this;


			this->aniVechime++;
			delete[] this->valoareAnualaVanzari;
			this->valoareAnualaVanzari = new float[this->aniVechime];

			for (int i = 0; i < aux.aniVechime; i++) {
				this->valoareAnualaVanzari[i] = aux.valoareAnualaVanzari[i];
			}

			this->valoareAnualaVanzari[this->aniVechime - 1] = valoare;

		}
		else {
			throw new CustomException("Valoarea pe care incercati sa o adaugati la vectorul valorii vanzarilor anuale este negativa! ");
		}
	}

	//overload  operator > -> verifica valoarea totala a vanzarilor celor 2 agenti
	bool operator>(Agent& a) {
		return (float)(*this) > (float)a; //folosim castul la float pt a compara cele 2 valori
	}

	//overload operator < ->verifica valoarea totala a vanzarilor celor 2 agenti
	bool operator<(Agent& a) {
		return (float)(*this) < (float)a;
	}

	//overload operator >=
	bool operator >=(Agent& a) {
		return (float)(*this) >= (float)a;
	}

	//overload operator <=
	bool operator <=(Agent& a) {
		return (float)(*this) <= (float)a;
	}

	//overload operator ! -> verifica daca id-ul este 0, numele este 'Necunoscut', anii vehime 0, si vectorii de valori ale vanzarilor 0
	bool operator!() {


		//verificam daca vectorul valorii vanzarilor anuale are doar valori de 0
		bool ok1 = true;

		if (this->aniVechime > 0) {
			for (int i = 0; i < this->aniVechime; i++) {
				if (this->valoareAnualaVanzari[i] != 0) {
					ok1 = false;
				}
			}
		}



		//verificam daca vectorul valorilor ultimelor vanzari are doar valori de 0
		bool ok2 = true;
		for (int i = 0; i < 3; i++) {
			if (this->valoareaUltimelorVanzari[i] != 0) {
				ok2 = false;
			}
		}


		//verificarea finala
		if (this->id == 0 && strcmp(this->nume, "Necunoscut") == 0 && ok1 == true && ok2 == true) {
			return true;
		}
		else {
			return false;
		}
	}



	//overload operatror ==
	bool operator==(const Agent& a) {

		//verificam vectorul cu valorile anuale ale vanzarilor
		bool ok1 = true;
		if (this->aniVechime = a.aniVechime) {
			for (int i = 0; i < this->aniVechime; i++) {
				if (this->valoareAnualaVanzari[i] != a.valoareAnualaVanzari[i]) {
					ok1 = false;
				}
			}
		}
		else {
			ok1 = false;
		}

		//verificam vectorul valorilor ultimelor vanzari
		bool ok2 = true;
		for (int i = 0; i < 3; i++) {
			if (this->valoareaUltimelorVanzari[i] != a.valoareaUltimelorVanzari[i]) {
				ok2 = false;
			}
		}

		if (this->id == a.id && strcmp(this->nume, a.nume) == 0 && ok1 == true && ok2 == true) {
			return true;
		}
		else {
			return false;
		}
	}

	//metoda care afiseaza anii de experienta a agentului, in ordine crescatoare, in functie de valoarea vanzarilor
	void aniInOrdineCrescatoare() {

		//facem o copie a obiectului pentru a avea vectorul initial cu pozitionarile sale
		Agent aux = *this;

		//sortam vectorul de venituri prin algoritmul bubble sort
		for (int i = 0; i < this->aniVechime; i++) {
			for (int j = i + 1; j < this->aniVechime; j++) {
				if (this->valoareAnualaVanzari[j] < this->valoareAnualaVanzari[i]) {
					float temp = this->valoareAnualaVanzari[i];
					this->valoareAnualaVanzari[i] = this->valoareAnualaVanzari[j];
					this->valoareAnualaVanzari[j] = temp;
				}
			}
		}

		//trecem prin vectorul initial, si gasim anii in functie de pozitia valorilor din primul vector
		for (int i = 0; i < this->aniVechime; i++) { //prmul for trece prin vectorul sortat
			for (int j = 0; j < this->aniVechime; j++) { //al doilea for trece prin cel nesortat
				if (this->valoareAnualaVanzari[i] == aux.valoareAnualaVanzari[j]) {
					cout << endl << "Anul " << j + 1 << ": " << this->valoareAnualaVanzari[i] << "euro";
					break;
				}
			}
		}
	}

	//metoda care returneaza numarul anilor in care agentul a obtinut vanzari peste un prag dat
	int numarAniPesteValoare(float valoare) {
		int numar = 0;
		for (int i = 0; i < this->aniVechime; i++) {
			if (valoare <= this->valoareAnualaVanzari[i]) {
				numar++;
			}
		}

		return numar;
	}

	//metoda care calculeaza media valorii vanzarilor pe an
	float mediaValoriiVanzarilorPeAni() {
		float sum = 0;
		for (int i = 0; i < this->aniVechime; i++) {
			sum += this->valoareAnualaVanzari[i];
		}

		return sum / this->aniVechime;
	}

	//metoda care calculeaza suma valorilor vanzarilor
	float sumaValoriiVanzarilor() {
		float suma = 0;
		for (int i = 0; i < this->aniVechime; i++) {
			suma += this->valoareAnualaVanzari[i];
		}

		return suma;
	}


	//overload ostream
	friend ostream& operator<<(ostream& out, const Agent& a) {
		out << "\n------------------";
		out << "\nId agent: " << a.id;
		out << "\nNume: " << a.nume;
		out << "\nNr. ani vechime: " << a.aniVechime << " ani";
		out << "\nValori anuale ale vanzarilor: ";
		for (int i = 0; i < a.aniVechime; i++) {
			out << "\nAnul " << i + 1 << ": " << a.valoareAnualaVanzari[i] << " euro";
		}
		out << "\nValoarile ultimelor 3 vanzari: ";
		for (int i = 0; i < 3; i++) {
			out << "\nVanzarea " << i + 1 << ": " << a.valoareaUltimelorVanzari[i] << " euro";
		}

		return out;

	}

	//overload istream
	friend istream& operator>>(istream& in, Agent& a) {
		char aux[100];
		cout << "\nNume: ";
		in.getline(aux, 99);
		if (strlen(aux) > 1 && aux != NULL) {
			delete[] a.nume;
			a.nume = new char[strlen(aux) + 1];
			strcpy(a.nume, aux);
		}

		int aux2; //variabila care pastreaza nr de ani dati de la tastatura
		cout << "Nr ani vechime: "; in >> aux2;

		//validare nr de ani
		if (aux2 > 0) {
			cout << "Valori ale vanzarilor anuale:";
			float* aux3 = new float[aux2]; //vector auxiliar in care stocam valorile vanzarilor anuale introduse de la tastatura cu scopul de a le valida
			for (int i = 0; i < aux2; i++) {
				cout << "Anul " << i + i << ": ";
				cin >> aux3[i];
			}

			//validare vectorul de valori ale vanzarilor
			bool ok = true;
			for (int i = 0; i < aux2; i++) {
				if (aux3[i] < 0) {
					ok = false;
				}
			}

			//daca si valoarea anilor vechime si cele ale vectorului de valoriVanzari sunt validate, se aloca
			if (ok == true) {
				delete[] a.valoareAnualaVanzari;

				a.aniVechime = aux2;

				a.valoareAnualaVanzari = new float[a.aniVechime];

				for (int i = 0; i < a.aniVechime; i++) {
					a.valoareAnualaVanzari[i] = aux3[i];
				}
			}


		}

		cout << "Valoarea ultimelor 3 vanzari: ";

		float aux4[3];
		for (int i = 0; i < 3; i++) {
			cout << "Vanzarea " << i + 1 << ": ";
			in >> aux4[i];
		}

		//validam vectorul introdus
		bool ok = true;
		for (int i = 0; i < 3; i++) {
			if (aux4[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 3; i++) {
				a.valoareaUltimelorVanzari[i] = aux4[i];
			}
		}
		return in;

	}

	//metoda care scrie textul in fisiere csv
	void writeToCsv(ofstream& out) {
		out << this->id << ", " << this->nume << ", " << this->aniVechime << ", ";
		for (int i = 0; i < aniVechime; i++) {
			out << this->valoareAnualaVanzari[i] << ", ";
		}

		for (int i = 0; i < 3; i++) {
			out << this->valoareaUltimelorVanzari[i] << ", ";
		}

		out << endl;
	}

	//metoda care scrie in fisiere binare
	void writeToBinary(fstream& f) {

		f.write((char*)&this->id, sizeof(int));

		int dimensiune = strlen(this->nume) + 1;
		f.write((char*)&dimensiune, sizeof(int));

		f.write(nume, dimensiune);

		f.write((char*)&aniVechime, sizeof(int));

		for (int i = 0; i < this->aniVechime; i++) {
			f.write((char*)&valoareAnualaVanzari[i], sizeof(float));
		}

		for (int i = 0; i < 3; i++) {
			f.write((char*)&valoareaUltimelorVanzari[i], sizeof(float));
		}

	}

	//metoda care citeste din fisiere binare
	void readFromBinary(fstream& f) {

		int id = 0;
		f.read((char*)&id, sizeof(int));

		int lg = 0;
		f.read((char*)&lg, sizeof(int));

		delete[] this->nume;
		this->nume = new char[lg];
		char* buffer = new char[lg];

		f.read(buffer, lg);
		strcpy(this->nume, buffer);

		f.read((char*)&this->aniVechime, sizeof(int));

		delete[] this->valoareAnualaVanzari;
		this->valoareAnualaVanzari = new float[this->aniVechime];

		for (int i = 0; i < this->aniVechime; i++) {
			f.read((char*)&this->valoareAnualaVanzari[i], sizeof(float));
		}

		for (int i = 0; i < 3; i++) {
			f.read((char*)&this->valoareaUltimelorVanzari[i], sizeof(float));
		}

	}

	//metoda care citeste din fisiere text
	void readFromText(ifstream& in) {

		char buffer[100];
		in.getline(buffer, 99);
		delete[] this->nume;
		this->nume = new char[strlen(buffer) + 1];
		strcpy(this->nume, buffer);


		in >> this->aniVechime;

		delete[] this->valoareAnualaVanzari;
		this->valoareAnualaVanzari = new float[this->aniVechime];

		for (int i = 0; i < this->aniVechime; i++) {
			in >> this->valoareAnualaVanzari[i];
		}

		for (int i = 0; i < 3; i++) {
			in >> valoareaUltimelorVanzari[i];
		}
	}

	//metoda care scrie intr-un fisier text
	void writeToText(ofstream& out) {
		out << this->nume << endl;
		out << this->aniVechime << endl;

		for (int i = 0; i < aniVechime; i++) {
			out << this->valoareAnualaVanzari[i] << endl;
		}

		for (int i = 0; i < 3; i++) {
			out << this->valoareaUltimelorVanzari[i] << endl;
		}
	}
};

int Agent::nrAgenti = 0;

enum TIP_PACHET {
	BRONZE,
	SILVER,
	GOLD,
};
class Proprietar {
	int id = 0;
	static int numarMaximAnunturi;
	char* nume = NULL;
	int varsta = 0;
	TIP_PACHET tipPachet = TIP_PACHET::BRONZE;
	int numarAnunturi = 0;
	float* preturiImobile = NULL; //->va fi transformata in Imobil* imobile
	float preturiAbonamentLunar[6]; //->preturile abonamentului pentru promvarea anunturilor in urmtoarele 6 luni


public:

	//contructor fara parametrii
	Proprietar() :id(0) {
		this->nume = new char[strlen("Necunoscut") + 1];
		strcpy(this->nume, "Necunoscut");
	}

	//constructor cu 2 parametrii
	Proprietar(int id, const char* nume) :id(id > 0 ? id : 0) {
		if (nume != NULL) {
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			this->nume = new char[strlen("Necunoscut") + 1];
			strcpy(this->nume, "Necunoscut");
		}
	}

	//constructor cu toti parametrii
	Proprietar(int id, const char* nume, int varsta, TIP_PACHET tipPachet, int numarAnunturi, float* preturiImobile, float* preturiAbonamentLunar) {

		this->id = id;
		//validare nume
		if (nume != NULL) {
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			this->nume = new char[strlen("Necunoscut") + 1];
			strcpy(this->nume, "Necunoscut");
		}

		//validare varsta
		if (varsta > 0) {
			this->varsta = varsta;
		}

		this->tipPachet = tipPachet;

		//validare numar anunturi si preturi imobile
		if (numarAnunturi != 0 && numarAnunturi <= Proprietar::numarMaximAnunturi && preturiImobile != NULL) {

			//validare elemente vector preturi
			bool ok = true;
			for (int i = 0; i < numarAnunturi; i++) {
				if (preturiImobile[i] < 0) {
					ok = false;
				}
			}

			if (ok == true) {
				this->numarAnunturi = numarAnunturi;
				this->preturiImobile = new float[this->numarAnunturi];
				for (int i = 0; i < this->numarAnunturi; i++) {
					this->preturiImobile[i] = preturiImobile[i];
				}
			}
		}

		//validare preturi urmatoarele 6 luni
		bool ok = true;
		for (int i = 0; i < 6; i++) {
			if (preturiAbonamentLunar[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 6; i++) {
				this->preturiAbonamentLunar[i] = preturiAbonamentLunar[i];
			}

		}
	}

	//constructor de copiere
	Proprietar(const Proprietar& p) {
		this->id = p.id;
		this->nume = new char[strlen(p.nume) + 1];
		strcpy(this->nume, p.nume);

		this->varsta = p.varsta;

		this->tipPachet = p.tipPachet;

		//validare numar anunturi si preturi imobile
		if (p.numarAnunturi > 0 && p.numarAnunturi <= Proprietar::numarMaximAnunturi && p.preturiImobile != NULL) {
			this->numarAnunturi = p.numarAnunturi;
			this->preturiImobile = new float[this->numarAnunturi];
			for (int i = 0; i < this->numarAnunturi; i++) {
				this->preturiImobile[i] = p.preturiImobile[i];
			}
		}

		for (int i = 0; i < 6; i++) {
			this->preturiAbonamentLunar[i] = p.preturiAbonamentLunar[i];
		}
	}

	//destructor
	~Proprietar() {
		delete[] this->nume;
		delete[] this->preturiImobile;
	}

	//getters

	int getId() {
		return this->id;
	}

	int getNumarMaximAnunturi() {
		return Proprietar::numarMaximAnunturi;
	}

	char* getNume() {
		return this->nume;
	}

	int getVarsta() {
		return this->varsta;
	}

	TIP_PACHET getTipPachet() {
		return this->tipPachet;
	}


	int getNumarAnunturi() {
		return this->numarAnunturi;
	}

	float* getPreturiImobile() {
		return this->preturiImobile;
	}


	float* getPreturiAbonamentLunar() {
		return this->preturiAbonamentLunar;
	}

	//setters
	void setNumarMaximAnunturi(int numar) {
		if (numar > Proprietar::numarMaximAnunturi) {
			Proprietar::numarMaximAnunturi = numar;
		}
		else {
			throw new CustomException("Numarul maxim de anunturi nu poate scadea");
		}
	}

	void setNume(const char* nume) {
		if (nume != NULL) {
			delete[] this->nume;
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
	}

	void setVarsta(int varsta) {
		if (varsta > 0) {
			this->varsta = varsta;
		}
		else {
			throw new CustomException("Varsta nu este corespunzatoare");
		}
	}

	void setTipPachet(TIP_PACHET tipPachet) {
		this->tipPachet = tipPachet;
	}

	void setPreturiImobile(int numarAnunturi, float* preturiImobile) {

		if (numarAnunturi != 0 && numarAnunturi <= Proprietar::numarMaximAnunturi && preturiImobile != NULL) {

			//validare elemente vector preturi
			bool ok = true;
			for (int i = 0; i < numarAnunturi; i++) {
				if (preturiImobile[i] < 0) {
					ok = false;
				}
			}

			if (ok == true) {
				this->numarAnunturi = numarAnunturi;
				this->preturiImobile = new float[this->numarAnunturi];
				for (int i = 0; i < this->numarAnunturi; i++) {
					this->preturiImobile[i] = preturiImobile[i];
				}
			}
			else {
				throw new CustomException("Preturile imobilelor nu pot fi negative");
			}
		}
		else {
			throw new CustomException("Numarul anunturilor sau vectorul preturilor nu este corespunzator");
		}
	}

	void setPreturiAbonamentLunar(float* preturiAbonamentLunar) {
		bool ok = true;
		for (int i = 0; i < 6; i++) {
			if (preturiAbonamentLunar[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 6; i++) {
				this->preturiAbonamentLunar[i] = preturiAbonamentLunar[i];
			}

		}
		else {
			throw new CustomException("Preturile lunare ale abonamentului nu pot fi negative");
		}
	}

	//overload operator = 
	Proprietar& operator=(const Proprietar& p) {
		if (this != &p) {
			this->id = p.id;
			this->nume = new char[strlen(p.nume) + 1];
			strcpy(this->nume, p.nume);

			this->varsta = p.varsta;

			this->tipPachet = p.tipPachet;

			//validare numar anunturi si preturi imobile
			if (p.numarAnunturi > 0 && p.numarAnunturi <= Proprietar::numarMaximAnunturi && p.preturiImobile != NULL) {
				this->numarAnunturi = p.numarAnunturi;
				this->preturiImobile = new float[this->numarAnunturi];
				for (int i = 0; i < this->numarAnunturi; i++) {
					this->preturiImobile[i] = p.preturiImobile[i];
				}
			}

			for (int i = 0; i < 6; i++) {
				this->preturiAbonamentLunar[i] = p.preturiAbonamentLunar[i];
			}
		}
		return *this;
	}

	//overload operator [] ->returneaza valoarea imobilului i
	float& operator[](int index) {
		if (index >= 0 && index < this->numarAnunturi) {
			return this->preturiImobile[index];
		}
	}

	//overload operator += (float) -> adauga un anunt cu valoarea imbilului float
	Proprietar& operator+=(float valoare) {
		if (valoare > 0) {
			Proprietar aux = *this;

			delete[] this->preturiImobile;
			this->numarAnunturi++;
			this->preturiImobile = new float[this->numarAnunturi];

			for (int i = 0; i < aux.numarAnunturi; i++) {
				this->preturiImobile[i] = aux.preturiImobile[i];
			}
			this->preturiImobile[this->numarAnunturi - 1] = valoare;
			return *this;
		}
		else {
			throw new CustomException("Anuntul adaugat trebuie sa aiba un pret pozitiv");
		}

	}

	//overload operator -=(int) ->elimina int anunturi si preturile acestora
	Proprietar& operator-=(int numar) {
		if (numar > 0 && numar < this->numarAnunturi) {
			Proprietar aux = *this;
			delete[] this->preturiImobile;
			this->numarAnunturi -= numar;
			this->preturiImobile = new float[this->numarAnunturi];
			for (int i = 0; i < this->numarAnunturi; i++) {
				this->preturiImobile[i] = aux.preturiImobile[i];
			}
			return *this;
		}
		else {
			throw new CustomException("Nu exista suficiente anunturi postate");
		}
	}

	//overload operator cast la float -> returneaza valoarea totala a imobilelor detinute
	explicit operator float() {
		float suma = 0;

		for (int i = 0; i < this->numarAnunturi; i++) {
			suma += this->preturiImobile[i];
		}

		return suma;
	}

	//overload operator ++ -> creste pretul abonamentului lunar cu 10%


		//precrementare
	Proprietar& operator++() {
		for (int i = 0; i < 6; i++) {
			this->preturiAbonamentLunar[i] *= 1.1;
		}
		return *this;
	}

	//postdecrementare
	Proprietar operator++(int) {
		Proprietar aux = *this;
		for (int i = 0; i < 6; i++) {
			this->preturiAbonamentLunar[i] *= 1.1;
		}
		return aux;
	}

	//overload operator -- -> scade pretul abonamentului lunar cu 10%

		//predecrementare
	Proprietar& operator--() {
		for (int i = 0; i < 6; i++) {
			this->preturiAbonamentLunar[i] *= 0.9;
		}
		return *this;
	}

	//postdecrementare
	Proprietar operator--(int) {
		Proprietar aux = *this;
		for (int i = 0; i < 6; i++) {
			this->preturiAbonamentLunar[i] *= 0.9;
		}
		return aux;
	}

	//overload operatori > < ->compara valoarea totala a imobilelor celor 2 proprietari

	bool operator>(Proprietar& p) {
		return (float)*this > (float)p;
	}

	bool operator<(Proprietar& p) {
		return (float)*this < (float)p;
	}

	//overload operator ! -> verifica toate atributele

	bool operator!() {

		//verificam vectorul pretului abonamentului lunar
		bool ok = true;
		for (int i = 0; i < 6; i++) {
			if (this->preturiAbonamentLunar[i] != 0) {
				ok = false;
			}
		}

		if (this->id == 0 && strcmp(this->nume, "Necunoscut") == 0 && this->varsta == 0 && this->tipPachet == TIP_PACHET::BRONZE && this->numarAnunturi == 0 && ok == true) {
			return true;
		}
		else {
			return false;
		}
	}

	//overload operator == -> compara toate atributele

	bool operator==(Proprietar& p) {

		bool ok1 = true;
		if (this->numarAnunturi == p.numarAnunturi) {
			for (int i = 0; i < this->numarAnunturi; i++) {
				if (this->preturiImobile[i] != p.preturiImobile[i]) {
					ok1 = false;
				}
			}
		}
		else {
			ok1 = false;
		}


		bool ok2 = true;

		for (int i = 0; i < 6; i++) {
			if (this->preturiAbonamentLunar[i] != p.preturiAbonamentLunar[i]) {
				ok2 = false;
			}
		}

		if (this->id == p.id && strcmp(this->nume, p.nume) == 0 && this->varsta == p.varsta && this->tipPachet == p.tipPachet && ok1 == true && ok2 == true) {
			return true;
		}
		else {
			return false;
		}

	}

	//metoda care caluleaza suma de plata a proprietarului in functie de tipul de abonament
	float calculSumaDePlata() {
		float suma = 0;
		for (int i = 0; i < 6; i++) {
			suma += this->preturiAbonamentLunar[i];
		}

		switch (this->tipPachet)
		{
		case TIP_PACHET::BRONZE: //daca tipul pachetului este bronze proprietarul plateste toata suma
			return suma;
			break;
		case TIP_PACHET::SILVER: //la silver plateste 80%
			return suma * 0.8;
			break;
		case TIP_PACHET::GOLD: //la gold plateste 60%
			return suma * 0.6;
			break;
		default:
			break;
		}

	}

	//metoda care calculeaza valoarea medie a imbilelor pe care le detine proprietarul

	float calculValoareMedieImobile() {

		if (this->numarAnunturi != 0) {
			return (float)*this / this->numarAnunturi;
		}
		else {
			return 0;
		}

	}




	//overload ostream
	friend ostream& operator<<(ostream& out, const Proprietar& p) {
		out << "\n------------";
		out << "\nId: " << p.id << endl;
		out << "Nume: " << p.nume << endl;
		out << "Varsta: " << p.varsta << endl;
		out << "Tip pachet: " << Proprietar::enumToString(p.tipPachet) << endl;
		out << "Numar anunturi: " << p.numarAnunturi << endl;
		out << "Preturi imobile: " << endl;
		for (int i = 0; i < p.numarAnunturi; i++) {
			out << "Imobil " << i + 1 << ": " << p.preturiImobile[i] << " euro" << endl;
		}

		out << "Preturi abonament lunar in urmatoarele 6 luni: ";
		for (int i = 0; i < 6; i++) {
			out << p.preturiAbonamentLunar[i] << " ";
		}

		out << endl;
		return out;
	}

	//overload istream
	friend istream& operator>>(istream& in, Proprietar& p) {
		cout << "Id: ";
		in >> p.id;
		in.ignore();
		char aux[100];

		cout << "\nNume: ";
		in.getline(aux, 99);
		delete[] p.nume;
		p.nume = new char[strlen(aux) + 1];
		strcpy(p.nume, aux);

		cout << "Varsta: "; in >> p.varsta;

		string aux2;
		cout << "Tip Pachet(bronze/silver/gold): ";
		in.ignore();
		getline(in, aux2);
		p.tipPachet = stringToEnum(aux2);

		cout << "Numar anunturi: ";
		in >> p.numarAnunturi;


		delete[] p.preturiImobile;

		p.preturiImobile = new float[p.numarAnunturi];

		cout << "Preturi imobile: ";
		for (int i = 0; i < p.numarAnunturi; i++) {
			cout << "Imobilul " << i + 1 << ": ";
			in >> p.preturiImobile[i];
		}

		cout << "Preturi abonament urmatoarele 6 luni: ";
		for (int i = 0; i < 6; i++) {
			cout << "Luna " << i + 1 << ": ";
			in >> p.preturiAbonamentLunar[i];
		}

		return  in;


	}

	static string enumToString(TIP_PACHET tipPachet) {
		switch (tipPachet)
		{
		case BRONZE:
			return "Bronze";
			break;
		case SILVER:
			return "Silver";
			break;
		case GOLD:
			return "Gold";
			break;
		default:
			break;
		}
	}

	static TIP_PACHET stringToEnum(string tipPachet) {

		//tranformam inputul in string cu litere mici
		std::transform(tipPachet.begin(), tipPachet.end(), tipPachet.begin(), std::tolower);

		if (tipPachet == "bronze") {
			return TIP_PACHET::BRONZE;
		}
		else if (tipPachet == "silver") {
			return TIP_PACHET::SILVER;
		}
		else if (tipPachet == "gold") {
			return TIP_PACHET::GOLD;
		}
	}

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) {
		out << this->id << ", " << this->nume << ", " << this->varsta << ", " << Proprietar::enumToString(this->tipPachet) << ", " << this->numarAnunturi << ", ";
		for (int i = 0; i < this->numarAnunturi; i++) {
			out << this->preturiImobile[i] << ", ";
		}

		for (int i = 0; i < 6; i++) {
			out << this->preturiAbonamentLunar[i] << ", ";
		}

		out << endl;
	}

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {
		//scriem id
		f.write((char*)&this->id, sizeof(int));

		//scriem nume
		int lungime = strlen(this->nume) + 1;
		f.write((char*)&lungime, sizeof(int));

		f.write(this->nume, lungime);

		//scriem varsta
		f.write((char*)&this->varsta, sizeof(int));

		//scriem tipul pachetului
		f.write((char*)&this->tipPachet, sizeof(TIP_PACHET));

		//scriem numarul de anunturi
		f.write((char*)&this->numarAnunturi, sizeof(int));

		//scriem preturile imobilelor
		for (int i = 0; i < this->numarAnunturi; i++) {
			f.write((char*)&this->preturiImobile[i], sizeof(float));
		}

		//scriem preturile abonamentului
		for (int i = 0; i < 6; i++) {
			f.write((char*)&this->preturiAbonamentLunar[i], sizeof(float));
		}



	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {
		f.read((char*)&this->id, sizeof(int));

		//citim numele
		int lg = 0;
		f.read((char*)&lg, sizeof(int));
		delete[] this->nume;
		this->nume = new char[lg];
		char* buffer = new char[lg];
		f.read(buffer, lg);
		strcpy(this->nume, buffer);

		//citim varsta
		f.read((char*)&this->varsta, sizeof(int));

		//citim tipul pachetului
		f.read((char*)&tipPachet, sizeof(TIP_PACHET));

		//citim numarul anunturilor
		f.read((char*)&numarAnunturi, sizeof(int));
		delete[] this->preturiImobile;
		this->preturiImobile = new float[this->numarAnunturi];

		//citim preturile imobilelor
		for (int i = 0; i < this->numarAnunturi; i++) {
			f.read((char*)&this->preturiImobile[i], sizeof(float));
		}

		//citim preturile abonamentului
		for (int i = 0; i < 6; i++) {
			f.read((char*)&this->preturiAbonamentLunar[i], sizeof(float));
		}

	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		in >> this->id;
		char buffer[100];
		in.ignore();
		in.getline(buffer, 99);
		delete[] this->nume;
		this->nume = new char[strlen(buffer) + 1];
		strcpy(this->nume, buffer);
		in >> this->varsta;
		string numePachet;
		in >> numePachet;
		this->tipPachet = Proprietar::stringToEnum(numePachet);

		in >> this->numarAnunturi;
		delete[] this->preturiImobile;
		this->preturiImobile = new float[this->numarAnunturi];
		for (int i = 0; i < this->numarAnunturi; i++) {
			in >> this->preturiImobile[i];
		}

		for (int i = 0; i < 6; i++) {
			in >> this->preturiAbonamentLunar[i];
		}
	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		out << this->id << endl;
		out << this->nume << endl;
		out << this->varsta << endl;
		out << Proprietar::enumToString(this->tipPachet) << endl;
		out << this->numarAnunturi << endl;
		for (int i = 0; i < this->numarAnunturi; i++) {
			out << this->preturiImobile[i] << endl;
		}

		for (int i = 0; i < 6; i++) {
			out << preturiAbonamentLunar[i] << endl;
		}
	}


};
int Proprietar::numarMaximAnunturi = 5;

class Client {
	int id = 0;
	static float venituriMinimeEligibilitateFinantare;
	char* nume = NULL;
	int varsta = 0;
	float venituriUltimulAn[12];
	bool doresteFinantare = false;
	int numarImobileDorite = 0;
	float* valoriImobileDorite = NULL;


public:
	//constructor fara parametrii
	Client() {
		this->nume = new char[strlen("Necunoscut") + 1];
		strcpy(this->nume, "Necunoscut");
	};

	//constructor cu 2 parametrii
	Client(int id, const char* nume) {
		this->id = id;
		this->nume = new char[strlen(nume) + 1];
		strcpy(this->nume, nume);
	}

	//constructor cu toti parametrii
	Client(int id, const char* nume, int varsta, float* venituriUltimulAn, bool doresteFinantare, int nrImobileDorite, float* valoriImobileDorite) {

		this->id = id;
		//validare nume
		if (nume != NULL && strlen(nume) > 0) {
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			this->nume = new char[strlen("Necunoscut") + 1];
			strcpy(this->nume, "Necunoscut");
		}

		//validare varsta
		if (varsta > 18) {
			this->varsta = varsta;
		}

		this->doresteFinantare = doresteFinantare;

		//validare vector venituri ultimul an
		bool ok1 = true;

		for (int i = 0; i < 12; i++) {
			if (venituriUltimulAn[i] < 0) {
				ok1 = false;
			}
		}

		if (ok1 == true) {
			for (int i = 0; i < 12; i++) {
				this->venituriUltimulAn[i] = venituriUltimulAn[i];
			}
		}


		//validare nr imobile dorite si vectorul cu valorile acestora
		if (nrImobileDorite > 0 && valoriImobileDorite != NULL) {
			this->numarImobileDorite = nrImobileDorite;
			this->valoriImobileDorite = new float[this->numarImobileDorite];

			for (int i = 0; i < this->numarImobileDorite; i++) {
				this->valoriImobileDorite[i] = valoriImobileDorite[i];
			}
		}
	}

	//constructor de copiere
	Client(const Client& c) :id(c.id) {

		this->id = c.id;

		//initializam varsta
		this->nume = new char[strlen(c.nume) + 1];
		strcpy(this->nume, c.nume);

		//initializam numele
		this->varsta = c.varsta;

		//initializam vectorul veniturilor
		for (int i = 0; i < 12; i++) {
			this->venituriUltimulAn[i] = c.venituriUltimulAn[i];

			//initializam daca clientul doreste finantare
			this->doresteFinantare = c.doresteFinantare;

			//initializam nrImobile dorite si valorile acestora
			this->numarImobileDorite = c.numarImobileDorite;
			this->valoriImobileDorite = new float[this->numarImobileDorite];

			for (int i = 0; i < this->numarImobileDorite; i++) {
				this->valoriImobileDorite[i] = c.valoriImobileDorite[i];
			}
		}
	}

	//destructor
	~Client() {
		delete[] this->nume;
		delete[] this->valoriImobileDorite;
	}



	//overload operator = 
	Client& operator=(const Client& c) {
		if (this != &c) {
			this->id = c.id;
			delete[] this->nume;
			this->nume = new char[strlen(c.nume) + 1];
			strcpy(this->nume, c.nume);


			this->varsta = c.varsta;


			for (int i = 0; i < 12; i++) {
				this->venituriUltimulAn[i] = c.venituriUltimulAn[i];


				this->doresteFinantare = c.doresteFinantare;

				delete[] this->valoriImobileDorite;
				this->numarImobileDorite = c.numarImobileDorite;
				this->valoriImobileDorite = new float[this->numarImobileDorite];

				for (int i = 0; i < this->numarImobileDorite; i++) {
					this->valoriImobileDorite[i] = c.valoriImobileDorite[i];
				}
			}
		}


		return *this;
	}

	//getters

	static float getVenituriMinimeEligibilitateFinantare() {
		return Client::venituriMinimeEligibilitateFinantare;
	}

	int getId() {
		return this->id;
	}

	char* getNume() {
		return this->nume;
	}

	int getVarsta() {
		return this->varsta;
	}

	float* getVenituriUltimulAn() {
		return this->venituriUltimulAn;
	}

	string getDoresteFinantare() {
		return Client::boolToString(this->doresteFinantare);
	}

	int getNrImobileDorite() {
		return this->numarImobileDorite;
	}

	float* getValoriImobileDorite() {
		return this->valoriImobileDorite;
	}

	//setters

	static void setVenituriMinimeEligibilitateFinantare(float valoare) {
		if (valoare > Client::venituriMinimeEligibilitateFinantare) {
			Client::venituriMinimeEligibilitateFinantare = valoare;
		}
		else {
			throw new CustomException("Valoarea minima pentru ca un client sa fie eligibil spre a primi finantare nu poate fi scazuta");
		}
	}


	void setNume(const char* nume) {
		if (nume != NULL && strlen(nume) > 1) {
			delete[] this->nume;
			this->nume = new char[strlen(nume) + 1];
			strcpy(this->nume, nume);
		}
		else {
			throw new CustomException("Numele introdus in setter nu este corespunzator");
		}


	}

	void setDoresteFinantare(string doresteFinantare) {
		//modificam stringul in litere mici pentru a putea fi comparat
		std::transform(doresteFinantare.begin(), doresteFinantare.end(), doresteFinantare.begin(), std::tolower);
		if (doresteFinantare == "da" || doresteFinantare == "nu") {
			this->doresteFinantare = stringToBool(doresteFinantare);
		}
		else {
			throw new CustomException("Valoarea string pentru doresteFinantare trebuie sa fie da/nu");
		}
	}

	void setVarsta(int varsta) {
		if (varsta > 18) {
			this->varsta = varsta;
		}
		else {
			throw new CustomException("Varsta clientului trebuie sa fie mai mare decat 18 ani");
		}
	}

	void setVenituriUltimulAn(float* venituriUltimulAn) {
		bool ok = true;
		for (int i = 0; i < 12; i++) {
			if (venituriUltimulAn[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			for (int i = 0; i < 12; i++) {
				this->venituriUltimulAn[i] = venituriUltimulAn[i];
			}
		}
		else {
			throw new CustomException("Veniturile din ultimul an trebuie sa fie > 0");
		}
	}

	void setValoriImobileDorite(int numarImobileDorite, float* valoriImobileDorite) {

		bool ok = true;

		//prima validare
		if (numarImobileDorite > 0 && valoriImobileDorite != NULL) {
			//a doua validare
			for (int i = 0; i < numarImobileDorite; i++) {
				if (valoriImobileDorite[i] < 0) {
					ok = false;
				}
			}
		}
		else {
			ok = false;
		}

		if (ok == true) {
			delete[] this->valoriImobileDorite;
			this->numarImobileDorite = numarImobileDorite;
			this->valoriImobileDorite = new float[this->numarImobileDorite];
			for (int i = 0; i < this->numarImobileDorite; i++) {
				this->valoriImobileDorite[i] = valoriImobileDorite[i];
			}
		}
		else {
			throw new CustomException("Numarul imobilelor dorite de catre client trebuie sa fie > 0 si valorileImbilelor trebuie sa fie pozitive");
		}

	}

	//overload [] ->returneaza venitul din luna i
	float& operator[](int index) {
		if (index >= 0 && index < 12) {
			return this->venituriUltimulAn[index];
		}
	}

	//overload cast float ->returneaza venitul total pe ultimul an
	explicit operator float() {
		float sum = 0;
		for (int i = 0; i < 12; i++) {
			sum += this->venituriUltimulAn[i];
		}

		return sum;
	}

	//overload operator + (Client) -> aduna veniturile totale ale celor 2 clienti pe ultimul an
	float operator+(Client& c) {
		return (float)*this + (float)c;
	}

	//overload operator -= (float) ->elimina din vectorul valorii imobilelor dorite elementul cu valoarea float
	Client& operator-=(float valoare) {

		if (valoare > 0) {
			Client aux = *this;

			delete[] this->valoriImobileDorite;

			this->numarImobileDorite--;
			this->valoriImobileDorite = new float[this->numarImobileDorite];

			int k = 0;
			for (int i = 0; i < aux.numarImobileDorite; i++) {
				if (aux.valoriImobileDorite[i] != valoare) {
					this->valoriImobileDorite[k++] = aux.valoriImobileDorite[i];
				}

			}
			return *this;
		}
		else {
			throw new CustomException("Valoarea care trebuie eliminata trebuie sa fie pozitiva");
		}
	}

	//overload operator ++ ->creste valoarea imobilelor dorite cu 10%

		//preincrementare
	Client& operator++() {
		for (int i = 0; i < this->numarImobileDorite; i++) {
			this->valoriImobileDorite[i] *= 1.1;
		}
		return *this;
	}

	//postincrementare
	Client operator++(int) {
		Client aux = *this;
		for (int i = 0; i < this->numarImobileDorite; i++) {
			this->valoriImobileDorite[i] *= 1.1;
		}

		return aux;
	}

	//overload operator -- -> scade valoarea imobilelor dorite cu 10%

		//predecrementare
	Client& operator--() {
		for (int i = 0; i < this->numarImobileDorite; i++) {
			this->valoriImobileDorite[i] *= 0.9;
		}
		return *this;
	}

	//postdecrementare
	Client operator--(int) {
		Client aux = *this;
		for (int i = 0; i < this->numarImobileDorite; i++) {
			this->valoriImobileDorite[i] *= 0.9;
		}

		return aux;
	}


	//overload operatori > < ->compara veniturile totale ale celor 2 clienti

	bool operator>(Client& c) {
		return (float)*this > (float)c;
	}

	bool operator<(Client& c) {
		return (float)*this < (float)c;
	}


	//overload operator ! -> verifica toate atributele

	bool operator!() {


		//verificam vectorul veniturilor
		bool ok = true;
		for (int i = 0; i < 12; i++) {
			if (this->venituriUltimulAn[i] != 0) {
				ok = false;
			}
		}

		if (this->id == 0 && strcmp(this->nume, "Necunoscut") == 0 && this->varsta == 0 && this->doresteFinantare == false && ok == true && this->numarImobileDorite == 0) {
			return true;
		}
		else {
			return false;
		}
	}


	//overload operator == ->compara toate atributele

	bool operator==(Client& c) {

		//verificam vectorul veniturilor
		bool ok1 = true;
		for (int i = 0; i < 12; i++) {
			if (this->venituriUltimulAn[i] != c.venituriUltimulAn[i]) {
				ok1 = false;
			}
		}

		//verificam vectorul valorii imobilelor dorite
		bool ok2 = true;
		if (this->numarImobileDorite == c.numarImobileDorite) {
			for (int i = 0; i < this->numarImobileDorite; i++) {
				if (this->valoriImobileDorite[i] != c.valoriImobileDorite[i]) {
					ok2 = false;
				}
			}
		}
		else {
			ok2 = false;
		}

		if (this->id == c.id && strcmp(this->nume, c.nume) == 0 && this->varsta == c.varsta && this->doresteFinantare == c.doresteFinantare && ok1 == true && ok2 == true) {
			return true;
		}
		else {
			return false;
		}

	}

	//metoda care calculeaza daca clientul este eligibil pentru finantare
	bool esteEligibil() {
		return (float)*this > Client::venituriMinimeEligibilitateFinantare;
	}


	//metoda care calculeaza daca clientul isi poate permite imobilele dorite in functie de venit si de finantare
	bool poateCumparaImobile() {
		//calculam suma valorii imobilelo pe care le doreste
		float suma = 0;
		for (int i = 0; i < this->numarImobileDorite; i++) {
			suma += this->valoriImobileDorite[i];
		}
		//daca clientul este eligibil pt finantare el va plati doar 60%
		if (this->esteEligibil()) {
			suma *= 0.6;
		}

		return (float)*this > suma;
	}

	//metoda care calculeaza venitul mediu lunar al clientului
	float venitLunarMediu() {
		return (float)*this / 12;
	}

	//overload ostream
	friend ostream& operator<<(ostream& out, const Client& c) {
		out << "\n----------------";
		out << "\nId: " << c.id << endl;
		out << "Nume: " << c.nume << endl;
		out << "Varsta: " << c.varsta << endl;
		out << "Veniturile din ultimul an: ";
		for (int i = 0; i < 12; i++) {
			out << c.venituriUltimulAn[i] << " ";
		}
		out << endl;
		out << "Doreste finantare?: " << Client::boolToString(c.doresteFinantare) << endl;
		out << "Numar imobile dorite: " << c.numarImobileDorite << endl;
		out << "Valori imobile dorite: " << endl;
		for (int i = 0; i < c.numarImobileDorite; i++) {
			out << "Valoare imobil " << i + 1 << ": " << c.valoriImobileDorite[i] << " euro" << endl;
		}

		return out;

	}

	//overload istream
	friend istream& operator>>(istream& in, Client& c) {

		cout << "\nId: ";
		cin >> c.id;
		in.ignore();
		char aux[100];
		cout << "Nume: ";
		cin.getline(aux, 99);
		delete[] c.nume;
		c.nume = new char[strlen(aux) + 1];
		strcpy(c.nume, aux);

		cout << "Varsta: "; in >> c.varsta;

		string aux2;
		cout << "Doreste finantare? (da/nu): ";
		cin >> aux2;
		c.doresteFinantare = stringToBool(aux2);


		cout << "Veniruti ultimul an: ";
		for (int i = 0; i < 12; i++) {
			cout << "Luna " << i + 1 << ": "; in >> c.venituriUltimulAn[i];
		}

		delete[] c.valoriImobileDorite;
		cout << "Nr imobile dorite: "; in >> c.numarImobileDorite;
		c.valoriImobileDorite = new float[c.numarImobileDorite];
		for (int i = 0; i < c.numarImobileDorite; i++) {
			cout << "Imobil " << i + 1 << ": "; in >> c.valoriImobileDorite[i];
		}

		return in;

	}


	//metoda care tranforma valoarea bool in string pentru afisare
	static string boolToString(bool valoare) {
		switch (valoare)
		{
		case false:
			return "Nu";
			break;
		case true:
			return "Da";
			break;
		}
	}



	//metoda care tranforma valoarea string in bool pentru atriburie
	static bool stringToBool(string doresteFinantare) {

		//tranformam stringul in lietere mici pentru a putea fi comparat
		std::transform(doresteFinantare.begin(), doresteFinantare.end(), doresteFinantare.begin(), std::tolower);

		if (doresteFinantare == "da") {
			return true;
		}
		else if (doresteFinantare == "nu") {
			return false;
		}
		else {
			return false;
		}
	}

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) {
		out << this->id << ", " << this->nume << ", " << this->varsta << ", ";
		for (int i = 0; i < 12; i++) {
			out << this->venituriUltimulAn[i] << ", ";
		}
		out << Client::boolToString(this->doresteFinantare) << ", " << this->numarImobileDorite << ", ";
		for (int i = 0; i < this->numarImobileDorite; i++) {
			out << this->valoriImobileDorite[i] << ", ";
		}

		out << endl;
	}

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {
		f.write((char*)&this->id, sizeof(int));

		//scriem numele
		int lungime = strlen(this->nume) + 1;
		f.write((char*)&lungime, sizeof(int));
		f.write(this->nume, lungime);

		//scriem varsta
		f.write((char*)&this->varsta, sizeof(int));

		//scriem veniturile din ultimul an
		for (int i = 0; i < 12; i++) {
			f.write((char*)&this->venituriUltimulAn[i], sizeof(float));
		}

		//scriem daca doreste finanare
		f.write((char*)&this->doresteFinantare, sizeof(bool));

		//scriem nr imobilelor dorite
		f.write((char*)&this->numarImobileDorite, sizeof(float));

		//scriem valorile imobilelor dorite
		for (int i = 0; i < this->numarImobileDorite; i++) {
			f.write((char*)&this->valoriImobileDorite[i], sizeof(float));
		}
	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {
		f.read((char*)&this->id, sizeof(int));

		//citim numele
		int lg;
		f.read((char*)&lg, sizeof(int));
		char* buffer = new char[lg];
		f.read(buffer, lg);
		delete[] this->nume;
		this->nume = new char[strlen(buffer) + 1];
		strcpy(this->nume, buffer);

		//citim varsta
		f.read((char*)&this->varsta, sizeof(int));

		//citim veniturile din ultimul an
		for (int i = 0; i < 12; i++) {
			f.read((char*)&this->venituriUltimulAn[i], sizeof(float));
		}

		//scriem daca doreste finanare
		f.read((char*)&this->doresteFinantare, sizeof(bool));

		//scriem nr imobilelor dorite
		f.read((char*)&this->numarImobileDorite, sizeof(float));

		delete[] this->valoriImobileDorite;
		this->valoriImobileDorite = new float[this->numarImobileDorite];

		//scriem valorile imobilelor dorite
		for (int i = 0; i < this->numarImobileDorite; i++) {
			f.read((char*)&this->valoriImobileDorite[i], sizeof(float));
		}



	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		in >> this->id;

		char buffer[100];
		in.ignore();
		in.getline(buffer, 99);
		delete[] this->nume;
		this->nume = new char[strlen(buffer) + 1];
		strcpy(this->nume, buffer);

		in >> this->varsta;
		for (int i = 0; i < 12; i++) {
			in >> this->venituriUltimulAn[i];
		}

		string boolString;
		in >> boolString;
		this->doresteFinantare = Client::stringToBool(boolString);
		in >> this->numarImobileDorite;
		delete[] this->valoriImobileDorite;
		this->valoriImobileDorite = new float[this->numarImobileDorite];
		for (int i = 0; i < this->numarImobileDorite; i++) {
			in >> this->valoriImobileDorite[i];
		}


	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		out << this->id << endl;
		out << this->nume << endl;
		out << this->varsta << endl;
		for (int i = 0; i < 12; i++) {
			out << this->venituriUltimulAn[i] << endl;
		}
		out << Client::boolToString(this->doresteFinantare) << endl;
		out << this->numarImobileDorite << endl;
		for (int i = 0; i < this->numarImobileDorite; i++) {
			out << this->valoriImobileDorite[i] << endl;
		}

	}
};
float Client::venituriMinimeEligibilitateFinantare = 10000;

class Contract {
	string serie = "N/A";
	static int nrContracte;
protected:
	Client client;
	Agent agent;
	float valoare = 0;
	float comisioane[2];

public:

	Contract() :serie("N/A") {};


	//constructor cu 3 parametrii
	Contract(string serie, Client client, Agent agent) {

		this->serie = serie;
		this->client = client;
		this->agent = agent;
	}

	//constructor cu toti parametrii
	Contract(string serie, Client client, Agent agent, float valoare, float* comisioane) {

		//initializam seria
		this->serie = serie;

		//initializam agentul
		this->client = client;

		//initializam clientul
		this->agent = agent;

		//initializem valoarea
		this->valoare = valoare;


		//validare comisioane
		int ok = true;
		for (int i = 0; i < 2; i++) {
			if (comisioane[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			this->comisioane[0] = comisioane[0];
			this->comisioane[1] = comisioane[1];
		}


		nrContracte++;

	}

	//constr de copiere
	Contract(const Contract& c) {

		this->serie = c.serie;
		this->valoare = c.valoare;

		this->client = c.client;

		this->agent = c.agent;

		this->valoare = valoare;

		this->comisioane[0] = c.comisioane[0];
		this->comisioane[1] = c.comisioane[1];



		nrContracte++;
	}

	//destructor
	~Contract() {
		nrContracte--;
	}

	//overload operator =
	Contract& operator=(const Contract& c) {
		if (this != &c) {

			this->serie = c.serie;

			this->client = c.client;

			this->agent = c.agent;

			this->valoare = c.valoare;

			this->comisioane[0] = c.comisioane[0];
			this->comisioane[1] = c.comisioane[1];


		}

		return *this;
	}

	//getters

	string getSerie() {
		return this->serie;
	}

	static int getNrContracte() {
		return Contract::nrContracte;
	}

	Client getNumeClient() {
		return this->client;
	}

	Agent getNumeAgent() {
		return this->agent;
	}

	float getValoare() {
		return this->valoare;
	}

	float* getComisioane() {
		return this->comisioane;
	}



	//setters
	void setSerie(string serie) {
		if (serie.size() > 2) {
			this->serie = serie;
		}
	}

	void setClient(Client client) {
		this->client = client;
	}

	void setNumeAgent(Agent agent) {
		this->agent = agent;
	}

	void setValoare(float valoare) {
		if (valoare > 0) {
			this->valoare = valoare;
		}
		else {
			throw new CustomException("Valoarea contractului trebuie sa fie poztivia");
		}
	}

	void setComisioane(float* comisioane) {
		bool ok = true;
		for (int i = 0; i < 2; i++) {
			if (comisioane[i] < 0) {
				ok = false;
			}
		}

		if (ok == true) {
			this->comisioane[0] = comisioane[0];
			this->comisioane[1] = comisioane[1];
		}
		else {
			throw new CustomException("Comisioanele nu pot fi negative");
		}
	}


	//overload operator [] ->returneaza comisionul i
	float& operator[](int index) {
		if (index >= 0 && index < 2) {
			return this->comisioane[index];
		}
	}

	//overload cast float -> returneaza valoarea totala a contractului(imobil+ procentComisioane)
	explicit operator float() {
		float total = this->valoare + this->comisioane[0] + this->comisioane[1];

		return total;
	}

	//overload operator +(Constract) -> aduna valorile totale ale celor 2 contracte
	float operator+(Contract& c) {
		return (float)*this + float(c);
	}

	//overload operator *=(float) ->inmulteste valoarea comisionului cu float
	Contract& operator*=(float procent) {

		//comisioanele maxim se pot dubla
		if (procent >= 1 && procent <= 2) {
			this->comisioane[0] *= procent;
			this->comisioane[1] *= procent;
			return *this;
		}
		else {
			throw new CustomException("Procentul cu care se multiplica comisioanele trebuie sa fie minim 1 si maxim 2");
		}
	}

	//overload operator ++ -> creste valoarea contractului si a comsioanelor cu 10%

		//preincrementare
	Contract& operator++() {
		this->valoare *= 1.1;
		this->comisioane[0] *= 1.1;
		this->comisioane[1] *= 1.1;
		return *this;
	}

	//postincrementare
	Contract operator++(int) {
		Contract aux = *this;
		this->valoare *= 1.1;
		this->comisioane[0] *= 1.1;
		this->comisioane[1] *= 1.1;
		return aux;
	}

	//overload operator -- -> scade imobil contractului si a comisionalor cu 10%

		//predecrementare
	Contract& operator--() {
		this->valoare *= 0.9;
		this->comisioane[0] *= 0.9;
		this->comisioane[1] *= 0.9;
		return *this;
	}


	//postdecrementate
	Contract operator--(int) {
		Contract aux = *this;
		this->valoare *= 0.9;
		this->comisioane[0] *= 0.9;
		this->comisioane[1] *= 0.9;
		return aux;
	}

	//overload operator > ->compara valorile totale ale celor 2 contracte

	bool operator>(Contract& c) {

		return (float)*this > (float)c;
	}

	//overload operator <

	bool operator<(Contract& c) {
		return (float)*this < (float)c;
	}

	//overload operator ! -> verifica toate atributele contractului
	bool operator!() {

		bool ok = true;
		for (int i = 0; i < 2; i++) {
			if (this->comisioane[i] != 0) {
				ok = false;
			}
		}

		if (this->serie == "N/A" && this->valoare == 0 && ok == true) {
			return true;
		}
		else {
			return false;
		}
	}

	//operator == -> egalitatea tuturor atributelor
	bool operator==(Contract& c) {
		bool ok = true;
		for (int i = 0; i < 2; i++) {
			if (this->comisioane[i] != c.comisioane[i]) {
				ok = false;
			}
		}

		if (this->serie == c.serie && agent == c.agent && client == c.client && this->valoare == c.valoare && ok == true) {
			return true;
		}
		else {
			return false;
		}
	}

	//metoda care afiseaza comisionul pt client si cel pt proprietar

	void afisareComisioane() {
		cout << "\nComsisionul Clientului este: " << this->comisioane[0] << " euro" << endl;
		cout << "Comisionul Proprietarului esteL " << this->comisioane[1] << " euro" << endl;
	}


	//metoda care afiseaza procentul comisionului(procentComisioane / val totala *100)
	void calculProcentComision() {
		float comisionClient = (this->comisioane[0] / (float)*this) * 100;

		cout << "\nComisionul clientului este de " << comisionClient << "%";

		float comisionProprietar = (this->comisioane[1] / (float)*this) * 100;

		cout << "\nComisionul proprietarului este de " << comisionProprietar << " %";
	}


	//overload ostream
	friend ostream& operator<<(ostream& out, const Contract& c) {
		out << "\nSeria contractului: " << c.serie << endl;
		out << "\n\nInformatii despre clientul care a semnat contractul:" << c.client << endl;
		out << "\n\nInformatiile despre agentul care a semnat contractul: " << c.agent << endl;
		out << "\n\nValoare contract: " << c.valoare << " euro" << endl;
		cout << "Comisioane: " << endl;
		for (int i = 0; i < 2; i++) {
			cout << "Comision " << i + 1 << ": " << c.comisioane[i] << " euro" << endl;
		}

		return out;
	}

	//overload istream
	friend istream& operator>>(istream& in, Contract& c) {

		cout << "\nSerie: ";
		getline(in, c.serie);

		cout << "\n\nInformatii despre clientul care a semnat contractul:";
		in >> c.client;
		cout << "\nInformatiile despre agentul care a semnat contractul: ";
		in.ignore();
		in >> c.agent;

		cout << "\n\nValoare contract: "; in >> c.valoare;
		cout << "Comisioane: ";
		for (int i = 0; i < 2; i++) {
			cout << "Comision " << i + 1 << ": "; in >> c.comisioane[i];
		}
		return in;

	}

	virtual float calculSumaDePlata() {
		return this->valoare + comisioane[0] + comisioane[1];
	}

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) {
		out << this->serie << ", " << this->client.getNume() << ", " << this->agent.getNume() << ", " << this->valoare << ", ";
		for (int i = 0; i < 2; i++) {
			out << this->comisioane[i] << ", ";
		}
	}

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {

		//scriem seria
		int lg;
		lg = this->serie.size() + 1;
		f.write((char*)&lg, sizeof(int));

		f.write(this->serie.data(), sizeof(char) * lg);

		//scriem clientul si agentul
		this->client.writeToBinary(f);
		this->agent.writeToBinary(f);

		//scriem valoarea
		f.write((char*)&this->valoare, sizeof(float));

		for (int i = 0; i < 2; i++) {
			f.write((char*)&this->comisioane[i], sizeof(float));
		}



	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {

		//citim seria
		int lg;
		f.read((char*)&lg, sizeof(int));
		char* buffer = new char[lg];
		f.read(buffer, lg);
		this->serie = buffer;

		//citim clientu si agentu
		client.readFromBinary(f);
		agent.readFromBinary(f);


		//citim valoarea
		f.read((char*)&this->valoare, sizeof(float));

		for (int i = 0; i < 2; i++) {
			f.read((char*)&this->comisioane[i], sizeof(float));
		}



	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		in.ignore();
		getline(in, this->serie);
		client.readFromText(in);
		in.ignore();
		agent.readFromText(in);
		in >> valoare;
		for (int i = 0; i < 2; i++) {
			in >> this->comisioane[i];
		}

	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		out << this->serie << endl;
		client.writeToText(out);
		agent.writeToText(out);
		out << this->valoare << endl;
		for (int i = 0; i < 2; i++) {
			out << this->comisioane[i] << endl;
		}

	}

	friend bool operator<(const Contract& e1, const Contract& c2) {
		return e1.valoare < c2.valoare;
	}

	friend class UtilClass;

};

class ContractInchiriere : public Contract {

	int nrLuniInchiriere = 0;
	float garantie = 0;
public:
	//constructor fara param
	ContractInchiriere() :Contract() {};

	//cnstr cu parametrii
	ContractInchiriere(string serie, Client client, Agent agent, float valoare, float* comisioane, int nrLuniInchiriere, float garantie) :Contract(serie, client, agent, valoare, comisioane) {

		if (nrLuniInchiriere > 0) {
			this->nrLuniInchiriere = nrLuniInchiriere;
		}

		if (garantie >= 0) {
			this->garantie = garantie;
		}
	}

	//constructor de copiere
	ContractInchiriere(const ContractInchiriere& c) :Contract(c) {

		this->nrLuniInchiriere = c.nrLuniInchiriere;
		this->garantie = c.garantie;
	}

	//destructor
	~ContractInchiriere() {};

	//operator =
	ContractInchiriere& operator=(const ContractInchiriere& c) {

		if (this != &c) {
			Contract::operator=(c);
			this->nrLuniInchiriere = c.nrLuniInchiriere;
			this->garantie = c.garantie;
		}
		return *this;

	}

	//ostream
	friend ostream& operator<<(ostream& out, const ContractInchiriere& c) {
		out << (Contract&)c << endl;
		out << "Numar luni inchiriere: " << c.nrLuniInchiriere << endl;
		out << "Garantie: " << c.garantie << endl;
		return out;
	}

	//istream
	friend istream& operator>>(istream& in, ContractInchiriere& c) {
		in >> (Contract&)c;
		cout << "Numar luni inchiriere: ";
		in >> c.nrLuniInchiriere;
		cout << "Garantie: ";
		in >> c.garantie;
		return in;
	}



	float calculSumaDePlata() {
		return this->calculChirieLunara() + this->comisioane[0] / nrLuniInchiriere + this->comisioane[1] / nrLuniInchiriere + garantie;
	}

	float calculChirieLunara() {
		return this->valoare / nrLuniInchiriere;
	}

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) {
		Contract::writeToCsv(out);
		out << this->nrLuniInchiriere << " " << this->garantie;
		out << endl;
	}

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {
		Contract::writeToBinary(f);
		f.write((char*)&this->nrLuniInchiriere, sizeof(int));
		f.write((char*)&this->garantie, sizeof(int));
	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {
		Contract::readFromBinary(f);
		f.read((char*)&this->nrLuniInchiriere, sizeof(int));
		f.read((char*)&this->garantie, sizeof(int));
	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		Contract::readFromText(in);
		in >> this->nrLuniInchiriere;
		in >> this->garantie;
	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		Contract::writeToText(out);
		out << this->nrLuniInchiriere << endl;
		out << this->garantie << endl;
	}

};

class ContractVanzare : public Contract {

	float avans = 0;
	static float procentMaximFinantareBanca;
	float procentFinantareBanca = 0;
public:
	//constr fara parametrii
	ContractVanzare() :Contract() {};

	//constr cu parametrii
	ContractVanzare(string serie, Client client, Agent agent, float valoare, float* comisioane, float avans, float procentFinantareBanca) :Contract(serie, client, agent, valoare, comisioane) {
		if (avans >= 0 && avans < this->valoare) {
			this->avans = avans;
		}

		if (this->procentFinantareBanca < ContractVanzare::procentMaximFinantareBanca) {
			this->procentFinantareBanca = procentFinantareBanca;
		}
	}

	//constr de copiere
	ContractVanzare(const ContractVanzare& c) :Contract(c) {

		this->avans = c.avans;
		this->procentFinantareBanca = c.procentFinantareBanca;
	}

	//destructor
	~ContractVanzare() {};


	//operator = 
	ContractVanzare& operator=(const ContractVanzare& c) {
		if (this != &c) {
			Contract::operator=(c);
			this->avans = c.avans;
			this->procentFinantareBanca = c.procentFinantareBanca;
		}

		return *this;
	}

	float calculSumaDePlata() {

		//daca clientul pt care se face contractul este eligibil
		if (this->client.esteEligibil()) {
			return this->valoare - avans - procentFinantareBanca * this->valoare;
		}

		//daca clientul pt care se face contractul nu este eligibil
		return this->valoare - avans;
	}

	//ostream 
	friend ostream& operator<<(ostream& out, const ContractVanzare& c) {
		out << (Contract&)c << endl;
		out << "Avans: " << c.avans << endl;
		out << "Procent finantare banca: " << c.procentFinantareBanca << endl;
		return out;
	}

	//istream
	friend istream& operator>>(istream& in, ContractVanzare& c) {
		in >> (Contract&)c;
		cout << "Avans: ";
		in >> c.avans;
		cout << "Procent finantare banca(intre 0 si 1):";
		in >> c.procentFinantareBanca;
		return in;
	}

	//metoda care scrie textul in fisiere csv
	virtual void writeToCsv(ofstream& out) {
		Contract::writeToCsv(out);
		out << this->avans << ", " << this->procentFinantareBanca << endl;

	}

	//metoda care scrie in fisiere binare
	virtual void writeToBinary(fstream& f) {
		Contract::writeToBinary(f);
		f.write((char*)&this->avans, sizeof(float));
		f.write((char*)&this->procentFinantareBanca, sizeof(float));

	}

	//metoda care citeste din fisiere binare
	virtual void readFromBinary(fstream& f) {
		Contract::readFromBinary(f);
		f.read((char*)&this->avans, sizeof(float));
		f.read((char*)&this->procentFinantareBanca, sizeof(float));
	}

	//metoda care citeste din fisiere text
	virtual void readFromText(ifstream& in) {
		Contract::readFromText(in);
		in >> avans;
		in >> procentFinantareBanca;
	}

	//metoda care scrie intr-un fisier text
	virtual void writeToText(ofstream& out) {
		Contract::writeToText(out);
		out << avans << endl;
		out << procentFinantareBanca << endl;
	}


};
//banca nu poate finanta mai mult de 50% din valoarea contractului
float ContractVanzare::procentMaximFinantareBanca = 0.5;
int Contract::nrContracte = 0;

//clasa care contine un vector de pointer la contracte pt a ilustra virtualizarea
class ArhivaContracte {

	int nrContracte = 0;
	Contract** contracte = NULL;

public:

	//contr fara param
	ArhivaContracte() {};

	//constr cu param
	ArhivaContracte(int nrContracte, Contract** contracte) {

		if (nrContracte > 0 && contracte != NULL) {
			this->nrContracte = nrContracte;

			this->contracte = new Contract * [this->nrContracte];

			for (int i = 0; i < this->nrContracte; i++) {
				this->contracte[i] = new Contract(*contracte[i]);
			}
		}
	}

	//constr de copiere
	ArhivaContracte(const ArhivaContracte& c) {
		this->nrContracte = c.nrContracte;
		this->contracte = new Contract * [this->nrContracte];
		for (int i = 0; i < this->nrContracte; i++) {
			this->contracte[i] = new Contract(*c.contracte[i]);
		}
	}

	//operator = 
	ArhivaContracte& operator=(const ArhivaContracte& c) {
		if (this != &c) {
			for (int i = 0; i < this->nrContracte; i++) {
				delete this->contracte[i];
			}

			delete[] contracte;

			this->nrContracte = c.nrContracte;
			this->contracte = new Contract * [this->nrContracte];
			for (int i = 0; i < this->nrContracte; i++) {
				this->contracte[i] = new Contract(*c.contracte[i]);
			}
		}

		return *this;
	}

	//destructor
	~ArhivaContracte() {
		for (int i = 0; i < this->nrContracte; i++) {
			delete this->contracte[i];
 		}

		delete[] contracte;
	}

	//setter
	void setContracte(int nrContracte, Contract** contract) {
		if (nrContracte > 0 && contracte != NULL) {
			this->nrContracte = nrContracte;

			for (int i = 0; i < this->nrContracte; i++) {
				this->contracte[i] = new Contract(*contracte[i]);
			}
		}
	}

	//getters
	Contract** getContracte() {
		return this->contracte;
	}

	int getNrContracte() {
		return this->nrContracte;
	}

	//ostream
	friend ostream& operator<<(ostream& out, const ArhivaContracte& a) {
		out << "Nr contracte: " << a.nrContracte << endl;
		out << "Detalii contracte:" << endl;
		for (int i = 0; i < a.nrContracte; i++) {
			out << "Serie: " << a.contracte[i]->getSerie() << ", " << "Suma de plata: " << a.contracte[i]->calculSumaDePlata() << endl;
		}
		return out;
	}

	//operator += ->adauga un pointer de contract la arhiva
	void operator+=(Contract* c) {
		
		ArhivaContracte aux = *this;

		
		for (int i = 0; i < this->nrContracte; i++) {
			delete this->contracte[i];
		}
		delete[] contracte;
		this->nrContracte += 1;
		this->contracte = new Contract * [this->nrContracte];

		for (int i = 0; i < aux.nrContracte; i++) {
			this->contracte[i] = new Contract(*aux.contracte[i]);
		}

		this->contracte[this->nrContracte - 1] = new Contract(*c);

		
	}
};

class UtilClass {
	vector<Casa> vectorCase;
	vector<Apartament> vectorApartamente;
	vector<Agent> vectorAgenti;
	vector<Client> vectorClienti;
	vector<Proprietar> vectorProprietari;
	vector<ContractInchiriere> vectorContracteInchiriere;
	vector<ContractVanzare> vectorContracteVanzare;


	ArhivaContracte arhivaContracte;

	string init_path_text = "C:/Users/user/source/repos/Proiect Individual/Proiect Individual/Fisiere_initializare/";
	string init_path_binar = "C:/Users/user/source/repos/Proiect Individual/Proiect Individual/Fisiere_initializare_binare/";

	string binar_path = "C:/Users/user/source/repos/Proiect Individual/Proiect Individual/Fisiere_binare/";
protected:
	//Metoda care salveaza datele primite ca fisiere text in obiecte
	void init_obiecte(int nrArgumente, char** numeFisiere) {

		//verificam daca primim fisiere text ca argumente
		if (nrArgumente  >1) {
			//initializam case
			init_Case(numeFisiere[1]);

			//initializam apartamente
			init_apartamente(numeFisiere[2]);

			//initializam agenti
			init_agenti(numeFisiere[3]);

			//initializam clientii
			init_clienti(numeFisiere[4]);

			//initializam proprietarii
			init_proprietari(numeFisiere[5]);

			//initializam contractele de inchiriere
			init_conracte_inchiriere(numeFisiere[6]);

			//initializam contractele de vanzare
			init_contracte_vanzare(numeFisiere[7]);

			//initializam arhiva de contracte
			init_arhiva_contracte();
		}
		//daca nu primim fisiere text ca argumente, initializam obiectele din fisierele binare
		else {
			citire_case_din_binar(init_path_binar+ "case_init_binar.txt");
			citire_apartamente_din_binar(init_path_binar+ "apartamente_init_binar.txt");
			citire_clienti_din_binar(init_path_binar + "clienti_init_binar.txt");
			citire_proprietari_din_binar(init_path_binar + "proprietari_init_binar.txt");
			citire_agenti_din_binar(init_path_binar + "agenti_init_binar.txt");
			citire_contracte_inchiriere_din_binar(init_path_binar + "contracte_inchiriere_init_binar.txt");
			citire_contracte_vanzare_din_binar(init_path_binar + "contracte_vanzare_init_binar.txt");

			init_arhiva_contracte();

		}
	}

	void init_Case(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			Casa c;
			c.readFromText(in);
			vectorCase.push_back(c);

		}
		cout << "\nCasele au fost initializate";
	}

	void init_apartamente(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			Apartament a;
			a.readFromText(in);
			vectorApartamente.push_back(a);
		}

		cout << "\nApartamentele au fost initializate";
	}

	void init_agenti(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			Agent a;
			in.ignore();
			a.readFromText(in);
			vectorAgenti.push_back(a);
		}

		cout << "\nAgentii au fost initializati";
	}

	void init_clienti(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			Client c;
			c.readFromText(in);
			vectorClienti.push_back(c);
		}

		cout << "\nClientii au fost initializati";
	}

	void init_proprietari(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			Proprietar p;
			p.readFromText(in);
			vectorProprietari.push_back(p);
		}

		cout << "\nProprietarii au fost initializati";
	}

	void init_conracte_inchiriere(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;

		for (int i = 0; i < numarInregistrari; i++) {
			ContractInchiriere c;
			c.readFromText(in);
			vectorContracteInchiriere.push_back(c);
		}

		cout << "\nContractele de inchiriere  au fost initializate";
	}

	void init_contracte_vanzare(string fisier) {
		ifstream in;
		in.open(init_path_text + fisier, ios::in);
		int numarInregistrari;
		in >> numarInregistrari;
		for (int i = 0; i < numarInregistrari; i++) {
			ContractVanzare c;
			c.readFromText(in);
			vectorContracteVanzare.push_back(c);

		}

		cout << "\nContractele de vanzare  au fost initializate";
	}

	void init_arhiva_contracte() {
		for (int i = 0; i < this->vectorContracteInchiriere.size(); i++) {
			this->arhivaContracte += &vectorContracteInchiriere[i];
		}

		for (int i = 0; i < this->vectorContracteVanzare.size(); i++) {
			this->arhivaContracte += &vectorContracteInchiriere[i];
		}

		
	}

	void afisare_case() {
		for (int i = 0; i < vectorCase.size(); i++) {
			cout << vectorCase[i];
		}
	}

	void afisare_apartamente() {
		for (int i = 0; i < vectorApartamente.size(); i++) {
			cout << vectorApartamente[i];
		}
	}

	void afisare_agenti() {
		for (int i = 0; i < vectorAgenti.size(); i++) {
			cout << vectorAgenti[i];
		}
	}

	void afisare_clienti() {
		for (int i = 0; i < vectorClienti.size(); i++) {
			cout << vectorClienti[i];
		}
	}

	void afisare_proprietari() {
		for (int i = 0; i < vectorProprietari.size(); i++) {
			cout << vectorProprietari[i];
		}
	}

	void afisare_contracte_inchiriere() {
		for (int i = 0; i < vectorContracteInchiriere.size(); i++) {
			cout << vectorContracteInchiriere[i];
		}
	}

	void afisare_contracte_vanzare() {
		for (int i = 0; i < vectorContracteVanzare.size(); i++) {
			cout << vectorContracteVanzare[i];
		}
	}

	void scriere_case_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi citit
		int lungime = vectorCase.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorCase.size(); i++) {
			vectorCase[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_apartamente_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorApartamente.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorApartamente.size(); i++) {
			vectorApartamente[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_agenti_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorAgenti.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorAgenti.size(); i++) {
			vectorAgenti[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_clienti_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorClienti.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorClienti.size(); i++) {
			vectorClienti[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_proprietari_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorProprietari.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorProprietari.size(); i++) {
			vectorProprietari[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_contracte_inchiriere_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorContracteInchiriere.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorContracteInchiriere.size(); i++) {
			vectorContracteInchiriere[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void scriere_contracte_vanzare_in_binar(string numeFisier) {
		fstream fout(binar_path+numeFisier, ios::out | ios::binary);

		//scriem prima data lungimea vectorului pentru a putea fi 
		int lungime = vectorContracteVanzare.size();

		fout.write((char*)&lungime, sizeof(int));

		for (int i = 0; i < vectorContracteVanzare.size(); i++) {
			vectorContracteVanzare[i].writeToBinary(fout);
		}

		fout.close();
		cout << "\nFisierul binar " + numeFisier << " a fost generat cu succes!";
	}

	void citire_case_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			Casa c;
			vectorCase.resize(nr, c);


			for (int i = 0; i < nr; i++) {
				vectorCase[i].readFromBinary(fin);
			}
			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar" << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void citire_apartamente_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			Apartament a;
			vectorApartamente.resize(nr, a);


			for (int i = 0; i < nr; i++) {
				vectorApartamente[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar" << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void citire_agenti_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			Agent a;
			vectorAgenti.resize(nr, a);


			for (int i = 0; i < nr; i++) {
				vectorAgenti[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar" << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void citire_clienti_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			Client c;
			vectorClienti.resize(nr, c);


			for (int i = 0; i < nr; i++) {
				vectorClienti[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar" << numeFisier << " nu a putut fi deschis" << endl;
		}
	}

	void citire_proprietari_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			Proprietar p;
			vectorProprietari.resize(nr, p);


			for (int i = 0; i < nr; i++) {
				vectorProprietari[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar" << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void citire_contracte_inchiriere_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			ContractInchiriere c;
			vectorContracteInchiriere.resize(nr, c);


			for (int i = 0; i < nr; i++) {
				vectorContracteInchiriere[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar " << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void citire_contracte_vanzare_din_binar(string numeFisier) {
		fstream fin(binar_path+numeFisier, ios::in | ios::binary);
		if (fin.is_open()) {
			int nr = 0;
			fin.read((char*)&nr, sizeof(int));
			//stergem toate elementele din vector
			ContractVanzare c;
			vectorContracteVanzare.resize(nr, c);


			for (int i = 0; i < nr; i++) {
				vectorContracteVanzare[i].readFromBinary(fin);
			}

			fin.close();
			cout << "Datele au fost citite cu succes din fisierul binar " << numeFisier << "!" << endl;
		}
		else {
			cout << "Fisierul binar " << numeFisier << " nu a putut fi deschis" << endl;
		}

	}

	void generare_csv_case(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorCase.size(); i++) {
			vectorCase[i].writeToCsv(out);
		}
	}

	void generare_csv_apartamente(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorApartamente.size(); i++) {
			vectorApartamente[i].writeToCsv(out);
		}
	}

	void generare_csv_agenti(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorAgenti.size(); i++) {
			vectorAgenti[i].writeToCsv(out);
		}
	}

	void generare_csv_cienti(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorClienti.size(); i++) {
			vectorClienti[i].writeToCsv(out);
		}
	}

	void generare_csv_proprietari(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorProprietari.size(); i++) {
			vectorProprietari[i].writeToCsv(out);
		}
	}

	void generare_csv_contracte_inchiriere(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorContracteInchiriere.size(); i++) {
			vectorContracteInchiriere[i].writeToCsv(out);
		}
	}

	void generare_csv_contracte_vanzare(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);

		for (int i = 0; i < vectorContracteVanzare.size(); i++) {
			vectorContracteVanzare[i].writeToCsv(out);
		}
	}

	void citire_casa_consola() {
		Casa c;
		cin >> c;
		vectorCase.push_back(c);
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_apartament_consola() {
		Apartament a;
		cin >> a;
		vectorApartamente.push_back(a);
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_agent_consola() {
		Agent a;
		cin >> a;
		vectorAgenti.push_back(a);
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_client_consola() {
		Client c;
		cin >> c;
		vectorClienti.push_back(c);
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_proprietar_consola() {
		Proprietar p;
		cin >> p;
		vectorProprietari.push_back(p);
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_contract_inchiriere_consola() {
		ContractInchiriere c;
		cin >> c;
		vectorContracteInchiriere.push_back(c);
		arhivaContracte += &c;
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	void citire_contract_vanzare_consola() {
		ContractVanzare c;
		cin >> c;
		vectorContracteVanzare.push_back(c);
		arhivaContracte += &c;
		cout << "\nInformatiile au fost adaugate cu succes in baza de date";
	}

	//alte rapoarte
	//metoda care creeaza un raport al tuturor contractelorin ordinea crescatoarea a valorii acestora (foloseste set)
	void contracte_ordonate_crescator(string numeFisier) {

		ofstream out;
		out.open(numeFisier, ios::out);

		//vom folosi un set intrucat acesta ordoneaza elementele
		set<Contract> setContracte;

		set<Contract>::iterator it;

		for (int i = 0; i < vectorContracteInchiriere.size(); i++) {
			setContracte.insert(vectorContracteInchiriere[i]);
		}

		for (int i = 0; i < vectorContracteVanzare.size(); i++) {
			setContracte.insert(vectorContracteVanzare[i]);
		}
		out << "Serie contract" << "\t" << "    Valoare" << endl;
		for (it = setContracte.begin(); it != setContracte.end(); it++) {
			out << it->serie << "\t            " << it->valoare << " euro\t" << endl;
		}

		out.close();
	}

	//metoda care creeaza un raport cu toti agentii si valoarea totala a vanzarilor obtinute de acestia (foloseste map)
	void agenti_valoare_vanzari(string numeFisier) {
		ofstream out;

		out.open(numeFisier, ios::out);

		//folosim o mapa ce contine ca si cheie id-ul agentului si ca valoare valoarea vanzarilor
		map<int, float> mapAgenti;

		//populam mapa
		for (int i = 0; i < vectorAgenti.size(); i++) {
			mapAgenti[this->vectorAgenti[i].getId()] = this->vectorAgenti[i].sumaValoriiVanzarilor();
		}
		
		//facem un iterator pentru a parcurge mapa si a afisa valorile intr-un fisier
		map<int, float>::iterator iterator;
		out << "ID agent\t" << "Nume agent\t\t" << "Valoare vanzari\n";
		for (iterator = mapAgenti.begin(); iterator != mapAgenti.end(); iterator++) {
			out << iterator->first << "\t\t" << cautare_agent(iterator->first) << "\t\t" << iterator->second <<" euro"<< endl;
		}

		out.close();



	}

	//operatii arhiva contracte
	void afisare_arhiva_contracte_consola() {
		cout << arhivaContracte;
	}

	void afisare_arhiva_contracte_text(string numeFisier) {
		ofstream out;
		out.open(numeFisier, ios::out);
		
		for (int i = 0; i < arhivaContracte.getNrContracte();i++) {
			out << "Serie contract: " << arhivaContracte.getContracte()[i]->getSerie() << ", Total suma de plata: " << arhivaContracte.getContracte()[i]->calculSumaDePlata() << endl;
		}
		cout << "\nFisierul " + numeFisier + " a fost generat cu succes!";
	}

	//metode ajutatoare

	//metoda care cauta numele unui agent in functie de id
	string cautare_agent(int id) {
		for (int i = 0; i < vectorAgenti.size(); i++) {
			if (vectorAgenti[i].getId() == id) {
				return vectorAgenti[i].getNume();
			}
		}
	}



};

class InitFisiere {

	ofstream out;
	string path_text = "C:/Users/user/source/repos/Proiect Individual/Proiect Individual/Fisiere_initializare/";
	string path_binar= "C:/Users/user/source/repos/Proiect Individual/Proiect Individual/Fisiere_initializare_binare/";

protected:

	//metoda care populeaza aplicatia cu fisiere text primite ca argumente
	void init_aplicatie() {

		incarcare_case();
		incarcare_apartamente();
		incarcare_agenti();
		incarcare_clienti();
		incarcare_proprietari();
		incarcare_contracte_inchiriere();
		incarcare_contracte_vanzare();
	}

	void incarcare_case() {

		Casa c1(2020, "Bucuresti", 100, new float[5] { 100000, 120000, 130000, 140000, 150000 }, 1, 30, 50);
		Casa c2(2017, "Iasi", 80, new float[5] { 67000, 72000, 88000, 91000, 99000 }, 3, 40, 60);
		Casa c3(2015, "Bacau", 75, new float[5] { 40000, 50000, 55000, 60000, 63000 }, 46, 30, 100);
		Casa c4(2004, "Bucuresti", 90, new float[5] { 60000, 67000, 75000, 80000, 86000 }, 23, 20, 120);

		//initializam in fisiere text
		out.open(path_text+ "case_init.txt", ios::out);
		out << 4 << endl;
		c1.writeToText(out);
		c2.writeToText(out);
		c3.writeToText(out);
		c4.writeToText(out);
		out.close();

		//initializam in fisiere binare
		fstream fout(path_binar+"case_init_binar.txt", ios::out | ios::binary);
		int nr = 4;
		fout.write((char*)&nr, sizeof(int));
		c1.writeToBinary(fout);
		c2.writeToBinary(fout);
		c3.writeToBinary(fout);
		c4.writeToBinary(fout);
		fout.close();

	}

	void incarcare_apartamente() {

		Apartament a1(2016, "Cluj", 75, new float[5] { 70000, 82000, 95000, 100000, 115000 }, 4, 2, new float[2] { 8, 10 }, 4);
		Apartament a2(2013, "Timisoara", 65, new float[5] { 55000, 67000, 80000, 92000, 98000 }, 2, 1, new float[1] {7}, 3);
		Apartament a3(2005, "Buzau", 80, new float[5] {50000, 52000, 55000, 58000, 61000}, 6, 2, new float[2] {5, 6}, 4);
		Apartament a4(2020, "Bucuresti", 100, new float[5] {100000, 110000, 120000, 130000, 180000}, 12, 3, new float[3] {6, 7, 8}, 10);

		//incarcam in fisiere text
		out.open(path_text + "apartamente_init.txt", ios::out);
		out << 4 << endl;
		a1.writeToText(out);
		a2.writeToText(out);
		a3.writeToText(out);
		a4.writeToText(out);
		out.close();

		//incarcam in fisiere binare
		fstream fout(path_binar + "apartamente_init_binar.txt", ios::out | ios::binary);
		int nr = 4;
		fout.write((char*)&nr, sizeof(int));
		a1.writeToBinary(fout);
		a2.writeToBinary(fout);
		a3.writeToBinary(fout);
		a4.writeToBinary(fout);
		fout.close();

	}

	void incarcare_agenti() {
		Agent a1("Machidon Andrei", 3, new float[3] { 240000, 320000, 450000 }, new float[3] { 770000, 82000, 94000 });
		Agent a2("Rusu Vlad", 2, new float[2] { 160000, 210000 }, new float[3] { 50000, 60000, 75000 });
		Agent a3("Paduraru Cosmin", 4, new float[4] { 230000, 270000, 180000, 215000 }, new float[3] { 80000, 90000, 100000 });
		Agent a4("Raul Geroge", 2, new float[2] { 70000, 120000 }, new float[3] { 50000, 52000, 56000 });

		//incarcam in fisiere text
		out.open(path_text + "agenti_init.txt", ios::out);
		out << 8 << endl;
		a1.writeToText(out);
		a2.writeToText(out);
		a3.writeToText(out);
		a4.writeToText(out);
		out.close();

		//incarcam in fisiere binare
		fstream fout(path_binar + "agenti_init_binar.txt", ios::out | ios::binary);
		int nr = 8;
		fout.write((char*)&nr, sizeof(int));
		a1.writeToBinary(fout);
		a2.writeToBinary(fout);
		a3.writeToBinary(fout);
		a4.writeToBinary(fout);
		fout.close();
	}

	void incarcare_clienti() {
		Client c1(1, "Dospinescu Mihai", 24, new float[12] { 800, 800, 850, 900, 1200, 1300, 1400, 1500, 1600, 1700, 1900, 2050 }, false, 3, new float[3] { 70000, 80000, 95000 });
		Client c2(2, "Arama Rares", 31, new float[12] { 650, 820, 750, 1000, 950, 1150, 1200, 1300, 1000, 1200, 970, 1500 }, true, 2, new float[3] { 60000, 70000 });
		Client c3(3, "Manole Tudor", 45, new float[12] { 600, 700, 850, 750, 1000, 1200, 1500, 1000, 1250, 1600, 1100, 1800 }, true, 2, new float[2] { 78000, 82000 });
		Client c4(4, "Chisareanu Marius", 34, new float[12] { 450, 500, 550, 600, 600, 700, 700, 750, 820, 970, 1000, 1050 }, true, 1, new float[1] { 65000 });

		//incarcam in fisiere text
		out.open(path_text + "clienti_init.txt", ios::out);
		out << 8 << endl;
		c1.writeToText(out);
		c2.writeToText(out);
		c3.writeToText(out);
		c4.writeToText(out);
		out.close();

		//incarcam in fisiere binare
		fstream fout(path_binar + "clienti_init_binar.txt", ios::out | ios::binary);
		int nr = 8;
		fout.write((char*)&nr, sizeof(int));
		c1.writeToBinary(fout);
		c2.writeToBinary(fout);
		c3.writeToBinary(fout);
		c4.writeToBinary(fout);
		fout.close();


	}

	void incarcare_proprietari() {
		Proprietar p1(1, "Pascariu George", 25, TIP_PACHET::SILVER, 3, new float[3] { 60000, 68000, 75000 }, new float[6] { 30, 45, 50, 50, 55, 60 });
		Proprietar p2(2, "Murariu Mihnea", 32, TIP_PACHET::SILVER, 2, new float[3] { 72000, 76000 }, new float[6] { 30, 45, 50, 50, 55, 60 });
		Proprietar p3(3, "Savin Ioana", 24, TIP_PACHET::BRONZE, 1, new float[3] { 50000 }, new float[6] { 15, 25, 30, 32, 38, 40 });
		Proprietar p4(4, "Balcan Laur", 48, TIP_PACHET::GOLD, 4, new float[4] { 80000, 90000, 100000, 120000 }, new float[6] { 65, 70, 80, 85, 85, 90 });

		//scriere in fisire text
		out.open(path_text + "proprietari_init.txt", ios::out);
		out << 4 << endl;
		p1.writeToText(out);
		p2.writeToText(out);
		p3.writeToText(out);
		p4.writeToText(out);
		out.close();

		fstream fout(path_binar + "proprietari_init_binar.txt", ios::out | ios::binary);
		int nr = 4;
		fout.write((char*)&nr, sizeof(int));
		p1.writeToBinary(fout);
		p2.writeToBinary(fout);
		p3.writeToBinary(fout);
		p4.writeToBinary(fout);
		fout.close();
	}

	void incarcare_contracte_inchiriere() {

		Client c1(5, "Mihaila Radu", 26, new float[12] { 1200, 1300, 1850, 1900, 1700, 1800, 1850, 1780, 1900, 2000, 2100, 2200 }, false, 3, new float[3] { 90000, 120000, 125000 });
		Client c2(6, "Bostan Maria", 33, new float[12] { 750, 870, 720, 1050, 980, 1250, 1200, 1100, 1600, 1400, 1270, 1380 }, true, 2, new float[2] { 75000, 800000 });
		Agent a1("Lazar Cristian", 4, new float[4] { 120000, 180000, 350000, 480000 }, new float[3] { 120000, 182000, 194000 });
		Agent a2("Turan Paul", 2, new float[2] { 90000, 150000 }, new float[3] { 40000, 50000, 65000 });

		ContractInchiriere ct1("HFKDO", c1, a1, 120000, new float[2] { 1200, 1300 }, 48, 1200);
		ContractInchiriere ct2("FNEIW", c2, a2, 80000, new float[2] { 900, 800 }, 36, 800);

		//prima data scriem clientii adaugati in fisier(in modul append pt a nu sterge datele)
		out.open(path_text + "clienti_init.txt", ios::app);
		c1.writeToText(out);
		c2.writeToText(out);
		out.close();


		//scriem agentii in fisier(in append)
		out.open(path_text + "agenti_init.txt", ios::app);
		a1.writeToText(out);
		a2.writeToText(out);
		out.close();

		//scriem contractele de inchiriere
		out.open(path_text + "contracte_inchiriere_init.txt", ios::out);
		out << 2 << endl;
		ct1.writeToText(out);
		ct2.writeToText(out);
		out.close();

		//scriem clientii si in fisierul binar
		fstream fout(path_binar + "clienti_init_binar.txt", ios::app | ios::binary);
		c1.writeToBinary(fout);
		c2.writeToBinary(fout);
		fout.close();

		//scriem agentii si in fisierul binar
		fstream fout2(path_binar + "agenti_init_binar.txt", ios::app | ios::binary);
		a1.writeToBinary(fout2);
		a2.writeToBinary(fout2);
		fout2.close();

		//scriem contractele si in fisierul binar
		fstream fout3(path_binar + "contracte_inchiriere_init_binar.txt", ios::out | ios::binary);
		int nr = 2;
		fout3.write((char*)&nr, sizeof(int));
		ct1.writeToBinary(fout3);
		ct2.writeToBinary(fout3);
		fout3.close();
	
	}

	void incarcare_contracte_vanzare() {
		Client c1(7, "Stoica Laura", 28, new float[12] { 800, 800, 850, 900, 1200, 1300, 1400, 1500, 1600, 1700, 1900, 2050 }, false, 3, new float[3] { 70000, 80000, 95000 });
		Client c2(8, "Chirila Rares", 38, new float[12] { 650, 820, 750, 1000, 950, 1150, 1200, 1300, 1000, 1200, 970, 1500 }, true, 2, new float[2] { 60000, 70000 });
		Agent a1("Tofan Alexandu", 3, new float[3] { 200000, 320000, 360000 }, new float[3] { 60000, 80000, 72000 });
		Agent a2("Anisiei Patrick", 1, new float[2] { 90000 }, new float[3] { 50000, 52000, 56000 });

		ContractVanzare ct1("TFJEA", c1, a1, 96000, new float[2] { 900, 950 }, 20000, 0.3);
		ContractVanzare ct2("FNEUF", c2, a2, 120000, new float[2] { 1200, 1000 }, 45000, 0.2);


		//prima data scriem clientii adaugati in fisier(in modul append pt a nu sterge datele)
		out.open(path_text + "clienti_init.txt", ios::app);
		c1.writeToText(out);
		c2.writeToText(out);
		out.close();

		//scriem agentii in fisier(in append)
		out.open(path_text + "agenti_init.txt", ios::app);
		a1.writeToText(out);
		a2.writeToText(out);
		out.close();

		//scriem contractele de vanzare
		out.open(path_text + "contracte_vanzare_init.txt", ios::out);
		out << 2 << endl;
		ct1.writeToText(out);
		ct2.writeToText(out);
		out.close();

		//scriem clientii si in fisierul binar
		fstream fout(path_binar + "clienti_init_binar.txt", ios::app | ios::binary);
		c1.writeToBinary(fout);
		c2.writeToBinary(fout);
		fout.close();

		//scriem agentii si in fisierul binar
		fstream fout2(path_binar + "agenti_init_binar.txt", ios::app | ios::binary);
		a1.writeToBinary(fout2);
		a2.writeToBinary(fout2);
		fout2.close();

		//scriem contractele si in fisierul binar
		fstream fout3(path_binar + "contracte_vanzare_init_binar.txt", ios::out | ios::binary);
		int nr = 2;
		fout3.write((char*)&nr, sizeof(int));
		ct1.writeToBinary(fout3);
		ct2.writeToBinary(fout3);
		fout3.close();


	}

};

class MeniuPrincipal : protected UtilClass, protected InitFisiere {

	list<string> activitateaUtilizatorului;
public:

	void init(int nrFisiere, char** numeFisiere) {

		InitFisiere::init_aplicatie();
		UtilClass::init_obiecte(nrFisiere, numeFisiere);
		activitateaUtilizatorului.push_back("Datele au fost initializate");

		bool ok = true;
		while (ok == true) {
			cout << "\n\n\t\t--------------------------------------------------------------------------------";
			cout << "\n\t\t\tBUNA ZIUA SI BINE ATI VENIT IN PROGRAMUL DE GESIUNE AL COMPANIEI REMAX" << endl;
			cout << "\n\tVA RUGAM SA ALEGETI OPERATIUNEA PE CARE DORITI SA O EFECTUATI" << endl << endl;
			cout << "\t1.Salvare date in fisiere binare." << endl;
			cout << "\t2.Generare rapoarte CSV" << endl;
			cout << "\t3.Citire date din fisiere binare" << endl;
			cout << "\t4.Introducere entitate din consola" << endl;
			cout << "\t5.Afisare informatii in consola" << endl;
			cout << "\t6.Afisare alte rapoarte" << endl;
			cout << "\t7.Operatii arhiva contracte" <<endl;
			cout << "\t8.Afisare activitate utilizator" << endl;
			cout << "\t9.Inchideti aplicatia" << endl << endl;

			int alegere;
			cout << "Alegere: ";
			cin >> alegere;

			if (alegere == 1) {
				alegere1();
			}
			else if (alegere == 2) {
				alegere2();
			}
			else if (alegere == 3) {
				alegere3();
			}
			else if (alegere == 4) {
				alegere4();
			}
			else if (alegere == 5) {
				alegere5();
			}
			else if (alegere == 6) {
				alegere6();
			}
			else if (alegere == 7) {
				alegere7();
			}
			else if (alegere == 8) {
				alegere8();
			}
			else if (alegere == 9) {
				exit(0);
			}
		}
	}

private:

	void alegere1() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "\nCE INFORMATII DORITI SA STOCATI IN FISIERE BINARE? " << endl << endl;
		cout << "1.Informatiile caselor tranzactionate la REMAX" << endl;
		cout << "2.Informatiile apartamentelor tranzactionate la REMAX" << endl;
		cout << "3.Informatiile agentilor imobiliari" << endl;
		cout << "4.Informatiile despre clientii REMAX" << endl;
		cout << "5.Informatiile despre proprietarii imobilelor" << endl;
		cout << "6.Informatii despre contractele de inchiriere incheiate: " << endl;
		cout << "7.Informatii despre contractele de vanzare incheiate " << endl;
		cout << "8.Intoarcere inapoi la meniu" << endl;

		cout << "\nAlegere: ";
		cin >> alegereSecundara;


		switch (alegereSecundara)
		{
		case 1:
			UtilClass::scriere_case_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris casele in fisiere binare");
			break;
		case 2:
			UtilClass::scriere_apartamente_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris apartamentele in fisiere binare");
			break;
		case 3:
			UtilClass::scriere_agenti_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris agentii in fisiere binare");
			break;
		case 4:
			UtilClass::scriere_clienti_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris clientii in fisiere binare");
			break;
		case 5:
			UtilClass::scriere_proprietari_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris proprietarii in fisiere binare");
			break;
		case 6:
			UtilClass::scriere_contracte_inchiriere_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scris contractele de inchiriere in fisiere binare");
			break;
		case 7:
			UtilClass::scriere_contracte_vanzare_in_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a scriscontractele de vanzare in fisiere binare");
			break;
		default:
			break;
		}
	}

	void alegere2() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "DESPRE CE DORITI SA REALIZATI UN RAPORT CSV? " << endl;
		cout << "1.Informatiile caselor tranzactionate la REMAX" << endl;
		cout << "2.Informatiile apartamentelor tranzactionate la REMAX" << endl;
		cout << "3.Informatiile agentilor imobiliari" << endl;
		cout << "4.Informatiile despre clientii REMAX" << endl;
		cout << "5.Informatiile despre proprietarii imobilelor" << endl;
		cout << "6.Informatii despre contractele de inchiriere incheiate: " << endl;
		cout << "7.Informatii despre contractele de vanzare incheiate " << endl;
		cout << "8.Intoarcere inapoi la meniu" << endl;


		cout << "\nAlegere: ";
		cin >> alegereSecundara;

		switch (alegereSecundara)
		{
		case 1:
			UtilClass::generare_csv_case(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru case");
			break;
		case 2:
			UtilClass::generare_csv_apartamente(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru apartamente");
			break;
		case 3:
			UtilClass::generare_csv_agenti(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru agenti");
			break;
		case 4:
			UtilClass::generare_csv_cienti(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru clienti");
			break;
		case 5:
			UtilClass::generare_csv_proprietari(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru proprietari");
			break;
		case 6:
			UtilClass::generare_csv_contracte_inchiriere(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru contractele de inchiriere");
			break;
		case 7:
			UtilClass::generare_csv_contracte_vanzare(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a generat un raport CSV pentru contractele de vanzare");
			break;
		default:
			break;
		}
	}

	void alegere3() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "\t\tCE INFORMATII DORITI SA EXTRAGETI DIN FISIERE BINARE?" << endl << endl;
		cout << "\t1.Informatiile caselor tranzactionate la REMAX" << endl;
		cout << "\t2.Informatiile apartamentelor tranzactionate la REMAX" << endl;
		cout << "\t3.Informatiile agentilor imobiliari" << endl;
		cout << "\t4.Informatiile despre clientii REMAX" << endl;
		cout << "\t5.Informatiile despre proprietarii imobilelor" << endl;
		cout << "\t6.Informatii despre contractele de inchiriere incheiate: " << endl;
		cout << "\t7.Informatii despre contractele de vanzare incheiate " << endl;
		cout << "\t8.Intoarcere inapoi la meniu" << endl;

		cout << "\nAlegere: ";
		cin >> alegereSecundara;
		switch (alegereSecundara)
		{
		case 1:
			UtilClass::citire_case_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre case din fisiere binare");
			break;
		case 2:
			UtilClass::citire_apartamente_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre apartamente din fisiere binare");
			break;
		case 3:
			UtilClass::citire_agenti_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre agenti din fisiere binare");
			break;
		case 4:
			UtilClass::citire_clienti_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre clienti din fisiere binare");
			break;
		case 5:
			UtilClass::citire_proprietari_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre proprietari din fisiere binare");
			break;
		case 6:
			UtilClass::citire_contracte_inchiriere_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre contractele de inchiriere din fisiere binare");
			break;
		case 7:
			UtilClass::citire_contracte_vanzare_din_binar(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a citit date despre contractele de vanzare din fisiere binare");
			break;
		default:
			break;
		}
	}

	void alegere4() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "\nCE DORITI SA ADAUGATI IN BAZA DE DATE? " << endl;
		cout << "1.O casa" << endl;
		cout << "2.Un apartament" << endl;
		cout << "3.Un agent imobiliar" << endl;
		cout << "4.Un client" << endl;
		cout << "5.Un proprietar" << endl;
		cout << "6.Un contract de inchiriere" << endl;
		cout << "7.Un contract de vanzare" << endl;
		cout << "8.Intoarcere la meniul principal" << endl;

		cout << "Alegere: ";
		cin >> alegereSecundara;
		cin.ignore();

		switch (alegereSecundara)
		{
		case 1:
			UtilClass::citire_casa_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre o casa");
			break;
		case 2:
			UtilClass::citire_apartament_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un apartament");
			break;
		case 3:
			UtilClass::citire_agent_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un agent");
			break;
		case 4:
			UtilClass::citire_client_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un client");
			break;
		case 5:
			UtilClass::citire_proprietar_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un proprietar");
			break;
		case 6:
			UtilClass::citire_contract_inchiriere_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un contract de inchiriere");
			break;
		case 7:
			UtilClass::citire_contract_vanzare_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a citit de la consola informatii despre un contract de vanzare");
			break;
		default:
			break;
		}
	}

	void alegere5() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "\nCE ENTITATI DORITI SA AFISATI IN CONSOLA? " << endl;
		cout << "\t1.Informatiile caselor tranzactionate la REMAX" << endl;
		cout << "\t2.Informatiile apartamentelor tranzactionate la REMAX" << endl;
		cout << "\t3.Informatiile agentilor imobiliari" << endl;
		cout << "\t4.Informatiile despre clientii REMAX" << endl;
		cout << "\t5.Informatiile despre proprietarii imobilelor" << endl;
		cout << "\t6.Informatii despre contractele de inchiriere incheiate: " << endl;
		cout << "\t7.Informatii despre contractele de vanzare incheiate " << endl;
		cout << "\t8.Intoarcere inapoi la meniu" << endl;

		cout << "Alegere: ";
		cin >> alegereSecundara;

		switch (alegereSecundara)
		{
		case 1:
			UtilClass::afisare_case();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor caselor");
			break;
		case 2:
			UtilClass::afisare_apartamente();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor apartamentelor");
			break;
		case 3:
			UtilClass::afisare_agenti();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor agentilor");
			break;
		case 4:
			UtilClass::afisare_clienti();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor clientilor");
			break;
		case 5:
			UtilClass::afisare_proprietari();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor proprietarilor");
			break;
		case 6:
			UtilClass::afisare_contracte_inchiriere();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor contractelor de inchiriere");
			break;
		case 7:
			UtilClass::afisare_contracte_vanzare();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat in consola informatiile tuturor contractelor de vanzare");
			break;
		default:
			break;
		}
	}

	void alegere6() {
		int alegereSecundara = 0;
		cout << "ALEGETI CE RAPORT DORITI SA GENERATI: " << endl;
		cout << "1.Toate contractele in ordinea crescatoare a valorii lor" << endl;
		cout << "2.Toti Agentii Remax si valoarea totala a vanzarilor obtinute de acestia" << endl;
		
		cout << "\nAlegere: ";
		cin >> alegereSecundara;

		switch (alegereSecundara)
		{
			case 1:
				UtilClass::contracte_ordonate_crescator(alegereNumeFisier());
				activitateaUtilizatorului.push_back("Utilizatorul a generat un raport cu contractele ordonate crescator in functie de suma de plata");
				break;
			case 2:
				UtilClass::agenti_valoare_vanzari(alegereNumeFisier());
				activitateaUtilizatorului.push_back("Utilizatorul a generat un raport cu agentii si valoarea totala a vanzarilor acestora");
				break;
		default:
			break;
		}



	};

	void alegere7() {
		int alegereSecundara = 0;
		system("CLS");
		cout << "ALEGETI CUM DORITI SA AFISATI ARHIVA CONTRACTELOR" << endl;
		cout << "1.In consola" << endl;
		cout << "2.In fisier text" << endl;

		cout << "Alegere:";
		cin >> alegereSecundara;
		switch (alegereSecundara)
		{
		case 1:
			UtilClass::afisare_arhiva_contracte_consola();
			activitateaUtilizatorului.push_back("Utilizatorul a afisat arhiva contractelor in consola");
			break;
		case 2:
			UtilClass::afisare_arhiva_contracte_text(alegereNumeFisier());
			activitateaUtilizatorului.push_back("Utilizatorul a afisat arhiva contractelor in fisier text");
			break;
		default:
			break;
		}
	};

	void alegere8(){
		list<string>::iterator it;
		cout << "\n\n\n\t\t\t----------------INFORMATII SESIUNE--------------\t\t\t" << endl << endl;
		for (it = activitateaUtilizatorului.begin(); it != activitateaUtilizatorului.end(); it++) {
			cout <<"\t\t\t" << *it << endl;
		}
		cout << endl << endl;
	}

	string alegereNumeFisier() {
		string numeFisier;
		cout << "Va rugam introduceti numele fisierului: ";
		cin >> numeFisier;
		return numeFisier;
	}
};

//argv incepe de la 1
int main(int argc, char* argv[]) {
	MeniuPrincipal m;
	m.init(argc, argv);

}

