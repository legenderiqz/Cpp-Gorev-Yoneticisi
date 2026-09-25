#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

struct Task 
{
    string text;
    bool completed;
};

vector<Task> tasks;
bool appOn = true;
int input; 

void checkInput();
void addTask();
void deleteTask();
void toggleTask();
void listTasks();
void loadTasks();
void saveTasks();
void editTask();
void getHelp();
void searchTask();
void clearTasks();

int main() 
{
    loadTasks();
    while (appOn) 
    {
        cout << "------------\n"
                "Gorev Yoneticisi\n"
                "0: Cikis\n"
                "1: Gorev Ekle\n"
                "2: Gorev Sil\n"
                "3: Gorev Bitir/Yeniden Baslat\n"
                "4: Gorevleri Listele\n"
                "5: Gorevleri Kaydet\n"
                "6: Gorevleri Yukle\n"
                "7: Tum Gorevleri Temizle\n"
                "8: Gorev Duzenle\n"
                "9: Gorev Ara\n"
                "10: Yardim\n"
                "\n"
                "Girdi: ";
        if (!(cin >> input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Lutfen gecerli bir sayi girin.\n";
            continue;
        }
        checkInput();
    }
    cout << "Program Sonlandirildi." << endl;
    return 0;
}


void checkInput() 
{
    if (input == 0)
    {   
        cout << "Cikiliyor... ";
        saveTasks();
        appOn = false;
        return;
    }
    else if (input == 1) 
    {
        addTask();
    }
    else if (input == 2) 
    {
        deleteTask();
    }
    else if (input == 3) 
    {
        toggleTask();

    }
    else if (input == 4) 
    {
        listTasks();
    }
    else if (input == 5) 
    {
        saveTasks();
        cout << "Gorevler kaydedildi.\n";
    }
    else if (input == 6) 
    {
        loadTasks();
        cout << "Gorevler yuklendi.\n";
    }
    else if (input == 7) 
    {
        clearTasks();
    }
    else if (input == 8) 
    {
        editTask();
    }
    else if (input == 9) 
    {
        searchTask();
    }
    else if (input == 10) 
    {
        getHelp();
    }
    else 
    {
        cout << "Gecersiz girdi. Lutfen tekrar deneyin.\n";
    }
}


void addTask() 
{
    string text;
    cout << "Eklenecek gorev metnini girin: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, text);
    tasks.push_back({
        text,
        false
    });
    saveTasks();
    cout << "Gorev basariyla eklendi. \n";
}


void deleteTask() 
{
    int number;
    cout << "Silinecek gorev nosunu girin (0 ile geri): ";
    cin >> number;
    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Gecersiz girdi. Lutfen tekrar deneyin.\n";
        return;
    }
    if (number == 0) 
    {
        cout << "Geri donuldu.\n";
        return;
    }
    else if (number >= 1 && number <= tasks.size()) 
    {
        tasks.erase(tasks.begin() + (number - 1));
        saveTasks();
        cout << number << " numarali gorev basariyla silindi.\n";
    }
    else 
    {
        cout << "Gecersiz gorev numarası\n";
    }
}


void toggleTask() 
{
    int complete;
    cout << "Aciklama: Bu komut bitmis gorevi yeniden baslatir, devam eden gorevi bitirir.\n";
    cout << "Bitirilecek/Yeniden Baslanacak gorev numarasi (0 ile geri): ";
    cin >> complete;
    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Gecersiz girdi. Lutfen tekrar deneyin.\n";
        return;
    }

    if (complete == 0) 
    {
        cout << "Geri donuldu.\n";
        return;
    }
    else if (complete >= 1 && complete <= tasks.size())
    {
        tasks.at(complete - 1).completed = !tasks.at(complete - 1).completed;
        saveTasks();
        cout 
            << complete 
            << " numarali gorev basariyla "
            << (tasks.at(complete - 1).completed ? "bitirildi." : "yeniden baslatildi.")
            << endl;
    }
    else 
    {
        cout << "Gecersiz gorev numarası\n";
        return;
    }
}


void listTasks() 
{
    cout
        << "------------\n"
        << "Gorevler\n";
    if (tasks.size() == 0) 
    { 
        cout << "(Gorev bulunamadi)\n";
        return;
    }

    for (size_t i = 0; i < tasks.size(); i++) 
    {
        cout 
            << i + 1 
            << ". ["
            << (tasks.at(i).completed ? "X" : " ") 
            << "] "
            << tasks.at(i).text 
            << "\n";
            // 1. [ ] Lorem ipsum dolor
            // 2. [X] Sit amet consectetur
            // 3. [ ] Adipiscing elit sed
    }
}


void loadTasks() 
{
    ifstream file("tasks.txt");
    if (!file.is_open())
    {
        cout << "Gorev dosyasi acilamadi.\n";
        return;
    }

    vector<Task> loadedTasks;
    string line;

    while (getline(file, line))
    {
        size_t separator = line.find('|');
        if (separator == string::npos)
            continue;

        string statusText = line.substr(0, separator);
        if (statusText != "0" && statusText != "1")
            continue;

        loadedTasks.push_back({
            line.substr(separator + 1),
            statusText == "1"
        });
    }

    tasks = loadedTasks;
}


void saveTasks() 
{   
    ofstream file("tasks.txt");
    if (!file.is_open()) 
    {
        cout << "Gorevler kaydedilemedi.\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); i++) 
    {
        string text = tasks.at(i).text;
        bool completed = tasks.at(i).completed;
        string starting = completed ? "1" : "0";
        string result = starting + "|" + text  + "\n";
        file << result;
    }
}

void editTask() 
{
    int number;
    cout << "Duzenlenecek gorev numarasini girin (0 ile geri): ";
    cin >> number;
    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Gecersiz girdi. Lutfen tekrar deneyin.\n";
        return;
    }
    if (number == 0) 
    {
        cout << "Geri donuldu.\n";
        return;
    }
    else if (number >= 1 && number <= tasks.size()) 
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Önceki girişten kalan karakterleri temizle
        string newText;
        cout << "Yeni gorev metnini girin: ";
        getline(cin, newText);
        tasks.at(number - 1).text = newText;
        saveTasks();
        cout << number << " numarali gorev basariyla duzenlendi.\n";
    }
    else 
    {
        cout << "Gecersiz gorev numarası\n";
    }
}

void clearTasks()
{
    cout << "Tum gorevler temizlenecek. Emin misiniz? (Y/N): ";
    char confirmation;
    cin >> confirmation;
    if (confirmation != 'Y' && confirmation != 'y')
    {
        cout << "Islem iptal edildi.\n";
        return;
    }

    tasks.clear();
    saveTasks();
    cout << "Gorevler temizlendi.\n";
}

void getHelp() 
{
        cout << "Yardim - Islemler ve aciklamalari:\n"
            << "0: Programdan cikis yapar.\n"
            << "1: Yeni bir gorev ekler; gorev metnini girmeniz istenir.\n"
            << "2: Sectiginiz gorevi listeden siler.\n"
            << "3: Sectiginiz gorevin tamamlanma durumunu degistirir.\n"
            << "4: Tum gorevleri ve tamamlanma durumlarini listeler.\n"
            << "5: Gorev listesini dosyaya kaydeder.\n"
            << "6: Kayitli gorev listesini dosyadan yukler.\n"
            << "7: Listedeki tum gorevleri temizler.\n"
            << "8: Sectiginiz gorevin metnini duzenler.\n"
            << "9: Girilen anahtar kelimeyle eslesen gorevleri arar.\n"
            << "10: Bu yardim ekranini tekrar gosterir.\n";
}


void searchTask() 
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Önceki girişten kalan karakterleri temizle
    string searchText;
    cout << "Aranacak anahtar kelimeyi girin: ";
    getline(cin, searchText);

    if (searchText.empty())
    {
        cout << "Arama metni bos olamaz.\n";
        return;
    }

    bool found = false;
    for (size_t i = 0; i < tasks.size(); i++) 
    {
        if (tasks.at(i).text.find(searchText) != string::npos) 
        {
            cout 
                << "Bulunan gorev:\n"
                << i + 1 
                << ". ["
                << (tasks.at(i).completed ? "X" : " ") 
                << "] "
                << tasks.at(i).text 
                << "\n";
            found = true;
        }
    }

    if (!found) 
    {
        cout << "Aranan anahtar kelimeyle eslesen gorev bulunamadi.\n";
    }
}