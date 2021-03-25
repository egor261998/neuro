#pragma once

#include "functions.h"
#include "CNeuroWeight.h"

namespace neuro
{
	/** нейрон */
	class CNeuron
	{
	public:
		/** функция активации нейрона */
		fMathFunc fActivation;

		/** функция корректировки нейрона */
		fMathFunc fCorrercion;

		/** список входных нейронов */
		std::list<std::pair<CNeuron*, CNeuroWeight*>> inputNeurons;

		/** список выходных нейронов */
		std::list<std::pair<CNeuron*, CNeuroWeight*>> outputNeurons;

		/** значение нейрона */
		DOUBLE fValue = 0.f;

		/** ошибка нейрона */
		DOUBLE fError = 0.f;

		/** признак нейрона смещения */
		BOOL bShift;

		/**
		 * конструктор нейрона.
		 * @param fCorrercion - функция активации нейрона.
		 * @param fCorrercion - функция корректировки нейрона.
		 * @param bShift - признак нейрона смещения.
		 */
		CNeuron(
			_In_opt_ fMathFunc fActivation = nullptr,
			_In_opt_ fMathFunc fCorrercion = nullptr,
			_In_opt_ BOOL bShift = false)
		{
			this->bShift = bShift;
			this->fActivation = fActivation;
			this->fCorrercion = fCorrercion;
		}

		/**
		 * добавить вес связи между нейронами.
		 * @param neuronWeights - список всех весов связи нейронной сети.
		 * @param inputNeuron - входной нейрон.
		 * @param outputNeuron - выходной нейрон.
		 * @return - вес связи между нейронами.
		 */
		static CNeuroWeight& addWeight(
			_Inout_ std::list<CNeuroWeight>& neuronWeights,
			_Inout_ CNeuron& inputNeuron,
			_Inout_ CNeuron& outputNeuron)
		{
			// создаем вес связи
			neuronWeights.push_back(CNeuroWeight(inputNeuron, outputNeuron));
			auto& weight = neuronWeights.back();

			// добавляем связь в входной нейрон
			inputNeuron.outputNeurons.push_back(std::pair(&outputNeuron, &weight));

			// добавляем связь в выходной нейрон
			outputNeuron.inputNeurons.push_back(std::pair(&inputNeuron, &weight));

			return weight;
		}
	};
}
