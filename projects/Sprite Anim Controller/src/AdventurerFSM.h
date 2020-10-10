#pragma once

#include "FSM/FiniteStateMach.h"
#include "NOU/Entity.h"

namespace nou {
	class AdventurerFSM : public FiniteStateMach {
	public:
		static const std::string idleClip;
		static const std::string crouchClip;
		static const std::string runClip;
		static const std::string flipClip;
		static const std::string attackClip;

		enum class AniState {
			IDLE = 0,
			CROUCH,
			RUN,
			FLIP,
			ATTACK
		};

		AdventurerFSM(Entity& entity);
		~AdventurerFSM() = default;

		void SetState(AniState state);

		void Update();

	private:
		Entity* m_entity;
		AniState m_state;
	};
}