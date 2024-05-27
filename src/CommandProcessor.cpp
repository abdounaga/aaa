// CommandProcessor.cpp
#include <Arduino.h>
#include "CommandProcessor.h"

const char *words[] = {
    "left",   // Command index 2: Turn on LED 2 for 2 seconds
    "right",  // Command index 3: Turn on LED 1 for 2 seconds
    "_nonsense",
};

void commandQueueProcessorTask(void *param)
{
    CommandProcessor *commandProcessor = (CommandProcessor *)param;
    while (true)
    {
        uint16_t commandIndex = 0;
        if (xQueueReceive(commandProcessor->m_command_queue_handle, &commandIndex, portMAX_DELAY) == pdTRUE)
        {
            commandProcessor->processCommand(commandIndex);
        }
    }
}

void CommandProcessor::processCommand(uint16_t commandIndex)
{
    switch (commandIndex)
    {
        case 2: // Turn on LED 2
            digitalWrite(GPIO_NUM_13, LOW); // Assuming LED 2 is connected to the same pin as the built-in LED
            delay(2000); // Wait for 2 seconds
            digitalWrite(GPIO_NUM_13, HIGH); // Turn off LED 2
            break;
        case 3: // Turn on LED 1
            digitalWrite(GPIO_NUM_12, HIGH); // Assuming LED 1 is connected to the same pin as the built-in LED
            delay(2000); // Wait for 2 seconds
            digitalWrite(GPIO_NUM_12, LOW); // Turn off LED 1
            break;
        default:
            // Handle invalid command index
            break;
    }
}

CommandProcessor::CommandProcessor()
{
    pinMode(GPIO_NUM_13, OUTPUT); 
    pinMode(GPIO_NUM_12,INPUT);// Initialize built-in LED pin
    // Other initialization code...
    // ...
}
void CommandProcessor::queueCommand(uint16_t commandIndex, float best_score)
{
    // unsigned long now = millis();
    if (commandIndex != 5 && commandIndex != -1)
    {
        Serial.printf("***** %ld Detected command %s(%f)\n", millis(), words[commandIndex], best_score);
        if (xQueueSendToBack(m_command_queue_handle, &commandIndex, 0) != pdTRUE)
        {
            Serial.println("No more space for command");
        }
    }
}