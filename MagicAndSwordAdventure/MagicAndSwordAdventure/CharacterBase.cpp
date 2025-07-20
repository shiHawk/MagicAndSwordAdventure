#include "CharacterBase.h"

CharacterBase::CharacterBase():
	m_pos({0.0,0.0,0.0}),
	m_vec({ 0.0,0.0,0.0 }),
	m_modelHandle(-1),
	m_isDirRight(true),
	m_isDead(false),
	m_isDying(false)
{
}

