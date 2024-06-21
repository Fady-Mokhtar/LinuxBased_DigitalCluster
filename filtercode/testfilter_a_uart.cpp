#include <windows.h>
#include <iostream>
#include "filter.h"
HANDLE setupSerialPort(const char* portName) {
    // Open the serial port
    HANDLE hSerial = CreateFile(portName,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    // Set parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting state" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting state" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Set timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting timeouts" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

bool readSerialPort(HANDLE hSerial, char* buffer, unsigned int buf_size) {
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(hSerial, NULL, NULL);
    if (!ReadFile(hSerial, buffer, buf_size, &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port" << std::endl;
        return false;
    }

    return true;
}




int main() {
    const char* portName = "\\\\.\\COM11";
    HANDLE hSerial = setupSerialPort(portName);

    if (hSerial == INVALID_HANDLE_VALUE) {
        return 1;
    }

    char buffer[256];
    MovingAverageFilter filter(3); // For smoothing the data

    while (true) {
        if (readSerialPort(hSerial, buffer, sizeof(buffer))) {
            int reading = atoi(buffer); // Assuming the reading is in ASCII format
            int smoothedReading = (int)filter.filter(reading);
            std::cout << "Actual Reading: " << reading << '\t';
            std::cout << "Smoothed Reading: " << smoothedReading << std::endl;
        }
        buffer[0] = 0;
        Sleep(100); // Adjust the delay as needed
    }

    CloseHandle(hSerial);
    return 0;
}
