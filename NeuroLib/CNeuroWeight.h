#pragma once

namespace neuro
{
	class CNeuron;

	/** вес связи между двумя нейронами */
	class CNeuroWeight
	{
	public:
		/** указатель на входной на входной нейрон */
		const CNeuron* pInputNeuron;

		/** указатель на выходной на входной нейрон */
		const CNeuron* pOutputNeuron;

		/** значение веса связи */
		DOUBLE fWeight = 0.f;

		/**
		 * создать связь между двумя нейронами.
		 * @param inputNeuron - входной нейрон.
		 * @param outputNeuron - выходной нейрон.
		 */
		CNeuroWeight(
			_In_ const CNeuron& inputNeuron,
			_In_ const CNeuron& outputNeuron) noexcept
		{
			this->pInputNeuron = &inputNeuron;
			this->pOutputNeuron = &outputNeuron;
		}
	};
}
