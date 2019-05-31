#pragma once
#ifndef SIGNATURES_H
#define SIGNATURES_H

#include "MemorySignature.h"
#include "CSigScan.h"

namespace BL2SDK
{
	namespace Signatures
	{
		MemorySignature GObjects {};
		MemorySignature GNames {};
		MemorySignature ProcessEvent {};
		MemorySignature CallFunction {};
		MemorySignature FrameStep {};
		MemorySignature StaticConstructor {};
		MemorySignature LoadPackage {};
		MemorySignature GMalloc {};
		MemorySignature FNameInit {};
		MemorySignature GetDefaultObject {};
		MemorySignature SetCommand {};


		void InitBL2() {
			GNames = {
				"\x00\x00\x00\x00\x83\x3C\x81\x00\x74\x5C",
				"????xxxxxx",
				10
			};
		}

		void InitTPS() {
			GNames = {
				"\x00\x00\x00\x00\x8B\x04\xB1\x5E\x5D\xC3\x8B\x15",
				"????xxxxxxxx",
				12
			};
		}

		void InitShared() {
			GObjects = {
				"\x00\x00\x00\x00\x8B\x04\xB1\x8B\x40\x08",
				"????xxxxxx",
				10
			};

			SetCommand = {
				"\xFF\x83\xC4\x0C\x85\xC0\x75\x1A\x6A\x01\x8D",
				"xxxxxxxxxxx",
				11
			};

			ProcessEvent = {
				"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x50\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xF0\x53\x56\x57\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x8B\xF1\x89\x75\xEC",
				"xxxxxx????xx????xxxxx????xxxxxxxxxxxxxx????xxxxx",
				48
			};

			CallFunction = {
				"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xF0\x53\x56\x57\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x8B\x7D\x10\x8B\x45\x0C",
				"xxxxxx????xx????xxx????x????xxxxxxxxxxxxxx????xxxxxx",
				52
			};

			FrameStep = {
				"\x55\x8B\xEC\x8B\x41\x18\x0F\xB6\x10",
				"xxxxxxxxx",
				9
			};

			StaticConstructor = {
				"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x10\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x8B\x7D\x08\x8A\x87",
				"xxxxxx????xx????xxxxxxxx????xxxxxxxx????xxxxx",
				45
			};

			LoadPackage = {
				"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x68\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xEC",
				"xxxxxx????xx????xxxxx????xxxxx",
				30
			};

			GMalloc = {
				"\x00\x00\x00\x00\xFF\xD7\x83\xC4\x04\x89\x45\xE4",
				"????xxxxxxxx",
				12
			};

			FNameInit = {
				"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x50\x81\xEC\x9C\x0C",
				"xxxxxx??????xxxxxxxxx",
				21
			};

			GetDefaultObject = {
				"\x55\x8B\xEC\x56\x8B\xF1\x83\xBE\x00\x01\x00\x00\x00\x57\x0F\x85",
				"xxxxxxxx?xxxxxxx",
				16
			};
		}

		void InitSignatures(std::string ExeName) {
			if (ExeName == "Borderlands2")
				InitBL2();
			else
				InitTPS();
			InitShared();
		}
	}
}

#endif