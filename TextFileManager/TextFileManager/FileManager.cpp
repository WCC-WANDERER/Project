#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <windows.h>


// Represents a file in the system
class File {

    std::string name;
    std::string path;
    std::string format;

public:
    File(const std::string& path) {
        this->path = path;
        this->name = std::filesystem::path(path).filename().string();
        this->format = std::filesystem::path(path).extension().string();
    }

    // Function to delete temporary files by setting attributes to normal
    void deleteTemporaryFile(const std::string& outputFilePath) {

        // Set file attributes to normal (if it exists)
        if (std::filesystem::exists(outputFilePath)) {

            if (!SetFileAttributes(std::filesystem::path(outputFilePath).c_str(), FILE_ATTRIBUTE_NORMAL)) {

                DWORD error = GetLastError();
                std::cerr << "Failed to set file attributes to normal for: " << outputFilePath
                    << ". Error Code: " << error << std::endl;
            }

            // Delete the original file
            try {
                std::filesystem::remove(outputFilePath);
            }
            catch (const std::exception& ex) {
                std::cerr << "Error deleting file: " << ex.what() << std::endl;
                return;
            }
        }
        else {
            std::cerr << "File does not exist: " << outputFilePath << std::endl;
            return;
        }
    }

    // Function to convert .docx, or .odt to .txt using Pandoc's command
    std::string convertToTxt(const std::string& inputFilePath, const std::string& outputDir) {

        // Convert input file path to absolute path
        std::filesystem::path inputAbsPath = std::filesystem::absolute(inputFilePath);

        // Convert outputDir to a std::filesystem::path
        std::filesystem::path outputDirPath(outputDir);

        // Extract the file name without extension
        std::filesystem::path inputPath(inputAbsPath);
        std::string outputFileName = inputPath.stem().string() + ".txt";

        // Combine the output directory and the output file name
        std::filesystem::path outputFilePath = outputDirPath / outputFileName;

        // Construct the Pandoc command with --wrap=none
        std::string command = "pandoc --to=plain+smart --wrap=none \"" + inputAbsPath.string() + "\" -o \"" + outputFilePath.string() + "\"";

        // Execute the command
        int result = system(command.c_str());

        // Check for errors during conversion
        if (result != 0) {
            std::cerr << "Error converting file: " << inputAbsPath.string() << std::endl;
            throw std::runtime_error("Failed to convert file: " + inputAbsPath.string());
        }
        else {
            std::cout << "File successfully converted to: " << outputFilePath.string() << std::endl;
        }

        // Temporary file for cleaned content
        const std::string tempFilePath = "temp_output.txt";

        // Open the original output file and the temporary file
        std::ifstream inputFile(outputFilePath);
        std::ofstream tempFile(tempFilePath);

        if (!inputFile.is_open() || !tempFile.is_open()) {
            std::cerr << "Error: Unable to open files for processing." << std::endl;
            throw std::runtime_error("File not found: " + outputFilePath.string() + " or " + tempFilePath);
        }

        // Remove empty lines from the file
        std::string line;
        while (std::getline(inputFile, line)) {
            if (!line.empty()) {  // Only write non-empty lines
                tempFile << line << "\n";
            }
        }

        // Close the files
        inputFile.close();
        tempFile.close();

        deleteTemporaryFile(outputFilePath.string());

        // Rename the temporary file to the original file name
        try {
            std::filesystem::rename(tempFilePath, outputFilePath);
        }
        catch (const std::exception& ex) {
            std::cerr << "Error renaming temporary file: " << ex.what() << std::endl;
        }

        // Return the output file path as a string
        return outputFilePath.string();

    }

    std::string getPath() { return path; }
    std::string getName() { return name; }
    std::string getExtension() { return format; }

};

// Represents a difference between two files
class Difference {

    int lineNumber;
    std::string firstFileContent;
    std::string secondFileContent;

public:
    Difference(int lineNumber, const std::string& firstFileContent, const std::string& secondFileContent) {
        this->lineNumber = lineNumber;
        this->firstFileContent = firstFileContent;
        this->secondFileContent = secondFileContent;
    }      

    // Methods used to show the list of differences in terms of line number and the corresponding content
    int getLineNumber() const { return lineNumber; }
    const std::string& getFirstFileContent() const { return firstFileContent; }
    const std::string& getSecondFileContent() const { return secondFileContent; }
};

// Responsible for comparing two input files
class Comparator {

    std::vector<Difference> differences;

public:

    void compareFilesContent(const std::string file1Path, const std::string file2Path, std::string& file1Str, std::string& file2Str) {

        // Open files
        std::ifstream file1(file1Path);
        std::ifstream file2(file2Path);
        if (!file1) throw std::runtime_error("File not found: " + file1Path);
        if (!file2) throw std::runtime_error("File not found: " + file2Path);

        //Prepare content buffers and differences
        std::ostringstream file1Content, file2Content;
        differences.clear();
        std::string line1, line2;
        int lineNumber = 0;

        while (true)
        {
            bool gotLine1 = static_cast<bool>(std::getline(file1, line1));
            bool gotLine2 = static_cast<bool>(std::getline(file2, line2));
            lineNumber++;

            if (!gotLine1 && !gotLine2)
            {
                break;
            }

            std::string currentLine1 = gotLine1 ? line1 : "";
            std::string currentLine2 = gotLine2 ? line2 : "";

            //Compare and store
            if (currentLine1 != currentLine2) {
                differences.emplace_back(lineNumber, currentLine1, currentLine2);
            }

            if (gotLine1) file1Content << line1 << "\n";
            if (gotLine2) file2Content << line2 << "\n";
        }

        // Close the files
        file1.close();
        file2.close();

        file1Str = file1Content.str();
        file2Str = file2Content.str();

    }

    std::vector<Difference> getDifferences() { return differences; }
};


//// Main Program
//int main(int argc, char * argv[]) {
//
// Not used
// 
//}
