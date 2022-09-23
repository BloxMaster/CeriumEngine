/*
   Copyright 2022 T.H.J. aka Blox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

namespace CeriumEngine {

	static bool FairCoinToss();

	static int CoinTossTakes(const bool result);

	static void CoinTossFairness(const float startingBias, const float targetBias, const int flips, const bool endAtTarget, const bool linearyWeigh, const bool neutralBiasCounted,
		float& currentBias, float& biasChange, float& biasChangePercent, int& simSteps, int& trueTosses, int& falseTosses);
}
