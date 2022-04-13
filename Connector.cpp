#include <vcclr.h>

#using <System.dll>

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::ComponentModel;


class Connector {
private:
    gcroot<Process^> myProcess;
    gcroot<StreamWriter^> myStreamWriter;
public:

    Connector() {
        myProcess = gcnew Process();
    }

    void connectorStart()
    {
        if (myProcess)
        {
            myProcess->StartInfo->FileName = "stockfish.exe";
            myProcess->StartInfo->UseShellExecute = false;
            myProcess->StartInfo->RedirectStandardInput = true;
            myProcess->StartInfo->RedirectStandardOutput = true;
            myProcess->Start();
            myStreamWriter = myProcess->StandardInput;
            System::Threading::Thread::Sleep(500);

            // get the Stockfish creators line
            String^ output = "";
            do {
                output = myProcess->StandardOutput->ReadLine();
            } while (myProcess->StandardOutput->Peek() > 0);
        }
    }
    String^ getNextMove(String^ position) {
        if (myStreamWriter)
        {
            String^ cmd = "position startpos moves ";
            cmd += position;
            myStreamWriter->WriteLine(cmd);
            myStreamWriter->WriteLine("go");
            // wait a few seconds for Stockfish to finish its move
            System::Threading::Thread::Sleep(500);

            // read all lines, the last one should be "bestmove"
            String^ output = "";
            do {
                output = myProcess->StandardOutput->ReadLine();
            } while (myProcess->StandardOutput->Peek() > 0);

            int n = output->IndexOf("bestmove");
            if (n != -1)
                return output->Substring(n + 9, 4);
        }
        String^ error = "error";
        return error;
    }

    void connectorClose() {
        myStreamWriter->WriteLine("quit");
        myStreamWriter->Close();
        myProcess->WaitForExit();
        myProcess->Close();
    }

};