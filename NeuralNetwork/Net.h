#pragma once
#include "Neuron.h"
#include <vector>

using namespace std;


class Net
{
public:
	Net(const vector<unsigned> &topology);
	void FeedForward(const vector<double> &inputVals);
	void BackProp(const vector<double> &targetVals);
	void GetResult(vector<double> &resultVals) const;
	~Net();
	vector<Layer> GetNet() { return m_Layers; }
	int GetNBLayers();

	void InitializeWithFile();

private:
	vector<Layer> m_Layers; // m_layers[layersNum][neuronNum]
	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};

