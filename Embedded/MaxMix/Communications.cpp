#include "Communications.h"

// Defined in the main file
extern DeviceSettings g_Settings;
extern SessionInfo g_SessionInfo;
extern SessionData g_Sessions[4];
extern uint32_t g_LastMessage;
extern uint32_t g_Now;

//#define TEST_HARNESS

namespace Communications
{
    void Initialize(void)
    {
        Serial.begin(BAUD_RATE);
        Serial.setTimeout(SERIAL_TIMEOUT);
    }

    Command Read(void)
    {
        // Future Notes: Serial.readBytes returns the number of bytes actually read
        // if necessary, we can use that compared against sizeof(T) to validate message
        // was recieved in it's entirity. However, if this happens, we should look at Serial timeout first.
        Command command = Command::NONE;
        if (Serial.available())
        {
            g_LastMessage = g_Now;
            command = (Command)Serial.read();
            if (command == Command::TEST)
                Write(command);
            else if (command == Command::SETTINGS)
                Serial.readBytes((char *)&g_Settings, sizeof(DeviceSettings));
            else if (command == Command::SESSION_INFO)
                Serial.readBytes((char *)&g_SessionInfo, sizeof(SessionInfo));
            else if (command >= Command::CURRENT_SESSION && command <= Command::NEXT_SESSION)
                // SessionIndex follows same ordering as Command.
                Serial.readBytes((char *)&g_Sessions[command - Command::CURRENT_SESSION], sizeof(SessionData));
            else if (command >= Command::VOLUME_CURR_CHANGE && command <= Command::VOLUME_NEXT_CHANGE)
                // SessionIndex follows same ordering as Command.
                Serial.readBytes((char *)&g_Sessions[command - Command::VOLUME_CURR_CHANGE].data, sizeof(VolumeData));
            // Do nothing: DEBUG, NONE, ERROR?
#ifdef TEST_HARNESS
            else if (command == Command::DEBUG)
            {
                Write(Command::SETTINGS);
                Write(Command::SESSION_INFO);
                Write(Command::CURRENT_SESSION);
                Write(Command::ALTERNATE_SESSION);
                Write(Command::PREVIOUS_SESSION);
                Write(Command::NEXT_SESSION);
                Write(Command::VOLUME_CURR_CHANGE);
                Write(Command::VOLUME_ALT_CHANGE);
                Write(Command::VOLUME_PREV_CHANGE);
                Write(Command::VOLUME_NEXT_CHANGE);
            }
#endif
            Write(Command::OK);
        }
        return command;
    }

    void Write(Command command)
    {
        if (command == Command::TEST)
        {
            Serial.write(command);
            Serial.println(F(VERSION));
        }
        else if (command == Command::SESSION_INFO)
        {
            Serial.write(command);
            Serial.write((char *)&g_SessionInfo, sizeof(SessionInfo));
        }
        else if (command == Command::VOLUME_CURR_CHANGE)
        {
            Serial.write(command);
            Serial.write((char *)&g_Sessions[SessionIndex::INDEX_CURRENT].data, sizeof(VolumeData));
        }
        else if (command == Command::VOLUME_ALT_CHANGE)
        {
            Serial.write(command);
            Serial.write((char *)&g_Sessions[SessionIndex::INDEX_ALTERNATE].data, sizeof(VolumeData));
        }
        else if (command == Command::OK)
        {
            Serial.write(command);
        }
        // Do nothing: SETTINGS, CURRENT_SESSION, ALTERNATE_SESSION, PREVIOUS_SESSION, NEXT_SESSION, VOLUME_PREV_CHANGE, VOLUME_NEXT_CHANGE, DEBUG, NONE, ERROR?
#ifdef TEST_HARNESS
        else if (command == Command::SETTINGS)
        {
            Serial.write(command);
            Serial.write((char *)&g_Settings, sizeof(DeviceSettings));
        }
        else if (command == Command::CURRENT_SESSION)
        {
            Serial.write(command);
            Serial.write((char *)&g_Session[SessionIndex::INDEX_CURRENT], sizeof(session_t));
        }
        else if (command == Command::ALTERNATE_SESSION)
        {
            Serial.write(command);
            Serial.write((char *)&g_Session[SessionIndex::INDEX_ALTERNATE], sizeof(session_t));
        }
        else if (command == Command::PREVIOUS_SESSION)
        {
            Serial.write(command);
            Serial.write((char *)&g_Session[SessionIndex::INDEX_PREVIOUS], sizeof(session_t));
        }
        else if (command == Command::NEXT_SESSION)
        {
            Serial.write(command);
            Serial.write((char *)&g_Session[SessionIndex::INDEX_NEXT], sizeof(session_t));
        }
        else if (command == Command::VOLUME_PREV_CHANGE)
        {
            Serial.write(command);
            Serial.write((char *)&g_Sessions[SessionIndex::INDEX_PREVIOUS].data, sizeof(VolumeData));
        }
        else if (command == Command::VOLUME_NEXT_CHANGE)
        {
            Serial.write(command);
            Serial.write((char *)&g_Sessions[SessionIndex::INDEX_NEXT].data, sizeof(VolumeData));
        }
        // Do nothing: DEBUG, NONE, ERROR?
#endif
        // Send buffered data
        Serial.flush();
    }
} // namespace Communications