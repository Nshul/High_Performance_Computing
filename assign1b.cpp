#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <signal.h>

using namespace std;

void shell_loop();
void shell_parse(string &, vector<string> &);
void exec_cmnd(vector<string> &);
void ctrlCHandler(int a);

int main(int argc, char **argv)
{
    /*
    signal(SIGINT, ctrlCHandler) listens for Ctrl+C press
    and when detected runs ctrlCHandler function
    */
    signal(SIGINT, ctrlCHandler);
    shell_loop();
    return 0;
}

void shell_loop()
{
    /*
    status tells whether to exit out of program or not
    */
    bool status = true;
    do
    {
        string inp_cmnd;
        cout << ">";
        /*
        Taking command input here
        */
        getline(cin, inp_cmnd);
        if (cin.eof())
        {
            // CTRL+D detected here
            cout << "Exiting\n";
            status = false;
        }
        else if (inp_cmnd == "exit")
        {
            // exit leads to Exit out of program
            status = false;
        }
        else if (status && (inp_cmnd.length() != 0))
        {
            vector<string> parsedInput;
            shell_parse(inp_cmnd, parsedInput);
            exec_cmnd(parsedInput);
        }
        /*
        If user simply presses ENTER
        inp_cmnd.length()=0 so status doesn't
        change and the loop continues
        */
    } while (status);
}

void shell_parse(string &inp_cmnd, vector<string> &arr)
{
    int pos = 0;
    /*
        Input Command / String is being parsed into keywords
        i.e. String is being split on ' '
        */
    while ((pos = inp_cmnd.find(' ')) != string::npos)
    {
        arr.push_back(inp_cmnd.substr(0, pos));
        inp_cmnd.erase(0, pos + 1);
    }
    arr.push_back(inp_cmnd);
}

void ctrlCHandler(int a){
    cout<<"\nProcess exited by User\n";
}

void exec_cmnd(vector<string> &parsedInput)
{
    pid_t pid;
    /*
    Checking if given command is run as a Background Process
    */
    bool isBackgroundProcess = parsedInput[parsedInput.size() - 1] == "&";
    switch (pid = fork())
    {
    case -1:
        cout << "Cannot Execute Process Right Now\n";
        break;
    case 0: //Child Process
        char **arg;
        arg = new char *[parsedInput.size()];
        for (int i = 0; i < parsedInput.size(); i++)
        {
            arg[i] = const_cast<char *>(parsedInput[i].c_str());
        }
        
        if (execvp(arg[0], arg) == -1)
        {
            exit(EXIT_FAILURE);
        }
        break;
    default: //Parent Process
        if (!isBackgroundProcess)
        {
            /*
            Since child was initiated as a foreground process
            Parent process waits for the child process to end
            */
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                /*
                CHILD EXITED PROPERLY
                */
            }
            else if (WIFSIGNALED(status))
            {
                cout << "Error and Child Exited with =" << WTERMSIG(status) << " \n";
            }
        }
    }
}