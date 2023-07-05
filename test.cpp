#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_SONGS = 1000;
const int MAX_PLAYLISTS = 20;

const string DEFAULT_LIBRARY_FILE = "musiclibrary.tsv";
const string DEFAULT_PLAYLIST_FILE = "musicplaylists.txt";

int Split(string line, char delim, string arr[], int size) {
    // if received an empty line, return 0
    if (line.size() == 0) {
        return 0;
    }

    // keep track of characters
    string word = "";
    // keep track of splits
    int j = 0;
    line = line + delim;

    for (int i = 0; i < line.length(); i++) {
        // if we've reached the delimiter, add word to array
        if (line[i] == delim) {
            // if word is empty, move to next iteration
            if (word.size() == 0) {
                continue;
            }
            // check if number of splits greater than max number
            if (j >= size) {
                return -1;
            }
            // add word to array
            arr[j] = word;
            // increment counter variable
            j++;
            // reset word variable
            word = "";
        } else {
            // if not at delimiter, add char to word
            word += line[i];
        }
    }
    // return number of splits
    return j;
}

int ReadLibrary(string inputFile, string artists[],
    string titles[], string genres[], string urls[]) {
    // declare variables
    string line;
    string temp[4];
    ifstream input(inputFile);
    int numSongs = 0;

    // read each line in the file
    while (getline(input, line)) {
        if (Split(line, '\t', temp, 4) != 4) { // split using tab characters
            cout << "Error reading line. Song skipped." << endl;
        } else {
            artists[numSongs] = temp[0];
            titles[numSongs] = temp[1];
            genres[numSongs] = temp[2];
            urls[numSongs] = temp[3];
            numSongs++;
        }
    }
    return numSongs;
}

bool ValidateInt(string input) {
    // check if the first digit is valid
    if (!isdigit(input[0]) && input[0] != '-') { // can be a number or minus
        return false;
    }
    // check if all other digits are valid for index 1 through the end
    for (int i = 1; i < input.length(); i++) {
        if (!isdigit(input[i])) { // if the character isn't a number return false
            return false;
        }
    }
    return true; // if it was never invalid, we return it was valid
}

int ReadPlaylists(string inputFile, int playlists[][MAX_SONGS], string playlistNames[]) {
    int numPlaylists = 0;
    int currentSong = 0;
    ifstream input(inputFile);
    string line;

    while (getline(input, line)) {
        if (ValidateInt(line)) {
            playlists[numPlaylists - 1][currentSong] = stoi(line);
            currentSong++;
        } else {
            playlistNames[numPlaylists] = line;
            numPlaylists++;
            currentSong = 0;
        }
    }
    return numPlaylists;
}

int main() {
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
        ifstream library(libraryFile);
        while(!library.is_open()) {
            cout << "Cannot open that file. Try again: ";
            cin >> libraryFile;
            ifstream library(libraryFile);
            if(library.is_open()) {
                break;
            }
        }
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
    string artists[MAX_SONGS];
    string titles[MAX_SONGS];
    string genres[MAX_SONGS];
    string urls[MAX_SONGS];
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
        ifstream playlists(playlistFile);
        while(!playlists.is_open()) {
            cout << "Cannot open that file. Try again: ";
            cin >> playlistFile;
            ifstream playlists(playlistFile);
            if(playlists.is_open()) {
                break;
            }
        }
    }

    // Attempt to open the playlist file and create it if it doesn't exist
    ifstream playlists(playlistFile);
    if (!playlists.is_open()) {
        ofstream createPlaylists(playlistFile);
        createPlaylists.close();
        playlists.open(playlistFile);
    }
    playlists.close();

    // Call the ReadPlaylists function
    int playlistsArr[MAX_PLAYLISTS][MAX_SONGS];
    string playlistNames[MAX_PLAYLISTS];
    int numPlaylists = ReadPlaylists(playlistFile, playlistsArr, playlistNames);

    // Print confirmation statement
    cout << "Library and playlists loaded." << endl;

    // Proceed with the central menu loop
    // ...

    return 0;
}