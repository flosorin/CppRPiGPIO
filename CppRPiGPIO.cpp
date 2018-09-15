#include "CppRPiGPIO.h"

#define GPIO_FOLDER "/sys/class/gpio/"

RPiGPIO::RPiGPIO(const uint8_t& gpioNumber) : gpioNumber_(gpioNumber)
{
    this->GetPermissions();
    usleep(1);
    this->ExportManager(EXPORT);
    usleep(1);
}

RPiGPIO::~RPiGPIO()
{
    this->ExportManager(UNEXPORT);
}

void RPiGPIO::GetPermissions()
{
    std::system("sudo chgrp gpio /sys/class/gpio/export");
    std::system("sudo chgrp gpio /sys/class/gpio/unexport");
    std::system("sudo chmod 775 /sys/class/gpio/export");
    std::system("sudo chmod 775 /sys/class/gpio/unexport");
}

void RPiGPIO::ExportManager(const ExportCommand& command)
{
    // Recover command
    const std::string commandsList[] = { "export", "unexport" };
    const std::string& commandStr = commandsList[command];
    
    // Construct full system command and execute it
    const std::string& fileName = GPIO_FOLDER + commandStr;
    const std::string& fullCommand = std::string("echo ") + std::to_string(this->gpioNumber_) + std::string(" > ") + fileName;
    std::system(fullCommand.c_str());
    
    // Change permissions for the exported GPIO
    if (command == EXPORT)
    {
        usleep(1);
        const std::string& gpioPath = GPIO_FOLDER + std::string("gpio") + std::to_string(this->gpioNumber_);
        const std::string& chgrpCmd = std::string("sudo chgrp -HR gpio ") + gpioPath;
        const std::string& chmodCmd = std::string("sudo chmod -R 775 ") + gpioPath;
        std::system(chgrpCmd.c_str());
        std::system(chmodCmd.c_str());
        usleep(1);
    }
}

void RPiGPIO::SetDirection(const GPIODirection& direction)
{
    // Recover direction
    const std::string directionsList[] = { "in", "out" };
    const std::string& directionStr = directionsList[direction];
    
    // Construct full system command and execute it
    const std::string& fileName = GPIO_FOLDER + std::string("gpio") + std::to_string(this->gpioNumber_) + std::string("/direction");
    const std::string& fullCommand = std::string("echo \"") + directionStr + std::string("\" > ") + fileName;
    std::system(fullCommand.c_str());
}

void RPiGPIO::SetValue(const bool& value)
{
    // Recover command
    const std::string& valueStr = value ? "1" : "0";
    
    // Construct full system command and excecute it
    const std::string& fileName = GPIO_FOLDER + std::string("gpio") + std::to_string(this->gpioNumber_) + std::string("/value");
    const std::string& fullCommand = std::string("echo \"") + valueStr + std::string("\" > ") + fileName;
    std::system(fullCommand.c_str());
}

bool RPiGPIO::GetValue(bool& value)
{
    // Construct file name and command
    const std::string& fileName = GPIO_FOLDER + std::string("gpio") + std::to_string(this->gpioNumber_) + std::string("/value");
    const std::string& fullCommand = std::string("cat ") + fileName;
    
    // Execute the command and recover the result
    FILE * stream;
    char buffer[10];
    stream = popen(fullCommand.c_str(), "r");
    if (stream) 
    {    
        if (fgets(buffer, 10, stream) != NULL) 
        {
           value = (buffer[0] == '1');
        }
        pclose(stream);
        
        return true;
    }

    return false;
}

bool RPiGPIO::GetDirection(GPIODirection& direction)
{
    // Construct file name and command
    const std::string& fileName = GPIO_FOLDER + std::string("gpio") + std::to_string(this->gpioNumber_) + std::string("/direction");
    const std::string& fullCommand = std::string("cat ") + fileName;
    
    // Execute the command and recover the result
    std::string directionStr;
    FILE * stream;
    char buffer[10];
    stream = popen(fullCommand.c_str(), "r");
    if (stream) 
    {    
        if (fgets(buffer, 10, stream) != NULL) 
        {
           directionStr.append(std::string(buffer));
        }
        pclose(stream);
        
        if (directionStr.find("in") != std::string::npos)
        {
            direction = IN;
            return true;
        }
        else if (directionStr.find("out") != std::string::npos)
        {
            direction = OUT;
            return true;
        }
    }

    return false;
}


uint8_t RPiGPIO::GetNumber() 
{
    return this->gpioNumber_;
}
