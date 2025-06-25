#include "CharacterBase.h"

void CharacterBase::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	if (attack.active && !m_vec.y > 0)
	{
		DrawSphere3D(VGet(attack.x, attack.y, attack.z), attack.radius, 8, 0xff0000, 0xffffff, false);
	}
#endif
}

