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
#include "CoinFlips.h"
#include "FloatCompare.h"
#include "RandomBool.h"
#include <iostream>
#include <random>
#include <functional>
namespace CeriumEngine {

	static bool FairCoinToss() {
		{
			// "Fair" coin-tosses.
			// Effectively, a coin-toss must be confirmed
			// by a follow-up toss, or else the result is
			// discarded and we re-flip for a new result.
			//
			// Inspired by (and helpful code snippets from):
			// https://www.xarg.org/201	8/01/make-a-fair-coin-from-a-biased-coin/
			bool toss1 = false;
			bool toss2 = false;
			do {
				// Keep tossing two coins:
				toss1 = randomBool();
				toss2 = randomBool();
			} while (toss1 == toss2);
			// Until they both match, heads or tails.
			return toss1;
		}
	}

	static int CoinTossTakes(const bool result) {
		// How many tries does it take to come up
		// with a heads or tails coin?
		//
		// Inspired by (and helpful code snippets from):
		// https://www.xarg.org/201	8/01/make-a-fair-coin-from-a-biased-coin/
		bool toss1 = false;
		int flips = 0;
		do {
			// Keep flipping coins:
			toss1 = randomBool();
			flips = flips++;
		} while (toss1 == result);
		// Until we get the result we asked for, heads or tails.
		// Output how many times it took.
		return flips;
	}

	static void CoinTossFairness(const float startingBias, const float targetBias, const int flips, const bool endAtTarget, const bool linearyWeigh, const bool neutralBiasCounted,
		float& currentBias, float& biasChange, float& biasChangePercent, int& simSteps, int& trueTosses, int& falseTosses) {
		// "Fair" coin-tosses. Given a biased coin (one that slightly favors
		// flipping to one side or another) and a target bias (a separate
		// coin with a different tendency for flipping), flip both of them
		// and an extra "fair coin" from the fair-coin concept, which will
		// confirm which way our bias sways. For each flip, modify our bias
		// accordingly. At the end of a specified number of flips, we output:
		//
		// - Number of encountered true and false coins.
		// - Our ending bias. We may have optionally been told to end early
		// if we met the "preferred bias" that the second coin has.
		// - The change that occurred. (eg, start - ending)
		// - How close we were to our target bias.
		// - How many flips/simulations it took. Might be the amount specified
		// in the input.
		//
		//
		// Inspired by (and helpful code snippets from):
		// https://www.xarg.org/201	8/01/make-a-fair-coin-from-a-biased-coin/

		bool StartingToss = false;
		bool NeutralToss = false;
		bool PreferredToss = false;
		bool ConfirmedToss = false;
		falseTosses = 0;
		trueTosses = 0;
		currentBias = startingBias;
		float SimPercent = 0.0;
		float SimStepInfluence = 0.0;
		simSteps = 0;
		int Iterations = 1000;

		// Set iteration count based on our passed variable, as long as it's more than none
		// and less than 10,000. Otherwise our default value of 1,000 should hold.
		if (flips > 0 && flips <= 10000) {
			Iterations = flips;
		}

		SimStepInfluence = (100.0 / Iterations);
		for (int i = 0; i < Iterations; i++) {
			// Keep flipping coins:

			// This is the starting or "unfavorable RNG" we begin with.
			// This acts as our starting "seed".
			// It has a weight/preference for one side or the other.
			StartingToss = randomBoolWeighted(startingBias);

			// True neutral; our "fair coin". It matches for the other two.
			// For obvious reasons, this one is left true random. No bias.
			NeutralToss = randomBool();

			// The weight/preference of heads/tails outcomes we'd like to have.
			// Therefore, this coin is also weighted.
			PreferredToss = randomBoolWeighted(targetBias);

			// Before we get into the matching stuff, update our 'distance'
			// traveled along our algo:
			SimPercent = ((Iterations - i) / Iterations);

			if (NeutralToss == StartingToss == PreferredToss) {
				// Both bias coins and our "fair" coin matched.
				// Move towards neutral if in settings.
				if (neutralBiasCounted == true) {
					currentBias = std::lerp(currentBias, 0.5, 0.5);
				}
				ConfirmedToss = NeutralToss;
			}

			if (NeutralToss != StartingToss && NeutralToss != PreferredToss) {
				// No match with either bias coin.
				// Move towards neutral if in settings.
				if (neutralBiasCounted == true) {
					currentBias = std::lerp(currentBias, 0.5, 0.5);
				}
				ConfirmedToss = NeutralToss;
			}

			if (NeutralToss == StartingToss && NeutralToss != PreferredToss) {
				// Match with starting bias coin.
				if (linearyWeigh == true) {
					// We do a lerp() ..inside of a lerp() here. Specifically we're
					// changing one of the first lerp vars (which would be starting
					// influence ordinarily; see below. We're weakening by how
					// far along the simulation we are (or how many loops have gone
					// by, more specifically. This is to make starting influence weaker
					// the further from the start. It's controlled by an optional bool.
					currentBias = std::lerp(currentBias, (std::lerp(startingBias, currentBias, SimPercent)), 0.5);
				}
				else
				{
					// Don't do that fancy stuff; lerp off of the full starting bias, as usual.
					currentBias = std::lerp(currentBias, startingBias, SimPercent);
				}
				ConfirmedToss = StartingToss;
			}

			if (NeutralToss != StartingToss && NeutralToss == PreferredToss) {
				// Match with preferred bias coin.
				currentBias = std::lerp(currentBias, targetBias, 0.5);
				ConfirmedToss = PreferredToss;
			}

			// Count up what our result ended up being.
			if (ConfirmedToss == true) {
				trueTosses = trueTosses + 1;
			}
			else {
				falseTosses = falseTosses + 1;
			}
			// Simulation has advanced:
			simSteps = simSteps + 1;

			// Finally, if our bias matches the targeted bias
			// and the settings want us to stop there:
			if (endAtTarget == true) {
				if (approximatelyEqual(currentBias, targetBias, 0.001) == true) {
					// Break out of the coin flips, just return.
					break;
				}
			}
		}

		biasChange = 0.0;
		biasChangePercent = 0.0;

		biasChange = ((abs(startingBias)) - (abs(currentBias)));
		float c = std::max(startingBias, targetBias);
		float d = std::min(startingBias, targetBias);
		if (abs(c - d) != 0.0) {
			biasChangePercent = (biasChange / abs(c - d));
		}

	}

}