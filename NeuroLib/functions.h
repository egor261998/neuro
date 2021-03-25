#pragma once

#define NEUROLIB_EXPORT __declspec(dllexport)

namespace neuro
{
#ifdef __cplusplus
	EXTERN_C_START
#endif
	/**
	 * прототип математической функции.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	typedef std::function<DOUBLE(
		_In_ DOUBLE x)> fMathFunc;

	/**
	 * делегат математической функции.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	typedef DOUBLE(__stdcall* fMathFuncDelegate)(
		_In_ DOUBLE x);

	/**
	 * логистическая функция активации.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Sigmoid(
		_In_ DOUBLE x)
	{
		return 1.0 / (1.0 + exp(-x));
	}

	/**
	 * логистическая функция корректировки сигмоида.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE SigmoidDx(
		_In_ DOUBLE x)
	{
		return x * (1.0 - x);
	}

	/**
	 * линейная функция активации.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Liner_01(
		_In_ DOUBLE x)
	{
		if (x > 1)
			return 1.0 + 0.01 * (x - 1);

		if (x < 0)
			return 0.01 * x;

		return x;
	}

	/**
	 * линейная функция корректировки.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Liner_01Dx(
		_In_ DOUBLE x)
	{
		if (x > 1)
			return 0.01;

		if (x < 0)
			return 0.01;

		return 1;
	}

	/**
	 * выгнутая тождественная  функция активации.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Bent_identity(
		_In_ DOUBLE x)
	{
		return (sqrt(x * x + 1.0) - 1.0) / 2.0 + x;
	}

	/**
	 * выгнутая тождественная  функция корректировки.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Bent_identityDx(
		_In_ DOUBLE x)
	{
		return x / (2.0 * sqrt(x * x + 1)) + 1;
	}

	/**
	 * тождественная функция активации.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE Identical(
		_In_ DOUBLE x)
	{
		return x;
	}

	/**
	 * тождественная функция корректировки.
	 * @param x - входной параметр вычисляемой функции.
	 * @return - результат выполнения функции.
	 */
	NEUROLIB_EXPORT
	inline DOUBLE IdenticalDx(
		_In_ DOUBLE x)
	{
		return  1.0;
	}

#ifdef __cplusplus
	EXTERN_C_END
#endif
}
