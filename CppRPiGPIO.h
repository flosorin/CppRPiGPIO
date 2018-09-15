#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef enum GPIODirection
{
    IN,
    OUT
} GPIODirection;

typedef enum ExportCommand
{
    EXPORT,
    UNEXPORT
} ExportCommand;

class RPiGPIO
{
public:
    RPiGPIO(const uint8_t& gpioNumber);
    ~RPiGPIO();
    
    void SetDirection(const GPIODirection& direction);
    void SetValue(const bool& value);
    bool GetValue(bool& value);
    bool GetDirection(GPIODirection& direction);
    uint8_t GetNumber();

private:
    void GetPermissions();
    void ExportManager(const ExportCommand& command);
    uint8_t gpioNumber_;
	
};