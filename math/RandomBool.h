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
#include <iostream>
#include <random>
#include <functional>

static bool randomBool() {
	static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
	return gen();
}

static bool randomBoolWeighted(float ratio) {
	if (ratio > 1) {
		return true;
	}
	if (ratio < 0) {
		return false;
	}
	int intWeight = ceil(ratio * 1000);
	static auto gen = std::bind(std::uniform_int_distribution<>(0, 1000), std::default_random_engine());
	if (gen() >= intWeight) {
		return true;
	}
	return false;
}