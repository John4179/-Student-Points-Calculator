#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>

class Person {
private:
    std::string firstName;
    std::string surname;
    std::vector<double> homeworkResults;
    double examResult;
    double finalGrade;

public:
    // Initializes a person with a given first name and surname
    Person(const std::string& firstName, const std::string& surname)
        : firstName(firstName), surname(surname), examResult(0.0), finalGrade(0.0) {}

    // Copy constructor for creating a copy of a Person object
    Person(const Person& other)
        : firstName(other.firstName), surname(other.surname),
          homeworkResults(other.homeworkResults), examResult(other.examResult),
          finalGrade(other.finalGrade) {}

    // Assignment operator to handle resource management during assignment
    Person& operator=(const Person& other) {
        if (this == &other) return *this;  // Check for self-assignment
        firstName = other.firstName;
        surname = other.surname;
        homeworkResults = other.homeworkResults;
        examResult = other.examResult;
        finalGrade = other.finalGrade;
        return *this;
    }

    // Destructor
    ~Person() {}

    // Method to set homework and exam scores
    void setScores(const std::vector<double>& hwResults, double exam) {
        homeworkResults = hwResults;
        examResult = exam;
    }

    // Overloaded output operator to display a person's information
    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << std::setw(10) << person.firstName
           << std::setw(15) << person.surname
           << std::setw(15) << std::fixed << std::setprecision(2) << person.finalGrade;
        return os;
    }

    // Method to calculate the final grade using either average or median of homework scores
    void calculateFinalGrade(bool useAverage) {
        double hwScore = 0.0;

        // Calculate the average or median based on user's choice
        if (useAverage) {
            hwScore = std::accumulate(homeworkResults.begin(), homeworkResults.end(), 0.0) / homeworkResults.size();
        } else {
            std::sort(homeworkResults.begin(), homeworkResults.end());
            if (homeworkResults.size() % 2 == 0) {
                hwScore = (homeworkResults[homeworkResults.size() / 2 - 1] +
                           homeworkResults[homeworkResults.size() / 2]) / 2.0;
            } else {
                hwScore = homeworkResults[homeworkResults.size() / 2];
            }
        }

        // Calculate final grade with weighted average
        finalGrade = 0.4 * hwScore + 0.6 * examResult;
    }
};

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    // Open the input file and verify it was successfully opened
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return 1;
    }

    std::vector<Person> persons;
    std::string line;

    // Skip the header line
    std::getline(infile, line);

    // Process each line of data
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string firstName, surname;
        double score;
        std::vector<double> homeworkScores;

        // Read first name and surname
        iss >> firstName >> surname;

        // Read homework scores
        for (int i = 0; i < 5; ++i) {  // Adjust this if the number of homework scores varies
            iss >> score;
            homeworkScores.push_back(score);
        }

        // Read the exam score
        double examScore;
        iss >> examScore;

        // Create a Person object and set scores
        Person person(firstName, surname);
        person.setScores(homeworkScores, examScore);
        persons.push_back(person);
    }

    // Close the file after reading
    infile.close();

    // Prompt user for grade calculation method
    char choice;
    std::cout << "Calculate final grade using (A)verage or (M)edian of homework scores? (A/M): ";
    std::cin >> choice;
    bool useAverage = (choice == 'A' || choice == 'a');

    // Calculate final grades for all persons
    for (auto& person : persons) {
        person.calculateFinalGrade(useAverage);
    }

    // Display final grades in a formatted table
    std::cout << std::setw(10) << "Name"
              << std::setw(15) << "Surname"
              << std::setw(15) << "Final_Point(Aver.)"
              << "\n-------------------------------------------\n";

    for (const auto& person : persons) {
        std::cout << person << std::endl;
    }

    return 0;
}
