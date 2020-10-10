#pragma once

#include <vector>
#include <string>

namespace nou {
	class FiniteStateMach {
		public:
			FiniteStateMach();
			virtual ~FiniteStateMach() = default;

			void ClearTrigs();

			// Getters
			bool GetVar(const std::string& key);
			bool GetTrig(const std::string& key);

			// Setters
			void SetVar(const std::string& key, bool value);
			void SetTrig(const std::string& key);

			virtual void Update() = 0;

		protected:
			typedef std::vector<std::string> Key;
			typedef std::vector<bool> Value;

			Key trigs;

			Key varKey;
			Value varVal;
	};
}