#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 5.0f;
	constexpr float kDashSpeed = 10.0f;
	constexpr float kJumpPower = 9.0f;
	constexpr float kJumpGravity = -0.4f;
	constexpr float kBackLimit = 240.0f;
	constexpr float kFrontLimit = -350.0f;
	constexpr float kLeftLimit = -2660.0f;
	// �����蔻��͈̔�
	constexpr float kColRadius = 40.0f;
	// �ő�HP
	constexpr int kMaxHp = 1000;
	// ����
	constexpr float kMoveDecRate = 0.80f;
	// �J�����ړ��p�̂������l
	constexpr float kMoveCameraThreshold = 60.0f;
	// �A���U���̎�t����
	constexpr float kMaxComboDuration = 120.0f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kModelScale = 50.0f; // ���f���̃X�P�[��
	constexpr VECTOR kPlayerInitPos = { -4800.0f, 0.0f, 0.0f }; // �v���C���[�̏����ʒu
	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 1;
	constexpr int kWalkAnimNo = 3;
	constexpr int kAttack1AnimNo = 31;
	constexpr int kAttack2AnimNo = 40;
	constexpr int kAttack3AnimNo = 42;
	constexpr int kDamageAnimNo = 24;
	constexpr int kDeathAnimNo = 26;
	constexpr int kJumpAttackAnimNo = 39;
	constexpr int kEvadeAnimNo = 16;
	// �A�j���[�V�����̑��x
	constexpr float kAnimSpeedFast = 0.5f; // �Z�߂̍Đ��̎�
	constexpr float kAnimSpeedMedium = 0.7f; // �����x�̃e���|
	constexpr float kAnimSpeedSlow = 1.0f; // ���߂̍Đ��̎�
	// ��������
	constexpr float kAttackDuration = 50.0f;
	constexpr float kEvadeDuration = 40.0f;
	// ��𑬓x�搔
	constexpr float kEvadeSpeedMultiplier = 3.0f;
	constexpr float kEvadeJumpMultiplier = 0.5f;
	// �e�U���̍U����
	constexpr int kFirstAttackPower = 20;
	constexpr int kSecondAttackPower = 40;
	constexpr int kThirdAttackPower = 60;

	constexpr float kWallOffset = 0.001f; // �ǂɓ����������̈ʒu�␳
	constexpr float kAttackHideY = -1000.0f; // ���S���̍U���̈ʒu

	bool isStartGravity = false;
	// �U��Ԃ邾���ňړ������Ɣ��肳���Ȃ����߂�
	bool isMove = false;
	// �A���ňړ��A�j���[�V�������Ă΂Ȃ��悤�ɂ���
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
m_isAttackDirRight(true),
m_screenPos({0.0f,0.0f,0.0f}),
m_isMovingFlag(false),
m_isNowDirRight(true),
m_isPrevDirRight(true),
m_prevPos({0,0,0}),
m_distanceAfterMoving(0.0f),
attack({ 30,{-500,0,0},false,0.0f,0,30.0f,60.0f,40.0f }),
m_isAttackingAnim(false)
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Animation> pAnimation)
{
	m_pos = kPlayerInitPos;
	m_vec = { 0, 0, 0 };
	m_pAnimation = pAnimation;
	m_modelHandle = MV1LoadModel("Data/model/Barbarian.mv1");
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, kRightDir);
	m_pAnimation->AttachAnim(m_modelHandle, 1);
	m_prevPos = m_pos;
	m_power = kFirstAttackPower;
	m_hp = kMaxHp;
	m_isDead = false;
	attack = { 30.0f, { -500,0,0 }, false, 0.0f, 0, 30.0f, 60.0f, 40.0f };
}

void Player::End()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Update()
{
	if (m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		UpdateCombo();
		if (m_vec.y > 0)
		{
			isStartGravity = true;
		}
		// B�{�^�����������Ƃ�
		m_isNowButton = Pad::isPress(PAD_INPUT_2);
		// B�{�^������������ςȂ��łȂ�
		if (m_isNowButton && !m_isPrevButton && !m_isJump && !isMove)
		{
			DoAttack();
		}
		if (attack.active)
		{
			attack.timer--;
			if (attack.timer <= 0 || m_pAnimation->GetIsAnimEnd())
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
				attack.pos = { 0.0f,-kAttackHideY,0.0f };
				attack.active = false;
			}
		}
		m_isPrevButton = m_isNowButton; // �{�^���̍X�V
		HandleJump();
		DoMove();
		HandleEvade();
		if (isMove)
		{
			idleCount = 0;
			if (moveCount < 1)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kWalkAnimNo, true, kAnimSpeedFast);
			}
			moveCount++;
		}

		if (m_pos.y < 0.0f)
		{
			m_pos.y = 0.0f;
			m_vec.y = 0;
			m_isJump = false;
			m_jumpCount = 0;
		}
		m_pAnimation->UpdateAnim();
		// �U�����Ȃ�A�w��J�E���g�ȍ~�ōU������ON
		if (m_isAttackingAnim  &&!attack.active && m_pAnimation->GetPlayTime() >= 11.0f)  // �U���A�j���[�V���������܂����肪�o�ĂȂ�
		{
			attack.active = true;
		}
		if (m_isAttackingAnim && m_pAnimation->GetIsAnimEnd())
		{
			attack.active = false;
			m_isAttackingAnim = false;
		}
	}
	else
	{
		End();
		return;
	}
}

void Player::Draw() const
{
	MV1DrawModel(m_modelHandle);
#if _DEBUG
	if (attack.active && !m_vec.y > 0)
	{
		DrawSphere3D(attack.pos, attack.radius, 8, 0xff0000, 0xffffff, false);
	}
#endif
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::OnDamage(int enemyPower)
{
	if (!evadeData.active)
	{
		m_hp -= enemyPower;
	}
	if (m_hp <= 0 )
	{
		m_hp = 0;
		m_pAnimation->ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedFast);
		m_isDying = true;
	}
	else
	{
		m_pAnimation->ChangeAnim(m_modelHandle, kDamageAnimNo, false, kAnimSpeedFast);
	}
	
	//printfDx(L"hp:%d m_isDying:%d\n", m_hp, m_isDying);
}

void Player::OnDeath()
{
	m_pAnimation->UpdateAnim();
	MV1SetPosition(m_modelHandle, m_pos);
	if (m_pAnimation->GetIsAnimEnd())
	{
		m_isDead = true;
		m_isDying = false;
	}
}

void Player::DoAttack()
{
	if (m_isAttackingAnim) return; // �U�����Ȃ玟�̍U�����󂯕t���Ȃ�
	m_isAttackingAnim = true;
	attack.count++;
	// �O��U��������܂����ڂɖ߂�
	if (attack.count > 3)
	{
		attack.count = 1;
	}
	attack.comboDuration = kMaxComboDuration;
	//attack.active = true;
	if (m_vec.y > 0)
	{
		m_pAnimation->ChangeAnim(m_modelHandle, kJumpAttackAnimNo, true, kAnimSpeedMedium);// �W�����v�U��
	}
	else
	{
		switch (attack.count) {
		case 1:
			m_power = kFirstAttackPower;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack1AnimNo, false, kAnimSpeedFast);
			break;
		case 2:
			m_power = kSecondAttackPower;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, kAnimSpeedMedium);
			break;
		case 3:
			m_power = kThirdAttackPower;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, kAnimSpeedSlow);
			break;
		}
	}
	//printfDx(L"attack.count:%d\n", attack.count);
	if (m_isAttackDirRight)
	{
		// �U���̎��ɏ����O�i����
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x + attack.attackOffSetX;
	}
	else 
	{
		// �U���̎��ɏ����O�i����
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x - attack.attackOffSetX;
	}
	attack.timer = kAttackDuration;
	attack.pos.y = m_pos.y+attack.attackOffSetY;
	attack.pos.z = m_pos.z;
}

void Player::DoEvade()
{
	if (evadeData.evadeCount < 1)
	{
		evadeData.active = true;
		// ����A�j���[�V�����ɐ؂�ւ�
		m_pAnimation->ChangeAnim(m_modelHandle, kEvadeAnimNo, false, kAnimSpeedFast);
		// ����񐔂𑝂₷
		evadeData.evadeCount++;
		evadeData.timer = kEvadeDuration;
		if (m_isDirRight)
		{
			MV1SetRotationXYZ(m_modelHandle, kLeftDir); // ���f���͐i�s�����ɔw��������
			m_vec.x = kMoveSpeed * kEvadeSpeedMultiplier;
			m_isAttackDirRight = false;
		}
		else
		{
			MV1SetRotationXYZ(m_modelHandle, kRightDir); // ���f���͐i�s�����ɔw��������
			m_vec.x = -kMoveSpeed * kEvadeSpeedMultiplier;
			m_isAttackDirRight = true;
		}
		m_vec.y = kJumpPower * kEvadeJumpMultiplier;
	}
	MV1SetPosition(m_modelHandle, m_pos);
	m_pos = VAdd(m_pos, m_vec);
}

bool Player::IsMoving()
{
	// �U��������狗�������Z�b�g
	if (m_isPrevDirRight != m_isNowDirRight)
	{
		m_prevPos = m_pos;
		m_isMovingFlag = false;
	}
	// �ړ��O�ƈړ���̋����̑傫���𑪂�
	m_distanceAfterMoving = VSize(VSub(m_pos, m_prevPos));
	m_isPrevDirRight = m_isNowDirRight;
	if (!m_isMovingFlag && m_distanceAfterMoving > kMoveCameraThreshold)
	{
		m_isMovingFlag = true;
	}
	return m_isMovingFlag;
}

bool Player::IsPinch()
{
	if (m_hp <= kMaxHp / 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Player::GetMaxHp()
{
	return kMaxHp;
}

void Player::AddPos(VECTOR offset)
{
	m_pos = VAdd(m_pos, offset);
}


void Player::DoMove()
{
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}
	if (!m_isAttackingAnim)
	{
		HandleInput();
	}
	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;
	MV1SetPosition(m_modelHandle, m_pos);
	if (m_pos.z >= kBackLimit)
	{
		m_vec.z = 0.0f;
		m_pos.z = kBackLimit - kWallOffset;
	}
	if (m_pos.z <= kFrontLimit)
	{
		m_vec.z = 0.0f;
		m_pos.z = kFrontLimit + kWallOffset;
	}
	if (m_pos.x < kLeftLimit)
	{
		m_vec.x = 0.0f;
		m_pos.x = kLeftLimit + kWallOffset;
	}
	m_pos = VAdd(m_pos, m_vec);
}

void Player::HandleJump()
{
	if (Pad::isTrigger(PAD_INPUT_1) && !attack.active)// A�{�^�����������Ƃ��W�����v
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

void Player::HandleEvade()
{
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
			m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
			evadeData.active = false;
			evadeData.evadeCount = 0;
		}
	}
}

void Player::UpdateCombo()
{
	if (attack.comboDuration > 0.0f)
	{
		attack.comboDuration--;
		if (attack.comboDuration <= 0.0f)
		{
			attack.count = 0;
		}
	}
}

void Player::HandleInput()
{
	if (Pad::isPress(PAD_INPUT_RIGHT) && !evadeData.active)
	{
		m_isNowDirRight = true;
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
		m_isNowDirRight = false;
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
			m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedFast);
		}
		idleCount++;
	}
}
