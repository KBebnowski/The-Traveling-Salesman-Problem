#include <iostream>
#include <chrono>
#include <time.h>
#include<fstream>
using namespace std::chrono;

class NNA {

private:
	//pola dla obu algorytmow 
	short** tabelaOdleglosci;
	short* tabelaWynikowDlaAlgorytmuNajblizszegoSasiada;
	int iloscMiast;			
	std::fstream plikCzasWykonaniaAlgorytmu;
	std::string nazwaPlikuCzasWykonaniaAlgorytmu;

	//pola dla algorytmu nablizszego sasiada
	int tempSasiada;
	int badaneMiasto;
	int zapis;
	bool blokada;
	int dlugoscDrogiNajblizszegoSasiada;

	//pola dla algorytmu najmniejszej krawedzi
	short** tabelaSprawdzeniePola;
	short* tabelaIloscPolaczenKrawedzi;
	short* tabelaPosortowaneKrawedzie;
	int tempKrawedzi;
	int tempKrawedzi_i;
	int tempKrawedzi_j;
	int dlugoscDrogiNajmniejszejKrawedzi;

public:
	NNA() {
		iloscMiast = 0;
		tempSasiada = 0;
		badaneMiasto = 0;
		zapis = 0;
		blokada = false;
		nazwaPlikuCzasWykonaniaAlgorytmu = "wynik";
		dlugoscDrogiNajblizszegoSasiada = 0;
		dlugoscDrogiNajmniejszejKrawedzi = 0;

		tempKrawedzi = 0;
		tempKrawedzi_i = 0;
		tempKrawedzi_j = 0;
	}

	void createTest() {
			
		plikCzasWykonaniaAlgorytmu.open(nazwaPlikuCzasWykonaniaAlgorytmu, std::ios::out | std::ios::app);
		
		tworzenieTablic();
		//wyswietlanieTablicyOdleglosci();
		//std::cout << std::endl;
		wykonanieAlgorytmuNajblizszegoSasiada();
		wykonanieAlgorytmuNajmniejszejKrawedzi();
		//std::cout << std::endl;
		//pokazWynik();
		//std::cout << std::endl;
	}

	void tworzenieTablic() {
		srand(time(NULL));
		std::cout << "Podaj ilosc miast - " << " ";
		std::cin >> iloscMiast;
		std::cout << std::endl;

		//tworzenie tabel dla obu algorytmow
		tabelaOdleglosci = new short* [iloscMiast];
		for (int i = 0; i < iloscMiast; ++i) {
			tabelaOdleglosci[i] = new short[iloscMiast];
		}

		for (int i = 0; i < iloscMiast; i++) {
			for (int j = 0; j < iloscMiast; j++) {
				if (i == j) {
					tabelaOdleglosci[i][j] = 10000;
				}
				else {
					tabelaOdleglosci[i][j] = (std::rand()%100);
				}
			}
		}

		//tworzenie tabel dla algorytmu najblizszego sasiada 
		tabelaWynikowDlaAlgorytmuNajblizszegoSasiada = new short[iloscMiast];

		//tworzenie tabel dla algorytmu Najmniejszej Krawedzi
		tabelaIloscPolaczenKrawedzi = new short[iloscMiast];
	
		tabelaPosortowaneKrawedzie = new short[iloscMiast];

		tabelaSprawdzeniePola = new short* [iloscMiast];
		for (int i = 0; i < iloscMiast; ++i) {
			tabelaSprawdzeniePola[i] = new short[iloscMiast];
		}

		for (int i = 0; i < iloscMiast; i++) {
			tabelaIloscPolaczenKrawedzi[i] = 0;
		}
		
	}

	void wyswietlanieTablicyOdleglosci() {

		
		std::cout << "Oto odleglosci miedzy miastami:" << std::endl;

		for (int i = 0; i < iloscMiast; i++) {
			for (int j = 0; j < iloscMiast; j++) {
				std::cout << tabelaOdleglosci[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	void wykonanieAlgorytmuNajblizszegoSasiada() {

		auto start = high_resolution_clock::now();

		for (int i = 0; i < iloscMiast; i++) {
			
			tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[i] = zapis;
			tempSasiada = 10000;
			
			for (int j = 0; j <= iloscMiast; j++) {
				
				if (j == iloscMiast) {
					badaneMiasto = zapis;
					continue;
				}

				for (int z = 0; z < iloscMiast; z++) {
				
					if (j == tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[z]) {
						blokada = true;
						break;
					}
					else {
						blokada = false;
						continue;
					}
				}
				
				if (blokada != true) {
					if (tempSasiada > tabelaOdleglosci[badaneMiasto][j]) {
						tempSasiada = tabelaOdleglosci[badaneMiasto][j];
						dlugoscDrogiNajblizszegoSasiada += tabelaOdleglosci[i][j];
						zapis = j;
					}
				}
			}
		}
		tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[iloscMiast - 1] = tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[0];
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		std::cout << "Czas wykonania algorytmu najblizszego sasiada: "  << duration.count() << "microseconds" << std::endl;
		std::cout << "Odleglosc pokonana: " << dlugoscDrogiNajblizszegoSasiada << std::endl;
		plikCzasWykonaniaAlgorytmu << "Czas wykonania algorytmu najbliszego sasiada dla " << iloscMiast << " miast: " << duration.count() << " micro sekund.\n" <<"Odleglosc pokonana: " << dlugoscDrogiNajblizszegoSasiada <<"\n";
	}

	void wykonanieAlgorytmuNajmniejszejKrawedzi() {

		auto start = high_resolution_clock::now();

		for (int h = 0; h < iloscMiast; h++)
		{
			tempKrawedzi = 10000;
			for (int i = 0; i < iloscMiast; i++) {
				if (tabelaIloscPolaczenKrawedzi[i] != 2) {
					for (int j = 0; j < iloscMiast; j++) {
						if (tabelaIloscPolaczenKrawedzi[j] != 2) {
							if (tabelaSprawdzeniePola[i][j] != tabelaOdleglosci[i][j] && tabelaSprawdzeniePola[j][i] != tabelaOdleglosci[j][i]) {
								if (tabelaOdleglosci[i][j] < tempKrawedzi) {
									tempKrawedzi = tabelaOdleglosci[i][j];
									tempKrawedzi_i = i;
									tempKrawedzi_j = j;
									dlugoscDrogiNajmniejszejKrawedzi += tabelaOdleglosci[i][j];
								}
							}
						}
					}
				}
			}
	
			tabelaPosortowaneKrawedzie[h] = tempKrawedzi;
			tabelaSprawdzeniePola[tempKrawedzi_i][tempKrawedzi_j] = tabelaOdleglosci[tempKrawedzi_i][tempKrawedzi_j];
			tabelaSprawdzeniePola[tempKrawedzi_j][tempKrawedzi_i] = tabelaOdleglosci[tempKrawedzi_j][tempKrawedzi_i];
			if (tabelaIloscPolaczenKrawedzi[tempKrawedzi_i] == 0) {
				tabelaIloscPolaczenKrawedzi[tempKrawedzi_i] = 1;
			}
			else if (tabelaIloscPolaczenKrawedzi[tempKrawedzi_i] == 1) {
				tabelaIloscPolaczenKrawedzi[tempKrawedzi_i] = 2;
			}
			if (tabelaIloscPolaczenKrawedzi[tempKrawedzi_j] == 0) {
				tabelaIloscPolaczenKrawedzi[tempKrawedzi_j] = 1;
			}
			else if (tabelaIloscPolaczenKrawedzi[tempKrawedzi_j] == 1) {
				tabelaIloscPolaczenKrawedzi[tempKrawedzi_j] = 2;
			}
		}

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		std::cout << "Czas wykonania algorytmu najmniejszej krawedzi: " << duration.count() << "microseconds" << std::endl;
		std::cout << "Odleglosc pokonana: " << dlugoscDrogiNajmniejszejKrawedzi << std::endl;
		plikCzasWykonaniaAlgorytmu << "Czas wykonania algorytmu najmniejszej krawedzi dla " << iloscMiast << " miast: " << duration.count() << " micro sekund.\n" << "Odleglosc pokonana: " << dlugoscDrogiNajmniejszejKrawedzi << "\n";
	}

	void pokazWynik() {

		//wynik dla algorytmu najblizszego sasiada
		std::cout << "Oto najszybsza droga od 1 do ostatniego miasta rozwiazana za pomoca algorytmu najblizszego sasiada \nKolejnosc miast: ";
		for (int i = 0; i < iloscMiast; i++) {
			if (i + 1 != iloscMiast) {
				std::cout << tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[i] << " -> ";
			}
			else
			{
				std::cout << tabelaWynikowDlaAlgorytmuNajblizszegoSasiada[i];
			}
		}

		//wynik dla algorytmu najmniejszej krawedzi
		std::cout << "\n\n" <<"Oto najszybsza droga od 1 do ostatniego miasta rozwiazana za pomoca algorytmu najmniejszej krawedzi \nKolejnosc tworzenia drog miedzy miastami(pokazane sa wartosci krawedzi): ";
		for (int i = 0; i < iloscMiast; i++) {
			std::cout<< tabelaPosortowaneKrawedzie[i] << " ";
		}
	}

	~NNA()
	{
		delete[] tabelaWynikowDlaAlgorytmuNajblizszegoSasiada;
		for (int z = 0; z < iloscMiast; z++) {
			delete[] tabelaOdleglosci[z];
		}
		delete[] tabelaOdleglosci;
	
		delete[] tabelaIloscPolaczenKrawedzi;
		delete[] tabelaPosortowaneKrawedzie;
		for (int z = 0; z < iloscMiast; z++) {
			delete[] tabelaSprawdzeniePola[z];
		}
		delete[] tabelaSprawdzeniePola;
	}

};

int main() {

	NNA nna;
	nna.createTest();

	return 0;
}