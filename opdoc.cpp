using namespace std;

#include <iostream>
#include <string>
#include <fstream>



int main(){

string phrase;
int nbligne;
int i;

	ifstream fichier("test.vhd", ios::in); 
		if (fichier)
			{

			cout<<" le fichier est bien ouvert"<< endl;
			while(std::getline(fichier, phrase)){
			nbligne++;
			cout << phrase << endl;}
			cout<< nbligne<<endl;
			getline(fichier, phrase);// lit la phrase et la met dans le string phrase
			
			// affiche
			
			//passage a la ligne + gere le caractere
			

			fichier.close();
			
			} 
		else cout<< "impossible d'ouvrir le fichier"<<endl;
return 0;
}


// ouvrir un fichier.vhd ==> ok
// mettre dans un tableau ok
//attention si phrase plus longue !!! ok
// afficher le fichier.txt g++ fichier.cpp -o nom
//en entree en sortie le type de donnée et les fcts (algorithme)





