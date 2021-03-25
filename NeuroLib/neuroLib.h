#pragma once

#ifdef __cplusplus
EXTERN_C_START
#endif

/**
* создание нейронной нести нейронной сети.
* @param fCorrercion - функция активации нейрона по-умолчанию.
* @param fCorrercion - функция корректировки нейрона по-умолчанию.
* @param fKf - коэффициент обучения нейронной сети.
* @return - указатель на нейронную сеть. при ошибки вернет nullptr. 
* GetlastError - ERROR_NOT_ENOUGH_MEMORY
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
neuro::CNeuro* createNeuro(
	_In_ neuro::fMathFuncDelegate fActivation,
	_In_ neuro::fMathFuncDelegate fCorrercion,
	_In_ DOUBLE fKf);

/**
* удалить нейронную сеть.
* @param pNeuro - указатель на нейронную сеть.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
VOID deleteNeuro(
	_Inout_ neuro::CNeuro* pNeuro);

/**
* добавить слой нейронов в нейронную сеть.
* @param pNeuro - указатель на нейронную сеть.
* @param szCountNeuron - количество нейронов в слое.
* @param bShift - признак наличия нейрона смещения. если true то последний нейрон будет нейроном смещения.
* @param fCorrercion - функция активации явная для текущего слоя.
* @param fCorrercion - функция корректировки явная для текущего слоя.
* @return - порядковый номер слоя. при ошибке вернет -1.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_NOT_ENOUGH_MEMORY
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
size_t addLayerNeuro(
	_Inout_ neuro::CNeuro* pNeuro,
	_In_ size_t szCountNeuron,
	_In_ BOOL bShift,
	_In_opt_ neuro::fMathFuncDelegate fActivation,
	_In_opt_ neuro::fMathFuncDelegate fCorrercion);

/**
* сгенерировать рандомные значения весов связи для нейронной сети.
* @param pNeuro - указатель на нейронную сеть.
* @param fMin - минимальное значение.
* @param fMax - максимальное значение.
* @return - успех операции. при ошибке вернет false.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
BOOL randomWeightNeuro(
	_Inout_ neuro::CNeuro* pNeuro,
	_In_ DOUBLE fMin,
	_In_ DOUBLE fMax);

/**
* прогнать значение через нейронную сеть.
* @param pNeuro - указатель на нейронную сеть.
* @param pInput - указатель на входные значения.
* @param countInput - количество входных значений.
* @param pOutput - указатель на выходные значения.
* @param countOutput - количество выходных значений.
* @return - результат прогона. при ошибки вернет nullptr.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_NOT_ENOUGH_MEMORY
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
DOUBLE* calcNeuro(
	_Inout_ neuro::CNeuro* pNeuro,
	_In_reads_(countInput) DOUBLE* pInput,
	_In_ size_t countInput,
	_Out_writes_all_(countOutput) DOUBLE* pOutput,
	_In_ size_t countOutput);

/**
* получить ошибку нейронной сети
* @param pNeuro - указатель на нейронную сеть.
* @param bCorrect - признак корректировки весов связи сразу после получения ошибки.
* @param pInput - указатель на входные значения.
* @param countInput - количество входных значений.
* @param pOutput - указатель на выходные значения.
* @param countOutput - количество выходных значений.
* @return - отклонение значений от результата. при ошибки вернет nullptr.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_NOT_ENOUGH_MEMORY
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
DOUBLE* calcErrorNeuro(
	_Inout_ neuro::CNeuro* pNeuro,
	_In_ BOOL bCorrect,
	_In_reads_(countInput) DOUBLE* pInput,
	_In_ size_t countInput,
	_Out_writes_all_(countOutput) DOUBLE* pOutput,
	_In_ size_t countOutput);

/**
* скорректировать веса связи нейронной сети, исходя из ошибки.
* @param pNeuro - указатель на нейронную сеть.
* @return - успех операции. при ошибке вернет false.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
BOOL correctWeightNeuro(
	_Inout_ neuro::CNeuro* pNeuro);

/**
* получить ошибку результирующего ответа прогона нейронной сети.
* @param pNeuro - указатель на нейронную сеть.
* @return - ошибка результирующего ответа прогона. при ошибки вернет отрицательное значение.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
DOUBLE getErrorNeuro(
	_Inout_ neuro::CNeuro* pNeuro);

/**
* получить ошибку всего прогона нейронной сети.
* @param pNeuro - указатель на нейронную сеть.
* @return - ошибка всего прогона. при ошибки вернет отрицательное значение.
* GetlastError - ERROR_INVALID_PARAMETER
* GetlastError - ERROR_INVALID_FUNCTION
*/
NEUROLIB_EXPORT
DOUBLE getFullErrorNeuro(
	_Inout_ neuro::CNeuro* pNeuro);

#ifdef __cplusplus
EXTERN_C_END
#endif
