#pragma once

#include "CNeuron.h"

namespace neuro
{
	/** нейронная сеть */
	class CNeuro
	{
	private:
		/** синхронизация */
		class Lock {
		public:
			Lock(std::mutex& mutex) : lock{ mutex } {};
		private:
			std::scoped_lock<std::mutex> lock;
		};

		/** объект синхронизации */
		std::mutex mutex;

		/** слои нейронов */
		std::list<std::pair<std::list<CNeuron>, BOOL>> layers;

		/** веса связи нейронов */
		std::list<CNeuroWeight> neuronWeights;

		/** функция активации нейрона по-умолчанию */
		fMathFunc fActivation;

		/** функция корректировки нейрона по-умолчанию */
		fMathFunc fCorrercion;

		/** коэффициент обучения нейронной сети */
		DOUBLE fKf;
	public:
		/**
		 * конструктор нейронной сети.
		 * @param fCorrercion - функция активации нейрона по-умолчанию.
		 * @param fCorrercion - функция корректировки нейрона по-умолчанию.
		 * @param fKf - коэффициент обучения нейронной сети.
		 */
		CNeuro(
			_In_ fMathFunc fActivation,
			_In_ fMathFunc fCorrercion,
			_In_ DOUBLE fKf);

		/**
		 * добавить слой нейронов.
		 * @param szCountNeuron - количество нейронов в слое.
		 * @param bShift - признак наличия нейрона смещения. если true то последний нейрон будет нейроном смещения. 
		 * @param fCorrercion - функция активации явная для текущего слоя.
		 * @param fCorrercion - функция корректировки явная для текущего слоя.
		 * @return - порядковый номер слоя.
		 */
		size_t addLayer(
			_In_ size_t szCountNeuron,
			_In_ BOOL bShift = false,
			_In_opt_ fMathFunc fActivation = nullptr,
			_In_opt_ fMathFunc fCorrercion = nullptr);
		
		/**
		 * сгенерировать рандомные значения для весов связи.
		 * @param fMin - минимальное значение.
		 * @param fMax - максимальное значение.
		 */
		VOID randomWeight(
			_In_ DOUBLE fMin = 0.f,
			_In_ DOUBLE fMax = 0.f);

		/**
		 * прогнать значение через нейронную сеть.
		 * @param input - входные значения.
		 * @return - результат прогона.
		 */
		std::vector<DOUBLE> calc(
			_In_ std::vector<DOUBLE> input);

		/**
		 * получить ошибку нейронной сети.
		 * @param input - входные значения.
		 * @param bCorrect - признак корректировки весов связи после получения ошибки.
		 * @return - отклонение значений от результата.
		 */
		std::vector<DOUBLE> calcError(
			_In_ std::vector<DOUBLE> input,
			_In_ BOOL bCorrect = false);

		/**
		 * скорректировать веса связи исходя из ошибки.
		 * @param pLock - объект синхронизации.
		 */
		VOID correctWeight(
			_In_opt_ Lock *pLock = nullptr);

		/**
		 * получить ошибку результирующего ответа прогона.
		 * @return - ошибка результирующего ответа прогона.
		 */
		DOUBLE getError();

		/**
		 * получить ошибку всего прогона.
		 * @return - ошибка всего прогона.
		 */
		DOUBLE getFullError();
	};
}
