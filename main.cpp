// GerrymanderingApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Mustafa Ausaf
//CS251-Spring2023
//Project#1
//GerrymanderingApp
//Dr.koehler


#include "ourvector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <tgmath.h>
#include <iomanip>
using namespace std;

// information of all districts.txt

struct ditstsInf {
    int repVote = 0;
    int democVotes = 0;
    int abc;
};



// information of all states and district vector.

struct stateInf {
    ourvector<ditstsInf> xyzes;
    string stateName;
    int voteElig;
};


//districts.txt and eligble_voters.txt files are being loaded
//information are stores in vector of state strucutres that returns void.
 

void load(string file1, string file2, ourvector<stateInf>& work, string& working, bool& distLoaded) {
    if (working == "Yes") {
        cout << "Already read data in, exit and start over.\n";
        return;
    }
    string s1;
    ifstream openDistFile(file1);
    if (!openDistFile.is_open()) {
        cout << "Invalid first file, try again.\n\n";
        return;
    }

    cout << "Reading: " << file1 << endl;
    while (getline(openDistFile, s1)) {
        stateInf estate;
        ditstsInf edist;
        estate.stateName = s1.substr(0, s1.find(','));
        s1 = s1.substr(s1.find(',') + 1);
        int ab = 0;
        while (s1.find(',') != string::npos) {
            if (ab == 0) {
                if (s1.substr(0, s1.find(',')) == "AL")
                    edist.abc = 1;
                else
                    edist.abc = stoi(s1.substr(0, s1.find(',')));
            }
            if (ab == 1) {
                edist.democVotes = stoi(s1.substr(0, s1.find(',')));
            }
            if (ab == 2) {
                edist.repVote = stoi(s1.substr(0, s1.find(',')));
                estate.xyzes.push_back(edist);
                ab = -1;
            }
            ab++;
            s1 = s1.substr(s1.find(',') + 1);
        }
        if (s1.size() > 0) {
            edist.repVote = stoi(s1);
            estate.xyzes.push_back(edist);
        }
        work.push_back(estate);
        cout << "..." << work.at(work.size() - 1).stateName << "..." << work.at(work.size() - 1).xyzes.size()
            << " districts total" << endl;
    }
    distLoaded = true;
    openDistFile.close();

    ifstream eFile(file2);
    if (!eFile.is_open()) {
        cout << "Invalid second file, try again.\n\n";
        return;
    }
    cout << "\nReading: " << file2 << endl;
    while (getline(eFile, s1)) {
        string dixState = s1.substr(0, s1.find(','));
        int eVoters = stoi(s1.substr(s1.find(',') + 1));
        for (int i = 0; i < work.size(); i++) {
            if (work.at(i).stateName == dixState) {
                work.at(i).voteElig = eVoters;
                cout << "..." << work.at(i).stateName << "..."
                    << work.at(i).voteElig << " eligible voters\n";
            }
        }
    }
    eFile.close();
    working = "Yes";
    cout << endl;
}

// Displaying Gerrymandering stats

void stats(stateInf& state) {
    int totDemVotes = 0;
    int totRepVotes = 0;
    int wasteDemVot = 0;
    int wasteRepVot = 0;

    for (int z = 0; z < state.xyzes.size(); z++) {
        int half = (state.xyzes.at(z).democVotes + state.xyzes.at(z).repVote) / 2 + 1;

        totDemVotes += state.xyzes.at(z).democVotes;
        totRepVotes += state.xyzes.at(z).repVote;

        if (state.xyzes.at(z).democVotes > state.xyzes.at(z).repVote) {
            wasteRepVot += state.xyzes.at(z).repVote;
            wasteDemVot += state.xyzes.at(z).democVotes - half;
        }
        else {
            wasteDemVot += state.xyzes.at(z).democVotes;
            wasteRepVot += state.xyzes.at(z).repVote - half;
        }

    }

    int combVote = totDemVotes + totRepVotes;
    float effSpace = 100 * float(abs(wasteDemVot - wasteRepVot)) / combVote;
    string collap;
    if (totDemVotes > totRepVotes)
        collap = "Republicans";
    else
        collap = "Democrats";


    if (effSpace >= 7 && state.xyzes.size() > 2) {
        cout << "Gerrymandered: Yes\n";
        cout << "Gerrymandered against: " << collap << endl;
        cout << "Efficiency Factor: " << setprecision(6) << effSpace << "%\n";
    }
    else
        cout << "Gerrymandered: No\n";
    cout << "Wasted Democratic votes: " << wasteDemVot << endl;
    cout << "Wasted Republican votes: " << wasteRepVot << endl;
    cout << "Eligible voters: " << state.voteElig << endl << endl;


}

void plot(stateInf state) {
    for (int i = 0; i < state.xyzes.size(); i++) {
        if ((state.xyzes.at(i).democVotes + state.xyzes.at(i).repVote) == 0) {
            cout << "District: " << i + 1 << endl << endl;
            continue;
        }
        int numOfDemoc = floor(100 * float(state.xyzes.at(i).democVotes) / (state.xyzes.at(i).democVotes + state.xyzes.at(i).repVote));
        int numOfRep = abs(100 - numOfDemoc);
        cout << "District: " << i + 1 << endl;
        for (int a = 0; a < numOfDemoc; a++) {
            cout << "D";
        }
        for (int a = 0; a < numOfRep; a++) {
            cout << "R";
        }
        cout << endl;
    }
}

/*case correction for searches*/

string correct(string line) {

    line = line.substr(1);
    string corrLine = "";

    while (line.find(' ') != string::npos) {
        string currLine;
        string lowLine;
        currLine += toupper(line[0]);
        for (int i = 1; i < line.substr(0, line.find(' ')).size(); i++) {
            lowLine += tolower(line[i]);
        }
        currLine += lowLine;
        corrLine += currLine + " ";
        line = line.substr(line.find(' ') + 1);
    }
    string lowLine;
    for (int i = 1; i < line.size(); i++) {
        lowLine += tolower(line[i]);
    }
    corrLine += toupper(line[0]);
    corrLine += lowLine;

    return corrLine;
}

//This function searches the primary state vector for a state specified by the user.It updates the relevant variables
//such as state nameand whether the state was found, by passing them as references.The function does not return any values.

void searchState(string inState, ourvector<stateInf>& states, string& NameOfState, stateInf& stateFound) {
    bool stateExists = false;
    for (int i = 0; i < states.size(); i++) {
        if (states.at(i).stateName == inState) {
            stateFound = states.at(i);
            NameOfState = stateFound.stateName;
            stateExists = true;
            break;
        }
    }
    if (!stateExists) {
        cout << "State does not exist, search again.\n";
    }
}

//This is the location of the user interface loop.

int main()
{
    string isWorking = "No";
    string stateName = "N/A";
    string in1;
    string in2;
    string in3;
    bool dixLoaded;
    ourvector<stateInf> states;
    stateInf foundState;

    cout << "Welcome to the Gerrymandering App!\n\n";
    while (in1 != "exit") {
        cout << "Data loaded? " << isWorking << endl;
        cout << "State: " << stateName;
        cout << endl << endl;
        cout << "Enter command: ";
        cin >> in1;
        cout << "\n-----------------------------\n\n";
        if (in1 == "load") {
            cin >> in2 >> in3;
            load(in2, in3, states, isWorking, dixLoaded);
        }
        if (in1 == "search") {
            getline(cin, in2);
            if (isWorking == "No" && !dixLoaded) {
                cout << "No data loaded, please load data first.\n\n";
            }
            else {
                searchState(correct(in2), states, stateName, foundState);
            }
        }
        if (in1 == "stats") {
            if (stateName == "N/A" && isWorking == "Yes") {
                cout << "No state indicated, please search for state first.\n\n";
            }
            if (isWorking == "No") {
                cout << "No data loaded, please load data first.\n\n";
            }
            if (isWorking == "Yes" && stateName != "N/A")
                stats(foundState);
        }
        if (in1 == "plot") {
            if (isWorking == "No")
                cout << "No data loaded, please load data first.\n\n";
            else
                plot(foundState);
        }
    }

    return 0;
}


