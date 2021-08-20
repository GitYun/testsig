#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <iostream>

#include <sigslot/signal.hpp>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
#include "lightmodbus/lightmodbus.h"
#include "lightmodbus/master.h"

#ifdef _WIN32
#include <windows.h>
#define imsleep(microsecond) Sleep(microsecond) // ms
#else
#include <unistd.h>
#define imsleep(microsecond) usleep(1000 * microsecond) // ms
#endif

#include <vector>
using namespace itas109;
using namespace std;


//For Linux
//g++ CSerialPortDemoNoGui.cpp SerialPort.cpp SerialPortBase.cpp SerialPortUnixBase.cpp -lpthread -o CSerialPortDemoNoGui
//g++ CSerialPortDemoNoGui.cpp SerialPortInfo.cpp SerialPortInfoBase.cpp SerialPortInfoUnixBase.cpp SerialPort.cpp SerialPortBase.cpp SerialPortUnixBase.cpp -lpthread -o CSerialPortDemoNoGui

class mySlot
{
public:
    mySlot(CSerialPort * sp)
    {
	    recLen = -1;
        p_sp = sp;
		memset(str, 0, sizeof(str));
		modbusMasterInit(&m_mb_master);
    };

	void OnSendMessage()
	{
		//read
        // recLen = p_sp->readAllData(str + countRead);
		recLen = p_sp->readAllData((char *)&m_mb_master.response.frame[m_mb_master.response.length]);


		if(recLen > 0)
		{
			// countRead += recLen;
			// str[countRead] = '\0';

			m_mb_master.response.length += recLen;
			m_mb_master.response.frame[m_mb_master.response.length] = '\0';

			// if(countRead >= 7)
			if (m_mb_master.response.length >= 7)
			{
				// m_echo(str);
				// countRead = 0;

				ModbusError err = modbusParseResponse(&m_mb_master);
				m_echo((char *)m_mb_master.response.frame);
				m_mb_master.response.length = 0;
			}
		}
	};

private:
	mySlot(){};

private:
    CSerialPort * p_sp;

	char str[1024];
	int recLen;
    int countRead = 0;

public:
	sigslot::signal<std::string> m_echo;
	ModbusMaster m_mb_master;
};

#endif