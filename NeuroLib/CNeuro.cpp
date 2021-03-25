#include <Windows.h>
#include <mutex>
#include <functional>

#include "CNeuro.h"

namespace neuro
{
	CNeuro::CNeuro(
		_In_ fMathFunc fActivation,
		_In_ fMathFunc fCorrercion,
		_In_ DOUBLE fKf)
	{
		this->fActivation = fActivation;
		this->fCorrercion = fCorrercion;
		this->fKf = fKf;
	}

	size_t CNeuro::addLayer(
		_In_ size_t szCountNeuron,
		_In_ BOOL bShift,
		_In_opt_ fMathFunc fActivation,
		_In_opt_ fMathFunc fCorrercion)
	{
		if (szCountNeuron < 1)
			throw std::invalid_argument("szCountNeuron < 1");

		Lock lock(mutex);

		if (layers.size() > 0 && !fActivation && !(fActivation = this->fActivation))
			throw std::invalid_argument("fActivation = nullptr");

		if (!fCorrercion)
			fCorrercion = this->fCorrercion;

		auto szCurrentSize = layers.size();
		layers.resize(++szCurrentSize);

		if (szCurrentSize == 1)
		{
			//это первый слой, связи не нужны
			auto& backLayer = layers.back();
			
			//добавляем все нейроны
			for (DWORD i = 0; i < szCountNeuron; i++)
				backLayer.first.push_back(CNeuron(fActivation, fCorrercion));

			//устанавливаем признак нейрона смещения
			backLayer.second = bShift;

			//нейрон смещения всегда равен 1
			if (bShift)
			{
				auto& backNeuron = backLayer.first.back();
				backNeuron.bShift = TRUE;
				backNeuron.fValue = 1.f;
			}
		}
		else
		{
			//впереди есть еще слои
			auto& parenLayer = *----layers.end();
			auto& childLayer = layers.back();
			
			//добавляем все нейроны
			for (DWORD i = 0; i < szCountNeuron; i++)
			{
				childLayer.first.push_back(CNeuron(fActivation, fCorrercion));
				
				auto& childNeuron = childLayer.first.back();

				//добавляем связи для всех нейронов, кроме нейрона смещения,
				//у него нет входящих связей
				for (auto& parentNeuron : parenLayer.first)
					if(!bShift || (bShift && (szCountNeuron - 1) != childLayer.first.size()))
						CNeuron::addWeight(neuronWeights, parentNeuron, childNeuron);
			}

			//устанавливаем признак нейрона смещения
			childLayer.second = bShift;

			//нейрон смещения всегда равен 1
			if (bShift)
			{
				auto& backNeuron = childLayer.first.back();
				backNeuron.bShift = TRUE;
				backNeuron.fValue = 1.f;
			}
		}

		//возвращаем порядковый номер слоя
		return szCurrentSize - 1;
	}

	VOID CNeuro::randomWeight(
		_In_ DOUBLE fMin,
		_In_ DOUBLE fMax)
	{
		std::srand((DWORD)time(NULL));

		auto randRange = [&fMin, &fMax]() 
		{ 
			return (double)rand() / (double)RAND_MAX * (fMax - fMin) + fMin;
		};

		Lock lock(mutex);

		for (auto& weight : neuronWeights)
			weight.fWeight = randRange();
	}

	std::vector<DOUBLE> CNeuro::calc(
		_In_ std::vector<DOUBLE> input)
	{
		Lock lock(mutex);

		if (layers.size() <= 0)
			throw std::invalid_argument("layers <= 0");

		auto& inputLayer = layers.front();
		auto sizeInputLayer = inputLayer.first.size() + (inputLayer.second ? -1 : 0);

		if(input.size() != sizeInputLayer)
			throw std::invalid_argument("input.size() != input");

		//присвоим значения входному слою
		auto itInput = inputLayer.first.begin();
		for (DWORD i = 0; i < sizeInputLayer; i++, itInput++)
			itInput->fValue = input[i];

		for (auto itChildLayer = ++layers.begin(); 
			itChildLayer != layers.end();
			itChildLayer++)
		{
			for (auto& childNeuron : itChildLayer->first)
			{
				DOUBLE fValue = 0.f;

				for (auto [pParentNeuron, pWeight] : childNeuron.inputNeurons)
					fValue += pParentNeuron->fValue * pWeight->fWeight;

				if(!childNeuron.fActivation)
					throw std::invalid_argument("fActivation = nullptr");

				childNeuron.fValue = childNeuron.fActivation(fValue);
			}
		}

		auto& outputLayer = layers.back();
		auto sizeOutputLayer = outputLayer.first.size() + (outputLayer.second ? -1 : 0);

		std::vector<DOUBLE> output;
		output.resize(sizeOutputLayer);

		//присвоим выходные значения 
		auto itOutput = outputLayer.first.begin();
		for (DWORD i = 0; i < sizeOutputLayer; i++, itOutput++)
			output[i] = itOutput->fValue;

		return output;
	}

	std::vector<DOUBLE> CNeuro::calcError(
		_In_ std::vector<DOUBLE> input,
		_In_ BOOL bCorrect)
	{
		Lock lock(mutex);

		if (layers.size() <= 0)
			throw std::invalid_argument("layers <= 0");

		auto& outputLayer = layers.back();
		auto sizeOutputLayer = outputLayer.first.size() + (outputLayer.second ? -1 : 0);

		if (input.size() != sizeOutputLayer)
			throw std::invalid_argument("input.size() != output neuron");

		std::vector<DOUBLE> output;
		output.resize(sizeOutputLayer);

		//присвоим значения ошибки выходному слою
		auto itOutput = outputLayer.first.begin();
		for (DWORD i = 0; i < sizeOutputLayer; i++, itOutput++)
		{
			itOutput->fError = input[i] - itOutput->fValue;
			output[i] = itOutput->fError;
		}

		for (auto itParentLayer = (----layers.end());
			itParentLayer != layers.begin();
			itParentLayer--)
		{
			for (auto& parentNeuron : itParentLayer->first)
			{
				DOUBLE fError = 0.f;

				for (auto [pChildNeuron, pWeight] : parentNeuron.outputNeurons)
					fError += pChildNeuron->fError * pWeight->fWeight;

				parentNeuron.fError = fError;
			}
		}

		//корректируем веса
		if(bCorrect)
			correctWeight(&lock);

		return output;
	}

	VOID CNeuro::correctWeight(
		_In_opt_ Lock *pLock)
	{
		auto correct = [&]()
		{
			if (layers.size() <= 0)
				throw std::invalid_argument("layers <= 0");

			for (auto& weight : neuronWeights)
			{
				if (!weight.pOutputNeuron->fCorrercion)
					throw std::invalid_argument("fCorrercion = nullptr");

				weight.fWeight += 
					fKf * 
					weight.pOutputNeuron->fError *
					weight.pOutputNeuron->fCorrercion(weight.pOutputNeuron->fValue) * 
					weight.pInputNeuron->fValue;
			}
		};

		if (pLock)
			correct();
		else
		{
			Lock lock(mutex);
			correct();
		}
	}

	DOUBLE CNeuro::getError()
	{
		Lock lock(mutex);

		if (layers.size() <= 0)
			throw std::invalid_argument("layers <= 0");

		DOUBLE fError = 0.f;

		for (auto& neuron : layers.back().first)
			fError += std::pow(neuron.fError, 2);

		return fError;
	}

	DOUBLE CNeuro::getFullError()
	{
		Lock lock(mutex);

		if (layers.size() <= 0)
			throw std::invalid_argument("layers <= 0");

		DOUBLE fError = 0.f;

		for (auto& layer : layers)
		{
			for (auto& neuron : layer.first)
				fError += std::pow(neuron.fError, 2);
		}

		return fError;
	}
}
