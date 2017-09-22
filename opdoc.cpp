using namespace std;

#include <iostream>
#include <string>
#include <fstream>
#include<cstdio>
string ouvrir_fichier(string nom);


int main(){
	 
	ouvrir_fichier("text.txt");
return 0;}



string ouvrir_fichier(string name){

string phrase;
int nbligne=0;
int i;
		ifstream fichier("test.txt", ios::in);
		if (fichier)
			{
		

			cout<<" le fichier est bien ouvert"<< endl;
			while(std::getline(fichier, phrase)){
			nbligne++;
			cout << phrase << endl;
			cout<< nbligne<<endl;
			}
			
			getline(fichier, phrase);// lit la phrase et la met dans le string phrase
			fichier.close();
			return phrase;
			} 
		else cout<< "impossible d'ouvrir le fichier"<<endl;

}


// ouvrir un fichier.vhd ==> ok
// mettre dans un tableau ok
//attention si phrase plus longue !!! ok
// afficher le fichier.txt g++ fichier.cpp -o nom
//en entree en sortie le type de donnée et les fcts (algorithme)





