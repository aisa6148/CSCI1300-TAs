#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
//#include<menu.h>



#define stringstream DO_NOT_USE_STRINGSTREAM
#define vector DO_NOT_USE_VECTORS
using namespace std;
const int MAX_SONGS = 1000;
const int MAX_PLAYLISTS = 20;
const string DEFAULT_LIBRARY_FILE = "musiclibrary.tsv";
const string DEFAULT_PLAYLIST_FILE = "musicplaylists.txt";

void displayMenu1(int, string[], string[], string[], string[], int[][MAX_SONGS], int, string[]);
void displayMenu2(int, string[], string[], string[], string[], int[][MAX_SONGS], int, string[]);
void displayMenu3 (int, string[], string[], string[], string[], int[][MAX_SONGS], int, string[]);
void displayMenu4(int, int, string[], string[], string[], string[], int, int[][MAX_SONGS], int, string[]);
void displayMenu5(int, int, string[], string[], string[], string[], int, int[][MAX_SONGS], int, string[]);
void displayMainMenu(int, string[], string[], string[], string[], int[][MAX_SONGS], int, string[]);


int Split(string line, char delim, string arr[], int size)
{
    // if received an empty line, return 0
    if (line.size() == 0)
    {
        return 0;
    }
    // keep track of characters
    string word = "";
    // keep track of splits
    int j = 0;
    line = line + delim;
    for (int i = 0; i < line.length(); i++)
    {
        // if we've reached the delimiter, add word to array
        if (line[i] == delim)
        {
            // if word is empty, move to next iteration
            if (word.size() == 0)
            {
                continue;
            }
            // check if number of splits greater than max number
            if (j >= size)
            {
                return -1;
            }
            // add word to array
            arr[j] = word;
            // increment counter variable
            j++;
            // reset word variable
            word = "";
        }
        else
        {
            // if not at delimiter, add char to word
            word += line[i];
        }
    }
    // return number of splits
    return j;
}
int ReadLibrary(string inputFile, string artists[],
                string titles[], string genres[], string urls[])
{
    // declare variables
    string line;
    string temp[4];
    ifstream input(inputFile);
    int numSongs = 0;
    // read each line in the file
    while (getline(input, line))
    {
        if (Split(line, '\t', temp, 4) != 4)
        { // split using tab characters
            cout << "Error reading line. Song skipped." << endl;
        }
        else
        {
            artists[numSongs] = temp[0];
            titles[numSongs] = temp[1];
            genres[numSongs] = temp[2];
            urls[numSongs] = temp[3];
            numSongs++;
        }
    }
    return numSongs;
}
bool ValidateInt(string input)
{
    // check if the first digit is valid
    if (!isdigit(input[0]) && input[0] != '-')
    { // can be a number or minus
        return false;
    }
    // check if all other digits are valid for index 1 through the end
    for (int i = 1; i < input.length(); i++)
    {
        if (!isdigit(input[i]))
        { // if the character isn't a number return false
            return false;
        }
    }
    return true; // if it was never invalid, we return it was valid
}
int ReadPlaylists(string inputFile, int playlists[][MAX_SONGS], string playlistNames[])
{
    int numPlaylists = 0;
    int currentSong = 0;
    ifstream input(inputFile);
    string line;
    while (getline(input, line))
    {
        if (ValidateInt(line))
        {
            playlists[numPlaylists - 1][currentSong] = stoi(line);
            currentSong++;
        }
        else
        {
            playlistNames[numPlaylists] = line;
            // cout << line << endl;
            numPlaylists++;
            currentSong = 0;
        }
    }
    return numPlaylists;
}
int NewPlaylist(int numPlaylists, string playlistNames[], string newName)
{
    if (numPlaylists < MAX_PLAYLISTS)
    {
        playlistNames[numPlaylists] = newName;
        return numPlaylists + 1;
    }
    else
    {
        return numPlaylists;
    }
}
int AddSongLibrary(int librarySize, string newArtist, string newTitle, string newGenre, string newURL,
                   string artists[], string titles[], string genres[], string urls[])
{
    if (librarySize < MAX_SONGS)
    {
        artists[librarySize] = newArtist;
        titles[librarySize] = newTitle;
        genres[librarySize] = newGenre;
        urls[librarySize] = newURL;
        return librarySize + 1;
    }
    else
    {
        return librarySize;
    }
}
bool AddSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS], int songID, int playlistID)
{
    if (numPlaylists <= playlistID)
    {
        return false;
    }
    for (int i = 0; i < MAX_SONGS; i++)
    {
        if (playlists[playlistID][i] == -1)
        {
            playlists[playlistID][i] = songID;
            return true;
        }
    }
    return false;
}
bool DeleteSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS],
                        int songID, int playlistID)
{
    bool removed = false;
    for (int i = 0; i < MAX_SONGS; i++)
    {
        if (playlists[playlistID][i] == songID)
        {
            removed = true;
            for (int j = i; j < MAX_SONGS - 1; j++)
            {
                playlists[playlistID][j] = playlists[playlistID][j + 1];
            }
            playlists[playlistID][MAX_SONGS - 1] = -1;
        }
    }
    return removed;
}

int DeleteSongLibrary(int librarySize, string artists[], string titles[], string genres[],
                      string urls[], int songID, int playlists[][MAX_SONGS])
{
    // check if the songID is invalid
    if (songID > librarySize - 1)
        return librarySize;
    // update library
    for (int i = songID; i < MAX_SONGS - 1; i++)
    {
        artists[i] = artists[i + 1];
        titles[i] = titles[i + 1];
        genres[i] = genres[i + 1];
        urls[i] = urls[i + 1];
    }
    artists[MAX_SONGS - 1] = "";
    titles[MAX_SONGS - 1] = "";
    genres[MAX_SONGS - 1] = "";
    urls[MAX_SONGS - 1] = "";
    // remove song from all playlists
    for (int i = 0; i < MAX_PLAYLISTS; i++)
    {
        DeleteSongPlaylist(i + 1, playlists, songID, i);
    }
    // all songIDs in playlists after the removed song must be decreased by 1
    for (int i = 0; i < MAX_PLAYLISTS; i++)
    {
        for (int j = 0; j < MAX_SONGS; j++)
        {
            if (playlists[i][j] > songID)
            {
                playlists[i][j]--;
            }
        }
    }
    return librarySize - 1;
}

bool SwapSongs(int numPlaylists, int playlists[][MAX_SONGS], int songID1,
               int songID2, int playlistID)
{
    int pos1 = MAX_SONGS;
    int pos2 = MAX_SONGS;
    for (int i = MAX_SONGS - 1; i >= 0; i--)
    {
        if (playlists[playlistID][i] == songID1)
        {
            pos1 = i;
        }
        if (playlists[playlistID][i] == songID2)
        {
            pos2 = i;
        }
    }
    if (pos1 < MAX_SONGS && pos2 < MAX_SONGS)
    {
        playlists[playlistID][pos1] = songID2;
        playlists[playlistID][pos2] = songID1;
        return true;
    }
    return false;
}

bool MoveSong(int numPlaylists, int playlists[][MAX_SONGS], int songID,
              int playlistID, int newLocation)
{
    int lastSong = 0;
    int firstPlace = MAX_SONGS;
    // Check that song is in playlist and store its first location if it is. Also find the place of the last song
    for (int i = MAX_SONGS - 1; i > 0; i--)
    {
        if (playlists[playlistID][i] == -1)
        {
            lastSong = i - 1;
        }
        if (playlists[playlistID][i] == songID)
        {
            firstPlace = i;
        }
    }
    if (firstPlace == MAX_SONGS)
        return false;
    // Update new place based on num songs
    if (lastSong < newLocation)
    {
        newLocation = lastSong;
    }
    // If we're moving the song backwards...
    if (firstPlace < newLocation)
    {
        for (int i = firstPlace; i < newLocation; i++)
        {
            playlists[playlistID][i] = playlists[playlistID][i + 1];
        }
        playlists[playlistID][newLocation] = songID;
        return true;
    }
    // If we're moving the song forwards...
    if (newLocation < firstPlace)
    {
        for (int i = firstPlace; i > newLocation; i--)
        {
            playlists[playlistID][i] = playlists[playlistID][i - 1];
        }
        playlists[playlistID][newLocation] = songID;
        return true;
    }
    // If the song is already in that spot...
    return false;
}

bool IsMatch(string small, string big){
    bool match = false;
    if (big.length() < small.length()) return false;
    for (int i = 0; i < big.length()-small.length()+1; i++){
        if (small == big.substr(i, small.length())){
            match = true;
        }
    }
    return match;
}
int FindSongID(string artists[], string titles[], string genres[], int librarySize){
    //declare variables
    char userChoice;
    int songID;
    int matches[MAX_SONGS];
    string userSearch;
    int numMatches = 0;

    cout << "Would you like to search by artist (A), title (T), or genre (G)?" << endl;
    cin >> userChoice;

    while (userChoice != 'A' && userChoice != 'T' && userChoice != 'G'){
        cout << "Invalid choice." << endl;
        cout << "Would you like to search by artist (A), title (T), or genre (G)?" << endl;
        cin >> userChoice;
    }
    
    cin.ignore();

    switch(userChoice){
        case 'A':
            cout << "What artist would you like to search for?" << endl;
            getline(cin, userSearch);
            for (int i = 0; i < librarySize; i++){
                if (IsMatch(userSearch, artists[i])){
                    matches[numMatches] = i;
                    numMatches++;
                    cout << numMatches << ": \"" << titles[i] << "\" by " << artists[i] << endl;
                }
            }
            break;

        case 'T':
            cout << "What title would you like to search for?" << endl;
            getline(cin, userSearch);

            for (int i = 0; i < librarySize; i++){
                if ((IsMatch(userSearch, titles[i]))){
                    matches[numMatches] = i;
                    numMatches++;
                    cout << numMatches << ": \"" << titles[i] << "\" by " << artists[i] << endl;
                }
            }
            break;

        case 'G':
            cout << "What genre would you like to search for?" << endl;
            getline(cin, userSearch);
            
            for (int i = 0; i < librarySize; i++){
                if (IsMatch(userSearch, genres[i])){
                    matches[numMatches] = i;
                    numMatches++;
                    cout << numMatches << ": \"" << titles[i] << "\" by " << artists[i] << endl;
                }
            }
    }
    
    if (numMatches < 1){
        cout << "No matches found." << endl;
        return -1;
    }

    cout << "Which number song is your choice? If none of the above, enter -1." << endl;
    cin >> songID;
    while (songID < -1 || songID > numMatches){
        cout << "Invalid choice." << endl;
        cout << "Which number song is your choice?" << endl;
        cin >> songID;
    }
    if (songID == -1) return -1;
    return matches[songID-1];
}
void PrintSpotify(int numPlaylists, int playlists[][MAX_SONGS], int playlistID,
                  string urls[])
{
    for (int i = 0; i < MAX_SONGS; i++)
    {
        if (playlists[playlistID][i] >= 0)
        {
            cout << urls[playlists[playlistID][i]] << endl;
        }
    }
}
void PrintPlaylist(int numPlaylists, int playlists[][MAX_SONGS], int playlistID,
                   string artists[], string titles[])
{
    for (int i = 0; i < MAX_SONGS; i++)
    {
        if (playlists[playlistID][i] >= 0)
        {
            cout << "\"" << titles[playlists[playlistID][i]] << "\" by " << artists[playlists[playlistID][i]] << endl;
        }
    }
}
void SaveChanges(string libraryFile, string playlistFile, int librarySize, int numPlaylists, string artists[], string titles[],
                 string genres[], string urls[], int playlists[][MAX_SONGS], string playlistNames[])
{
    char userChoice;
    string location;
    ofstream library(libraryFile);
    ofstream playlistStream(playlistFile);
    for (int i = 0; i < librarySize; i++)
    {
        library << artists[i] << "\t" << titles[i] << "\t" << genres[i] << "\t" << urls[i] << "\n";
    }

    for (int i = 0; i < numPlaylists; i++)
    {
        playlistStream << playlistNames[i] << endl;
        for (int j = 0; j < MAX_SONGS; j++)
        {
            if (playlists[i][j] >= 0)
            {
                playlistStream << playlists[i][j] << endl;
            }
        }
    }
}

void displayMenu1(int libSize, string artists[], string titles[], string genres[], string urls[], int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int option, songID, s;
    string newName, newTitle, newGenre, newURL;
    cout << "Select an option: \n1. Add a song to your library \n2. Remove a song from your library \n3. Search your library \n4. Go back";
    cin >> option;
    switch (option) {
        case 1:
            cout << "What is the artist name?";
            cin >> newName;
            cout << "What is the song title?";
            cin >> newTitle;
            cout << "What is the genre?";
            cin >> newGenre;
            cout << "What is the spotify URL?";
            cin >> newURL;
            s = AddSongLibrary(libSize, newName, newTitle, newGenre, newURL, artists, titles, genres, urls);
            cout<< s << libSize;
            if (s>libSize) {
                cout <<"song added\n";
                displayMenu1(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);

            }
            break;
        case 2:
            songID = FindSongID(artists, titles, genres, libSize);
            DeleteSongLibrary(libSize, artists, titles, genres, urls, songID, playlists);
            break;
        case 3:
            FindSongID(artists, titles, genres, libSize);
            break;
        default: 
            int op;
            cout << "Select an option \n 1. Access Music Library \n 2. Access Playlists \n 3. Quit";
            cin >> op;
            switch (op) {
                case 1:
                    displayMenu1(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                case 2:
                    displayMenu2(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                case 3:
                    displayMenu3(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                default: 
                    displayMainMenu(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
            }
            break;
    }
}

void displayMenu2(int libSize, string artists[], string titles[], string genres[], string urls[], int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int option1, option2, option3, songID, playlistID;
    string newName;
    cout << "Open an existing playlist \n Select an option: \n1. Open an existing playlist \n2. Create a new playlist. \n4. Go back";
    cin >> option1;
    switch (option1) {
        case 1:
            cout << "Select a playlist:";
            for(int i=0; i<numPlaylists; i++) {
                cout << i+1 << ". " << playlists[i];
            }
            cin >> playlistID;
            cout << "Select an option: \n1. Add a song to your playlist \n2. Remove a song from your playlist. \n4. Swap the position of two songs in your playlist \n 5. Move a song to a new position in your playlist \n 6. Print your playlist 7. Go Back";
            cin >> option2;
            displayMenu4(option2, playlistID, artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
            break;
        case 2:
            cout << "Enter name of your new playlist:";
            cin >> newName;
            numPlaylists = NewPlaylist(numPlaylists, playlistNames, newName);
            cout << "Create a new playlist. \n Select an option: \n1. Add a song to your playlist \n2. Remove a song from your playlist. \n3. Swap the position of two songs in your playlist \n 4. Move a song to a new position in your playlist \n 5. Print your playlist 6. Go Back";
            cin >> option3;
            displayMenu5(option3, (numPlaylists - 1), artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
            break;
        default:
            displayMenu2(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
    }
}

void displayMenu3 (int libSize, string artists[], string titles[], string genres[], string urls[], int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int option; 
    string loc;
    cout << "Select an option:\n 1. Save and quit \n 2. Quit without saving \n 3. Go back";
    cin >> option;
        switch (option) {
        case 1:
            cout << "Would you like to save the library to the current location? Y/N: ";
            cin >> loc;
            if (loc == "Y" || loc == "y") {
                // Use the library file that you opened earlier
                SaveChanges(DEFAULT_LIBRARY_FILE, DEFAULT_PLAYLIST_FILE, libSize, numPlaylists, artists, titles, genres, urls, playlists, playlistNames);
            } else if (loc == "N" || loc == "n") {
                cout << "Enter the new save location for the library: ";
                cin >> loc;
                // Call SaveChanges with the new library file location
                SaveChanges(loc, DEFAULT_PLAYLIST_FILE, libSize, numPlaylists, artists, titles, genres, urls, playlists, playlistNames);
            } else {
                cout << "Invalid option. Library will not be saved." << endl;
            }

            cout << "Would you like to save the playlists to the current location? Y/N: ";
            cin >> loc;
            if (loc == "Y" || loc == "y") {
                // Use the playlist file that you opened earlier
                SaveChanges(DEFAULT_LIBRARY_FILE, DEFAULT_PLAYLIST_FILE, libSize, numPlaylists, artists, titles, genres, urls, playlists, playlistNames);
            } else if (loc == "N" || loc == "n") {
                cout << "Enter the new save location for the playlists: ";
                cin >> loc;
                // Call SaveChanges with the new playlist file location
                SaveChanges(DEFAULT_LIBRARY_FILE, loc, libSize, numPlaylists, artists, titles, genres, urls, playlists, playlistNames);
            } else {
                cout << "Invalid option. Playlists will not be saved." << endl;
            }

            break;
        case 2:
            return;
        default:
            int op;
            cout << "Select an option \n 1. Access Music Library \n 2. Access Playlists \n 3. Quit";
            cin >> op;
            switch (op) {
                case 1:
                    displayMenu1(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                case 2:
                    displayMenu2(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                case 3:
                    displayMenu3(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
                default: 
                    displayMainMenu(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
                    break;
            }
            break;
    }
}

void displayMenu4(int option2, int playlistID, string artists[], string titles[], string genres[], string urls[], int libSize, int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int songID, songID1, songID2, option;
    string pos;
    int newLocation;
    switch (option2) {
        case 1:
            songID = FindSongID(artists, titles, genres, libSize);
            AddSongPlaylist(numPlaylists, playlists, songID, playlistID);
            break;
        case 2:
            songID = FindSongID(artists, titles, genres, libSize);
            DeleteSongPlaylist(numPlaylists, playlists, songID, playlistID);
            break;
        case 3:
            songID1 = FindSongID(artists, titles, genres, libSize);
            songID2 = FindSongID(artists, titles, genres, libSize);
            SwapSongs(numPlaylists, playlists, songID1, songID2, playlistID);
            break;
        case 4:
            songID = FindSongID(artists, titles, genres, libSize);
            cout << "Enter a numerical position or the word 'end' to add to the end of the playlist.";
            getline(cin, pos);
            if (pos == "end") {
                newLocation = MAX_SONGS;
            } else {
                newLocation = std::stoi(pos);
            }
            MoveSong(numPlaylists, playlists, songID, playlistID, newLocation);
            break;
        case 5:
            cout << "Select an option: \n1. Print by artists/titles \n2. Print Spotify Playlist \n3. Go Back";
            cin >> option;
            switch (option) {
                case 1: 
                    PrintPlaylist(numPlaylists, playlists, playlistID, artists, titles);
                    break;
                case 2:
                    PrintSpotify(numPlaylists, playlists, playlistID, urls);
                    break;
                case 3:
                    displayMenu4(option2, playlistID, artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
                    break;
                default:
                    displayMenu4(option2, playlistID, artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
                    break;
            }
        default:
            displayMenu2(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;

    }
}

void displayMenu5(int option3, int playlistID, string artists[], string titles[], string genres[], string urls[], int libSize, int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int songID, songID1, songID2, option;
    string pos;
    int newLocation;
    switch (option3) {
        case 1:
            songID = FindSongID(artists, titles, genres, libSize);
            AddSongPlaylist(numPlaylists, playlists, songID, playlistID);
            break;
        case 2:
            songID = FindSongID(artists, titles, genres, libSize);
            DeleteSongPlaylist(numPlaylists, playlists, songID, playlistID);
            break;
        case 3:
            songID1 = FindSongID(artists, titles, genres, libSize);
            songID2 = FindSongID(artists, titles, genres, libSize);
            SwapSongs(numPlaylists, playlists, songID1, songID2, playlistID);
            break;
        case 4:
            songID = FindSongID(artists, titles, genres, libSize);
            cout << "Enter a numerical position or the word 'end' to add to the end of the playlist: ";
            getline(cin, pos);
            if (pos == "end") {
                newLocation = MAX_SONGS;
            } else {
                newLocation = std::stoi(pos);
            }
            MoveSong(numPlaylists, playlists, songID, playlistID, newLocation);
            break;
        case 5:
            cout << "Select an option:\n1. Print by artists/titles\n2. Print Spotify Playlist\n3. Go Back" << endl;
            cin >> option;
            switch (option) {
                case 1: 
                    PrintPlaylist(numPlaylists, playlists, playlistID, artists, titles);
                    break;
                case 2:
                    PrintSpotify(numPlaylists, playlists, playlistID, urls);
                    break;
                case 3:
                    displayMenu5(option3, playlistID, artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
                    break;
                default:
                    displayMenu5(option3, playlistID, artists, titles, genres, urls, libSize, playlists, numPlaylists, playlistNames);
                    break;
            }
        default:
            displayMenu2(libSize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
    }
}

void displayMainMenu(int librarySize, string artists[], string titles[], string genres[], string urls[], int playlists[][MAX_SONGS], int numPlaylists, string playlistNames[]) {
    int option;
    cout << "Select an option \n 1. Access Music Library \n 2. Access Playlists \n 3. Quit";
    cin >> option;
    switch (option) {
        case 1:
            displayMenu1(librarySize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
        case 2:
            displayMenu2(librarySize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
        case 3:
            displayMenu3(librarySize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
        default: 
            displayMainMenu(librarySize, artists, titles, genres, urls, playlists, numPlaylists, playlistNames);
            break;
    }
}


int main() {
    int librarySize = -1;        // number of songs in the library
    int numPlaylists = -1;       // number of playlists
    string artists[MAX_SONGS];   // will be an array to store the artist names for all the songs in your library;
    string titles[MAX_SONGS];    // will be an array to store the titles of all the songs in your library.
    string genres[MAX_SONGS];    // will be an array to store the genres of all the songs in your library.
    string urls[MAX_SONGS];      // will be an array to store the Spotify URLs for all the songs in your library.
    string playlistNames[MAX_PLAYLISTS];
    int playlistsArr[MAX_PLAYLISTS][MAX_SONGS];

    // Create variables
    string libraryFile = DEFAULT_LIBRARY_FILE;
    string playlistFile = DEFAULT_PLAYLIST_FILE;

    // Ask if the user wants to open the default library file
    cout << "Would you like to open the default library? Y/N" << endl;
    char choice;
    cin >> choice;

    // Validate user input for opening the default library file
    while (choice != 'Y' && choice != 'N') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> choice;
    }

    // Check user's choice and set the library file accordingly
    if (choice == 'N') {
        cout << "What library file would you like to open? ";
        cin >> libraryFile;
    }

    // Attempt to open the library file and create it if it doesn't exist
    ifstream library(libraryFile);
    if (!library.is_open()) {
        ofstream createLibrary(libraryFile);
        createLibrary.close();
        library.open(libraryFile);
    }
    library.close();

    // Call the ReadLibrary function
    int numSongs = ReadLibrary(libraryFile, artists, titles, genres, urls);

    // Ask if the user wants to open the default playlist file
    cout << "Would you like to open the default playlists? Y/N" << endl;
    cin >> choice;

    // Validate user input for opening the default playlist file
    while (choice != 'Y' && choice != 'N') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> choice;
    }

    // Check user's choice and set the playlist file accordingly
    if (choice == 'N') {
        cout << "What playlist file would you like to open? ";
        cin >> playlistFile;
    }

    // Attempt to open the playlist file and create it if it doesn't exist
    ifstream playlistFileInput(playlistFile);
    if (!playlistFileInput.is_open()) {
        ofstream createPlaylists(playlistFile);
        createPlaylists.close();
        playlistFileInput.open(playlistFile);
    }
    playlistFileInput.close();

    // Call the ReadPlaylists function
    numPlaylists = ReadPlaylists(playlistFile, playlistsArr, playlistNames);

    // Print confirmation statement
    cout << "Library and playlists loaded." << endl;

    displayMainMenu(librarySize, artists, titles, genres, urls, playlistsArr, numPlaylists, playlistNames);

    return 0;
}