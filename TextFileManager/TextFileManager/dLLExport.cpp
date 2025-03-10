#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <windows.h>
#include "FileManager.cpp"


extern "C" {


    struct FileComparisonResult {
        char* file1ReturnContent;
        char* file2ReturnContent;
        char* differences;
    };


    __declspec(dllexport) FileComparisonResult CompareFiles(const char* file1Path, const char* file2Path) {

        FileComparisonResult result;
        std::string file1Str, file2Str;
        std::string file1ConvertedPath, file2ConvertedPath;
        bool isFile1Converted = false;
        bool isFile2Converted = false;
        File firstFile(file1Path);
        File secondFile(file2Path);
        std::string outputDir = std::filesystem::current_path().string();

        try {

            // Convert file1
            file1ConvertedPath = firstFile.getPath();
            if (firstFile.getExtension() != ".txt") {
                file1ConvertedPath = firstFile.convertToTxt(file1ConvertedPath, outputDir);
                isFile1Converted = true;
            }

            // Convert file2
            file2ConvertedPath = secondFile.getPath();
            if (secondFile.getExtension() != ".txt") {
                file2ConvertedPath = secondFile.convertToTxt(file2ConvertedPath, outputDir);
                isFile2Converted = true;
            }

            Comparator comparator;
            comparator.compareFilesContent(file1ConvertedPath, file2ConvertedPath, file1Str, file2Str);

            result.file1ReturnContent = new char[file1Str.size() + 1];
            std::copy(file1Str.begin(), file1Str.end(), result.file1ReturnContent);
            result.file1ReturnContent[file1Str.size()] = '\0';

            result.file2ReturnContent = new char[file2Str.size() + 1];
            std::copy(file2Str.begin(), file2Str.end(), result.file2ReturnContent);
            result.file2ReturnContent[file2Str.size()] = '\0';

            //Store differences
            std::ostringstream diffStream;
            for (const auto& diff : comparator.getDifferences()) {
                diffStream << "Line " << diff.getLineNumber()
                    << ": File1 -> " << diff.getFirstFileContent()
                    << ", File2 -> " << diff.getSecondFileContent() << "\n";
            }

            std::string diffStr = diffStream.str();
            result.differences = new char[diffStr.size() + 1];
            std::copy(diffStr.begin(), diffStr.end(), result.differences);
            result.differences[diffStr.size()] = '\0';

            //Cleanup temporary files
            if (isFile1Converted) firstFile.deleteTemporaryFile(file1ConvertedPath);
            if (isFile2Converted) secondFile.deleteTemporaryFile(file2ConvertedPath);

            return result;

        }
        catch (const std::exception& ex) {
            // Cleanup on error
            if (isFile1Converted) firstFile.deleteTemporaryFile(file1ConvertedPath);
            if (isFile2Converted) secondFile.deleteTemporaryFile(file2ConvertedPath);
            std::cerr << "Error: " << ex.what() << std::endl;
            return {};
        }
    }

    // Function to free the memory allocated for the result string
    __declspec(dllexport) void FreeMemory(char* ptr) {
        if (ptr != nullptr) {
            delete[] ptr;  // Free the memory allocated for the result string
        }
    }

}


