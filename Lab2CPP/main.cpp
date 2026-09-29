#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "stack.h"

using namespace std;

struct ProcessorItem
{
    int value;
    bool return_address;
};

int register_index(const string& name)
{
    if (name == "A") return 0;
    if (name == "B") return 1;
    if (name == "C") return 2;
    if (name == "D") return 3;
    return -1;
}

bool parse_value(const string& text, const int registers[], int& value)
{
    const int index = register_index(text);
    if (index >= 0)
    {
        value = registers[index];
        return true;
    }

    istringstream stream(text);
    char extra;
    if (!(stream >> value)) return false;
    return !(stream >> extra);
}

bool get_item(Stack* values, Stack* types, ProcessorItem& item)
{
    if (stack_empty(values) || stack_empty(types)) return false;
    item.value = stack_get(values);
    item.return_address = stack_get(types) != 0;
    return true;
}

void remove_item(Stack* values, Stack* types)
{
    stack_pop(values);
    stack_pop(types);
}

void put_item(Stack* values, Stack* types, int value, bool return_address)
{
    stack_push(values, value);
    stack_push(types, return_address ? 1 : 0);
}

int main(int argc, char* argv[])
{
    ifstream input_file;
    istream* input = &cin;

    if (argc == 2)
    {
        input_file.open(argv[1]);
        if (!input_file.is_open())
        {
            cerr << "Cannot open input file\n";
            return 1;
        }
        input = &input_file;
    }
    vector<string> lines;
    string line;
    while (getline(*input, line))
    {
        lines.push_back(line);
    }

    Stack* values = stack_create();
    Stack* types = stack_create();

    int registers[4] = { 0, 0, 0, 0 };
    int instruction = 0;

    while (instruction >= 0 && instruction < static_cast<int>(lines.size()))
    {
        istringstream stream(lines[instruction]);
        string command;

        if (!(stream >> command))
        {
            ++instruction;
            continue;
        }

        if (command == "push")
        {
            string argument;
            string extra;

            if (!(stream >> argument) || (stream >> extra))
            {
                cout << "BAD COMMAND\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            int value = 0;
            if (!parse_value(argument, registers, value))
            {
                cout << "BAD VALUE\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            put_item(values, types, value, false);
            ++instruction;
        }
        else if (command == "pop")
        {
            string argument;
            string extra;

            if (!(stream >> argument) || (stream >> extra))
            {
                cout << "BAD COMMAND\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            const int index = register_index(argument);
            if (index < 0)
            {
                cout << "BAD REGISTER\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            ProcessorItem item;
            if (!get_item(values, types, item))
            {
                cout << "BAD POP\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            if (item.return_address)
            {
                cout << "BAD POP\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            registers[index] = item.value;
            remove_item(values, types);
            ++instruction;
        }
        else if (command == "add" || command == "sub" || command == "mul")
        {
            string extra;
            if (stream >> extra)
            {
                cout << "BAD COMMAND\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            ProcessorItem first;
            ProcessorItem second;

            if (!get_item(values, types, first) || first.return_address)
            {
                cout << "BAD OPERATION\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }
            remove_item(values, types);

            if (!get_item(values, types, second) || second.return_address)
            {
                cout << "BAD OPERATION\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }
            remove_item(values, types);

            int result = 0;
            if (command == "add")
                result = second.value + first.value;
            else if (command == "sub")
                result = second.value - first.value;
            else
                result = second.value * first.value;

            put_item(values, types, result, false);
            ++instruction;
        }
        else if (command == "call")
        {
            string extra;
            if (stream >> extra)
            {
                cout << "BAD COMMAND\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            put_item(values, types, instruction + 1, true);
            ++instruction;
        }
        else if (command == "ret")
        {
            string extra;
            if (stream >> extra)
            {
                cout << "BAD COMMAND\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            ProcessorItem item;
            if (!get_item(values, types, item) || !item.return_address)
            {
                cout << "BAD RET\n";
                stack_delete(values);
                stack_delete(types);
                return 0;
            }

            remove_item(values, types);
            instruction = item.value + 1;
        }
        else
        {
            cout << "BAD COMMAND\n";
            stack_delete(values);
            stack_delete(types);
            return 0;
        }
    }

    cout << "A = " << registers[0] << '\n';
    cout << "B = " << registers[1] << '\n';
    cout << "C = " << registers[2] << '\n';
    cout << "D = " << registers[3] << '\n';

    stack_delete(values);
    stack_delete(types);
    return 0;
}