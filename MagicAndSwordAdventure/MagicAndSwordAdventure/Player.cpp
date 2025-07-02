#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kDashSpeed = 10.0f;
	constexpr float kJumpPower = 9.0f;
	constexpr float kJumpGravity = -0.4f;
	// �ړ����E
	constexpr float kBackLimit = 100.0f;
	constexpr float kFrontLimit = -340.0f;

	// �����蔻��͈̔�
	constexpr float kColRadius = 50.0f;
	// �ő�HP
	constexpr int kMaxHp = 50;
	// ����
	constexpr float kMoveDecRate = 0.80f;

	constexpr float playerHeadOffSet = 90;
	constexpr float playerFootOffSet = 20;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };

	const char* kIdleAnimName = "CharacterArmature|Idle";
	const char* kRunAnimName = "CharacterArmature|Run";
	bool isStartGravity = false;
	bool isMove = false;
	int moveCount = 0;
	int idleCount = 0;
}


Player::Player():
m_damageFrame(0),
m_hp(kMaxHp),
m_rotMtx(MGetIdent()),
m_isJump(false),
m_isDirRight(true),
m_isPrevButton(false),
m_isNowButton(false),
m_jumpCount(0),
m_isAttackDirRight(true)
{
	m_pos = { 0, 0, 0 };
	m_vec = { 0, 0, 0 };
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Animation> pAnimation)
{
	m_pos = VGet(0, 0, 0);
	m_pAnimation = pAnimation;
	m_modelHandle = MV1LoadModel(L"Data/model/Barbarian.mv1");
	MV1SetScale(m_modelHandle, VGet(50, 50, 50));
	MV1SetRotationXYZ(m_modelHandle, kRightDir);
	m_pAnimation->AttachAnim(m_modelHandle, 1);
}

void Player::End()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Update()
{
	if (m_vec.y > 0)
	{
		isStartGravity = true;
	}
	// B�{�^�����������Ƃ�
	m_isNowButton = Pad::isPress(PAD_INPUT_2);
	// B�{�^������������ςȂ��łȂ�
	if (m_isNowButton && !m_isPrevButton)
	{
		DoAttack();
	}
	if (attack.active)
	{
		attack.timer--;
		attack.comboDuration--;
		if (attack.timer <= 0)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 1, true, 0.5f);
			attack.active = false;
		}
	}

	m_isPrevButton = m_isNowButton;
	
	DoMove();
	DoJump();
	

	//RB�{�^�����������Ƃ�
	if (Pad::isTrigger(PAD_INPUT_6))
	{
		DoEvade();
	}
	if (evadeData.active)
	{
		evadeData.timer--;
		// ������Ԃ��I�������
		if (evadeData.timer <= 0)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 1, true, 0.5f);
			evadeData.active = false;
			evadeData.evadeCount = 0;
		}
	}

	if (isMove)
	{
		idleCount = 0;
		if (moveCount < 1)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 3, true, 0.5f);
		}
		moveCount++;
	}
	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;
	MV1SetPosition(m_modelHandle, m_pos);
	m_pos = VAdd(m_pos, m_vec);
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_vec.y = 0;
		m_isJump = false;
		m_jumpCount = 0;
	}

	//printfDx(L"%f\n", m_screenPos.x);
	m_pAnimation->UpdateAnim();
}

void Player::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	DrawCapsule3D(VGet(m_pos.x, m_pos.y + playerHeadOffSet, m_pos.z), VGet(m_pos.x, m_pos.y+ playerFootOffSet, m_pos.z), 30, 8, 0x00ff00, 0xffffff, false);
	if (attack.active && !m_vec.y > 0)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0xff0000, 0xffffff, false);
	}
#endif
}

VECTOR Player::GetScreenPos()
{
	// �X�N���[�����W�ɕϊ�
	m_screenPos = ConvWorldPosToScreenPos(m_pos);
	return m_screenPos;
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::OnDamage()
{
	m_hp -= 10;
	printfDx(L"hp:%d\n", m_hp);
}

void Player::DoAttack()
{
	attack.active = true;
	if (m_vec.y > 0)
	{
		m_pAnimation->ChangeAnim(m_modelHandle, 39, false, 0.7f);
	}
	else
	{
		m_pAnimation->ChangeAnim(m_modelHandle, 31, false, 0.5f);
	}
	attack.timer = 50.0f;
	attack.comboDuration = 20.0f;
	attack.count++;
	if (attack.count > 3)
	{
		attack.count = 1;
	}
	//printfDx(L"attack.count:%d\n", attack.count);
	if (m_isAttackDirRight)
	{
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x + attack.attackOffSetX;
		if (attack.count == 2 && !m_vec.y > 0)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 40, false, 0.7f);
		}
		if (attack.count == 3 && !m_vec.y > 0)
		{
			m_vec.x = +kMoveSpeed;
			attack.pos.x = m_pos.x + attack.attackOffSetX;
			m_pAnimation->ChangeAnim(m_modelHandle, 42, false, 1.0f);
		}
	}
	else 
	{
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x - attack.attackOffSetX;
		if (attack.count == 2 && !m_vec.y > 0)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 40, false, 0.7f);
		}
		if (attack.count == 3 && !m_vec.y > 0)
		{
			m_vec.x = -kMoveSpeed;
			attack.pos.x = m_pos.x - attack.attackOffSetX;
			m_pAnimation->ChangeAnim(m_modelHandle, 42, false, 1.0f);
		}
	}

	attack.pos.y = m_pos.y+attack.attackOffSetY;
	attack.pos.z = m_pos.z;
}

void Player::DoEvade()
{
	if (evadeData.evadeCount < 1)
	{
		evadeData.active = true;
		// ����A�j���[�V�����ɐ؂�ւ�
		m_pAnimation->ChangeAnim(m_modelHandle, 16, false, 0.5f);
		// ����񐔂𑝂₷
		evadeData.evadeCount++;
		evadeData.timer = 30.0f;
		if (m_isDirRight)
		{
			MV1SetRotationXYZ(m_modelHandle, kLeftDir); // ���f���͐i�s�����ɔw��������
			m_vec.x = kMoveSpeed * 3.0f;
			m_isAttackDirRight = false;
		}
		else
		{
			MV1SetRotationXYZ(m_modelHandle, kRightDir); // ���f���͐i�s�����ɔw��������
			m_vec.x = -kMoveSpeed * 3.0f;
			m_isAttackDirRight = true;
		}
		m_vec.y = kJumpPower * 0.50f;
	}
}

VECTOR Player::GetPlayerPosHead()
{
	m_playerPosHead = VGet(m_pos.x, m_pos.y + 90, m_pos.z);
	return m_playerPosFoot;
}

VECTOR Player::GetPlayerPosFoot()
{
	m_playerPosFoot = VGet(m_pos.x, m_pos.y + 20, m_pos.z);
	return m_playerPosFoot;
}

void Player::DoMove()
{
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}

	if (Pad::isPress(PAD_INPUT_RIGHT) && !evadeData.active)
	{
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_modelHandle, kRightDir);
		}
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = kMoveSpeed;
		// �_�b�V���{�^�����������ꍇ
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = kDashSpeed;
		}
		m_isAttackDirRight = true;
		m_isDirRight = true;
	}
	else if (Pad::isPress(PAD_INPUT_LEFT) && !evadeData.active)
	{
		if (!evadeData.active)
		{
			MV1SetRotationXYZ(m_modelHandle, kLeftDir);
		}
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = -kMoveSpeed;
		// �_�b�V���{�^�����������ꍇ
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x = -kDashSpeed;
		}
		m_isAttackDirRight = false;
		m_isDirRight = false;
	}
	else if (Pad::isPress(PAD_INPUT_UP))
	{
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = kMoveSpeed;
	}
	else if (Pad::isPress(PAD_INPUT_DOWN))
	{
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = -kMoveSpeed;
	}
	else
	{
		isMove = false;
		moveCount = 0;
		if (idleCount < 1)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, 1, true, 0.5f);
		}
		idleCount++;
	}
}

void Player::DoJump()
{
	// A�{�^�����������Ƃ��W�����v
	if (Pad::isTrigger(PAD_INPUT_1) && !m_isJump)
	{
		m_vec.y = kJumpPower;
		m_jumpCount++;
	}
	// �W�����v��2��܂�
	if (m_jumpCount > 1)
	{
		m_isJump = true;
	}
}


