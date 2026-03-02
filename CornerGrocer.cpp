/*
 * CornerGrocer.cpp
 * CS 210 - Project Three
 *
 * Description:
 *   Item-tracking program for the Corner Grocer. Reads a daily purchase
 *   log (CS210_Project_Three_Input_File.txt), builds a frequency map,
 *   backs the data up to frequency.dat automatically on startup, then
 *   presents a four-option menu so staff can query purchase frequencies.
 *
 * Menu Options:
 *   1 - Look up frequency of a single item
 *   2 - Display all items with their frequencies
 *   3 - Display a text-based histogram of all item frequencies
 *   4 - Exit the program
 */

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <limits>
#include <cctype>

// ============================================================
//  Class: GroceryTracker
//
//  Public:
//    GroceryTracker()       - constructor, loads data + writes backup
//    getFrequency()         - returns purchase count for one item
//    printAllFrequencies()  - prints every item and its count
//    printHistogram()       - prints asterisk-based histogram
//
//  Private:
//    m_frequencyMap         - stores item -> count pairs
//    normalise()            - converts input to Title Case for matching
//    loadFromFile()         - reads input file into m_frequencyMap
//    writeBackupFile()      - writes m_frequencyMap to frequency.dat
// ============================================================
class GroceryTracker {
public:

    // Constructor: loads purchase data then immediately writes the backup file
    GroceryTracker(const std::string& inputFilename, const std::string& outputFilename) {
        loadFromFile(inputFilename);       // populate the frequency map
        writeBackupFile(outputFilename);   // write frequency.dat backup
    }

    // Menu Option 1: return the frequency of a single user-specified item
    int getFrequency(const std::string& item) const {
        std::string key = normalise(item);  // normalise to Title Case
        auto it = m_frequencyMap.find(key);
        if (it != m_frequencyMap.end()) {
            return it->second;  // return the count if item exists
        }
        return 0;  // item was not found in today's records
    }

    // Menu Option 2: print every item paired with its purchase count
    void printAllFrequencies() const {
        std::cout << "\n--- Item Frequencies ---\n";
        // Iterate through the map (automatically sorted alphabetically)
        for (const auto& pair : m_frequencyMap) {
            std::cout << pair.first << " " << pair.second << "\n";
        }
        std::cout << "------------------------\n";
    }

    // Menu Option 3: print a text histogram using '*' per purchase
    void printHistogram() const {
        std::cout << "\n--- Frequency Histogram ---\n";
        for (const auto& pair : m_frequencyMap) {
            // Print item name, padded to 15 chars so asterisks align
            std::cout << pair.first;
            int padding = 15 - static_cast<int>(pair.first.size());
            if (padding > 0) {
                std::cout << std::string(padding, ' ');
            } else {
                std::cout << " ";  // at least one space if name is long
            }
            // Print one '*' for each purchase
            std::cout << std::string(pair.second, '*') << "\n";
        }
        std::cout << "---------------------------\n";
    }

private:

    // Private data member: maps item name (Title Case) to purchase count
    std::map<std::string, int> m_frequencyMap;

    // Private helper: converts a string to Title Case (e.g. "APPLE" -> "Apple")
    // Ensures "apple", "Apple", and "APPLE" all map to the same key
    std::string normalise(const std::string& word) const {
        if (word.empty()) return word;
        std::string result = word;
        // First character uppercase
        result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[0])));
        // Remaining characters lowercase
        for (size_t i = 1; i < result.size(); ++i) {
            result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
        }
        return result;
    }

    // Private: open input file and count how often each item appears
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            // Alert the user if the input file cannot be found
            std::cerr << "[ERROR] Could not open input file: " << filename << "\n";
            return;
        }
        std::string word;
        while (inFile >> word) {
            // Increment the count for this item (inserts with 0 if new)
            m_frequencyMap[normalise(word)]++;
        }
        inFile.close();
    }

    // Private: write every item and its count to the backup file
    void writeBackupFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "[ERROR] Could not create backup file: " << filename << "\n";
            return;
        }
        for (const auto& pair : m_frequencyMap) {
            outFile << pair.first << " " << pair.second << "\n";
        }
        outFile.close();
    }
};

// ============================================================
//  displayMenu: prints the four menu options to the console
// ============================================================
void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "       Corner Grocer Item Tracker\n";
    std::cout << "========================================\n";
    std::cout << "  1. Look up a single item's frequency\n";
    std::cout << "  2. Display all item frequencies\n";
    std::cout << "  3. Display histogram of all items\n";
    std::cout << "  4. Exit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Enter your choice (1-4): ";
}

// ============================================================
//  getValidMenuChoice: keeps prompting until user enters 1-4
//  Handles non-integer input gracefully (input validation)
// ============================================================
int getValidMenuChoice() {
    int choice;
    while (true) {
        std::cin >> choice;

        // If the user typed something that isn't an integer, clear the error
        if (std::cin.fail()) {
            std::cin.clear();  // clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard bad input
            std::cout << "[ERROR] Please enter a number between 1 and 4: ";
            continue;
        }

        // Discard any extra characters after the number (e.g. "2abc")
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Validate range
        if (choice >= 1 && choice <= 4) {
            return choice;
        }
        std::cout << "[ERROR] Choice must be 1, 2, 3, or 4. Try again: ";
    }
}

// ============================================================
//  main: entry point - creates tracker, runs menu loop
// ============================================================
int main() {
    const std::string INPUT_FILE  = "CS210_Project_Three_Input_File.txt";
    const std::string BACKUP_FILE = "frequency.dat";

    // Create the tracker object - this also auto-generates frequency.dat
    GroceryTracker tracker(INPUT_FILE, BACKUP_FILE);
    std::cout << "[INFO] Purchase data loaded. Backup saved to '" << BACKUP_FILE << "'.\n";

    int choice = 0;

    // Main menu loop - runs until user selects option 4 to exit
    do {
        displayMenu();
        choice = getValidMenuChoice();

        switch (choice) {

            case 1: {
                // Option 1: look up frequency of one specific item
                std::cout << "Enter item name: ";
                std::string itemName;
                std::getline(std::cin, itemName);

                // Trim leading and trailing whitespace from the input
                size_t start = itemName.find_first_not_of(" \t");
                size_t end   = itemName.find_last_not_of(" \t");

                if (start == std::string::npos) {
                    // User entered nothing or only spaces
                    std::cout << "[ERROR] No item name entered. Returning to menu.\n";
                    break;
                }

                itemName = itemName.substr(start, end - start + 1);
                int freq = tracker.getFrequency(itemName);

                if (freq == 0) {
                    std::cout << "'" << itemName << "' was not found in today's records.\n";
                } else {
                    std::cout << itemName << " was purchased " << freq << " time(s) today.\n";
                }
                break;
            }

            case 2:
                // Option 2: print full frequency list
                tracker.printAllFrequencies();
                break;

            case 3:
                // Option 3: print histogram
                tracker.printHistogram();
                break;

            case 4:
                // Option 4: exit the program
                std::cout << "\nThank you for using the Corner Grocer Tracker. Goodbye!\n";
                break;
        }

    } while (choice != 4);  // keep looping until exit is chosen

    return 0;
}
