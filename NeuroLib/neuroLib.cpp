#include "stdafx.h"

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

neuro::CNeuro* createNeuro(
    _In_ neuro::fMathFuncDelegate fActivation,
	_In_ neuro::fMathFuncDelegate fCorrercion,
	_In_ DOUBLE fKf)
{
    neuro::CNeuro* pNeuro = nullptr;
    try
    {
		pNeuro = new neuro::CNeuro(fActivation, fCorrercion, fKf);
        return pNeuro;
    }
    catch (const std::bad_alloc&)
    {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return nullptr;
    }
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return nullptr;
	}
}

VOID deleteNeuro(
	_Inout_ neuro::CNeuro* pNeuro)
{
	if (!pNeuro)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return;
	}

	try
	{
		delete pNeuro;
		return;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return;
	}
}

size_t addLayerNeuro(
    _Inout_ neuro::CNeuro* pNeuro,
    _In_ size_t szCountNeuron,
    _In_ BOOL bShift,
    _In_opt_ neuro::fMathFuncDelegate fActivation,
    _In_opt_ neuro::fMathFuncDelegate fCorrercion)
{
    if (!pNeuro)
    {
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
    }

    try
    {
        return pNeuro->addLayer(szCountNeuron, bShift, 
            fActivation, fCorrercion);
    }
	catch (const std::bad_alloc&)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return -1;
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}
}

BOOL randomWeightNeuro(
    _Inout_ neuro::CNeuro* pNeuro,
    _In_ DOUBLE fMin,
    _In_ DOUBLE fMax)
{
	if (!pNeuro)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	try
	{
		pNeuro->randomWeight(fMin, fMax);
        return TRUE;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return FALSE;
	}
}

DOUBLE* calcNeuro(
    _Inout_ neuro::CNeuro* pNeuro,
    _In_reads_(countInput) DOUBLE* pInput,
    _In_ size_t countInput,
    _Out_writes_all_(countOutput) DOUBLE* pOutput,
    _In_ size_t countOutput)
{
	if (!pNeuro || !pInput || !pOutput)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}

    try
    {
        std::vector<DOUBLE> input;
        input.insert(input.end(), &pInput[0], &pInput[0] + countInput);

        auto output = pNeuro->calc(input);

        if (output.size() != countOutput)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return nullptr;
        }

        return (DOUBLE*)std::memcpy(
            pOutput, &output[0], countOutput * sizeof(DOUBLE));
    }
	catch (const std::bad_alloc&)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return nullptr;
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return nullptr;
	}
}

DOUBLE* calcErrorNeuro(
    _Inout_ neuro::CNeuro* pNeuro,
	_In_ BOOL bCorrect,
    _In_reads_(countInput) DOUBLE* pInput,
    _In_ size_t countInput,
    _Out_writes_all_(countOutput) DOUBLE* pOutput,
    _In_ size_t countOutput)
{
	if (!pNeuro || !pInput || !pOutput)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}

	try
	{
		std::vector<DOUBLE> input;
		input.insert(input.end(), &pInput[0], &pInput[0] + countInput);

		auto output = pNeuro->calcError(input, bCorrect);

		if (output.size() != countOutput)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return nullptr;
		}

		return (DOUBLE*)std::memcpy(
			pOutput, &output[0], countOutput * sizeof(DOUBLE));
	}
	catch (const std::bad_alloc&)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return nullptr;
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return nullptr;
	}
}

BOOL correctWeightNeuro(
	_Inout_ neuro::CNeuro* pNeuro)
{
	if (!pNeuro)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	try
	{
		pNeuro->correctWeight();
		return TRUE;	
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return FALSE;
	}
}

DOUBLE getErrorNeuro(
	_Inout_ neuro::CNeuro* pNeuro)
{
	if (!pNeuro)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1.f;
	}

	try
	{
		return pNeuro->getError();
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1.f;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1.f;
	}
}

DOUBLE getFullErrorNeuro(
	_Inout_ neuro::CNeuro* pNeuro)
{
	if (!pNeuro)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1.f;
	}

	try
	{
		return pNeuro->getFullError();
	}
	catch (const std::invalid_argument&)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1.f;
	}
	catch (const std::exception&)
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1.f;
	}
}
 