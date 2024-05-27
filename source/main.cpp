///////////////////////////////////////////////////
//          Desynced: Assault Cube Cheat         //
//                  Build: Beta                  //
//                Developer: Loab                //
//                                               //
//        Desynced is made for Assault Cube      //
//               version 1.2.0.2                 //
//     other versions may have break offsets.    //
///////////////////////////////////////////////////

// Desynced is created for the version 1.2.0.2 of Assault Cube. Using a different version may break the offsets!

// Included Libraries
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "utilities/memory.h"

// Menu Colors
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string blue = "\033[34m";
const std::string magenta = "\033[35m";
const std::string cyan = "\033[36m";
const std::string white = "\033[37m";
const std::string reset = "\033[0m";

// 1.2.0.2 Offsets
constexpr auto localPlayer = 0x10F4F4;
constexpr auto health = 0xF8;
constexpr auto armor = 0xFC;
constexpr auto mtpAmmo = 0x150;
constexpr auto nameOffsetWithinPadding = 0x0225;

// Name Padding
class NamePadding {
public:
    char preNamePadding[549];
    char maxCharactersInName[16];
    char postNamePadding[3609];
};

// Main Entrypoint
int main() {
    Memory memory{ "ac_client.exe" };
    const auto ModuleBaseAddress = memory.GetModuleAddress("ac_client.exe");

    // De-reference the base & define memory addresses relative to the local player
    const auto localPlayerAddress = memory.Read<std::uintptr_t>(ModuleBaseAddress + localPlayer);
    const auto healthAddress = localPlayerAddress + health;
    const auto armorAddress = localPlayerAddress + armor;
    const auto mtpAmmoAddress = localPlayerAddress + mtpAmmo;
    const auto paddingAddress = localPlayerAddress + nameOffsetWithinPadding;

    std::string name = memory.Read<NamePadding>(paddingAddress).preNamePadding;

    std::vector<std::string> logo {
        "  _____  ______  _______     ___   _  _____ ______ _____  ",
        " |  __ \\|  ____|/ ____\\ \\   / / \\ | |/ ____|  ____|  __ \\ ",
        " | |  | | |__  | (___  \\ \\_/ /|  \\| | |    | |__  | |  | |",
        " | |  | |  __|  \\___ \\  \\   / | . ` | |    |  __| | |  | |",
        " | |__| | |____ ____) |  | |  | |\\  | |____| |____| |__| |",
        " |_____/|______|_____/   |_|  |_| \\_|\\_____|______|_____/ "
    };

    std::cout << std::endl;
    for (const auto& line : logo) std::cout << cyan << line << reset << std::endl;
    std::cout << std::endl << yellow << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << reset << std::endl << std::endl;

    std::cout << "Welcome to desynced, " << magenta << name << "\n" << std::endl;
    std::cout << green << "F1: Health Hack" << reset << std::endl;
    std::cout << green << "F2: Armor Hack" << reset << std::endl;
    std::cout << green << "F3: Rifle Ammo Hack" << reset << std::endl << std::endl;

    while (true) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) { memory.Write<int>(healthAddress, 1000); std::cout << red << "Health hacked!" << std::endl; }
        else if (GetAsyncKeyState(VK_F2) & 0x8000) { memory.Write<int>(armorAddress, 1000); std::cout << red << "Armor hacked!" << std::endl; }
        else if (GetAsyncKeyState(VK_F3) & 0x8000) { memory.Write<int>(mtpAmmoAddress, 1000); std::cout << red << "Rifle ammo hacked!" << std::endl; }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
