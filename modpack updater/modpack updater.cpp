// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string> 
#include <shlobj.h>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <Lmcons.h>
#include <direct.h>


using namespace std;


int main(void)
{
	
	//trouvage du nom
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	cout << "salut mon bo " << username << endl;

	//trouvage du chemin
	TCHAR mcdir[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_APPDATA | CSIDL_FLAG_CREATE,
		NULL,
		0,
		mcdir)))
	{
		std::cout << "dossier Appdata trouve" << endl;
	}
	
	//formatage du chemin
	string mcdi = mcdir;
	mcdi += "\\.minecraft";

	//conversion de l'adresse en string
	TCHAR* bob = mcdir;
	string s_mcdir = bob;
	
	//ajout d'un profil
	//moddifcation de prof.json
	string prof = mcdi += "\\launcher_profiles.json";

	string line;
	ifstream myfile(prof);
	ostringstream o_prof;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.find("\"profiles\" : {") == 2) {
				bool f_da = 0;
				o_prof << "  \"profiles\" : {" << endl;
				while (getline(myfile, line)) {
					if (line == "    \"7492cfc59345268a57518353a3b46b80\" : {") {
						cout << "Profile deja ajoute\n";
						f_da = 1;
					}
					if (line == "    }" and not f_da) {
						o_prof
							<< "    }," << endl
							<< "    \"7492cfc59345268a57518353a3b46b80\" : {" << endl
							<< "      \"created\" : \"2020-01-24T04:07:39.132Z\", " << endl
							<< "      \"gameDir\" : \"C:\\\\Users\\\\" << username << "\\\\AppData\\\\Roaming\\\\.modpack\", " << endl
							<< "      \"icon\" : \"Cake\", " << endl
							<< "      \"javaArgs\" : \"-Xmx4G -XX:+UnlockExperimentalVMOptions -XX:+UseG1GC -XX:G1NewSizePercent=20 -XX:G1ReservePercent=20 -XX:MaxGCPauseMillis=50 -XX:G1HeapRegionSize=32M\"," << endl
							<< "      \"lastUsed\" : \"2020-01-24T04:07:50.464Z\"," << endl
							<< "      \"lastVersionId\" : \"1.12.2-forge1.12.2-14.23.5.2847\"," << endl
							<< "      \"name\" : \"Le serveur qui est le meilleur\"," << endl
							<< "      \"type\" : \"custom\"" << endl
							<< "	}" << endl;
						cout << "profile ajoute avec succes\n";
					}
					else o_prof << line << '\n';
				}
			}
			else o_prof << line << '\n';
		}
		myfile.close();

	}
	else cout << "Unable to open file\n";

	//sauvegarde de launcher_profile.json
	ofstream newprof;
	newprof.open(prof.c_str());
	if (newprof.is_open()) {
		newprof << o_prof.str();
		newprof.close();
		puts("profile modifie avec succes\n");
	}
	else
		puts("Erreur lors de l'ecriture des profils");
	
	//update/installation du modpack
	string moddir = mcdir;
	moddir += "\\.modpack";
	cout << moddir << endl;
	if (CreateDirectory(moddir.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		if (_chdir(moddir.c_str()) == 0) {
			TCHAR NPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, NPath);
			string git_p = string(NPath) + "\\.git";
			struct stat info;

			//test de l'existance de .git
			if (stat(git_p.c_str(), &info) != 0) {
				printf(".git existe po\n");
				system("git init");
			}
			else if (info.st_mode & S_IFDIR) {
				printf(".git existe\n");
			}
			else
				printf(".git est po un dossier\n");

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			//update du modpack
			system("git add config");
			system("git add mods");
			system("git add resourcepacks");
			system("git add resource");
			system("git add servers.dat");
			system("git -c user.name='normie' -c user.email='my@email.org' commit -m '...'");
			/*printf("config\n");
			string conf = moddir + "\\config";
			printf(conf.c_str());
			filesystem::remove_all(conf);*/

			SetConsoleTextAttribute(hConsole, 10);
			printf("-------------------------------------anakin stop panikin everything's chill-------------------------------------\n");
			SetConsoleTextAttribute(hConsole, 14);
			printf("Sauf si �a dit: error: The following untracked working tree files would be overwritten by merge:\nDans ce cas il faut aller supprimer les fichiers dont il parle\n");
			SetConsoleTextAttribute(hConsole, 15);
			printf("pull\n");
			system("git pull https://github.com/MrMushroomkiller/le-meiuleur-serheuer.git");
			printf("restore\n");
			system("git restore .");
			system("pause");
		}
	}
	else
	{
		cout << "Failed to create directory\n";
	}



	return 0;
}

/*
"59efa1743311ab816385ef945539c77b" : {
	  "created" : "2020-01-23T16:18:06.380Z",
	  "gameDir" : "C:\\Users\\mimas\\AppData\\Roaming\\le_meilleur_serveur",
	  "icon" : "Cake",
	  "javaArgs" : "-Xmx4G -XX:+UnlockExperimentalVMOptions -XX:+UseG1GC -XX:G1NewSizePercent=20 -XX:G1ReservePercent=20 -XX:MaxGCPauseMillis=50 -XX:G1HeapRegionSize=32M",
	  "lastUsed" : "2020-01-21T01:48:52.398Z",
	  "lastVersionId" : "1.12.2-forge1.12.2-14.23.5.2847",
	  "name" : "Le meilllllllleur serv\u00e9",
	  "type" : "custom"
	},
*/