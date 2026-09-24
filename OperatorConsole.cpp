#include "OperatorConsole.h"
#include "OperatorCommand.h"

#include <iostream>

OperatorConsole::OperatorConsole(const std::string& operatorName)
    : operatorName(operatorName)
{
}

OperatorConsole::~OperatorConsole()
{
    for (OperatorCommand* command : pending)
    {
        delete command;
    }
    for (OperatorCommand* command : history)
    {
        delete command;
    }
}

void OperatorConsole::queueCommand(OperatorCommand* command)
{
    if (command == nullptr)
    {
        std::cout << "[Console " << operatorName << "] refused to queue an empty command" << std::endl;
        return;
    }
    pending.push_back(command);
    std::cout << "[Console " << operatorName << "] queued: " << command->describe() << std::endl;
}

void OperatorConsole::executePending()
{
    std::vector<OperatorCommand*> batch;
    batch.swap(pending);

    for (OperatorCommand* command : batch)
    {
        std::cout << "[Console " << operatorName << "] executing: " << command->describe() << std::endl;
        if (command->execute())
        {
            history.push_back(command);
        }
        else
        {
            std::cout << "[Console " << operatorName << "] FAILED: " << command->describe() << " (discarded)" << std::endl;
            delete command;
        }
    }
}

bool OperatorConsole::cancelLast()
{
    if (history.empty())
    {
        std::cout << "[Console " << operatorName << "] nothing to cancel" << std::endl;
        return false;
    }

    OperatorCommand* command = history.back();
    history.pop_back();
    std::cout << "[Console " << operatorName << "] cancelling: " << command->describe() << std::endl;

    bool cancelled = command->undo();
    if (cancelled)
    {
        std::cout << "[Console " << operatorName << "] cancelled: " << command->describe() << std::endl;
    }
    else
    {
        std::cout << "[Console " << operatorName << "] could not cancel: " << command->describe()
                  << " (remains in effect, removed from cancel history)" << std::endl;
    }
    delete command;
    return cancelled;
}
