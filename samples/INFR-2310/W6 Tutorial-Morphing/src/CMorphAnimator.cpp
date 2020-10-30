/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	CMorphAnimator::AnimData::AnimData()
	{
		frame0 = nullptr;
		frame1 = nullptr;
		frameTime = 1.0f;
		index = 0;
	}

	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_forwards = true;
	}

	void CMorphAnimator::Update(float deltaTime)
	{
		//if(!m_data->frame0 || !m_data->frame1)
		if (m_data->frames.empty())
			return;

		float t;
		int i;

		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;

			if (m_timer > m_data->frameTime) {
				m_data->index++;

				if (m_data->index >= m_data->frames.size())
					m_data->index = 0;
			}

			//This gives us the floating-point remainder
			//of dividing m_timer by m_data->frameTime.
			//(Allowing us to keep any extra time we'd gone over by.)
			m_timer = fmod(m_timer, m_data->frameTime);

			t = m_timer / m_data->frameTime;
		}
		else
			t = 0.0f;

		size_t indOne, indTwo;
		indTwo = m_data->index;
		indOne = (indTwo == 0) ? m_data->frames.size() - 1 : indTwo - 1;

		m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[indOne], *m_data->frames[indTwo], t);
	}

	//TODO (for the exercise): You'll need to modify this function to deal with an 
	//arbitrary number of frames (i.e., accept/keep a container of mesh pointers).
	void CMorphAnimator::SetFrames(const Mesh& frame0, const Mesh& frame1)
	{
		m_data->frame0 = &frame0;
		m_data->frame1 = &frame1;
	}

	void CMorphAnimator::SetFrames(const std::vector<std::unique_ptr<Mesh>> &frameList)
	{
		for (int i = 0; i < frameList.size(); i++)
		{
			m_data->frames.push_back(frameList[i].get());
		}
	}

	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}
}