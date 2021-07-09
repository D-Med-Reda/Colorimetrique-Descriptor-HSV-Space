// Descripteur_Color_Descriptor_RBG.cpp.

// Importation des fichier d'entete
#include "Video_Operation.hpp"


using namespace std;

bool Mode = true;
char rec;

int main() {


	Video_Operation Obj;                                              

	/* Infinit loop to ask the user if he want to retry.*/ 

	while (Mode == true) {                                           
		cout << "\t \t \t \t \t \t ORB-SV DESCRIPTOR \n "
			<< "\t\t\t---------------------------------------------------------------------------- \n\n";

		cout << "\n Detection, description and matching of a video file.\n";
		
		Obj.Video_Process(Obj);

		cout << "\n Do you want to retry (y/n) ? : ";
		cin >> rec;

		if (rec == 'y' || rec == 'Y') {

		}
		else {
			Mode = false;
			break;
		}
	}


	return 0;
}