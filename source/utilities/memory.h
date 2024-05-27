#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <string_view>

class Memory
{
private:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;
public:

	// Memory Constructor - used for finding the processId and opening the handle
	Memory(const std::string_view processName) noexcept
	{
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(processSnapshot, &entry))
		{
			if (!processName.compare(entry.szExeFile))
			{
				processId = entry.th32ProcessID;
				processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
				break;
			}
		}

		// Close the handle
		if (processSnapshot)
			::CloseHandle(processSnapshot);
	}

	// Memory Deconstructor - cleanup by closing the process handle to the game's memory.
	~Memory()
	{
		if (processHandle)
			::CloseHandle(processHandle);
	}

	// Obtain the Base Address of the given Module
	const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
	{
		::MODULEENTRY32 entry = {};
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

		std::uintptr_t moduleBaseAddress = 0;

		while (::Module32Next(processSnapshot, &entry))
		{
			if (!moduleName.compare(entry.szModule))
			{
				moduleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (processSnapshot)
			::CloseHandle(processSnapshot);

		return moduleBaseAddress;
	}

	// Read Process Memory
	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	// Write Process Memory
	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};