#include "Pipe.h"

using namespace std;

Pipe::Pipe()
{
	this->name = "pipe";
	this->size = 0;
	this->pipe = nullptr;
}

Pipe::~Pipe()
{
	CloseHandle(this->pipe);
}

void Pipe::SetPipeAttributes(const string& name, unsigned int size)
{
	this->name = "\\\\.\\pipe\\" + name;
	this->size = size;
}

unsigned int Pipe::Send(const unsigned char* stream)
{
	DWORD dwActuallyWritten;

	if (!WriteFile(this->pipe, stream, this->size, &dwActuallyWritten, nullptr))
		return 0;
	else
		return dwActuallyWritten;
}

void Pipe::Receive(unsigned char* stream)
{
	DWORD dwActuallyRead;

	if (!ReadFile(this->pipe, stream, this->size, &dwActuallyRead, nullptr))
		return;
}

bool PipeServer::CreateServer()
{
	if (this->size == 0)
		return false;

	this->pipe = CreateNamedPipe(this->name.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, this->size, this->size, 0, nullptr);

	if (this->pipe == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

bool PipeServer::ConnectToServer()
{
	if (this->pipe == INVALID_HANDLE_VALUE)
		return false;

	if (ConnectNamedPipe(this->pipe, nullptr))
		return true;

	return false;
}

bool PipeClient::ConnectToServer()
{
	if (this->name == "pipe")
		return false;

	while (true)
	{
		this->pipe = CreateFile(this->name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

		if (this->pipe != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY)
			return false;

		if (!WaitNamedPipe(this->name.c_str(), 5000))
			return false;
	}

	DWORD dwMode = PIPE_READMODE_MESSAGE;

	if (!SetNamedPipeHandleState(this->pipe, &dwMode, nullptr, nullptr))
		return false;

	return true;
}
