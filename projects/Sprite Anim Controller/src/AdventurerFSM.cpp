#include "AdventurerFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou {
	const std::string AdventurerFSM::idleClip = "idle";
	const std::string AdventurerFSM::crouchClip = "crouch";
	const std::string AdventurerFSM::runClip = "run";
	const std::string AdventurerFSM::flipClip = "flip";
	const std::string AdventurerFSM::attackClip = "attack";

	AdventurerFSM::AdventurerFSM(Entity& entity) {
		m_entity = &entity;
		m_state = AniState::IDLE;

		SetState(m_state);
	}

	void AdventurerFSM::SetState(AdventurerFSM::AniState state) {
		m_state = state;

		auto& anime = m_entity->Get<CSpriteAnimator>();

		ClearTrigs();

		switch (m_state) {
			case AniState::IDLE:
				anime.PlayLoop(idleClip);
				break;

			case AniState::CROUCH:
				anime.PlayLoop(crouchClip);
				break;

			case AniState::RUN:
				anime.PlayLoop(runClip);
				break;

			case AniState::FLIP:
				anime.PlayOnce(flipClip);
				break;

			case AniState::ATTACK:
				anime.PlayOnce(attackClip);
				break;

			default:
				break;
		}
	}

	void AdventurerFSM::Update() {
		auto& anime = m_entity->Get<CSpriteAnimator>();

		switch (m_state) {
			case AniState::IDLE:
				if (GetVar("running"))
					SetState(AniState::RUN);
				else if (GetVar("crouching"))
					SetState(AniState::CROUCH);
				else if (GetTrig("flipping"))
					SetState(AniState::FLIP);
				else if (GetTrig("attacking"))
					SetState(AniState::ATTACK);
				break;

			case AniState::CROUCH:
				if (!GetVar("crouching"))
					SetState(AniState::IDLE);
				break;

			case AniState::RUN:
				if (!GetVar("running"))
					SetState(AniState::IDLE);
				else if (GetVar("crouching"))
					SetState(AniState::CROUCH);
				else if (GetTrig("flipping"))
					SetState(AniState::FLIP);
				else if (GetTrig("attacking"))
					SetState(AniState::ATTACK);
				break;

			default:
				if (anime.IsDone())
					SetState(AniState::IDLE);
				break;
		}
	}
}