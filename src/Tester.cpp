
#include "Tester.h"


bool Tester::testFilters() {
    // Create a temporary configuration file with different values
    const std::string configFilePath = "./examples/config.txt";
    const std::string backupFilePath = "./examples/config_backup.txt";

    // Check if the config file exists
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Config file does not exist: " << configFilePath << std::endl;
        return false;
    }
    configFile.close();

    // Backup the current config file
    if (std::rename(configFilePath.c_str(), backupFilePath.c_str()) != 0) {
        std::cerr << "Failed to create a backup of the config file" << std::endl;
        return false;
    }

    // Create a new config file with modified values for testing
    std::ofstream config_file(configFilePath);
    if (!config_file.is_open()) {
        std::cerr << "Failed to create test configuration file" << std::endl;
        std::rename(backupFilePath.c_str(), configFilePath.c_str());  // Restore the backup
        return false;
    }

    // Write test configuration
    config_file << "WidthMaximiserFilter true 3.0" << std::endl;
    config_file << "HeightMaximiserFilter false" << std::endl;
    config_file << "StepFilter true 10.0" << std::endl;
    // Add more filters and their values as needed

    config_file.close();

    // Load filters using the test configuration
    MainManager manager;
    std::map<FiltersEnums, std::unique_ptr<Filter>> filters;
    manager.loadFilters(filters, 0);

    // Validate loaded filters against the expected values
    bool success = true;

    if (filters[FiltersEnums::WidthMaximiser]->get_value() != 3.0) {
        std::cerr << "WidthMaximiserFilter parameter is incorrect" << std::endl;
        success = false;
    }

    if (filters[FiltersEnums::HeightMaximiser]->isActive) {
        std::cerr << "HeightMaximiserFilter should be disabled" << std::endl;
        success = false;
    }

    if (filters[FiltersEnums::Step]->get_value() != 10.0) {
        std::cerr << "StepFilter parameter is incorrect" << std::endl;
        success = false;
    }

    if (success) {
        std::cout << "All filters loaded successfully" << std::endl;
    } else {
        std::cerr << "Filter loading failed" << std::endl;
    }

    // Remove the temporary configuration file
    std::remove(configFilePath.c_str());

    // Restore the backup config file
    std::rename(backupFilePath.c_str(), configFilePath.c_str());

    return success;
}

bool Tester::testSpaces(){
    
     MainManager manager;
    bool allTestsPassed = true;

    // Test cases for removeLeadingWhitespaces
    std::vector<std::pair<std::string, std::string>> leadingWhitespaceTests = {
        {"   Hello, World!", "Hello, World!"},
        {"\t\tTabs and spaces", "Tabs and spaces"},
        {"\n\rNew lines", "New lines"},
        {"   \t  \n\r   ", ""}, // Only whitespaces
        {"", ""} // Empty string
    };

    for (const auto& test : leadingWhitespaceTests) {
        std::string input = test.first;
        std::string expectedOutput = test.second;

        std::string result = manager.removeLeadingWhitespaces(input);

        if (result != expectedOutput) {
            allTestsPassed = false;
            std::cout << "removeLeadingWhitespaces test failed:\n";
            std::cout << "Input: \"" << input << "\"\n";
            std::cout << "Expected output: \"" << expectedOutput << "\"\n";
            std::cout << "Actual output: \"" << result << "\"\n\n";
        }
    }

    // Test cases for removeTrailingSpaces
    std::vector<std::pair<std::string, std::string>> trailingWhitespaceTests = {
        {"Hello, World!   ", "Hello, World!"},
        {"Tabs and spaces\t\t", "Tabs and spaces"},
        {"New lines\n\r", "New lines"},
        {"   \t  \n\r   ", ""}, // Only whitespaces
        {"", ""} // Empty string
    };

    for (const auto& test : trailingWhitespaceTests) {
        std::string input = test.first;
        std::string expectedOutput = test.second;

        std::string result = manager.removeTrailingSpaces(input);

        if (result != expectedOutput) {
            allTestsPassed = false;
            std::cout << "removeTrailingSpaces test failed:\n";
            std::cout << "Input: \"" << input << "\"\n";
            std::cout << "Expected output: \"" << expectedOutput << "\"\n";
            std::cout << "Actual output: \"" << result << "\"\n\n";
        }
    }

    if(allTestsPassed) std::cout << "Spaces removal functions work properly\n";

    return allTestsPassed;

}