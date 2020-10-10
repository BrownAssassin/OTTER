#include "FiniteStateMach.h"

namespace nou {
	FiniteStateMach::FiniteStateMach() {}

	void FiniteStateMach::ClearTrigs() {
		trigs.erase(trigs.begin(), trigs.end());
	}

	bool FiniteStateMach::GetVar(const std::string& key) {
		Key::iterator iter = std::find(varKey.begin(), varKey.end(), key);

		if (iter == varKey.end())
			return false;

		return varVal[iter - varKey.begin()];
	}

	bool FiniteStateMach::GetTrig(const std::string& key) {
		Key::iterator iter = std::find(trigs.begin(), trigs.end(), key);

		if (iter == trigs.end())
			return false;

		return true;
	}

	void FiniteStateMach::SetVar(const std::string& key, bool value) {
		Key::iterator iter = std::find(varKey.begin(), varKey.end(), key);

		if (iter != varKey.end()) {
			varVal[iter - varKey.begin()] = value;
		}
		else {
			varKey.push_back(key);
			varVal.push_back(value);
		}
	}

	void FiniteStateMach::SetTrig(const std::string& key) {
		trigs.push_back(key);
	}	
}