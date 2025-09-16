#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

using namespace std;
const string ClientsFileName = "Clients.txt";

// The Starting OF The Project
void PauseFunction();
void ShowMainMenueScreen();
void ChoicesDirection();
void PauseTransactionsFunction();
struct sClientData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool FlagToDelete = false;
};
enum eChoices
{
    ShowingList = 1,
    AddingNewClient = 2,
    DeletingClient = 3,
    UpdatingClient = 4,
    FindingClient = 5,
    Transactions = 6,
    Exiting = 7
};

enum eTransactionsChoices {
    Deposit = 1,
    Withdraw = 2,
    TotalBalances = 3,
    MainMenue = 4,
};
string ReadData(string s) {
    cout << "\n";
    string sentence;
    cout << "Enter Your " << s << "? ";
    getline(cin >> ws, sentence);
    return sentence;
}
double ReadDouData(string s) {
    cout << "\n";
    double sentence;
    cout << "Enter Your " << s << "? ";
    cin >> sentence;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sentence;
}
    
string tabs(int number) {
    string str = "";
    for (int i = 0; i < number; i++)
    {
        str += "\t";
    }
    return str;
}
void HeaderOfScreens(string s) {
    system("cls");
    cout << "\n\n";
    cout << "*************************************************************************\n";
    cout << "\t" << s << "Screen" << endl;
    cout << "*************************************************************************\n";
}
vector<string> SplitString(string Sentence, string Delim) {
    int pos;
    vector<string> Vstring;
    string sWord;
    while ((pos = Sentence.find(Delim)) != std::string::npos) {
        sWord = Sentence.substr(0, pos);
        if (!sWord.empty())
        {
            Vstring.push_back(sWord);
        }

        Sentence.erase(0, pos + Delim.length());
    }
    if (Sentence != "") {
        Vstring.push_back(Sentence);
    }
    return Vstring;
}
string ConvertRecordToLine(sClientData& ClientData, string Delim) {
    string stClientRecord = "";
    stClientRecord += ClientData.AccountNumber + Delim;
    stClientRecord += ClientData.PinCode + Delim;
    stClientRecord += ClientData.Name + Delim;
    stClientRecord += ClientData.Phone + Delim;
    stClientRecord += to_string(ClientData.AccountBalance);
    return stClientRecord;
}
sClientData ConvertLinetoRecord(string stLine) {
    vector<string> vSplit = SplitString(stLine, "#//#");
    sClientData ClientData;
    ClientData.AccountNumber = vSplit[0];
    ClientData.PinCode = vSplit[1];
    ClientData.Name = vSplit[2];
    ClientData.Phone = vSplit[3];
    ClientData.AccountBalance = stod(vSplit[4]);
    return ClientData;
}
vector<sClientData> LoadCleintsDataFromFile(string FileName) { // vector store struct of client data
    vector<sClientData> vClientData;
    sClientData ClientData;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string line;
        while (getline(MyFile, line)) {
            ClientData = ConvertLinetoRecord(line);
            vClientData.push_back(ClientData);
        }
        MyFile.close();
    }

    return vClientData;
}

void PrintClientAsRow(sClientData& Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintClientAsRowTotalBalancesTable(sClientData& Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData() {
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    system("cls");
    cout << tabs(20) << "Client List (" << vClientData.size() << ") Client(s).\n" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n\n--------------------------------------------------------------------------------------------------" << endl;
    for (sClientData Client : vClientData) {
        PrintClientAsRow(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    PauseFunction();
}

void AddClientToFile(string FileName, sClientData& ClientData) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << ConvertRecordToLine(ClientData, "#//#") << endl;
    }
    MyFile.close();
}
bool AccountNumberExists(string accountNumber,
    const vector<sClientData>& vClientData,
    sClientData* ClientData = nullptr)
{
    for (const sClientData& Client : vClientData) {
        if (accountNumber == Client.AccountNumber) {
            if (ClientData != nullptr) {
                *ClientData = Client;
            }
            return true;
        }
    }
    return false;
}

sClientData ReadNewClientData(string accountNumber = "") { // using default parameter
    sClientData ClientData;
    if (accountNumber == "") {
        ClientData.AccountNumber = ReadData("Account Number");
    }
    else {
        ClientData.AccountNumber = accountNumber;
    }
    ClientData.PinCode = ReadData("Pin Code");
    ClientData.Name = ReadData("Name");
    ClientData.Phone = ReadData("Phone");
    ClientData.AccountBalance = ReadDouData("AccountBalance");
    return ClientData;
}

void AddingNewClientFun() {
    HeaderOfScreens("Add New Clients ");
    cout << "Adding New Client:\n";
    string accountNumber;
    char AddMore;
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    do {
        cout << "Enter Account Number? ";
        getline(cin >> ws, accountNumber);
        while (AccountNumberExists(accountNumber, vClientData))
        {
            cout << "Client with [" << accountNumber << "] already exists, Enter Another Account Number? ";
            getline(cin >> ws, accountNumber);
        }
        sClientData ClientData = ReadNewClientData(accountNumber);
        vClientData.push_back(ClientData);
        AddClientToFile(ClientsFileName, ClientData);
        cout << "Client AddedSuccessfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
    PauseFunction();
}
void MakeFlagToDelete(string AccountNumber, vector<sClientData>& vClientData) {
    for (sClientData& Client : vClientData)
    {
        if (Client.AccountNumber == AccountNumber) {
            Client.FlagToDelete = true;
        }
    }
}
void SaveClientToFile(string FileName, vector<sClientData>& vClientData) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (sClientData& Client : vClientData)
        {
            if (!Client.FlagToDelete)
            {
                string reAddClient = ConvertRecordToLine(Client, "#//#");
                MyFile << reAddClient << endl;
            }

        }
        MyFile.close();
        vClientData = LoadCleintsDataFromFile(FileName);
    }
}

void DeleteClientByAccountNumber(string AccountNumber, vector<sClientData>& vClientData) {
    char ToDelete = 'y';
    cout << "\n\n" << "Are You Sure you want to delete this Client? y/n ? ";
    cin >> ToDelete;
    if (toupper(ToDelete) == 'Y')
    {
        MakeFlagToDelete(AccountNumber, vClientData);
        SaveClientToFile(ClientsFileName, vClientData);
        cout << "\nClient Deleted Successfully";
    }

}
void PrintClientDetails(sClientData &ClientData) {
    cout << "\n";
    cout << "\n\nThe following are the client details\n";
    cout << "-------------------------------------------------\n";
    cout << "\nAccout Number: " << ClientData.AccountNumber;
    cout << "\nPin Code     : " << ClientData.PinCode;  
    cout << "\nName         : " << ClientData.Name;
    cout << "\nPhone        : " << ClientData.Phone;
    cout << "\nAccount Balance: " << ClientData.AccountBalance;
    cout << "\n-------------------------------------------------\n";
}
void DeletingClientFun() {
    HeaderOfScreens("Delete Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    cin >> accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!AccountNumberExists(accountNumber, vClientData , &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    DeleteClientByAccountNumber(ClientData.AccountNumber, vClientData);
    PauseFunction();
}
void UpdateClientData(sClientData &ClientData) {
    
    ClientData.PinCode = ReadData("Pin Code");
    ClientData.Name = ReadData("Name");
    ClientData.Phone = ReadData("Phone");
    ClientData.AccountBalance = ReadDouData("AccountBalance");
  
}

void UpdateClientByAccountNumber(string AccountNumber, vector<sClientData>& vClientData) {
    char Update = 'y';
    cout << "\nAre you want Update this client? y/n ? ";
    cin >> Update;
    if (toupper(Update) == 'Y')
    {
        for (sClientData& Client : vClientData)
        {
            if (AccountNumber == Client.AccountNumber) {
                UpdateClientData(Client);
                break;
            }

        }
        SaveClientToFile(ClientsFileName, vClientData);
        cout << "\nThe Client data is Updated";
    }

}
void UpdatingClientFun() {
    HeaderOfScreens("Update Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    cin >> accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    UpdateClientByAccountNumber(ClientData.AccountNumber, vClientData);
    PauseFunction();
}
void FindingClientFun() {
    HeaderOfScreens("Find Client ");
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    sClientData ClientData;
    cout << "Please enter Account Number? ";
    getline(cin >> ws, accountNumber);
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists, Enter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    PauseFunction();
}
void DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClientData>& vClients) {
    char Answer;
    cout << "Are you Sure you want to preforme this transaction? Y/N? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        for (sClientData& Client : vClients) {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.AccountBalance += Amount;
                cout << "\nNew Balance = " << Client.AccountBalance << endl;
                break;
            }
        }
    }
    SaveClientToFile(ClientsFileName, vClients);
    PauseTransactionsFunction();
}
void DepositFun() {
    HeaderOfScreens("Deposit ");

    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    double AddedValue;
    sClientData ClientData;
    cout << "\n\nPlease enter Account Number? ";
    cin >> accountNumber;
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists,\nEnter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    cout << "Enter the deposite amount: ";
    cin >> AddedValue;
    DepositBalanceToClientByAccountNumber(accountNumber, AddedValue, vClientData);
}
void WithdrawFun() {
    HeaderOfScreens("Withdraw ");
    
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    string accountNumber;
    double WithdrawAmount;
    sClientData ClientData;
    cout << "\n\nPlease enter Account Number? ";
    cin >> accountNumber;
    while (!AccountNumberExists(accountNumber, vClientData, &ClientData))
    {
        cout << "Client with [" << accountNumber << "] does not exists,\nEnter Another Account Number? ";
        getline(cin >> ws, accountNumber);
    }
    PrintClientDetails(ClientData);
    cout << "Enter the Withdraw amount: ";
    cin >> WithdrawAmount;
    while (WithdrawAmount > ClientData.AccountBalance) { 
       cout << "\nAmount Exceeds the balance, you can withdraw up to : " << ClientData.AccountBalance << endl;       
       cout << "Please enter another amount? ";        
       cin >> WithdrawAmount; 
    }   
       
    DepositBalanceToClientByAccountNumber(accountNumber, (-1 * WithdrawAmount) , vClientData);
}
void TotalBalancesFun() {
    double TotalBalances = 0;
    vector<sClientData> vClientData = LoadCleintsDataFromFile(ClientsFileName);
    system("cls");
    cout << tabs(20) << "Client List (" << vClientData.size() << ") Client(s).\n" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n\n--------------------------------------------------------------------------------------------------" << endl;
    for (sClientData Client : vClientData) {
        TotalBalances += Client.AccountBalance;
        PrintClientAsRowTotalBalancesTable(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << tabs(24) << "Total Balances = " << TotalBalances << endl;
    PauseTransactionsFunction();
}
void ChoicesTransactionsDirection() {

    int Choice;
    cout << "Choose what do You want to do? [1 to 4]?";
    cin >> Choice;
    switch (Choice)
    {
    case eTransactionsChoices::Deposit:
        DepositFun();
        break;
    case eTransactionsChoices::Withdraw:
        WithdrawFun();
        break;
    case eTransactionsChoices::TotalBalances:
        TotalBalancesFun();
        break;
    case eTransactionsChoices::MainMenue:
        ShowMainMenueScreen();
        break;

    default:
        cout << "\nWrong Choice, Choose Again: \n";
        ChoicesTransactionsDirection();
        break;
    }
}
void ShowTransactionsMenuSecreen() {

    system("cls");
    HeaderOfScreens("Transactios ");
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "*************************************************************************\n";
    ChoicesTransactionsDirection();
}
void ChoicesDirection(){
    int Choice;
    cout << "Choose what do You want to do? [1 to 6]?";
    cin >> Choice;
    switch (Choice)
    {
    case eChoices::ShowingList:
        PrintAllClientsData();
        break;
    case eChoices::AddingNewClient:
        AddingNewClientFun();
        break;
    case eChoices::DeletingClient:
        DeletingClientFun();
        break;
    case eChoices::UpdatingClient:
        UpdatingClientFun();
        break;
    case eChoices::FindingClient:
        FindingClientFun();
        break;
    case eChoices::Transactions:
        ShowTransactionsMenuSecreen();
        break;

    case eChoices::Exiting:
        HeaderOfScreens("End Of Program ");
        break;
    
    default:
        cout << "\nWrong Choice, Choose Again: \n";
        ChoicesDirection();
        break;
    }
}
void ShowMainMenueScreen() {

    system("cls");
    cout << "*************************************************************************\n";
    cout << tabs(2) << "Main Menue Screen\n";
    cout << "*************************************************************************\n";
    cout << "\t[1] Show Client list.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactios.\n";
    cout << "\t[7] Exit.\n";
    cout << "*************************************************************************\n";
    ChoicesDirection();
}
void PauseFunction() {
    cout << "\n\nPress any key to go back to Main Menue, ";    
    system("pause");
    ShowMainMenueScreen();
}
void PauseTransactionsFunction() {
    cout << "\n\nPress any key to go back to Transactions Menue,"; 
    system("pause");
    ShowTransactionsMenuSecreen();
}
int main()
{   
    ShowMainMenueScreen();
    system("pause");
    return 0;
}
