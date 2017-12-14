#include "arbre.h"

void fonction_arbre(const vector<Lexeme> &lex){
	tree<Lexeme> tr;
	tree<Lexeme>::pre_order_iterator it=tr.begin();
	it=tr.insert(it, Lexeme("root","root",0));
	string::size_type i;
	for(i=0;i<lex.size();++i){
		if(lex[i].getetiquette() == "library")
			arbre_lib(tr,lex,i);
		if(lex[i].getetiquette() == "entity")
			arbre_entity(tr,lex,i);
		if(lex[i].getetiquette() == "architecture")
			arbre_arch(tr,lex,i);
	}
	cout << "voici l'arbre" << endl;
	affiche_arbre(tr);
}

//fonction qui crée l'arbre de la déclaration des libs. V1.0.
//Pour l'instant ne gère qu'une seule lib ou plusieurs si déclarées correctement (tous les cas ne sont pas traités)
//A pofiner si y'a le temps
void arbre_lib(tree<Lexeme> &tr,const vector<Lexeme> &lex, string::size_type &i){ 
	tree<Lexeme>::pre_order_iterator it=tr.begin();
	tree<Lexeme>::pre_order_iterator it_nom;
	tree<Lexeme>::pre_order_iterator it_pack;
	it=tr.append_child(it, Lexeme("library","root",0));
	while(lex[i].getetiquette() != "entity"){
		if(lex[i].getetiquette() == "library"){
			++i;
			it_nom=tr.append_child(it, lex[i]);
		}
		if(lex[i].getetiquette() == "use"){
			i=i+3;
			it_pack=tr.append_child(it_nom, lex[i]);
		}
		++i;
	}
}

// fonction qui crée l'arbre de l'entity
void arbre_entity(tree<Lexeme> &tr,const vector<Lexeme> &lex, string::size_type &i){
	tree<Lexeme>::pre_order_iterator it=tr.begin();
	it=tr.append_child(it, lex[i]);
	tree<Lexeme>::sibling_iterator it_nom; // dans le cas ou il y a plusieurs entity
	tree<Lexeme>::sibling_iterator it_in;
	tree<Lexeme>::sibling_iterator it_out;
	while(lex[i].getetiquette() != "architecture"){ // tant qu'on est dans l'entity
		if(lex[i].getetiquette() == "entity" && lex[i+1].getetiquette() == "id"){ // on récupère l'id de l'entity
			++i;
			it_nom=tr.append_child(it, lex[i]); // on le place en dessous d'entity dans l'arbre
		}
		if(lex[i].getetiquette() == "port"){ // on rentre dans le port : gestion des entrées/sorties
			it=tr.append_child(it_nom,lex[i]); // on crée le noeud port
			arbre_port(tr,lex,i,it);
		}
		++i;
	}
}

void arbre_port(tree<Lexeme> &tr,const vector<Lexeme> &lex, string::size_type &i,tree<Lexeme>::pre_order_iterator &it){
	tree<Lexeme>::sibling_iterator it_in;
	tree<Lexeme>::sibling_iterator it_out;
	it_in=tr.append_child(it,Lexeme("in","in",lex[i].getligne())); // on place le noeud des entrées
	it_out=tr.append_child(it,Lexeme("out","out",lex[i].getligne())); // le noeud des sorties
	while(lex[i].getnom() != ";" && lex[i+1].getnom() != "end"){
		if(port_in_out(lex,i) == "in"){ // si on a "in", on met les id en  tant qu'entrée
			while(lex[i+1].getnom() != ";"){
				if(lex[i].getetiquette() == "id")
					it=tr.append_child(it_in,lex[i]);
				if(lex[i].getetiquette() == "in"){
					it=tr.append_child(it,lex[i+1]); // nature de l'entrée
					if(lex[i+1].getnom() == "std_logic_vector" || lex[i+1].getnom() == "bit_vector"){
						while(lex[i+2].getnom() != ";"){
							if(lex[i].getetiquette() == "downto"){
								int nb1 = stoi(lex[i-1].getnom(),nullptr,10); // on converti le nom du lexeme en un entier
								int nb2 = stoi(lex[i+1].getnom(),nullptr,10);
								nb1=nb1-nb2+1; // on fait les opérations qui vont bien sur les entiers
								tr.append_child(it,Lexeme(to_string(nb1),"nombre",lex[i].getligne())); // on place le résultat dans l'arbre
							}
							else if (lex[i].getetiquette() == "to"){
								int nb1 = stoi(lex[i-1].getnom(),nullptr,10);
								int nb2 = stoi(lex[i+1].getnom(),nullptr,10);
								nb1=nb2-nb1+1;
								tr.append_child(it,Lexeme(to_string(nb1),"nombre",lex[i].getligne()));
							}
							++i;
						}
					}
				}	
				++i;
			}
		++i;
		}
		if(port_in_out(lex,i) == "out"){ // si on a "out", on met les id en  tant que sortie
			while(lex[i+1].getnom() != ";"){
				if(lex[i].getetiquette() == "id")
					it=tr.append_child(it_out,lex[i]);
				if(lex[i].getetiquette() == "out"){
					it=tr.append_child(it,lex[i+1]); // nature de l'entrée
					if(lex[i+1].getnom() == "std_logic_vector" || lex[i+1].getnom() == "bit_vector"){
						while(lex[i+2].getnom() != ";"){
							if(lex[i].getetiquette() == "downto"){
								int nb1 = stoi(lex[i-1].getnom(),nullptr,10);
								int nb2 = stoi(lex[i+1].getnom(),nullptr,10);
								nb1=nb1-nb2+1;
								tr.append_child(it,Lexeme(to_string(nb1),"nombre",lex[i].getligne()));
							}
							else if (lex[i].getetiquette() == "to"){
								int nb1 = stoi(lex[i-1].getnom(),nullptr,10);
								int nb2 = stoi(lex[i+1].getnom(),nullptr,10);
								nb1=nb2-nb1+1;
								tr.append_child(it,Lexeme(to_string(nb1),"nombre",lex[i].getligne()));
							}
							++i;
						}
					}
				}
				++i;
			}
			++i;
		}
		++i;
	}
}

void arbre_arch(tree<Lexeme> &tr,const vector<Lexeme> &lex, string::size_type &i){
	tree<Lexeme>::pre_order_iterator it=tr.begin();
	it=tr.append_child(it, lex[i]); // on crée la branche architecture
	tree<Lexeme>::sibling_iterator it_partie;
	while(lex[i].getnom() != "end" && lex[i+1].getnom() != "architecture"){
		if(lex[i].getetiquette() == "architecture" && lex[i+1].getetiquette() == "id"){
			++i;
			it_partie=tr.append_child(it, lex[i]); // branche
		}
		if(lex[i].getetiquette() == "component" && lex[i-1].getnom() != "end"){
			++i;
			it=tr.append_child(it_partie,lex[i]);
			++i;
			arbre_port(tr,lex,i,it);
		}
		if(lex[i].getetiquette() == "if"){
			it=append_child(it_partie,lex[i]);
			arbre_if(tr,lex,i,it);
		}
		++i;
	}
}

void arbre_if(tree<Lexeme> &tr,const vector<Lexeme> &lex, string::size_type &i,tree<Lexeme>::pre_order_iterator &it){
	tree<Lexeme>::sibling_iterator it_if = tr.append_child(it,lex[i]);
	int compteur = 0;
	while(lex[i].getnom() != "end" && lex[i+1].getnom() != "if"){
		while(lex[i].getetiquette() != "symbole" && lex[i].getnom() != "(") ++i;
		string nom_operande = "";
		while(lex[i].getetiquette() == "symbole"){ nom_operande = nom_operande + lex[i].getnom(); ++i; compteur++;}
		it=tr.append_child(it_if,Lexeme("nom_operande","operande",lex[i].getligne())); // branche de l'opérande
		tr.append_child(it,lex[i-compteur]; // sous-branche de l'opérande 1 : membre de gauche
		tr.append_child(it,lex[i+1]; // sous-branche de l'opérande 2 : membre de droite
		while(lex[i].getnom() == "then") ++i;
		it=tr.append_child(it_if,lex[i]);
		++i;
		tr.append_child(it, lex[i]); // branche then
		
		++i;
	}
}

//fonction qui permet de déterminer si ce sont des entrées ou des sorties.
string port_in_out (const vector<Lexeme> &lex, string::size_type i){
	for(string::size_type j=i;lex[j].getnom() != ";";++j){
		if(lex[j].getetiquette() == "in")
			return "in";
		else if(lex[j].getetiquette() == "out")
			return "out";
	}
	return "erreur, pas de port";
}

	
// fonction qui affiche simplement l'arbre
void affiche_arbre(const tree<Lexeme> &tr){
	tree<Lexeme>::pre_order_iterator it=tr.begin();
	while(it != tr.end()){
		for(int i=0; i<tr.depth(it);++i)
			cout << " | ";
		cout << (*it).getnom() << endl;
		++it;
	}
}
