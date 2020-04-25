#pragma once
#include "Programmer.h"

class DecoderProgrammer final : public Programmer
{
public:
	#pragma region Methods
	void run() override;
	#pragma endregion
};