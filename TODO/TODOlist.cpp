#include <ctime>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>

std::string create_ID(){ 
    //Cette première fonction extrait un chiffre d'un document (id.txt) et l'incrémente de un et renvoie le résultat. Dans notre programme, id.txt contiendra la donnée de la
    //plus grande ID utlisée. Ce programme permet alors de générer des ID uniques. 
    std::ifstream ID ("ID.txt");
    std::string line;
    getline (ID,line);
    int i = std::stoi(line);
    // i contient la plus grande ID
    ID.close();
    i++; // On incrémente i
    std::ofstream ID2 ("ID.txt");
    ID2 << std::to_string(i) << "\n"; // On le stocke dans id.txt
    ID2.close();
    return std::to_string(i); // enfin on le renvoie
}

std::string date(){
    //date utilise la libraire ctime pour renvoyer la date à l'instant de son exécution, par exemple Tue Mar 31 11:55:45 2020
    time_t now = std::time(0);
   
    std::string dt = std::ctime(&now);
    return dt;
}

class tache{
    public:
    std::string titre;
    std::string description;
    std::string debut;
    std::string fin;
    std::string statut;
    std::string avancement;
    std::string priorite;
    std::string coms;
    std::string ID;

    tache():debut(date()),fin("Tache non cloturée \n"),ID(create_ID()){ 
        //Cette fonction permet à l'utilisateur de créer sa tache petit à petit, plutot que de demander à ce qu'il mette tous les paramètres d'un coup
        std::cout << "Bienvenue dans l'outil de création de tache pour commencer, veuillez lui donner un titre"<<std::endl;
        std::cout << "Titre: ";
        std::getline(std::cin,titre);
        //pour récupérer les entrées utilisateurs j'ai utlisé cette notation plutôt que directement "std::cin titre" ce qui permet de ne garder que
        std::cout << std::endl<<"Description: ";
        std::getline(std::cin,description);
        std::cout << std::endl<<"Statut: ";
        std::getline(std::cin,statut);
        std::cout << std::endl<<"Avancement: ";
        std::getline(std::cin,avancement);
        std::cout << std::endl<<"Priorité: ";
        std::getline(std::cin,priorite);
        std::cout << std::endl<< "Commentaires: ";
        std::getline(std::cin,coms);
        //Une fois que la tâche a été crée on la stocke dans un fichier texte grâce à la fonction store(). J'ai choisi ce mode de stockage car il m'a semblé simple à implémenter.
        store();      
    }
    tache(std::string t){
        //Ce constructeur effectue l'opération inverse du précédent, il prend une tache stockée dans un fichier texte pour créer un élément de la classe tache
        std::string extension=".txt";
        std::ifstream task (t+extension);
        std::getline(task,titre);
        std::getline(task,description);
        std::getline(task,debut);
        std::getline(task,fin);
        std::getline(task,statut);
        std::getline(task,avancement);
        std::getline(task,priorite);
        std::getline(task,coms);
        std::getline(task,ID);
        task.close();
    }
    void affiche(){
        //Affiche() permet de montrer tous les éléments d'une tache de manière lisible.
        std::cout << "Titre: "<< titre << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Date de création: " << debut << std::endl;
        std::cout << "Date de cloture: " << fin << std::endl;
        std::cout << "Statut: "<< statut << std::endl;
        std::cout << "Avancement: "<< avancement << std::endl;
        std::cout << "Priorité: " << priorite << std::endl;
        std::cout << "Commentaires: " << coms<< std::endl;
    }
    void store(){
        //Stockage des taches dans des fichiers textes
        std::ofstream myfile (ID+".txt");
        //Les fichiers textes ont pour nom l'ID unique qu'on leur a attribué, pour éviter les problèmes en cas de noms identiques
        //Dans un premier temps on crée la tache 
        if (myfile.is_open()){
            myfile << titre << "\n";
            myfile << description << "\n";
            myfile << debut;
            myfile << fin;
            myfile << statut << "\n";
            myfile << avancement << "\n";
            myfile << priorite << "\n";
            myfile << coms << "\n";
            myfile << ID << "\n";
            myfile.close();  //Il est important de fermer les fichiers utilisés pour éviter tout problème lorsqu'on les réutilise.
            // Ensuite, on stocke l'ID de la tache dans la TODOlist
            std::ofstream list ("TODOlist.txt",std::ios::app); // le deuxième argument (std::ios::app) permet d'écrire à la suite du document, sinon le contenu de celui-ci
                                                               // est effacé quand on rajoute du texte
                if (list.is_open()){
                    list << ID << "\n";
                    list.close();
                    std::cout << "La tâche a bien été créée \n";
                }   
                else{
                    std::cout << "Erreur lors de la création, veuillez réessayer \n";
                    }
        }
        else{
            std::cout << "Erreur lors de la création, veuillez réessayer \n";
        }
    }
    
};

void clear_list(){
    //Clear_list() permet de supprimer la TODOlist, elle comporte 3 étapes différentes
    //D'abord, on parcourt todolist.txt pour avoir le nom des taches et ensuite les supprimer.
    std::cout << "Voulez vous vraiment réinitialiser votre to-do list? Entrez 'oui' pour confirmer ou autre chose pour annuler \n"<< "Réponse: ";
    std::string rep;
    std::string titres;
    std::string extension= ".txt";
    std::getline(std::cin, rep);
    if ((rep == "oui") || (rep == "Oui")){
        std::ifstream list ("todolist.txt");
        
        while (std::getline(list,titres)){
        //La fonction std::getline parcourt à chaque itération une nouvelle ligne du document. Si cette ligne contient quelque chose elle renvoie true et la stocke dans
        //titres. Sinon elle renvoie false et on sort de la boucle, tout le document a été parcouru.
            std::remove((titres+extension).c_str());
        }
        list.close();

        //Ensuite on supprime le contenu de la TODOlist
        std::ofstream liste ("todolist.txt");
        //En ouvrant todolist.txt avec ofstream et sans std::ios::app, son contenu est supprimé.
        liste.close();
        std::cout << "Réinitialisation réussie \n";

        //Enfin on réinitialise les ID à 0
        std::ofstream ID ("ID.txt");//On efface le contenu de ID avec la même méthode
        ID << 0 << "\n";//ON remet le compteur à 0
        ID.close();
    }
    else{
        std::cout << "Opération annulée \n";
    }
}


void affiche_compact_liste( std::string list = "TODOlist.txt", std::string txt = "Voici la liste compacte des taches existantes \n"){
    //Cette fonction renvoie la liste des titres des taches existantes, elle sera aussi réutilisée lors du tri par certains critères
    std::ifstream liste (list);
    std::string ligne;
    std::string extension= ".txt";
    std::string nom;
    std::string titre;
    std::cout << txt;
    if (liste.is_open())
        {
        while ( std::getline (liste,ligne) ){
            nom=ligne+extension;
            std::ifstream tache (nom);
            std::getline(tache,titre);
            //getline stocke ici la première ligne du document dans titre, qui est justement celle qui nous intéresse car elle correspond au titre.
            std::cout << titre << "\n";
    }
    liste.close();
  }
}

void affiche_details_liste(std::string list= "TODOlist.txt", std::string txt = "Voici la liste détaillée des taches existantes \n"){ 
    //Cette fonction renvoie la liste des taches existantes avec toutes leurs caractéristiques, elle sera aussi réutilisée lors du tri par certains critères
    std::ifstream liste (list);
    std::string ligne;
    std::string extension= ".txt";
    std::string nom;
    std::string titre;
    std::cout << txt;
    int k=1;
    if (liste.is_open()){
        while ( std::getline (liste,ligne) ){// A chaque étape on récupère l'id d'une tache
            tache t(ligne); //Et on utilise le constructeur dédié
            std::cout<< "Tache n°"<<k<<"\n";
            t.affiche();// Et la fonction affiche définie plus haut, cela simplifie l'écriture de cette fonction
            std::cout<< '\n';
            std::cin.get();
            k++;
        }   
        liste.close();
    }
    std::cout << "\n";
    
}

void affiche_choix(){
    //Simple fonction permettant à l'utilisateur de choisir entre les 2 modes d'affichage
    std::string k;
    std::cout << " \nSouhaitez-vous un affichage compact ou détaillé? \n \n";
    std::cout << "1.     Compact \n";
    std::cout << "2.     Détaillé \n";
    std::cout << "Affichage: ";
    std::getline(std::cin,k);
    if (k == "1"){
        affiche_compact_liste("TODOlist.txt", "Voici l'affichage compact des taches: \n");
    }
     else{
        if (k=="2"){
            affiche_details_liste("TODOlist.txt", "Voici l'affichage détaillé des taches: \n");
        }
        else{
            std::cout << "Entrée invalide \n";
        }
    }
}

void supprimer(){
    //Supprime toutes les taches possédant un certain nom.
    affiche_compact_liste();
    std::cout<< "\nQuelle tache voulez vous supprimer ? \n\nTitre:";
    std::string t;
    std::getline(std::cin,t);
    std::string ligne;
    std::string nom;
    std::cout << "Voulez vous vraiment supprimer toutes les taches nommées " << t <<" ? Entrez 'oui' pour confirmer ou autre chose pour annuler \n"<< "Réponse: ";
    std::string rep;
    std::string titre;
    std::string extension= ".txt";
    std::getline(std::cin, rep);
    std::ofstream temp;
    //On crée un fichier texte temp.txt qui va être notre nouvelle TODOlist.txt, sans les éléments supprimés
    temp.open("temp.txt");
    if ((rep == "oui") || (rep == "Oui")){
        std::ifstream list ("todolist.txt");
        
        while (std::getline(list,ligne)){
            nom=ligne+extension;
            std::ifstream tache (nom);
            std::getline(tache,titre);
            if (titre != t){
            //Si titre est différent de t, il faut conserver la tache donc on stocke son id dans temp.txt
               temp << ligne << "\n";
            }
            else{
                //Sinon on supprime le fichier grâce à la fonction remove qui prend en argument un char* donc on utlise c_srt pour convertir nos strings
                std::remove((nom).c_str());
            } 
        }
        temp.close();
        list.close();
        std::remove("todolist.txt");
        std::rename("temp.txt","todolist.txt");
        //Ces deux lignes permettent de remplacer todolist.txt par temp.txt
        std::cout<<"Opération réussie \n";
    }
    else{
        std::cout << "Opération annulée \n";
    }
}

void modif(std::string nom){
    //Prend en argument le nom d'un fichier texte (une tache) et viens modifier un de ses champs
    //Pour cela on va utiliser une méthode similaire à celle de la fonction supprimer() avec un fichier temportaire
    std::cout <<"\nSelectionnez le champ que vous voulez modifier en rentrant le nombre correspondant \n";
    std::cout << "1.           Statut \n";
    std::cout << "2.           Avancement \n";
    std::cout << "3.           Priorité \n";
    std::cout << "4.           Commentaires \n";
    std::cout << "Champ: ";
    std::string input;
    std::getline(std::cin,input);
    if (input=="1" ||input=="2" ||input=="3" ||input=="4"){
        int j = std::stoi(input);
        //On utilise le nombre renseigné par l'utilisateur pour savoir quel ligne du fichier modifier.
        std::ofstream temp;
        temp.open("temp.txt");
        //Le fichier temp va remplacer celui que l'on veut modifier
        std::ifstream task;
        std::string ligne;
        int i = 0;
        task.open(nom);
        while (std::getline(task,ligne)){
            if (i != j+3){
            //Si  i est différent de j on est sur la ligne que l'on ne veut pas modifier donc on se contente de la copier dans temp
                temp << ligne << "\n";
            }
            else{
                //Sinon l'utilisateur renseigne ce par quoi il veut remplacer le champ
                std::cout << "Ce champ contient actuellement le texte suivant: '" << ligne << "'. Par quoi voulez-vous le remplacer? \nNouveau contenu: ";
                std::string nouveau;
                std::getline(std::cin, nouveau);
                temp << nouveau << "\n";
            } 
            i++;
        }
        temp.close();
        task.close();
        //Enfin, on remplace le fichier à modifier par temp
        std::remove(nom.c_str());
        std::rename("temp.txt",nom.c_str());
        std::cout<<"Opération réussie \n";
    } 
    else{
        std::cout << "Entrée invalide";
    }
}
 
void modifier (){
    //Permet à l'utilisateur de choisir quel tache il souhaite modifier
    affiche_compact_liste();
    std::cout << "Quelle tache voulez vous modifier? \n\n"<< "Titre: ";
    std::string titre;
    std::string t;
    std::getline(std::cin,t);
    std::ifstream liste ("TODOlist.txt");
    std::string id;
    std::string ligne;
    std::string extension= ".txt";
    std::string nom;
    bool trouve= false;
    while ((std::getline(liste,ligne)) && (!trouve)){
        nom=ligne+extension;
        std::ifstream tache (nom);
        std::getline(tache,titre);
        if (titre == t){
            //Une fois que l'on a trouvé une tache ayant le bon nom, on lance la fonction modif.
            //Le problème ici c'est que si deux documents ont le même nom, c'est celui avec l'ID le plus petit qui sera modifié, on ne pourra jamais modifier l'autre
            modif(nom);
            trouve = true;
        }
    }
    if (!trouve){
        std::cout<< "Cette tache n'est pas dans la liste \n";
    }

    liste.close();

}

void affiche(){ 
    //Affiche() permet d'afficher (de manière compacte ou détaillé) toutes les taches qui ont une certaine caractéristique à un certain champs
    // Cette fonction est très générale. Pour une véritable utilisation, je pense que l'on pourra se restreindre à Statut, 
    //avancement et priorité comme critère de tri
    std::cout << "Par quel champ voulez vous sélectionner? Entrez le chiffre correspondant \n";
    std::cout << "1.           Titre \n";
    std::cout << "2.           Description \n";
    std::cout << "3.           Date de debut \n";
    std::cout << "4.           Date de fin \n";
    std::cout << "5.           Statut \n";
    std::cout << "6.           Avancement \n";
    std::cout << "7.           Priorité \n";
    std::cout << "8.           Commentaires \n";
    std::cout << "Champ:";
    std::string input;
    std::getline(std::cin,input);
    int i;

    //On s'assure qu'input est bien un entier
    try{
        i=std::stoi(input)-1;
    }
    catch (...){
        std::cout << "Entrée invalide \n";
        return ;
    }
    i=std::stoi(input)-1;
    

    if (i > 8){//Alors i ne correspond à aucun champ
        std::cout << "Entrée invalide \n";
        return ;
    }

    std::cout<< " \nQuelle caractéristique voulez-vous récupérer? \nCaractéristique: ";
    std::string input2;
    std::getline(std::cin,input2);

    std::ifstream list ("TODOlist.txt");
    std::string ids;
    std::string extension = ".txt";
    std::string nom;
    std::string champ;
    std::ofstream temp; // Ici temp permet de stocker les ID des taches répondant au critère de sélection
    int j=0;
    temp.open ("temp.txt");
    while (std::getline(list,ids)){
        nom=ids+extension;
        std::ifstream tache (nom);
        while (std::getline(tache,champ) and j<i){
            j++;
        }
        if (champ==input2){
            temp << ids << "\n";
        }
        j=0;
        tache.close();

    }
    temp.close();

    std::string k;
    std::cout << " \nSouhaitez-vous un affichage compact ou détaillé? \n \n";
    std::cout << "1.     Compact \n";
    std::cout << "2.     Détaillé \n";
    std::cout << "Affichage:";
    std::getline(std::cin,k);
    //On réutilise ici les deux fonctions d'affichage mais appliqués à temp
    if (k == "1"){
        affiche_compact_liste("temp.txt", "Voici l'affichage compact des taches ayant cette caractéristique \n");
    }
     else{
        if (k=="2"){
            affiche_details_liste("temp.txt", "Voici l'affichage détaillé des taches ayant cette caractéristique \n");
        }
        else{
            std::cout << "Entrée invalide \n";
            return;
        }
    }

    std::remove("temp.txt");// On retire temp car il nous est maintenant inutile


}

void close(std::string id){
    //Remplace la partie "fin" d'une tache par la date actuelle, permet de signifier que la tâche est finie
    //On crée un fichier temp qui va remplacer la tâche
    int i = 0;
    std::ofstream temp ;
    temp.open("temp.txt");
    std::ifstream tache (id);
    std::string line;
    while (std::getline(tache,line)){
        if (i !=3){
            temp << line <<"\n";
        }
        else{
            if (line=="Tache non cloturée "){ // On s'assure que l'on n'a pas déjà cloturé la tache
                temp << date();
                std::cout<<"Opération réussie \n";
            }
            else{
                std::cout << "Cette tache est déjà cloturée! \n";
                temp << line <<"\n";
            }
        }
        i++;
    }
    temp.close();
    tache.close();
    std::remove(id.c_str());
    std::rename("temp.txt",id.c_str());
}


void cloture(){
    //Permet de choisir une tache à cloturer
    affiche_compact_liste();
    std::cout << "\nQuelle tache voulez vous cloturer? \n\n";
    std::cout << "Titre:";
    std::string t;
    std::getline(std::cin,t);
    std::ifstream liste ("TODOlist.txt");
    std::string id;
    std::string ligne;
    std::string titre;
    std::string extension= ".txt";
    std::string nom;
    bool trouve= false;
    while ((std::getline(liste,ligne)) && (!trouve)){
        nom=ligne+extension;
        std::ifstream tache (nom);
        std::getline(tache,titre);
        if (titre == t){
            //On se retrouve avec le même problème que précedemment si 2 taches ont le même nom
            close(nom);
            trouve = true;
        }
    }
    if (!trouve){
        std::cout<< "Cette tache n'est pas dans la liste \n";
    }

    liste.close();

}


int main(){
    //Enfin, le main est en quelque sorte le centre du programme, il permet à l'utilisateur de choisir quelles opérations il souhaite réaliser et de quitter une fois
    //qu'il a fini
    std::string entree;
    bool running = true;
    while (running){
        std::cout << "Bienvenue dans votre todolist, que souhaitez vous faire? Entrez le numéro correspondant \n";
        std::cout << "1.       Ajouter une tache \n";
        std::cout << "2.       Modifier une tache \n";
        std::cout << "3.       Afficher la liste des taches existantes \n";
        std::cout << "4.       Cloturer une tache \n";
        std::cout << "5.       Afficher les taches ayant une certaine propriété \n";
        std::cout << "6.       Supprimer une tache \n";
        std::cout << "7.       Supprimer la todolist \n";
        std::cout << "8.       Quitter la todolist \n";
        std::cout << "Opération:";
        std::getline(std::cin,entree);
        if (entree== "1"){
            tache t;
            std::cin.get();
        }
        else if (entree== "2"){
            modifier();
            std::cin.get();
        }
        else if (entree== "3"){
            affiche_choix();
            std::cin.get();
        }
        else if (entree== "4"){
            cloture();
            std::cin.get();
        }
        else if (entree== "5"){
            affiche();
            std::cin.get();
        }
        else if (entree== "6"){
            supprimer();
            std::cin.get();
        }
        else if (entree== "7"){
            clear_list();
            std::cin.get();
        }
        else if (entree== "8"){
            running = false;
            std::cout << "A bientôt! \n";
        }
        else{
            std::cout << "Entrée invalide, veuillez recommencer \n";
        }

    }
    return(0);
}
    