#define OPEN_MODE ios::out | ios::binary | ios::in
#define LONG_TEXT 100
#define SHORT_TEXT 50
#define MASTER_FILEPATH "Dealers.dat"
#define SLAVE_FILEPATH "SportEvent.dat"
#define OUTPUT_SPORTEVENT_FILEPATH "SportEvent.txt"
#define OUTPUT_MEMBER_FILEPATH "outputDealers.txt"
#define TEMP_FILEPATH "temp.dat"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//slave
struct SportEvent
{
    char _isDeleted;
    char Model[SHORT_TEXT];
    int SportEventID;
    int HP;
    int MaxSpeed;
    int Weigth;
    int NumberOfNextSportEventInFile;
};

//master
struct Member
{
    char _isDeleted;
    char Name[SHORT_TEXT];
    char Adress[LONG_TEXT];
    int MemberID;
    int FirstSportEventNumberInFile;
};

SportEvent SportEventReturn;
Member MemberReturn;

int NumberOfElementsInFile(bool _isSlave)
{
    ifstream readfile((_isSlave ? SLAVE_FILEPATH : MASTER_FILEPATH), OPEN_MODE);

    readfile.seekg(0, readfile.end);
    size_t rcount = readfile.tellg() / (_isSlave ? sizeof(SportEvent) : sizeof(Member));

    readfile.close();
    if (!readfile.good())
        cout << "Error NumberOfElementsInFile";

    return rcount;
}

int NumberOfSportEventsInMember(int MemberID)
{
    int NumberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    Member tempMember;
    SportEvent tempSportEvent;
    bool checker = false;
    int counter = 0;
    int currentSportEventNumber = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.MemberID == MemberID)
        {
            currentSportEventNumber = tempMember.FirstSportEventNumberInFile;
            checker = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error NumberOfSportEventsInMember M1";

    if (checker)
    {
        ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

        while (currentSportEventNumber != -1)
        {
            ReadSlave1.seekg(currentSportEventNumber * sizeof(SportEvent), ReadSlave1.beg);
            ReadSlave1.read((char*)&tempSportEvent, sizeof(SportEvent));

            currentSportEventNumber = tempSportEvent.NumberOfNextSportEventInFile;
            counter++;
        }

        ReadSlave1.close();
        if (!ReadSlave1.good())
            cout << "Error NumberOfSportEventsInMember S1";
    }
    else
        cout << "Error NumberOfSportEventsInMember. No Member found";

    return counter;
}

void GetAllMembersInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_MEMBER_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllMembersInfo O1";

    string Res;
    int i = 0;
    int NumberOfMembers = NumberOfElementsInFile(false);
    Member tempMember;

    ofstream OutputFile2(OUTPUT_MEMBER_FILEPATH);

    for (; i < NumberOfMembers; i++)
    {
        ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));

        Res += "Name: " + string(tempMember.Name);
        Res += "\nAdress: " + string(tempMember.Adress);
        Res += "\nMemberID: " + to_string(tempMember.MemberID);
        Res += "\nFirstSportEventNumberInFile: " + to_string(tempMember.FirstSportEventNumberInFile);
        Res += "\n\n";

        ReadMaster1.close();
        if (!ReadMaster1.good())
            cout << "Error GetAllMembersInfo M1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllMembersInfo O2";
}

void GetAllSportEventsInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_SPORTEVENT_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllSportEventsInfo O1";

    string Res;
    int i = 0;
    int NumberOfSportEvents = NumberOfElementsInFile(true);
    SportEvent tempSportEvent;

    ofstream OutputFile2(OUTPUT_SPORTEVENT_FILEPATH);

    for (; i < NumberOfSportEvents; i++)
    {
        ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

        ReadSlave1.seekg(i * sizeof(SportEvent), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempSportEvent, sizeof(SportEvent));

        Res += "Model: " + string(tempSportEvent.Model);
        Res += "\nSportEvent ID: " + to_string(tempSportEvent.SportEventID);
        Res += "\nHP: " + to_string(tempSportEvent.HP);
        Res += "\nMaxSpeed: " + to_string(tempSportEvent.MaxSpeed);
        Res += "\nWeigth: " + to_string(tempSportEvent.Weigth);
        Res += "\nNumberOfNextSportEventInFile: " + to_string(tempSportEvent.NumberOfNextSportEventInFile);
        Res += "\n\n";
        ReadSlave1.close();
        if (!ReadSlave1.good())
            cout << "Error GetAllSportEventsInfo S1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllSportEventsInfo O2";
}

SportEvent GetSportEvent(int SportEventID)
{
    int NumerOfSportEvents = NumberOfElementsInFile(true);
    int i = 0;
    SportEvent tempSportEvent = {};

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumerOfSportEvents; i++)
    {
        ReadSlave1.seekg(i * sizeof(SportEvent), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempSportEvent, sizeof(SportEvent));
        if (tempSportEvent.SportEventID == SportEventID)
            break;
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error GetSportEvent";

    return tempSportEvent;
}

Member GetMember(int MemberID)
{
    int NumberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    Member tempMember = {};

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.MemberID == MemberID)
            break;
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error GetMember";

    return tempMember;
}

void UpdateSportEvent(int SportEventID)
{
    int NumberOfSportEvents = NumberOfElementsInFile(true);
    int i = 0;
    SportEvent tempSportEvent;
    bool _isSportEventExists = false;

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumberOfSportEvents; i++)
    {
        ReadSlave1.seekg(i * sizeof(SportEvent), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempSportEvent, sizeof(SportEvent));
        if (tempSportEvent.SportEventID == SportEventID)
        {
            _isSportEventExists = true;
            break;
        }
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error UpdateSportEvent S1";

    if (_isSportEventExists)
    {
        cout << "Enter Value to change: 0 - SportEventID 1 - Model 2 - HP 3 - MaxSpeed 4 - Weigth\n";
        int n;
        cin >> n;
        cout << "Enter new value:\n";
        switch (n)
        {
        case 0:
            cin >> tempSportEvent.SportEventID;
            break;
        case 1:
            cin >> tempSportEvent.Model;
            break;
        case 2:
            cin >> tempSportEvent.HP;
            break;
        case 3:
            cin >> tempSportEvent.MaxSpeed;
            break;
        case 4:
            cin >> tempSportEvent.Weigth;
            break;
        default:
            cout << "Error UpdateSportEvent not in range 0-4";
            break;
        }

        ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);
        WriteSlave2.seekp(i * sizeof(SportEvent), WriteSlave2.beg);
        WriteSlave2.write((char*)&tempSportEvent, sizeof(SportEvent));

        WriteSlave2.close();
        if (!WriteSlave2.good())
            cout << "Error UpdateSportEvent S2";
    }
    else
        cout << "Error UpdateSportEvent No SportEvent\n";
}

void UpdateMember(int MemberID)
{
    int NumberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    Member tempMember;
    bool _isMemberExists = false;
    int TaskNumber;
    int temp;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.MemberID == MemberID)
        {
            _isMemberExists = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error UpdateMember M1";

    if (_isMemberExists)
    {
        cout << "Enter Value to change: 0 - ID 1 - Name 2 - Adress\n";

        cin >> TaskNumber;
        cout << "Enter new value:\n";
        switch (TaskNumber)
        {
        case 0:
            cin >> temp;
            tempMember.MemberID = temp;
            break;
        case 1:
            cin >> tempMember.Name;
            break;
        case 2:
            cin >> tempMember.Adress;
            break;
        default:
            cout << "Error UpdateMember Not in range 0-2";
            break;
        }

        ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);
        WriteMaster2.seekp(i * sizeof(Member), WriteMaster2.beg);
        WriteMaster2.write((char*)&tempMember, sizeof(Member));

        WriteMaster2.close();
        if (!WriteMaster2.good())
            cout << "Error UpdateMember M2";
    }
    else {
        cout << "Error UpdateMember Member doesnt exists\n";
    }
}

void AddSportEvent(int MemberID, SportEvent NewSportEvent)
{
    int NumberOfMembers = NumberOfElementsInFile(false);
    int NumberOfSportEvents = NumberOfElementsInFile(true);
    bool checker = false;
    int i = 0;
    Member tempMember;
    SportEvent tempSportEvent;
    int currentSportEventNumber = -1;
    int previousSportEventNumber = -1;




    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.MemberID == MemberID)
        {
            checker = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddSportEvent M1";

    if (checker)
    {
        currentSportEventNumber = tempMember.FirstSportEventNumberInFile;
        checker = (currentSportEventNumber == -1);

        if (checker)
        {
            tempMember.FirstSportEventNumberInFile = NumberOfSportEvents;

            ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);

            WriteSlave2.seekp(NumberOfSportEvents * sizeof(SportEvent), WriteSlave2.beg);
            WriteSlave2.write((char*)&NewSportEvent, sizeof(SportEvent));

            WriteSlave2.close();
            if (!WriteSlave2.good())
                cout << "Error AddSportEvent S2";

            ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);

            WriteMaster2.seekp(i * sizeof(Member), WriteMaster2.beg);
            WriteMaster2.write((char*)&tempMember, sizeof(Member));

            WriteMaster2.close();
            if (!WriteMaster2.good())
                cout << "Error AddSportEvent M2";
        }
        else
        {
            ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

            while (currentSportEventNumber != -1)
            {
                ReadSlave1.seekg(currentSportEventNumber * sizeof(SportEvent), ReadSlave1.beg);
                ReadSlave1.read((char*)&tempSportEvent, sizeof(SportEvent));

                previousSportEventNumber = currentSportEventNumber;
                currentSportEventNumber = tempSportEvent.NumberOfNextSportEventInFile;

            }

            ReadSlave1.close();
            if (!ReadSlave1.good())
                cout << "Error Addv S1";

            tempSportEvent.NumberOfNextSportEventInFile = NumberOfSportEvents;

            ofstream WriteSlave3(SLAVE_FILEPATH, OPEN_MODE);
            WriteSlave3.seekp(previousSportEventNumber * sizeof(SportEvent), WriteSlave3.beg);
            WriteSlave3.write((char*)&tempSportEvent, sizeof(SportEvent));

            WriteSlave3.seekp(NumberOfSportEvents * sizeof(SportEvent), WriteSlave3.beg);
            WriteSlave3.write((char*)&NewSportEvent, sizeof(SportEvent));

            WriteSlave3.close();
            if (!WriteSlave3.good())
                cout << "Error AddSportEvent S3";
        }
    }
}

void AddMember(Member NewMember)
{
    ofstream WriteMaster1(MASTER_FILEPATH, OPEN_MODE);

    WriteMaster1.seekp(0, WriteMaster1.end);
    WriteMaster1.write((char*)&NewMember, sizeof(Member));

    WriteMaster1.close();
    if (!WriteMaster1.good())
        cout << "Error AddMember M1";
}

void makeFiles(bool _removeOldFiles) {
    if (_removeOldFiles)
    {
        remove(MASTER_FILEPATH);
        remove(SLAVE_FILEPATH);
        remove(OUTPUT_SPORTEVENT_FILEPATH);
        remove(OUTPUT_MEMBER_FILEPATH);
    }

    ifstream master(MASTER_FILEPATH, OPEN_MODE);
    bool isopen = master.is_open();
    master.close();

    if (!isopen) {
        ofstream master(MASTER_FILEPATH);
        master.close();
    }

    ifstream slave(SLAVE_FILEPATH, OPEN_MODE);
    isopen = slave.is_open();
    slave.close();

    if (!isopen) {
        ofstream slave(SLAVE_FILEPATH);
        slave.close();
    }

    ifstream outputSportEvents(OUTPUT_SPORTEVENT_FILEPATH, OPEN_MODE);
    isopen = outputSportEvents.is_open();
    outputSportEvents.close();

    if (!isopen) {
        ofstream outputSportEvents(OUTPUT_SPORTEVENT_FILEPATH);
        outputSportEvents.close();
    }

    ifstream outputMembers(OUTPUT_MEMBER_FILEPATH, OPEN_MODE);
    isopen = outputMembers.is_open();
    outputMembers.close();

    if (!isopen) {
        ofstream outputMembers(OUTPUT_MEMBER_FILEPATH);
        outputMembers.close();
    }

}

SportEvent CreateSportEvent()
{


    SportEvent tempSportEvent;

    cout << "Enter SportEvent model:\n";
    cin >> tempSportEvent.Model;
    cout << "Enter SportEvent id:\n";
    cin >> tempSportEvent.SportEventID;
    cout << "Enter SportEvent HP:\n";
    cin >> tempSportEvent.HP;
    cout << "Enter SportEvent MaxSpeed:\n";
    cin >> tempSportEvent.MaxSpeed;
    cout << "Enter SportEvent Weigth:\n";
    cin >> tempSportEvent.Weigth;
    tempSportEvent.NumberOfNextSportEventInFile = -1;

    return tempSportEvent;
}

Member CreateMember()
{
    Member tempMember;

    cout << "Enter Member Name:\n";
    cin >> tempMember.Name;
    cout << "Enter Member Adress:\n";
    cin >> tempMember.Adress;
    cout << "Enter Member ID:\n";
    cin >> tempMember.MemberID;
    cout << "\n\n";
    tempMember.FirstSportEventNumberInFile = -1;

    return tempMember;
}

void DeleteSportEvent(int SportEventID)
{
    int numberOfSportEvents = NumberOfElementsInFile(true);
    int numberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    SportEvent tempSportEvent;
    SportEvent SportEvent, prevSportEvent;
    Member MemberOfSportEventToDel;
    bool SportEventExists = false;
    bool isSportEventFoundInMembers = false;
    bool prevSportEventExists = false;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < numberOfSportEvents; i++)
    {
        rs1.seekg(i * sizeof(SportEvent), rs1.beg);
        rs1.read((char*)&SportEvent, sizeof(SportEvent));

        if (SportEvent.SportEventID == SportEventID) {
            SportEventExists = true;
            break;
        }
    }

    if (!SportEventExists) {
        cout << "Error SportEvent doesn't exist\n";
        return;
    }

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfMembers; j++)
    {
        rm1.seekg(j * sizeof(Member), rm1.beg);
        rm1.read((char*)&MemberOfSportEventToDel, sizeof(Member));

        if (MemberOfSportEventToDel.FirstSportEventNumberInFile == -1)
            continue;

        if (MemberOfSportEventToDel.FirstSportEventNumberInFile == i) {
            MemberOfSportEventToDel.FirstSportEventNumberInFile = (SportEvent.NumberOfNextSportEventInFile == -1)
                ? -1 : SportEvent.NumberOfNextSportEventInFile;

            ofstream wm(MASTER_FILEPATH, OPEN_MODE);

            wm.seekp(j * sizeof(Member), wm.beg);
            wm.write((char*)&MemberOfSportEventToDel, sizeof(Member));

            wm.close();
            if (!wm.good()) {
                cout << "Error Edit Master";
                return;
            }

            isSportEventFoundInMembers = true;
            break;
        }


        rs1.seekg(MemberOfSportEventToDel.FirstSportEventNumberInFile * sizeof(SportEvent), rs1.beg);
        rs1.read((char*)&prevSportEvent, sizeof(SportEvent));

        if (prevSportEvent.NumberOfNextSportEventInFile == -1)
            continue;

        do {
            rs1.seekg(prevSportEvent.NumberOfNextSportEventInFile * sizeof(SportEvent), rs1.beg);
            rs1.read((char*)&tempSportEvent, sizeof(SportEvent));

            if (prevSportEvent.NumberOfNextSportEventInFile != i) {
                prevSportEvent = tempSportEvent;
                continue;
            }

            prevSportEvent.NumberOfNextSportEventInFile = tempSportEvent.NumberOfNextSportEventInFile;

            prevSportEventExists = true;
            isSportEventFoundInMembers = true;
            break;
        } while (tempSportEvent.NumberOfNextSportEventInFile != -1);

        if (isSportEventFoundInMembers)
            break;
    }

    if (!isSportEventFoundInMembers) {
        cout << "Error isSportEventFoundInMembers=false";
        return;
    }

    rs1.close();
    if (!rs1.good()) {
        cout << "Error DeleteMember M1";
        return;
    }

    rm1.close();
    if (!rm1.good()) {
        cout << "Error rm.close()";
        return;
    }


    // rewriting indexes in SLAVE

    ifstream tempSlave(TEMP_FILEPATH, OPEN_MODE);
    bool isopenSlave = tempSlave.is_open();
    tempSlave.close();

    if (!isopenSlave) {
        ofstream tempFile(TEMP_FILEPATH);
        tempFile.close();
    }

    ifstream rs2(SLAVE_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfSportEvents; j++)
    {
        if (j == i) {
            continue;
        }

        rs2.seekg(j * sizeof(SportEvent), rs2.beg);
        rs2.read((char*)&tempSportEvent, sizeof(SportEvent));

        if (prevSportEventExists && (prevSportEvent.SportEventID == tempSportEvent.SportEventID))
            tempSportEvent.NumberOfNextSportEventInFile = prevSportEvent.NumberOfNextSportEventInFile;

        int num = tempSportEvent.NumberOfNextSportEventInFile;
        tempSportEvent.NumberOfNextSportEventInFile = (num > i) ? (num - 1) : num;
        int newNumberInFile = (j > i) ? (j - 1) : j;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(newNumberInFile * sizeof(SportEvent), wtemp.beg);
        wtemp.write((char*)&tempSportEvent, sizeof(SportEvent));

        wtemp.close();
        if (!wtemp.good()) {
            cout << "Error DeleteMember T1";
            return;
        }
    }

    rs2.close();
    if (!rs2.good()) {
        cout << "Error DeleteMember M2";
        return;
    }

    remove(SLAVE_FILEPATH);
    rename(TEMP_FILEPATH, SLAVE_FILEPATH);


    // rewriting indexes in MASTER

    ifstream tempMaster(TEMP_FILEPATH, OPEN_MODE);
    bool isopenMaster = tempMaster.is_open();
    tempMaster.close();

    if (!isopenMaster) {
        ofstream tempMaster(TEMP_FILEPATH);
        tempMaster.close();
    }

    Member tempMember;
    ifstream rm2(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfMembers; j++)
    {
        rm2.seekg(j * sizeof(Member), rm2.beg);
        rm2.read((char*)&tempMember, sizeof(Member));

        int num = tempMember.FirstSportEventNumberInFile;
        tempMember.FirstSportEventNumberInFile = (num > i) ? (num - 1) : num;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(j * sizeof(Member), wtemp.beg);
        wtemp.write((char*)&tempMember, sizeof(Member));

        wtemp.close();
        if (!wtemp.good()) {
            cout << "Error";
            return;
        }
    }

    rm2.close();
    if (!rm2.good()) {
        cout << "Error DeleteMember M2";
        return;
    }

    remove(MASTER_FILEPATH);
    rename(TEMP_FILEPATH, MASTER_FILEPATH);
}

void DeleteSportEventsFromMember(int MemberID)
{
    int numberOfSportEvents = NumberOfElementsInFile(true);
    int numberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    Member Member;
    bool MemberExists = false;


    // check if the Member exists

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < numberOfSportEvents; i++)
    {
        rm1.seekg(i * sizeof(Member), rm1.beg);
        rm1.read((char*)&Member, sizeof(Member));

        if (Member.MemberID == MemberID) {
            MemberExists = true;
            break;
        }
    }

    if (!MemberExists) {
        cout << "Error Member doesn't exist\n";
        return;
    }

    rm1.close();
    if (!rm1.good()) {
        cout << "Error DeleteSportEventsFromMember rm.close()";
        return;
    }


    // find the IDs of the Member's SportEvents

    if (Member.FirstSportEventNumberInFile == -1) {
        cout << "Member has no SportEvents!";
        return;
    }

    SportEvent tempSportEvent;
    vector<int> SportEventIDs;
    int indx = Member.FirstSportEventNumberInFile;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    while (true) {
        rs1.seekg(indx * sizeof(SportEvent), rs1.beg);
        rs1.read((char*)&tempSportEvent, sizeof(SportEvent));

        SportEventIDs.push_back(tempSportEvent.SportEventID);

        if (tempSportEvent.NumberOfNextSportEventInFile == -1)
            break;

        indx = tempSportEvent.NumberOfNextSportEventInFile;
    }

    rs1.close();
    if (!rs1.good()) {
        cout << "Error DeleteSportEventsFromMember rs1.good()";
        return;
    }


    // delete all SportEvents of the Member

    for (int& id : SportEventIDs)
        DeleteSportEvent(id);
}

void DeleteMember(int MemberID)
{
    int NumberOfMembers = NumberOfElementsInFile(false);
    int i = 0;
    Member tempMember;
    bool _isMemberExists = false;
    int counter = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.MemberID == MemberID)
        {
            _isMemberExists = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error DeleteMember M1";

    if (_isMemberExists)
    {
        if (tempMember.FirstSportEventNumberInFile != -1)
        {
            DeleteSportEventsFromMember(tempMember.MemberID);
        }

        ifstream tempFile(TEMP_FILEPATH, OPEN_MODE);
        bool isopen = tempFile.is_open();
        tempFile.close();

        if (!isopen) {
            ofstream tempFile(TEMP_FILEPATH);
            tempFile.close();
        }

        for (int j = 0; j < NumberOfMembers; j++)
        {
            if (j == i)
                continue;

            ifstream ReadMaster2(MASTER_FILEPATH, OPEN_MODE);

            ReadMaster2.seekg(j * sizeof(Member), ReadMaster2.beg);
            ReadMaster2.read((char*)&tempMember, sizeof(Member));

            ReadMaster2.close();
            if (!ReadMaster2.good())
                cout << "Error DeleteMember M2";

            ofstream WriteTemp1(TEMP_FILEPATH, OPEN_MODE);

            WriteTemp1.seekp(counter++ * sizeof(Member), WriteTemp1.beg);
            WriteTemp1.write((char*)&tempMember, sizeof(Member));

            WriteTemp1.close();
            if (!WriteTemp1.good())
                cout << "Error DeleteMember T1";
        }

        remove(MASTER_FILEPATH);
        rename(TEMP_FILEPATH, MASTER_FILEPATH);

    }
    else {
        cout << "Error UpdateMember Member doesnt exists\n";
    }
}

void printAllMembers()
{
    cout << "\nList of delaers\nName - id\n";
    int NumberOfMembers = NumberOfElementsInFile(false);
    Member tempMember;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        cout << tempMember.Name << " - " << tempMember.MemberID << "\n";
    }
    cout << "\n";

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddSportEvent M1";
}

void printAllSportEvents()
{
    cout << "\nList of SportEvents\nModel - id\n";
    int NumberOfMembers = NumberOfElementsInFile(true);
    SportEvent tempSportEvent;

    ifstream ReadMaster1(SLAVE_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfMembers; i++)
    {
        ReadMaster1.seekg(i * sizeof(SportEvent), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempSportEvent, sizeof(SportEvent));
        cout << tempSportEvent.Model << " - " << tempSportEvent.SportEventID << "\n";
    }
    cout << "\n";

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddSportEvent M1";
}

void Menu()
{
    int taskNumber = 0;
    int SportEventId;
    int MemberId;
    while (true)
    {
        cout <<
            "Exit - 0\n" <<
            "Get SportEvent - 1\n" <<
            "Get Member - 2\n" <<
            "Delete SportEvent - 3\n" <<
            "Delete Member - 4\n" <<
            "Update SportEvent - 5\n" <<
            "Update Member - 6\n" <<
            "Add SportEvent - 7\n" <<
            "Add Member - 8\n" <<
            "Calculate all Members - 9\n" <<
            "Calculate SportEvents in Member - 10\n" <<
            "Calculate all SportEvents - 11\n" <<
            "Print Members info - 12\n" <<
            "Print SportEvents info - 13\n";
        cin >> taskNumber;

        switch (taskNumber)
        {
        case 0:
            exit(0);
            break;
        case 1:
            printAllSportEvents();
            cout << "Enter SportEvent id\n";
            cin >> SportEventId;
                SportEventReturn = GetSportEvent(SportEventId);
            break;
        case 2:
            printAllMembers();
            cout << "Enter Member id\n";
            cin >> MemberId;
                MemberReturn = GetMember(MemberId);
            break;
        case 3:
            printAllSportEvents();
            cout << "Enter SportEvent id\n";
            cin >> SportEventId;
            DeleteSportEvent(SportEventId);
            break;
        case 4:
            printAllMembers();
            cout << "Enter Member id\n";
            cin >> MemberId;
            DeleteMember(MemberId);
            break;
        case 5:
            printAllSportEvents();
            cout << "Enter SportEvent id\n";
            cin >> SportEventId;
            UpdateSportEvent(SportEventId);
            break;
        case 6:
            printAllMembers();
            cout << "Enter Member id\n";
            cin >> MemberId;
            UpdateMember(MemberId);
            break;
        case 7:
            printAllSportEvents();
            printAllMembers();
            cout << "Enter Member id\n";
            cin >> MemberId;
            AddSportEvent(MemberId, CreateSportEvent());
            break;
        case 8:
            printAllMembers();
            AddMember(CreateMember());
            break;
        case 9:
            cout << NumberOfElementsInFile(false) << "\n";
            break;
        case 10:
            cout << "Enter Member id\n";
            cin >> MemberId;
            cout << NumberOfSportEventsInMember(MemberId) << "\n";
            break;
        case 11:
            cout << NumberOfElementsInFile(true) << "\n";
            break;
        case 12:
            GetAllMembersInfo();
            cout << "Check " << OUTPUT_MEMBER_FILEPATH << " file\n";
            break;
        case 13:
            GetAllSportEventsInfo();
            cout << "Check " << OUTPUT_SPORTEVENT_FILEPATH << " file\n";
            break;
        default:
            cout << "Error executeTasks not in range 1-13\n";
            break;
        }
    }
}

//void test1()
////{
////    makeFiles(true);
////    const int SIZE = 5;
////    Member old_Members[SIZE] = {
////        {
////            {'B', 'M', 'W'},
////            {'K', 'y', 'i', 'v'},
////            1,
////            -1
////        },
////        {
////            {'B', 'e', 'n', 'z'},
////            {'L', 'v', 'i', 'v'},
////            2,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
////            3,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'K', 'a', 'n', 'i', 'v'},
////            4,
////            -1
////        },
////        {
////            {'B', 'M', 'W'},
////            {'L', 'v', 'i', 'v'},
////            5,
////            -1
////        },
////    };
////
////    for (int i = 0; i < SIZE; i++)
////    {
////        AddMember(old_Members[i]);
////    }
////
////    SportEvent M3 =
////    {
////        {'M', '3'},
////        1,
////        345,
////        300,
////        1500,
////        -1
////    };
////
////    SportEvent M2 =
////    {
////        {'M', '2'},
////        2,
////        300,
////        230,
////        1300,
////        -1
////    };
////
////    SportEvent G63 =
////    {
////        {'A', 'M', 'G'},
////        3,
////        200,
////        210,
////        3000,
////        -1
////    };
////
////    SportEvent A4 =
////    {
////        {'A', '4'},
////        4,
////        310,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent etron =
////    {
////        {'E', '-', 't', 'r', 'o', 'n'},
////        5,
////        260,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent Rs7 =
////    {
////        {'R', 's', '-', '7'},
////        6,
////        380,
////        210,
////        2600,
////        -1
////    };
////
////    AddSportEvent(1, M3);
////    AddSportEvent(1, M2);
////    AddSportEvent(2, G63);
////    AddSportEvent(4, A4);
////    AddSportEvent(4, etron);
////    AddSportEvent(4, Rs7);
////
////    GetAllSportEventsInfo();
////    GetAllMembersInfo();
////}
////
////void test2()
////{
////    makeFiles(true);
////    const int SIZE = 5;
////    Member old_Members[SIZE] = {
////        {
////            {'B', 'M', 'W'},
////            {'K', 'y', 'i', 'v'},
////            1,
////            -1
////        },
////        {
////            {'B', 'e', 'n', 'z'},
////            {'L', 'v', 'i', 'v'},
////            2,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
////            3,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'K', 'a', 'n', 'i', 'v'},
////            4,
////            -1
////        },
////        {
////            {'B', 'M', 'W'},
////            {'L', 'v', 'i', 'v'},
////            5,
////            -1
////        },
////    };
////
////    for (int i = 0; i < SIZE; i++)
////    {
////        AddMember(old_Members[i]);
////    }
////
////    SportEvent M3 =
////    {
////        {'M', '3'},
////        1,
////        345,
////        300,
////        1500,
////        -1
////    };
////
////    SportEvent M2 =
////    {
////        {'M', '2'},
////        2,
////        300,
////        230,
////        1300,
////        -1
////    };
////
////    SportEvent G63 =
////    {
////        {'A', 'M', 'G'},
////        3,
////        200,
////        210,
////        3000,
////        -1
////    };
////
////    SportEvent A4 =
////    {
////        {'A', '4'},
////        4,
////        310,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent etron =
////    {
////        {'E', '-', 't', 'r', 'o', 'n'},
////        5,
////        260,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent Rs7 =
////    {
////        {'R', 's', '-', '7'},
////        6,
////        380,
////        210,
////        2600,
////        -1
////    };
////
////    AddSportEvent(1, M3);
////    AddSportEvent(1, M2);
////    AddSportEvent(2, G63);
////    AddSportEvent(4, A4);
////    AddSportEvent(4, etron);
////    AddSportEvent(4, Rs7);
////
////    DeleteMember(1);
////    DeleteSportEvent(5);
////
////    GetAllSportEventsInfo();
////    GetAllMembersInfo();
////}
////
////void test3()
////{
////    makeFiles(true);
////    const int SIZE = 5;
////    Member old_Members[SIZE] = {
////        {
////            {'B', 'M', 'W'},
////            {'K', 'y', 'i', 'v'},
////            1,
////            -1
////        },
////        {
////            {'B', 'e', 'n', 'z'},
////            {'L', 'v', 'i', 'v'},
////            2,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
////            3,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'K', 'a', 'n', 'i', 'v'},
////            4,
////            -1
////        },
////        {
////            {'B', 'M', 'W'},
////            {'L', 'v', 'i', 'v'},
////            5,
////            -1
////        },
////    };
////
////    for (int i = 0; i < SIZE; i++)
////    {
////        AddMember(old_Members[i]);
////    }
////
////    SportEvent M3 =
////    {
////        {'M', '3'},
////        1,
////        345,
////        300,
////        1500,
////        -1
////    };
////
////    SportEvent M2 =
////    {
////        {'M', '2'},
////        2,
////        300,
////        230,
////        1300,
////        -1
////    };
////
////    SportEvent G63 =
////    {
////        {'A', 'M', 'G'},
////        3,
////        200,
////        210,
////        3000,
////        -1
////    };
////
////    SportEvent A4 =
////    {
////        {'A', '4'},
////        4,
////        310,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent etron =
////    {
////        {'E', '-', 't', 'r', 'o', 'n'},
////        5,
////        260,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent Rs7 =
////    {
////        {'R', 's', '-', '7'},
////        6,
////        380,
////        210,
////        2600,
////        -1
////    };
////
////    AddSportEvent(1, M3);
////    AddSportEvent(1, M2);
////    AddSportEvent(2, G63);
////    AddSportEvent(4, A4);
////    AddSportEvent(4, etron);
////    AddSportEvent(4, Rs7);
////
////    DeleteMember(1);
////    DeleteSportEvent(5);
////
////    AddMember
////    (
////        {
////            {'A', 'u', 'd', 'i'},
////            {'O','d','e','s','s','a'},
////            10,
////            -1
////        }
////    );
////    AddSportEvent
////    (
////        10,
////        {
////        {'E', '-', 't', 'r', 'o', 'n', '-', 'G', 'T'},
////        599,
////        345,
////        111,
////        3000,
////        -1
////        }
////    );
////    GetAllSportEventsInfo();
////    GetAllMembersInfo();
////}
////
////void test4()
////{
////    makeFiles(true);
////    const int SIZE = 5;
////    Member old_Members[SIZE] = {
////        {
////            {'B', 'M', 'W'},
////            {'K', 'y', 'i', 'v'},
////            1,
////            -1
////        },
////        {
////            {'B', 'e', 'n', 'z'},
////            {'L', 'v', 'i', 'v'},
////            2,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
////            3,
////            -1
////        },
////        {
////            {'A', 'u', 'd', 'i'},
////            {'K', 'a', 'n', 'i', 'v'},
////            4,
////            -1
////        },
////        {
////            {'B', 'M', 'W'},
////            {'L', 'v', 'i', 'v'},
////            5,
////            -1
////        },
////    };
////
////    for (int i = 0; i < SIZE; i++)
////    {
////        AddMember(old_Members[i]);
////    }
////
////    SportEvent M3 =
////    {
////        {'M', '3'},
////        1,
////        345,
////        300,
////        1500,
////        -1
////    };
////
////    SportEvent M2 =
////    {
////        {'M', '2'},
////        2,
////        300,
////        230,
////        1300,
////        -1
////    };
////
////    SportEvent G63 =
////    {
////        {'A', 'M', 'G'},
////        3,
////        200,
////        210,
////        3000,
////        -1
////    };
////
////    SportEvent A4 =
////    {
////        {'A', '4'},
////        4,
////        310,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent etron =
////    {
////        {'E', '-', 't', 'r', 'o', 'n'},
////        5,
////        260,
////        210,
////        2400,
////        -1
////    };
////
////    SportEvent Rs7 =
////    {
////        {'R', 's', '-', '7'},
////        6,
////        380,
////        210,
////        2600,
////        -1
////    };
////
////    AddSportEvent(1, M3);
////    AddSportEvent(1, M2);
////    AddSportEvent(2, G63);
////    AddSportEvent(4, A4);
////    AddSportEvent(4, etron);
////    AddSportEvent(4, Rs7);
////
////    DeleteMember(1);
////    DeleteSportEvent(5);
////
////    AddMember
////    (
////        {
////            {'A', 'u', 'd', 'i'},
////            {'O','d','e','s','s','a'},
////            10,
////            -1
////        }
////    );
////    AddSportEvent
////    (
////        10,
////        {
////        {'E', '-', 't', 'r', 'o', 'n', '-', 'G', 'T'},
////        599,
////        345,
////        111,
////        3000,
////        -1
////        }
////    );
////    printAllMembers();
////    int temp;
////    cout << "Enter Delaer ID\n";
////    cin >> temp;
////    UpdateMember(temp);
////    printAllSportEvents();
////    cout << "Enter SportEvent ID\n";
////    cin >> temp;
////    UpdateSportEvent(temp);
////
////    GetAllSportEventsInfo();
////    GetAllMembersInfo();
////}

void runTests()
{
    int n = 0;
    while (n != -1)
    {
        cout << "Enter the number of test(1-4); -1 to exit :\n";
        cin >> n;
        switch (n)
        {
        case 1:
            test1();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 2:
            test2();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 3:
            test3();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 4:
            test4();
            cout << "Test " << n << " completed!\n\n";
            break;
        default:
            break;
        }

    }
}
int main()
{
    makeFiles(false);
    Menu();
    //runTests();
}